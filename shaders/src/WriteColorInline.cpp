#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteColorInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteColorInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterRGBA("input", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterStr("aov_name", "");
   AiParameterArray("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.RGBA() = AiShaderEvalParamRGBA(p_input);

   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetRGBA(sg, AtString(AiShaderEvalParamStr(p_name)), sg->out.RGBA());
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
