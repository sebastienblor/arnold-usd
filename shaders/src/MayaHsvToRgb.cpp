#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaHsvToRgbMtd);

namespace
{

enum HsvToRgbParams
{
   p_inHsv
};

}

node_parameters
{
   AiParameterVEC("inHsv", 0.0f, 0.0f, 0.0f);

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "hsvToRgb");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52483252);
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
   sg->out.RGB = HSVtoRGB(AiShaderEvalParamVec(p_inHsv));
}

