#include "points_parameters.h"
#include "utils.h"

PointsParams::PointsParams(const AtNode* node) : BifrostParams(node) {
    GET_FLT(radius);
    GET_BOOL(enable_radius_channel);
    GET_STR(radius_channel);

    GET_BOOL(enable_camera_radius);
    GET_VEC2(camera_distance);
    GET_VEC2(camera_factor);
    GET_FLT(camera_factor_exponent);

    GET_BOOL(enable_multi_pointing);
    GET_INT(mp_samples);
    GET_VEC2(mp_radius);
    GET_FLT(mp_surface_attract);
    GET_INT(mp_falloff_type);
    GET_VEC2(mp_falloff_range);
    GET_FLT(mp_displacement);
    GET_FLT(mp_displacement_noise_frequency);

    GET_INT(skip);

    GET_FLT(step_size);
    GET_UINT(chunk_size);
}

std::string PointsParams::str() const{
    std::stringstream ss;
    ss << BifrostParams::str();

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

    // arnold specific
    DUMP_PARAM(step_size);
    DUMP_PARAM(chunk_size);

    return ss.str();
}

void PointsParams::declare(AtList* params, AtNodeEntry* nentry)
{
    BifrostParams::declare(params, nentry);
    PARAM_FLT(radius, .1);
    PARAM_BOOL(enable_radius_channel, false);
    PARAM_STR(radius_channel, "density");

    PARAM_BOOL(enable_camera_radius, false);
    PARAM_VEC2(camera_distance, 0, 1);
    PARAM_VEC2(camera_factor, 0, 1);
    PARAM_FLT(camera_factor_exponent, 1);

    PARAM_BOOL(enable_multi_pointing, false);
    PARAM_INT(mp_samples, 1);
    PARAM_VEC2(mp_radius, 0, 1);
    PARAM_FLT(mp_surface_attract, 0);
    PARAM_INT(mp_falloff_type, 0);
    PARAM_VEC2(mp_falloff_range, 0, 1);
    PARAM_FLT(mp_displacement, 0);
    PARAM_FLT(mp_displacement_noise_frequency, 8);

    PARAM_INT(skip, 0);

    PARAM_FLT(step_size, .025);
    PARAM_UINT(chunk_size, 100000);
}
