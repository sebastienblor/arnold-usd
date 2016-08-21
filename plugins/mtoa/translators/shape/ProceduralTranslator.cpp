#include "ProceduralTranslator.h"
#include "../NodeTranslatorImpl.h"

#include "render/RenderSession.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "scene/MayaScene.h"

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

CArnoldProceduralTranslator::CArnoldProceduralTranslator()  :
   CShapeTranslator()
{
   // Just for debug info, translator creates whatever arnold nodes are required
   // through the CreateArnoldNodes method
   m_impl->m_abstract.arnold = "procedural";
}

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

AtByte CArnoldProceduralTranslator::ComputeOverrideVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == GetSession()->IsRenderablePath(m_dagPath))
      return AI_RAY_UNDEFINED;

   AtByte visibility = AI_RAY_ALL;
   MPlug plug;

   return visibility;
}

void CArnoldProceduralTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else
   {
      ExportProcedural(anode, IsExported());
   }
}

void CArnoldProceduralTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CArnoldProceduralTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());

   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   AtByte visibility = AiNodeGetByte(masterNode, "visibility");
   AiNodeSetByte(instance, "visibility", visibility);

   m_DagNode.setObject(masterInstance);
   
   if (m_DagNode.findPlug("overrideShaders").asBool() &&
      ((CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER)
       || CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines()))
   {
      ExportStandinsShaders(instance);
   }
   if (m_DagNode.findPlug("overrideLightLinking").asBool())
   {
      ExportLightLinking(instance);
   }

   return instance;
}

void CArnoldProceduralTranslator::ExportShaders()
{
   AiMsgWarning( "[mtoa] Shaders untested with new multitranslator and standin code.");
   /// TODO: Test shaders with standins.
   ExportStandinsShaders(GetArnoldRootNode());
}

void CArnoldProceduralTranslator::ExportStandinsShaders(AtNode* procedural)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportNode(shadingGroupPlug);
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

   // my shaders are now up-to-date
   m_updateShaders = false;
}

void CArnoldProceduralTranslator::ExportBoundingBox(AtNode* procedural)
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


AtNode* CArnoldProceduralTranslator::ExportProcedural(AtNode* procedural, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(procedural, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(procedural);
   ProcessRenderFlags(procedural);
   ExportStandinsShaders(procedural);
   ExportLightLinking(procedural);
   MString dso = m_DagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
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
      
   GetSession()->FormatProceduralPath(resolvedName);
   AiNodeSetStr(procedural, "dso", resolvedName.asChar());

   MPlug deferStandinLoad = m_DagNode.findPlug("deferStandinLoad");
   if (!deferStandinLoad.asBool())
       AiNodeSetBool(procedural, "load_at_init", true);
   else
       ExportBoundingBox(procedural);

   MPlug data = m_DagNode.findPlug("data");
   int sizeData = strlen(data.asString().asChar());
   if (sizeData != 0)
   {
       AiNodeSetStr(procedural, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
   }

   return procedural;
}

// FIXME we should be able to remove this as arnold core is starting to support procedurals update
// however several crashes are happening due to shaders creation / deletion

// If we want to port that to standins we should either copy this in CStandinTranslator,
// or better make CStandinTranslator inherit from Procedural as it should...

void CArnoldProceduralTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   // procedurals need to clear and re-export at next update (ticket #2314)
   
   AtNode *rootNode = GetArnoldRootNode();
   if (rootNode != NULL)
   {
      const AtParamEntry *pe = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(rootNode), "allow_updates");
      bool allowUpdates = (pe != NULL && AiNodeGetBool(rootNode, "allow_updates") == true);
      allowUpdates |= (AiNodeLookUpUserParameter(rootNode, "allow_updates") ? AiNodeGetBool(rootNode, "allow_updates") : false); 

      if (allowUpdates)
      {
         if (m_impl->m_holdUpdates) return;
         // check if user data exists
         if(s_idleCallback == 0)
         {
            MStatus status;
            s_idleCallback = MEventMessage::addEventCallback("idle",
               CArnoldProceduralTranslator::IdleCallback, (void*)NULL, &status);
         }
         s_updatedProcedurals.push_back(this);
         m_impl->m_holdUpdates = true;
         return;

      }
   }

   // call parent function
   CShapeTranslator::NodeChanged(node, plug);
}


void CArnoldProceduralTranslator::IdleCallback(void *data)
{
   if(s_idleCallback)
   {
      MMessage::removeCallback(s_idleCallback);
      s_idleCallback = 0;
   }

   std::vector<CArnoldProceduralTranslator *> updateProcs = s_updatedProcedurals;
   s_updatedProcedurals.clear();

   for (size_t i = 0; i < updateProcs.size(); ++i)
   {
      CArnoldProceduralTranslator *translator = updateProcs[i];
      
      if (translator == NULL) continue;
      translator->SetUpdateMode(AI_RECREATE_NODE);
      translator->m_impl->m_holdUpdates = false;
      translator->RequestUpdate();
   }
}

