
#include "Cameras.h"
#include "utils/AttrHelper.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>

#include <maya/MFnCamera.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MImage.h>

using namespace std;

double MM_TO_INCH = 0.03937;


void CCameraTranslator::ExportImagePlane(AtUInt step)
{
    MPlugArray connectedPlugs;
    MPlug      imagePlanePlug;
    MPlug      imagePlaneNodePlug;
    MStatus    status;

    // first we get the image planes connected to this camera
    imagePlanePlug = m_fnNode.findPlug("imagePlane");

    if (imagePlanePlug.numConnectedElements() > 0)
    {
        for(AtUInt ips = 0; (ips < imagePlanePlug.numElements()); ips++)
        {
            imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
            imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);
            MObject resNode = connectedPlugs[0].node(&status);

            if (status)
            {
                // get the dependency node of the image plane
                MFnDependencyNode fnRes(resNode);

                // check if the image plane should be created
                int displayMode = fnRes.findPlug("displayMode", &status).asInt();
                if (displayMode > 1)
                {

                    MString imagePlaneName(m_fnNode.partialPathName());
                    imagePlaneName += "_IP_";
                    imagePlaneName += ips;
                    MString imageName;
                    MImage mImage;

                    //type: 0 == Image, 1 == Texture, 2 == Movie
                    int type = fnRes.findPlug("type", &status).asInt();
                
                    if (type == 2)//Not supporting type Movie for now....
                    {
                        AiMsgWarning("Image Planes of type Movie are unsupported");
                        return;
                    }

                    if (type == 0)//If we have an Image type lets get the image.
                    {
                        // get data
                        if ((!m_motion) || (step == 0))
                        {
                            MString frameNumber("0");
                            frameNumber += m_scene->GetCurrentFrame() + fnRes.findPlug("frameOffset").asInt();
                            imageName = MRenderUtil::exactFileTextureName(imageName, fnRes.findPlug("useFrameExtension").asBool(), frameNumber);
                            imageName = MRenderUtil::exactImagePlaneFileName(resNode);
                            mImage = MImage();
                            mImage.readFromFile(imageName);
                        }
                    }
                             
                    double planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
                    double planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();

                    int lockedToCamera = fnRes.findPlug("lockedToCamera", &status).asInt();
                    if (!lockedToCamera)
                    {
                        planeSizeX = fnRes.findPlug("width", &status).asDouble();
                        planeSizeY = fnRes.findPlug("height", &status).asDouble();
                    }

                    double planeDepth = fnRes.findPlug("depth").asDouble();
                    double camFocal = m_fnNode.findPlug("focalLength").asDouble();
                    double camScale = m_fnNode.findPlug("cameraScale").asDouble();
                    double lensSqueeze = m_fnNode.findPlug("lensSqueezeRatio").asDouble();

                    float ipCoverageX = 1.0f;
                    float ipCoverageY = 1.0f;
                    float ipTranslateX = 0.0f;
                    float ipTranslateY = 0.0f;

                    unsigned int iWidth = 0;
                    unsigned int iHeight = 0;
                    if (type == 0)
                    {
                        //0:Fill 1:Best 2:Horizontal 3:Vertical 4:ToSize
                        mImage.getSize(iWidth, iHeight);

                        int fit = fnRes.findPlug("fit", &status).asInt();                  
                        double planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;
                        double iAspect = double(iWidth) / iHeight;  

                        if (fit == 1)//Best fit
                        {
                            if (iAspect > 1)
                                fit = 2;
                            else
                                fit = 3;
                        }

                        bool clip = false;

                        if (fit == 0)//Fill: Here the image is cropped to fit if aspect is off
                        {
                            if (iAspect != planeAspect)
                            {
                                clip = true;
                            }
                        }
                        else if (fit == 2)//Horizontal fit
                        {
                            if (iAspect > 1)//Double check here against the image aspect
                                planeSizeY = (planeSizeX * lensSqueeze) / iAspect;

                            planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;
                            if (iAspect != planeAspect)
                            {
                                clip = true;
                            }
                        }
                        else if (fit == 3)//Vertical fit
                        {
                            if (iAspect < 1)
                                planeSizeX = planeSizeY / iAspect;

                            planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;
                            if (iAspect != planeAspect)
                            {
                                clip = true;
                            }
                        }
                        else if (fit == 5)//To Size: Here the image is squashed or stretched to size if aspect is off
                        {
                            double squeezeCorrection = fnRes.findPlug("squeezeCorrection").asDouble();
                            if (squeezeCorrection < 0.0001f)
                                squeezeCorrection = 1.0f;
                            planeSizeX /= squeezeCorrection;
                        }

                        if (clip)
                        {
                            if (planeAspect < iAspect)//We have some horizontal clipping
                            {
                                ipCoverageX = static_cast<float>((iAspect / planeAspect));
                                ipTranslateX = (1 - ipCoverageX) * 0.5f;
                            }
                            else//We have vertical clipping
                            {
                                ipCoverageY = static_cast<float>((planeAspect / iAspect));
                                ipTranslateY = (1 - ipCoverageY) * 0.5f;
                            }
                        }
                    }

                    double ipWidth = planeSizeX;
                    double ipHeight = planeSizeY;

                    if (lockedToCamera)
                    {
                        ipWidth = (planeSizeX * planeDepth * lensSqueeze) / ((camFocal * MM_TO_INCH) / camScale);
                        ipHeight = (planeSizeY * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
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
                      
                        //UV Values
                        //Use the coverage options as well as the coverage origin options here
                        //to manipulate the UV's of the image plane.
                        double uMin(0), vMin(0);
                        double uMax(1.0f), vMax(1.0f);

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

                        AtNode* imagePlaneShader = AiNode("MayaImagePlane");
                        if (type == 0)
                        {
                            AiNodeSetStr(imagePlaneShader, "filename", imageName.asChar());
                            AiNodeSetInt(imagePlaneShader, "displayMode", displayMode);
                            AiNodeSetPnt2(imagePlaneShader, "coverage", ipCoverageX, ipCoverageY);
                            AiNodeSetPnt2(imagePlaneShader, "translate", ipTranslateX, ipTranslateY);
                        }
                        else if (type == 1)
                        {
                            MPlug colorPlug  = fnRes.findPlug("sourceTexture");
                            MPlugArray conn;
                            colorPlug.connectedTo(conn, true, false);
                            if (conn.length())
                            {
                                MPlug outputPlug = conn[0];
                                AiNodeLink(m_scene->ExportShader(outputPlug), "color", imagePlaneShader);
                            }
                        }
                        
                        AiNodeSetPtr(imagePlane, "shader", imagePlaneShader);
                        AiNodeSetBool(imagePlane, "opaque", 0);
                      
                        // Check if we have a texture file or a simple color
                        // in our colorGain and colorOffset
                        if (type == 0)
                        {
                            colorPlug  = fnRes.findPlug("colorGain");
                            colorPlug.connectedTo(conn, true, false);
                            if (!conn.length())
                                AiNodeSetRGB(imagePlaneShader, "colorGain", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
                            else
                            {
                                MPlug outputPlug = conn[0];
                                AiNodeLink(m_scene->ExportShader(outputPlug), "colorGain", imagePlaneShader);
                            }

                            colorPlug  = fnRes.findPlug("colorOffset");
                            colorPlug.connectedTo(conn, true, false);
                            if (!conn.length())
                                AiNodeSetRGB(imagePlaneShader, "colorOffset", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
                            else
                            {
                                MPlug outputPlug = conn[0];
                                AiNodeLink(m_scene->ExportShader(outputPlug), "colorOffset", imagePlaneShader);
                            }

                            float alphaGain = fnRes.findPlug("alphaGain", &status).asFloat();
                            AiNodeSetFlt(imagePlaneShader, "alphaGain", alphaGain);
                        }
                    }

                    AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());

                    AtMatrix offsetMatrix;
                    AtMatrix scaleMatrix;
                    AtMatrix rotationMatrix;
                    AtMatrix imagePlaneMatrix;
                    AtVector offsetVector;
                    AtVector scaleVector;
                   
                    double offsetX = fnRes.findPlug("offsetX", &status).asDouble();
                    double offsetY = fnRes.findPlug("offsetY", &status).asDouble();
                    double offsetZ = -planeDepth;
                   
                    //if the plane is locked to the camera find and use the offset values
                    //otherwise it isn't locked so use the center attribute values to offset the plane
                    if (lockedToCamera)
                    {
                        offsetX = (offsetX * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
                        offsetY = (offsetY * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
                    }
                    else
                    {
                        double centerX = fnRes.findPlug("centerX", &status).asDouble();
                        double centerY = fnRes.findPlug("centerY", &status).asDouble();
                        double centerZ = fnRes.findPlug("centerZ", &status).asDouble();
                        offsetX = centerX;
                        offsetY = centerY;
                        offsetZ = centerZ;
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
                            AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
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
        }
    }
}

void CCameraTranslator::ExportCameraData(AtNode* camera)
{
   AtMatrix matrix;

   AiNodeSetFlt(camera, "near_clip", m_fnNode.findPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip", m_fnNode.findPlug("farClipPlane").asFloat());
   
   if (m_fnNode.findPlug("enableDOF").asBool())
   {
      AiNodeSetFlt(camera, "focal_distance", m_fnNode.findPlug("focal_distance").asFloat());
      AiNodeSetFlt(camera, "aperture_size", m_fnNode.findPlug("aperture_size").asFloat());
      AiNodeSetInt(camera, "aperture_blades", m_fnNode.findPlug("aperture_blades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation", m_fnNode.findPlug("aperture_rotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature", m_fnNode.findPlug("aperture_blade_curvature").asFloat());
   }
   
   if (m_scene->IsMotionBlurEnabled())
   {
      float halfShutter = m_scene->GetShutterSize() * 0.5f;
      AiNodeSetFlt(camera, "shutter_start", 0.5f - halfShutter);
      AiNodeSetFlt(camera, "shutter_end", 0.5f + halfShutter);
      AiNodeSetInt(camera, "shutter_type", m_scene->GetShutterType());
   }

   GetMatrix(matrix);
   
   if (m_motion)
   {
      AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
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

   m_cameraData.deviceAspect = deviceAspect;
   return deviceAspect;
}

void CCameraTranslator::ExportOrthoFilmback(AtNode* camera)
{
   double deviceAspect = GetDeviceAspect();

   double width = m_fnCamera.orthoWidth();

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kFillFilmFit){
      if (deviceAspect >= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kOverscanFilmFit){
      if (deviceAspect <= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if (filmFit == MFnCamera::kHorizontalFilmFit)
   {
      width = width/2;
   }
   else if (filmFit == MFnCamera::kVerticalFilmFit)
   {
      width = (width/2)*deviceAspect;
   }
   
   MVectorArray filmTransforms = GetFilmTransform(width, false);
   
   m_cameraData.width = width;
   m_cameraData.factorX = filmTransforms[0].x;
   m_cameraData.factorY = filmTransforms[0].y;
   m_cameraData.focalLength = 35;

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(filmTransforms[0].x), static_cast<float>(filmTransforms[0].y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(filmTransforms[1].x), static_cast<float>(filmTransforms[1].y));
}

void CCameraTranslator::ExportPerspFilmback(AtNode* camera)
{

   double deviceAspect = GetDeviceAspect();
   float fov = 1.0f;
   double cameraAspect = m_fnCamera.aspectRatio();
   double lensSqueeze = m_fnCamera.lensSqueezeRatio();
   double focalLength = m_fnCamera.focalLength();
   double cameraScale = m_fnCamera.cameraScale();
   
   double apertureX = m_fnCamera.horizontalFilmAperture();
   double apertureY = m_fnCamera.verticalFilmAperture();
   
   double deviceApertureX = apertureX;
   double deviceApertureY = apertureY;
   
   double filmFitOffset = m_fnCamera.filmFitOffset();
   double filmOffsetX = m_fnCamera.horizontalFilmOffset();
   double filmOffsetY = m_fnCamera.verticalFilmOffset();
   double factorX = 0.0;
   double factorY = 0.0;

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kFillFilmFit){
      filmFitOffset = 0;
      if ((cameraAspect * lensSqueeze) < deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kOverscanFilmFit){
      filmFitOffset = 0;
      if ((cameraAspect * lensSqueeze) > deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //HORIZONTAL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kHorizontalFilmFit)
   {
      //Find the camera's Field Of View with Lens Squeeze and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * apertureX * lensSqueeze) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new vertical aperture based off of the Render Aspect Ratio
      deviceApertureY = apertureX / (deviceAspect / lensSqueeze);

      //Apply the film fit offset if the camera's aspect ratio is greater than the render aspect ratio
      if ((cameraAspect * lensSqueeze) > deviceAspect && filmFitOffset != 0.0f)
      {
         factorY = ((apertureY - deviceApertureY) * filmFitOffset) / deviceApertureY;
      }
   }

   //VERTICAL FILM RESOLUTION GATE
   else if (filmFit == MFnCamera::kVerticalFilmFit)
   {
      //Find the camera's Field Of View with the Render Aspect Ratio and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * (deviceAspect * apertureY)) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new horizontal aperture based off of the Render Aspect Ratio
      deviceApertureX = apertureY * (deviceAspect / lensSqueeze);
      
      //Apply the film fit offset if the camera's aspect ratio is less than the render aspect ratio
      if ((cameraAspect * lensSqueeze) < deviceAspect && filmFitOffset != 0.0f)
      {
         factorX = ((apertureX - deviceApertureX) * filmFitOffset) / deviceApertureX;
      }
   }
   
   //If we have any filmOffsets we add them here
   if (filmOffsetX!=0.0f || filmOffsetY!=0.0f)
   {
      factorX += (filmOffsetX/deviceApertureX)*2;
      factorY += (filmOffsetY/deviceApertureY)*2;
   }
   
   MVectorArray filmTransforms = GetFilmTransform();
   MVector minPoint = filmTransforms[0];
   MVector maxPoint = filmTransforms[1];

   //Add on any offsets from filmOffsetX or Y, or filmFitOffset
   minPoint += MVector(factorX, factorY);
   maxPoint += MVector(factorX, factorY);

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(minPoint.x), static_cast<float>(minPoint.y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(maxPoint.x), static_cast<float>(maxPoint.y));
   
   m_cameraData.apertureX = deviceApertureX;
   m_cameraData.apertureY = deviceApertureY;
   m_cameraData.fov = fov;
   m_cameraData.scale = cameraScale;
   m_cameraData.focalLength = focalLength;
   m_cameraData.factorX = factorX;
   m_cameraData.factorY = factorY;
   m_cameraData.lensSqueeze = lensSqueeze;

   //return fov;
}

MVectorArray CCameraTranslator::GetFilmTransform(double width, bool persp)
{
   double deviceAspect = GetDeviceAspect();
   double cameraAspect = m_fnCamera.aspectRatio();
   double preScale = m_fnCamera.preScale();
   double filmTranslateX = m_fnCamera.filmTranslateH();
   double filmTranslateY = m_fnCamera.filmTranslateV();
   double filmRollValue = m_fnCamera.filmRollValue();
   //We need a roll attribute from the guys at SolidAngle
   //double filmRollPivotX = m_fnCamera.verticalRollPivot();
   //double filmRollPivotY = m_fnCamera.horizontalRollPivot();
   MFnCamera::RollOrder filmRollOrder = m_fnCamera.filmRollOrder();
   double postScale = m_fnCamera.postScale();

   //2D Transform default Vectors for Perspective
   MVector minPoint(-1, -1);
   MVector maxPoint(1, 1);

   if (persp) // perspective camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;

      filmTranslateX = (filmTranslateX * preScale) / postScale;
      if (cameraAspect > deviceAspect)
         filmTranslateY = ((filmTranslateY/cameraAspect) * 2 * preScale) / postScale;
      else
         filmTranslateY = (filmTranslateY * deviceAspect * preScale) / postScale;
   }
   else //Ortho camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;
      double orthoWidth = m_fnCamera.orthoWidth() / 2;
      
      if (orthoWidth == width)//We are in Horizontal Mode
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
         //ROTATE
      }
   }
   else //Rotate-Translate order
   {
      if (filmRollValue != 0.0f)
      {
         //ROTATE
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

   MVector src[] = {minPoint, maxPoint};
   return MVectorArray(src, 2);
}

// Orthographic Camera
//
void CCameraTranslator::ExportOrtho(AtNode* camera)
{
   ExportCameraData(camera);
   ExportOrthoFilmback(camera);
   ExportImagePlane(0);
}

void CCameraTranslator::ExportOrthoMotion(AtNode* camera, AtInt step)
{
   ExportCameraMBData(camera, step);
   ExportImagePlane(step);
}

// Perspective Camera
//
void CCameraTranslator::ExportPersp(AtNode* camera)
{
   ExportPerspFilmback(camera);
   float fov = m_cameraData.fov;

   ExportCameraData(camera);
   ExportImagePlane(0);

   if (m_motion)
   {
      AtArray* fovs = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(fovs, 0, fov);
      AiNodeSetArray(camera, "fov", fovs);
   }
   else
   {
      AiNodeSetFlt(camera, "fov", fov);
   }
}

void CCameraTranslator::ExportPerspMotion(AtNode* camera, AtInt step)
{
   ExportCameraMBData(camera, step);
   ExportImagePlane(step);

   AtArray* fovs = AiNodeGetArray(camera, "fov");
   AiArraySetFlt(fovs, step, m_cameraData.fov);
}

const char* CCameraTranslator::GetArnoldNodeType()
{
   if (m_fnCamera.isOrtho())
   {
      return "ortho_camera";
   }
   else
   {
      return "persp_camera";
   }
}

void CCameraTranslator::Export(AtNode* camera)
{
   if (m_fnCamera.isOrtho())
   {
      ExportOrtho(camera);
   }
   else
   {
      ExportPersp(camera);
   }
}

void CCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   if (m_fnCamera.isOrtho())
   {
      return ExportOrthoMotion(camera, step);
   }
   else
   {
      return ExportPerspMotion(camera, step);
   }
}

void CCameraTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "persp_camera");
   helper.MakeInput("focal_distance");
   helper.MakeInput("aperture_size");
   helper.MakeInput("aperture_blades");
   helper.MakeInput("aperture_blade_curvature");
   helper.MakeInput("aperture_rotation");
   
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "enableDOF";
   data.shortName = "edof";
   helper.MakeInputBoolean(data);
}
