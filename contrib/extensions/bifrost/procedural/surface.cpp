#include "surface.h"
#include "defs.h"
#include <ai_params.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <ai_msg.h>
#include <ai.h>
#include <vector>
#include <string>

namespace{

Bifrost::API::StringArray ArrayToStrings(const AtArray* array){
    Bifrost::API::StringArray strings;
    for(unsigned int i = 0; i < AiArrayGetNumElements(array); ++i)
        strings.add(AiArrayGetStr(array,i).c_str());
    return strings;
}

AtArray* CreateStringArray(const std::vector<std::string>& strings){
    AtArray* array = AiArrayAllocate(strings.size(),1,AI_TYPE_STRING);
    for(unsigned int i = 0; i < strings.size(); ++i)
        AiArraySetStr(array, i, strings[i].c_str());
    return array;
}

}

Surface::Surface() : Bifrost::Processing::Surface(){}

Surface::Surface(const AtNode* node) : Bifrost::Processing::Surface() {
    GET_STR(cache_file);
    GET_STR(object);
    GET_STR_ARRAY(channels);
    GET_STR_ARRAY(velocity_channels);
    GET_STR(uv_channel);

    GET_FLT(velocity_scale);
    GET_FLT(space_scale);
    GET_UINT(fps);

    GET_BOOL(clip);
    GET_BBOX(clip);

    GET_ENUM(render_component, Bifrost::Processing::RenderComponent);

    GET_STR(distance_channel);

    GET_FLT(levelset_resolution_factor);
    GET_FLT(levelset_droplet_reveal_factor);
    GET_FLT(levelset_surface_radius);
    GET_FLT(levelset_droplet_radius);
    GET_FLT(levelset_max_volume_of_holes_to_close);

    GET_FLT(dilate);
    GET_FLT(erode);
    GET_FLT(smooth);
    GET_ENUM(smooth_mode, Bifrost::Processing::SmoothMode);
    GET_INT(smooth_iterations);

    GET_BOOL(enable_infinite_blending);
    GET_FLT(infinite_blending_height);
    GET_VEC2(infinite_blending_center);
    GET_VEC2(infinite_blending_dimension);
    GET_FLT(infinite_blending_radius);
    GET_ENUM(infinite_blending_blend, Bifrost::Processing::BlendType);
    GET_BOOL(enable_infinite_blending_uvs);
}

Bifrost::API::String Surface::str() const{
    return Bifrost::Processing::Surface::str();
}

void Surface::declare(AtList* params, AtNodeEntry* nentry)
{
    PARAM_STR(cache_file);
    PARAM_STR(object);
    PARAM_ARRAY(channels, CreateStringArray({"vorticity"}));
    PARAM_ARRAY(velocity_channels, CreateStringArray({"velocity_u", "velocity_v", "velocity_w"}));
    PARAM_STR(uv_channel);

    PARAM_FLT(velocity_scale);
    PARAM_FLT(space_scale);
    PARAM_UINT(fps);

    PARAM_BOOL(clip);
    PARAM_BBOX(clip, -1,-1,-1, 1,1,1)

    static const char* rc_enums[] = RENDER_COMPONENT_STRINGS;
    PARAM_ENUM(render_component, rc_enums);
    // VOXELS
    PARAM_STR(distance_channel);

    // PARTICLES
    PARAM_FLT(levelset_droplet_reveal_factor);
    PARAM_FLT(levelset_surface_radius);
    PARAM_FLT(levelset_droplet_radius);
    PARAM_FLT(levelset_resolution_factor);
    PARAM_FLT(levelset_max_volume_of_holes_to_close);

    // Filter attributes
    PARAM_FLT(dilate);
    PARAM_FLT(erode);
    PARAM_FLT(smooth);
    static const char* sm_enums[] = SMOOTH_MODE_STRINGS;
    PARAM_ENUM(smooth_mode, sm_enums);
    PARAM_INT(smooth_iterations);

    PARAM_BOOL(enable_infinite_blending);
    PARAM_FLT(infinite_blending_height);
    PARAM_VEC2(infinite_blending_center);
    PARAM_VEC2(infinite_blending_dimension);
    PARAM_FLT(infinite_blending_radius);
    static const char* bt_enums[] = BLEND_TYPE_STRINGS;
    PARAM_ENUM(infinite_blending_blend, bt_enums);
    PARAM_BOOL(enable_infinite_blending_uvs);
}

Polymesh::Polymesh() : Surface() {}

Polymesh::Polymesh(const AtNode *node) : Surface(node){
    GET_UINT(subdivisions);
    GET_BOOL(smoothing);
    disp_map = AiArrayCopy(AiNodeGetArray(node, "disp_map"));
    GET_FLT(disp_padding);
    GET_FLT(disp_height);
    GET_FLT(disp_zero_value);
    GET_BOOL(disp_autobump);
}

Bifrost::API::String Polymesh::str() const{
    std::stringstream ss;
    ss << Surface::str().c_str();
    DUMP_PARAM(subdivisions);
    DUMP_PARAM(smoothing);

    DUMP_PARAM(disp_padding);
    DUMP_PARAM(disp_height);
    DUMP_PARAM(disp_zero_value);
    DUMP_PARAM(disp_autobump);
    return ss.str().c_str();
}

void Polymesh::declare(AtList *params, AtNodeEntry *nentry){
    Surface::declare(params, nentry);
    PARAM_UINT(subdivisions);
    PARAM_BOOL(smoothing);

    PARAM_ARRAY(disp_map, AiArrayAllocate(0, 1, AI_TYPE_NODE));
    PARAM_FLT(disp_padding);
    PARAM_FLT(disp_height);
    PARAM_FLT(disp_zero_value);
    PARAM_BOOL(disp_autobump);
}
