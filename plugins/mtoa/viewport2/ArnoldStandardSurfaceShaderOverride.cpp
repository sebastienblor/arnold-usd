#include "ArnoldStandardSurfaceShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldStandardSurfaceShaderOverride::creator(const MObject& obj)
{
   return new ArnoldStandardSurfaceShaderOverride(obj);
}

ArnoldStandardSurfaceShaderOverride::ArnoldStandardSurfaceShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj), m_fragmentName("")
{
   MStringArray reqs;
   reqs.append("standardShaderLightingData");
   reqs.append("standardSurfaceShaderCombiner");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderEmission");
   reqs.append("standardShaderSpecular");
   reqs.append("standardShaderLightingCombiner");
   reqs.append("standardfloatPassThrough");
   if (LoadFragmentGraph("standardSurfaceShader", reqs))
      m_fragmentName = "standardSurfaceShader";
}

ArnoldStandardSurfaceShaderOverride::~ArnoldStandardSurfaceShaderOverride()
{

}

MHWRender::DrawAPI ArnoldStandardSurfaceShaderOverride::supportedDrawAPIs() const
{
   return MHWRender::kAllDevices;
}

MString ArnoldStandardSurfaceShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldStandardSurfaceShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
}

void ArnoldStandardSurfaceShaderOverride::updateDG()
{   
}

void ArnoldStandardSurfaceShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
}

MString ArnoldStandardSurfaceShaderOverride::bumpAttribute() const
{
   return "normalCamera";
}

MString ArnoldStandardSurfaceShaderOverride::transparencyParameter() const
{
    return "transmission";
}
