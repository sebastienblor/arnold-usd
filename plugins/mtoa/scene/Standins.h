
#ifndef STANDINS_H
#define STANDINS_H

#include "scene/Geometry.h"
#include <maya/MFnMesh.h>
#include <maya/MNodeMessage.h>
#include <maya/MPxLocatorNode.h>

#include <maya/MString.h>

class DLLEXPORT CArnoldStandInsTranslator
   :   public CGeoTranslator
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

#endif /* STANDINS_H_ */
