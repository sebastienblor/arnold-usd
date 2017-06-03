#include "implicit.h"
#include <bifrostprocessing/bifrostprocessing_intersector.h>
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
    : _surface(Bifrost::Processing::Surface(params)),
      _sampler(Sampler(_surface.voxels(), valid()? AI_MAX_THREADS : 0)),
      _intersector(Bifrost::Processing::Intersector(_surface.voxels().layout())) {
    DUMP(params.str());
    report(_surface.status());
    if(!valid()) return;
    const Bifrost::API::Component& component = _surface.voxels();
    for(unsigned int i = 0; i < params.channels.count(); ++i){
        ChannelSampler sampler(component, params.channels[i]);
        if(!sampler.valid()){
            AiMsgWarning("[BIFROST] Don't know how to export channel '%s'. Available channels are: '%s'", params.channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
    }
    Bifrost::API::Layout layout(_surface.voxels().layout());
    // padding to avoid qb spline samplers to pick up background value when evaluating sdf
    _padding = -( layout.voxelScale() * layout.tileDimInfo().tileWidth * .5 );
    float shutter_start, shutter_end;
    _hasMotion = !params.ignore_motion_blur && getMotion(shutter_start, shutter_end) && (shutter_start != 0 || shutter_start != shutter_end);
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
    data->auto_step_size = Bifrost::API::Layout(implicit->surface().voxels().layout()).voxelScale()*.2;
    data->bbox = pad(Convert(implicit->surface().bbox()), implicit->padding());
    return true;
}
volume_sample
{
    if(!data->private_info) return false;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    Bifrost::API::Layout layout(implicit->surface().voxels().layout());
    float invDx = 1./layout.voxelScale();
    int depth = layout.tileAccessor().index((int)(sg->Po.x*invDx), (int)(sg->Po.y*invDx), (int)(sg->Po.z*invDx), layout.maxDepth()).depth;
    if(depth != layout.maxDepth()){
        return false;
    }
    return implicit->sampler().sample(channel, sg->P, sg->tid, value, type, interp, implicit->hasMotion()? sg->time : 0);
}
volume_gradient
{
    if(!data->private_info) return false;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    return implicit->sampler().sampleGradient(channel, sg->P, sg->tid, *gradient, interp, implicit->hasMotion()? sg->time : 0);
}
volume_ray_extents
{
    if(!data->private_info) return;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    float l = direction->x*direction->x+direction->y*direction->y+direction->z*direction->z;
    assert(fabs(l-1) < EPS);
    //AiVolumeAddIntersection(info, t0, t1); return;
    Bifrost::Processing::Intersector intersector(implicit->intersector());
    const float padding = implicit->padding() * 1;
    //t0 -= padding; t1 += padding;
    bool debug = false;
    intersector.init(Convert(*origin), Convert(*direction), t0, t1, true);
    std::vector<Bifrost::Processing::Interval> intervals;
    Bifrost::Processing::Interval interval;
    while((interval = intersector.next()).valid()){
        intervals.push_back(interval);
        interval.t0 -= padding;
        interval.t1 += padding;
        if(interval.valid())
            AiVolumeAddIntersection(info, interval.t0, interval.t1);
    }
    if(debug){
        intersector.init(Convert(*origin), Convert(*direction), t0, t1, false);
        unsigned int i = 0;
        while((interval = intersector.next()).valid()){
            float t0diff = fabs(interval.t0 - intervals[i].t0);
            float t1diff = fabs(interval.t1 - intervals[i].t1);
            if(t0diff > 1e-4 || t1diff > 1e-4){
                DUMP(t0diff);
                DUMP(t1diff);
                DUMP(interval.t0);
                DUMP(interval.t1);
                DUMP(intervals[i].t0);
                DUMP(intervals[i].t1);
                DUMP(*origin);
                DUMP(*origin + t1*(*direction));
                DUMP(t0);
                DUMP(t1);
                assert(false);
            }
            ++i;
        }
    }
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
