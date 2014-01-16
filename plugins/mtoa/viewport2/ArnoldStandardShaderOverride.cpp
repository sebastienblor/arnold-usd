#include "ArnoldStandardShaderOverride.h"

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldStandardShaderOverride::creator(const MObject& obj)
{
   return new ArnoldStandardShaderOverride(obj);
}

ArnoldStandardShaderOverride::ArnoldStandardShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj)
{

}

ArnoldStandardShaderOverride::~ArnoldStandardShaderOverride()
{

}

MHWRender::DrawAPI ArnoldStandardShaderOverride::supportedDrawAPIs() const
{
   return MHWRender::kOpenGL | MHWRender::kDirectX11;
}

MString ArnoldStandardShaderOverride::fragmentName() const
{
   return "mayaLambertSurface"; // TODO : replace this later with our own shader
}

void ArnoldStandardShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
   MHWRender::MAttributeParameterMapping colorMapping("color", "color", true, true);
   mappings.append(colorMapping);
}

MString ArnoldStandardShaderOverride::primaryColorParameter() const
{
   return "color";
}
