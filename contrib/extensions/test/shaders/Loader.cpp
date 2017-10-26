#include <ai.h>

#include <cstdio>

extern const AtNodeMethods* MtoaTestShaderMtd;

node_loader
{
    switch (i)
    {
    case 0:
        node->methods      = MtoaTestShaderMtd;
        node->output_type  = AI_TYPE_RGBA;
        node->name         = "mtoa_test_shader";
        node->node_type    = AI_NODE_SHADER;
        break;

    default:
        return false;
    }

    sprintf(node->version, AI_VERSION);
    return true;
}
