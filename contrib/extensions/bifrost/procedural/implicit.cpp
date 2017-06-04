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
    //DUMP(params.str());
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
    _padding = -( layout.voxelScale() * layout.tileDimInfo().tileWidth );
    float shutter_start, shutter_end;
    _hasMotion = !params.ignore_motion_blur && getMotion(shutter_start, shutter_end) && (shutter_start != 0 || shutter_start != shutter_end);
    _accessor = layout.tileAccessor();
    invDx = 1./layout.voxelScale();
    maxDepth = layout.maxDepth();
}

int Implicit::depth(const AtVector &p){
    float ofs = .5;
    return _accessor.index((int)floor(p.x*invDx+ofs), (int)floor(p.y*invDx+ofs), (int)floor(p.z*invDx+ofs), maxDepth).depth;
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
    if(implicit->depth(sg->Po) != 7){
        //return false;
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
    //AiVolumeAddIntersection(info, t0, t1); return;
    Bifrost::Processing::Intersector intersector(implicit->intersector());
    const float padding = -1e-4;// implicit->padding()*.75;
    //t0 -= padding; t1 += padding;
    bool debug = true;
    bool fromCam = false;
    AtVector start, end;
    if(debug){
        AtVector tmp = (*origin - AtVector(-27.2573, 34.2993, -0.472827));
        fromCam = (tmp.x*tmp.x + tmp.y *tmp.y + tmp.z*tmp.z) < 1e-4;
    }
    intersector.init(Convert(*origin), Convert(*direction), t0, t1, false);
    std::vector<Bifrost::Processing::Interval> intervals;
    Bifrost::Processing::Interval interval;
    while((interval = intersector.next()).valid()){
        intervals.push_back(interval);
        interval.t0 -= padding;
        interval.t1 += padding;
        if(!interval.valid()){
            if(debug) std::cerr << "# skip: " << interval.t0 << ", " << interval.t1 << std::endl;
            continue;
        }
        if(debug){
            start = *origin + interval.t0 * (*direction);
            end   = *origin + interval.t1 * (*direction);
        }
        if(debug && (implicit->depth(start) != 7 || implicit->depth(end) != 7)){
            float radius = .1;
            interval.t0 += padding;
            interval.t1 -= padding;
            start = *origin + interval.t0 * (*direction);
            AtVector END = *origin + t1 * (*direction);
            std::stringstream ss; ss << std::endl;
            ss << "#" << ((implicit->depth(start) != 7)? " start" : "") << ((implicit->depth(end) != 7)? " end" : "") << std::endl;
            ss << "sphere {" << std::endl;
            ss << "    center " << start.x << " " << start.y << " " << start.z << std::endl;
            ss << "    radius " << radius << std::endl;
            ss << "    shader red\n}" << std::endl;
            if(true){
            ss << "sphere {" << std::endl;
            ss << "    center " << end.x << " " << end.y << " " << end.z << std::endl;
            ss << "    radius " << (radius*2) << std::endl;
            ss << "    shader green\n}" << std::endl;

            ss << "cylinder {" << std::endl;
            ss << "    bottom " << origin->x << " " << origin->y << " " << origin->z << " ";
            ss << "    top " << END.x << " " << END.y << " " << END.z << std::endl;
            ss << "    radius " << (radius*.1) << std::endl;
            ss << "    shader black\n}" << std::endl;
            }
            std::cerr << ss.str();
            interval.t0 -= padding;
            interval.t1 += padding;
        }
        AiVolumeAddIntersection(info, interval.t0, interval.t1);
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
