#include <ai.h>

#include <cstdio>

extern const AtNodeMethods* MtoaTestShaderMtd;
extern const AtNodeMethods* MtoaTestMultipleOutputShaderMtd;

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

    case 1:
        node->methods      = MtoaTestMultipleOutputShaderMtd;
        node->output_type  = AI_TYPE_BOOLEAN;
        node->name         = "mtoa_test_multi_out_shader";
        node->node_type    = AI_NODE_SHADER;
        return true;

    default:
        return false;
    }

    sprintf(node->version, AI_VERSION);
    return true;
}
