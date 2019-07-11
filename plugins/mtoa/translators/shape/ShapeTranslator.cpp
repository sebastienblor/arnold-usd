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
         if (AiNodeLookUpUserParameter(node, "sss_setname") == NULL)
            AiNodeDeclareConstant(node, "sss_setname", AI_TYPE_STRING);
         AiNodeSetStr(node, "sss_setname", setname.asChar());
      }
   }   
   plug = FindMayaPlug("aiToonId", &status);
   if (status && !plug.isNull())
   {
      MString toonId = plug.asString();
      if (toonId != "")
      {
         if (AiNodeLookUpUserParameter(node, "toon_id") == NULL)
            AiNodeDeclareConstant(node, "toon_id", AI_TYPE_STRING);
         AiNodeSetStr(node, "toon_id", toonId.asChar());
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

   // eventually export dcc_name user attribute
   // FIXME this could be moved to a separate function but this might be temporary
   if (GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(node, "dcc_name") == NULL)
         AiNodeDeclare(node, "dcc_name", "constant STRING");
   
      MString partialName = m_dagPath.partialPathName();
      AiNodeSetStr(node, "dcc_name", AtString(partialName.asChar()));
   }

   if (!GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(node, "maya_full_name") == NULL)
         AiNodeDeclare(node, "maya_full_name", "constant STRING");
   
      MString fullName = m_dagPath.fullPathName();
      AiNodeSetStr(node, "maya_full_name", AtString(fullName.asChar()));
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
   data.keyable = false;
   data.channelBox = false;
   data.type = AI_TYPE_STRING;
   
   helper.MakeInput(data);
   
   data.stringDefault = "";
   data.name = "aiSssSetname";
   data.shortName = "ai_sss_setname";
   data.keyable = false;
   data.channelBox = false;
   data.type = AI_TYPE_STRING;
   helper.MakeInput(data);

   data.stringDefault = "";
   data.name = "aiToonId";
   data.shortName = "ai_toon_id";
   data.keyable = false;
   data.channelBox = false;
   data.type = AI_TYPE_STRING;
   helper.MakeInput(data);

   MakeArnoldVisibilityFlags(helper);
}


// called for root shaders that have already been created
void CShapeTranslator::SetRootShader(AtNode *rootShader)
{
   AiMsgWarning("[mtoa] SetRootShader function is deprecated, please set the shader with AiNodeSetPtr");
   AtNode *shape = GetArnoldNode();
   if (shape != NULL)
      AiNodeSetPtr(shape, "shader", rootShader);
}


MPlug CShapeTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{   
   MPlug shadingGroupPlug;
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MPlug sgPlug = connections[k];
      if (sgPlug.node().apiType() == MFn::kShadingEngine)
      {
         // this is my Shading Engine plug. 
         // However, what I want now is the corresponding shader
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
