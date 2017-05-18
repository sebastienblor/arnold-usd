#pragma once

#include "bifrost_parameters.h"

struct SurfaceParams : public BifrostParams {
    RenderComponent render_component;
    // VOXELS
    std::string distance_channel;

    // PARTICLES
    float levelset_resolution_factor;
    float levelset_droplet_reveal_factor;
    float levelset_surface_radius;
    float levelset_droplet_radius;
    float levelset_max_volume_of_holes_to_close;

    // Filter attributes
    float dilate;
    float erode;
    float smooth;
    SmoothMode smooth_mode;
    //intsmooth_kernel;
    int smooth_iterations;

    bool enable_infinite_blending;
    float infinite_blending_height;
    amino::Math::vec2f infinite_blending_center;
    amino::Math::vec2f infinite_blending_dimension;
    float infinite_blending_radius;
    BlendType infinite_blending_blend;
    bool enable_infinite_blending_uvs;

    SurfaceParams(const AtNode* node);
    virtual std::string str() const;
    static void declare(AtList* params, AtNodeEntry* nentry);
};

struct PolymeshParams : public SurfaceParams{
    unsigned int subdivisions;
    bool smoothing;

    AtArray* disp_map;
    float disp_padding;
    float disp_height;
    float disp_zero_value;
    bool disp_autobump;

    PolymeshParams(const AtNode* node);
    virtual std::string str() const override;
    static void declare(AtList *params, AtNodeEntry *nentry);
};

struct ImplicitParams : public SurfaceParams{
    ImplicitParams(const AtNode* node);
    static void declare(AtList *params, AtNodeEntry *nentry);
};

