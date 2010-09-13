
#include <ai.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlusMinusAverage2DMtd);

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
   AtPoint2 def = {0.0f, 0.0f};
   AtArray *vdef = AiArrayAllocate(1, 1, AI_TYPE_POINT2);
   AiArraySetPnt2(vdef, 0, def);
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
   
   AtPoint2 result = {0.0f, 0.0f};
   AtPoint2 item;

   if (values->nelements > 0)
   {
      switch (op)
      {
      case OP_PLUS:
      case OP_AVERAGE:
         for (AtUInt32 i = 0; (i < values->nelements); ++i)
         {
            item = AiArrayGetPnt2(values, i);
            result.x += item.x;
            result.y += item.y;
         }
         break;
      case OP_MINUS:
         result = AiArrayGetPnt2(values, 0);
         for (AtUInt32 i = 1; (i < values->nelements); ++i)
         {
            item = AiArrayGetPnt2(values, i);
            result.x -= item.x;
            result.y -= item.y;
         }
         break;
      default:
         result = AiArrayGetPnt2(values, 0);
         break;
      }

      if (op == OP_AVERAGE)
      {
         float divider = 1.0f / float(values->nelements);
         result.x *= divider;
         result.y *= divider;
      }
   }

   sg->out.PNT2 = result;
}
