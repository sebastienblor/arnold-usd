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

protected:
   CMeshTranslator();
   
   // overridden from CDagTranslator to add a GetNumMeshGroups check
   virtual bool DoIsMasterInstance(const MDagPath& dagPath, MDagPath &masterDag);
   virtual void NodeChanged(MObject& node, MPlug& plug);
   virtual bool Tessellate(const MDagPath &dagPath);

private:
   MObject m_dataMesh;
   
   unsigned int GetNumMeshGroups(const MDagPath& dagPath);
};
