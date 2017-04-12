#include <ai.h>
#include <algorithm>


AI_SHADER_NODE_EXPORT_METHODS(MayaBump2DMtd);

namespace MSTR
{
   static const AtString tangent("tangent");
   static const AtString bitangent("bitangent");
}

static const char* useAsNames[] = {"bump", "tangent_normal", "object_normal", 0}; 

enum mayaBump2DModes{
   BM_BUMP = 0,
   BM_TANGENT_NORMAL,
   BM_OBJECT_NORMAL
};

node_parameters
{
   AiParameterFlt("bump_map", 0.f);
   AiParameterFlt("bump_height", 1.f);
   AiParameterRGB("normal_map", 0.f, 0.f, 1.f);
   AiParameterBool("flip_r", true);
   AiParameterBool("flip_g", true);
   AiParameterBool("swap_tangents", false);
   AiParameterBool("use_derivatives", true);
   AiParameterEnum("use_as", 0, useAsNames)
   AiParameterVec("normal", 0.f, 1.f, 0.f);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);

   AiMetaDataSetStr(nentry, NULL, "_synonym", "mayaBump2D");
}

enum mayaBump2DParams {
   p_bump_map,
   p_bump_height,
   p_normal_map,
   p_flip_r,
   p_flip_g,
   p_swap_tangents,
   p_use_derivatives,
   p_use_as,
   p_normal
};

struct mayaBump2DData{
   AtRGBA shaderValue;
   AtNode* bumpMap;
   bool hasChainedBump;
   int bumpMode;
   bool flipR, flipG, swapTangents, useDerivatives;
};

node_initialize
{
   mayaBump2DData* data = (mayaBump2DData*)AiMalloc(sizeof(mayaBump2DData));
   data->bumpMap = 0;
   data->bumpMode = 0;
   data->flipR = true;
   data->flipG = true;
   data->swapTangents = false;
   data->useDerivatives = true;
   data->hasChainedBump = false;
   AiNodeSetLocalData(node, data);
}

node_update
{
   mayaBump2DData* data = (mayaBump2DData*)AiNodeGetLocalData(node);
   data->bumpMode = AiNodeGetInt(node, "use_as");
   data->bumpMap = AiNodeGetLink(node, "bump_map");
   
   AtNode* options = AiUniverseGetOptions();
   if (AiNodeGetBool(options, "ignore_bump"))
      data->bumpMap = 0;
   
   data->flipR = AiNodeGetBool(node, "flip_r");
   data->flipG = AiNodeGetBool(node, "flip_g");
   data->swapTangents = AiNodeGetBool(node, "swap_tangents");
   data->useDerivatives = AiNodeGetBool(node, "use_derivatives");
   
   data->hasChainedBump = AiNodeIsLinked(node, "normal");
}

node_finish
{
   
}

struct BumpEvalData{
   float bumpHeight;
   AtNode* bumpMap;
};

static float BumpFunction(AtShaderGlobals* sg, void* d)
{
   BumpEvalData* data = (BumpEvalData*)d;
   AiShaderEvaluate(data->bumpMap, sg);
   return (sg->out.FLT() - .5f) * data->bumpHeight;
}

shader_evaluate
{
   mayaBump2DData* data = (mayaBump2DData*)AiNodeGetLocalData(node);

   // If this same shader has bump, evaluate it first so that it 
   // serves as a basis for this bump
   if (data->hasChainedBump)
   {
      sg->Nf = sg->N = AiShaderEvalParamVec(p_normal);
      sg->Nf = AiFaceViewer(sg);
   }
   AtVector result = sg->N;
   if (sg->Op == 0)
   {
      return;
   }
   
   if (sg->Rt & AI_RAY_DIFFUSE_REFLECT || sg->Rt & AI_RAY_SHADOW || (data->bumpMode == BM_BUMP && data->bumpMap == 0))
   { 
      return;
   }
   

   if (data->bumpMode == BM_BUMP) // do classic bump mapping
   {
      const float bumpHeight = AiShaderEvalParamFlt(p_bump_height);
      if (fabsf(bumpHeight) > AI_EPSILON)
      {
         BumpEvalData evalData;
         evalData.bumpHeight = bumpHeight;
         evalData.bumpMap = data->bumpMap;
         result = AiShaderGlobalsEvaluateBump(sg, BumpFunction, &evalData);
         AiFaceForward(result, sg->N);
         result = -result;
      }
   }
   else if(data->bumpMode == BM_TANGENT_NORMAL) // tangent space normal mapping
   {
      AtRGB normalMap = AiShaderEvalParamRGB(p_normal_map);
      
      normalMap = (normalMap - .5f) * 2.f;
      AtVector tangent = sg->dPdu;
      AtVector bitangent = sg->dPdv;
      if (!data->useDerivatives)
      {
         AiUDataGetVec(MSTR::tangent, tangent);
         AiUDataGetVec(MSTR::bitangent, bitangent);
      }
      tangent = AiV3Normalize(tangent);
      bitangent = AiV3Normalize(bitangent);
      if (data->flipR)
         normalMap.r *= -1.f;
      if (data->flipG)
         normalMap.g *= -1.f;
      if (data->swapTangents)
         std::swap(tangent, bitangent);
      result = normalMap.r * tangent +
              normalMap.g * bitangent +
              normalMap.b * sg->N;
      result = AiV3Normalize(result);
      if (!AiV3IsFinite(result))
         result = sg->N;
   }
   else // object space normal mapping
   {
      AtRGB normalMap = AiShaderEvalParamRGB(p_normal_map);
      AtVector normalMapV(normalMap.r, normalMap.g, normalMap.b);
      result = AiM4VectorByMatrixMult(sg->M, normalMapV);
   }
   if (AiV3Dot(sg->Ng, sg->Rd) > 0.0f)
      result = -result;

   sg->out.VEC() = result;
}
