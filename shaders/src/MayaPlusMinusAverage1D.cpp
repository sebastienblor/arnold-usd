#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage1DMtd);

namespace
{

enum MayaPlusMinusAverage1DParams
{
   p_operation,
   p_input1D
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
   AiParameterEnum("operation", OP_PLUS, MathOperationNames);
   AiParameterArray("input1D", AiArray(0, 0, AI_TYPE_FLOAT));

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
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
   AtArray* inputs = AiShaderEvalParamArray(p_input1D);
   
   float result = 0.0f;

   if (AiArrayGetNumElements(inputs) > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (uint32_t i=0; i<AiArrayGetNumElements(inputs); ++i)
            {
               result += AiArrayGetFlt(inputs, i);
            }
            break;
         case OP_MINUS:
            result = AiArrayGetFlt(inputs, 0);
            for (uint32_t i=1; i<AiArrayGetNumElements(inputs); ++i)
            {
               result -= AiArrayGetFlt(inputs, i);
            }
            break;
         default:
            result = AiArrayGetFlt(inputs, 0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(AiArrayGetNumElements(inputs));
         result *= divider;
      }
   }

   sg->out.FLT() = result;
}
