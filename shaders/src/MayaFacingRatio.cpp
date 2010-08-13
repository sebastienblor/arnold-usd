#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaFacingRatioMtd);

node_parameters
{
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
   sg->out.FLT = AiV3Dot(sg->Nf, - sg->Rd);
}
