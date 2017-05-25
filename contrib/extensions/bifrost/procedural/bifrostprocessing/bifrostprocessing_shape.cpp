#include <bifrostprocessing/bifrostprocessing_shape.h>
#include <bifrostapi/bifrost_all.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostapi/bifrost_cacheresource.h>
#include <aminomath/vec.h>
#include "defs.h"
#include <stdio.h>
#include <stdarg.h>

namespace Bifrost {
namespace Processing {

std::ostream& operator <<(std::ostream& out, RenderComponent rc){
    return out << ((rc == RenderComponent::Volume)? "Volume" : "Particle");
}
std::ostream& operator <<(std::ostream& out, SmoothMode sm){
    switch(sm){
    case SmoothMode::Mean: return out << "Mean";
    case SmoothMode::Gaussian: return out << "Gaussian";
    case SmoothMode::Median: return out << "Median";
    case SmoothMode::Laplacian: return out << "Laplacian";
    case SmoothMode::Curvature: return out << "Curvature";
    default: return out;
    }
}
std::ostream& operator <<(std::ostream& out, BlendType bt){
    switch(bt){
    case BlendType::Linear: return out << "Linear";
    case BlendType::Smooth: return out << "Smooth";
    case BlendType::Smoother: return out << "Smoother";
    default: return out;
    }
}

Status& Status::operator =(const Status& o){
    _error = o.error();
    _warnings = o._warnings.copy();
    return *this;
}

void Status::error(const char* format, ...){
    // TODO: fix this
    char buffer[1000];
    va_list args;
    va_start(args, format);
    sprintf(buffer, format, args);
    va_end(args);
    _error = buffer;
}

void Status::warn(const char* format, ...){
    // TODO: fix this
    char buffer[1000];
    va_list args;
    va_start(args, format);
    sprintf(buffer, format, args);
    va_end(args);
    _warnings.add(buffer);
}

Bifrost::API::String ShapeParameters::str() const{
    std::stringstream ss;
    ss << std::endl;
    DUMP_PARAM(cache_file);
    DUMP_PARAM(object);
    DUMP_ARRAY_PARAM(channels);
    DUMP_ARRAY_PARAM(velocity_channels);
    DUMP_PARAM(uv_channel);

    DUMP_PARAM(velocity_scale);
    DUMP_PARAM(space_scale);
    DUMP_PARAM(fps);

    DUMP_PARAM(clip);
    DUMP_PARAM(clip_bbox);

    return ss.str().c_str();
}

Shape::Shape(const ShapeParameters& params){
    Bifrost::API::ObjectModel om;
    Bifrost::API::Object object;
    Bifrost::API::Component component;

    Bifrost::API::Query query = om.createQuery();
    Bifrost::API::RefArray objects;
    if(query.load(params.object.c_str())){
        // find in-memory object
        objects = query.run();
        if(objects.count() == 0){
            _status.warn("Failed to find object from descriptor '%s'. Using cache file '%s'.", params.object.c_str(), params.cache_file.c_str());
        }else if((objects = query.run()).count() != 1){
            _status.error("Can't find bif object from descriptor '%s' (%d objects exist).", params.object.c_str(), (int)objects.count());
            return;
        }else{ // count = 1
            object = objects[0];
            Bifrost::API::RefArray components;
            components = object.findComponentsByType(params.render_component == RenderComponent::Volume? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
            if(components.count()==0){
                _status.error("NOOO");
                return;
            }
            component = components[0];

            Bifrost::API::String tmp_folder = Bifrost::API::File::createTempFolder();
            tmp_folder = tmp_folder.c_str();
            Bifrost::API::String tmp_cache_file = tmp_folder.append(component.name()).append(".bif");
            Bifrost::API::FileIO fio = om.createFileIO(tmp_cache_file);
            if(!fio.save(component, Bifrost::API::BIF::Compression::Level0, 0).succeeded()){
                _status.error("Failed to write temporary bif file '%s'.", tmp_cache_file.c_str());
                return;
            }
        }
    }

    Bifrost::API::Status status;
    Bifrost::API::FileIO fio = om.createFileIO(Bifrost::API::File::forwardSlashes(params.cache_file.c_str()));
    Bifrost::API::StateServer ss = om.createStateServer();

    // get object from file
    status = params.clip? fio.load(ss, params.clip_bbox) : fio.load(ss);
    if(status != Bifrost::API::Status::Success){
        _status.error("Failed to load bif file '%s'.", params.cache_file.c_str());
        return;
    }
    objects = ss.objects();
    if(objects.count() != 1) {
        _status.error("Can't find bif object in file '%s' (%d objects exist).", params.cache_file.c_str(), (int)objects.count());
        return;
    }
    object = objects[0];
    Bifrost::API::RefArray components;
    components = object.findComponentsByType(params.render_component == RenderComponent::Volume? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
    if(components.count()==0){
        _status.error("wrong component type");
        return;
    }
    _component = components[0];
    if(components.count() > 1){
        _status.warn("too many components");
    }

    Bifrost::API::Layout layout(_component.layout());
    layout.setVoxelScale(layout.voxelScale()*params.space_scale);
    float vscale = params.velocity_scale / params.fps;
    if(vscale != 1){
        if(params.velocity_channels.count()==3){
            for(unsigned int i = 0; i < params.velocity_channels.count(); ++i){
                Bifrost::API::Channel v = _component.findChannel(params.velocity_channels[i]);
                ScaleFilter<float>(vscale).filter(v,v);
            }
        }else if(params.velocity_channels.count()==1){
            Bifrost::API::Channel v = _component.findChannel(params.velocity_channels[0]);
            ScaleFilter<amino::Math::vec3f>(amino::Math::vec3f(vscale)).filter(v,v);
        }
    }
}

amino::Math::bboxf Shape::bbox() const{
    return amino::Math::bboxf(amino::Math::vec3f(-5), amino::Math::vec3f(5));
}

Shape::~Shape(){
    if(!tmp_folder.empty()){
        Bifrost::API::File::deleteFolder(tmp_folder);
    }
}

}} // Bifrost::Processing
