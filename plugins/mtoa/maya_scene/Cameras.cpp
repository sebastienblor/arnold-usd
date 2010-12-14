
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>

#include <maya/MFnCamera.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>

using namespace std;

double MM_TO_INCH = 0.03937;

void CMayaScene::ExportCameraData(AtNode* camera, const MDagPath& dagPath, bool mb)
{
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);

   AiNodeSetStr(camera, "name", fnDagNode.partialPathName().asChar());

   AiNodeSetFlt(camera, "near_clip", fnDagNode.findPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip", fnDagNode.findPlug("farClipPlane").asFloat());
   
   if ( fnDagNode.findPlug("enable_dof").asBool())
   {
      AiNodeSetFlt(camera, "focal_distance", fnDagNode.findPlug("focal_distance").asFloat());
      AiNodeSetFlt(camera, "aperture_size", fnDagNode.findPlug("aperture_size").asFloat());
      AiNodeSetInt(camera, "aperture_blades", fnDagNode.findPlug("aperture_blades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation", fnDagNode.findPlug("aperture_rotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature", fnDagNode.findPlug("aperture_blade_curvature").asFloat());
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
         ExportImagePlane(dagPath, mb, step);

         GetOrthoFilmback(camera, fnCamera);
      }
      else if (mb)
      {
         ExportCameraMBData(dagPath, step);
         ExportImagePlane(dagPath, mb, step);
      }
   }
   else
   {
      camera = AiNode("persp_camera");
      float fov = GetPerspFilmback(camera, fnCamera);

      if (step == 0)
      {
         ExportCameraData(camera, dagPath, mb);
         ExportImagePlane(dagPath, mb, step);

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
         ExportImagePlane(dagPath, mb, step);

         camera = AiNodeLookUpByName(fnDagNode.partialPathName().asChar());

         AtArray* fovs = AiNodeGetArray(camera, "fov");
         AiArraySetFlt(fovs, step, fov);
      }
   }
}

double CMayaScene::GetDeviceAspect()
{
   MStatus        status;
   MSelectionList list;
   MObject        node;
   double deviceAspect = 0;

   list.add("defaultRenderGlobals");
   list.getDependNode(0, node);
   MFnDependencyNode fnRenderGlobals(node);

   MPlugArray connectedPlugs;
   MPlug      resPlug = fnRenderGlobals.findPlug("resolution");

   resPlug.connectedTo(connectedPlugs,
                       true,  // asDestination
                       false, // asSource
                       &status);

   // Must be length 1 or we would have fan-in
   if (status && (connectedPlugs.length() == 1))
   {
      MObject resNode = connectedPlugs[0].node(&status);

      if (status)
      {
         MFnDependencyNode fnRes(resNode);
         deviceAspect = fnRes.findPlug("deviceAspectRatio").asFloat();
      }
   }

   return deviceAspect;
}

void CMayaScene::GetOrthoFilmback(AtNode* camera, MFnCamera& fnCamera)
{
   double deviceAspect = GetDeviceAspect();

   double width = fnCamera.orthoWidth();

   MFnCamera::FilmFit filmFit = fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kFillFilmFit){
      if(deviceAspect >= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kOverscanFilmFit){
      if(deviceAspect <= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if(filmFit == MFnCamera::kHorizontalFilmFit)
   {
      width = width/2;
   }
   else if(filmFit == MFnCamera::kVerticalFilmFit)
   {
      width = (width/2)*deviceAspect;
   }
   
   MVectorArray filmTransforms = GetFilmTransform(fnCamera, width, false);
   
   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(filmTransforms[0].x), static_cast<float>(filmTransforms[0].y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(filmTransforms[1].x), static_cast<float>(filmTransforms[1].y));
}

float CMayaScene::GetPerspFilmback(AtNode* camera, MFnCamera& fnCamera)
{

   double deviceAspect = GetDeviceAspect();
   float fov = 1.0f;
   double cameraAspect = fnCamera.aspectRatio();
   double lensSqueeze = fnCamera.lensSqueezeRatio();
   double focalLength = fnCamera.focalLength();
   double cameraScale = fnCamera.cameraScale();
   
   double apertureX = fnCamera.horizontalFilmAperture();
   double apertureY = fnCamera.verticalFilmAperture();
   
   double deviceApertureX = apertureX;
   double deviceApertureY = apertureY;
   
   double filmFitOffset = fnCamera.filmFitOffset();
   double filmOffsetX = fnCamera.horizontalFilmOffset();
   double filmOffsetY = fnCamera.verticalFilmOffset();
   double factorX = 0.0;
   double factorY = 0.0;

   MFnCamera::FilmFit filmFit = fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kFillFilmFit){
      filmFitOffset = 0;
      if((cameraAspect * lensSqueeze) < deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kOverscanFilmFit){
      filmFitOffset = 0;
      if((cameraAspect * lensSqueeze) > deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //HORIZONTAL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kHorizontalFilmFit)
   {
      //Find the camera's Field Of View with Lens Squeeze and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * apertureX * lensSqueeze) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new vertical aperture based off of the Render Aspect Ratio
      deviceApertureY = apertureX / (deviceAspect / lensSqueeze);

      //Apply the film fit offset if the camera's aspect ratio is greater than the render aspect ratio
      if((cameraAspect * lensSqueeze) > deviceAspect && filmFitOffset != 0.0f)
      {
         factorY = ((apertureY - deviceApertureY) * filmFitOffset) / deviceApertureY;
      }
   }

   //VERTICAL FILM RESOLUTION GATE
   else if(filmFit == MFnCamera::kVerticalFilmFit)
   {
      //Find the camera's Field Of View with the Render Aspect Ratio and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * (deviceAspect * apertureY)) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new horizontal aperture based off of the Render Aspect Ratio
      deviceApertureX = apertureY * (deviceAspect / lensSqueeze);
      
      //Apply the film fit offset if the camera's aspect ratio is less than the render aspect ratio
      if((cameraAspect * lensSqueeze) < deviceAspect && filmFitOffset != 0.0f)
      {
         factorX = ((apertureX - deviceApertureX) * filmFitOffset) / deviceApertureX;
      }
   }
   
   //If we have any filmOffsets we add them here
   if(filmOffsetX!=0.0f || filmOffsetY!=0.0f)
   {
      factorX += (filmOffsetX/deviceApertureX)*2;
      factorY += (filmOffsetY/deviceApertureY)*2;
   }
   
   MVectorArray filmTransforms = GetFilmTransform(fnCamera);
   MVector minPoint = filmTransforms[0];
   MVector maxPoint = filmTransforms[1];

   //Add on any offsets from filmOffsetX or Y, or filmFitOffset
   minPoint += MVector(factorX, factorY);
   maxPoint += MVector(factorX, factorY);

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(minPoint.x), static_cast<float>(minPoint.y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(maxPoint.x), static_cast<float>(maxPoint.y));
   
   return fov;
}

MVectorArray CMayaScene::GetFilmTransform(MFnCamera& fnCamera, double width, bool persp)
{
   double deviceAspect = GetDeviceAspect();
   double cameraAspect = fnCamera.aspectRatio();
   double preScale = fnCamera.preScale();
   double filmTranslateX = fnCamera.filmTranslateH();
   double filmTranslateY = fnCamera.filmTranslateV();
   double filmRollValue = fnCamera.filmRollValue();
   //We need a roll attribute from the guys at SolidAngle
   //double filmRollPivotX = fnCamera.verticalRollPivot();
   //double filmRollPivotY = fnCamera.horizontalRollPivot();
   MFnCamera::RollOrder filmRollOrder = fnCamera.filmRollOrder();
   double postScale = fnCamera.postScale();

   //2D Transform default Vectors for Perspective
   MVector minPoint(-1, -1);
   MVector maxPoint(1, 1);

   if(persp)//If the perspective parameter is true, indicating we are operating on a perspective camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;

      filmTranslateX = (filmTranslateX * preScale) / postScale;
      if(cameraAspect > deviceAspect)
         filmTranslateY = ((filmTranslateY/cameraAspect) * 2 * preScale) / postScale;
      else
         filmTranslateY = (filmTranslateY * deviceAspect * preScale) / postScale;
   }
   else//Ortho camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;
      double orthoWidth = fnCamera.orthoWidth() / 2;
      
      if(orthoWidth == width)//We are in Horizontal Mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth * preScale) / postScale;
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect * preScale) / postScale;
      }
      else//Vertical mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth * deviceAspect * preScale) / postScale;
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect * deviceAspect * preScale) / postScale;
      }

      //Set the point to the width passed in for Ortho mode.
      minPoint = MVector(-width, -width);
      maxPoint = MVector(width, width);
   }
   
   if(preScale != 1.0f)
   {
      minPoint *= preScale;
      maxPoint *= preScale;
   }

   if(filmRollOrder == MFnCamera::kTranslateRotate)
   {
      if(filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
      if(filmRollValue != 0.0f)
      {
         //ROTATE
      }
   }
   else //Rotate-Translate order
   {
      if(filmRollValue != 0.0f)
      {
         //ROTATE
      }
      if(filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
   }
  
   if(postScale != 1.0f)
   {
      minPoint *= postScale;
      maxPoint *= postScale;
   }
   
   MVector src[] = {minPoint, maxPoint};
   return MVectorArray(src, 2);
}