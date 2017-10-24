#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatMaskMtd);

namespace
{

enum ldkFloatMaskParams
{
   p_inFloat,
   p_mask
};

};

node_parameters
{
   AiParameterFlt("inFloat", 1.0f);
   AiParameterFlt("mask", 0.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "floatMask");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81624);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   const float inFloat = AiShaderEvalParamFlt(p_inFloat);
   const float mask = AiShaderEvalParamFlt(p_mask);

   sg->out.FLT() = inFloat;
   if (mask > 0.0f)
   {
      sg->out.FLT() -= mask;
   }
}
