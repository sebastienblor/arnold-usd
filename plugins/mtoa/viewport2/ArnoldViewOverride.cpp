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

#include "scene/MayaScene.h"
#include "translators/DagTranslator.h"

#if MAYA_API_VERSION >= 201800

static MFloatPoint s_ViewRectangle = MFloatPoint(0.33f, 0.33f, 0.66f, 0.66f);
// For override creation we return a UI name so that it shows up in as a
// renderer in the 3d viewport menus.
// 
ArnoldViewOverride::ArnoldViewOverride(const MString & name)
    : MRenderOverride(name)
    , mUIName("Arnold")
    , mTexture(nullptr)
    , mRendererChangeCB(0)
    , mRenderOverrideChangeCB(0)
{
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
    if (mRenderOverrideChangeCB)
    {
        MMessage::removeCallback(mRenderOverrideChangeCB);
    }
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

    if (!renderGlobals.renderAll)
    {
        MSelectionList selected;
        MGlobal::getActiveSelectionList(selected);
        CMayaScene::Export(&selected);
    }
    else
    {
        CMayaScene::Export();
    }

    // SetExportCamera mus be called AFTER CMayaScene::Export
    CMayaScene::GetArnoldSession()->SetExportCamera(camera);
    CRenderSession *renderSession = CMayaScene::GetRenderSession();
    // Set resolution and camera as passed in.
    if (renderSession)
    {
        renderSession->SetResolution(width, height);
        renderSession->RenderOptions()->UpdateImageDimensions();
        renderSession->SetCamera(camera);

        CRenderOptions *renderOptions = renderSession->RenderOptions();

		if (renderOptions->useRenderRegion())
		{
			// the crop region was already enabled (perhaps from the render settings)
			// set viewRectangle as in the render options
	        int width = renderOptions->width();
			int height = renderOptions->height();
			int minx = renderOptions->minX();
			int miny = renderOptions->minY();
			int maxx = renderOptions->maxX();
			int maxy = renderOptions->maxY();

			s_ViewRectangle.x = (float)minx / (float)width;
			s_ViewRectangle.y = 1.f - ((float)maxy / (float)height);
			s_ViewRectangle.z = (float)maxx / (float)width;
			s_ViewRectangle.w = 1.f - ((float)miny / (float)height);
		} else
        {
            renderOptions->SetRegion(AiClamp(int(s_ViewRectangle.x * width), 0, width -1), AiClamp(int(s_ViewRectangle.z * width), 0, width -1),
                        AiClamp(int((1.f - s_ViewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - s_ViewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/     
        }
        renderSession->SetRenderViewOption(MString("Crop Region"), MString("1"));

    }

}

MStatus ArnoldViewOverride::setup(const MString & destination)
{
    bool firstRender = !CMayaScene::IsActive();

    MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer)
        return MStatus::kFailure;

    // Track changes to the renderer and override for this viewport (nothing
    // will be printed unless mDebugOverride is true)
    if (!mRendererChangeCB)
    {
        mRendererChangeCB = MUiMessage::add3dViewRendererChangedCallback(destination, sRendererChangeFunc, nullptr);
    }
    if (!mRenderOverrideChangeCB)
    {
        MGlobal::executeCommandOnIdle("aiViewRegionCmd -create;");
        MGlobal::executeCommandOnIdle("arnoldViewOverrideOptionBox;");
        mRenderOverrideChangeCB = MUiMessage::add3dViewRenderOverrideChangedCallback(destination, sRenderOverrideChangeFunc, nullptr);
    }

    MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();

    // Create a new set of operations as required
    if (mOperations.length() == 0)
    {
        // create and setup the clear operation
        MClearOperation* clearOp = new MClearOperation(MRenderOperation::kStandardBackgroundName);
        clearOp->setOverridesColors(false);

        // create and setup the scene operation
        MSceneRender* sceneOp = new MSceneRender(MRenderOperation::kStandardSceneName);
        sceneOp->clearOperation().setMask(MClearOperation::kClearNone);

        UIObjectDraw* uiOp = new UIObjectDraw();
        uiOp->clearOperation().setMask(MClearOperation::kClearNone);

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
            MS::kFailure;
        height = mView.portHeight(&status);
        if (status != MS::kSuccess || (height < 2))
            MS::kFailure;

        mView.getCamera(camera);
    }


    if (firstRender)
    {
        startRenderView(camera, width, height);
    } else
	{
		CRenderSession* renderSession = CMayaScene::GetRenderSession();
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
		
			if (mTexture)
			{
                // clear the texture so that it's generated at next display
				MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
				MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
				textureManager->releaseTexture(mTexture);
				mTexture = NULL;
			}
			if (renderOptions->useRenderRegion())
			{
                // eventually adapt the arnold crop region
				renderOptions->SetRegion(AiClamp(int(s_ViewRectangle.x * width), 0, width - 1), AiClamp(int(s_ViewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - s_ViewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - s_ViewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/

			}

            // now restart the render and early out. We'll be called here again in the next refresh.
    		renderSession->SetRenderViewOption(MString("Refresh Render"), MString("1"));
			return MS::kSuccess;
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

            // now restart the render and early out. We'll be called here again in the next refresh.
            renderSession->SetRenderViewOption(MString("Refresh Render"), MString("1"));
            return MS::kSuccess;
        }

	} 
	

    CRenderSession* renderSession = CMayaScene::GetRenderSession();
    CRenderOptions *renderOptions = renderSession->RenderOptions();

	if (renderSession->IsRegionCropped() != renderOptions->useRenderRegion())
	{
		// ARV settings are different from the ones in the render options. ARV wins !
		if (renderSession->IsRegionCropped())
		{
	        renderOptions->SetRegion(AiClamp(int(s_ViewRectangle.x * width), 0, width - 1), AiClamp(int(s_ViewRectangle.z * width), 0, width - 1),
				    AiClamp(int((1.f - s_ViewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - s_ViewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top*/

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

        s_ViewRectangle.x = (float)minx / (float)width;
        s_ViewRectangle.y = 1.f - ((float)maxy / (float)height);
        s_ViewRectangle.z = (float)(maxx) / (float)width;
        s_ViewRectangle.w = 1.f - ((float)miny / (float)height);
    }

    if (!CMayaScene::IsActive())
    {
        MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
        return MS::kFailure;
    }

    if (firstRender)
    {
        renderSession->RunInteractiveRenderer();
    
    }

    // now get the current bits
    // and create a texture from them.
    AtBBox2 bounds;
    // even if there are no new results provided by the renderview, 
    // we need to proceed, otherwise we'll be painting black.
    // Note that we're not using the "bounds" region here, about which region
    // of the buffer needs to be updated
    renderSession->HasRenderResults(bounds);
    
    // FIXME show the desired AOV
    const AtRGBA *buffer = renderSession->GetDisplayedBuffer();
    if (buffer)
    {
        if (mTexture == nullptr)
        {
            MHWRender::MTextureDescription desc;
            desc.setToDefault2DTexture();
            desc.fWidth = width;
            desc.fHeight = height;
            desc.fFormat = MHWRender::kR32G32B32A32_FLOAT;

            mTexture = textureManager->acquireTexture("arnoldResults", desc, buffer, false);
        }
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
        CRenderSession* renderSession = CMayaScene::GetRenderSession();
        if (renderSession)
        {
            renderSession->InterruptRender(true);
            renderSession->CloseRenderViewWithSession(false); // don't close ARV with CMayaScene::End()
            CMayaScene::End();
        }
    }
    if (newOverride == "arnoldViewOverride")
    {
        MGlobal::executeCommand("aiViewRegionCmd -delete;");
        MGlobal::executeCommand("aiViewRegionCmd -create;");
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

    mColorTexture.texture = NULL;
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