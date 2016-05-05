#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteVectorInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteVectorInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterVec("input", 0.0f, 0.0f, 0.0f);
   AiParameterStr("aov_name", "");
   AiParameterArray("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.VEC() = AiShaderEvalParamVec(p_input);

   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetVec(sg, AtString(AiShaderEvalParamStr(p_name)), sg->out.VEC());
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
