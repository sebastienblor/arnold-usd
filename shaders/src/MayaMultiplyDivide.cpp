#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaMultiplyDivideMtd);

namespace
{

enum MayaMultiplyDivideParams
{
   p_input1,
   p_input2,
   p_operation
};

enum MathOperation
{
   OP_NOP = 0,
   OP_MULTIPLY,
   OP_DIVIDE,
   OP_POWER
};

const char* MathOperationNames[] =
{
   "nop",
   "multiply",
   "divide",
   "power",
   NULL
};

};

node_parameters
{
   AiParameterPNT("input1", 0.0f, 0.0f, 0.0f);
   AiParameterPNT("input2", 0.0f, 0.0f, 0.0f);
   AiParameterENUM("operation", 0, MathOperationNames);

   AiMetaDataSetStr(mds, NULL, "maya.name", "multiplyDivide");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x524d4449);
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
   AtPoint input1 = AiShaderEvalParamPnt(p_input1);
   AtPoint input2 = AiShaderEvalParamPnt(p_input2);

   switch(AiShaderEvalParamInt(p_operation))
   {
   case OP_NOP:      // No operation (output = input1)
      sg->out.PNT = input1;
      break;
   case OP_MULTIPLY: // Multiply (output = input1 * input2)
      sg->out.PNT = input1 * input2;
      break;
   case OP_DIVIDE:   // Divide (output = input1 / input2)
      sg->out.PNT.x = (input2.x == 0) ? input1.x : (input1.x / input2.x);
      sg->out.PNT.y = (input2.y == 0) ? input1.y : (input1.y / input2.y);
      sg->out.PNT.z = (input2.z == 0) ? input1.z : (input1.z / input2.z);
      break;
   case OP_POWER:    // Power (output = input1 ^ input2)
      sg->out.PNT.x = powf(input1.x, input2.x);
      sg->out.PNT.y = powf(input1.y, input2.y);
      sg->out.PNT.z = powf(input1.z, input2.z);
      break;
   default:
      sg->out.PNT = AI_P3_ZERO;
   }
}
