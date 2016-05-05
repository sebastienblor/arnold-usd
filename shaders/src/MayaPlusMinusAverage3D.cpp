#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage3DMtd);

namespace
{

enum MayaPlusMinusAverage3DParams
{
   p_operation,
   p_input3D
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
   AiParameterArray("input3D", AiArray(0, 0, AI_TYPE_VECTOR));

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
   AtArray* inputs = AiShaderEvalParamArray(p_input3D);
   
   AtVector result(0.0f, 0.0f, 0.0f);

   if (AiArrayGetNumElements(inputs) > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (uint32_t i=0; i<AiArrayGetNumElements(inputs); ++i)
            {
               result += AiArrayGetVec(inputs, i);
            }
            break;
         case OP_MINUS:
            result = AiArrayGetVec(inputs, 0);
            for (uint32_t i=1; i<AiArrayGetNumElements(inputs); ++i)
            {
               result -= AiArrayGetVec(inputs, i);
            }
            break;
         default:
            result = AiArrayGetVec(inputs, 0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(AiArrayGetNumElements(inputs));
         result *= divider;
      }
   }

   sg->out.VEC() = result;
}
