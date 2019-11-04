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

#include <map>

//
// Simple override class derived from MRenderOverride
//
class ArnoldViewOverride : public MHWRender::MRenderOverride
{
public:
    ArnoldViewOverride(const MString & name);
    virtual ~ArnoldViewOverride();
    virtual MHWRender::DrawAPI supportedDrawAPIs() const;

    // Basic setup and cleanup
    virtual MStatus setup(const MString & destination);
    virtual MStatus cleanup();

    // UI name
    virtual MString uiName() const
    {
        return mUIName;
    }

protected:

    void startRenderView(const MDagPath &camera, int width, int height);

    static void stopExistingOverrides(const MString & destination);

    // UI name 
    MString mUIName;
    MHWRender::MTexture *mTexture;

    // Callback IDs for tracking viewport changes
    MCallbackId mRendererChangeCB;
    MCallbackId mRenderOverrideChangeCB;
    MCallbackId mPlablastCB;
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
    static void sPreFileOpen(
        void* clientData);


    std::map<std::string, MCallbackId> callbackIdMap;

    struct RegionRenderState {
        bool enabled;
        bool useRegion;
        bool initialized;
        MFloatPoint viewRectangle;
    };
    std::map<std::string, RegionRenderState> mRegionRenderStateMap;
    MCallbackId mFileOpenCallbackID;
    MCallbackId mFileNewCallbackID;
};

//
// Image blit used to perform the 'scene render'
//
class TextureBlit : public MHWRender::MQuadRender
{
public:
    TextureBlit(const MString &name);
    virtual ~TextureBlit();

    virtual const MHWRender::MShaderInstance * shader();
    virtual MHWRender::MClearOperation & clearOperation();

    const virtual MHWRender::MBlendState* blendStateOverride();

    inline void setColorTexture(MHWRender::MTexture* texture)
    {
        mColorTexture.texture = texture;
        mColorTextureChanged = true;
    }

    MFloatPoint ViewRectangle() const;

protected:
    // Shader to use for the quad render
    MHWRender::MShaderInstance *mShaderInstance;
    const MHWRender::MBlendState* mBlendState;
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

    virtual MSceneFilterOption renderFilterOverride()
    {
        return MHWRender::MSceneRender::kRenderNonShadedItems;
    }

    virtual const MSelectionList* objectSetOverride();

    MFloatPoint ViewRectangle() const;

protected:
    MSelectionList m_selectionList;
};

#endif
