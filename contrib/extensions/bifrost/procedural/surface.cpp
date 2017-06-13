#include "surface.h"
#include "utils.h"

SurfaceParameters::SurfaceParameters() : Bifrost::Processing::SurfaceParameters(){}

SurfaceParameters::SurfaceParameters(const AtNode* node) : Bifrost::Processing::SurfaceParameters() {
    GET_SHAPE();
    GET_ENUM(render_component, Bifrost::Processing::RenderComponent);
    distance_channel = AiNodeGetStr(node, "field_channel");

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

    GET_BOOL(enable_ocean_blending);
    GET_FLT(ocean_blending_height);
    GET_VEC2(ocean_blending_center);
    GET_VEC2(ocean_blending_dimension);
    GET_FLT(ocean_blending_radius);
    GET_STR(ocean_blending_out_channel);
    GET_BOOL(enable_ocean_blending_uvs);

    GET_BOOL(export_laplacian);
    GET_BOOL(export_curvature);

    GET_BOOL(ignore_motion_blur);
}

Bifrost::API::String SurfaceParameters::str() const{
    std::stringstream ss;
    ss << Bifrost::Processing::SurfaceParameters::str().c_str();
    DUMP_PARAM(ignore_motion_blur);
    return ss.str().c_str();
}

void SurfaceParameters::declare(AtList* params, AtNodeEntry* nentry)
{
    static const char* rc_enums[] = RENDER_COMPONENT_STRINGS;
    PARAM_ENUM(render_component, rc_enums);
    PARAM_SHAPE(({ "vorticity" }));

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

    PARAM_BOOL(enable_ocean_blending);
    PARAM_FLT(ocean_blending_height);
    PARAM_VEC2(ocean_blending_center);
    PARAM_VEC2(ocean_blending_dimension);
    PARAM_FLT(ocean_blending_radius);
    PARAM_STR(ocean_blending_out_channel);
    PARAM_BOOL(enable_ocean_blending_uvs);

    PARAM_BOOL(export_laplacian);
    PARAM_BOOL(export_curvature);

    PARAM_BOOL(ignore_motion_blur);
}
