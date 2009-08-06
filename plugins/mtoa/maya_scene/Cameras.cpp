
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>

#include <maya/MFnCamera.h>
#include <maya/MPlug.h>

void CMayaScene::ExportCamera(const MDagPath& dagPath, AtUInt step)
{
   AtNode* camera;
   AtMatrix matrix;
   MFnCamera fnCamera(dagPath);
   MFnDagNode fnDagNode(dagPath);

   bool mb = m_motionBlurData.enabled && m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool();

   GetMatrix(matrix, dagPath);
   
   float fov = static_cast<float>(AI_RTOD * fnCamera.horizontalFieldOfView());

   if (step == 0)
   {
      camera = AiNode("persp_camera");

      AiNodeSetStr(camera, "name", fnDagNode.name().asChar());

      if (mb)
      {
         float shutter = m_motionBlurData.shutter_end - m_motionBlurData.shutter_start;
         AiNodeSetFlt(camera, "shutter_start", 0);
         AiNodeSetFlt(camera, "shutter_end", shutter);
         AiNodeSetInt(camera, "shutter_type", m_fnArnoldRenderOptions->findPlug("shutter_type").asInt());

         AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, step, matrix);
         AiNodeSetArray(camera, "matrix", matrices);

         AtArray* fovs = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_FLOAT);
         AiArraySetFlt(fovs, step, fov);
         AiNodeSetArray(camera, "fov", fovs);
      }
      else
      {
         AiNodeSetMatrix(camera, "matrix", matrix);
         AiNodeSetFlt(camera, "fov", fov);
      }
   }
   else if (mb)
   {
      camera = AiNodeLookUpByName(fnDagNode.name().asChar());

      AtArray* matrices = AiNodeGetArray(camera, "matrix");
      AiArraySetMtx(matrices, step, matrix);

      AtArray* fovs = AiNodeGetArray(camera, "fov");
      AiArraySetFlt(fovs, step, fov);
   }
}
