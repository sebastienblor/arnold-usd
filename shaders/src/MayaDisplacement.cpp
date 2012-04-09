#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaDisplacementMtd);

namespace
{

enum DisplacementParams
{
   p_displacement,
   p_vectorDisplacement,
   p_scale,
   p_vectorEncoding,
   p_vectorSpace,
   p_tangent
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
   AiMetaDataSetStr(mds, NULL, "maya.name", "displacementShader");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52445348);

   AiParameterFlt("displacement", 0.0f);
   AiParameterVec("vectorDisplacement", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("scale", 1.0f);
   AiParameterEnum("vectorEncoding", 1, vector_encoding_enum);
   AiParameterEnum("vectorSpace", 1, vector_space_enum);
   AiParameterVec("tangent", 0.0f, 0.0f, 0.0f);
}

shader_evaluate
{
   float disp = AiShaderEvalParamFlt(p_displacement);
   AtVector vectorDisp = AiShaderEvalParamVec(p_vectorDisplacement);
   int vectorEncoding = AiShaderEvalParamInt(p_vectorEncoding);
   int vectorSpace = AiShaderEvalParamInt(p_vectorSpace);
   float scale = AiShaderEvalParamFlt(p_scale);
   
   AtVector normal = sg->Nf;
   
   AtVector totalDisp = normal * disp;
   
   AtVector transformedVectorDisplacement = AI_V3_ZERO;
   if (vectorEncoding == VE_SIGNED)
   {
      vectorDisp *= 2.0f;
      vectorDisp -= 1.0f;
   }
   switch (vectorSpace)
   {
   case VS_WORLD:
      AiM4VectorByMatrixTMult(&transformedVectorDisplacement, sg->Minv, &vectorDisp);
      break;
   case VS_OBJECT:
      transformedVectorDisplacement = vectorDisp;
      break;
   case VS_TANGENT:

      AtVector v, T, B;
      AtMatrix m;

      AiM4Identity(m);

      if (!AiUDataGetVec("tangent", &T) || !AiUDataGetVec("bitangent", &B))
      {
         // Cannot convert
         transformedVectorDisplacement.x = 0.0f;
         transformedVectorDisplacement.y = 0.0f;
         transformedVectorDisplacement.z = 0.0f;
         return;
      }

      m[0][0] = T.x;
      m[0][1] = T.y;
      m[0][2] = T.z;
      m[1][0] = B.x;
      m[1][1] = B.y;
      m[1][2] = B.z;
      m[2][0] = sg->Nf.x;
      m[2][1] = sg->Nf.y;
      m[2][2] = sg->Nf.z;

      AiV3Create(v, vectorDisp.x, vectorDisp.z, vectorDisp.y);

      AiM4VectorByMatrixMult(&(transformedVectorDisplacement), m, &v);
      
      break;
   }
   
   totalDisp += transformedVectorDisplacement;
   
   sg->out.VEC = totalDisp * scale;
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
