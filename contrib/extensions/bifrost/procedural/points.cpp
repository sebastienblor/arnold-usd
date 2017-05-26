#include "points.h"
#include <ai.h>
#include <bifrostapi/bifrost_pointcomponent.h>
#include "defs.h"
#include "utils.h"
#include "debug.h"
#include "tbb.h"
#include "sampler.h"

PointsParameters::PointsParameters() : Bifrost::Processing::PointsParameters() {}

PointsParameters::PointsParameters(const AtNode* node) : Bifrost::Processing::PointsParameters() {
    GET_SHAPE();
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
    GET_ENUM(mode, PointsParameters::Mode);
}

Bifrost::API::String PointsParameters::str() const{
    std::stringstream ss;
    ss << Bifrost::Processing::PointsParameters::str().c_str();
    DUMP_PARAM(step_size);
    DUMP_PARAM(chunk_size);
    DUMP_PARAM(mode);
    return ss.str().c_str();
}

void PointsParameters::declare(AtList* params, AtNodeEntry* nentry){
    PARAM_SHAPE(({}));

    PARAM_FLT(radius);
    PARAM_BOOL(enable_radius_channel);
    PARAM_STR(radius_channel);

    PARAM_BOOL(enable_camera_radius);
    PARAM_VEC2(camera_distance);
    PARAM_VEC2(camera_factor);
    PARAM_FLT(camera_factor_exponent);

    PARAM_BOOL(enable_multi_pointing);
    PARAM_INT(mp_samples);
    PARAM_VEC2(mp_radius);
    PARAM_FLT(mp_surface_attract);
    PARAM_INT(mp_falloff_type);
    PARAM_VEC2(mp_falloff_range);
    PARAM_FLT(mp_displacement);
    PARAM_FLT(mp_displacement_noise_frequency);

    PARAM_INT(skip);

    PARAM_FLT(step_size);
    PARAM_UINT(chunk_size);
    static const char* mode_enums[] = { "disk", "sphere" };
    PARAM_ENUM(mode, mode_enums);
}

std::vector<AtNode*>* PointsParameters::nodes() const{
    Bifrost::Processing::Points points(*this);
    DUMP(this->str());
    report(points.status());
    if(!points.status()){
        return nullptr;
    }

    float shutter_start, shutter_end;
    bool motion = getMotion(shutter_start, shutter_end);
    bool need_velocity = motion || (shutter_start != 0);

    Bifrost::API::PointComponent pointComponent(points.points());
    Bifrost::API::VoxelComponent voxels(points.voxels());

    AtNode *node = AiNode("points");
    Bifrost::API::Array<amino::Math::vec3f> positions;
    points.positions(positions);

    {// export positions
        // TODO: factorize that out (same in polymesh.cpp for vertices)
        ChannelSampler velocities;
        if(need_velocity){
            velocities = ChannelSampler(voxels, velocity_channel);
            if(!velocities.valid()){
                motion = need_velocity = false;
                shutter_start = shutter_end = 0;
                AiMsgWarning("[BIFROST] Invalid velocity channel: %s. Ignoring motion...", velocity_channel.c_str());
            }
        }

        AtArray *vlist = AiArrayAllocate( positions.count(), motion? 2 : 1, AI_TYPE_VECTOR );
        if(need_velocity){
            TBB_FOR_ALL(0, positions.count(), 100, [vlist,&positions,&velocities,motion,shutter_start,shutter_end](size_t start, size_t end){
                ChannelSampler vsampler(velocities);
                AtParamValue value;
                for(size_t i = start; i < end; ++i){
                    AtVector pos(Convert(positions[i]));
                    vsampler.sample(pos, &value);
                    AtVector vel(value.VEC());
                    AiArraySetVec(vlist, i, pos + shutter_start * vel);
                    if(motion) AiArraySetVec(vlist, positions.count()+i, pos + shutter_end * vel);
                }
            });
            AiNodeSetFlt(node, "motion_start", shutter_start);
            AiNodeSetFlt(node, "motion_end", shutter_end);
        }else{
            TBB_FOR_ALL(0, positions.count(), 100, [vlist,&positions](size_t i){
                AiArraySetVec(vlist, i, Convert(positions[i]));
            });
        }
        AiNodeSetArray(node, "points", vlist);
    }

    {// export radius
        const float radius = this->radius;
        AtArray* radius_array = AiArrayAllocate(pointComponent.elementCount(), 1, AI_TYPE_FLOAT);
        TBB_FOR_ALL(0, positions.count(), 100, [radius_array, radius](size_t i){
            AiArraySetFlt(radius_array, i, radius);
        });
        AiNodeSetArray(node, "radius", radius_array);
    }

    if(voxels.valid()){
        for(unsigned int i = 0; i < channels.count(); ++i){
            ChannelSampler sampler(voxels, channels[i]);
            if(!sampler.valid()){
                AiMsgWarning("[BIFROST] Don't know how to export channel '%s', Available channels are: %s", channels[i].c_str(), availableChannels(voxels).c_str());
                continue;
            }
            AiNodeDeclare(node, channels[i].c_str(), (std::string("uniform ")+typeName(sampler.type())).c_str()); \
            AiNodeSetArray(node, channels[i].c_str(), sampler.array(positions));
        }
    }else if(channels.count() > 0){
        AiMsgWarning("[BIFROST] Invalid voxels. Ignoring channel export...");
    }

    AiNodeSetInt(node, "mode", mode);
    AiNodeSetFlt(node, "step_size", step_size);

    return new std::vector<AtNode*>({ node });
}

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPointsMtds)

node_parameters
{
    PointsParameters().declare(params,nentry);
}
procedural_init
{
    return (*user_ptr = PointsParameters(node).nodes()) != nullptr;
}
procedural_cleanup
{
    if(user_ptr) delete static_cast< std::vector<AtNode*>* >(user_ptr);
    return true;
}
procedural_num_nodes
{
    return user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr)->size() : 0;
}
procedural_get_node
{
    std::vector<AtNode*>* nodes = user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr) : nullptr;
    return (nodes && i >= 0 && i < (int) nodes->size())? (*nodes)[i] : nullptr;
}
