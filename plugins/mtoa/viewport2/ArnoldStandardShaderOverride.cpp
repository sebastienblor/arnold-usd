#include "ArnoldStandardShaderOverride.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>

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

   static bool loaded = false;

   if (!loaded)
   {      
      MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
      if (theRenderer)
      {
         MHWRender::MFragmentManager* fragmentMgr = theRenderer->getFragmentManager();
         if (fragmentMgr)
         {
            loaded = true;
            fragmentMgr->addFragmentPath("/work/deploy/2015/vp2");
            fragmentMgr->addShadeFragmentFromFile("standardShaderCombiner.xml", false);
            fragmentMgr->addShadeFragmentFromFile("standardShaderDiffuse.xml", false);
            fragmentMgr->addFragmentGraphFromFile("standardShader.xml");
         }
      }
   }  
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
   return "standardShader"; // TODO : replace this later with our own shader
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
