
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>

#include <maya/MFnCamera.h>
#include <maya/MPlug.h>

void CMayaScene::ExportCameraData(AtNode* camera, const MDagPath& dagPath, bool mb)
{
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);

   AiNodeSetStr(camera, "name", fnDagNode.partialPathName().asChar());

   AiNodeSetFlt(camera, "near_clip", fnDagNode.findPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip", fnDagNode.findPlug("farClipPlane").asFloat());

   if (m_fnArnoldRenderOptions->findPlug("enable_dof").asBool())
   {
      AiNodeSetFlt(camera, "focal_distance", m_fnArnoldRenderOptions->findPlug("focal_distance").asFloat());
      AiNodeSetFlt(camera, "aperture_size", m_fnArnoldRenderOptions->findPlug("aperture_size").asFloat());
      AiNodeSetInt(camera, "aperture_blades", m_fnArnoldRenderOptions->findPlug("aperture_blades").asInt());
      AiNodeSetInt(camera, "aperture_rotation", m_fnArnoldRenderOptions->findPlug("aperture_rotation").asInt());
      AiNodeSetFlt(camera, "aperture_blade_curvature", m_fnArnoldRenderOptions->findPlug("aperture_blade_curvature").asFloat());
   }
   
   if (m_motionBlurData.enabled)
   {
      float halfShutter = m_motionBlurData.shutter_size * 0.5f;
      AiNodeSetFlt(camera, "shutter_start", 0.5f - halfShutter);
      AiNodeSetFlt(camera, "shutter_end", 0.5f + halfShutter);
      AiNodeSetInt(camera, "shutter_type", m_fnArnoldRenderOptions->findPlug("shutter_type").asInt());
   }

   GetMatrix(matrix, dagPath);
   
   if (mb)
   {
      AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(camera, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(camera, "matrix", matrix);
   }
}

void CMayaScene::ExportCameraMBData(const MDagPath& dagPath, AtUInt step)
{
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);

   AtNode* camera = AiNodeLookUpByName(fnDagNode.partialPathName().asChar());

   GetMatrix(matrix, dagPath);

   AtArray* matrices = AiNodeGetArray(camera, "matrix");
   AiArraySetMtx(matrices, step, matrix);
}

void CMayaScene::ExportCamera(const MDagPath& dagPath, AtUInt step)
{
   AtNode* camera;
   MFnCamera fnCamera(dagPath);
   MFnDagNode fnDagNode(dagPath);

   bool mb = m_motionBlurData.enabled && m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool();

   if (fnCamera.isOrtho())
   {
      camera = AiNode("ortho_camera");

      if (step == 0)
      {
         ExportCameraData(camera, dagPath, mb);

         // TODO: This is probably wrong, but for now it is working
         float width = fnCamera.orthoWidth();
         float height = width;

         AiNodeSetPnt2(camera, "screen_window_min", -width/2, -height/2);
         AiNodeSetPnt2(camera, "screen_window_max", width/2, height/2);
      }
      else if (mb)
      {
         ExportCameraMBData(dagPath, step);
      }
   }
   else
   {
      camera = AiNode("persp_camera");

      float fov = static_cast<float>(AI_RTOD * fnCamera.horizontalFieldOfView());

      if (step == 0)
      {
         ExportCameraData(camera, dagPath, mb);

         if (mb)
         {
            AtArray* fovs = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_FLOAT);
            AiArraySetFlt(fovs, step, fov);
            AiNodeSetArray(camera, "fov", fovs);
         }
         else
         {
            AiNodeSetFlt(camera, "fov", fov);
         }
      }
      else if (mb)
      {
         ExportCameraMBData(dagPath, step);

         camera = AiNodeLookUpByName(fnDagNode.partialPathName().asChar());

         AtArray* fovs = AiNodeGetArray(camera, "fov");
         AiArraySetFlt(fovs, step, fov);
      }
   }
}
