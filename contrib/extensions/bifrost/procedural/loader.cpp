#include <ai.h>
#include <cstdio>

extern AtNodeMethods* BifrostVolumeMtds;
extern AtNodeMethods* BifrostPointsMtds;
extern AtNodeMethods* BifrostImplicitMtds;
extern AtNodeMethods* BifrostPolymeshMtds;

enum {
    BIFROST_POLYMESH = 0,
    BIFROST_POINTS,
    BIFROST_VOLUME
};

node_loader
{
    switch(i){
    case BIFROST_POLYMESH:
        node->methods = BifrostPolymeshMtds;
        node->name = "bifrost_polymesh";
        node->node_type = AI_NODE_SHAPE_PROCEDURAL;
        break;
    case BIFROST_POINTS:
        node->methods = BifrostPointsMtds;
        node->name = "bifrost_points";
        node->node_type = AI_NODE_SHAPE_PROCEDURAL;
        break;
    default:
        return false;
    }
    strcpy(node->version, AI_VERSION);
    return true;
}
