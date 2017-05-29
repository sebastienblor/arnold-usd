#pragma once
#include <bifrostprocessing/bifrostprocessing_shape.h>

namespace Bifrost{
namespace Processing{

struct SurfaceParameters : public ShapeParameters {
    RenderComponent render_component = RenderComponent::Volume;
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
    SmoothMode smooth_mode = SmoothMode::Laplacian;
    int smooth_iterations = 1;

    bool enable_ocean_blending = false;
    float ocean_blending_height = 0;
    amino::Math::vec2f ocean_blending_center = amino::Math::vec2f(0,0);
    amino::Math::vec2f ocean_blending_dimension = amino::Math::vec2f(10,10);
    float ocean_blending_radius = 1;
    BlendType ocean_blending_blend = BlendType::Smooth;
    bool enable_ocean_blending_uvs = true;

    bool export_laplacian = true;
    bool export_curvature = true;

    virtual Bifrost::API::String str() const override;
};

class Surface : public Shape {
public:
    Surface(const SurfaceParameters& params);
    void mesh(Bifrost::API::Array<amino::Math::vec3f>& vertices, Bifrost::API::Array<amino::Math::vec3i>& indices, unsigned int subdivisions=1) const;

private:
    Bifrost::API::String distance_channel;
};

}}
