#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatConditionMtd);

namespace
{

enum ldkFloatConditionParams
{
   p_floatA,
   p_floatB,
   p_condition
};

};

node_parameters
{
   AiParameterFlt("floatA", 1.0f);
   AiParameterFlt("floatB", 1.0f);
   AiParameterBool("condition", false);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "floatCondition");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x816832 );
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
   sg->out.FLT() = AiShaderEvalParamBool(p_condition) ? 
      AiShaderEvalParamFlt(p_floatA) :
      AiShaderEvalParamFlt(p_floatB);
}