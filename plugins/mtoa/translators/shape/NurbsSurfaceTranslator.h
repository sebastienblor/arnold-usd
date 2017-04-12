#pragma once

#include "GeometryTranslator.h"
#include <maya/MFnMeshData.h>
#include <maya/MTesselationParams.h>
#include <maya/MFnNurbsSurface.h>

class CNurbsSurfaceTranslator : public CPolygonGeometryTranslator
{
public:
   virtual void ExportMotion(AtNode* anode);
   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   AtNode* CreateArnoldNodes();
protected:
   CNurbsSurfaceTranslator() :
      CPolygonGeometryTranslator()
   {}
   
   virtual bool Tessellate(const MDagPath &dagPath);
   virtual void NodeChanged(MObject& node, MPlug& plug);


private:
   
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};
