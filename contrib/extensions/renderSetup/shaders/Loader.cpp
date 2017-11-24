#include <ai.h>
#include <cstdio>

extern const AtNodeMethods* applyAbsFloatOverrideMtd;
extern const AtNodeMethods* applyAbs2FloatsOverrideMtd;
extern const AtNodeMethods* applyAbs3FloatsOverrideMtd;
extern const AtNodeMethods* applyAbsIntOverrideMtd;
extern const AtNodeMethods* applyRelFloatOverrideMtd;
extern const AtNodeMethods* applyRel2FloatsOverrideMtd;
extern const AtNodeMethods* applyRel3FloatsOverrideMtd;
extern const AtNodeMethods* applyRelIntOverrideMtd;

enum{
    RENDER_SETUP_APPLY_ABS_FLOAT_OVERRIDE = 0,
    RENDER_SETUP_APPLY_ABS_2FLOATS_OVERRIDE,
    RENDER_SETUP_APPLY_ABS_3FLOATS_OVERRIDE,
    RENDER_SETUP_APPLY_ABS_INT_OVERRIDE,
    RENDER_SETUP_APPLY_REL_FLOAT_OVERRIDE,
    RENDER_SETUP_APPLY_REL_2FLOATS_OVERRIDE,
    RENDER_SETUP_APPLY_REL_3FLOATS_OVERRIDE,
    RENDER_SETUP_APPLY_REL_INT_OVERRIDE
};

node_loader
{
    switch (i)
    {
    case RENDER_SETUP_APPLY_ABS_FLOAT_OVERRIDE:
        node->methods     = applyAbsFloatOverrideMtd;
        node->output_type = AI_TYPE_FLOAT;
        node->name        = "applyAbsFloatOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_ABS_2FLOATS_OVERRIDE:
        node->methods     = applyAbs2FloatsOverrideMtd;
        node->output_type = AI_TYPE_VECTOR2;
        node->name        = "applyAbs2FloatsOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_ABS_3FLOATS_OVERRIDE:
        node->methods     = applyAbs3FloatsOverrideMtd;
        node->output_type = AI_TYPE_RGB;
        node->name        = "applyAbs3FloatsOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_ABS_INT_OVERRIDE:
        node->methods     = applyAbsIntOverrideMtd;
        node->output_type = AI_TYPE_INT;
        node->name        = "applyAbsIntOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_REL_FLOAT_OVERRIDE:
        node->methods     = applyRelFloatOverrideMtd;
        node->output_type = AI_TYPE_FLOAT;
        node->name        = "applyRelFloatOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_REL_2FLOATS_OVERRIDE:
        node->methods     = applyRel2FloatsOverrideMtd;
        node->output_type = AI_TYPE_VECTOR2;
        node->name        = "applyRel2FloatsOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_REL_3FLOATS_OVERRIDE:
        node->methods     = applyRel3FloatsOverrideMtd;
        node->output_type = AI_TYPE_RGB;
        node->name        = "applyRel3FloatsOverride";
        node->node_type   = AI_NODE_SHADER;
        break;
    case RENDER_SETUP_APPLY_REL_INT_OVERRIDE:
        node->methods     = applyRelIntOverrideMtd;
        node->output_type = AI_TYPE_INT;
        node->name        = "applyRelIntOverride";
        node->node_type   = AI_NODE_SHADER;
        break;

    default:
        return false;
    }

    sprintf(node->version, AI_VERSION);

    return true;
}
