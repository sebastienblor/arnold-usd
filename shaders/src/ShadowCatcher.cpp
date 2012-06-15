#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ShadowCatcherMtd);

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiShadowCatcher");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D19);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);  
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
   sg->out.RGB = AiLightsGetShadowMatte(sg);
}
