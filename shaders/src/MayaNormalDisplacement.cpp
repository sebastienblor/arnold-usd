#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaNormalDisplacementMtd);

namespace
{

enum DisplacementParams
{
   p_displacement,
   p_vectorDisplacement,
   p_scale,
   p_vectorEncoding,
   p_vectorSpace,
   p_tangent,
   p_zeroValue
};

enum VectorEncodingType
{
   VE_ABSOLUTE = 0,
   VE_SIGNED
};

const char* vector_encoding_enum[] =
{
   "absolute",
   "signed",
   NULL
};

enum VectorSpaceType
{
   VS_WORLD = 0,
   VS_OBJECT,
   VS_TANGENT
};

const char* vector_space_enum[] =
{
   "world",
   "object",
   "tangent",
   NULL
};

};

node_parameters
{
   AiMetaDataSetStr(nentry, NULL, "maya.name", "displacementShader");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52445348);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);

   AiParameterFlt("displacement", 0.0f);
   AiParameterVec("vectorDisplacement", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("scale", 1.0f);
   AiParameterEnum("vectorEncoding", 0, vector_encoding_enum);
   AiParameterEnum("vectorSpace", 1, vector_space_enum);
   AiParameterVec("tangent", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("zeroValue", 0.0f);
}

shader_evaluate
{
   float disp = AiShaderEvalParamFlt(p_displacement);
   float scale = AiShaderEvalParamFlt(p_scale);
   float zeroValue = AiShaderEvalParamFlt(p_zeroValue);
   
   sg->out.FLT() = (disp - zeroValue) * scale;
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
