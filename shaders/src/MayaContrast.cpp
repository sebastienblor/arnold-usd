#include <ai.h>

#include <cstdio>
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

AI_SHADER_NODE_EXPORT_METHODS(MayaContrastMtd);

namespace
{

enum MayaContrastParams
{
   p_value,
   p_contrast,
   p_bias
};

}

node_parameters
{
   AiParameterVec("value", 0.0f, 0.0f, 0.0f);
   AiParameterVec("contrast", 2.0f, 2.0f, 2.0f);
   AiParameterVec("bias", 0.5f, 0.5f, 0.5f);

   //AiMetaDataSetStr(nentry, NULL, "maya.name", "contrast");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52434f4e);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   AiMetaDataSetBool(nentry, NULL, "deprecated", false);
   
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   AtVector inputValue = AiShaderEvalParamVec(p_value);
   AtVector contrast = AiShaderEvalParamVec(p_contrast);
   AtVector bias = AiShaderEvalParamVec(p_bias);

   AtVector outValue;

   // To shift inflection point of gain function,
   // modify input value to return 0.5 if input value equal to bias value.
   //   f(0) = 0
   //   f(bias) = 0.5
   //   f(1) = 1
   // To achieve this function, use combination of exponential function and logarithmic function
   //   f(x) = x^(log(0.5)/log(bias))
   float log05 = logf(0.5f);

   AtVector modifiedValue;
   modifiedValue.x = powf(inputValue.x, log05 / logf(bias.x));
   modifiedValue.y = powf(inputValue.y, log05 / logf(bias.y));
   modifiedValue.z = powf(inputValue.z, log05 / logf(bias.z));

   // Contrast convert calculation based on Perlin's gain function
   outValue.x = modifiedValue.x < 0.5f ? 0.5f * powf(2.0f * modifiedValue.x, contrast.x) : 1.0f - 0.5f * powf(2.0f * (1.0f - modifiedValue.x), contrast.x);
   outValue.y = modifiedValue.y < 0.5f ? 0.5f * powf(2.0f * modifiedValue.y, contrast.y) : 1.0f - 0.5f * powf(2.0f * (1.0f - modifiedValue.y), contrast.y);
   outValue.z = modifiedValue.z < 0.5f ? 0.5f * powf(2.0f * modifiedValue.z, contrast.z) : 1.0f - 0.5f * powf(2.0f * (1.0f - modifiedValue.z), contrast.z);

   // Clamp 0 to 1 to avoid INF00 or -INF00
   outValue.x = AiClamp(outValue.x, 0.0f, 1.0f);
   outValue.y = AiClamp(outValue.y, 0.0f, 1.0f);
   outValue.z = AiClamp(outValue.z, 0.0f, 1.0f);

   sg->out.RGBA().r = outValue.x;
   sg->out.RGBA().g = outValue.y;
   sg->out.RGBA().b = outValue.z;

}

