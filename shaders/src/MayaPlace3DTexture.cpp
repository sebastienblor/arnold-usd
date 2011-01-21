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

typedef std::map<AtVoid*, AtMatrix*> ThreadMatrixMap;

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

#ifdef _WIN32
#  include <windows.h>
AtVoid* CurrentThread()
{
   // AiThreadSelf() on windows return current thread pseudo handle, which has the same value for all threads
   // This cannot be used to identify the thread as we intend here
   return (AtVoid*) GetCurrentThreadId();
}
#else
AtVoid* CurrentThread()
{
   // Under linux, this should be fine if Arnold is using pthread
   return AiThreadSelf();
}
#endif

node_initialize
{
   // Called once for all threads
   ThreadMatrixMap *tmm = new ThreadMatrixMap();
   node->local_data = (AtVoid*) tmm;
}

node_update
{
   // Called once for all threads
}

node_finish
{
   ThreadMatrixMap *tmm = (ThreadMatrixMap*) node->local_data;
   ThreadMatrixMap::iterator it = tmm->begin();
   while (it != tmm->end())
   {
      AiFree(it->second);
      ++it;
   }
   tmm->clear();
   delete tmm;
}

shader_evaluate
{
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

   AtMatrix *pM = 0;

   // Allocate one matrix per running thread
   AtVoid *currentThread = CurrentThread();
   ThreadMatrixMap *tmm = (ThreadMatrixMap*) node->local_data;
   ThreadMatrixMap::iterator it = tmm->find(currentThread);
   if (it != tmm->end())
   {
      pM = it->second;
   }
   else
   {
      pM = (AtMatrix*) AiMalloc(sizeof(AtMatrix));
      (*tmm)[currentThread] = pM;
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
