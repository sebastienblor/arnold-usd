#include <ai.h>

namespace
{

enum TangentToObjectSpaceParams
{
   p_map = 0,
   p_scale
};

};

AI_SHADER_NODE_EXPORT_METHODS(TangentToObjectSpaceMtd);

node_parameters
{
   AiParameterRGB("map", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("scale", 1.0f, 1.0f, 1.0f);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtRGB col = AiShaderEvalParamRGB(p_map);
   AtVector scale = AiShaderEvalParamVec(p_scale);

   AtVector v, T, B;
   AtMatrix m;

   AiM4Identity(m);

   if (!AiUDataGetVec("tangent", &T) || !AiUDataGetVec("bitangent", &B))
   {
      // Cannot convert
      sg->out.VEC.x = 0.0f;
      sg->out.VEC.y = 0.0f;
      sg->out.VEC.z = 0.0f;
      return;
   }

   m[0][0] = T.x;
   m[0][1] = T.y;
   m[0][2] = T.z;
   m[1][0] = B.x;
   m[1][1] = B.y;
   m[1][2] = B.z;
   m[2][0] = sg->N.x;
   m[2][1] = sg->N.y;
   m[2][2] = sg->N.z;

   AiV3Create(v, col.r * scale.x, col.b * scale.z, col.g * scale.y);

   AiM4VectorByMatrixMult(&(sg->out.VEC), m, &v);
}
