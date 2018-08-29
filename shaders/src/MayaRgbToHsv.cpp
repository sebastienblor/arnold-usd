#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRgbToHsvMtd);

namespace
{

enum MayaRgbToHsvParams
{
   p_inRgb
};

}

node_parameters
{
   AiParameterRGB("inRgb", 0.0f, 0.0f, 0.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "rgbToHsv");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52523248);
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
   sg->out.VEC() = RGBtoHSV(AiShaderEvalParamRGB(p_inRgb));
}
