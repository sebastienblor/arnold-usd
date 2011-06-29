#ifndef MESHTRANSLATOR_H
#define MESHTRANSLATOR_H

#include "GeometryTranslator.h"

class CMeshTranslator : public CGeometryTranslator
{
public:
   virtual AtNode* Init(CMayaScene* scene, MDagPath& dagPath, MString outputAttr="")
   {
      m_fnMesh.setObject(dagPath);
      return CGeometryTranslator::Init(scene, dagPath, outputAttr);
   }

   virtual void Export(AtNode* anode);
   static void* creator()
   {
      return new CMeshTranslator();
   }
   AtNode* CreateArnoldNodes();
private:
   unsigned int GetNumMeshGroups();
};

#endif // MESHTRANSLATOR_H
