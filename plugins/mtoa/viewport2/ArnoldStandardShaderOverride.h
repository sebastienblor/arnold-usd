#pragma once

#include <maya/MPxSurfaceShadingNodeOverride.h>

class ArnoldStandardShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
   static MHWRender::MPxSurfaceShadingNodeOverride* creator(const MObject& obj);

   virtual ~ArnoldStandardShaderOverride();

   virtual MHWRender::DrawAPI supportedDrawAPIs() const;
   virtual MString fragmentName() const;
   virtual void getCustomMappings(MHWRender::MAttributeParameterMappingList& mappings);

   virtual void updateDG();
   virtual void updateShader(MHWRender::MShaderInstance& shader,
                              const MHWRender::MAttributeParameterMappingList& mappings);

   virtual MString primaryColorParameter() const;
   virtual MString bumpAttribute() const;
private:
   ArnoldStandardShaderOverride(const MObject& obj);

   MObject m_object;

   MString m_resolvedColorName;
   float m_color[3];
};
