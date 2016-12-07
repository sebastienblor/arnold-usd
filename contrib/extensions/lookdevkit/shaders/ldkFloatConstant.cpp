#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatConstantMtd);

namespace
{

enum ldkFloatMathParams
{
   p_inFloat
};

};

node_parameters
{
   AiParameterFlt("inFloat", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "floatConstant");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81620);
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
   sg->out.FLT() = AiShaderEvalParamFlt(p_inFloat);
}
