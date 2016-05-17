#include <ai.h>

namespace MSTR
{
   static const AtString tangent("tangent");
   static const AtString bitangent("bitangent");
}

AI_SHADER_NODE_EXPORT_METHODS(MayaVectorDisplacementMtd);

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
   AiMetaDataSetStr(nentry, NULL, "maya.name", "displacementShader");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52445348);

   AiParameterFlt("displacement", 0.0f);
   AiParameterVec("vectorDisplacement", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("scale", 1.0f);
   AiParameterEnum("vectorEncoding", 0, vector_encoding_enum);
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
   AtVector tangent = AiShaderEvalParamVec(p_tangent);
   
   AtVector normal = sg->N;
   
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
      AiM4VectorByMatrixMult(&transformedVectorDisplacement, sg->Minv, &vectorDisp);
      break;
   case VS_OBJECT:
      transformedVectorDisplacement = vectorDisp;
      break;
   case VS_TANGENT:
      AtVector T, B, N;
      
      N = AiV3Normalize(sg->N);

      if (!AiV3IsSmall(tangent))
      {
         T = AiV3Normalize(tangent);
         B = AiV3Cross(N,T);
      }
      else if (!AiUDataGetVec(MSTR::tangent, &T) || !AiUDataGetVec(MSTR::bitangent, &B))
      {
         if (!AiV3IsSmall(sg->dPdu) && !AiV3IsSmall(sg->dPdv))
         {
            // tangents available, use them
            T = AiV3Normalize(sg->dPdu - AiV3Dot(sg->dPdu, N) * N);
            B = AiV3Cross(N,T);
         }
         else
         {
            // no tangents given, compute a pair
            AiBuildLocalFramePolar(&T, &B, &sg->N);
         }
      }
      transformedVectorDisplacement = vectorDisp.x * T + vectorDisp.z * B + vectorDisp.y * N;
      break;
   }
   
   totalDisp += transformedVectorDisplacement;
   
   sg->out.VEC() = totalDisp * scale;
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
