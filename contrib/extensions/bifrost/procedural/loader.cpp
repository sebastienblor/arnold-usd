#include <ai.h>
#include <cstdio>

extern AtNodeMethods* BifrostVolumeMtds;
extern AtNodeMethods* BifrostPointsMtds;
extern AtNodeMethods* BifrostImplicitMtds;
extern AtNodeMethods* BifrostPolymeshMtds;

node_loader
{
    if (i>0) return false;
    node->methods = BifrostPolymeshMtds;
    node->name = "bifrost_polymesh";
    node->node_type = AI_NODE_SHAPE_PROCEDURAL;
    strcpy(node->version, AI_VERSION);
    return true;
}
