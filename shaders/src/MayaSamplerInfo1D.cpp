#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaSamplerInfo1DMtd);

namespace
{

enum MaySamplerInfo1DParams
{
   p_mode
};

enum Mode
{
   FACING_RATIO = 0,
   FLIPPED_NORMAL
};

const char* mode_enum[] =
{
   "facingRatio",
   "flippedNormal",
   NULL
};

}


node_parameters
{
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   AiParameterEnum("mode", 0, mode_enum);
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
   switch (AiShaderEvalParamEnum(p_mode))
   {
   case FACING_RATIO:
      sg->out.FLT() = AiV3Dot(sg->Nf, - sg->Rd);
      break;
   case FLIPPED_NORMAL:
      sg->out.FLT() = (AiV3Dot(sg->Nf, sg->N) < 0.0f ? 1.0f : 0.0f);
      break;
   }
}
