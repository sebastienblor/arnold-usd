#include "ImagePlaneTranslator.h"
#include "CameraTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "utils/MayaUtils.h"
#include "utils/ConstantStrings.h"
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
   MDagPath camera = GetSessionOptions().GetExportCamera();
   MFnDependencyNode camNode (camera.node());
   m_camera =  camNode.name();

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
      AiNodeSetRGBA(colorCorrect, str::input, 0.f, 0.f, 0.f, 0.f);
      AiNodeSetFlt(colorCorrect, str::alpha_multiply, 0.f);      
   }
   else
   {
      if (displayMode == 5 ||displayMode == 4) 
      {
         AtNode *rgba_to_float = GetArnoldNode("conv");
         if (rgba_to_float == NULL)
            rgba_to_float = AddArnoldNode("rgba_to_float", "conv");

         AiNodeSetStr(rgba_to_float, str::mode, (displayMode == 5) ? str::a : str::luminance);
         AiNodeLink(rgba_to_float, str::input, colorCorrect);
         AiNodeLink(uv_transform, str::input, rgba_to_float);

      } else
         AiNodeLink(uv_transform, str::input, colorCorrect);

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
      AiNodeLink(image, str::passthrough, uv_transform);

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
      MString prevFilename = AiNodeGetStr(image, str::filename).c_str();
      AtString prevColorSpace = AiNodeGetStr(image, str::color_space);

      MString resolvedFilename = imageName; // do we need to do anything else to resolve the filename ?


      AiNodeSetStr(image, str::filename, AtString(resolvedFilename.asChar()));

      if (!m_impl->m_session->IsColorManagementEnabled())
      {
         // If color management is disabled in the Maya preferences, we want an empty color space here
         AiNodeSetStr(image, str::color_space, AtString());
      } else if (m_impl->m_session->IsFileExport() && (GetSessionOptions().outputAssMask() & AI_NODE_COLOR_MANAGER) == 0)
      {   
         // if the export option for color managers is turned off, consider that color management is disabled #2995
         // here we want to reset the color_space attribute so that it's left to arnold's "automatic" default mode,
         // we don't want to force it to an empty string which behaves differently (see #MTOA-727)
         AiNodeResetParameter(image, str::color_space);
      }
      else 
      {
         // only set the color_space if the texture isn't a TX. Otherwise force it to an empty value (passthrough)
         AiNodeSetStr(image, str::color_space, AtString(FindMayaPlug("colorSpace").asString().asChar()));
         if (resolvedFilename.length() > 4)
         {
            MString extension = resolvedFilename.substring(resolvedFilename.length() - 3, resolvedFilename.length() - 1);

            if (extension == ".tx" || extension ==  ".TX")
            {
               AiNodeSetStr(image, str::color_space, AtString());
            }
         }
      }   

      AtString colorSpace = AiNodeGetStr(image, str::color_space);
      if (GetSessionOptions().GetAutoTx() && 
            ((!IsExported()) || (colorSpace != prevColorSpace) || (prevFilename != resolvedFilename)))
         RequestTxUpdate(std::string(resolvedFilename.asChar()), std::string(colorSpace.c_str()));   

      AiNodeSetBool(image, str::ignore_missing_textures, true);
      ProcessParameter(image, "missing_texture_color", AI_TYPE_RGBA, "aiOffscreenColor");
      
      if (coverageOriginX < 0)
         coverageX = AiMax((double)AI_EPSILON, coverageX + coverageOriginX);
      if (coverageOriginY < 0)
         coverageY = AiMax((double)AI_EPSILON, coverageY + coverageOriginY);
      
      AiNodeSetVec2(uv_transform, str::coverage,  (float)iWidth / (float)coverageX , (float)iHeight / (float)coverageY);

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
      AiNodeSetStr(uv_transform, str::wrap_frame_u, str::color);
      AiNodeSetStr(uv_transform, str::wrap_frame_v, str::color);
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
            AiNodeLink(sourceTexture, str::passthrough, uv_transform);
         }

      }
      coverageX = coverageY = 1.0;
      coverageOriginX = coverageOriginY = 0.0;
      scaleX = planeSizeX;
      scaleY = planeSizeY;
      AiNodeSetStr(uv_transform, str::wrap_frame_u, str::periodic);
      AiNodeSetStr(uv_transform, str::wrap_frame_v, str::periodic);
      AiNodeResetParameter(uv_transform, str::coverage);
   }

   float rotate = FindMayaPlug("rotate").asFloat() * AI_RTOD;
   AiNodeSetFlt(uv_transform, str::rotate, rotate);
   AiNodeSetVec2(uv_transform, str::translate_frame, (float)offsetX, (float)offsetY);
   AiNodeSetVec2(uv_transform, str::scale_frame, 1.f / AiMax(AI_EPSILON, (float)scaleX), 1.f / AiMax(AI_EPSILON, (float)(scaleY)));
   
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