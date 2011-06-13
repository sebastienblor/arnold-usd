#include "StandinsTranslator.h"

#include "render/RenderSession.h"
#include "attributes/AttrHelper.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>


#include <maya/MString.h>

void CArnoldStandInsTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "procedural");

}

AtNode* CArnoldStandInsTranslator::CreateArnoldNodes()
{
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
   {
      AtNode * tmpRes = AddArnoldNode("procedural");
      return  tmpRes;
   }
   else
   {
      AtNode * tmpRes = AddArnoldNode("ginstance");
      return  tmpRes;
   }
}


void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(anode->base_node);
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, m_masterDag);
   }
   else
   {
      ExportProcedural(anode, false);
   }
}

void CArnoldStandInsTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   ExportMatrix(anode, step);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CArnoldStandInsTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.fullPathName().asChar());
   int instanceNum = m_dagPath.instanceNumber();

   AiNodeSetStr(instance, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(instance, 0);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);

   //
   // SHADERS
   //
   MFnMesh standInNode(m_dagPath.node());
   MObjectArray shaders, shadersMaster;
   MIntArray indices, indicesMaster;

   standInNode.getConnectedShaders(instanceNum, shaders, indices);
   // FIXME: it is incorrect to assume that instance 0 is the master as it may be hidden (chad)
   standInNode.getConnectedShaders(0, shadersMaster, indicesMaster);

   return instance;
}

void CArnoldStandInsTranslator::ExportShaders()
{
   AiMsgWarning( "[mtoa] Shaders untested with new multitranslator and standin code.");
   /// TODO: Test shaders with standins.
   ExportStandinsShaders(GetArnoldRootNode());
}

void CArnoldStandInsTranslator::ExportStandinsShaders(AtNode* procedural)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      MPlugArray connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
      shaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // shader assigned to node
         AtNode* shader = m_scene->ExportShader(connections[0].node());

         AiNodeSetPtr(procedural, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
         AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input.",
               fnDGNode.name().asChar());
   }
   //
   // DISPLACEMENT
   //
   // currently does not work for per-face assignment
   if (!shadingGroup.isNull())
   {
      MPlugArray connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("displacementShader");
      shaderPlug.connectedTo(connections, true, false);

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         MFnDependencyNode dispNode(connections[0].node());

         // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
         // it only influences the computation of the displacement bounds
         AiNodeSetFlt(procedural, "disp_height", dispNode.findPlug("disp_height").asFloat());
         AiNodeSetFlt(procedural, "disp_zero_value", dispNode.findPlug("disp_zero_value").asFloat());
         AiNodeSetBool(procedural, "autobump", dispNode.findPlug("autobump").asBool());

         connections.clear();
         dispNode.findPlug("disp_map").connectedTo(connections, true, false);

         if (connections.length() > 0)
         {
            MString attrName = connections[0].partialName(false, false, false, false, false, true);
            AtNode* dispImage(m_scene->ExportShader(connections[0].node(), attrName));

            MPlug pVectorDisp = dispNode.findPlug("vector_displacement", false);
            if (!pVectorDisp.isNull() && pVectorDisp.asBool())
            {
               AtNode* tangentToObject = AiNode("tangentToObjectSpace");
               m_scene->ProcessShaderParameter(dispNode, "vector_displacement_scale",
                     tangentToObject, "scale", AI_TYPE_VECTOR);
               AiNodeLink(dispImage, "map", tangentToObject);

               AiNodeSetPtr(procedural, "disp_map", tangentToObject);
            }
            else
            {
               AiNodeSetPtr(procedural, "disp_map", dispImage);
            }
         }
      }
   }
}

void CArnoldStandInsTranslator::ExportBoundingBox(AtNode* procedural)
{
   MBoundingBox boundingBox = m_DagNode.boundingBox();
   MPoint bbMin = boundingBox.min();
   MPoint bbMax = boundingBox.max();

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   AiNodeSetPnt(procedural, "min", minCoords[0], minCoords[1], minCoords[2]);
   AiNodeSetPnt(procedural, "max", maxCoords[0], maxCoords[1], maxCoords[2]);
}


AtNode* CArnoldStandInsTranslator::ExportProcedural(AtNode* procedural, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(procedural, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(procedural, 0);
   ProcessRenderFlags(procedural);
   ExportStandinsShaders(procedural);
   if (!update)
   {
      MString dso = m_DagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
      MString filename;

      int frame = m_DagNode.findPlug("frameNumber").asInt();
      int frameOffset = m_DagNode.findPlug("frameOffset").asInt();
      bool useFrameExtension = m_DagNode.findPlug("useFrameExtension").asBool();


      MString frameNumber = "0";

      frameNumber += frame + frameOffset;

      bool resolved = MRenderUtil::exactFileTextureName(dso, useFrameExtension, frameNumber,
            filename);

      if (resolved)
         AiNodeSetStr(procedural, "dso", filename.asChar());
      else
         AiNodeSetStr(procedural, "dso", dso.asChar());

      MPlug loadInit = m_DagNode.findPlug("loadAtInit");
      if (!loadInit.asBool())
         AiNodeSetBool(procedural, "load_at_init", true);
      else
         ExportBoundingBox(procedural);

      MPlug data = m_DagNode.findPlug("data");
      int sizeData = strlen(data.asString().asChar());
      if (sizeData != 0)
      {
         AiNodeSetStr(procedural, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
      }

   }
   return procedural;
}
