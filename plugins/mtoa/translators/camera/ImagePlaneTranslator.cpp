#include "ImagePlaneTranslator.h"
#include "CameraTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "utils/MayaUtils.h"
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
      lensSqueeze = fnCamera.findPlug("lensSqueezeRatio", true).asDouble();   
   }
   unsigned int iWidth = 1;
   unsigned int iHeight = 1;

   double scaleX = 1.;
   double scaleY = 1.;

   AtNode *image = GetArnoldNode();
   AtNode *colorCorrect = GetArnoldNode("cc");
   AtNode *uv_transform = GetArnoldNode("uv");

   if (colorCorrect == NULL || uv_transform == NULL)
      return;

   int type = FindMayaPlug("type").asInt();
   
   int displayMode = FindMayaPlug("displayMode").asInt();
   if (type >= 2 || displayMode <= 1)
   {
      AiNodeUnlink(colorCorrect, "input");
      AiNodeSetRGBA(colorCorrect, "input", 0.f, 0.f, 0.f, 0.f);
      AiNodeSetFlt(colorCorrect, "alpha_multiply", 0.f);      
   }
   else
   {
      if (displayMode == 5 ||displayMode == 4) 
      {
         AtNode *rgba_to_float = GetArnoldNode("conv");
         if (rgba_to_float == NULL)
            rgba_to_float = AddArnoldNode("rgba_to_float", "conv");

         AiNodeSetStr(rgba_to_float, "mode", (displayMode == 5) ? "a" : "luminance");
         AiNodeLink(rgba_to_float, "input", colorCorrect);
         AiNodeLink(uv_transform, "input", rgba_to_float);

      } else
         AiNodeLink(uv_transform, "input", colorCorrect);

      ProcessParameter(colorCorrect, "multiply", AI_TYPE_RGB, "colorGain");
      ProcessParameter(colorCorrect, "add", AI_TYPE_RGB, "colorOffset");
      ProcessParameter(colorCorrect, "alpha_multiply", AI_TYPE_FLOAT, "alphaGain");
   }

   
   double planeSizeX = fnRes.findPlug("sizeX", true, &status).asDouble();
   double planeSizeY = fnRes.findPlug("sizeY", true, &status).asDouble();
   
   double coverageX = fnRes.findPlug("coverageX", true, &status).asDouble();
   double coverageY = fnRes.findPlug("coverageY", true, &status).asDouble();

   double coverageOriginX = fnRes.findPlug("coverageOriginX", true, &status).asDouble();
   double coverageOriginY = fnRes.findPlug("coverageOriginY", true, &status).asDouble();

   double offsetX = fnRes.findPlug("offsetX", true, &status).asDouble();
   double offsetY = fnRes.findPlug("offsetY", true, &status).asDouble();

   MPlug colorPlug;
   MPlugArray conn;
   double iAspect = 1.0;
   double planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;


   if (type == 0)
   {
      // Image
      AiNodeLink(image, "passthrough", uv_transform);

      MString imageName;
      MImage mImage;

      imageName = MRenderUtil::exactImagePlaneFileName(imgPlane);
      mImage = MImage();
      mImage.readFromFile(imageName);

      //0:Fill 1:Best 2:Horizontal 3:Vertical 4:ToSize
      mImage.getSize(iWidth, iHeight);
      if (iWidth > 0 && iHeight > 0)
      {
         iAspect = double(iWidth) / iHeight;
      }

      // check if filename has changed. If it has we tell ArnoldSession to update tx
      MString prevFilename = AiNodeGetStr(image, "filename").c_str();

      MString resolvedFilename = imageName; // do we need to do anything else to resolve the filename ?


      MString colorSpace = FindMayaPlug("colorSpace").asString();
      
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

      AiNodeSetStr(image, "filename", resolvedFilename.asChar());

      // only set the color_space if the texture isn't a TX
      AiNodeSetStr(image, "color_space", "");
      if (resolvedFilename.length() > 4)
      {
         MString extension = resolvedFilename.substring(resolvedFilename.length() - 3, resolvedFilename.length() - 1);

         if (extension != ".tx" && extension !=  ".TX")
            AiNodeSetStr(image, "color_space", colorSpace.asChar());
      }
      
      AiNodeSetBool(image, "ignore_missing_textures", true);
      ProcessParameter(image, "missing_texture_color", AI_TYPE_RGBA, "aiOffscreenColor");
      
      if (requestUpdateTx)
      {
         m_impl->m_session->RequestUpdateTx();
      }

      if (coverageOriginX < 0)
         coverageX = AiMax((double)AI_EPSILON, coverageX + coverageOriginX);
      if (coverageOriginY < 0)
         coverageY = AiMax((double)AI_EPSILON, coverageY + coverageOriginY);
      
      AiNodeSetVec2(uv_transform, "coverage",  (float)iWidth / (float)coverageX , (float)iHeight / (float)coverageY);

      if (iAspect != planeAspect)
      {
         FitType fit = (FitType)fnRes.findPlug("fit", true, &status).asInt();

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
            scaleY = iAspect / planeAspect;
         }
         else if (fit == FIT_VERTICAL)
         {
            scaleX =  planeAspect/iAspect;
            scaleY = 1.f;
         }
      }
      ProcessParameter(uv_transform, "wrap_frame_color", AI_TYPE_RGBA, "aiOffscreenColor");
      AiNodeSetStr(uv_transform, "wrap_frame_u", "color");
      AiNodeSetStr(uv_transform, "wrap_frame_v", "color");
   } else
   {
      // Connect shader to uv_transform
      MPlug sourceTexturePlug  = FindMayaPlug("sourceTexture");
      sourceTexturePlug.connectedTo(conn, true, false);
      if (conn.length())
      {
         AtNode *sourceTexture = ExportConnectedNode(conn[0]);
         if (sourceTexture)
         {
            AiNodeLink(sourceTexture, "passthrough", uv_transform);
         }

      }
      coverageX = coverageY = 1.0;
      coverageOriginX = coverageOriginY = 0.0;
      scaleX = planeSizeX;
      scaleY = planeSizeY;
      AiNodeSetStr(uv_transform, "wrap_frame_u", "periodic");
      AiNodeSetStr(uv_transform, "wrap_frame_v", "periodic");
      AiNodeResetParameter(uv_transform, "coverage");
   }

   float rotate = FindMayaPlug("rotate").asFloat() * AI_RTOD;
   AiNodeSetFlt(uv_transform, "rotate", rotate);
   AiNodeSetVec2(uv_transform, "translate_frame", (float)offsetX, (float)offsetY);
   AiNodeSetVec2(uv_transform, "scale_frame", 1.f / AiMax(AI_EPSILON, (float)scaleX), 1.f / AiMax(AI_EPSILON, (float)(scaleY)));
   
   // FIXME we're not supporting coverageOrigin here
   // AiNodeSetVec2(uv_transform, "coverageOrigin", (float)coverageOriginX / (float)iWidth , (float)coverageOriginY / (float)iHeight);
   
   
}

void CImagePlaneTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "image");
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
   AddArnoldNode("image");
   AddArnoldNode("uv_transform", "uv");
   return AddArnoldNode("color_correct", "cc");
   
}

bool CImagePlaneTranslator::RequiresColorCorrect()
{
   int displayMode = FindMayaPlug("displayMode").asInt();
   
   if (displayMode > 3)
      return true;
   
   return !(IsRGBAttrDefault(FindMayaPlug("colorGain"), 1.f, 1.f, 1.f) &&
         IsRGBAttrDefault(FindMayaPlug("colorOffset"), 0.f, 0.f, 0.f) &&
         IsFloatAttrDefault(FindMayaPlug("alphaGain"), 1.f));
}
/*
void CImagePlaneTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "displayMode")
      SetUpdateMode(AI_RECREATE_NODE);

   CNodeTranslator::NodeChanged(node, plug);
}*/