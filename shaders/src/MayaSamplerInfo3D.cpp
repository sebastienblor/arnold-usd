#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaSamplerInfo3DMtd);

namespace
{

enum MaySamplerInfo3DParams
{
   p_mode
};


enum Mode
{
   POINT_WORLD = 0,
   POINT_OBJ,
   POINT_CAMERA,
   NORMAL_CAMERA,
   RAY_DIRECTION,
   TANGENT_U_CAMERA,
   TANGENT_V_CAMERA
};

const char* mode_enum[] =
{
   "pointWorld",
   "pointObj",
   "pointCamera",
   "normalCamera",
   "rayDirection",
   "tangentUCamera",
   "tangentVCamera",
   NULL
};

}


node_parameters
{
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);

   AiParameterEnum("mode", 0, mode_enum);
}

node_initialize
{
   // cache camera to world matrix
   AtMatrix* worldToCam = (AtMatrix*)AiMalloc(sizeof(AtMatrix));
   AiNodeSetLocalData(node, worldToCam);
}

node_update
{
   AtMatrix* worldToCam = (AtMatrix*)AiNodeGetLocalData(node);
   AtNode* options = AiUniverseGetOptions();
   AtNode* camera = AiUniverseGetCamera();
   AiWorldToCameraMatrix(camera, AiNodeGetFlt(options, "reference_time"), *worldToCam);
}

node_finish
{
   AiFree(AiNodeGetLocalData(node));
}

shader_evaluate
{
   AtMatrix* worldToCam = (AtMatrix*)AiNodeGetLocalData(node);
   switch (AiShaderEvalParamEnum(p_mode))
   {
   case POINT_WORLD:
      sg->out.VEC() = sg->P;
      break;
   case POINT_OBJ:
      sg->out.VEC() = sg->Po;
      break;
   case POINT_CAMERA:
      AiM4PointByMatrixMult(&sg->out.VEC(), *worldToCam, &sg->P);
      break;
   case NORMAL_CAMERA:
      AiM4VectorByMatrixMult(&sg->out.VEC(), *worldToCam, &sg->N);
      break;
   case RAY_DIRECTION:
      AiM4VectorByMatrixMult(&sg->out.VEC(), *worldToCam, &sg->Rd);
      break;
   case TANGENT_U_CAMERA:
      AiM4VectorByMatrixMult(&sg->out.VEC(), *worldToCam, &sg->dPdu);
      sg->out.VEC() = AiV3Normalize(sg->out.VEC());
      break;
   case TANGENT_V_CAMERA:
      AiM4VectorByMatrixMult(&sg->out.VEC(), *worldToCam, &sg->dPdv);
      sg->out.VEC() = AiV3Normalize(sg->out.VEC());
      break;
   }
}
