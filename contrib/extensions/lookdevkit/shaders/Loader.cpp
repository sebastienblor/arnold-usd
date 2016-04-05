#include <ai.h>
#include <cstdio>

extern AtNodeMethods* ldkFloatConstantMtd;
extern AtNodeMethods* ldkFloatCompositeMtd;
extern AtNodeMethods* ldkFloatConditionMtd;
extern AtNodeMethods* ldkFloatCorrectMtd;
extern AtNodeMethods* ldkFloatLogicMtd;
extern AtNodeMethods* ldkFloatMathMtd;
extern AtNodeMethods* ldkFloatMaskMtd;
extern AtNodeMethods* ldkColorCompositeMtd;
extern AtNodeMethods* ldkColorConditionMtd;
extern AtNodeMethods* ldkColorConstantMtd;
extern AtNodeMethods* ldkColorCorrectMtd;
extern AtNodeMethods* ldkColorLogicMtd;
extern AtNodeMethods* ldkColorMaskMtd;
extern AtNodeMethods* ldkColorMathMtd;
extern AtNodeMethods* ldkChannelsMtd;
extern AtNodeMethods* ldkPremultiplyMtd;
extern AtNodeMethods* ldkSimplexNoiseMtd;

enum{
   LDK_FLOAT_CONSTANT = 0,
   LDK_FLOAT_COMPOSITE,
   LDK_FLOAT_CONDITION,
   LDK_FLOAT_CORRECT,
   LDK_FLOAT_LOGIC,
   LDK_FLOAT_MASK,
   LDK_FLOAT_MATH,
   LDK_COLOR_COMPOSITE,
   LDK_COLOR_CONDITION,
   LDK_COLOR_CONSTANT,
   LDK_COLOR_CORRECT,
   LDK_COLOR_LOGIC,
   LDK_COLOR_MASK,
   LDK_COLOR_MATH,
   LDK_CHANNELS,
   LDK_PREMULTIPLY,
   LDK_SIMPLEX_NOISE
};

node_loader
{
   switch (i)
   {
   case LDK_FLOAT_CONSTANT:
      node->methods     = ldkFloatConstantMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatConstant";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_COMPOSITE:
      node->methods     = ldkFloatCompositeMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatComposite";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_CONDITION:
      node->methods     = ldkFloatConditionMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatCondition";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_CORRECT:
      node->methods     = ldkFloatCorrectMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatCorrect";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_LOGIC:
      node->methods     = ldkFloatLogicMtd;
      node->output_type = AI_TYPE_BOOLEAN;
      node->name        = "ldkFloatLogic";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_MASK:
      node->methods     = ldkFloatMaskMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatMask";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_FLOAT_MATH:
      node->methods     = ldkFloatMathMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "ldkFloatMath";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_COMPOSITE:
      node->methods     = ldkColorCompositeMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorComposite";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_CONDITION:
      node->methods     = ldkColorConditionMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorCondition";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_CONSTANT:
      node->methods     = ldkColorConstantMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorConstant";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_CORRECT:
      node->methods     = ldkColorCorrectMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorCorrect";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_LOGIC:
      node->methods     = ldkColorLogicMtd;
      node->output_type = AI_TYPE_BOOLEAN;
      node->name        = "ldkColorLogic";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_MASK:
      node->methods     = ldkColorMaskMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorMask";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_COLOR_MATH:
      node->methods     = ldkColorMathMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkColorMath";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_CHANNELS:
      node->methods     = ldkChannelsMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkChannels";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_PREMULTIPLY:
      node->methods     = ldkPremultiplyMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkPremultiply";
      node->node_type   = AI_NODE_SHADER;
      break;

   case LDK_SIMPLEX_NOISE:
      node->methods     = ldkSimplexNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "ldkSimplexNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
