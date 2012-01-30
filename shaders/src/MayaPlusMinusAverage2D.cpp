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
   AiParameterENUM("operation", OP_PLUS, MathOperationNames);
   AiParameterARRAY("input2D", AiArray(0, 0, AI_TYPE_POINT2));

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
   
   AtPoint2 result = {0.0f, 0.0f};
   AtPoint2 value;

   if (inputs->nelements > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (AtUInt32 i=0; i<inputs->nelements; ++i)
            {
               value = AiArrayGetPnt2(inputs, i);
               AiV2Add(result, result, value);
            }
            break;
         case OP_MINUS:
            result = AiArrayGetPnt2(inputs, 0);
            for (AtUInt32 i=1; i<inputs->nelements; ++i)
            {
               value = AiArrayGetPnt2(inputs, i);
               AiV2Sub(result, result, value);
            }
            break;
         default:
            result = AiArrayGetPnt2(inputs, 0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(inputs->nelements);
         AiV2Scale(result, result, divider);
      }
   }

   sg->out.PNT2 = result;
}
