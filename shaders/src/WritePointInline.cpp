#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WritePointInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WritePointInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterPNT("input", 0.0f, 0.0f, 0.0f);
   AiParameterSTR("aov_name", "");
   AiParameterARRAY("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.PNT = AiShaderEvalParamPnt(p_input);

   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetPnt(sg, AiShaderEvalParamStr(p_name), sg->out.PNT);
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
