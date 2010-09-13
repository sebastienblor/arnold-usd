
#include <ai.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage3DMtd);

namespace
{

enum PlusMinusAverageParams
{
   p_op,
   p_values
};

enum Operations
{
   OP_NONE = 0,
   OP_PLUS,
   OP_MINUS,
   OP_AVERAGE
};

const char* enum_operation[] =
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
   AiParameterENUM("operation", OP_PLUS, enum_operation);
   AtArray *vdef = AiArrayAllocate(1, 1, AI_TYPE_RGB);
   AiArraySetRGB(vdef, 0, AI_RGB_BLACK);
   AiParameterARRAY("values", vdef);
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
   AtInt op = AiShaderEvalParamEnum(p_op);
   AtArray *values = AiShaderEvalParamArray(p_values);
   
   AtRGB result = {0.0f, 0.0f, 0.0f};

   if (values->nelements > 0)
   {
      switch (op)
      {
      case OP_PLUS:
      case OP_AVERAGE:
         for (AtUInt32 i=0; i<values->nelements; ++i)
         {
            result += AiArrayGetRGB(values, i);
         }
         break;
      case OP_MINUS:
         result = AiArrayGetRGB(values, 0);
         for (AtUInt32 i=1; i<values->nelements; ++i)
         {
            result -= AiArrayGetRGB(values, i);
         }
         break;
      default:
         result = AiArrayGetRGB(values, 0);
         break;
      }

      if (op == OP_AVERAGE)
      {
         float divider = 1.0f / float(values->nelements);
         result *= divider;
      }
   }

   sg->out.RGB = result;
}
