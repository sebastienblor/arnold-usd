#include <ai.h>
#include <cstdio>

extern AtNodeMethods* BifrostVolumeMtds;
extern AtNodeMethods* BifrostPointsMtds;
extern AtNodeMethods* BifrostImplicitMtds;
extern AtNodeMethods* BifrostPolymeshMtds;
extern AtNodeMethods* BifrostBlocksMtds;

enum {
    BIFROST_POLYMESH = 0,
    BIFROST_POINTS,
    BIFROST_VOLUME,
    BIFROST_IMPLICIT,
    BIFROST_BLOCKS
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
    case BIFROST_VOLUME:
        node->methods = BifrostVolumeMtds;
        node->name = "bifrost_volume";
        node->node_type = AI_NODE_SHAPE_VOLUME;
        break;
    case BIFROST_IMPLICIT:
        node->methods = BifrostImplicitMtds;
        node->name = "bifrost_implicit";
        node->node_type = AI_NODE_SHAPE_IMPLICIT;
        break;
    case BIFROST_BLOCKS:
        node->methods = BifrostBlocksMtds;
        node->name = "bifrost_blocks";
        node->node_type = AI_NODE_SHAPE_PROCEDURAL;
        break;
    default:
        return false;
    }
    strcpy(node->version, AI_VERSION);
    return true;
}
