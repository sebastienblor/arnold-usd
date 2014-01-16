#include "ArnoldStandardShaderOverride.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldStandardShaderOverride::creator(const MObject& obj)
{
   return new ArnoldStandardShaderOverride(obj);
}

ArnoldStandardShaderOverride::ArnoldStandardShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj),
  m_resolvedColorName("")
{
   m_color[0] = 0.8f;
   m_color[1] = 0.8f;
   m_color[2] = 0.8f;
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
   MHWRender::MAttributeParameterMapping colorMapping("color", "", true, true);
   mappings.append(colorMapping);

   MHWRender::MAttributeParameterMapping diffuseMapping("diffuse", "", true, true);
   mappings.append(diffuseMapping);

   MHWRender::MAttributeParameterMapping specularMapping("specularColor", "KsColor", true, true);
   mappings.append(colorMapping);
}

void ArnoldStandardShaderOverride::updateDG()
{
   MStatus status;
   MFnDependencyNode dnode(m_object, &status);
   if (status)
   {
      MPlug colorPlug = dnode.findPlug("color");
      MPlug kdPlug = dnode.findPlug("Kd");
      const float kd = kdPlug.asFloat();

      m_color[0] = colorPlug.child(0).asFloat() * kd;
      m_color[1] = colorPlug.child(1).asFloat() * kd;
      m_color[2] = colorPlug.child(2).asFloat() * kd;

      std::cerr << m_color[0] << " " << m_color[1] << " " << m_color[2] << std::endl;
   }
   else
      std::cerr << "bbb" << std::endl;
}

void ArnoldStandardShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
   std::cerr << "ccc\n";
   if (m_resolvedColorName.length() == 0)
   {
      const MHWRender::MAttributeParameterMapping* mapping = mappings.findByParameterName("color");
      if (mapping)
         m_resolvedColorName = mapping->resolvedParameterName();
   }

   if (m_resolvedColorName.length() > 0)
   {
      std::cerr << "aaa\n";
      shader.setParameter(m_resolvedColorName, m_color);
   }
}

MString ArnoldStandardShaderOverride::primaryColorParameter() const
{
   return "";
}

MString ArnoldStandardShaderOverride::bumpAttribute() const
{
   return "normalCamera";
}
