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
   AiMetaDataSetStr(nentry, NULL, "_synonym", "aovWriteFloat");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.false", false);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);

   AiParameterFlt("input", 0.0f);
   AiParameterStr("aov_name", "");
   AiParameterArray("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.FLT() = AiShaderEvalParamFlt(p_input);
   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetFlt(sg, AtString(AiShaderEvalParamStr(p_name)), sg->out.FLT());
}

node_initialize
{
}

node_update
{
   AtArray* sets = (AtArray*)AiNodeGetLocalData(node);
   if (sets != NULL)
      AiArrayDestroy(sets);
   sets = StringArrayToNodeArray(AiNodeGetArray(node, "sets"));
   AiNodeSetLocalData(node, sets);
}

node_finish
{
   AiArrayDestroy((AtArray*)AiNodeGetLocalData(node));
}