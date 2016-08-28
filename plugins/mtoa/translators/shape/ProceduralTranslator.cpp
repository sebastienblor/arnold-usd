#include "ProceduralTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MEventMessage.h>

#include <maya/MString.h>

static MCallbackId s_idleCallback = 0;
static std::vector<CArnoldProceduralTranslator *> s_updatedProcedurals;

void CArnoldProceduralTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "deferStandinLoad";
   data.shortName = "deferStandinLoad";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.name = "dso";
   data.shortName = "dso";
   data.channelBox = false;
   helper.MakeInputString(data);

   data.name = "data";
   data.shortName = "data";
   data.channelBox = false;
   helper.MakeInputString(data);

}

AtNode* CArnoldProceduralTranslator::CreateArnoldNodes()
{
   if (IsMasterInstance())
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

/* Commented this out as nobody calls this function. 
   Was this removed by mistake ?? Should this be called by ProcessRenderFlags ??

AtByte CArnoldProceduralTranslator::ComputeOverrideVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (!IsRenderable())
      return AI_RAY_UNDEFINED;

   AtByte visibility = AI_RAY_ALL;
   MPlug plug;

   return visibility;
}
*/

// Moved both ExportProcedural and ExportInstance here, for API simplicity
void CArnoldProceduralTranslator::Export(AtNode* node)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(node));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      // Export the instance. 
      ExportInstance(node);
   }
   else
   {
      // Export the procedural
      MFnDagNode dagNode(m_dagPath.node());
      AiNodeSetStr(node, "name", m_dagPath.partialPathName().asChar());
      ExportMatrix(node);
      ProcessRenderFlags(node);
      ExportShaders();
      ExportLightLinking(node);
      MString dso = dagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
      MString resolvedName = dso.asChar();

      unsigned int nchars = resolvedName.numChars();
      if (nchars > 3 && resolvedName.substringW(nchars-3, nchars) == ".so")
      {
          resolvedName = resolvedName.substringW(0, nchars-4)+LIBEXT;
      }
      else if (nchars > 4 && resolvedName.substringW(nchars-4, nchars) == ".dll")
      {
          resolvedName = resolvedName.substringW(0, nchars-5)+LIBEXT;
      }
      else if (nchars > 6 && resolvedName.substringW(nchars-6, nchars) == ".dylib")
      {
          resolvedName = resolvedName.substringW(0, nchars-7)+LIBEXT;
      }
         
      GetSessionOptions().FormatProceduralPath(resolvedName);
      AiNodeSetStr(node, "dso", resolvedName.asChar());

      MPlug deferStandinLoad = dagNode.findPlug("deferStandinLoad");
      if (!deferStandinLoad.asBool())
          AiNodeSetBool(node, "load_at_init", true);
      else
          ExportBoundingBox(node);

      MPlug data = dagNode.findPlug("data");
      int sizeData = strlen(data.asString().asChar());
      if (sizeData != 0)
      {
          AiNodeSetStr(node, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
      }
   }
}

void CArnoldProceduralTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);
}

void CArnoldProceduralTranslator::ExportInstance(AtNode *instance)
{
   MDagPath masterInstance = GetMasterInstance();
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());
   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   AtByte visibility = AiNodeGetByte(masterNode, "visibility");
   AiNodeSetByte(instance, "visibility", visibility);

   MFnDagNode dagNode(masterInstance);
   
   if (dagNode.findPlug("overrideShaders").asBool() &&
      RequiresShaderExport())
   {
      ExportShaders();
   }
   if (dagNode.findPlug("overrideLightLinking").asBool())
   {
      ExportLightLinking(instance);
   }      

}
void CArnoldProceduralTranslator::ExportShaders()
{
   AtNode *procedural = GetArnoldNode();
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(procedural, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), MFnDependencyNode(shadingGroupPlug.node()).name().asChar());
         /*AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input.",
               fnDGNode.name().asChar());*/
         AiNodeSetPtr(procedural, "shader", NULL);
      }
      if (meshShaders.size() > 0)
      {
         AiNodeDeclare(procedural, "mtoa_shading_groups", "constant ARRAY NODE");
         AiNodeSetArray(procedural, "mtoa_shading_groups",
                        AiArrayConvert(meshShaders.size(), 1, AI_TYPE_NODE, &(meshShaders[0])));
      }
   }

}

void CArnoldProceduralTranslator::ExportBoundingBox(AtNode *procedural)
{
   MFnDagNode dagNode(m_dagPath.node());

   MBoundingBox boundingBox = dagNode.boundingBox();
   MPoint bbMin = boundingBox.min();
   MPoint bbMax = boundingBox.max();

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   AiNodeSetPnt(procedural, "min", minCoords[0], minCoords[1], minCoords[2]);
   AiNodeSetPnt(procedural, "max", maxCoords[0], maxCoords[1], maxCoords[2]);
}


