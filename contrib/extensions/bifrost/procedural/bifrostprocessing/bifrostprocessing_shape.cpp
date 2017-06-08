#include <bifrostprocessing/bifrostprocessing_shape.h>
#include <bifrostapi/bifrost_all.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostprocessing/bifrostprocessing_tools.h>
#include <bifrostapi/bifrost_cacheresource.h>
#include <bifrostapi/bifrost_context.h>
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
    case SmoothMode::Laplacian: return out << "Laplacian";
    case SmoothMode::Curvature: return out << "Curvature";
    }
    return out;
}
std::ostream& operator <<(std::ostream& out, BlendType bt){
    switch(bt){
    case BlendType::Linear: return out << "Linear";
    case BlendType::Smooth: return out << "Smooth";
    case BlendType::Smoother: return out << "Smoother";
    }
    return out;
}

Status& Status::operator =(const Status& o){
    _error = o.error();
    _warnings = o._warnings.copy();
    return *this;
}

// TODO: fix logging
void Status::error(const Bifrost::API::String& format, ...) {
    _error = format;
}
void Status::warn(const Bifrost::API::String& format, ...) {
    _warnings.add(format);
}

Bifrost::API::String ShapeParameters::str() const{
    std::stringstream ss;
    ss << std::endl;
    DUMP_PARAM(cache_folder);
    DUMP_PARAM(activeGraph);
    DUMP_PARAM(resourceId);
    DUMP_PARAM(object);
    DUMP_PARAM(point_component);
    DUMP_PARAM(voxel_component);
    DUMP_PARAM(frame);
    DUMP_PARAM(object);
    DUMP_ARRAY_PARAM(channels);
    DUMP_PARAM(velocity_channel);
    DUMP_PARAM(uv_channel);

    DUMP_PARAM(velocity_scale);
    DUMP_PARAM(space_scale);
    DUMP_PARAM(fps);

    DUMP_PARAM(clip);
    DUMP_PARAM(clip_bbox);

    return ss.str().c_str();
}

namespace{

    Bifrost::API::Component findComponent(const Bifrost::API::Object& obj, const Bifrost::API::String& name, const Bifrost::API::TypeID& type, Bifrost::Processing::Status& status){
        Bifrost::API::Component component;
        if(name.empty()){
            Bifrost::API::RefArray refs = obj.findComponentsByType(type);
            if(refs.count() == 0){
                status.warn("Could not find components of type '%s'.", type.c_str());
            }else{
                component = refs[0];
                if(refs.count() > 1){
                    status.warn("Found multiple components of type '%s'. Using '%s'.", type.c_str(), component.name().c_str());
                }
            }
        }else{
            component = obj.findComponent(name);
            if(!component.valid()){
                status.warn("Could not find component named '%s'.", name.c_str());
            }
        }
        return component;
    }

}

Shape::Shape(const ShapeParameters& params){
    Bifrost::API::ObjectModel om;
    Bifrost::API::String cacheFolder = params.cache_folder;
    Bifrost::API::String objectName = params.object;
    Bifrost::API::Runtime::Frame frame(params.frame);
    Bifrost::API::Runtime::CacheResource::LoadOptions options;
    if(params.clip) options.bbox = params.clip_bbox;

    {// try to get the object from the active graph and save to temporary file
        Bifrost::API::Query query = om.createQuery(params.activeGraph, frame, params.resourceId);
        query.addFilter('&', "type = Object");
        query.addFilter('&', "name = "+params.object);
        Bifrost::API::RefArray objs = query.run();
        if(objs.count() > 0){
            Bifrost::API::Object obj = objs[0];
            this->tmp_folder = cacheFolder = Bifrost::API::File::createTempFolder();
            Bifrost::API::Runtime::ActiveGraph ag = om.activeGraph(query.binding().activeGraph);
            Bifrost::API::Runtime::CacheResource cache = ag.createCacheResource(frame, cacheFolder, params.object);
            Bifrost::API::Runtime::ResID resId = cache.resID();
            if(!cache.save(obj, Bifrost::API::BIF::Compression::Level0).succeeded()){
                _status.error("Failed to write to temporary cache folder.");
                return;
            }
            cache.reset();
            ag.removeResource(frame, resId);
        }
    }

    Bifrost::API::Runtime::ActiveGraph ag = om.activeGraph(cacheFolder);
    if(!ag.valid()) ag = om.createActiveGraph(cacheFolder);
    if(!ag.valid()){
        _status.error("Could not create active graph for path: '%s'", cacheFolder.c_str());
        return;
    }
    activeGraph = ag.name();
    Bifrost::API::Runtime::CacheResourceImporter importer = ag.createCacheResourceImporter(cacheFolder);
    if(!params.object.empty()){
        importer.readFiles(false, false, params.object);
        if(importer.objectNames().count() != 1){
            _status.error("Could not find object '%s' in path '%s'", params.object.c_str(), cacheFolder.c_str());
            return;
        }
    }else{
        importer.readFiles(false, false);
        if(importer.objectNames().count() == 0){
            _status.error("Could not find objects in path '%s'", cacheFolder.c_str());
            return;
        }
        objectName = importer.objectNames()[0];
        if(importer.objectNames().count() > 1){
            _status.warn("Found multiple objects in path '%s'. Using object '%n'.", cacheFolder.c_str(), objectName.c_str());
        }
    }
    Bifrost::API::Runtime::ResIDArray resIds;
    importer.resIDs(objectName, frame, resIds);
    if(resIds.count() != 1){
        _status.error("Could not figure what resource ID to use: %d found.", resIds.count());
        return;
    }
    Bifrost::API::Runtime::CacheResource resource = ag.resource(frame, Bifrost::API::Runtime::ResID(resIds[0]));
    if(!resource.load(options).succeeded()){
        _status.error("Failed to load frame '%d'.", frame.value);
        return;
    }
    Bifrost::API::StateServer stateServer = resource.dataSource();
    if(!stateServer.valid()){
        _status.error("Invalid state server.");
        return;
    }
    Bifrost::API::Object obj = stateServer.findObject(objectName);
    if(!obj.valid()){
        _status.error("Could not find object in state server '%s'", stateServer.name().c_str());
        return;
    }
    _points = findComponent(obj, params.point_component, Bifrost::API::PointComponentType, _status);
    _voxels = findComponent(obj, params.voxel_component, Bifrost::API::VoxelComponentType, _status);

    float vscale = params.space_scale * params.velocity_scale / params.fps;
    if(vscale != 1){
        Bifrost::API::String velocities[3] = { params.velocity_channel+"_u", params.velocity_channel+"_v", params.velocity_channel+"_w" };
        for(unsigned int i = 0; i < 3; ++i){
            Bifrost::API::Channel v = _voxels.findChannel(velocities[i]);
            if(v.valid()) ScaleFilter<float>(vscale).filter(v,v);
        }
        {
            Bifrost::API::Channel v = _voxels.findChannel(params.velocity_channel);
            if(v.valid()) ScaleFilter<amino::Math::vec3f>(amino::Math::vec3f(vscale)).filter(v,v);
        }
        Bifrost::API::Channel v = _points.findChannel(params.velocity_channel);
        if(v.valid()) ScaleFilter<amino::Math::vec3f>(amino::Math::vec3f(vscale)).filter(v,v);
    }
    scale(params.space_scale);
}

void Shape::scale(float space_scale){
    Bifrost::API::Layout layout = _voxels.layout().valid()? _voxels.layout() : _points.layout();
    layout.setVoxelScale(layout.voxelScale()*space_scale);
}

amino::Math::bboxf Shape::bbox() const{
    return computeBBox(voxels().layout());
}

Shape::~Shape(){
    Bifrost::API::ObjectModel om;
    if(om.activeGraph(activeGraph).valid()){
        om.removeActiveGraph(activeGraph);
    }
    if(!tmp_folder.empty()){
        Bifrost::API::File::deleteFolder(tmp_folder);
    }
}

}} // Bifrost::Processing
