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
   virtual void ExportMotion(AtNode* anode, unsigned int step);
   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   AtNode* CreateArnoldNodes();
protected:
   CNurbsSurfaceTranslator()  :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "polymesh";
   }
private:
   MObject m_data_mobj;
   bool Tessellate(MDagPath & dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};

#endif // NURBSSURFACETRANSLATOR_H
