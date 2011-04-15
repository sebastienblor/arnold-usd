#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage2DMtd);

namespace
{

enum MayaPlusMinusAverageParams
{
   p_operation,
   p_numInputs,
   p_value0,
   p_value1,
   p_value2,
   p_value3,
   p_value4,
   p_value5,
   p_value6,
   p_value7
};

enum MathOperation
{
   OP_NONE = 0,
   OP_PLUS,
   OP_MINUS,
   OP_AVERAGE
};

const char* MathOperationNames[] =
{
   "none",
   "sum",
   "subtract",
   "average",
   NULL
};

};

node_parameters
{
   AiParameterENUM("operation", OP_PLUS, MathOperationNames);
   AiParameterUINT("numInputs", 0);
   AiParameterPNT2("value0", 0.0f, 0.0f);
   AiParameterPNT2("value1", 0.0f, 0.0f);
   AiParameterPNT2("value2", 0.0f, 0.0f);
   AiParameterPNT2("value3", 0.0f, 0.0f);
   AiParameterPNT2("value4", 0.0f, 0.0f);
   AiParameterPNT2("value5", 0.0f, 0.0f);
   AiParameterPNT2("value6", 0.0f, 0.0f);
   AiParameterPNT2("value7", 0.0f, 0.0f);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtInt operation = AiShaderEvalParamEnum(p_operation);
   AtUInt numInputs = AiShaderEvalParamUInt(p_numInputs);
   
   AtPoint2 result = {0.0f, 0.0f};
   AtPoint2 value;

   if (numInputs > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (AtUInt32 i=0; i<numInputs; ++i)
            {
               value = AiShaderEvalParamPnt2(p_value0+i);
               AiV2Add(result, result, value);
            }
            break;
         case OP_MINUS:
            result = AiShaderEvalParamPnt2(p_value0);
            for (AtUInt32 i=1; i<numInputs; ++i)
            {
               value = AiShaderEvalParamPnt2(p_value0+i);
               AiV2Sub(result, result, value);
            }
            break;
         default:
            result = AiShaderEvalParamPnt2(p_value0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(numInputs);
         AiV2Scale(result, result, divider);
      }
   }

   sg->out.PNT2 = result;
}
