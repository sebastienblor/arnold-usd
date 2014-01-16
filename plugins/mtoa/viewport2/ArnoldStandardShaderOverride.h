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

   virtual MString primaryColorParameter() const;

private:
   ArnoldStandardShaderOverride(const MObject& obj);
};
