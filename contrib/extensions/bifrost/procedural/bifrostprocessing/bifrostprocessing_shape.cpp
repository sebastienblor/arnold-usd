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

Shape::~Shape(){
    if(!tmp_folder.empty()){
        Bifrost::API::File::deleteFolder(tmp_folder);
    }
}

Status Shape::initialize(){
    Status retStatus;

    Bifrost::API::ObjectModel om;
    Bifrost::API::Object object;
    Bifrost::API::Component component;

    Bifrost::API::Query query = om.createQuery();
    Bifrost::API::RefArray objects;
    if(query.load(this->object.c_str())){
        // find in-memory object
        objects = query.run();
        if(objects.count() == 0){
            retStatus.warn("Failed to find object from descriptor '%s'. Using cache file '%s'.", this->object.c_str(), cache_file.c_str());
        }else if((objects = query.run()).count() != 1){
            retStatus.error("Can't find bif object from descriptor '%s' (%d objects exist).", this->object.c_str(), (int)objects.count());
            return retStatus;
        }else{ // count = 1
            object = objects[0];
            Bifrost::API::RefArray components;
            components = object.findComponentsByType(render_component == RenderComponent::Volume? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
            if(components.count()==0){
                retStatus.error("NOOO");
                return retStatus;
            }
            component = components[0];

            Bifrost::API::String tmp_folder = Bifrost::API::File::createTempFolder();
            tmp_folder = tmp_folder.c_str();
            Bifrost::API::String tmp_cache_file = tmp_folder.append(component.name()).append(".bif");
            cache_file = tmp_cache_file.c_str();
            Bifrost::API::FileIO fio = om.createFileIO(tmp_cache_file);
            if(!fio.save(component, Bifrost::API::BIF::Compression::Level0, 0).succeeded()){
                retStatus.error("Failed to write temporary bif file '%s'.", tmp_cache_file.c_str());
                return retStatus;
            }
        }
    }

    Bifrost::API::Status status;
    Bifrost::API::FileIO fio = om.createFileIO(Bifrost::API::File::forwardSlashes(cache_file.c_str()));
    Bifrost::API::StateServer ss = om.createStateServer();

    // get object from file
    status = clip? fio.load(ss, clip_bbox) : fio.load(ss);
    if(status != Bifrost::API::Status::Success){
        retStatus.error("Failed to load bif file '%s'.", cache_file.c_str());
        return retStatus;
    }
    objects = ss.objects();
    if(objects.count() != 1) {
        retStatus.error("Can't find bif object in file '%s' (%d objects exist).", cache_file.c_str(), (int)objects.count());
        return retStatus;
    }
    object = objects[0];
    Bifrost::API::RefArray components;
    components = object.findComponentsByType(render_component == Volume? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
    if(components.count()==0){
        retStatus.error("wrong component type");
        return retStatus;
    }
    _component = components[0];
    if(components.count() > 1){
        retStatus.warn("too many components");
    }

    Bifrost::API::Layout layout(_component.layout());
    layout.setVoxelScale(layout.voxelScale()*space_scale);
    float vscale = velocity_scale / fps;
    if(vscale != 1){
        if(velocity_channels.count()==3){
            for(unsigned int i = 0; i < velocity_channels.count(); ++i){
                Bifrost::API::Channel v = _component.findChannel(velocity_channels[i]);
                ScaleFilter<float>(vscale).filter(v,v);
            }
        }else if(velocity_channels.count()==1){
            Bifrost::API::Channel v = _component.findChannel(velocity_channels[0]);
            ScaleFilter<amino::Math::vec3f>(amino::Math::vec3f(vscale)).filter(v,v);
        }
    }
    return retStatus;
}

Bifrost::API::String Shape::str() const{
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

}} // Bifrost::Processing
