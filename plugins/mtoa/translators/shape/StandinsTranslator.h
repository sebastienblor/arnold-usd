#ifndef STANDINSTRANSLATOR_H
#define STANDINSTRANSLATOR_H

#include "translators/NodeTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MNodeMessage.h>
#include <maya/MPxLocatorNode.h>

#include <maya/MString.h>

class DLLEXPORT CArnoldStandInsTranslator : public CDagTranslator
{
public:
   static void* creator()
   {
      return new CArnoldStandInsTranslator();
   }
   virtual AtNode* CreateArnoldNodes();

   static void NodeInitializer(MString nodeClassName);
   virtual void AddIPRCallbacks();
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);

protected:
   void ExportBoundingBox(AtNode* procedural);

   void ExportStandinsShaders(AtNode* procedural);
   virtual void ExportShaders();

   MObject GetNodeShadingGroup(MObject dagNode, int instanceNum);

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural, bool update);
   static void ShaderAssignmentCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*);
   void AddShaderAssignmentCallbacks(MObject & dagNode);

protected:
   MFnDagNode m_DagNode;
};

#endif // STANDINSTRANSLATOR_H
