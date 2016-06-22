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
   AiParameterFLT("inFloat", 1.0f);

   AiMetaDataSetStr(mds, NULL, "maya.name", "floatConstant");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x81620);
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
   sg->out.FLT = AiShaderEvalParamFlt(p_inFloat);
}
