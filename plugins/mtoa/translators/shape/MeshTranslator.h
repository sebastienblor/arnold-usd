#ifndef MESHTRANSLATOR_H
#define MESHTRANSLATOR_H

#include "GeometryTranslator.h"

class CMeshTranslator : public CGeometryTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_fnMesh.setObject(dagPath);
      return CGeometryTranslator::Init(session, dagPath, outputAttr);
   }

   virtual void Export(AtNode* anode);
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
private:
   unsigned int GetNumMeshGroups();
};

#endif // MESHTRANSLATOR_H
