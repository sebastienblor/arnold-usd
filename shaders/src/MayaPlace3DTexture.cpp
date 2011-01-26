#include <ai.h>

#include <map>

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


class P3DTData
{
public:

   typedef std::map<AtByte, AtMatrix*> ThreadMatrixMap;

   P3DTData()
      : mBeingDestroyed(false)
   {
      AiCritSecInitRecursive(&mMutex);
   }

   ~P3DTData()
   {
      AiCritSecEnter(&mMutex);

      mBeingDestroyed = true;

      ThreadMatrixMap::iterator it = mThreadMatrices.begin();
      while (it != mThreadMatrices.end())
      {
         AiFree(it->second);
         ++it;
      }
      mThreadMatrices.clear();

      AiCritSecLeave(&mMutex);

      AiCritSecClose(&mMutex);
   }

   AtMatrix* getOrCreateMatrix(AtShaderGlobals *sg)
   {
      AtMatrix *rv = 0;

      AiCritSecEnter(&mMutex);

      if (!mBeingDestroyed)
      {
         ThreadMatrixMap::iterator it = mThreadMatrices.find(sg->tid);
         if (it != mThreadMatrices.end())
         {
            rv = it->second;
         }
         else
         {
            rv = (AtMatrix*) AiMalloc(sizeof(AtMatrix));
            mThreadMatrices[sg->tid] = rv;
         }
      }

      AiCritSecLeave(&mMutex);

      return rv;
   }

protected:

  AtCritSec mMutex;
  ThreadMatrixMap mThreadMatrices;
  bool mBeingDestroyed;
};

};

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);
   
   AiParameterVEC("translate", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("rotate", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("scale", 1.0f, 1.0f, 1.0f);
   AiParameterVEC("shear", 0.0f, 0.0f, 0.0f);
   AiParameterENUM("rotateOrder", 0, gs_RotateOrderNames);
   AiParameterVEC("rotateAxis", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("scalePivot", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("rotatePivot", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("scalePivotTranslate", 0.0f, 0.0f, 0.0f);
   AiParameterVEC("rotatePivotTranslate", 0.0f, 0.0f, 0.0f);
   AiParameterBOOL("inheritsTransform", true);
   AiParameterMTX("parentMatrix", id);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
   node->local_data = (AtVoid*) new P3DTData();
}

node_update
{
}

node_finish
{
   delete (reinterpret_cast<P3DTData*> (node->local_data));
}

shader_evaluate
{
   P3DTData *data = reinterpret_cast<P3DTData*> (node->local_data);

   AtVector translate = AiShaderEvalParamPnt(p_translate);
   AtVector rotate = AiShaderEvalParamPnt(p_rotate);
   AtVector scale = AiShaderEvalParamPnt(p_scale);
   AtVector shear = AiShaderEvalParamPnt(p_shear);
   AtInt rotateOrder = AiShaderEvalParamInt(p_rotate_order);
   AtVector rotateAxis = AiShaderEvalParamPnt(p_rotate_axis);
   AtVector scalePivot = AiShaderEvalParamPnt(p_scale_pivot);
   AtVector rotatePivot = AiShaderEvalParamPnt(p_rotate_pivot);
   AtVector scalePivotTranslate = AiShaderEvalParamPnt(p_scale_pivot_translate);
   AtVector rotatePivotTranslate = AiShaderEvalParamPnt(p_rotate_pivot_translate);
   bool inheritsTransform = (AiShaderEvalParamBool(p_inherits_transform) == TRUE);
   AtMatrix *parentMatrix = AiShaderEvalParamMtx(p_parent_matrix);

   AtMatrix *pM = data->getOrCreateMatrix(sg);
   if (!pM)
   {
      AiMsgWarning("[mtoa_shaders] MayaPlace3DTexture: Trying to access node matrix data while node is being destroyed");
      return;
   }
   
   AtMatrix &M = *pM;
   AtMatrix T, R, Rx, Ry, Rz, Ro, Rp, Rpi, Rt, S, Sp, Spi, Sh, St, tmp;

   AiM4Translation(T, &translate);

   AiM4Scaling(S, &scale);

   AiM4Identity(Sh);
   Sh[1][0] = shear.x;
   Sh[2][0] = shear.y;
   Sh[2][1] = shear.z;

   AiM4Translation(St, &scalePivotTranslate);

   AiM4Translation(Sp, &scalePivot);

   AiM4Identity(Spi);
   Spi[3][0] = -scalePivot.x;
   Spi[3][1] = -scalePivot.y;
   Spi[3][2] = -scalePivot.z;

   AiM4Translation(Rt, &rotatePivotTranslate);

   AiM4Translation(Rp, &rotatePivot);

   AiM4Identity(Rpi);
   Rpi[3][0] = -rotatePivot.x;
   Rpi[3][1] = -rotatePivot.y;
   Rpi[3][2] = -rotatePivot.z;

   AiM4Identity(R);
   AiM4Identity(Ro);

   AiM4RotationX(Rx, static_cast<AtFloat>(rotate.x * AI_RTOD));
   AiM4RotationY(Ry, static_cast<AtFloat>(-rotate.y * AI_RTOD));
   AiM4RotationZ(Rz, static_cast<AtFloat>(rotate.z * AI_RTOD));
   switch (rotateOrder)
   {
   case RO_XYZ:
      AiM4Mult(tmp, Rx, Ry);
      AiM4Mult(R, tmp, Rz);
      break;
   case RO_YZX:
      AiM4Mult(tmp, Ry, Rz);
      AiM4Mult(R, tmp, Rx);
      break;
   case RO_ZXY:
      AiM4Mult(tmp, Rz, Rx);
      AiM4Mult(R, tmp, Ry);
      break;
   case RO_XZY:
      AiM4Mult(tmp, Rx, Rz);
      AiM4Mult(R, tmp, Ry);
      break;
   case RO_YXZ:
      AiM4Mult(tmp, Ry, Rx);
      AiM4Mult(R, tmp, Rz);
      break;
   case RO_ZYX:
   default:
      AiM4Mult(tmp, Rz, Ry);
      AiM4Mult(R, tmp, Rx);
      break;
   }

   AiM4RotationX(Rx, static_cast<AtFloat>(rotateAxis.x * AI_RTOD));
   AiM4RotationY(Ry, static_cast<AtFloat>(-rotateAxis.y * AI_RTOD));
   AiM4RotationZ(Rz, static_cast<AtFloat>(rotateAxis.z * AI_RTOD));
   AiM4Mult(tmp, Rx, Ry);
   AiM4Mult(Ro, tmp, Rz);

   AiM4Mult(tmp, Spi, S);
   AiM4Mult(M, tmp, Sh);
   AiM4Mult(tmp, M, Sp);
   AiM4Mult(M, tmp, St);
   AiM4Mult(tmp, M, Rpi);
   AiM4Mult(M, tmp, Ro);
   AiM4Mult(tmp, M, R);
   AiM4Mult(M, tmp, Rp);
   AiM4Mult(tmp, M, Rt);
   AiM4Mult(M, tmp, T);

   if (inheritsTransform)
   {
      AiM4Mult(tmp, M, *parentMatrix);
      AiM4Copy(M, tmp);
   }

   sg->out.pMTX = pM;
}

