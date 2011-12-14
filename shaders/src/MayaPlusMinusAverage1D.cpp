#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage1DMtd);

namespace
{

enum MayaPlusMinusAverage1DParams
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
   AiParameterFLT("value0", 0.0f);
   AiParameterFLT("value1", 0.0f);
   AiParameterFLT("value2", 0.0f);
   AiParameterFLT("value3", 0.0f);
   AiParameterFLT("value4", 0.0f);
   AiParameterFLT("value5", 0.0f);
   AiParameterFLT("value6", 0.0f);
   AiParameterFLT("value7", 0.0f);

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
   int operation = AiShaderEvalParamEnum(p_operation);
   AtUInt numInputs = AiShaderEvalParamUInt(p_numInputs);
   
   float result = 0.0f;

   if (numInputs > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (AtUInt32 i=0; i<numInputs; ++i)
            {
               result += AiShaderEvalParamFlt(p_value0+i);
            }
            break;
         case OP_MINUS:
            result = AiShaderEvalParamFlt(p_value0);
            for (AtUInt32 i=1; i<numInputs; ++i)
            {
               result -= AiShaderEvalParamFlt(p_value0+i);
            }
            break;
         default:
            result = AiShaderEvalParamFlt(p_value0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(numInputs);
         result *= divider;
      }
   }

   sg->out.FLT = result;
}
