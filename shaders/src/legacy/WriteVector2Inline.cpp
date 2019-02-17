#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteVector2InlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteVector2InlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetStr(nentry, NULL, "_synonym", "aovWriteVector2");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.false", false);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);


   AiParameterVec2("input", 0.0f, 0.0f);
   AiParameterStr("aov_name", "");
   AiParameterArray("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.VEC2() = AiShaderEvalParamVec2(p_input);

   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetVec2(sg, AtString(AiShaderEvalParamStr(p_name)), sg->out.VEC2());
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
