#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkColorMathMtd);

namespace
{

enum ldkColorMathParams
{
   p_colorA,
   p_alphaA,
   p_colorB,
   p_alphaB,
   p_operation
};

enum operation
{
   OP_ADD = 0,
   OP_SUBTRACT,
   OP_MULTIPLY,
   OP_DIVIDE,
   OP_MIN,
   OP_MAX
};

const char* enum_operation[] =
{
   "Add",
   "Subtract",
   "Multiply",
   "Divide",
   "Min",
   "Max",
   NULL
};

};

node_parameters
{
   AiParameterRGB("colorA", 1.0f, 0.0f, 0.0f);
   AiParameterFlt("alphaA", 1.0f);
   AiParameterRGB("colorB", 0.0f, 1.0f, 0.0f);
   AiParameterFlt("alphaB", 1.0f);
   AiParameterEnum("operation", OP_ADD, enum_operation);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorMath");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81642);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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
   const AtRGB colorA = AiShaderEvalParamRGB(p_colorA);
   const AtRGB colorB = AiShaderEvalParamRGB(p_colorB);
   const float alphaA = AiShaderEvalParamFlt(p_alphaA);
   const float alphaB = AiShaderEvalParamFlt(p_alphaB);
   const int op = AiShaderEvalParamEnum(p_operation);
   switch (op)
   {
   case OP_ADD:
      sg->out.RGB() = colorA + colorB;
      sg->out.RGBA().a = alphaA + alphaB;
      break;
   case OP_SUBTRACT:
      sg->out.RGB() = colorA - colorB;
      sg->out.RGBA().a = alphaA - alphaB;
      break;
   case OP_MULTIPLY:
      sg->out.RGB() = colorA * colorB;
      sg->out.RGBA().a = alphaA * alphaB;
      break;
   case OP_DIVIDE:
      sg->out.RGBA().r = colorB.r != 0.0f ? colorA.r / colorB.r : 0.0f;
      sg->out.RGBA().g = colorB.g != 0.0f ? colorA.g / colorB.g : 0.0f;
      sg->out.RGBA().b = colorB.b != 0.0f ? colorA.b / colorB.b : 0.0f;
      sg->out.RGBA().a = alphaB != 0.0f ? alphaA / alphaB : 0.0f;
      break;
   case OP_MIN:
      if ((colorA.r + colorA.g + colorA.b) < (colorB.r + colorB.g + colorB.b))
      {
         sg->out.RGB() = colorA;
         sg->out.RGBA().a = alphaA;
      }
      else
      {
         sg->out.RGB() = colorB;
         sg->out.RGBA().a = alphaB;
      }
      break;
   case OP_MAX:
      if ((colorA.r + colorA.g + colorA.b) > (colorB.r + colorB.g + colorB.b))
      {
         sg->out.RGB() = colorA;
         sg->out.RGBA().a = alphaA;
      }
      else
      {
         sg->out.RGB() = colorB;
         sg->out.RGBA().a = alphaB;
      }
      break;
   default:
   // FIXME Arnold5 we sed to call AiRGBAReset
      sg->out.RGBA() = AI_RGBA_ZERO;
      break;
   }
}
