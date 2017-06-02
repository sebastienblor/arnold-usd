#include "volume.h"
#include <bifrostprocessing/bifrostprocessing_intersector.h>
#include <ai.h>
#include "defs.h"
#include "utils.h"
#include "debug.h"

VolumeParameters::VolumeParameters() : Bifrost::Processing::VolumeParameters(){}

VolumeParameters::VolumeParameters(const AtNode* node) : Bifrost::Processing::VolumeParameters() {
    GET_SHAPE();
    GET_STR(density_channel);
    GET_FLT(smooth);
    GET_UINT(smooth_iterations);
    GET_FLT(step_size);
}
void VolumeParameters::declare(AtList* params, AtNodeEntry* nentry){
    PARAM_SHAPE(({"vorticity"}));
    PARAM_STR(density_channel);
    PARAM_FLT(smooth);
    PARAM_UINT(smooth_iterations);
}

Bifrost::API::String VolumeParameters::str() const {
    std::stringstream ss;
    ss << Bifrost::Processing::VolumeParameters::str().c_str();
    DUMP_PARAM(step_size);
    return ss.str().c_str();
}

Volume* VolumeParameters::volume() const{
    Volume* volume = new Volume(*this);
    if(!volume->valid()){
        delete volume;
        return nullptr;
    }
    return volume;
}

Volume::Volume(const VolumeParameters& params)
    : _volume(Bifrost::Processing::Volume(params)), _sampler(Sampler(_volume.voxels(), valid()? AI_MAX_THREADS : 0)) {
    DUMP(params.str());
    report(_volume.status());
    if(!valid()) return;
    const Bifrost::API::Component& component = _volume.voxels();
    for(unsigned int i = 0; i < params.channels.count(); ++i){
        ChannelSampler sampler(component, params.channels[i]);
        if(!sampler.valid()){
            AiMsgWarning("[BIFROST] Don't know how to export channel '%s'. Available channels are: '%s'", params.channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
    }
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
    data->auto_step_size = Bifrost::API::Layout(volume->volume().voxels().layout()).voxelScale()*.5;
    data->bbox = Convert(volume->volume().bbox());
    return true;
}
volume_sample
{
    if(!data->private_info) return false;
    Volume* volume = static_cast<Volume*>(data->private_info);
    return volume->sampler().sample(channel, sg->P, sg->tid, value, type, interp, sg->time);
}
volume_gradient
{
    if(!data->private_info) return false;
    Volume* volume = static_cast<Volume*>(data->private_info);
    return volume->sampler().sampleGradient(channel, sg->P, sg->tid, *gradient, interp, sg->time);
}
volume_ray_extents
{
    if(!data->private_info) return;
    //AiVolumeAddIntersection(info, t0, t1); return;
    Volume* volume = static_cast<Volume*>(data->private_info);
    Bifrost::Processing::Intersector intersector(volume->volume().voxels().layout());
    intersector.init(Convert(*origin), Convert(*direction), t0, t1);
    Bifrost::Processing::Interval interval;
    while((interval = intersector.next()).valid()){
        AiVolumeAddIntersection(info, interval.t0, interval.t1);
    }
}
volume_cleanup
{
    if(data->private_info) delete (Volume*) data->private_info;
    data->private_info = nullptr;
    return true;
}
volume_update
{
    return true;
}
