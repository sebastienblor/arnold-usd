#include <ai.h>
#include <cstdio>

extern const AtNodeMethods* BifrostPolymeshMtd;
extern const AtNodeMethods* BifrostPrimitivesMtd;
extern const AtNodeMethods* BifrostVolumeMtd;
extern const AtNodeMethods* BifrostImplicitsMtd;

enum{
    BIFROST_POLYMESH = 0,
    BIFROST_PRIMITIVES,
    BIFROST_VOLUME,
    BIFROST_IMPLICITS
};

node_loader
{
    node->output_type  = AI_TYPE_NONE;
    switch (i)
    {

    case BIFROST_POLYMESH:
        node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
        node->methods      = BifrostPolymeshMtd;
        node->name         = "bifrostPolymesh";
        break;
    case BIFROST_PRIMITIVES:
        node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
        node->methods      = BifrostPrimitivesMtd;
        node->name         = "bifrostPrimitives";
        break;
    case BIFROST_VOLUME:
        node->node_type    = AI_NODE_SHAPE_VOLUME;
        node->methods      = BifrostVolumeMtd;
        node->name         = "bifrostVolume";
        break;
    case BIFROST_IMPLICITS:
        node->node_type    = AI_NODE_SHAPE_IMPLICIT;
        node->methods      = BifrostImplicitsMtd;
        node->name         = "bifrostImplicits";
        break;
    default:
        return false;
    }

    sprintf(node->version, AI_VERSION);

    return true;
}
