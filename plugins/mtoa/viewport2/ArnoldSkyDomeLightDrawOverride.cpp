#include "ArnoldSkydomeLightDrawOverride.h"

MHWRender::MPxDrawOverride* CArnoldSkyDomeLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldSkyDomeLightDrawOverride(obj);
}

CArnoldSkyDomeLightDrawOverride::CArnoldSkyDomeLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldSkyDomeLightDrawOverride::~CArnoldSkyDomeLightDrawOverride()
{

}

bool CArnoldSkyDomeLightDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return false;
}

MBoundingBox CArnoldSkyDomeLightDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    return bbox;
}

bool CArnoldSkyDomeLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

MUserData* CArnoldSkyDomeLightDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    return 0;
}

MHWRender::DrawAPI CArnoldSkyDomeLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldSkyDomeLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{

}
