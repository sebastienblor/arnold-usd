#include "implicit.h"
#include <bifrostprocessing/bifrostprocessing_intersector.h>
#include <ai.h>
#include "debug.h"
#include "utils.h"
#include <bifrostapi/bifrost_layout.h>

//#define DEBUG_EXTENTS_FILE "/home/beauchc/dev/mtoa/data/spheres.ass"
#ifdef DEBUG_EXTENTS_FILE
namespace{
void init_output();
void output(const AtVector &origin, const AtVector &direction, float t0, float t1, const Bifrost::Processing::Interval &interval, bool ray=true);
}
#endif

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
    _padding = -( layout.voxelScale() * layout.tileDimInfo().tileWidth );
    float shutter_start, shutter_end;
    _hasMotion = !params.ignore_motion_blur && getMotion(shutter_start, shutter_end) && (shutter_start != 0 || shutter_start != shutter_end);
    _accessor = layout.tileAccessor();
    invDx = 1./layout.voxelScale();
    maxDepth = layout.maxDepth();
#ifdef DEBUG_EXTENTS_FILE
    init_output();
#endif
}

int Implicit::depth(const AtVector &p){
    return _accessor.index((int)floor(p.x*invDx), (int)floor(p.y*invDx), (int)floor(p.z*invDx), maxDepth).depth;
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
    //data->bbox = pad(Convert(implicit->surface().bbox()), implicit->padding());
    data->bbox = Convert(implicit->surface().bbox());
    return true;
}

volume_sample
{
    if(!data->private_info) return false;
    Implicit* implicit = static_cast<Implicit*>(data->private_info);
    //if(implicit->depth(sg->Po) != 7){ return false; }
    return implicit->sampler().sample(channel, sg->Po, sg->tid, value, type, interp, implicit->hasMotion()? sg->time : 0);
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
    intersector.init(Convert(*origin), Convert(*direction), t0, t1, false);
    Bifrost::Processing::Interval interval;
    while((interval = intersector.next()).valid()){
        AiVolumeAddIntersection(info, interval.t0, interval.t1);
#ifdef DEBUG_EXTENTS_FILE
        output(*origin, *direction, t0, t1, interval, true);
#endif
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
    return false;
}


#ifdef DEBUG_EXTENTS_FILE
#include <fstream>
namespace{

void init_output(){
    std::ofstream out(DEBUG_EXTENTS_FILE);
    out << "standard_surface { name red base_color 1 0 0 }\n";
    out << "standard_surface { name orange base_color 1 .5 0 }\n";
    out << "standard_surface { name green base_color 0 1 0 }\n";
    out << "standard_surface { name turquoise base_color 0 1 .5 }\n";
    out << "standard_surface { name black base_color .1 .1 .1 }\n";
    out.close();
}

void output(const AtVector& origin, const AtVector& direction, float t0, float t1, const Bifrost::Processing::Interval& interval, bool ray){
    AtVector start = origin + interval.t0 * direction;
    AtVector end = origin + interval.t1 * direction;
    float radius = .1;
    std::ofstream out(DEBUG_EXTENTS_FILE, std::ofstream::out | std::ofstream::app);
    out << "sphere {" << std::endl;
    out << "    center " << start.x << " " << start.y << " " << start.z << std::endl;
    out << "    radius " << radius << std::endl;
    out << "    shader red\n}" << std::endl;
    out << "sphere {" << std::endl;
    out << "    center " << end.x << " " << end.y << " " << end.z << std::endl;
    out << "    radius " << radius << std::endl;
    out << "    shader green\n}" << std::endl;
    if(ray){
        out << "cylinder {" << std::endl;
        out << "    bottom " << start.x << " " << start.y << " " << start.z << " ";
        out << "    top " << end.x << " " << end.y << " " << end.z << std::endl;
        out << "    radius " << (radius*.1) << std::endl;
        out << "    shader black\n}" << std::endl;
    }
    out.close();
}
}
#endif
