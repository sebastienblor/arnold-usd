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

enum ImagePlaneParams {
    p_filename,
    p_color,
    p_display_mode,
    p_colorGain,
    p_colorOffset,
    p_alphaGain,
    p_coverage,
    p_translate,
    p_camera
};

typedef struct AtImageData
{
   AtTextureHandle* texture_handle;
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

node_parameters
{
   AiParameterStr("filename", "");
   AiParameterRGB("color", 1.0f, 0.0f, 0.0f);
   AiParameterEnum("displayMode", 1, display_mode_enum);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("alphaGain", 1.0f);
   AiParameterVec2("coverage", 1.0f, 1.0f);
   AiParameterVec2("translate", 0.0f, 0.0f);

   AiParameterNode("camera", NULL); 

   AiMetaDataSetBool(mds, "colorGain", "always_linear", true);
   AiMetaDataSetBool(mds, "colorOffset", "always_linear", true);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AiNodeSetLocalData(node, idata);    
}

node_update
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);
   AiTextureHandleDestroy(idata->texture_handle);
   idata->texture_handle = AiTextureHandleCreate(AiNodeGetStr(node, "filename"));
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
   
   AtRGB color = AiShaderEvalParamRGB(p_color);
   AtRGB colorGain = AiShaderEvalParamRGB(p_colorGain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_colorOffset);
   float alphaGain = AiShaderEvalParamFlt(p_alphaGain);
   AtVector2 coverage = AiShaderEvalParamVec2(p_coverage);
   AtVector2 translate = AiShaderEvalParamVec2(p_translate);
   int displayMode = AiShaderEvalParamInt(p_display_mode);
   
   AtRGBA result; 
   result.r = color.r;
   result.g = color.g;
   result.b = color.b;
   result.a = 1.0f;

   if (idata->texture_handle != NULL && (coverage.x != 1.0f || coverage.y != 1.0f))
   {
       float inU = sg->u;
       float inV = sg->v;

       float outU = inU;
       float outV = inV;

       int wrapU = 0;
       int wrapV = 0;
       // If coverage.x or coverage.y are <= 1.0f
       //   check of the wrapped u or v coordinades respectively wraps in a valid range
       // If wrap is off, check incoming coordinate is in the range [0, 1]
       if (mod(outU, 1.0f) > coverage.x ||
           mod(outV, 1.0f) > coverage.y ||
           (!wrapU && (outU < 0 || outU > 1)) ||
           (!wrapV && (outV < 0 || outV > 1)))
       {
          // we are out of the texture frame, return invalid u,v
          outU = -1000000.0f;
          outV = -1000000.0f;
       }
       else
       {
          if (coverage.x < 1.0f)
          {
             outU = mod(outU, 1.0f);
          }

          if (coverage.y < 1.0f)
          {
             outV = mod(outV, 1.0f);
          }

          outU -= translate.x;
          outV -= translate.y;

          outU /= coverage.x;
          outV /= coverage.y;

       }

       sg->u = outU;
       sg->v = outV;

       // do texture lookup
       AtTextureParams texparams;
       AiTextureParamsSetDefaults(&texparams);
       // setup filter?
       texparams.wrap_s = AI_WRAP_BLACK;
       texparams.wrap_t = AI_WRAP_BLACK;

       result = AiTextureHandleAccess(sg, idata->texture_handle, &texparams, NULL);
       //AtRGBA result = color;
       sg->u = inU;
       sg->v = inV;
   }
   else if (idata->texture_handle != NULL)
   {   
       // do texture lookup
       AtTextureParams texparams;
       AiTextureParamsSetDefaults(&texparams);
       // setup filter?
       texparams.wrap_s = AI_WRAP_BLACK;
       texparams.wrap_t = AI_WRAP_BLACK;

       result = AiTextureHandleAccess(sg, idata->texture_handle, &texparams, NULL);
   }
   if (displayMode == 2)
   {
       result.a = 1.0f;
   }
   if (displayMode > 2)
   {
      sg->out_opacity = result.a;
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
   sg->out_opacity *= alphaGain;
}
