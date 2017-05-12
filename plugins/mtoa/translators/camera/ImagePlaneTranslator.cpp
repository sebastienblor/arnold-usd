#include "ImagePlaneTranslator.h"
#include "CameraTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "session/ArnoldSession.h"
#include "translators/NodeTranslatorImpl.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_ray.h>

#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MString.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MImage.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>
using namespace std;

void CImagePlaneTranslator::Export(AtNode *imagePlane)
{
   CNodeTranslator::Export(imagePlane);
   ExportImagePlane();
}

void CImagePlaneTranslator::SetCamera(MString cameraName)
{
   m_camera = cameraName;
   ExportImagePlane();
}

void CImagePlaneTranslator::ExportImagePlane()
{
   MObject imgPlane = GetMayaObject();
   // get the dependency node of the image plane
   MFnDependencyNode fnRes(imgPlane);
   MStatus status;

   MSelectionList activeList;
   bool validCamera = false;
   
   MDagPath pathCamera;

   if (m_camera.length() > 0)
   {
      activeList.add(m_camera);
      if (activeList.length() > 0)
      {
         activeList.getDagPath(0,pathCamera);         
         if (pathCamera.isValid())
         {
            validCamera = true;
         }
      }
   }
   double lensSqueeze = 1.f;

   bool visible = false;
   if (validCamera)
   {
      MFnCamera fnCamera(pathCamera);
      lensSqueeze = fnCamera.findPlug("lensSqueezeRatio").asDouble();   
   }

   
   AtNode *imagePlaneShader = GetArnoldNode();

   // check if the image plane should be created
   int displayMode = fnRes.findPlug("displayMode").asInt();
   if (displayMode <= 1)
      return;


   MString imageName;
   MImage mImage;

   //type: 0 == Image, 1 == Texture, 2 == Movie
   int type = fnRes.findPlug("type", &status).asInt();

   if (type == 2)//Not supporting type Movie for now....
   {
      AiMsgWarning("[mtoa] [translator %s] Image Planes of type Movie are unsupported", GetTranslatorName().asChar());
      return;
   }

   // UV Values
   double uMin = 0.0f;
   double vMin = 0.0f;
   double uMax = 1.0f;
   double vMax = 1.0f;

   double planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
   double planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
   
   double coverageX = fnRes.findPlug("coverageX", &status).asDouble();
   double coverageY = fnRes.findPlug("coverageY", &status).asDouble();

   double coverageOriginX = fnRes.findPlug("coverageOriginX", &status).asDouble();
   double coverageOriginY = fnRes.findPlug("coverageOriginY", &status).asDouble();

   double offsetX = fnRes.findPlug("offsetX", &status).asDouble();
   double offsetY = fnRes.findPlug("offsetY", &status).asDouble();
   

   unsigned int iWidth = 0;
   unsigned int iHeight = 0;

   double scaleX = 1.;
   double scaleY = 1.;

   if (type == 0)
   {
      imageName = MRenderUtil::exactImagePlaneFileName(imgPlane);
      mImage = MImage();
      mImage.readFromFile(imageName);

      //0:Fill 1:Best 2:Horizontal 3:Vertical 4:ToSize
      mImage.getSize(iWidth, iHeight);
      double iAspect = 1.0;
      if (iWidth > 0 && iHeight > 0)
      {
         iAspect = double(iWidth) / iHeight;
      }
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
            scaleX = 1.f;
            scaleY =iAspect/planeAspect;

         }
         else if (fit == FIT_VERTICAL)
         {
            scaleX =  planeAspect/iAspect;
            scaleY = 1.f;
         }
      }
   }

   // create a flat shader with the needed image
   MPlug colorPlug;
   MPlugArray conn;

   
   if (type == 0)
   {
      // check if filename has changed. If it has we tell ArnoldSession to update tx
      MString prevFilename = AiNodeGetStr(imagePlaneShader, "filename").c_str();

      bool requestUpdateTx = true;
      int prevFilenameLength = prevFilename.length();

      if (prevFilenameLength > 0)
      {
         // arnold filename param
         if (prevFilenameLength > 3 && prevFilename.substring(prevFilenameLength - 3, prevFilenameLength - 1) == MString(".tx"))
         {
            MString prevBasename = prevFilename.substring(0, prevFilenameLength - 4);

            int dotPos = imageName.rindexW(".");
            if (dotPos > 0)
            {
               MString basename = imageName.substring(0, dotPos - 1);
               requestUpdateTx = (prevBasename != basename);
            }
         } else
         {
            // if previous filename and new one are exactly identical, it's useless to update Tx
            requestUpdateTx = (prevFilename != imageName);
         }
      }

      MString colorSpace = fnRes.findPlug("colorSpace").asString();
      
      if (colorSpace != m_colorSpace) requestUpdateTx = true;
      m_colorSpace = colorSpace;

      AiNodeSetStr(imagePlaneShader, "color_space", colorSpace.asChar());

      if (requestUpdateTx)
      {
         AiNodeSetStr(imagePlaneShader, "filename", imageName.asChar());
         m_impl->m_session->RequestUpdateTx();
      }

      AiNodeSetInt(imagePlaneShader, "displayMode", displayMode);
      AiNodeSetVec2(imagePlaneShader, "fitFactor", (float)scaleX, (float)scaleY);
      AiNodeSetVec2(imagePlaneShader, "coverage", (float)coverageX / (float)iWidth , (float)coverageY / (float)iHeight);
      AiNodeSetVec2(imagePlaneShader, "coverageOrigin", (float)coverageOriginX / (float)iWidth , (float)coverageOriginY / (float)iHeight);
      AiNodeSetVec2(imagePlaneShader, "translate", (float)offsetX, (float)offsetY);
      
      colorPlug  = fnRes.findPlug("colorGain");
      colorPlug.connectedTo(conn, true, false);
      if (!conn.length())
        AiNodeSetRGB(imagePlaneShader, "colorGain", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
      else
      {
         AiNodeLink(ExportConnectedNode(conn[0]), "colorGain", imagePlaneShader);
      }

      colorPlug  = fnRes.findPlug("colorOffset");
      colorPlug.connectedTo(conn, true, false);
      if (!conn.length())
         AiNodeSetRGB(imagePlaneShader, "colorOffset", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
      else
      {
         AiNodeLink(ExportConnectedNode(conn[0]), "colorOffset", imagePlaneShader);
      }

      float alphaGain = fnRes.findPlug("alphaGain", &status).asFloat();
      AiNodeSetFlt(imagePlaneShader, "alphaGain", alphaGain);


     float rotate = fnRes.findPlug("rotate", &status).asFloat();
     AiNodeSetFlt(imagePlaneShader, "rotate", rotate);
      
   }
}

void CImagePlaneTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "MayaImagePlane");
   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiAutoTx";
   data.shortName = "autotx";
   helper.MakeInputBoolean(data);
}

AtNode*  CImagePlaneTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaImagePlane");

}
