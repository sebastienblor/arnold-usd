#ifndef MESHTRANSLATOR_H
#define MESHTRANSLATOR_H

#include "GeometryTranslator.h"

class CMeshTranslator : public CGeometryTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      return CGeometryTranslator::Init(session, dagPath, outputAttr);
   }

   virtual void Export(AtNode* anode);
   
   virtual void ExportMotion(AtNode* anode, unsigned int step);

   virtual bool IsGeoDeforming();

   static void* creator()
   {
      return new CMeshTranslator();
   }
   AtNode* CreateArnoldNodes();
protected:
   CMeshTranslator()  :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "polymesh";
   }
   // overridden from CDagTranslator to add a GetNumMeshGroups check
   virtual bool IsMasterInstance(MDagPath &masterDag);
private:
   MObject m_dataMesh;
   MStatus Tessellate(const MDagPath &dagPath);
   unsigned int GetNumMeshGroups(const MDagPath& dagPath);
};

#endif // MESHTRANSLATOR_H
