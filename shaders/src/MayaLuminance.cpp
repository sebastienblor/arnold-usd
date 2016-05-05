#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLuminanceMtd);

namespace
{

enum MayaLuminanceParams
{
   p_value
};

}

node_parameters
{
   AiParameterRGB("value", 0, 0, 0);

   AiMetaDataSetStr(mds, NULL, "maya.name", "luminance");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x524c554d);
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
   sg->out.FLT() = Luminance(AiShaderEvalParamRGB(p_value));
}
