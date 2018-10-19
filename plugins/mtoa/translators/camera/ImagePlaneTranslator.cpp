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

void CImagePlaneTranslator::Export(AtNode *node)
{
   //CNodeTranslator::Export(imagePlane);
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

   if (validCamera)
   {
      MFnCamera fnCamera(pathCamera);
      lensSqueeze = fnCamera.findPlug("lensSqueezeRatio").asDouble();   
   }

   
   AtNode *imagePlaneShader = GetArnoldNode();

   // check if the image plane should be created
   int displayMode = fnRes.findPlug("displayMode").asInt();
   AiNodeSetInt(imagePlaneShader, "displayMode", displayMode);

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
   
   double planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
   double planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
   
   double coverageX = fnRes.findPlug("coverageX", &status).asDouble();
   double coverageY = fnRes.findPlug("coverageY", &status).asDouble();

   double coverageOriginX = fnRes.findPlug("coverageOriginX", &status).asDouble();
   double coverageOriginY = fnRes.findPlug("coverageOriginY", &status).asDouble();

   double offsetX = fnRes.findPlug("offsetX", &status).asDouble();
   double offsetY = fnRes.findPlug("offsetY", &status).asDouble();
   

   unsigned int iWidth = 1;
   unsigned int iHeight = 1;

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

      MString resolvedFilename = imageName; // do we need to do anything else to resolve the filename ?


      MString colorSpace = fnRes.findPlug("colorSpace").asString();
      
      bool requestUpdateTx = (colorSpace != m_colorSpace);
      m_colorSpace = colorSpace;

      if (!requestUpdateTx)
      {
         // Color Space is the same, so let's check if the filename was modified
         int prevFilenameLength = prevFilename.length();

         if (prevFilenameLength > 0)
         {
            // compare against previous filename to see if we need to re-generate the TX
            if (prevFilenameLength > 3 && prevFilename.substring(prevFilenameLength - 3, prevFilenameLength - 1) == MString(".tx"))
            {
               // Previous Filename was .tx, either because of "use existing tx", 
               // or because it's explicitely targeting the .tx
               MString prevBasename = prevFilename.substring(0, prevFilenameLength - 4);

               int dotPos = resolvedFilename.rindexW(".");
               if (dotPos > 0)
               {
                  MString basename = resolvedFilename.substring(0, dotPos - 1);
                  
                  // Let's compare the basenames (without extension)
                  if (prevBasename != basename)
                  {
                     // the basename was modified, this needs an update of TX
                     requestUpdateTx = true;
                  } else
                  {
                     //basename hasn't changed. However, I'm probably setting it back to non-tx here
                     // so let's keep the previous one (where Use Tx was applied)
                     resolvedFilename = prevFilename;
                  }
               }
            } else
            {
               // if previous filename and new one are exactly identical, it's useless to update Tx
               requestUpdateTx = (prevFilename != resolvedFilename);
            }
         } else if (resolvedFilename.length() > 0)
         {
            requestUpdateTx = true;
         }
      }

      AiNodeSetStr(imagePlaneShader, "filename", resolvedFilename.asChar());

      // only set the color_space if the texture isn't a TX
      AiNodeSetStr(imagePlaneShader, "color_space", "");
      if (resolvedFilename.length() > 4)
      {
         MString extension = resolvedFilename.substring(resolvedFilename.length() - 3, resolvedFilename.length() - 1);

         if (extension != ".tx" && extension !=  ".TX")
            AiNodeSetStr(imagePlaneShader, "color_space", colorSpace.asChar());
      }
      AiNodeSetPtr(imagePlaneShader, "sourceTexture", NULL);

      ProcessParameter(imagePlaneShader, "offscreenColor", AI_TYPE_RGBA);
      /* TODO: in case we want to force an opaque alpha for non-zero colors
      if (!AiNodeIsLinked(imagePlaneShader, "offscreenColor"))
      {
         AtRGBA offscreenColor = AiNodeGetRGBA(imagePlaneShader, "offscreenColor");
         if (offscreenColor.r > AI_EPSILON || offscreenColor.g > AI_EPSILON || offscreenColor.b > AI_EPSILON)
         {
            AiNodeSetRGBA(imagePlaneShader, "offscreenColor", offscreenColor.r, offscreenColor.g, offscreenColor.b, 1.f);
         }

      }*/

      if (requestUpdateTx)
      {
         m_impl->m_session->RequestUpdateTx();
      }
   } else if (type == 1)
   {
      coverageX = coverageY = 1.0;
      coverageOriginX = coverageOriginY = 0.0;
      scaleX = planeSizeX;
      scaleY = planeSizeY;

      MPlug sourceTexturePlug  = fnRes.findPlug("sourceTexture");
      sourceTexturePlug.connectedTo(conn, true, false);
      if (conn.length())
      {
         AiNodeSetPtr(imagePlaneShader, "sourceTexture", ExportConnectedNode(conn[0]));
      }
   }

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

void CImagePlaneTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "MayaImagePlane");
   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiAutoTx";
   data.shortName = "autotx";
   data.channelBox = false;
   data.keyable = false;       

   helper.MakeInputBoolean(data);

   data.defaultValue.RGBA() = AI_RGBA_ZERO;
   data.linkable = true;
   data.name = "aiOffscreenColor";
   data.shortName = "ai_offrscreen_color";
   helper.MakeInputRGBA(data);
}

AtNode*  CImagePlaneTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaImagePlane");

}
