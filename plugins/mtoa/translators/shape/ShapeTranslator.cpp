#include "ShapeTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "scene/MayaScene.h"
#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

#include <algorithm>

#include <maya/MPlugArray.h>

#include <common/UtilityFunctions.h>
#include <utils/HashUtils.h>
#include "utils/MtoaLog.h"

void CShapeTranslator::Init()
{
   CDagTranslator::Init();
   const CSessionOptions &session = GetSessionOptions();
   m_motion = session.IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);
   m_motionDeform = session.IsMotionBlurEnabled(MTOA_MBLUR_DEFORM);
}
bool CShapeTranslator::RequiresMotionData()
{
   // m_motion / m_motionDeform need to be updated here otherwise these variables might be outdated (#2660)
   const CSessionOptions &session = GetSessionOptions();
   m_motion = session.IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);
   m_motionDeform = session.IsMotionBlurEnabled(MTOA_MBLUR_DEFORM);

   return ((m_motion || m_motionDeform) && IsLocalMotionBlurEnabled());
}



void CShapeTranslator::ExportTraceSets(AtNode* node, const MPlug& traceSetsPlug)
{
   if (!traceSetsPlug.isNull())
   {
      MString traceSets = traceSetsPlug.asString();
      if (traceSets != "")
      {
         MStringArray traceSetArray;
         traceSets.split(' ', traceSetArray);
         const unsigned int numSets = traceSetArray.length();
         AtArray* array = AiArrayAllocate(numSets, 1, AI_TYPE_STRING);
         for (unsigned int i = 0; i < numSets; ++i)
            AiArraySetStr(array, i, traceSetArray[i].asChar());
         AiNodeSetArray(node, "trace_sets", array);
      }
   }
}

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetByte(node, "visibility", ComputeVisibility());

   MPlug plug;

   ProcessParameter(node, "self_shadows", AI_TYPE_BOOLEAN, "aiSelfShadows");
   ProcessParameter(node, "opaque", AI_TYPE_BOOLEAN, "aiOpaque");
   ProcessParameter(node, "matte", AI_TYPE_BOOLEAN, "aiMatte");
   ProcessParameter(node, "receive_shadows", AI_TYPE_BOOLEAN, "receiveShadows");
   MStatus status;
   plug = FindMayaPlug("aiSssSetname", &status);
   if (status && !plug.isNull())
   {
      MString setname = plug.asString();
      if (setname != "")
      {
         AiNodeDeclareConstant(node, "sss_setname", AI_TYPE_STRING);
         AiNodeSetStr(node, "sss_setname", setname.asChar());
      }
   }   
   
   ExportTraceSets(node, FindMayaPlug("aiTraceSets"));

   MFnDependencyNode dnode(m_dagPath.node(), &status);
   if (status)
      AiNodeSetUInt(node, "id",  getHash(node));

   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(node, "motion_start", (float)motionStart);
      AiNodeSetFlt(node, "motion_end", (float)motionEnd);
   }
}


void CShapeTranslator::ExportLightLinking(AtNode* shape)
{
   m_impl->m_session->ExportLightLinking(shape, m_dagPath);
   return;
}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   MString nodeType = helper.GetMayaNodeTypeName();
   
   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");
   helper.MakeInput("matte");
   
   CAttrData data;
   data.stringDefault = "";
   data.name = "aiTraceSets";
   data.shortName = "trace_sets";
   data.type = AI_TYPE_STRING;
   
   helper.MakeInput(data);
   
   data.stringDefault = "";
   data.name = "aiSssSetname";
   data.shortName = "ai_sss_setname";
   data.type = AI_TYPE_STRING;
   helper.MakeInput(data);

   MakeArnoldVisibilityFlags(helper);
}


// called for root shaders that have already been created
void CShapeTranslator::SetRootShader(AtNode *rootShader)
{
   // handle the situation where a previous rootShader was deleted
   if (rootShader == NULL)
   {
      AtNode *shape = GetArnoldNode();
      if (shape != NULL)
         AiNodeSetPtr(shape, "shader", NULL);

      return;
   }
   
   // First check if the internal SG node has already been created
   AtNode *shadingEngine = GetArnoldNode("SG");
   if (shadingEngine == NULL)
   {
      // register this AtNode in our Translator, so that it is properly cleared later
      shadingEngine = AddArnoldNode("MayaShadingEngine", "SG");
   }

   AiNodeLink(rootShader, "beauty", shadingEngine);

   AtNode *shape = GetArnoldNode();
   if (shape != NULL)
      AiNodeSetPtr(shape, "shader", shadingEngine);
}


MPlug CShapeTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MPlug sgPlug = connections[k];
      if (sgPlug.node().apiType() == MFn::kShadingEngine)
      {
         return sgPlug;
      }
   }
   return MPlug();
}


static void ShaderAssignmentCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*clientData)
{
   // Shading assignments are done with the instObjGroups attr, so we only
   // need to update when that is the attr that changes.

   MString plugName = plug.partialName();
   std::string plugNameStr = plugName.asChar();

   // Need to also consider the per-face shading asssignments that can return a plug.partialName()="iog[0].og[0]"
   bool isIog = (plugNameStr == "iog") || 
               ((plugNameStr.length() >= 9) && (plugNameStr.substr(0, 4) == "iog[") && (plugNameStr.find("].og") != std::string::npos));

   // We're only interested in connection changes. We had to add kConnectionBroken because of #3140
   if (isIog &&((msg & MNodeMessage::kConnectionMade) || (msg & MNodeMessage::kConnectionBroken)))
   {
      CShapeTranslator * translator = static_cast< CShapeTranslator* >(clientData);
      translator->RequestUpdate();
   }
}

void CShapeTranslator::AddUpdateCallbacks()
{
   CDagTranslator::AddUpdateCallbacks();

   MObject dagPathNode = m_dagPath.node();
   MStatus status;
   MCallbackId id = MNodeMessage::addAttributeChangedCallback(dagPathNode, ShaderAssignmentCallback, this, &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);
}

bool CShapeTranslator::RequiresShaderExport()
{
   CRenderOptions *renderOptions = CMayaScene::GetRenderSession()->RenderOptions();
   if (renderOptions == NULL) return false;
   return (renderOptions->outputAssMask() & AI_NODE_SHADER) ||
       renderOptions->forceTranslateShadingEngines();
}

AtNode *CShapeTranslator::ExportShadingGroup(const MPlug &shadingGroupPlug)
{
   if (!RequiresShaderExport())
      return NULL;

   AtNode *node = GetArnoldNode();
   bool volumeShading = false;
   static AtString polymeshStr("polymesh");
   if (node != NULL && AiNodeIs(node, polymeshStr))
      volumeShading = (AiNodeGetFlt(node, "step_size") > AI_EPSILON);

   CNodeTranslator *shadingGroupTranslator = NULL;
   AtNode *shadingEngineNode = m_impl->ExportConnectedNode(shadingGroupPlug, true, &shadingGroupTranslator);

   if (shadingGroupTranslator == NULL)
      return NULL;

   std::vector<AtNode*> aovShaders;
   AtNode* rootShader = NULL;
   MPlugArray        connections;

   MString shaderName = (volumeShading) ? "volumeShader" : "surfaceShader";
   MString aiShaderName =  (volumeShading) ? "aiVolumeShader" : "aiSurfaceShader";

   MPlug shaderPlug = shadingGroupTranslator->FindMayaPlug(aiShaderName);
   shaderPlug.connectedTo(connections, true, false);
   if (connections.length() == 0)
   {
      shaderPlug = shadingGroupTranslator->FindMayaPlug(shaderName);
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] CShadingEngineTranslator::Export found surfaceShader plug "+ shaderPlug.name());
      shaderPlug.connectedTo(connections, true, false);
   }
   CNodeTranslator* shaderNodeTranslator = 0;

   if (connections.length() > 0)
      rootShader = m_impl->ExportConnectedNode(connections[0], true, &shaderNodeTranslator);
   
   if (shadingEngineNode == NULL)
      return rootShader;

   // if Shading Engine translator returned a shader, this is the one we need to return
   // (it will be assigned as the mesh shader)
   // I just need to connect it to our rootShader
   AtNode *aovShader = shadingEngineNode;
   while(aovShader)
   {
      const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(aovShader), "passthrough");
      if (!paramEntry)
         return rootShader; // something didn't work right

      if (!AiNodeIsLinked(aovShader, "passthrough"))
      {
         AiNodeLink(rootShader, "passthrough", aovShader);
         break;
      }
      aovShader = AiNodeGetLink(aovShader, "passthrough");   // this aovWrite node is linked     
   }
   
   return shadingEngineNode;   
}

