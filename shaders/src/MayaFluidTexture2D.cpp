#include <ai.h>

#include "MayaFluidData.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidTexture2DMtd);

node_parameters
{
   CMayaFluidData::InitializeShaderParameters(params, false);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   sg->out.RGB = AI_RGB_RED;
}
