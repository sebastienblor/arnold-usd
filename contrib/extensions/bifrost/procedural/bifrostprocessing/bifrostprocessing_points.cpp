#include <bifrostprocessing/bifrostprocessing_points.h>
#include <sstream>
#include "defs.h"

namespace Bifrost{
namespace Processing{

Bifrost::API::String PointsParameters::str() const{
    std::stringstream ss;
    ss << ShapeParameters::str().c_str();

    DUMP_PARAM(radius);
    DUMP_PARAM(enable_radius_channel);
    DUMP_PARAM(radius_channel);

    DUMP_PARAM(enable_camera_radius);
    DUMP_PARAM(camera_distance);
    DUMP_PARAM(camera_factor);
    DUMP_PARAM(camera_factor_exponent);

    DUMP_PARAM(enable_multi_pointing);
    DUMP_PARAM(mp_samples);
    DUMP_PARAM(mp_radius);
    DUMP_PARAM(mp_surface_attract);
    DUMP_PARAM(mp_falloff_type);
    DUMP_PARAM(mp_falloff_range);
    DUMP_PARAM(mp_displacement);
    DUMP_PARAM(mp_displacement_noise_frequency);

    DUMP_PARAM(skip);

    return ss.str().c_str();
}

Points::Points(const PointsParameters& params) : Shape(params) {}

}} // Bifrost::Processing

