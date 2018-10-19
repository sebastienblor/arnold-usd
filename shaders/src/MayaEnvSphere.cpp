#include <ai.h>

namespace
{

enum MayaEnvSphereParams
{
   p_image,
   //p_shearUV,
   p_flip,
   p_matrix
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaEnvSphereMtd);

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterRGB("image", 0, 0, 0);
   //AiParameterVec2("shearUV", 0, 0);
   AiParameterBool("flip", false);
   AiParameterMtx("placementMatrix", id);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "envSphere");
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
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
   AtVector rdir = AiReflectSafe(sg->Rd, sg->Nf, sg->Ng);

   AtMatrix *matrix = AiShaderEvalParamMtx(p_matrix);
   rdir = AiM4VectorByMatrixMult(*matrix, rdir);
   rdir = AiM4VectorByMatrixMult(AiM4RotationX(90.0), rdir);
   rdir = AiM4VectorByMatrixMult(AiM4RotationZ(90.0), rdir);

   // copy original globals
   float inU = sg->u;
   float inV = sg->v;

//   sg->dudx = outDuDx;
//   sg->dudy = outDuDy;
//   sg->dvdx = outDvDx;
//   sg->dvdy = outDvDy;

   // replace globals
   float *pU;
   float *pV;
   float R = sqrt((rdir.x*rdir.x) + (rdir.y*rdir.y) + (rdir.z*rdir.z));
   if (AiShaderEvalParamBool(p_flip))
   {
      // flip U and V
      pU = &sg->v;
      pV = &sg->u;
   }
   else
   {
      pU = &sg->u;
      pV = &sg->v;
   }
   *pU = (0.5f * (atanf(rdir.y/rdir.x) / (float)AI_PI));
   if      (rdir.y > 0.0f && rdir.x > 0.0f) *pU = *pU + .5f;
   else if (rdir.y < 0.0f && rdir.x > 0.0f) *pU = *pU + .5f;
   *pV = 1 - (acosf(rdir.z/R) / (float)AI_PI);

   // call input
   sg->out.RGB() = AiShaderEvalParamRGB(p_image);

   // restore globals
   sg->u = inU;
   sg->v = inV;
//   sg->dudx = inDuDx;
//   sg->dudy = inDuDy;
//   sg->dvdx = inDvDx;
//   sg->dvdy = inDvDy;
}
