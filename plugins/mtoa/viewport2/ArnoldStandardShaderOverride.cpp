#include "ArnoldStandardShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldStandardShaderOverride::creator(const MObject& obj)
{
   return new ArnoldStandardShaderOverride(obj);
}

ArnoldStandardShaderOverride::ArnoldStandardShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj), m_fragmentName("")
{
   MStringArray reqs;
   reqs.append("standardShaderLightingData");
   reqs.append("standardShaderCombiner");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderEmission");
   reqs.append("standardShaderSpecular");   
   reqs.append("standardfloatPassThrough");
   reqs.append("standardShaderLightingCombiner");
   if (LoadFragmentGraph("standardShader", reqs))
      m_fragmentName = "standardShader";
}

ArnoldStandardShaderOverride::~ArnoldStandardShaderOverride()
{

}

MHWRender::DrawAPI ArnoldStandardShaderOverride::supportedDrawAPIs() const
{
   return MHWRender::kAllDevices;
}

MString ArnoldStandardShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldStandardShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
}

void ArnoldStandardShaderOverride::updateDG()
{   
}

void ArnoldStandardShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
}

MString ArnoldStandardShaderOverride::bumpAttribute() const
{
   return "normalCamera";
}
