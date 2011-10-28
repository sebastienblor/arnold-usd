#ifndef NURBSSURFACETRANSLATOR_H
#define NURBSSURFACETRANSLATOR_H

#include "GeometryTranslator.h"

class MFnMeshData;
class MTesselationParams;
class MFnNurbsSurface;

class CNurbsSurfaceTranslator : public CGeometryTranslator
{
public:
   virtual void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode, AtUInt step);
   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   AtNode* CreateArnoldNodes();
private:
   MObject m_data_mobj;
   bool Tessellate(MDagPath & dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};

#endif // NURBSSURFACETRANSLATOR_H
