#include "ArnoldAreaLightDrawOverride.h"

class CArnoldAreaLightUserData : public MUserData{
public:
    CArnoldAreaLightUserData() : MUserData(true)
    {

    }
};

MHWRender::MPxDrawOverride* CArnoldAreaLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldAreaLightDrawOverride(obj);
}

CArnoldAreaLightDrawOverride::CArnoldAreaLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldAreaLightDrawOverride::~CArnoldAreaLightDrawOverride()
{

}

bool CArnoldAreaLightDrawOverride::isBounded(
                                        const MDagPath& objPath,
                                        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldAreaLightDrawOverride::boundingBox(
                                                const MDagPath& objPath,
                                                const MDagPath& cameraPath) const
{
    return MBoundingBox();
}

bool CArnoldAreaLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return false;
}

MUserData* CArnoldAreaLightDrawOverride::prepareForDraw(
                                                    const MDagPath& objPath,
                                                    const MDagPath& cameraPath,
                                                    const MHWRender::MFrameContext& frameContext,
                                                    MUserData* oldData)
{
    return new CArnoldAreaLightUserData();
}

MHWRender::DrawAPI CArnoldAreaLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}


void CArnoldAreaLightDrawOverride::draw(
                                    const MHWRender::MDrawContext& context,
                                    const MUserData* data)
{

}
