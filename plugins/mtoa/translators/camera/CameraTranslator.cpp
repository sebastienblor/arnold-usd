
#include "CameraTranslator.h"
#include "attributes/AttrHelper.h"

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

void CCameraTranslator::ExportImagePlane(AtUInt step, MObject& imgPlane)
{
   // get the dependency node of the image plane
   MFnDependencyNode fnRes(imgPlane);
   MStatus status;
   // check if the image plane should be created
   int displayMode = fnRes.findPlug("displayMode", &status).asInt();
   if (displayMode > 1)
   {
      //MString imagePlaneName = GetFnDagNode().partialPathName();
      MString imagePlaneName = GetFnNode().name();
      imagePlaneName += fnRes.name();
      //imagePlaneName += "_IP_";
      //imagePlaneName += ips;
      MString imageName;
      MImage mImage;

      //type: 0 == Image, 1 == Texture, 2 == Movie
      int type = fnRes.findPlug("type", &status).asInt();

      if (type == 2)//Not supporting type Movie for now....
      {
         AiMsgWarning("[mtoa] [translator %s] Image Planes of type Movie are unsupported", GetName().asChar());
         return;
      }

      double planeSizeX;
      double planeSizeY;

      // UV Values
      double uMin = 0.0f;
      double vMin = 0.0f;
      double uMax = 1.0f;
      double vMax = 1.0f;

      int lockedToCamera = fnRes.findPlug("lockedToCamera", &status).asInt();
      if (lockedToCamera)
      {
         planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
         planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
      }
      else
      {
         planeSizeX = fnRes.findPlug("width", &status).asDouble();
         planeSizeY = fnRes.findPlug("height", &status).asDouble();
      }

      double planeDepth = fnRes.findPlug("depth").asDouble();

      // values from camera
      double camFocal = GetFnNode().findPlug("focalLength").asDouble();
      double camScale = GetFnNode().findPlug("cameraScale").asDouble();
      double lensSqueeze = GetFnNode().findPlug("lensSqueezeRatio").asDouble();

      double ipWidth;
      double ipHeight;

      unsigned int iWidth = 0;
      unsigned int iHeight = 0;

      if (type == 0)
      {
         // get data
         if ((!m_motion) || (step == 0)) // why not simply check the step? won't step always be 0 if motion is off?
         {
            MString frameNumber("0");
            frameNumber += GetCurrentFrame() + fnRes.findPlug("frameOffset").asInt();
            imageName = MRenderUtil::exactFileTextureName(imageName, fnRes.findPlug("useFrameExtension").asBool(), frameNumber);
            imageName = MRenderUtil::exactImagePlaneFileName(imgPlane);
            mImage = MImage();
            mImage.readFromFile(imageName);
         }

         //0:Fill 1:Best 2:Horizontal 3:Vertical 4:ToSize
         mImage.getSize(iWidth, iHeight);
         double iAspect = double(iWidth) / iHeight;
         double planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;
         if (iAspect != planeAspect)
         {
            FitType fit = (FitType)fnRes.findPlug("fit", &status).asInt();

            if (fit == FIT_BEST)
            {
               // fit all of the image in the plane
               if (iAspect > planeAspect)
                  fit = FIT_HORIZONTAL;
               else
                  fit = FIT_VERTICAL;
            }
            else if (fit == FIT_FILL)
            {
               // fill the entire plane with the image
               if (iAspect < planeAspect)
                  fit = FIT_HORIZONTAL;
               else
                  fit = FIT_VERTICAL;
            }

            // in either case we trim the plane to the dimensions of the image, or
            // the image overflows and we adjust the plane's UVs to compensate
            if (fit == FIT_HORIZONTAL)
            {
               // left and right side of image registers with plane
               if (iAspect > planeAspect)
               {
                  // trim the plane vertically to fit image
                  planeSizeY *= (planeAspect / iAspect);
               }
               else
               {
                  // adjust the uv's vertically
                  vMin += ((1.0f - (iAspect / planeAspect)) * 0.5f);
                  vMax -= ((1.0f - (iAspect / planeAspect)) * 0.5f);
               }

            }
            else if (fit == FIT_VERTICAL)
            {
               // top and bottom of image registers with plane
               if (iAspect < planeAspect)
               {
                  // trim the plane horizontally to fit image
                  planeSizeX *= (iAspect / planeAspect);
               }
               else
               {
                  // adjust the uv's horizontally
                  uMin += ((1.0f - (planeAspect / iAspect)) * 0.5f);
                  uMax -= ((1.0f - (planeAspect / iAspect)) * 0.5f);
               }
            }
            else if (fit ==  FIT_TOSIZE)
            {
               // stretch image to fit plane
               double squeezeCorrection = fnRes.findPlug("squeezeCorrection").asDouble();
               if (squeezeCorrection < 0.0001f)
                  squeezeCorrection = 1.0f;
               planeSizeX /= squeezeCorrection;
            }
         }
      }

      if (lockedToCamera)
      {
         ipWidth = (planeSizeX * planeDepth * lensSqueeze) / ((camFocal * MM_TO_INCH) / camScale);
         ipHeight = (planeSizeY * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
      }
      else
      {
         ipWidth = planeSizeX;
         ipHeight = planeSizeY;
      }

      if ((!m_motion) || (step == 0))
      {
         // CREATE PLANE
         AtNode* imagePlane = AiNode("polymesh");
         AiNodeSetStr(imagePlane, "name", imagePlaneName.asChar());

         AiNodeSetArray(imagePlane, "nsides", AiArray(1, 1, AI_TYPE_BYTE, 4));
         AiNodeSetArray(imagePlane, "vidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 3, 2));
         AiNodeSetArray(imagePlane, "nidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 2, 3));
         AiNodeSetArray(imagePlane, "uvidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 3, 2));

         AtPoint p1, p2, p3, p4, n1;
         AtPoint2 uv1, uv2, uv3, uv4;
         AiV3Create(p1, -0.5, -0.5, 0.0);
         AiV3Create(p2, 0.5, -0.5, 0.0);
         AiV3Create(p3, -0.5, 0.5, 0.0);
         AiV3Create(p4, 0.5, 0.5, 0.0);
         AiV3Create(n1, 0.0, 0.0, 1.0);

         /*
         if (type == 0)
         {
            double coverageX = fnRes.findPlug("coverageX", &status).asDouble();
            double coverageY = fnRes.findPlug("coverageY", &status).asDouble();
            double uOffset = 1.0f + (((0.0f - coverageX)/(0.0f - iWidth)) * (0.0f - 1.0f));
            double vOffset = 1.0f + (((0.0f - coverageY)/(0.0f - iHeight)) * (0.0f - 1.0f));

            int coverageOriginX = fnRes.findPlug("coverageOriginX", &status).asInt();
            int coverageOriginY = fnRes.findPlug("coverageOriginY", &status).asInt();

            double uOriginOffset = (((coverageOriginX - 0.0f)/(iWidth - 0.0f)) * (1.0f - 0.0f));
            double vOriginOffset = (((coverageOriginY - 0.0f)/(iHeight - 0.0f)) * (1.0f - 0.0f));

            uMin = uOriginOffset;
            uMax = 1.0f - (uOffset - uOriginOffset);
            if (uMax > 1)
               uMax = 1.0f;

            vMin = vOriginOffset;
            vMax = 1.0f - (vOffset - vOriginOffset);
            if (vMax > 1)
               vMax = 1.0f;
         }
         */
         AiV2Create(uv1, uMin, vMin);
         AiV2Create(uv2, uMax, vMin);
         AiV2Create(uv3, uMin, vMax);
         AiV2Create(uv4, uMax, vMax);

         AiNodeSetArray(imagePlane, "vlist", AiArray(4, 1, AI_TYPE_POINT, p1, p2, p3, p4));
         AiNodeSetArray(imagePlane, "nlist", AiArray(4, 1, AI_TYPE_VECTOR, n1, n1, n1, n1));
         AiNodeSetArray(imagePlane, "uvlist", AiArray(4, 1, AI_TYPE_POINT2, uv1, uv2, uv3, uv4));
         AiNodeSetInt(imagePlane, "visibility", 65425);

         // create a flat shader with the needed image
         MPlug colorPlug;
         MPlugArray conn;

         //AtNode* imagePlaneShader = AiNode("MayaImagePlane");
         AtNode* imagePlaneShader = AiNode("flat");

         if (type == 0)
         {
            AtNode* file = AiNode("image");
            AiNodeSetStr(file, "filename", imageName.asChar());
            AiNodeLink(file, "color", imagePlaneShader);
            /*
            AiNodeSetStr(imagePlaneShader, "filename", imageName.asChar());
            AiNodeSetInt(imagePlaneShader, "displayMode", displayMode);
            AiNodeSetPnt2(imagePlaneShader, "coverage", ipCoverageX, ipCoverageY);
            AiNodeSetPnt2(imagePlaneShader, "translate", ipTranslateX, ipTranslateY);

            colorPlug  = fnRes.findPlug("colorGain");
            colorPlug.connectedTo(conn, true, false);
            if (!conn.length())
              AiNodeSetRGB(imagePlaneShader, "colorGain", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
            else
            {
               MPlug outputPlug = conn[0];
               AiNodeLink(ExportShader(outputPlug), "colorGain", imagePlaneShader);
            }

            colorPlug  = fnRes.findPlug("colorOffset");
            colorPlug.connectedTo(conn, true, false);
            if (!conn.length())
               AiNodeSetRGB(imagePlaneShader, "colorOffset", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
            else
            {
               MPlug outputPlug = conn[0];
               AiNodeLink(ExportShader(outputPlug), "colorOffset", imagePlaneShader);
            }

            float alphaGain = fnRes.findPlug("alphaGain", &status).asFloat();
            AiNodeSetFlt(imagePlaneShader, "alphaGain", alphaGain);
            */
         }
         else if (type == 1)
         {
            MPlug colorPlug  = fnRes.findPlug("sourceTexture");
            MPlugArray conn;
            colorPlug.connectedTo(conn, true, false);
            if (conn.length())
            {
               MPlug outputPlug = conn[0];
               AiNodeLink(ExportShader(outputPlug), "color", imagePlaneShader);
            }
         }

         AiNodeSetPtr(imagePlane, "shader", imagePlaneShader);
         AiNodeSetBool(imagePlane, "opaque", 0);

      }// ((!m_motion) || (step == 0))

      AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());

      AtMatrix offsetMatrix;
      AtMatrix scaleMatrix;
      AtMatrix rotationMatrix;
      AtMatrix imagePlaneMatrix;
      AtVector offsetVector;
      AtVector scaleVector;

      double offsetX;
      double offsetY;
      double offsetZ;

      //if the plane is locked to the camera find and use the offset values
      //otherwise it isn't locked so use the center attribute values to offset the plane
      if (lockedToCamera)
      {
         offsetX = (fnRes.findPlug("offsetX", &status).asDouble() * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
         offsetY = (fnRes.findPlug("offsetY", &status).asDouble() * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
         offsetZ = -planeDepth;
      }
      else
      {
         offsetX = fnRes.findPlug("centerX", &status).asDouble();
         offsetY = fnRes.findPlug("centerY", &status).asDouble();
         offsetZ = fnRes.findPlug("centerZ", &status).asDouble();
      }

      AiV3Create(offsetVector, static_cast<float>(offsetX), static_cast<float>(offsetY), static_cast<float>(offsetZ));
      AiV3Create(scaleVector, static_cast<float>(ipWidth), static_cast<float>(ipHeight), 1.0f);
      AiM4Translation(offsetMatrix, &offsetVector);
      AiM4Scaling(scaleMatrix, &scaleVector);

      if (lockedToCamera)
      {
         double ipRotate = fnRes.findPlug("rotate", &status).asDouble() * AI_RTOD * -1.0f;
         AiM4RotationZ(rotationMatrix, float(ipRotate));
      }
      else
      {
         //Get the camera's object space rotation matrix
         GetRotationMatrix(rotationMatrix);
      }

      // multiply in order
      AiM4Identity(imagePlaneMatrix);
      AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, scaleMatrix);
      AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, rotationMatrix);
      AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, offsetMatrix);
      //if the imageplane is locked we use the camera's matrix
      if (lockedToCamera)
      {
         // get cam's matrix
         AtMatrix translateMatrix;
         GetMatrix(translateMatrix);
         AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, translateMatrix);
      }

      // image plane should move with the camera to render it with no motion blur
      if (m_motion)
      {
         if (step == 0)
         {
             AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
             AiArraySetMtx(matrices, 0, imagePlaneMatrix);
             AiNodeSetArray(imagePlane, "matrix", matrices);
         }
         else
         {
             AtArray* matrices = AiNodeGetArray(imagePlane, "matrix");
             AiArraySetMtx(matrices, step, imagePlaneMatrix);
         }
      }
      else
      {
         AiNodeSetMatrix(imagePlane, "matrix", imagePlaneMatrix);
      }
   }
}


void CCameraTranslator::ExportImagePlanes(AtUInt step)
{
    MPlugArray connectedPlugs;
    MPlug      imagePlanePlug;
    MPlug      imagePlaneNodePlug;
   MStatus    status;

   // first we get the image planes connected to this camera
   imagePlanePlug = GetFnNode().findPlug("imagePlane");

   if (imagePlanePlug.numConnectedElements() > 0)
   {
      for (AtUInt ips = 0; (ips < imagePlanePlug.numElements()); ips++)
      {
         imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
         imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);
         MObject resNode = connectedPlugs[0].node(&status);

         if (status)
         {
            ExportImagePlane(step, resNode);
         }
      }
   }
}

void CCameraTranslator::ExportDOF(AtNode* camera)
{
   // FIXME: focus_distance and aperture_size are animated and should be exported with motion blur
   if (GetFnNode().findPlug("aiEnableDOF").asBool())
   {
      AiNodeSetFlt(camera, "focus_distance",          GetFnNode().findPlug("aiFocusDistance").asFloat());
      AiNodeSetFlt(camera, "aperture_size",           GetFnNode().findPlug("aiApertureSize").asFloat());
      AiNodeSetInt(camera, "aperture_blades",         GetFnNode().findPlug("aiApertureBlades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation",       GetFnNode().findPlug("aiApertureRotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature",GetFnNode().findPlug("aiApertureBladeCurvature").asFloat());
   }
}

void CCameraTranslator::ExportCameraData(AtNode* camera)
{
   AtMatrix matrix;

   AiNodeSetFlt(camera, "near_clip", GetFnNode().findPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip",  GetFnNode().findPlug("farClipPlane").asFloat());

   if (IsMotionBlurEnabled())
   {
      float halfShutter = GetShutterSize() * 0.5f;
      AiNodeSetFlt(camera, "shutter_start", 0.5f - halfShutter);
      AiNodeSetFlt(camera, "shutter_end", 0.5f + halfShutter);
      AiNodeSetInt(camera, "shutter_type", GetShutterType());
   }

   GetMatrix(matrix);
   
   if (m_motion)
   {
      AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(camera, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(camera, "matrix", matrix);
   }
}

void CCameraTranslator::ExportCameraMBData(AtNode *camera, AtUInt step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(camera, "matrix");
   AiArraySetMtx(matrices, step, matrix);
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

      double filmAspect = m_fnCamera.horizontalFilmAperture() / m_fnCamera.verticalFilmAperture();
      double panX = m_fnCamera.findPlug("horizontalPan").asDouble() / m_fnCamera.horizontalFilmAperture() * 2;
      double panY = m_fnCamera.findPlug("verticalPan").asDouble() / m_fnCamera.verticalFilmAperture() * 2;
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
   helper.MakeInput("filtermap");
}

void CCameraTranslator::MakeDOFAttributes(CExtensionAttrHelper &helper)
{
   helper.MakeInput("focus_distance");
   helper.MakeInput("aperture_size");
   helper.MakeInput("aperture_blades");
   helper.MakeInput("aperture_blade_curvature");
   helper.MakeInput("aperture_rotation");

   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "aiEnableDOF";
   data.shortName = "ai_edof";
   helper.MakeInputBoolean(data);
}

