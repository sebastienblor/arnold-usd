#include <ai.h>
#include <cstdio>

extern const AtNodeMethods* ldkFloatConstantMtd;
extern const AtNodeMethods* ldkFloatCompositeMtd;
extern const AtNodeMethods* ldkFloatConditionMtd;
extern const AtNodeMethods* ldkFloatCorrectMtd;
extern const AtNodeMethods* ldkFloatLogicMtd;
extern const AtNodeMethods* ldkFloatMathMtd;
extern const AtNodeMethods* ldkFloatMaskMtd;
extern const AtNodeMethods* ldkColorCompositeMtd;
extern const AtNodeMethods* ldkColorConditionMtd;
extern const AtNodeMethods* ldkColorConstantMtd;
extern const AtNodeMethods* ldkColorCorrectMtd;
extern const AtNodeMethods* ldkColorLogicMtd;
extern const AtNodeMethods* ldkColorMaskMtd;
extern const AtNodeMethods* ldkColorMathMtd;
extern const AtNodeMethods* ldkChannelsMtd;
extern const AtNodeMethods* ldkPremultiplyMtd;
extern const AtNodeMethods* ldkSimplexNoiseMtd;

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
