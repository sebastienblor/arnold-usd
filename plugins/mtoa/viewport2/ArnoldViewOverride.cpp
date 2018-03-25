//-
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+


#include "ArnoldViewOverride.h"
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MTextureManager.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include <maya/MSceneMessage.h>

#include "scene/MayaScene.h"
#include "translators/DagTranslator.h"

#if MAYA_API_VERSION >= 201700

static MFloatPoint s_ViewRectangle = MFloatPoint(0.33f, 0.33f, 0.66f, 0.66f);
static MString s_activeViewport(""); // store the name of the last active viewport

// For override creation we return a UI name so that it shows up in as a
// renderer in the 3d viewport menus.
// 
ArnoldViewOverride::ArnoldViewOverride(const MString & name)
    : MRenderOverride(name)
    , mUIName("Arnold")
    , mTexture(NULL)
    , mRendererChangeCB(0)
    , mRenderOverrideChangeCB(0)
{
    // register a file open and file new callback
    mFileNewCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeNew, sPreFileOpen, this);
    mFileOpenCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeOpen, sPreFileOpen, this);
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
}

// Drawing uses all internal code so will support all draw APIs
//
MHWRender::DrawAPI ArnoldViewOverride::supportedDrawAPIs() const
{
    return MHWRender::kAllDevices;
}

void ArnoldViewOverride::startRenderView(const MDagPath &camera, int width, int height)
{
    CMayaScene::End();

    MCommonRenderSettingsData renderGlobals;
    MRenderUtil::getCommonRenderSettings(renderGlobals);

    CMayaScene::ExecuteScript(renderGlobals.preMel);
    CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

    CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);

    // SetExportCamera mus be called AFTER CMayaScene::Export
    CMayaScene::GetArnoldSession()->SetExportCamera(camera);
    CRenderSession *renderSession = CMayaScene::GetRenderSession();
    // Set resolution and camera as passed in.
    if (renderSession)
    {
        renderSession->SetResolution(width, height);
        renderSession->RenderOptions()->UpdateImageDimensions();
        renderSession->SetCamera(camera);
    }
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
    // 3) Initialize some default state for this panel
    RegionRenderState state;
    if (callbackIdMap.find(destination.asChar()) == callbackIdMap.end())
    {
        stopExistingOverrides(destination);

        MCallbackId callbackID = MUiMessage::add3dViewRenderOverrideChangedCallback(destination, sRenderOverrideChangeFunc, this);
        callbackIdMap[destination.asChar()] = callbackID;

        state.enabled = false;
        state.useRegion = true;
        state.initialized = false;
        state.viewRectangle = MFloatPoint(0.33f, 0.33f, 0.66f, 0.66f);
        mRegionRenderStateMap[destination.asChar()] = state;
    }
    else
    {
        state = mRegionRenderStateMap[destination.asChar()];
    }

    // if there's no active scene, or if we switched to a new viewport, then this is considered as a first render
    bool firstRender = (!CMayaScene::IsActive() || destination != s_activeViewport);
    
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
        mOperations.append(new MHWRender::MHUDRender());
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

    if (firstRender)
    {
        startRenderView(camera, width, height);

        if (mTexture)
        {
            // clear the texture so that it's generated at next display
            MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
            MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
            textureManager->releaseTexture(mTexture);
            mTexture = NULL;
        }
    }
    s_activeViewport = destination; // set this viewport as the "active" one

    CRenderSession* renderSession = CMayaScene::GetRenderSession();

    if (!state.initialized)
    {
        CRenderOptions *renderOptions = renderSession->RenderOptions();
        MString enabledStr = (state.enabled) ? MString("1") : MString("0");
        MString useRegionStr = (state.useRegion) ? MString("1"): MString("0");
        renderSession->SetRenderViewOption(MString("Run IPR"), enabledStr);
        renderSession->SetRenderViewOption(MString("Crop Region"),useRegionStr);
        renderSession->SetResolution(width, height);
        renderOptions->UpdateImageDimensions();

        if (state.useRegion)
        {
            renderOptions->SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
                AiClamp(int((1.f - state.viewRectangle.w) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/     
        }
        else
        {
            renderOptions->ClearRegion();
        }
    }

    bool needsRefresh = false;

    if (!firstRender)
	{
        CRenderOptions *renderOptions = renderSession->RenderOptions();

        // A render had already started, we want to check if the window size has changed
		int previousWidth = renderOptions->width();
		int previousHeight = renderOptions->height();
		if (previousWidth != width || previousHeight != height)
		{
			AtBBox2 bounds;
			renderSession->HasRenderResults(bounds);
			renderSession->PostDisplay();
			renderSession->InterruptRender(true);
			renderSession->SetResolution(width, height);
			renderOptions->UpdateImageDimensions();
		
			if (renderOptions->useRenderRegion())
			{
                // eventually adapt the arnold crop region
				renderOptions->SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - state.viewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/

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
        if (!(renderSession->GetCamera() == camera))
        {
            // the camera has changed
            renderSession->InterruptRender(true);
            CDagTranslator *camTranslator = CMayaScene::GetArnoldSession()->ExportDagPath(camera, true);
            // SetExportCamera mus be called AFTER CMayaScene::Export
            CMayaScene::GetArnoldSession()->SetExportCamera(camera);
            renderSession->SetCamera(camera);
            AtNode *camNode = (camTranslator) ? camTranslator->GetArnoldNode() : NULL;
            if (camNode)
                AiNodeSetPtr(AiUniverseGetOptions(), "camera", camNode);

            needsRefresh = true;
        }
	}

    if (!state.initialized)
    {
        state.initialized = true;
        mRegionRenderStateMap[destination.asChar()] = state;

        MGlobal::executeCommandOnIdle("aiViewRegionCmd -create;");
        MGlobal::executeCommand("arnoldViewOverrideOptionBox;");
    }

    renderSession = CMayaScene::GetRenderSession();
    CRenderOptions *renderOptions = renderSession->RenderOptions();
	if (renderSession->IsRegionCropped() != renderOptions->useRenderRegion())
	{
		// ARV settings are different from the ones in the render options. ARV wins !
		if (renderSession->IsRegionCropped())
		{
	        renderOptions->SetRegion(AiClamp(int(state.viewRectangle.x * width), 0, width - 1), AiClamp(int(state.viewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - state.viewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - state.viewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/

		} else
		{
			renderOptions->ClearRegion();
		}
	}
	if (renderOptions && renderOptions->useRenderRegion())
    {
        int width = renderOptions->width();
        int height = renderOptions->height();
        int minx = renderOptions->minX();
        int miny = renderOptions->minY();
        int maxx = renderOptions->maxX();
        int maxy = renderOptions->maxY();

        state.viewRectangle.x = (float)minx / (float)width;
        state.viewRectangle.y = 1.f - ((float)maxy / (float)height);
        state.viewRectangle.z = (float)(maxx) / (float)width;
        state.viewRectangle.w = 1.f - ((float)miny / (float)height);

        s_ViewRectangle = state.viewRectangle;
    }

    state.enabled = !renderSession->IsIPRPaused();
    mRegionRenderStateMap[destination.asChar()] = state;

    if (!CMayaScene::IsActive())
    {
        MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
        return MS::kFailure;
    }

    // if the session is not rendering, but the state is enabled
    // end the current empty scene and start a new rendering
    if (!renderSession->IsRendering() && state.enabled)
    {
        // end the existing (empty) scene
        renderSession->CloseRenderViewWithSession(false);
        CMayaScene::End();

        needsRefresh = true;
    }

    // now get the current bits
    // and create a texture from them.
    AtBBox2 bounds;
    // Note that we're not using the "bounds" region here, about which region
    // of the buffer needs to be updated
    bool results = renderSession->HasRenderResults(bounds);

    if (needsRefresh)
    {
        // now restart the render and early out. We'll be called here again in the next refresh.
        renderSession->SetRenderViewOption(MString("Refresh Render"), MString("1"));
        // disable the arnold operation until the next frame
        mOperations[2]->setEnabled(false);
        return MS::kSuccess;
    }

    // disable the arnold operation if paused
    mOperations[2]->setEnabled(state.enabled);
    if (!state.enabled)
        return MS::kSuccess;

    const AtRGBA *buffer = renderSession->GetDisplayedBuffer();
    if (buffer)
    {
        if (mTexture == NULL)
        {
            MHWRender::MTextureDescription desc;
            desc.setToDefault2DTexture();
            desc.fWidth = width;
            desc.fHeight = height;
            desc.fFormat = MHWRender::kR32G32B32A32_FLOAT;

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

    // FIXME temp. function that we need to call for now. We'll manage to remove it
    // after the renderview switches to the new Render Control API
    renderSession->PostDisplay();

    return MStatus::kSuccess;
}

// On cleanup we just return for returning the list of operations for
// the next render
//
MStatus ArnoldViewOverride::cleanup()
{
    return MStatus::kSuccess;
}

// Callback for tracking renderer changes
void ArnoldViewOverride::sRendererChangeFunc(
    const MString& panelName,
    const MString& oldRenderer,
    const MString& newRenderer,
    void* clientData)
{
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
        
        // Closing the options window through this will also properly interupt the renderer and end the scene.
        CRenderSession* renderSession = CMayaScene::GetRenderSession();
        if (renderSession)
            renderSession->CloseOptionsWindow();

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

void ArnoldViewOverride::sPreFileOpen(void* clientData)
{
    static_cast<ArnoldViewOverride*>(clientData)->mRegionRenderStateMap.clear();
    stopExistingOverrides("");
}

void ArnoldViewOverride::stopExistingOverrides(const MString & destination)
{
    CRenderSession* renderSession = CMayaScene::GetRenderSession();
    if (renderSession)
    {
        renderSession->CloseOtherViews(destination);
    }
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
		CRenderSession *renderSession = CMayaScene::GetRenderSession();

        bool regionCropped = (renderSession) ? renderSession->IsRegionCropped() : false; // check if option is enabled in ARV
		
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

#endif