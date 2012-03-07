#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WritePoint2InlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WritePoint2InlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterPnt2("input", 0.0f, 0.0f);
   AiParameterSTR("aov_name", "");
   AiParameterARRAY("sets", AiArray(0, 0, AI_TYPE_STRING));
}

shader_evaluate
{
   sg->out.PNT2 = AiShaderEvalParamPnt2(p_input);

   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup((AtArray*)AiNodeGetLocalData(node), sg))
      AiAOVSetPnt2(sg, AiShaderEvalParamStr(p_name), sg->out.PNT2);
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
