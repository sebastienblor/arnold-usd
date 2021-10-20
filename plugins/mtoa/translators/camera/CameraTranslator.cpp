#include "CameraTranslator.h"
#include "ImagePlaneTranslator.h"
#include "../NodeTranslatorImpl.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "session/ArnoldSession.h"

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

   
}
void CCameraTranslator::Init()
{
   CDagTranslator::Init();
   m_fnCamera.setObject(m_dagPath);
}

// Do we need this function ? a previous comment said it shouldn't be required
bool CCameraTranslator::RequiresMotionData()
{
   MPlug motionBlurOverridePlug = FindMayaPlug("motionBlurOverride");
   if (motionBlurOverridePlug.isNull())
      return m_impl->m_session->GetOptions().IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
   else
   {
      const short motionBlurOverride = motionBlurOverridePlug.asShort();
      if (motionBlurOverride == 0)
         return m_impl->m_session->GetOptions().IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
      else
         return (motionBlurOverride == 1) ? true : false;
   }      
}


void CCameraTranslator::ExportDOF(AtNode* camera)
{
   CSessionOptions &options = m_impl->m_session->GetOptions();
   // FIXME: focus_distance and aperture_size are animated and should be exported with motion blur
   if (FindMayaPlug("aiEnableDOF").asBool())
   {
      float distance = FindMayaPlug("aiFocusDistance").asFloat();
      options.ScaleDistance(distance);      
      float apertureSize = FindMayaPlug("aiApertureSize").asFloat();
      options.ScaleDistance(apertureSize);
      AiNodeSetFlt(camera, "focus_distance",          distance);
      AiNodeSetFlt(camera, "aperture_size",           apertureSize);
      AiNodeSetInt(camera, "aperture_blades",         FindMayaPlug("aiApertureBlades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation",       FindMayaPlug("aiApertureRotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature",FindMayaPlug("aiApertureBladeCurvature").asFloat());
      AiNodeSetFlt(camera, "aperture_aspect_ratio",   FindMayaPlug("aiApertureAspectRatio").asFloat());
   }
   else
   {
      AiNodeResetParameter(camera, "focus_distance");
      AiNodeResetParameter(camera, "aperture_size");
      AiNodeResetParameter(camera, "aperture_blades");
      AiNodeResetParameter(camera, "aperture_rotation");
      AiNodeResetParameter(camera, "aperture_blade_curvature");
      AiNodeResetParameter(camera, "aperture_aspect_ratio");
   }
}

void CCameraTranslator::ExportCameraData(AtNode* camera)
{
   if (IsExportingMotion())
   {
      // for motion steps, only set the matrix at current step
      // mayaUSD : if (!GetSessionOptions().IsMayaUsd())
      {
         AtMatrix matrix;
         GetMatrix(matrix);

         AtArray* matrices = AiNodeGetArray(camera, "matrix");
         AiArraySetMtx(matrices, GetMotionStep(), matrix);
      }
      return;
   }
   AtMatrix matrix;

   AiNodeSetFlt(camera, "exposure", FindMayaPlug("aiExposure").asFloat());
   
   AiNodeSetFlt(camera, "near_clip", FindMayaPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip",  FindMayaPlug("farClipPlane").asFloat());
   AiNodeSetInt(camera, "rolling_shutter", FindMayaPlug("aiRollingShutter").asInt());
   AiNodeSetFlt(camera, "rolling_shutter_duration", FindMayaPlug("aiRollingShutterDuration").asFloat());

   double motionStart, motionEnd;
   GetSessionOptions().GetMotionRange(motionStart, motionEnd);
   AiNodeSetFlt(camera, "motion_start", (float)motionStart);
   AiNodeSetFlt(camera, "motion_end", (float)motionEnd);

   if (FindMayaPlug("aiUseGlobalShutter").asBool())
   {      
      // Use the Global motion range as shutter (default)
      AiNodeSetFlt(camera, "shutter_start", (float)motionStart);
      AiNodeSetFlt(camera, "shutter_end", (float)motionEnd);
   } else
   {
      // Use the camera's shutter. Note that camera motion will 
      // still be exported in the range [motion_start/motion_end]
      AiNodeSetFlt(camera, "shutter_start", FindMayaPlug("aiShutterStart").asFloat());
      AiNodeSetFlt(camera, "shutter_end", FindMayaPlug("aiShutterEnd").asFloat());
   }
   
   AiNodeSetInt(camera, "shutter_type", FindMayaPlug("aiShutterType").asInt());
   
   ProcessArrayParameter(camera, "shutter_curve", FindMayaPlug("aiShutterCurve"));

   // mayaUSD if (!GetSessionOptions().IsMayaUsd())
   {
      GetMatrix(matrix);
      
      if (RequiresMotionData())
      {
         AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, GetMotionStep(), matrix);
         AiNodeSetArray(camera, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(camera, "matrix", matrix);
      }
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

   if (!GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(camera, "maya_full_name") == NULL)
         AiNodeDeclare(camera, "maya_full_name", "constant STRING");
   
      MString fullName = m_dagPath.fullPathName();
      AiNodeSetStr(camera, "maya_full_name", AtString(fullName.asChar()));
   }
}

double CCameraTranslator::GetDeviceAspect()
{
   double deviceAspect = 0;
   // FIXME if (GetSessionMode() == MTOA_SESSION_RENDERVIEW && CRenderSession::IsViewportRendering())
   if (false)
   {
      // We do an exception for AVP, since the resolution isn't taken from defaultRenderGlobals.resolution.
      // It depends on the viewport resolution. We assume that ArnoldViewOverride has set the option's xres/yres at this point
      AtNode *options = AiUniverseGetOptions(m_impl->m_session->GetUniverse());
      int width = AiNodeGetInt(options, "xres");
      int height = AiNodeGetInt(options, "yres");

      deviceAspect = double(width) / double(height);
   } else
   {
      MStatus        status;
      MSelectionList list;
      MObject        node;
      

      // TODO: replace with a function on CNodeTranslator to get globals node
      list.add("defaultRenderGlobals");
      list.getDependNode(0, node);
      MFnDependencyNode fnRenderGlobals(node);

      MPlugArray connectedPlugs;
      MPlug      resPlug = fnRenderGlobals.findPlug("resolution", true);

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
            deviceAspect = fnRes.findPlug("deviceAspectRatio", true).asFloat();
         }
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

   if (m_fnCamera.findPlug("renderPanZoom", true).asBool() && m_fnCamera.findPlug("panZoomEnabled", true).asBool())
   {      
      double zoom = m_fnCamera.findPlug("zoom", true).asDouble();
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

      double panX = m_fnCamera.findPlug("horizontalPan", true).asDouble() / horizontalAperture * 2;
      double panY = m_fnCamera.findPlug("verticalPan", true).asDouble() / verticalAperture * 2;

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

   AiNodeSetVec2(camera, "screen_window_min", static_cast<float>(minPoint.x), static_cast<float>(minPoint.y));
   AiNodeSetVec2(camera, "screen_window_max", static_cast<float>(maxPoint.x), static_cast<float>(maxPoint.y));
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

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiUseGlobalShutter";
   data.shortName = "ai_ugs";
   data.keyable = false;
   data.channelBox = false;
   helper.MakeInputBoolean(data);
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
   data.defaultValue.BOOL() = false;
   data.name = "aiEnableDOF";
   data.shortName = "ai_edof";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
}

void CCameraTranslator::GetMatrix(AtMatrix& matrix)
{
   MStatus status;
   MMatrix mayaMatrix = m_dagPath.inclusiveMatrix(&status);
   if (status)
   {
      if (m_impl->m_session)
      {
         CSessionOptions &options = m_impl->m_session->GetOptions();
         MTransformationMatrix trMat = mayaMatrix;
         trMat.addTranslation((-1.0) * options.GetOrigin(), MSpace::kWorld);
         MMatrix copyMayaMatrix = trMat.asMatrix();
         copyMayaMatrix[3][0] = options.ScaleDistance(copyMayaMatrix[3][0]); // is this a copy or a reference?
         copyMayaMatrix[3][1] = options.ScaleDistance(copyMayaMatrix[3][1]);
         copyMayaMatrix[3][2] = options.ScaleDistance(copyMayaMatrix[3][2]);

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
void CCameraTranslator::RequestUpdate()
{
   // if this is not the default camera, and if it doesn't have any back connection
   // we should not request an update

   AtNode *node = GetArnoldNode();
   AtNode *renderCam = (AtNode*)AiNodeGetPtr(AiUniverseGetOptions(m_impl->m_session->GetUniverse()), "camera");
   if (node == renderCam || !m_impl->m_backReferences.empty())
      CDagTranslator::RequestUpdate();
   else
      m_impl->m_session->QueueForUpdate(this); // still queue me for update, so that arnold scene remains sync'ed
}

void CCameraTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   CSessionOptions &options = m_impl->m_session->GetOptions();

   if (plugName == "overscan" && !options.GetExportOverscan()) return;

   if (plugName.length() >= 7 && plugName.substringW(0, 6) == "display") return;

   if (plugName == "bookmarks") return;
   if (plugName == "panZoomEnabled" || plugName == "horizontalPan" || plugName == "verticalPan" || plugName == "zoom")
   {
      if (!FindMayaPlug("renderPanZoom").asBool())
         return;
   }

   CDagTranslator::NodeChanged(node, plug);
}

float CCameraTranslator::GetFocalFactor() const
{
   CSessionOptions &options = m_impl->m_session->GetOptions();
   if (!options.GetExportOverscan())
      return 1.f;

   // We need to take into account overscan for AVP
   MPlug overscanPlug = FindMayaPlug("overscan");
   if (!overscanPlug.isNull())
      return 1.f / AiMax(overscanPlug.asFloat(), AI_EPSILON);

   return 1.f;
}