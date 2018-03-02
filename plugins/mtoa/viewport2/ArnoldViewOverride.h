#ifndef ArnoldViewOverride_h_
#define ArnoldViewOverride_h_
//-
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MUiMessage.h>
#include <maya/MSelectionList.h>

#if MAYA_API_VERSION >= 201700
//
// Simple override class derived from MRenderOverride
//
class ArnoldViewOverride : public MHWRender::MRenderOverride
{
public:
    ArnoldViewOverride(const MString & name);
    ~ArnoldViewOverride() override;
    MHWRender::DrawAPI supportedDrawAPIs() const override;

    // Basic setup and cleanup
    MStatus setup(const MString & destination) override;
    MStatus cleanup() override;

    // UI name
    MString uiName() const override
    {
        return mUIName;
    }

    void startRenderView(const MDagPath &camera, int width, int height);

protected:
    // UI name 
    MString mUIName;
    MHWRender::MTexture *mTexture;

    // Callback IDs for tracking viewport changes
    MCallbackId mRendererChangeCB;
    MCallbackId mRenderOverrideChangeCB;
    static void sRendererChangeFunc(
        const MString& panelName,
        const MString& oldRenderer,
        const MString& newRenderer,
        void* clientData);
    static void sRenderOverrideChangeFunc(
        const MString& panelName,
        const MString& oldRenderer,
        const MString& newRenderer,
        void* clientData);
};

//
// Image blit used to perform the 'scene render'
//
class TextureBlit : public MHWRender::MQuadRender
{
public:
    TextureBlit(const MString &name);
    ~TextureBlit() override;

    const MHWRender::MShaderInstance * shader() override;
    MHWRender::MClearOperation & clearOperation() override;

    inline void setColorTexture(MHWRender::MTexture* texture)
    {
        mColorTexture.texture = texture;
        mColorTextureChanged = true;
    }

    MFloatPoint ViewRectangle() const;

protected:
    // Shader to use for the quad render
    MHWRender::MShaderInstance *mShaderInstance;
    // Texture used for the quad render. Not owned by operation.
    MHWRender::MTextureAssignment mColorTexture;
    bool mColorTextureChanged;
};



class UIObjectDraw : public MHWRender::MSceneRender
{
public:
    UIObjectDraw() : MSceneRender(MHWRender::MRenderOperation::kStandardSceneName)
    {
    }

    MSceneFilterOption renderFilterOverride() override
    {
        return MHWRender::MSceneRender::kRenderNonShadedItems;
    }

    const MSelectionList* objectSetOverride() override;

    MFloatPoint ViewRectangle() const;

protected:
    MSelectionList m_selectionList;
};

#endif 

#endif
