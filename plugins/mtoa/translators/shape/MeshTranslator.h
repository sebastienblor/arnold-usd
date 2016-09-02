#pragma once

#include "GeometryTranslator.h"

class CMeshTranslator : public CPolygonGeometryTranslator
{
public:

   virtual void ExportMotion(AtNode* anode);

   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CMeshTranslator();
   }
   AtNode* CreateArnoldNodes();

   virtual bool IsRenderable() const;

protected:
   CMeshTranslator() :
      CPolygonGeometryTranslator()
   {}
   
   virtual void NodeChanged(MObject& node, MPlug& plug);
   virtual bool Tessellate(const MDagPath &dagPath);

private:
   MObject m_dataMesh;
   
};
