#pragma once

#include "GeometryTranslator.h"

class CMeshTranslator : public CPolygonGeometryTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      return CPolygonGeometryTranslator::Init(session, dagPath, outputAttr);
   }

   virtual void Export(AtNode* anode);
   
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
   
private:
   MObject m_dataMesh;
   MStatus Tessellate(const MDagPath &dagPath, bool doRef);
   unsigned int GetNumMeshGroups(const MDagPath& dagPath);
};
