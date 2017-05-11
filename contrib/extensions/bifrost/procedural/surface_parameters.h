#pragma once

#include <vector>
#include <string>
#include <aminomath/vec.h>
#include <aminomath/bbox.h>
#include <ai_nodes.h>
#include <ai_msg.h>

namespace Surface{

enum RenderComponent{
    VOXELS, PARTICLES
};
enum SmoothMode{
    MEAN, GAUSSIAN, MEDIAN, LAPLACIAN_FLOW, CURVATURE_FLOW
};
enum BlendType{
    LINEAR, SMOOTH, SMOOTHER
};

struct SurfaceParams{
    std::string cache_file;
    std::string object;
    std::vector<std::string> channels;
    std::vector<std::string> velocity_channels;
    std::string uv_channel;

    float velocity_scale;
    float space_scale;
    unsigned int fps;

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

    bool clip;
    amino::Math::bboxf clip_bbox;

    bool enable_infinite_blending;
    float infinite_blending_height;
    amino::Math::vec2f infinite_blending_center;
    amino::Math::vec2f infinite_blending_dimension;
    float infinite_blending_radius;
    BlendType infinite_blending_blend;
    bool enable_infinite_blending_uvs;

    int debug;

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

}

