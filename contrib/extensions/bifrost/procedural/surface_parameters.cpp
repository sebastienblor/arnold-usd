#include "surface_parameters.h"

#include <ai_params.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <ai_msg.h>
#include <aminomath/bbox.h>
#include <aminomath/vec.h>
#include <vector>
#include "utils.h"

SurfaceParams::SurfaceParams(const AtNode* node) : BifrostParams(node) {
    GET_ENUM(render_component, RenderComponent);

    GET_STR(distance_channel);

    GET_FLT(levelset_resolution_factor);
    GET_FLT(levelset_droplet_reveal_factor);
    GET_FLT(levelset_surface_radius);
    GET_FLT(levelset_droplet_radius);
    GET_FLT(levelset_max_volume_of_holes_to_close);

    GET_FLT(dilate);
    GET_FLT(erode);
    GET_FLT(smooth);
    GET_ENUM(smooth_mode, SmoothMode);
    GET_INT(smooth_iterations);

    GET_BOOL(enable_infinite_blending);
    GET_FLT(infinite_blending_height);
    GET_VEC2(infinite_blending_center);
    GET_VEC2(infinite_blending_dimension);
    GET_FLT(infinite_blending_radius);
    GET_ENUM(infinite_blending_blend, BlendType);
    GET_BOOL(enable_infinite_blending_uvs);
}

std::string SurfaceParams::str() const{
    std::stringstream ss;
    ss << BifrostParams::str();
    DUMP_PARAM(render_component);

    DUMP_PARAM(distance_channel);

    DUMP_PARAM(levelset_droplet_reveal_factor);
    DUMP_PARAM(levelset_surface_radius);
    DUMP_PARAM(levelset_droplet_radius);
    DUMP_PARAM(levelset_resolution_factor);
    DUMP_PARAM(levelset_max_volume_of_holes_to_close);

    DUMP_PARAM(dilate);
    DUMP_PARAM(erode);
    DUMP_PARAM(smooth);
    DUMP_PARAM(smooth_mode);
    DUMP_PARAM(smooth_iterations);

    DUMP_PARAM(enable_infinite_blending);
    DUMP_PARAM(infinite_blending_height);
    DUMP_PARAM(infinite_blending_center);
    DUMP_PARAM(infinite_blending_dimension);
    DUMP_PARAM(infinite_blending_radius);
    DUMP_PARAM(infinite_blending_blend);
    DUMP_PARAM(enable_infinite_blending_uvs);

    return ss.str();
}

void SurfaceParams::declare(AtList* params, AtNodeEntry* nentry)
{
    BifrostParams::declare(params, nentry);
    static const char* rc_enums[] = RENDER_COMPONENT_STRINGS;
    PARAM_ENUM(render_component, VOXELS, rc_enums);
    // VOXELS
    PARAM_STR(distance_channel, "");

    // PARTICLES
    PARAM_FLT(levelset_droplet_reveal_factor, 3);
    PARAM_FLT(levelset_surface_radius, 1.4);
    PARAM_FLT(levelset_droplet_radius, 1.2);
    PARAM_FLT(levelset_resolution_factor, 1);
    PARAM_FLT(levelset_max_volume_of_holes_to_close, 8);

    // Filter attributes
    PARAM_FLT(dilate, 0);
    PARAM_FLT(erode, 0);
    PARAM_FLT(smooth, 0);
    static const char* sm_enums[] = SMOOTH_MODE_STRINGS;
    PARAM_ENUM(smooth_mode, MEAN, sm_enums);
    PARAM_INT(smooth_iterations, 1);

    PARAM_BOOL(enable_infinite_blending, false);
    PARAM_FLT(infinite_blending_height, -1);
    PARAM_VEC2(infinite_blending_center, 0, 0);
    PARAM_VEC2(infinite_blending_dimension, 50, 50);
    PARAM_FLT(infinite_blending_radius, -1);
    static const char* bt_enums[] = BLEND_TYPE_STRINGS;
    PARAM_ENUM(infinite_blending_blend, SMOOTH, bt_enums);
    PARAM_BOOL(enable_infinite_blending_uvs, true);
}


PolymeshParams::PolymeshParams(const AtNode* node) : SurfaceParams(node){
    GET_UINT(subdivisions);
    GET_BOOL(smoothing);
    disp_map = AiArrayCopy(AiNodeGetArray(node, "disp_map"));
    GET_FLT(disp_padding);
    GET_FLT(disp_height);
    GET_FLT(disp_zero_value);
    GET_BOOL(disp_autobump);
}
std::string PolymeshParams::str() const{
    std::stringstream ss;
    ss << SurfaceParams::str();
    DUMP_PARAM(subdivisions);
    DUMP_PARAM(smoothing);

    DUMP_PARAM(disp_padding);
    DUMP_PARAM(disp_height);
    DUMP_PARAM(disp_zero_value);
    DUMP_PARAM(disp_autobump);
    return ss.str();
}

void PolymeshParams::declare(AtList *params, AtNodeEntry *nentry){
    SurfaceParams::declare(params, nentry);
    PARAM_UINT(subdivisions, 1);
    PARAM_BOOL(smoothing, true);

    PARAM_ARRAY(disp_map, AiArrayAllocate(0, 1, AI_TYPE_NODE));
    PARAM_FLT(disp_padding, 0);
    PARAM_FLT(disp_height, 1);
    PARAM_FLT(disp_zero_value, 0);
    PARAM_BOOL(disp_autobump, false);
}


ImplicitParams::ImplicitParams(const AtNode* node) : SurfaceParams(node){
}

void ImplicitParams::declare(AtList *params, AtNodeEntry *nentry){
    SurfaceParams::declare(params, nentry);
}

