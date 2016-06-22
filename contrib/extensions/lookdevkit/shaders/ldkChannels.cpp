#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkChannelsMtd);

namespace
{

enum ldkChannelsParams
{
   p_inColor,
   p_inAlpha,
   p_channelR,
   p_channelG,
   p_channelB,
   p_channelA
};

enum Operation
{
   CHANNEL_R = 0,
   CHANNEL_G,
   CHANNEL_B,
   CHANNEL_A,
};

const char* OperationNames[] =
{
   "R",
   "G",
   "B",
   "A",
   NULL
};

};

node_parameters
{
   AiParameterRGB("inColor", 0.3f, 0.3f, 0.3f);
   AiParameterFlt("inAlpha", 1.0f);
   AiParameterEnum("channelR", CHANNEL_R, OperationNames);
   AiParameterEnum("channelG", CHANNEL_G, OperationNames);
   AiParameterEnum("channelB", CHANNEL_B, OperationNames);
   AiParameterEnum("channelA", CHANNEL_A, OperationNames);

   AiMetaDataSetStr(mds, NULL, "maya.name", "channels");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x81648);
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
   const int channelR = AiShaderEvalParamEnum(p_channelR);
   const int channelG = AiShaderEvalParamEnum(p_channelG);
   const int channelB = AiShaderEvalParamEnum(p_channelB);
   const int channelA = AiShaderEvalParamEnum(p_channelA);

   AtRGBA input = AiRGBACreate(0.0f, 0.0f, 0.0f, 0.0f);

   // Evaluate color input if needed
   if (channelR < CHANNEL_A || channelG < CHANNEL_A || channelB < CHANNEL_A || channelA < CHANNEL_A) 
   {
      input.rgb() = AiShaderEvalParamRGB(p_inColor);
   }
   // Evaluate alpha input if needed
   if (channelR == CHANNEL_A || channelG == CHANNEL_A || channelB == CHANNEL_A || channelA == CHANNEL_A) 
   {
      input.a = AiShaderEvalParamFlt(p_inAlpha);
   }

   // Pick the right channel from the input
   sg->out.RGBA.r = input[channelR];
   sg->out.RGBA.g = input[channelG];
   sg->out.RGBA.b = input[channelB];
   sg->out.RGBA.a = input[channelA];
}
