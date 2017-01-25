#include "ArnoldSkinShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldSkinShaderOverride::creator(const MObject& obj)
{
   return new ArnoldSkinShaderOverride(obj);
}

ArnoldSkinShaderOverride::ArnoldSkinShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj), m_fragmentName("")
{
   MStringArray reqs;
   reqs.append("skinShaderCombiner");
   reqs.append("standardShaderLightingData");
   reqs.append("standardShaderCombiner");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderLightingCombiner");
   if (LoadFragmentGraph("skinShader", reqs))
      m_fragmentName = "skinShader";
}

ArnoldSkinShaderOverride::~ArnoldSkinShaderOverride()
{

}

MHWRender::DrawAPI ArnoldSkinShaderOverride::supportedDrawAPIs() const
{
   // need to make sure it supports OpenGL core profile (#2551)
   return MHWRender::kAllDevices;
   //return MHWRender::kOpenGL | MHWRender::kDirectX11;
}

MString ArnoldSkinShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldSkinShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
}

void ArnoldSkinShaderOverride::updateDG()
{   
}

void ArnoldSkinShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
}

MString ArnoldSkinShaderOverride::bumpAttribute() const
{
   return "normalCamera";
}
