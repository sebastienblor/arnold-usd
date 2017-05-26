#include "implicit.h"
#include <ai.h>
#include "debug.h"
#include "utils.h"
#include <bifrostapi/bifrost_layout.h>

Implicit* ImplicitParameters::implicit() const{
    Implicit* implicit = new Implicit(*this);
    if(!implicit->valid()){
        delete implicit;
        return nullptr;
    }
    return implicit;
}

Implicit::Implicit(const ImplicitParameters& params)
    : _surface(Bifrost::Processing::Surface(params)), _sampler(Sampler(_surface.component(), valid()? AI_MAX_THREADS : 0)) {
    DUMP(params.str());
    report(_surface.status());
    if(!valid()) return;
    const Bifrost::API::Component& component = _surface.component();
    for(unsigned int i = 0; i < params.channels.count(); ++i){
        ChannelSampler sampler(component, params.channels[i]);
        if(!sampler.valid()){
            AiMsgWarning("[BIFROST] Don't know how to export channel '%s'. Available channels are: '%s'", params.channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
    }
}

AI_VOLUME_NODE_EXPORT_METHODS(BifrostImplicitMtds)

node_parameters
{
    ImplicitParameters().declare(params, nentry);
}
volume_create
{
    Implicit* implicit = ImplicitParameters(node).implicit();
    data->private_info = implicit;
    if(!data->private_info)
        return false;
    data->auto_step_size = .1;
    data->bbox = Convert(implicit->surface().bbox());
    DUMP(data->bbox);
    return true;
}
volume_sample
{
    if(!data->private_info) return false;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    return implicit->sampler().sample(channel, sg->P, sg->tid, value, type, interp, sg->time);
}
volume_gradient
{
    if(!data->private_info) return false;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    return implicit->sampler().sampleGradient(channel, sg->P, sg->tid, *gradient, interp, sg->time);
}
volume_ray_extents
{
    // TODO: compute extents
    AiVolumeAddIntersection(info, t0, t1);
}
volume_cleanup
{
    if(data->private_info) delete (Implicit*) data->private_info;
    data->private_info = nullptr;
    return true;
}
volume_update
{
    return true;
}
