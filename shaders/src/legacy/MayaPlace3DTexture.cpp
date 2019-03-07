#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlace3DTextureMtd);

namespace
{

enum MayaPlace3DTextureParams
{
   p_translate = 0,
   p_rotate,
   p_scale,
   p_shear,
   p_rotate_order,
   p_rotate_axis,
   p_scale_pivot,
   p_rotate_pivot,
   p_scale_pivot_translate,
   p_rotate_pivot_translate,
   p_inherits_transform,
   p_parent_matrix
};

enum RotateOrder
{
   RO_XYZ = 0,
   RO_YZX,
   RO_ZXY,
   RO_XZY,
   RO_YXZ,
   RO_ZYX
};

const char* gs_RotateOrderNames[] =
{
   "XYZ",
   "YZX",
   "ZXY",
   "XZY",
   "YXZ",
   "ZYX",
   NULL
};


};

node_parameters
{
   AtMatrix id = AiM4Identity();
   
   AiParameterVec("translate", 0.0f, 0.0f, 0.0f);
   AiParameterVec("rotate", 0.0f, 0.0f, 0.0f);
   AiParameterVec("scale", 1.0f, 1.0f, 1.0f);
   AiParameterVec("shear", 0.0f, 0.0f, 0.0f);
   AiParameterEnum("rotateOrder", 0, gs_RotateOrderNames);
   AiParameterVec("rotateAxis", 0.0f, 0.0f, 0.0f);
   AiParameterVec("scalePivot", 0.0f, 0.0f, 0.0f);
   AiParameterVec("rotatePivot", 0.0f, 0.0f, 0.0f);
   AiParameterVec("scalePivotTranslate", 0.0f, 0.0f, 0.0f);
   AiParameterVec("rotatePivotTranslate", 0.0f, 0.0f, 0.0f);
   AiParameterBool("inheritsTransform", true);
   AiParameterMtx("parentMatrix", id);

   //AiMetaDataSetStr(nentry, NULL, "maya.name", "place3dTexture");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52504c44);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
   AtVector translate = AiShaderEvalParamVec(p_translate);
   AtVector rotate = AiShaderEvalParamVec(p_rotate);
   AtVector scale = AiShaderEvalParamVec(p_scale);
   AtVector shear = AiShaderEvalParamVec(p_shear);
   int rotateOrder = AiShaderEvalParamInt(p_rotate_order);
   AtVector rotateAxis = AiShaderEvalParamVec(p_rotate_axis);
   AtVector scalePivot = AiShaderEvalParamVec(p_scale_pivot);
   AtVector rotatePivot = AiShaderEvalParamVec(p_rotate_pivot);
   AtVector scalePivotTranslate = AiShaderEvalParamVec(p_scale_pivot_translate);
   AtVector rotatePivotTranslate = AiShaderEvalParamVec(p_rotate_pivot_translate);
   bool inheritsTransform = (AiShaderEvalParamBool(p_inherits_transform) == true);
   AtMatrix *parentMatrix = AiShaderEvalParamMtx(p_parent_matrix);
 
   AtMatrix T = AiM4Translation(translate);
   AtMatrix S = AiM4Translation(scale);

   AtMatrix Sh = AiM4Identity();
   Sh[1][0] = shear.x;
   Sh[2][0] = shear.y;
   Sh[2][1] = shear.z;

   AtMatrix St = AiM4Translation(scalePivotTranslate);

   AtMatrix Sp = AiM4Translation(scalePivot);

   AtMatrix Spi = AiM4Identity();
   Spi[3][0] = -scalePivot.x;
   Spi[3][1] = -scalePivot.y;
   Spi[3][2] = -scalePivot.z;

   AtMatrix Rt = AiM4Translation(rotatePivotTranslate);

   AtMatrix Rp = AiM4Translation(rotatePivot);

   AtMatrix Rpi = AiM4Identity();
   Rpi[3][0] = -rotatePivot.x;
   Rpi[3][1] = -rotatePivot.y;
   Rpi[3][2] = -rotatePivot.z;

   AtMatrix R = AiM4Identity();
   AtMatrix Ro = AiM4Identity();

   AtMatrix Rx = AiM4RotationX(rotate.x * AI_RTOD);
   AtMatrix Ry = AiM4RotationY(-rotate.y * AI_RTOD);
   AtMatrix Rz = AiM4RotationZ(rotate.z * AI_RTOD);
   switch (rotateOrder)
   {
   case RO_XYZ:
      R = AiM4Mult(AiM4Mult(Rx, Ry), Rz);
      break;
   case RO_YZX:
      R = AiM4Mult(AiM4Mult(Ry, Rz), Rx);
      break;
   case RO_ZXY:
      R = AiM4Mult(AiM4Mult(Rz, Rx), Ry);
      break;
   case RO_XZY:
      R = AiM4Mult(AiM4Mult(Rx, Rz), Ry);
      break;
   case RO_YXZ:
      R = AiM4Mult(AiM4Mult(Ry, Rx), Rz);
      break;
   case RO_ZYX:
   default:
      R = AiM4Mult(AiM4Mult(Rz, Ry), Rx);
      break;
   }

   Rx = AiM4RotationX(rotateAxis.x * AI_RTOD);
   Ry = AiM4RotationY(-rotateAxis.y * AI_RTOD);
   Rz = AiM4RotationZ(rotateAxis.z * AI_RTOD);
   Ro = AiM4Mult(AiM4Mult(Rx, Ry), Rz);

   // the memory allocated here by QuickAlloc will automatically 
   // be released at the end of this camera ray
   AtMatrix *M = (AtMatrix*) AiShaderGlobalsQuickAlloc(sg, sizeof(AtMatrix));

   *M = AiM4Mult(AiM4Mult(Spi, S), Sh);
   *M = AiM4Mult(AiM4Mult(*M, Sp), St);
   *M = AiM4Mult(AiM4Mult(*M, Rpi), Ro);
   *M = AiM4Mult(AiM4Mult(*M, R), Rp);
   *M = AiM4Mult(AiM4Mult(*M, Rt), T);

   if (inheritsTransform)
      *M = AiM4Mult(*M, *parentMatrix);
      
   sg->out.pMTX() = M;
}

