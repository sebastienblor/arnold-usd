//-
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+


#include "ArnoldViewOverride.h"
#include "session/ArnoldRenderViewSession.h"
#include "session/SessionManager.h"
#include "utils/ConstantStrings.h"
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MTextureManager.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include <maya/MSceneMessage.h>
#include <maya/MConditionMessage.h>
#include <maya/MQtUtil.h>
#include <QtWidgets/QWidget>

#include "translators/DagTranslator.h"

#include <sstream>

static MFloatPoint s_ViewRectangle = MFloatPoint(0.0f, 0.0f, 0.0f, 0.0f);
static MString s_activeViewport(""); // store the name of the last active viewport

static unsigned int s_width = 0;
static unsigned int s_height = 0;


// For override creation we return a UI name so that it shows up in as a
// renderer in the 3d viewport menus.
// 
ArnoldViewOverride::ArnoldViewOverride(const MString & name)
    : MRenderOverride(name)
    , mUIName("Arnold")
    , mTexture(NULL)
    , mRendererChangeCB(0)
    , mRenderOverrideChangeCB(0)
    , mCurrentOperation(-1)
    , mDebugEnabled(false)
{
    // register a file open and file new callback
    mFileNewCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeNew, sPreFileOpen, this);
    mFileOpenCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeOpen, sPreFileOpen, this);
    mPlablastCB = MConditionMessage::addConditionCallback( "playblasting", sPlayblasting, this);

    mHUDRender = new ArnoldViewHUDRender( "viewOverrideArnold_HUD" );	

    // clean up old scnes that have the legacy worksapce control still
    int ws_exists;
    MGlobal::executeCommand("workspaceControl -q -ex \"ArnoldViewportRendererOptions\"", ws_exists);
    if (ws_exists)
        MGlobal::executeCommand("workspaceControl -edit -cl \"ArnoldViewportRendererOptions\"");  

}

// On destruction all operations are deleted.
//
ArnoldViewOverride::~ArnoldViewOverride()
{
    if (mTexture)
    {
        MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
        MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
        textureManager->releaseTexture(mTexture);
    }

	cleanup();

    // Clean up callbacks
    //
    if (mRendererChangeCB)
    {
        MMessage::removeCallback(mRendererChangeCB);
    }

    for (std::map<std::string, MCallbackId>::iterator it = callbackIdMap.begin(); it != callbackIdMap.end(); ++it)
    {
        MMessage::removeCallback(it->second);
    }

    MSceneMessage::removeCallback(mFileOpenCallbackID);
    MSceneMessage::removeCallback(mFileNewCallbackID);
    MMessage::removeCallback(mPlablastCB);
}

// Drawing uses all internal code so will support all draw APIs
//
MHWRender::DrawAPI ArnoldViewOverride::supportedDrawAPIs() const
{
    return MHWRender::kAllDevices;
}


// Basic iterator methods
bool ArnoldViewOverride::startOperationIterator()
{
    mCurrentOperation = 0;
    return true;
}

MHWRender::MRenderOperation * ArnoldViewOverride::renderOperation()
{
    if (mCurrentOperation >= 0 && mCurrentOperation < 6)
    {
        if (mOperations[mCurrentOperation])
        {
            if (mDebugEnabled)
            {
                printf("\t%s : Queue render operation[%d] = (%s)\n",
                        mName.asChar(),
                        mCurrentOperation,
                        mOperations[mCurrentOperation]->name().asChar());
            }
            return mOperations[mCurrentOperation];
        }
    }
    return NULL;
}

bool ArnoldViewOverride::nextRenderOperation()
{
    mCurrentOperation++;
    if (mCurrentOperation < 6)
    {
        return true;
    }
    return false;
}

MStatus ArnoldViewOverride::setup(const MString & destination)
{    
    MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer)
        return MStatus::kFailure;

    // Track changes to the renderer and override for this viewport
    if (!mRendererChangeCB)
    {
        mRendererChangeCB = MUiMessage::add3dViewRendererChangedCallback(destination, sRendererChangeFunc, NULL);
    }

    // If this is the first time the override was used for this panel we need to do a couple things.
    // 1) Switch off any existing render overrides.  If one existed the current rendering will be stopped.
    //    and a new render will be started below.
    // 2) Register an override changed callback to catch when the override for this panel is turned off.
    //    The callback is used to stop any active rendering when the override is turned off.
    if (callbackIdMap.find(destination.asChar()) == callbackIdMap.end())
    {
        // The below call will close the other arnold viewports, so it might destroy the previous AVP session. 
        stopExistingOverrides(destination);

        MCallbackId callbackID = MUiMessage::add3dViewRenderOverrideChangedCallback(destination, sRenderOverrideChangeFunc, this);
        callbackIdMap[destination.asChar()] = callbackID;
    }
    // 3) Initialize some default state for this panel
    RegionRenderState state;
    if (mRegionRenderStateMap.find(destination.asChar()) == mRegionRenderStateMap.end())
    {
        state.enabled = false;
        state.useRegion = false;
        state.initialized = false;
        state.viewRectangle = MFloatPoint(0.0f, 0.0f, 0.0f, 0.0f);
        mRegionRenderStateMap[destination.asChar()] = state;
    }
    else
    {
        state = mRegionRenderStateMap[destination.asChar()];
    }

    CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
    bool firstRender = (session == nullptr);

    if (session == nullptr)
    {
        session = new CArnoldRenderViewSession(true);
        CSessionManager::AddActiveSession(CArnoldRenderViewSession::GetViewportSessionId(), session);
    }
    if (session->IsExportingMotion())
        return MS::kFailure;
    CSessionOptions &sessionOptions = session->GetOptions();

    // if there's no active scene, or if we switched to a new viewport, then this is considered as a first render
    if (destination != s_activeViewport)
        firstRender = true;
    
    MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();

    // Create a new set of operations as required
    if (mOperations.length() == 0)
    {
        // create and setup the clear operation
        MHWRender::MClearOperation* clearOp = new MHWRender::MClearOperation(MHWRender::MRenderOperation::kStandardBackgroundName);
        clearOp->setOverridesColors(false);

        // create and setup the scene operation
        MHWRender::MSceneRender* sceneOp = new MHWRender::MSceneRender(MHWRender::MRenderOperation::kStandardSceneName);
        sceneOp->clearOperation().setMask(MHWRender::MClearOperation::kClearNone);

        UIObjectDraw* uiOp = new UIObjectDraw();
        uiOp->clearOperation().setMask(MHWRender::MClearOperation::kClearNone);

        mOperations.append(clearOp);
        mOperations.append(sceneOp);
        mOperations.append(new TextureBlit("viewOverrideArnold_Blit"));
        mOperations.append(uiOp);
        mOperations.append(mHUDRender);
        mOperations.append(new MHWRender::MPresentTarget("viewOverrideArnold_Present"));
    }

    int width = 0, height = 0;
    MDagPath camera;

    M3dView mView;
    if (destination.length() &&
        (M3dView::getM3dViewFromModelPanel(destination, mView) == MStatus::kSuccess))
    {
        MStatus status;
        width = mView.portWidth(&status);
        if (status != MS::kSuccess || (width < 2))
            return MS::kFailure;
        height = mView.portHeight(&status);
        if (status != MS::kSuccess || (height < 2))
            return MS::kFailure;

        mView.getCamera(camera);
    }
    std::string newCamName;

    if (firstRender)
    {
        session->SetExportCamera(camera, false);

        session->SetStatus("");
        mHUDRender->setStatus("");

        if (mTexture)
        {
            // clear the texture so that it's generated at next display
            MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
            MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
            textureManager->releaseTexture(mTexture);
            mTexture = NULL;
        }
        CDagTranslator *camTranslator = session->ExportDagPath(camera, true);
        AtNode *camNode = (camTranslator) ? camTranslator->GetArnoldNode() : NULL;
        if (camNode)
        {
            camTranslator->RequestUpdate(); // ensure the camera is re-exported so that it considers the viewport resolution for FOV
            // We need to use the partial name as this is the one considered by ARV's camera menu
            newCamName = camera.partialPathName().asChar(); 
            // Need to give the list of cameras + the current camera to ARV's options window #4370
            
            session->SetRenderViewOption(MString("Cameras"), MString(newCamName.c_str()));
            session->SetRenderViewOption(MString("Camera"), MString(newCamName.c_str()));
        }
        MGlobal::executePythonCommandOnIdle("import mtoa.viewport;mtoa.viewport.run_override(\""+destination+"\")"); 
    }
    s_activeViewport = destination; // set this viewport as the "active" one

    if (!state.initialized)
    {
        sessionOptions.SetResolution(width, height);
        // ensure the options are re-exported
        if (state.useRegion)
        {
            sessionOptions.SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
                AiClamp(int((1.f - state.viewRectangle.w) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top     
        }
        else
        {
            sessionOptions.ClearRegion();
        }
    }

    bool needsRefresh = false;
    
    if (!firstRender)
	{
        // A render had already started, we want to check if the window size has changed

        int previousWidth = width;
        int previousHeight = height;
        sessionOptions.GetResolution(previousWidth, previousHeight);

        if (previousWidth != width || previousHeight != height)
		{
            AtBBox2 bounds;
            CRenderViewMtoA &renderView = session->GetRenderView();
			renderView.HasRenderResults(bounds);
			renderView.PostDisplay();
			renderView.InterruptRender(true);
        
			sessionOptions.SetResolution(width, height);
			// ensure the options are re-exported
		
			if (sessionOptions.UseRenderRegion())
			{
                // eventually adapt the arnold crop region
				sessionOptions.SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - state.viewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top

			}
            
            COptionsTranslator *optionsTranslator = session->GetOptionsTranslator();
            if (optionsTranslator)
                optionsTranslator->RequestUpdate();

            CDagTranslator *camTranslator = session->ExportDagPath(camera, true);
            if (camTranslator)
            {
                camTranslator->RequestUpdate(); // camera needs to re-export to take into account resolution
            }            

            if (mTexture)
            {
                // clear the texture so that it's generated at next display
                MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
                MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
                textureManager->releaseTexture(mTexture);
                mTexture = NULL;
            }
            needsRefresh = true;
		}
        if (!(sessionOptions.GetCamera() == camera))
        {
            // the camera has changed
            session->GetRenderView().InterruptRender(true);
            CDagTranslator *camTranslator = session->ExportDagPath(camera, true);
            // SetExportCamera mus be called AFTER CMayaScene::Export
            session->SetExportCamera(camera);
            AtNode *camNode = (camTranslator) ? camTranslator->GetArnoldNode() : NULL;
            if (camNode)
            {
                // We need to use the partial name as this is the one considered by ARV's camera menu
                newCamName =  camera.partialPathName().asChar();
                // Need to give the list of cameras + the current camera to ARV's options window #4370
                session->SetRenderViewOption(MString("Cameras"), MString(newCamName.c_str()));
                session->SetRenderViewOption(MString("Camera"), MString(newCamName.c_str()));
            }
            needsRefresh = true;
        }
        
	}
    bool restoreIPR = false;
    if (!state.initialized)
    {
        state.initialized = true;
        mRegionRenderStateMap[destination.asChar()] = state;

        MGlobal::executeCommandOnIdle("aiViewRegionCmd -create;");
        MGlobal::executeCommandOnIdle("arnoldViewportRegionToolContext;");
        MGlobal::executeCommand("arnoldViewport;");
        // ensure the scene is fully rendered at next update
        session->GetRenderView().RequestFullSceneUpdate();

        if (!state.enabled)
            session->SetRenderViewOption(MString("Run IPR"), "0");    
        else
        {
            MString isIPRRunning = session->GetRenderViewOption(MString("Run IPR"));
            if (isIPRRunning == MString("0"))
                restoreIPR = true;
        } 
        // MString useRegionStr = (state.useRegion) ? MString("1"): MString("0");
        // FIXME this is causing a render restart and we don't want this now
        
        // session->SetRenderViewOption(MString("Crop Region"),useRegionStr);
    }

    MString arvCrop = (session) ? session->GetRenderViewOption(MString("Crop Region")) : MString();
    bool regionCropped = (arvCrop == MString("1"));
    
    if (regionCropped != sessionOptions.UseRenderRegion())
	{
        // ARV settings are different from the ones in the render options. ARV wins !
		if (regionCropped)
		{
            sessionOptions.SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - state.viewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top

		} else
		{
			sessionOptions.ClearRegion();
		}
	}
    int w, h, minx, miny, maxx, maxy;

    if (sessionOptions.GetRegion(minx, miny, maxx, maxy) && sessionOptions.GetResolution(w, h))
    {
        state.viewRectangle.x = (float)minx / (float)w;
        state.viewRectangle.y = 1.f - ((float)maxy / (float)h);
        state.viewRectangle.z = (float)(maxx) / (float)w;
        state.viewRectangle.w = 1.f - ((float)miny / (float)h);

        s_ViewRectangle = state.viewRectangle;
    }

    MString arvRunIpr = (session) ? session->GetRenderViewOption(MString("Run IPR")) : MString();
    state.enabled = (restoreIPR) ? true : arvRunIpr != MString("0");
    mRegionRenderStateMap[destination.asChar()] = state;

    // if the session is not rendering, but the state is enabled
    // end the current empty scene and start a new rendering
    /* FIXME, do we still need this ?
    if (!session->IsRendering() && state.enabled)
    {
        session->InterruptRender(true); // we shouldn't need this based on the condition above
        if (AiUniverseIsActive())
            AiEnd();

        needsRefresh = true;
    }*/

    // now get the current bits
    // and create a texture from them.
    AtBBox2 bounds;
    // Note that we're not using the "bounds" region here, about which region
    // of the buffer needs to be updated
    bool results = session->GetRenderView().HasRenderResults(bounds);

    if (needsRefresh || restoreIPR)
    {
        // disable the arnold operation until the next frame
        mOperations[2]->setEnabled(false);

        // now restart the render and early out. We'll be called here again in the next refresh.
        if (!newCamName.empty())
        {
            session->SetRenderViewOption(MString("Cameras"), MString(newCamName.c_str()));
            session->SetRenderViewOption(MString("Camera"), MString(newCamName.c_str()));
        }

        session->SetRenderViewOption(MString("Refresh Render"), MString("1"));

        if (restoreIPR)
            session->SetRenderViewOption(MString("Run IPR"), "1");
        
        return MS::kSuccess;
    }


    // disable the arnold operation if paused
    // mOperations[2]->setEnabled(state.enabled);
    mOperations[2]->setEnabled(true);
    // if (!state.enabled)
    //     return MS::kSuccess;

    unsigned int buffer_width = 0, buffer_height = 0;
    const AtRGBA *buffer = session->GetRenderView().GetDisplayedBuffer(&buffer_width, &buffer_height);
    if (buffer)
    {
        if (mTexture != NULL && (buffer_width != s_width || buffer_height != s_height))
        {
            textureManager->releaseTexture(mTexture);
            mTexture = NULL;
        }
        if (mTexture == NULL)
        {
            MHWRender::MTextureDescription desc;
            desc.setToDefault2DTexture();
            desc.fWidth = buffer_width;
            desc.fHeight = buffer_height;
            s_width = buffer_width;
            s_height = buffer_height;
            desc.fFormat = MHWRender::kR32G32B32A32_FLOAT;
            results = true;
            mTexture = textureManager->acquireTexture("arnoldResults", desc, NULL, false);
        }         
        if (results)
            mTexture->update(buffer, false);
        

        int blitIndex = mOperations.indexOf("viewOverrideArnold_Blit");
        if (blitIndex >= 0)
        {
            TextureBlit* textureBlit = (TextureBlit*)mOperations[blitIndex];
            // Set viewport rectangle
            textureBlit->setColorTexture(mTexture);
        }
    }

    // update the details in the HUD
    if (mHUDRender)
    {

        QWidget * view = MQtUtil::findControl(destination);
        float scaleFactor = MQtUtil::dpiScale(100.0f)/100.0f;

        mHUDRender->setPixelRatio(view->devicePixelRatioF()*scaleFactor);

        int hudRows = 0;
        MStringArray huds;
        MGlobal::executeCommand("headsUpDisplay -lh;", huds);
        for (uint h=0;h<huds.length();h++)
        {
            MString hudcmd = "headsUpDisplay -q -visible ";
            hudcmd += huds[h];
            int hud_visible ;
            MGlobal::executeCommand(hudcmd, hud_visible, false, false);
            hudcmd = "headsUpDisplay -q -section ";
            hudcmd += huds[h];
            int section;
            MGlobal::executeCommand(hudcmd, section, false, false);
            if (hud_visible && section == 0)
            {
                hudRows++;
            }
        }
        mHUDRender->setHorizontalOffset(hudRows);
        // get the current render time from the current render
        size_t rtime = session->GetRenderView().GetRenderTime();
        std::string display_status(session->GetRenderView().GetDisplayedStatus());
        if (rtime > 0 && display_status.length())
        {

            bool isFinalPass = false;
            float progress =  session->GetRenderView().GetProgress(isFinalPass);
            mHUDRender->setProgress(progress);

            std::string render_status("Rendering");
            mHUDRender->setRenderStatus(ltrim(render_status).c_str());

            std::stringstream statusStr;
            const char *debugShading = session->GetRenderViewOption(MString("Debug Shading")).asChar();
            if (debugShading != NULL && std::strlen(debugShading) && std::strcmp(debugShading,"Disabled"))
            {
                char *tempDebugShading = strdup(debugShading);
                // adjust copy to uppercase
                unsigned char *tptr = (unsigned char *)tempDebugShading;
                while(*tptr) {
                    *tptr = toupper(*tptr);
                    tptr++;
                }
                statusStr << "[" << tempDebugShading << "] ";
                free(tempDebugShading);
            }

            static size_t s_sixty(60);
            size_t secondsCount = rtime / (size_t)1000000;
            size_t minutesCount = secondsCount / s_sixty;
            secondsCount -= minutesCount * s_sixty;
            size_t hoursCount = minutesCount / s_sixty;
            minutesCount -= hoursCount * s_sixty;

            if (hoursCount <= 0) statusStr << "00:";
            else if (hoursCount < 10) statusStr << "0" << (int)hoursCount << ":";
            else statusStr << (int)hoursCount << ":";

            if (minutesCount <= 0) statusStr << "00:";
            else if (minutesCount < 10) statusStr << "0" << (int)minutesCount << ":";
            else statusStr << (int)minutesCount << ":";

            if (secondsCount <= 0) statusStr << "00";
            else if (secondsCount < 10) statusStr << "0" << (int)secondsCount;
            else statusStr << (int)secondsCount;
            
            statusStr << " ";
            statusStr << ltrim(display_status);

            mHUDRender->setStatus(statusStr.str().c_str());
        }
    }

    if (previousDestination != destination)
    {
        // update the viewport panel controls to reflect the current state
        MGlobal::executePythonCommand("import mtoa.viewport;mtoa.viewport.update_controls(\""+destination+"\")");
        previousDestination = destination;
    }

    session->GetRenderView().PostDisplay();

	mCurrentOperation = -1;

    return MStatus::kSuccess;
}

// On cleanup we just return for returning the list of operations for
// the next render
//
MStatus ArnoldViewOverride::cleanup()
{
	mCurrentOperation = -1;
    return MStatus::kSuccess;
}

// Callback for tracking renderer changes
void ArnoldViewOverride::sRendererChangeFunc(
    const MString& panelName,
    const MString& oldRenderer,
    const MString& newRenderer,
    void* clientData)
{
    if (newRenderer == "arnoldViewOverride")
    {
        MGlobal::executePythonCommand("import mtoa.viewport;mtoa.viewport.run_override(\""+panelName+"\")");
    }
}

// Callback for tracking render override changes
void ArnoldViewOverride::sRenderOverrideChangeFunc(
    const MString& panelName,
    const MString& oldOverride,
    const MString& newOverride,
    void* clientData)
{
           
    if (oldOverride == "arnoldViewOverride")
    {
        // Kill everything !!
        MGlobal::executeCommand("aiViewRegionCmd -delete;");
        s_activeViewport = MString("");
        
        // Close the options window and end the scene.
        CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
        if (session)
        {
            // session->GetRenderView().CloseOptionsWindow(); // could it be done at deletion ?
            CSessionManager::DeleteActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
        }
    
        RegionRenderState state = static_cast<ArnoldViewOverride*>(clientData)->mRegionRenderStateMap[panelName.asChar()];
        state.initialized = false;
        static_cast<ArnoldViewOverride*>(clientData)->mRegionRenderStateMap[panelName.asChar()] = state;
    }
    if (newOverride == "arnoldViewOverride")
    {
        // If this panel is switched back to an arnold render override
        // then stop any existing render overrides.
        stopExistingOverrides(panelName);
    }
}

void ArnoldViewOverride::sPlayblasting(bool state, void* clientData)
{
    CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
    if (session)
        session->SetPlayblasting(state);
}
void ArnoldViewOverride::sPreFileOpen(void* clientData)
{
    MGlobal::executeCommand("aiViewRegionCmd -delete;");
    
    CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
    if (session)
    {
        session->GetRenderView().CloseOptionsWindow();
        CSessionManager::DeleteActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
    }

    static_cast<ArnoldViewOverride*>(clientData)->mRegionRenderStateMap.clear();
    stopExistingOverrides("");
    s_activeViewport = MString("");
    
    //MGlobal::executeCommand("workspaceControl -edit -cl \"ArnoldViewportRendererOptions\"");  
}

void ArnoldViewOverride::stopExistingOverrides(const MString & destination)
{
    CArnoldRenderViewSession::CloseOtherViews(destination);
}

//
// Simple quad blit of a texture
//
TextureBlit::TextureBlit(const MString &name)
    : MQuadRender(name)
    , mShaderInstance(NULL)
    , mColorTextureChanged(false)
{
    mColorTexture.texture = NULL;
    mBlendState = NULL;
}

TextureBlit::~TextureBlit()
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (!renderer)
        return;

    // Release any shader used
    if (mShaderInstance)
    {
        const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
        if (shaderMgr)
        {
            shaderMgr->releaseShader(mShaderInstance);
        }
        mShaderInstance = NULL;
    }

    if (mBlendState)
        MHWRender::MStateManager::releaseBlendState(mBlendState);

    mColorTexture.texture = NULL;
}

const MHWRender::MBlendState* TextureBlit::blendStateOverride()
{
    // for now we disable the blending as it's not a good default. We should have an options somewhere for that
    return NULL;

    if (!mBlendState) {
        MHWRender::MBlendStateDesc desc;

        desc.targetBlends[0].blendEnable = true;
        desc.targetBlends[0].sourceBlend = MHWRender::MBlendState::kSourceAlpha;
        desc.targetBlends[0].destinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
        desc.targetBlends[0].blendOperation = MHWRender::MBlendState::kAdd;
        desc.targetBlends[0].alphaSourceBlend = MHWRender::MBlendState::kOne;
        desc.targetBlends[0].alphaDestinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
        desc.targetBlends[0].alphaBlendOperation = MHWRender::MBlendState::kAdd;

        mBlendState = MHWRender::MStateManager::acquireBlendState(desc);
    }
    return mBlendState;
}

const MHWRender::MShaderInstance * TextureBlit::shader()
{
    if (!mShaderInstance)
    {
        MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
        const MHWRender::MShaderManager* shaderMgr = renderer ? renderer->getShaderManager() : NULL;
        if (shaderMgr)
        {
            // Just need a simple color blit
            mShaderInstance = shaderMgr->getEffectsFileShader("aiCopy", "");
        }
    }
    MStatus status = MStatus::kFailure;

    if (mShaderInstance)
    {
        CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
        MString arvCrop = session ? session->GetRenderViewOption(MString("Crop Region")) : MString();
        bool regionCropped = arvCrop == MString("1");
      
        mShaderInstance->setParameter("gUseScissorRect", regionCropped);
        mShaderInstance->setParameter("gScissorRect", &ViewRectangle()[0]);
        mShaderInstance->setParameter("gDisableAlpha", false);

        // If texture changed then bind new texture to the shader
        // 
        status = MStatus::kSuccess;
        if (mColorTextureChanged)
        {
            status = mShaderInstance->setParameter("gInputTex", mColorTexture);
            mColorTextureChanged = false;
        }
    }
    if (status != MStatus::kSuccess)
    {
        return NULL;
    }
    return mShaderInstance;
}

MHWRender::MClearOperation &
TextureBlit::clearOperation()
{
    mClearOperation.setClearGradient(false);
    mClearOperation.setMask(MHWRender::MClearOperation::kClearNone);
    return mClearOperation;
}

MFloatPoint TextureBlit::ViewRectangle() const { return s_ViewRectangle; }
MFloatPoint UIObjectDraw::ViewRectangle() const { return s_ViewRectangle; }

const MSelectionList* UIObjectDraw::objectSetOverride()
{
    // only render selected items
    MGlobal::getActiveSelectionList(m_selectionList);
    return &m_selectionList;
}

/*
	Class for testing HUD operation options
*/

ArnoldViewHUDRender::ArnoldViewHUDRender(const MString &name)
	: MHWRender::MHUDRender()
    , mUserUIDrawables(true)
	, mName(name)
    , mProgress(0.0f)
    , mPixelRatio(1.0)
    , mHOffset(0)
	{
	}

void ArnoldViewHUDRender::addUIDrawables( MHWRender::MUIDrawManager& drawManager2D, const MHWRender::MFrameContext& frameContext )
{
    if (mUserUIDrawables && mStatus != MString(""))
    {
        // Start draw UI
        drawManager2D.beginDrawable();
        // Set font color
        drawManager2D.setColor( MColor( 0.8f, 0.8f, 0.3f ) );
        // Set font size
        drawManager2D.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
    #ifdef _DARWIN
        drawManager2D.setFontName("monaco");
    #else
        drawManager2D.setFontName("monospace");
    #endif

        int x=0, y=0, w=0, h=0;
        frameContext.getViewportDimensions( x, y, w, h );
        float offset = 20.0f*mPixelRatio;
        float hoffset = (20.0f*mHOffset)*mPixelRatio;

        MColor backgroundColor( 0.1, 0.1, 0.1);
        
        if (mOldProgress < 100.0)
        {
            float progressbar_max_width = 300.0f;
            float progress_step_width = 0.001;
            if (mProgress >= 0.0)
                progress_step_width = (mProgress*((progressbar_max_width/2)/100.0)*2);
            
            int statuslength = mRenderStatus.numChars()+1;
            float statusOffset = (statuslength*9.0)*mPixelRatio;
            std::cout << "mPixelRatio " << mPixelRatio << " :: statusOffset " << statusOffset << std::endl; 
            drawManager2D.text( MPoint(offset, (h*0.95f)-hoffset), mRenderStatus, MHWRender::MUIDrawManager::kLeft );
            // Draw progress bar
            drawManager2D.setLineStyle( MHWRender::MUIDrawManager::kSolid );
            drawManager2D.rect2d(MPoint((progressbar_max_width/2)+(offset)+5.0+(statusOffset), h*0.955-hoffset), MVector(0.0,1.0), 
                                (progressbar_max_width/2)+4.0, 
                                4.0);
            drawManager2D.setLineWidth( 4.0f );
            drawManager2D.line2d(MPoint((offset)+4.0+(statusOffset), h*0.95525f-hoffset), MPoint((offset)+progress_step_width+5.5+(statusOffset), h*0.95525-hoffset));
            char buffer[10];
            std::snprintf(buffer, 10, "%.2f%%", mProgress);
            drawManager2D.text( MPoint((offset)+314.0+(statusOffset), h*0.95f-hoffset), MString(buffer), MHWRender::MUIDrawManager::kLeft );
            
        }
        // Draw status text
        drawManager2D.text( MPoint(offset, ((h*0.97f)-hoffset)), mStatus, MHWRender::MUIDrawManager::kLeft );

        // End draw UI
        drawManager2D.endDrawable();
    }
}


// Options
void ArnoldViewHUDRender::setUserUIDrawables(bool val)
{
    mUserUIDrawables = val;
}

void ArnoldViewHUDRender::setProgress(float value)
{
    mOldProgress = mProgress;
    mProgress = value;
}

void ArnoldViewHUDRender::setStatus(MString status)
{
    mStatus = status;
}

void ArnoldViewHUDRender::setHorizontalOffset(int offset)
{
    mHOffset = offset;
}

void ArnoldViewHUDRender::setPixelRatio(float ratio)
{
    mPixelRatio = ratio;
}

void ArnoldViewHUDRender::setRenderStatus(MString status)
{
    mRenderStatus = ltrim(status.asChar()).c_str();
}
