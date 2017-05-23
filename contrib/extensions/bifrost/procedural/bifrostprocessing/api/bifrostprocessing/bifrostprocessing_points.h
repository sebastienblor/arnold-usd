#pragma once
#include "bifrostprocessing_shape.h"

namespace Bifrost{
namespace Processing{

class Points : public Shape {
public:
    float radius;
    bool enable_radius_channel = true;
    Bifrost::API::String radius_channel = "density"; // empty => do not modulate

    bool enable_camera_radius = false;
    amino::Math::vec2f camera_distance = amino::Math::vec2f(0,1);
    amino::Math::vec2f camera_factor = amino::Math::vec2f(0,1);
    float camera_factor_exponent = 1;

    bool enable_multi_pointing = false;
    int mp_samples = 0;
    amino::Math::vec2f mp_radius = amino::Math::vec2f(.5,1.5);
    float mp_surface_attract = 0;
    //FallOffType mp_falloff_type;
    int mp_falloff_type = 0;
    amino::Math::vec2f mp_falloff_range = amino::Math::vec2f(0,1);
    float mp_displacement = 0;
    float mp_displacement_noise_frequency = 8;

    int skip = 0;

    Points(const amino::Math::vec3f& camera_position);
    virtual Status initialize() override;
    virtual Bifrost::API::String str() const;

private:
    amino::Math::vec3f camera_position;
};


}} // Bifrost::Processing
