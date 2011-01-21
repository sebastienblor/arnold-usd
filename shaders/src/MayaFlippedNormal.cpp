#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaFlippedNormalMtd);

node_parameters
{
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
   sg->out.FLT = (AiV3Dot(sg->Nf, sg->N) < 0.0f ? 1.0f : 0.0f);
}
