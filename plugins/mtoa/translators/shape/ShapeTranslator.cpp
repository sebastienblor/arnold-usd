#include "ShapeTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "scene/MayaScene.h"
#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

#include <algorithm>

#include <maya/MPlugArray.h>

#include <common/UtilityFunctions.h>
#include <utils/HashUtils.h>

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
   AiMsgDebug("[mtoa] Creating common Arnold shape attributes on Maya \"%s\" nodes", nodeType.asChar());

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
      std::vector<AtNode*> aovShaders;
      m_impl->AddAOVDefaults(shadingEngine, aovShaders);
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
   if ((msg & MNodeMessage::kConnectionMade) && (plug.partialName() == "iog"))
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
