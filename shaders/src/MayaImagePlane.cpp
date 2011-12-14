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
   AiParameterSTR("filename", "");
   AiParameterRGB("color", 1.0f, 0.0f, 0.0f);
   AiParameterENUM("displayMode", 1, display_mode_enum);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("alphaGain", 1.0f);
   AiParameterPNT2("coverage", 1.0f, 1.0f);
   AiParameterPNT2("translate", 0.0f, 0.0f);

   AiParameterNODE("camera", NULL); 
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
        if (node->base_node == mytype && AiNodeGetPtr(node, "camera") != cam)
        {
            //AiNodeSetInt(node, "visibility", 0);
        }
    }

    AiNodeIteratorDestroy(iter);
    */
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   const char *filename = AiShaderEvalParamStr(p_filename);
   AtRGB color = AiShaderEvalParamRGB(p_color);
   AtRGB colorGain = AiShaderEvalParamRGB(p_colorGain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_colorOffset);
   float alphaGain = AiShaderEvalParamFlt(p_alphaGain);
   AtPoint2 coverage = AiShaderEvalParamPnt2(p_coverage);
   AtPoint2 translate = AiShaderEvalParamPnt2(p_translate);
   int displayMode = AiShaderEvalParamInt(p_display_mode);
   
   AtRGBA result; 
   result.r = color.r;
   result.g = color.g;
   result.b = color.b;
   result.a = 1.0f;

   if (strlen(filename) != 0 && (coverage.x != 1.0f || coverage.y != 1.0f))
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

       result = AiTextureAccess(sg, filename, &texparams);
       //AtRGBA result = color;
       sg->u = inU;
       sg->v = inV;
   }
   else if (strlen(filename) != 0)
   {   
       // do texture lookup
       AtTextureParams texparams;
       AiTextureParamsSetDefaults(&texparams);
       // setup filter?
       result = AiTextureAccess(sg, filename, &texparams);
   }
   if (displayMode == 2)
   {
       result.a = 1.0f;
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
       sg->out.RGBA = result;
       return;
   }
  
   result.r = (result.r * colorGain.r) + colorOffset.r;
   result.g = (result.g * colorGain.g) + colorOffset.g;
   result.b = (result.b * colorGain.b) + colorOffset.b;
   result.a = (result.a * alphaGain);
   sg->out.RGBA = result;
}
