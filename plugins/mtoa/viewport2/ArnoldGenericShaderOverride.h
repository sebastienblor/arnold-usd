#pragma once

#include <maya/MPxSurfaceShadingNodeOverride.h>

class ArnoldGenericShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
   static MHWRender::MPxSurfaceShadingNodeOverride* creator(const MObject& obj);

   virtual ~ArnoldGenericShaderOverride();

   virtual MHWRender::DrawAPI supportedDrawAPIs() const;
   virtual MString fragmentName() const;
   virtual void getCustomMappings(MHWRender::MAttributeParameterMappingList& mappings);

   virtual void updateDG();
   virtual void updateShader(MHWRender::MShaderInstance& shader,
                              const MHWRender::MAttributeParameterMappingList& mappings);

   virtual bool valueChangeRequiresFragmentRebuild(const MPlug* plug) const;
private:
   ArnoldGenericShaderOverride(const MObject& obj);

   MObject m_object;
   MString m_fragmentName;

   mutable const MHWRender::MSamplerState* m_samplerState;
};
