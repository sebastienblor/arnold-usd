#include "surface_parameters.h"

#include <ai_params.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <ai_msg.h>
#include <aminomath/bbox.h>
#include <aminomath/vec.h>
#include <vector>
#include "utils.h"

#define DUMP_PARAM(param) ss << "    " << #param << " = " << (param) << std::endl;
#define DUMP_ARRAY_PARAM(param) \
    ss << "    " << #param << " = " << std::endl; \
    for(const std::string& s : param) \
        ss << "        " << s << std::endl;

namespace {
    std::vector<std::string> ArrayToStrings(const AtArray* array){
        std::vector<std::string> strings;
        for(unsigned int i = 0; i < AiArrayGetNumElements(array); ++i)
            strings.push_back(AiArrayGetStr(array,i).c_str());
        return strings;
    }

    AtArray* CreateStringArray(const std::vector<std::string>& strings){
        AtArray* array = AiArrayAllocate(strings.size(),1,AI_TYPE_STRING);
        for(unsigned int i = 0; i < strings.size(); ++i)
            AiArraySetStr(array, i, strings[i].c_str());
        return array;
    }
}

namespace Surface{

SurfaceParams::SurfaceParams(const AtNode* node){
    cache_file = AiNodeGetStr(node, "cache_file");
    object = AiNodeGetStr(node, "object");
    channels = ArrayToStrings(AiNodeGetArray(node, "channels"));
    velocity_channels = ArrayToStrings(AiNodeGetArray(node, "velocity_channels"));
    uv_channel = AiNodeGetStr(node, "uv_channel");

    velocity_scale = AiNodeGetFlt(node, "velocity_scale");
    space_scale = AiNodeGetFlt(node, "space_scale");
    fps = AiNodeGetUInt(node, "fps");

    render_component = (RenderComponent) AiNodeGetInt(node, "render_component");

    distance_channel = AiNodeGetStr(node, "distance_channel");

    levelset_resolution_factor = AiNodeGetFlt(node, "levelset_resolution_factor");
    levelset_droplet_reveal_factor = AiNodeGetFlt(node, "levelset_droplet_reveal_factor");
    levelset_surface_radius = AiNodeGetFlt(node, "levelset_surface_radius");
    levelset_droplet_radius = AiNodeGetFlt(node, "levelset_droplet_radius");
    levelset_max_volume_of_holes_to_close = AiNodeGetFlt(node, "levelset_max_volume_of_holes_to_close");

    dilate = AiNodeGetFlt(node, "dilate");
    erode = AiNodeGetFlt(node, "erode");
    smooth = AiNodeGetFlt(node, "smooth");
    smooth_mode = (SmoothMode) AiNodeGetInt(node, "smooth_mode");
    smooth_iterations = AiNodeGetInt(node, "smooth_iterations");

    clip = AiNodeGetBool(node, "clip");
    clip_bbox = amino::Math::bboxf( AtVectorToAminoVec3f(AiNodeGetVec(node,"clip_min")),
                                    AtVectorToAminoVec3f(AiNodeGetVec(node,"clip_max")) );

    enable_infinite_blending = AiNodeGetBool(node, "enable_infinite_blending");
    infinite_blending_height = AiNodeGetFlt(node, "infinite_blending_height");
    infinite_blending_center = AtVector2ToAminoVec2f(AiNodeGetVec2(node,"infinite_blending_center"));
    infinite_blending_dimension = AtVector2ToAminoVec2f(AiNodeGetVec2(node,"infinite_blending_dimension"));
    infinite_blending_radius = AiNodeGetFlt(node, "infinite_blending_radius");
    infinite_blending_blend = (BlendType) AiNodeGetInt(node, "infinite_blending_blend");
    enable_infinite_blending_uvs = AiNodeGetBool(node, "enable_infinite_blending_uvs");

    debug = AiNodeGetInt(node, "debug");
}

std::string SurfaceParams::str() const{
    std::stringstream ss;
    ss << std::endl;
    DUMP_PARAM(cache_file);
    DUMP_PARAM(object);
    DUMP_ARRAY_PARAM(channels);
    DUMP_ARRAY_PARAM(velocity_channels);
    DUMP_PARAM(uv_channel);

    DUMP_PARAM(velocity_scale);
    DUMP_PARAM(space_scale);
    DUMP_PARAM(fps);

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

    DUMP_PARAM(clip);
    DUMP_PARAM(clip_bbox);

    DUMP_PARAM(enable_infinite_blending);
    DUMP_PARAM(infinite_blending_height);
    DUMP_PARAM(infinite_blending_center);
    DUMP_PARAM(infinite_blending_dimension);
    DUMP_PARAM(infinite_blending_radius);
    DUMP_PARAM(infinite_blending_blend);
    DUMP_PARAM(enable_infinite_blending_uvs);

    DUMP_PARAM(debug);
    return ss.str();
}

void SurfaceParams::declare(AtList* params, AtNodeEntry* nentry){
    static const char* RenderComponents[] = {
        "voxels",
        "particles",
        NULL
    };
    static const char* SmoothModes[] = {
        "mean",
        "gaussian",
        "median",
        "laplacian_flow",
        "curvature_flow",
        NULL
    };
    static const char* BlendTypes[] = {
        "linear",
        "smooth",
        "smoother",
        NULL
    };

    AiParameterStr("cache_file", "");
    AiParameterStr("object", "");
    AiParameterArray("channels", CreateStringArray({"vorticity"}));
    AiParameterArray("velocity_channels", CreateStringArray({"velocity_u", "velocity_v", "velocity_w"}));
    AiParameterStr("uv_channel", "uv");

    AiParameterFlt("velocity_scale", 1);
    AiParameterFlt("space_scale", 1);
    AiParameterUInt("fps", 24);

    AiParameterEnum("render_component", VOXELS, RenderComponents);
    // VOXELS
    AiParameterStr("distance_channel", "");

    // PARTICLES
    AiParameterFlt("levelset_droplet_reveal_factor", 3);
    AiParameterFlt("levelset_surface_radius", 1.4);
    AiParameterFlt("levelset_droplet_radius", 1.2);
    AiParameterFlt("levelset_resolution_factor", 1);
    AiParameterFlt("levelset_max_volume_of_holes_to_close", 8);

    // Filter attributes
    AiParameterFlt("dilate", 0);
    AiParameterFlt("erode", 0);
    AiParameterFlt("smooth", 0);
    AiParameterEnum("smooth_mode", MEAN, SmoothModes);
    //AiParameterInt("smooth_kernel" , 0);
    AiParameterInt("smooth_iterations", 1);

    AiParameterBool("clip", 0);
    AiParameterVec("clip_min", -1, -1, -1);
    AiParameterVec("clip_max",  1,  1,  1);

    AiParameterBool("enable_infinite_blending", false);
    AiParameterBool("infinite_blending_auto_height", true);
    AiParameterFlt("infinite_blending_height", -1);
    AiParameterVec2("infinite_blending_center", 0, 0);
    AiParameterVec2("infinite_blending_dimension", 100, 100);
    AiParameterFlt("infinite_blending_radius", -1);
    AiParameterEnum("infinite_blending_blend", SMOOTH, BlendTypes);
    AiParameterBool("enable_infinite_blending_uvs", true);

    AiParameterInt("debug", 0);
}


PolymeshParams::PolymeshParams(const AtNode* node) : SurfaceParams(node){
    subdivisions = AiNodeGetUInt(node, "subdivisions");
    smoothing = AiNodeGetBool(node, "smoothing");
    disp_map = AiArrayCopy(AiNodeGetArray(node, "disp_map"));
    disp_padding = AiNodeGetFlt(node, "disp_padding");
    disp_height = AiNodeGetFlt(node, "disp_height");
    disp_zero_value = AiNodeGetFlt(node, "disp_zero_value");
    disp_autobump = AiNodeGetBool(node, "disp_autobump");
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
    AiParameterUInt("subdivisions", 1);
    AiParameterBool("smoothing", true);

    AiParameterArray("disp_map", AiArrayAllocate(0, 1, AI_TYPE_NODE));
    AiParameterFlt("disp_padding", 0);
    AiParameterFlt("disp_height", 1);
    AiParameterFlt("disp_zero_value", 0);
    AiParameterBool("disp_autobump", false);
}


ImplicitParams::ImplicitParams(const AtNode* node) : SurfaceParams(node){
}

void ImplicitParams::declare(AtList *params, AtNodeEntry *nentry){
    SurfaceParams::declare(params, nentry);
}

} // Surface
