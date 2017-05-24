#include <ai.h>
#include "volume.h"
#include "defs.h"
#include "utils.h"
#include "debug.h"

VolumeParameters::VolumeParameters() : Bifrost::Processing::VolumeParameters(){}

VolumeParameters::VolumeParameters(const AtNode* node) : Bifrost::Processing::VolumeParameters() {
    GET_SHAPE();
    GET_STR(density_channel);
}

Bifrost::API::String VolumeParameters::str() const {
    std::stringstream ss;
    ss << Bifrost::Processing::VolumeParameters::str().c_str();
    DUMP_PARAM(density_channel);
    return ss.str().c_str();
}

void VolumeParameters::declare(AtList* params, AtNodeEntry* nentry){
    PARAM_SHAPE(({"vorticity"}), ({"velocity"}));
    PARAM_STR(density_channel);
}

Volume* VolumeParameters::volume() const{
    Volume* volume = new Volume(*this);
    report(volume->status());
    if(!volume->status()){
        delete volume;
        return nullptr;
    }
    return volume;
}

Volume::Volume(const VolumeParameters& params) : Bifrost::Processing::Volume(params) {
    samplers.resize(AI_MAX_THREADS, ComponentSampler(component()));
}

bool Volume::sample(const AtString& channel, const AtVector& pos, uint16_t tid, AtParamValue *value, uint8_t *type, int interp){
    const ChannelSampler& sampler = samplers[tid].channelSampler(std::string(channel.c_str()), interp);
    if(!sampler.valid()) return false;
    *type = sampler.type();
    sampler.sample(pos, value);
    return true;
}

AI_VOLUME_NODE_EXPORT_METHODS(BifrostVolumeMtds)

node_parameters
{
    VolumeParameters().declare(params, nentry);
}
volume_create
{
    data->private_info = VolumeParameters(node).volume();
    if(!data->private_info)
        return false;
    data->auto_step_size = .1;
    data->bbox.min.x = data->bbox.min.y = data->bbox.min.z = -10;
    data->bbox.max.x = data->bbox.max.y = data->bbox.max.z =  10;
    return true;
}
volume_cleanup
{
    if(data->private_info) delete (Volume*) data->private_info;
    data->private_info = nullptr;
    return true;
}
volume_sample
{
    if(!data->private_info) return false;
    Volume* volume = static_cast<Volume*>(data->private_info);
    return volume->sample(channel, sg->P, sg->tid, value, type, interp);
}
volume_ray_extents
{
    // TODO: compute extents
    AiVolumeAddIntersection(info, t0, t1);
}
volume_gradient
{
    DUMP("salut");
    return false;
}
volume_update
{
    return true;
}
