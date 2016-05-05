#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage2DMtd);

namespace
{

enum MayaPlusMinusAverage2DParams
{
   p_operation,
   p_input2D
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
   AiParameterArray("input2D", AiArray(0, 0, AI_TYPE_VECTOR2));

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
   AtArray* inputs = AiShaderEvalParamArray(p_input2D);
   
   AtVector2 result(0.0f, 0.0f);
   AtVector2 value;

   if (AiArrayGetNumElements(inputs) > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (uint32_t i=0; i<AiArrayGetNumElements(inputs); ++i)
            {
               value = AiArrayGetVec2(inputs, i);
               result += value;
            }
            break;
         case OP_MINUS:
            result = AiArrayGetVec2(inputs, 0);
            for (uint32_t i=1; i<AiArrayGetNumElements(inputs); ++i)
            {
               value = AiArrayGetVec2(inputs, i);
               result -= value;
            }
            break;
         default:
            result = AiArrayGetVec2(inputs, 0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         result /= float(AiArrayGetNumElements(inputs));
      }
   }

   sg->out.VEC2() = result;
}
