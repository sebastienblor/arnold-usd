#include "bifrost_parameters.h"

struct PointsParams : public BifrostParams {
    float radius;
    bool enable_radius_channel;
    std::string radius_channel; // empty => do not modulate

    bool enable_camera_radius;
    amino::Math::vec2f camera_distance;
    amino::Math::vec2f camera_factor;
    float camera_factor_exponent;

    bool enable_multi_pointing;
    int mp_samples;
    amino::Math::vec2f mp_radius;
    float mp_surface_attract;
    //FallOffType mp_falloff_type;
    int mp_falloff_type;
    amino::Math::vec2f mp_falloff_range;
    float mp_displacement;
    float mp_displacement_noise_frequency;

    int skip;

    // arnold specific
    float step_size;
    unsigned int chunk_size;

    PointsParams(const AtNode* node);
    virtual std::string str() const;
    static void declare(AtList* params, AtNodeEntry* nentry);
};
