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
   AiParameterVec("input1", 0.0f, 0.0f, 0.0f);
   AiParameterVec("input2", 0.0f, 0.0f, 0.0f);
   AiParameterEnum("operation", 0, MathOperationNames);

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
   AtVector input1 = AiShaderEvalParamVec(p_input1);
   AtVector input2 = AiShaderEvalParamVec(p_input2);

   switch(AiShaderEvalParamInt(p_operation))
   {
   case OP_NOP:      // No operation (output = input1)
      sg->out.VEC() = input1;
      break;
   case OP_MULTIPLY: // Multiply (output = input1 * input2)
      sg->out.VEC() = input1 * input2;
      break;
   case OP_DIVIDE:   // Divide (output = input1 / input2)
      sg->out.VEC().x = (input2.x == 0) ? input1.x : (input1.x / input2.x);
      sg->out.VEC().y = (input2.y == 0) ? input1.y : (input1.y / input2.y);
      sg->out.VEC().z = (input2.z == 0) ? input1.z : (input1.z / input2.z);
      break;
   case OP_POWER:    // Power (output = input1 ^ input2)
      sg->out.VEC().x = powf(input1.x, input2.x);
      sg->out.VEC().y = powf(input1.y, input2.y);
      sg->out.VEC().z = powf(input1.z, input2.z);
      break;
   default:
      sg->out.VEC() = AI_P3_ZERO;
   }
}
