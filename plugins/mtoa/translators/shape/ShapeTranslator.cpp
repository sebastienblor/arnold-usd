#include "ShapeTranslator.h"

#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

#include <algorithm>

#include <maya/MPlugArray.h>

#include <common/UtilityFunctions.h>

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

// http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed

union DJB2HashUnion{
   unsigned int hash;
   int hashInt;
};

int DJB2Hash(unsigned char *str)
{
   DJB2HashUnion hashUnion;
   hashUnion.hash = 5381;
   int c;

   while ((c = *str++))
      hashUnion.hash = ((hashUnion.hash << 5) + hashUnion.hash) + c; /* hash * 33 + c */

   return hashUnion.hashInt;
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
      AiNodeSetInt(node, "id", DJB2Hash((unsigned char*)dnode.name().asChar()));
}


void CShapeTranslator::ExportLightLinking(AtNode* shape)
{
   CArnoldSession* session = GetSession();
   session->ExportLightLinking(shape, m_dagPath);
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
AtNode* CShapeTranslator::ExportRootShader(const MPlug& plug, CNodeTranslator** outTranslator)
{
   return ExportRootShader(ExportNode(plug, true, outTranslator));
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
