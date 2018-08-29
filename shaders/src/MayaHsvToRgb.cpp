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

   AiMetaDataSetStr(nentry, NULL, "maya.name", "hsvToRgb");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52483252);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
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

