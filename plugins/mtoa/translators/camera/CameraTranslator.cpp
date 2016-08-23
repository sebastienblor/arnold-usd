#include "CameraTranslator.h"
#include "ImagePlaneTranslator.h"
#include "../NodeTranslatorImpl.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>

#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MImage.h>

using namespace std;

void CCameraTranslator::ExportImagePlanes()
{   
   MPlug      imagePlanePlug;
   MPlug      imagePlaneNodePlug;
   MStatus    status;

   // first we get the image planes connected to this camera
   imagePlanePlug = FindMayaPlug("imagePlane");

   if (imagePlanePlug.numConnectedElements() > 0)
   {
      for (unsigned int ips = 0; (ips < imagePlanePlug.numElements()); ips++)
      {
         MPlugArray connectedPlugs;
         imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
         imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);

         if (status && (connectedPlugs.length() > 0))
         {
            // Shouldn't we rather call NodeTranslator::ExportNode ?
            CNodeTranslator *imgTranslator = GetSession()->ExportNode(connectedPlugs[0], NULL, NULL, true);
            CImagePlaneTranslator *imgPlaneTranslator =  dynamic_cast<CImagePlaneTranslator*>(imgTranslator);

            // FIXME if we remove step from this function, will imgPlane get the right one ?
            if (GetMotionStep() == 0) imgPlaneTranslator->SetCamera(GetMayaNodeName());
            else  imgPlaneTranslator->ExportMotion(imgPlaneTranslator->GetArnoldRootNode());            
         }
      }
   }
}
void CCameraTranslator::Init()
{
   CDagTranslator::Init();
   m_fnCamera.setObject(m_dagPath);
}

bool CCameraTranslator::RequiresMotionData()
{
   MPlug motionBlurOverridePlug = FindMayaPlug("motionBlurOverride");
   if (motionBlurOverridePlug.isNull())
      return GetSession()->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
   else
   {
      const short motionBlurOverride = motionBlurOverridePlug.asShort();
      if (motionBlurOverride == 0)
         return GetSession()->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
      else
         return (motionBlurOverride == 1) ? true : false;
   }      
}


void CCameraTranslator::ExportDOF(AtNode* camera)
{
   // FIXME: focus_distance and aperture_size are animated and should be exported with motion blur
   if (FindMayaPlug("aiEnableDOF").asBool())
   {
      float distance = FindMayaPlug("aiFocusDistance").asFloat();
      GetSession()->ScaleDistance(distance);      
      float apertureSize = FindMayaPlug("aiApertureSize").asFloat();
      GetSession()->ScaleDistance(apertureSize);
      AiNodeSetFlt(camera, "focus_distance",          distance);
      AiNodeSetFlt(camera, "aperture_size",           apertureSize);
      AiNodeSetInt(camera, "aperture_blades",         FindMayaPlug("aiApertureBlades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation",       FindMayaPlug("aiApertureRotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature",FindMayaPlug("aiApertureBladeCurvature").asFloat());
      AiNodeSetFlt(camera, "aperture_aspect_ratio",   FindMayaPlug("aiApertureAspectRatio").asFloat());
   }
   else
   {
      AiNodeSetFlt(camera, "focus_distance", 1.f);
      AiNodeSetFlt(camera, "aperture_size", 0.f);
      AiNodeSetInt(camera, "aperture_blades", 0);
      AiNodeSetFlt(camera, "aperture_rotation", 0.f);
      AiNodeSetFlt(camera, "aperture_blade_curvature", 0.f);
      AiNodeSetFlt(camera, "aperture_aspect_ratio", 1.0f);
   }
}

void CCameraTranslator::ExportCameraData(AtNode* camera)
{
   AtMatrix matrix;

   AiNodeSetFlt(camera, "exposure", FindMayaPlug("aiExposure").asFloat());
   
   AiNodeSetFlt(camera, "near_clip", FindMayaPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip",  FindMayaPlug("farClipPlane").asFloat());
   AiNodeSetInt(camera, "rolling_shutter", FindMayaPlug("aiRollingShutter").asInt());
   AiNodeSetFlt(camera, "rolling_shutter_duration", FindMayaPlug("aiRollingShutterDuration").asFloat());

   AiNodeSetFlt(camera, "shutter_start", FindMayaPlug("aiShutterStart").asFloat());
   AiNodeSetFlt(camera, "shutter_end", FindMayaPlug("aiShutterEnd").asFloat());
   AiNodeSetInt(camera, "shutter_type", FindMayaPlug("aiShutterType").asInt());
   
   ProcessArrayParameter(camera, "shutter_curve", FindMayaPlug("aiShutterCurve"));

   GetMatrix(matrix);
   
   if (RequiresMotionData())
   {
      AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(camera, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(camera, "matrix", matrix);
   }
   MPlug plug = FindMayaPlug("aiFiltermap");
   if (!plug.isNull())
   {
      MPlugArray filtermapPlug;
      plug.connectedTo(filtermapPlug, true, false);
      if (filtermapPlug.length() > 0)
      {
         AtNode* filtermap = ExportConnectedNode(filtermapPlug[0]);
         AiNodeSetPtr(camera, "filtermap", filtermap);
      }
   }
}

void CCameraTranslator::ExportCameraMBData(AtNode *camera)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(camera, "matrix");
   AiArraySetMtx(matrices, GetMotionStep(), matrix);
}

double CCameraTranslator::GetDeviceAspect()
{
   MStatus        status;
   MSelectionList list;
   MObject        node;
   double deviceAspect = 0;

   // TODO: replace with a function on CNodeTranslator to get globals node
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

void CCameraTranslator::SetFilmTransform(AtNode* camera, double factorX, double factorY, double width, bool persp)
{
   double deviceAspect = GetDeviceAspect();
   // double cameraAspect = m_fnCamera.aspectRatio();
   double preScale = m_fnCamera.preScale();
   double filmTranslateX = m_fnCamera.filmTranslateH();
   double filmTranslateY = m_fnCamera.filmTranslateV();
   double filmRollValue = m_fnCamera.filmRollValue();
   //TODO: We need a roll attribute from the guys at SolidAngle
   //double filmRollPivotX = m_fnCamera.verticalRollPivot();
   //double filmRollPivotY = m_fnCamera.horizontalRollPivot();
   MFnCamera::RollOrder filmRollOrder = m_fnCamera.filmRollOrder();
   double postScale = m_fnCamera.postScale();

   // 2D Transform default Vectors for Perspective
   MVector minPoint(-1, -1);
   MVector maxPoint(1, 1);

   preScale = 1 / preScale;
   postScale = 1 / postScale;

   filmTranslateX *= preScale;
   filmTranslateY *= preScale;

   if (persp) // perspective camera
   {
      filmTranslateY *= deviceAspect;
   }
   else // Ortho camera
   {
      // FIXME: most likely problems exist below
      double orthoWidth = m_fnCamera.orthoWidth() / 2;
      if (orthoWidth == width)// We are in Horizontal Mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth);
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect);
      }
      else// Vertical mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth * deviceAspect);
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect * deviceAspect);
      }

      // Set the point to the width passed in for Ortho mode.
      minPoint = MVector(-width, -width);
      maxPoint = MVector(width, width);
   }

   if (m_fnCamera.findPlug("renderPanZoom").asBool() && m_fnCamera.findPlug("panZoomEnabled").asBool())
   {      
      double zoom = m_fnCamera.findPlug("zoom").asDouble();
      filmTranslateX *= zoom;
      filmTranslateY *= zoom;

      minPoint *= zoom;
      maxPoint *= zoom;

      MFnCamera::FilmFit fitResolutionGate = m_fnCamera.filmFit();

      double horizontalAperture = m_fnCamera.horizontalFilmAperture();
      double verticalAperture = m_fnCamera.verticalFilmAperture();

      double filmAspect = horizontalAperture / verticalAperture;

      switch (fitResolutionGate)
      {
         case MFnCamera::kFillFilmFit:
            if (deviceAspect < filmAspect)
            {
               verticalAperture *= deviceAspect / filmAspect;
               horizontalAperture = verticalAperture * filmAspect;
            }
         break;
         case MFnCamera::kVerticalFilmFit:
            verticalAperture *= deviceAspect / filmAspect;
            horizontalAperture = verticalAperture * filmAspect;
         break;
         case MFnCamera::kOverscanFilmFit:
            if (deviceAspect > filmAspect)
            {
               horizontalAperture *= deviceAspect / filmAspect;
               verticalAperture = horizontalAperture / filmAspect;
            }
         break;
         case MFnCamera::kHorizontalFilmFit:
         default:
         break;
      }

      double panX = m_fnCamera.findPlug("horizontalPan").asDouble() / horizontalAperture * 2;
      double panY = m_fnCamera.findPlug("verticalPan").asDouble() / verticalAperture * 2;

      filmTranslateX += panX;
      filmTranslateY += (panY * (deviceAspect / filmAspect));
   }

   filmTranslateX /= postScale;
   filmTranslateY /= postScale;

   if (preScale != 1.0f)
   {
      minPoint *= preScale;
      maxPoint *= preScale;
   }

   if (filmRollOrder == MFnCamera::kTranslateRotate)
   {
      if (filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
      if (filmRollValue != 0.0f)
      {
         // TODO: ROTATE
      }
   }
   else //Rotate-Translate order
   {
      if (filmRollValue != 0.0f)
      {
         // TODO: ROTATE
      }
      if (filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
   }
   if (postScale != 1.0f)
   {
      minPoint *= postScale;
      maxPoint *= postScale;
   }

   // Add on any offsets from filmOffsetX or Y, or filmFitOffset
   minPoint += MVector(factorX, factorY);
   maxPoint += MVector(factorX, factorY);

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(minPoint.x), static_cast<float>(minPoint.y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(maxPoint.x), static_cast<float>(maxPoint.y));
}

void CCameraTranslator::MakeDefaultAttributes(CExtensionAttrHelper &helper)
{
   helper.MakeInput("exposure");
   helper.MakeInput("filtermap");
   helper.MakeInput("rolling_shutter");
   helper.MakeInput("rolling_shutter_duration");
   helper.MakeInput("shutter_start");
   helper.MakeInput("shutter_end");
   helper.MakeInput("shutter_type");
   helper.MakeInput("shutter_curve");
}

void CCameraTranslator::MakeDOFAttributes(CExtensionAttrHelper &helper)
{
   helper.MakeInput("focus_distance");
   helper.MakeInput("aperture_size");
   helper.MakeInput("aperture_blades");
   helper.MakeInput("aperture_blade_curvature");
   helper.MakeInput("aperture_rotation");
   helper.MakeInput("aperture_aspect_ratio");
   helper.MakeInput("filtermap");

   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "aiEnableDOF";
   data.shortName = "ai_edof";
   helper.MakeInputBoolean(data);
}

void CCameraTranslator::GetMatrix(AtMatrix& matrix)
{
   MStatus status;
   MMatrix mayaMatrix = m_dagPath.inclusiveMatrix(&status);
   if (status)
   {
      if (GetSession())
      {
         MTransformationMatrix trMat = mayaMatrix;
         trMat.addTranslation((-1.0) * GetSession()->GetOrigin(), MSpace::kWorld);
         MMatrix copyMayaMatrix = trMat.asMatrix();
         copyMayaMatrix[3][0] = GetSession()->ScaleDistance(copyMayaMatrix[3][0]); // is this a copy or a reference?
         copyMayaMatrix[3][1] = GetSession()->ScaleDistance(copyMayaMatrix[3][1]);
         copyMayaMatrix[3][2] = GetSession()->ScaleDistance(copyMayaMatrix[3][2]);

         for (int J = 0; (J < 4); ++J)
         {
            for (int I = 0; (I < 4); ++I)
            {
               matrix[I][J] = (float) copyMayaMatrix[I][J];
            }
         }
      }
      else
      {
         for (int J = 0; (J < 4); ++J)
         {
            for (int I = 0; (I < 4); ++I)
            {
               matrix[I][J] = (float) mayaMatrix[I][J];
            }
         }  
      }
   }
}
