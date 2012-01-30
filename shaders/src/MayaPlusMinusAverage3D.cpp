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
   AiParameterENUM("operation", OP_PLUS, MathOperationNames);
   AiParameterARRAY("input3D", AiArray(0, 0, AI_TYPE_POINT));

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
   
   AtPoint result = {0.0f, 0.0f, 0.0f};

   if (inputs->nelements > 0)
   {
      switch (operation)
      {
         case OP_PLUS:
         case OP_AVERAGE:
            for (AtUInt32 i=0; i<inputs->nelements; ++i)
            {
               result += AiArrayGetPnt(inputs, i);
            }
            break;
         case OP_MINUS:
            result = AiArrayGetPnt(inputs, 0);
            for (AtUInt32 i=1; i<inputs->nelements; ++i)
            {
               result -= AiArrayGetPnt(inputs, i);
            }
            break;
         default:
            result = AiArrayGetPnt(inputs, 0);
            break;
      }

      if (operation == OP_AVERAGE)
      {
         float divider = 1.0f / float(inputs->nelements);
         result *= divider;
      }
   }

   sg->out.PNT = result;
}
