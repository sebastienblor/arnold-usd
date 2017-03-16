#include "ArnoldGenericShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldGenericShaderOverride::creator(const MObject& obj)
{
   return new ArnoldGenericShaderOverride(obj);
}

ArnoldGenericShaderOverride::ArnoldGenericShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj), m_fragmentName("")
{
   MStringArray reqs;
   reqs.append("genericShaderCombiner");
   reqs.append("standardShaderLightingData");
   reqs.append("standardShaderCombiner");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderLightingCombiner");
   if (LoadFragmentGraph("genericShader", reqs))
      m_fragmentName = "genericShader";
}

ArnoldGenericShaderOverride::~ArnoldGenericShaderOverride()
{

}

MHWRender::DrawAPI ArnoldGenericShaderOverride::supportedDrawAPIs() const
{
   // need to make sure it supports OpenGL core profile (#2551)
   return MHWRender::kAllDevices;
   // return MHWRender::kOpenGL | MHWRender::kDirectX11;
}

MString ArnoldGenericShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldGenericShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
}

void ArnoldGenericShaderOverride::updateDG()
{   
}

void ArnoldGenericShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
}
