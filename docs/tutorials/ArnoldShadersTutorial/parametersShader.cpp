#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ParametersShaderMtd);

namespace
{

enum ParametersShaderParams { p_int, p_uint, p_bool, p_float,
                              p_RGB, p_vector, p_vector2,
                              p_string, p_matrix, p_enum };

const char* enum_list[] =
{
   "First value",
   "Second value",
   "Third value",
   NULL
};

};
                              
node_parameters
{
   AtMatrix id;
   AiM4Identity(id);
   
   AiParameterInt("IntParam", 0);
   AiParameterUInt("UIntParam", 0);
   AiParameterBool("BoolParam", 0);
   AiParameterFlt("FltParam", 0.0f);
   AiParameterRGB("RGBParam", 1.0f, 0.0f, 0.0f);
   AiParameterVec("VecParam", 1.0f, 1.0f, 1.0f);
   AiParameterVec2("Vec2Param", 0.7f, 0.7f);
   AiParameterStr("StrParam", "");
   AiParameterMtx("MtxParam", id);
   AiParameterEnum("EnumParam", 0, enum_list);
   AiParameterArray("ArrayParam", AiArray(0, 0, AI_TYPE_RGB));
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
   AtColor color = AiShaderEvalParamRGB(p_RGB);
   sg->out.RGB = color;
}