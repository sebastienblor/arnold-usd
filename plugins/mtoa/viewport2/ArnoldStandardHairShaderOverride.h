#pragma once

#include <maya/MPxSurfaceShadingNodeOverride.h>

class ArnoldStandardHairShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
   static MHWRender::MPxSurfaceShadingNodeOverride* creator(const MObject& obj);

   virtual ~ArnoldStandardHairShaderOverride();

   virtual MHWRender::DrawAPI supportedDrawAPIs() const;
   virtual MString fragmentName() const;
   virtual void getCustomMappings(MHWRender::MAttributeParameterMappingList& mappings);

   virtual void updateDG();
   virtual void updateShader(MHWRender::MShaderInstance& shader,
                              const MHWRender::MAttributeParameterMappingList& mappings);

   virtual MString bumpAttribute() const;
   virtual MString transparencyParameter() const;
private:
   ArnoldStandardHairShaderOverride(const MObject& obj);

   MObject m_object;
   MString m_fragmentName;
};
