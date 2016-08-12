#pragma once

#include "GeometryTranslator.h"

class MFnMeshData;
class MTesselationParams;
class MFnNurbsSurface;

class CNurbsSurfaceTranslator : public CPolygonGeometryTranslator
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
      CPolygonGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "polymesh";
   }
private:
   MStatus Tessellate(const MDagPath &dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};
