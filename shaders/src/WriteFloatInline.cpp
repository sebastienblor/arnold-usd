#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteFloatInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteFloatInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterFLT("input", 0.0f);
   AiParameterSTR("aov_name", "");
   AiParameterARRAY("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.FLT = AiShaderEvalParamFlt(p_input);
   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetFlt(sg, AiShaderEvalParamStr(p_name), sg->out.FLT);
}

node_initialize
{
}

node_update
{
   AtArray* sets = (AtArray*)AiNodeGetLocalData(node);
   if (sets != NULL)
      AiArrayDestroy(sets);
   AtArray* setNames = AiNodeGetArray(node, "sets");
   std::vector<AtNode*> setNodes;
   for (unsigned int i=0; i < setNames->nelements; i++)
   {
      const char* nodeName = AiArrayGetStr(setNames, i);
      AtNode* setNode = AiNodeLookUpByName(nodeName);
      if (setNode != NULL)
         setNodes.push_back(setNode);
   }
   sets = AiArrayConvert((int)setNodes.size(), 1, AI_TYPE_NODE, &setNodes[0]);
   AiNodeSetLocalData(node, sets);
}

node_finish
{
   AiArrayDestroy((AtArray*)AiNodeGetLocalData(node));
}
