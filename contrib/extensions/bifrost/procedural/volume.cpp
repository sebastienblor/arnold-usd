#include "volume.h"
#include <ai.h>
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

Volume::Volume(const VolumeParameters& params) : volume(Bifrost::Processing::Volume(params)) {
    if(!volume.status()) return;
    samplers.resize(AI_MAX_THREADS, ComponentSampler(volume.component()));
}

bool Volume::sample(const AtString& channel, const AtVector& pos, uint16_t tid, AtParamValue *value, uint8_t *type, int interp){
    const ChannelSampler& sampler = samplers[tid].channelSampler(std::string(channel.c_str()), interp);
    if(!sampler.valid()) return false;
    *type = sampler.type();
    sampler.sample(pos, value);
    return true;
}
AtBBox Volume::bbox() const{
    return Convert(volume.bbox());
}

AI_VOLUME_NODE_EXPORT_METHODS(BifrostVolumeMtds)

node_parameters
{
    VolumeParameters().declare(params, nentry);
}
volume_create
{
    Volume* volume = VolumeParameters(node).volume();
    data->private_info = volume;
    if(!data->private_info)
        return false;
    data->auto_step_size = .1;
    data->bbox = volume->bbox();
    DUMP(data->bbox);
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
