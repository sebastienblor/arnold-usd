#pragma once

#include <maya/MPxSurfaceShadingNodeOverride.h>

class ArnoldUtilityShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Add(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Abs(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Atan(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Min(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Max(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Negate(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Subtract(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Multiply(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Divide(const MObject& obj);

    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Pow(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Sqrt(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Dot(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Log(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Exp(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Cross(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Normalize(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Length(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Modulo(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Fraction(const MObject& obj);
    static MHWRender::MPxSurfaceShadingNodeOverride* creator_Clamp(const MObject& obj);

    virtual ~ArnoldUtilityShaderOverride();

    virtual MHWRender::DrawAPI supportedDrawAPIs() const;
    virtual MString fragmentName() const;

private:
    ArnoldUtilityShaderOverride(const MObject& obj, MString graphName, MStringArray reqs);
    ArnoldUtilityShaderOverride(const MObject& obj, MString fragmentName);

    MString m_fragmentName;
};
