#include <ai.h>
#include "../utils.h"

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatCompositeMtd);

namespace
{

enum ldkFloatCompositeParams
{
   p_floatA,
   p_floatB,
   p_operation,
   p_factor
};

};

node_parameters
{
   AiParameterFlt("floatA", 1.0f);
   AiParameterFlt("floatB", 1.0f);
   AiParameterEnum("operation", COP_ADD, CompositeOperationNames);
   AiParameterFlt("factor", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "floatComposite");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81628);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
   const float floatA = AiShaderEvalParamFlt(p_floatA);
   const float factor = AiShaderEvalParamFlt(p_factor);

   if (factor <= 0.0f)
   {
      sg->out.FLT() = floatA;
   }
   else
   {
      const float floatB = AiShaderEvalParamFlt(p_floatB);
      const int op = AiShaderEvalParamEnum(p_operation);
      switch (op)
      {
      case COP_ADD:
         sg->out.FLT() = CompositeAdd(floatA, floatB, factor);
         break;
      case COP_SUBTRACT:
         sg->out.FLT() = CompositeSubtract(floatA, floatB, factor);
         break;
      case COP_MIX:
         sg->out.FLT() = CompositeMix(floatA, floatB, factor);
         break;
      case COP_MULTIPLY:
         sg->out.FLT() = CompositeMultiply(floatA, floatB, factor);
         break;
      case COP_SCREEN:
         sg->out.FLT() = CompositeScreen(floatA, floatB, factor);
         break;
      case COP_OVERLAY:
         sg->out.FLT() = CompositeOverlay(floatA, floatB, factor);
         break;
      case COP_DIFFERENCE:
         sg->out.FLT() = CompositeDifference(floatA, floatB, factor);
         break;
      case COP_DODGE:
         sg->out.FLT() = CompositeDodge(floatA, floatB, factor);
         break;
      case COP_BURN:
         sg->out.FLT() = CompositeBurn(floatA, floatB, factor);
         break;
      default:
         sg->out.FLT() = 0.0f;
         break;
      }
   }
}
