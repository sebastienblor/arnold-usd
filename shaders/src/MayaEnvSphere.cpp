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
   AtMatrix id;
   AiM4Identity(id);

   AiParameterRGB("image", 0, 0, 0);
   //AiParameterVec2("shearUV", 0, 0);
   AiParameterBOOL("flip", false);
   AiParameterMTX("placementMatrix", id);

   AiMetaDataSetStr(mds, NULL, "maya.name", "envSphere");
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
   AtVector rdir;
   AiReflectSafe(&sg->Rd, &sg->Nf, &sg->Ng, &rdir);

   AtMatrix *matrix = AiShaderEvalParamMtx(p_matrix);
   AiM4VectorByMatrixMult(&rdir, *matrix, &rdir);

   AtMatrix rot;
   AiM4RotationX(rot, 90.0);
   AiM4VectorByMatrixMult(&rdir, rot, &rdir);

   AiM4RotationZ(rot, 90.0);
   AiM4VectorByMatrixMult(&rdir, rot, &rdir);

   // copy original globals
   AtFloat inU = sg->u;
   AtFloat inV = sg->v;

//   sg->dudx = outDuDx;
//   sg->dudy = outDuDy;
//   sg->dvdx = outDvDx;
//   sg->dvdy = outDvDy;

   // replace globals
   AtFloat *pU;
   AtFloat *pV;
   AtFloat R = sqrt((rdir.x*rdir.x) + (rdir.y*rdir.y) + (rdir.z*rdir.z));
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
   *pU = (0.5f * (atanf(rdir.y/rdir.x) / (AtFloat)AI_PI));
   if      (rdir.y > 0.0f && rdir.x > 0.0f) *pU = *pU + .5f;
   else if (rdir.y < 0.0f && rdir.x > 0.0f) *pU = *pU + .5f;
   *pV = 1 - (acosf(rdir.z/R) / (AtFloat)AI_PI);

   // call input
   sg->out.RGB = AiShaderEvalParamRGB(p_image);

   // restore globals
   sg->u = inU;
   sg->v = inV;
//   sg->dudx = inDuDx;
//   sg->dudy = inDuDy;
//   sg->dvdx = inDvDx;
//   sg->dvdy = inDvDy;
}
