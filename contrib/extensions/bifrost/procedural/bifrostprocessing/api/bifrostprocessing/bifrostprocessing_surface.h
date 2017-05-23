#pragma once
#include "bifrostprocessing_shape.h"

namespace Bifrost{
namespace Processing{

class Surface : public Shape {
public:
    // VOXELS
    Bifrost::API::String distance_channel = "distance";

    // PARTICLES
    float levelset_resolution_factor;
    float levelset_droplet_reveal_factor;
    float levelset_surface_radius;
    float levelset_droplet_radius;
    float levelset_max_volume_of_holes_to_close;

    // Filter attributes
    float dilate = 0;
    float erode = 0;
    float smooth = 0;
    SmoothMode smooth_mode = Mean;
    int smooth_iterations = 1;

    bool enable_ocean_blending = false;
    float ocean_blending_height = 0;
    amino::Math::vec2f ocean_blending_center = amino::Math::vec2f(0,0);
    amino::Math::vec2f ocean_blending_dimension = amino::Math::vec2f(10,10);
    float ocean_blending_radius = 1;
    BlendType ocean_blending_blend = Smooth;
    bool enable_ocean_blending_uvs = true;

    virtual Status initialize() override;
    void mesh(Bifrost::API::Array<amino::Math::vec3f>& vertices, Bifrost::API::Array<amino::Math::vec3i>& indices, unsigned int subdivisions=1) const;
    virtual Bifrost::API::String str() const override;
};

}}
