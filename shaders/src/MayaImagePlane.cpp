#include <ai.h>
#include <ai_metadata.h>
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

#include "MayaUtils.h"
#include <iostream>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(MayaImagePlaneMtd);
namespace {

enum ImagePlaneParams {
    p_filename,
    p_color,
    p_color_space,
    p_display_mode,
    p_colorGain,
    p_colorOffset,
    p_alphaGain,
    p_coverage,
    p_coverageOrigin,
    p_fit_factor,
    p_translate,
    p_rotate,
    p_camera,
    p_sourceTexture
};

typedef struct AtImageData
{
   AtTextureHandle* texture_handle;
   AtString color_space;
   int xres;
   int yres;
   double sinAngle;
   double cosAngle;
   unsigned int iWidth;
   unsigned int iHeight;
   AtNode *sourceTexture;

} AtImageData;

inline float mod(float n, float d)
{
   return (n - (floor(n / d) * d));
}

enum DisplayMode
{
   NONE = 0,
   OUTLINE,
   RGB,
   RGBA,
   LUMINANCE,
   ALPHA
};

const char* display_mode_enum[] =
{
   "none",
   "outline",
   "rgb",
   "rgba",
   "luminance",
   "alpha",
   NULL
};
}

node_parameters
{
   AiParameterStr("filename", "");
   AiParameterRGB("color", 1.0f, 0.0f, 0.0f);
   AiParameterStr("color_space", "");
   AiParameterEnum("displayMode", 1, display_mode_enum);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("alphaGain", 1.0f);
   AiParameterVec2("coverage", 1.0f, 1.0f);
   AiParameterVec2("coverageOrigin", 0.0f, 0.0f);
   AiParameterVec2("fitFactor", 1.0f, 1.0f);
   AiParameterVec2("translate", 0.0f, 0.0f);
   AiParameterFlt("rotate", 0.0f);


   AiParameterNode("camera", NULL); 
   AiParameterNode("sourceTexture", NULL); 

   AiMetaDataSetBool(nentry, "colorGain", "always_linear", true);
   AiMetaDataSetBool(nentry, "colorOffset", "always_linear", true);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

node_initialize
{
    //NONWORKING code that would essentially allow the user to have image planes only render out
    //of their respective cameras.  Not worth the extra overhead but will leave the code here
    //commented out for now.
    /*
    AtNodeIterator* iter = AiUniverseGetNodeIterator(AI_NODE_SHADER);
    const AtNodeEntry* mytype = AiNodeEntryLookUp("MayaImagePlane");
    AtNode* cam = AiUniverseGetCamera();
    while (!AiNodeIteratorFinished(iter))
    {
        AtNode* node = AiNodeIteratorGetNext(iter);
        const char *name = AiNodeGetStr(node, "name");
        if (AiNodeGetNodeEntry(node) == mytype && AiNodeGetPtr(node, "camera") != cam)
        {
            //AiNodeSetInt(node, "visibility", 0);
        }
    }

    AiNodeIteratorDestroy(iter);
    */
   AtImageData *idata = (AtImageData*) AiMalloc(sizeof(AtImageData));
   idata->texture_handle = NULL;
   idata->sourceTexture = NULL;
   AiNodeSetLocalData(node, idata);    
}

node_update
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);

   if (idata->texture_handle)
      AiTextureHandleDestroy(idata->texture_handle);

   idata->color_space = AiNodeGetStr(node, "color_space");

   idata->sourceTexture = (AtNode*)AiNodeGetPtr(node, "sourceTexture");
   idata->xres =  AiNodeGetInt(AiUniverseGetOptions(), "xres");
   idata->yres = AiNodeGetInt(AiUniverseGetOptions(), "yres");
   idata->texture_handle = NULL;
   idata->iWidth = idata->xres;
   idata->iHeight = idata->yres;

   if (!idata->sourceTexture)
   {
      const char *filename = AiNodeGetStr(node, "filename");
      std::string filenameStr(filename);
      if (!filenameStr.empty())
      {
         idata->texture_handle = AiTextureHandleCreate(filename, idata->color_space);
         AiTextureGetResolution(filename, &idata->iWidth, &idata->iHeight);
      }
   }
  
   
   float angle = AiNodeGetFlt(node, "rotate");
   idata->sinAngle = sin(angle);
   idata->cosAngle = cos(angle);
   
}

node_finish
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);
   AiTextureHandleDestroy(idata->texture_handle);
   AiFree(AiNodeGetLocalData(node));
}

shader_evaluate
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);

   if (sg->bounces > 1)
   {
      sg->out.RGBA() = AI_RGBA_ZERO;
      return;
   }
   int displayMode = AiShaderEvalParamInt(p_display_mode);
   if (displayMode <= 1)
   {
      sg->out.RGBA() = AI_RGBA_ZERO;
      return;  
   }


   AtRGB color = AiShaderEvalParamRGB(p_color);
   AtRGB colorGain = AiShaderEvalParamRGB(p_colorGain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_colorOffset);
   float alphaGain = AiShaderEvalParamFlt(p_alphaGain);
   AtVector2 coverage = AiShaderEvalParamVec2(p_coverage);
   AtVector2 coverageOrigin = AiShaderEvalParamVec2(p_coverageOrigin);
   AtVector2 fit_factor = AiShaderEvalParamVec2(p_fit_factor);
   AtVector2 translate = AiShaderEvalParamVec2(p_translate);

   AtRGBA result; 
   result.r = color.r;
   result.g = color.g;
   result.b = color.b;
   result.a = 1.0f;

   if (coverageOrigin.x < 0.f)
   {
      coverage.x *= (1.f + coverageOrigin.x);
      coverageOrigin.x = 0.f;
   } else
   {
      coverage.x = AiMin(1.f, coverage.x/(1.f- coverageOrigin.x));
   }


   if (coverageOrigin.y < 0.f)
   {
      coverage.y *= (1.f + coverageOrigin.y);
      coverageOrigin.y = 0.f;
   } else
   {
      coverage.y = AiMin(1.f, coverage.y/(1.f - coverageOrigin.y));
   }
   float resolutionInvRatio =  (float)idata->yres / (float)idata->xres;
   float imgInvRatio =  (float)idata->iHeight / (float)idata->iWidth;

   if (resolutionInvRatio < 1.f)
      translate.x *= imgInvRatio;
   else
      translate.y /= imgInvRatio;

   if (idata->texture_handle != NULL || idata->sourceTexture != NULL)
   {  
      float inU = sg->u;
      float inV = sg->v;

      float sx = -1.f + float((sg->x + sg->px) * (2.0f / idata->xres));
      float sy =  1.f - float((sg->y + sg->py) * (2.0f /idata->yres));

      sx *= fit_factor.x;
      sy *= fit_factor.y;

      float origSx = sx;
      float origSy = sy* imgInvRatio;

      
      sx = (float)(idata->cosAngle * origSx - idata->sinAngle * origSy);
      sy = (float)(idata->sinAngle * origSx + idata->cosAngle * origSy);

      sy /= imgInvRatio;

      sx = sx * 0.5f + 0.5f;
      sy = sy * 0.5f + 0.5f;
      
      sx *= coverage.x;
      sy *= coverage.y;
      sx = sx + ((1.f - sx)*coverageOrigin.x);      
      sy = sy + ((1.f - sy)*coverageOrigin.y);
      sy = 1.0f - sy;

      sg->u = (sx - translate.x);
      sg->v =  (1.0f - ((sy + translate.y)));

      if (idata->sourceTexture != NULL)
      {
         AiShaderEvaluate(idata->sourceTexture, sg);
         result = sg->out.RGBA();
         sg->out.RGBA() = AI_RGBA_ZERO;
      }
      else if (idata->texture_handle != NULL)
      {
         // do texture lookup
         AtTextureParams texparams;
         AiTextureParamsSetDefaults(texparams);
         // setup filter?
         texparams.wrap_s = AI_WRAP_BLACK;
         texparams.wrap_t = AI_WRAP_BLACK;

         result = AiTextureHandleAccess(sg, idata->texture_handle, texparams, NULL);
      }

      sg->u = inU;
      sg->v = inV;
   }

   if (displayMode == 2)
   {
       result.a = 1.0f;
   }
   if (displayMode > 2)
   {
      sg->out.RGBA().a = result.a;
      //sg->out_opacity = result.a;
   }
   if (displayMode == 4)
   {
        result.r = Luminance(result);
        result.g = result.r;
        result.b = result.r;
   }
   if (displayMode == 5)
   {
       result.r = result.a;
       result.g = result.a;
       result.b = result.a;
       result.a = 1.0f;
       sg->out.RGBA() = result;
       return;
   }
  
   result.r = (result.r * colorGain.r) + colorOffset.r;
   result.g = (result.g * colorGain.g) + colorOffset.g;
   result.b = (result.b * colorGain.b) + colorOffset.b;
   result.a = (result.a * alphaGain);
   sg->out.RGBA() = result;
   
}
