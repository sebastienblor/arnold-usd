#include "ArnoldUtilityShaderOverrides.h"
#include "viewport2/ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MRenderUtil.h>
#include <algorithm>


ArnoldUtilityShaderOverride::ArnoldUtilityShaderOverride(const MObject& obj, MString fragmentName, MStringArray reqs)
    : MPxSurfaceShadingNodeOverride(obj)
{
    if (LoadFragmentGraph(fragmentName, reqs))
        m_fragmentName = fragmentName;
}

ArnoldUtilityShaderOverride::ArnoldUtilityShaderOverride(const MObject& obj, MString fragmentName)
    : MPxSurfaceShadingNodeOverride(obj)
{
    if (LoadShadeFragment(fragmentName))
        m_fragmentName = fragmentName;
}

ArnoldUtilityShaderOverride::~ArnoldUtilityShaderOverride()
{
}

MHWRender::DrawAPI ArnoldUtilityShaderOverride::supportedDrawAPIs() const
{
    return MHWRender::kAllDevices;
}

MString ArnoldUtilityShaderOverride::fragmentName() const
{
    return m_fragmentName;
}

// static constructors for all the various utility nodes.
MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Add(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiAdd");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Subtract(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiSubtract");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Multiply(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiMultiply");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Divide(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiDivide");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Min(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiMin");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Max(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiMax");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Abs(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiAbs");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Atan(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiAtan");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Negate(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiNegate");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Pow(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiPow");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Sqrt(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiSqrt");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Dot(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiDot");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Length(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiLength");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Log(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiLog");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Normalize(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiNormalize");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Exp(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiExp");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Cross(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiCross");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Modulo(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiModulo");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Fraction(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiFraction");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Clamp(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiClamp");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Flat(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiFlat");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Mix(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiMixShader");
}

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldUtilityShaderOverride::creator_Round_Corners(const MObject& obj)
{
    return new ArnoldUtilityShaderOverride(obj, "aiRoundCorners");
}