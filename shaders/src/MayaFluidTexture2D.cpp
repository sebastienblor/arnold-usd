#include <ai.h>

#include "MayaFluidData.h"
#include "FluidUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidTexture2DMtd);

struct MayaFluidTexture2D {
   CMayaFluidData<false>* fluidData;

   GradientDescription<AtRGB> colorGradient;
   GradientDescription<AtRGB> incandescenceGradient;
   GradientDescription<float, false, false> opacityGradient;  

   int dropoffShape;
   int filterType;
   

   static void* operator new(size_t size)
   {
      return AiMalloc(size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }

   MayaFluidTexture2D() : fluidData(0)
   {

   }

   ~MayaFluidTexture2D()
   {
      if (fluidData)
         delete fluidData;
   }
};

enum FluidTexture2DParams{
   p_edge_dropoff = 0
};


const char* filterType2DEnums[] = {"Closest", "Linear", 0};

node_parameters
{
   AiParameterEnum("filter_type", FT_LINEAR, filterType2DEnums);

   InitializeFluidShaderAdditionalParameters(params);
   InitializeFluidShaderParameters(params, false);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

node_initialize
{
   AiNodeSetLocalData(node, new MayaFluidTexture2D());
}

node_update
{
   MayaFluidTexture2D* data = reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
   if (data->fluidData)
      delete data->fluidData;
   data->fluidData = new CMayaFluidData<false>(node);

   data->colorGradient.type = AiNodeGetInt(node, "color_gradient_type");   
   data->colorGradient.inputBias = AiNodeGetFlt(node, "color_gradient_input_bias");
   data->colorGradient.ReadValues(node, "color_gradient_values",
                                  AiNodeGetArray(node, "color_gradient_positions"),
                                  AiNodeGetArray(node, "color_gradient_interps"));
   data->incandescenceGradient.type = AiNodeGetInt(node, "incandescence_gradient_type");   
   data->incandescenceGradient.inputBias = AiNodeGetFlt(node, "incandescence_gradient_input_bias");
   data->incandescenceGradient.ReadValues(node, "incandescence_gradient_values",
                                          AiNodeGetArray(node, "incandescence_gradient_positions"),
                                          AiNodeGetArray(node, "incandescence_gradient_interps"));   
   data->opacityGradient.type = AiNodeGetInt(node, "opacity_gradient_type");
   data->opacityGradient.inputBias = AiNodeGetFlt(node, "opacity_gradient_input_bias");
   data->opacityGradient.ReadValues(node, "opacity_gradient_values",
                                    AiNodeGetArray(node, "opacity_gradient_positions"),
                                    AiNodeGetArray(node, "opacity_gradient_interps"));

   data->dropoffShape = AiNodeGetInt(node, "dropoff_shape");
   data->filterType = AiNodeGetInt(node, "filter_type");
}

node_finish
{
   delete reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
}

shader_evaluate
{
   MayaFluidTexture2D* data = reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
   AtVector lPt(sg->u, sg->v, 0.0f);
   float dropoff = CalculateDropoff(data->fluidData, lPt, data->dropoffShape, AiClamp(AiShaderEvalParamFlt(p_edge_dropoff), 0.0f, 1.0f), data->filterType);
   const AtRGB opacity = AiMax(0.f, GetValue(sg, data->fluidData, lPt, data->opacityGradient, data->filterType, 1.0f, AI_V3_ONE)) * dropoff * AI_RGB_WHITE; // * data->transparency;
   AtRGB color = AI_RGB_BLACK;
   if (data->fluidData->colorGridEmpty())
      color = GetValue(sg, data->fluidData, lPt, data->colorGradient, data->filterType, 1.0f, AI_V3_ONE);
   else
      color = data->fluidData->readColors(lPt, FT_LINEAR);

   color.r = AiMax(0.f, color.r);
   color.g = AiMax(0.f, color.g);
   color.b = AiMax(0.f, color.b);
   AtRGB incandescence = GetValue(sg, data->fluidData, lPt, data->incandescenceGradient, data->filterType, 1.0f, AI_V3_ONE);
   incandescence.r = AiMax(0.f, incandescence.r);
   incandescence.g = AiMax(0.f, incandescence.g);
   incandescence.b = AiMax(0.f, incandescence.b); // do we need this?

   sg->out.RGB() = color + incandescence;
   // FIXME Arnold5 closure
   sg->out.RGBA().a = AiMax(opacity.r, opacity.g, opacity.b);
   //sg->out_opacity = opacity;
}
