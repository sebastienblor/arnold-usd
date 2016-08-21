#pragma once

#include "GeometryTranslator.h"

class MFnMeshData;
class MTesselationParams;
class MFnNurbsSurface;

class CNurbsSurfaceTranslator : public CPolygonGeometryTranslator
{
public:
   virtual void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode);
   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   AtNode* CreateArnoldNodes();
protected:
   CNurbsSurfaceTranslator();
   
private:
   MStatus Tessellate(const MDagPath &dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};
