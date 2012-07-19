#include "ShapeTranslator.h"

#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

#include <algorithm>

#include <maya/MPlugArray.h>

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;

   ProcessParameter(node, "self_shadows", AI_TYPE_BOOLEAN, "aiSelfShadows");
   ProcessParameter(node, "opaque", AI_TYPE_BOOLEAN, "aiOpaque");
   ProcessParameter(node, "receive_shadows", AI_TYPE_BOOLEAN, "receiveShadows");
   ProcessParameter(node, "sss_sample_distribution", AI_TYPE_INT, "aiSssSampleDistribution");
   ProcessParameter(node, "sss_sample_spacing", AI_TYPE_FLOAT, "aiSssSampleSpacing");
}


void CShapeTranslator::ExportLightLinking(AtNode* shape)
{
   MFnDependencyNode dNode(m_dagPath.node());
   CArnoldSession* session = GetSession();
   session->ExportLightLinking(shape, dNode);
   return;
}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   MString nodeType = helper.GetMayaNodeTypeName();
   AiMsgDebug("[mtoa] Creating common Arnold shape attributes on Maya \"%s\" nodes", nodeType.asChar());

   helper.MakeInput("sss_sample_distribution");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   MakeArnoldVisibilityFlags(helper);
}

AtNode* CShapeTranslator::CreateShadingGroupShader(AtNode *rootShader, std::vector<AtNode*> &aovShaders)
{
   // insert shading group shader to evaluate extra AOV inputs
   AtNode* shadingEngine = AiNode("MayaShadingEngine");

   AddAOVDefaults(shadingEngine, aovShaders);

   AiNodeSetStr(shadingEngine, "name", (GetMayaNodeName() + "@SG").asChar());
   AiNodeLink(rootShader, "beauty", shadingEngine);
   return shadingEngine;
}

// called for shaders connected directly to shapes
AtNode* CShapeTranslator::ExportRootShader(const MPlug& plug)
{
   return ExportRootShader(ExportNode(plug));
}

// called for root shaders that have already been created
AtNode* CShapeTranslator::ExportRootShader(AtNode *rootShader)
{
   std::vector<AtNode*> aovShaders;
   return CreateShadingGroupShader(rootShader, aovShaders);
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
