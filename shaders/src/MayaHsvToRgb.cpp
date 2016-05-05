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
   AiParameterVec("inHsv", 0.0f, 0.0f, 0.0f);

   AiMetaDataSetStr(mds, NULL, "maya.name", "hsvToRgb");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52483252);
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
   sg->out.RGB() = HSVtoRGB(AiShaderEvalParamVec(p_inHsv));
}

