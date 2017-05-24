#include "surface.h"
#include "utils.h"

SurfaceParameters::SurfaceParameters() : Bifrost::Processing::SurfaceParameters(){}

SurfaceParameters::SurfaceParameters(const AtNode* node) : Bifrost::Processing::SurfaceParameters() {
    GET_SHAPE();
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

    GET_BOOL(enable_ocean_blending);
    GET_FLT(ocean_blending_height);
    GET_VEC2(ocean_blending_center);
    GET_VEC2(ocean_blending_dimension);
    GET_FLT(ocean_blending_radius);
    GET_ENUM(ocean_blending_blend, Bifrost::Processing::BlendType);
    GET_BOOL(enable_ocean_blending_uvs);
}

Bifrost::API::String SurfaceParameters::str() const{ return Bifrost::Processing::SurfaceParameters::str(); }

void SurfaceParameters::declare(AtList* params, AtNodeEntry* nentry)
{
    PARAM_SHAPE(({ "vorticity" }), ({ "velocity_u", "velocity_v", "velocity_w" }));
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

    PARAM_BOOL(enable_ocean_blending);
    PARAM_FLT(ocean_blending_height);
    PARAM_VEC2(ocean_blending_center);
    PARAM_VEC2(ocean_blending_dimension);
    PARAM_FLT(ocean_blending_radius);
    static const char* bt_enums[] = BLEND_TYPE_STRINGS;
    PARAM_ENUM(ocean_blending_blend, bt_enums);
    PARAM_BOOL(enable_ocean_blending_uvs);
}
