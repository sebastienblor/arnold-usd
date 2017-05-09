#include "surface_parameters.h"

#include <ai_params.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <ai_msg.h>
#include <aminomath/bbox.h>
#include <aminomath/vec.h>
#include <vector>
#include "utils.h"

namespace Surface{

SurfaceParams::SurfaceParams(const AtNode* node){
    cache_file = AiNodeGetStr(node, "cache_file");
    object = AiNodeGetStr(node, "object");
    AtArray* channels = AiNodeGetArray(node, "channels");
    for(unsigned int i = 0; i < AiArrayGetNumElements(channels); ++i)
        this->channels.push_back(AiArrayGetStr(channels,i).c_str());

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
    clip_bbox = amino::Math::bboxf( AtVectorToAminoVec3(AiNodeGetVec(node,"clip_min")),
                                    AtVectorToAminoVec3(AiNodeGetVec(node,"clip_max")) );

    enable_infinite_blending = AiNodeGetBool(node, "enable_infinite_blending");
    infinite_blending_height = AiNodeGetFlt(node, "infinite_blending_height");
    infinite_blending_center = AtVector2ToAminoVec2(AiNodeGetVec2(node,"infinite_blending_center"));
    infinite_blending_dimension = AtVector2ToAminoVec2(AiNodeGetVec2(node,"infinite_blending_dimension"));
    infinite_blending_radius = AiNodeGetFlt(node, "infinite_blending_radius");
    infinite_blending_blend = (BlendType) AiNodeGetInt(node, "infinite_blending_blend");
    enable_infinite_blending_uvs = AiNodeGetBool(node, "enable_infinite_blending_uvs");

    debug = AiNodeGetInt(node, "debug");
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
    AtArray* channels = AiArrayAllocate(1,1,AI_TYPE_STRING);
    AiArraySetStr(channels, 0, "vorticity");
    AiParameterArray("channels", channels);

    AiParameterFlt("velocity_scale", 1);
    AiParameterFlt("space_scale", 1);
    AiParameterUInt("fps", 24);

    AiParameterEnum("render_component", VOXELS, RenderComponents);
    // VOXELS
    AiParameterStr("distance_channel", "");

    // PARTICLES
    AiParameterFlt("levelset_resolution_factor", 0);
    AiParameterFlt("levelset_droplet_reveal_factor", 0);
    AiParameterFlt("levelset_surface_radius", 0);
    AiParameterFlt("levelset_droplet_radius", 0);
    AiParameterFlt("levelset_max_volume_of_holes_to_close", 0);

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
