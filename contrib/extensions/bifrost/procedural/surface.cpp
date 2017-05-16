#include "surface.h"
#include <bifrostapi/bifrost_all.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_meshing.h>
#include <bifrostprocessing/bifrostprocessing_extend.h>
#include <bifrostapi/bifrost_cacheresource.h>
#include <aminomath/vec.h>
#include "utils.h"
#include "debug.h"

namespace{

bool getComponent(Surface::SurfaceParams& params, Bifrost::API::Component& component){
    Bifrost::API::ObjectModel om;
    Bifrost::API::Object object;

    Bifrost::API::Query query = om.createQuery();
    Bifrost::API::RefArray objects;
    if(query.load(params.object.c_str())){
        // find in-memory object
        objects = query.run();
        if(objects.count() == 0){
            AiMsgWarning("[BIFROST] Failed to find object from descriptor '%s'. Using cache file '%s'.", params.object.c_str(), params.cache_file.c_str());
        }else if((objects = query.run()).count() != 1){
            AiMsgError("[BIFROST] Can't find bif object from descriptor '%s' (%d objects exist).", params.object.c_str(), (int)objects.count());
            return false;
        }else{ // count = 1
            object = objects[0];
            Bifrost::API::RefArray components;
            components = object.findComponentsByType(params.render_component == Surface::VOXELS? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
            if(components.count()==0){
                AiMsgError("NOOO");
                return false;
            }
            component = components[0];

            Bifrost::API::String tmp_cache_file = Bifrost::API::File::createTempFolder().append(component.name()).append(".bif");
            Bifrost::API::FileIO fio = om.createFileIO(tmp_cache_file);
            DUMP(fio.save(component, Bifrost::API::BIF::Compression::Level0, 0).succeeded());
            params.cache_file = tmp_cache_file.c_str();
        }
    }

    Bifrost::API::Status status;
    Bifrost::API::FileIO fio = om.createFileIO(Bifrost::API::File::forwardSlashes(params.cache_file.c_str()));
    Bifrost::API::StateServer ss = om.createStateServer();

    // get object from file
    status = params.clip? fio.load(ss, params.clip_bbox) : fio.load(ss);
    if(status != Bifrost::API::Status::Success){
        AiMsgError("[BIFROST] Failed to load bif file '%s'.", params.cache_file.c_str());
        return false;
    }
    objects = ss.objects();
    if(objects.count() != 1) {
        AiMsgError("[BIFROST] Can't find bif object in file '%s' (%d objects exist).", params.cache_file.c_str(), (int)objects.count());
        return false;
    }
    object = objects[0];
    Bifrost::API::RefArray components;
    components = object.findComponentsByType(params.render_component == Surface::VOXELS? Bifrost::API::VoxelComponentType : Bifrost::API::PointComponentType);
    if(components.count()==0){
        AiMsgError("wrong component type");
        return false;
    }
    component = components[0];
    if(components.count() > 1){
        AiMsgWarning("too many components");
    }
    return true;
}

bool convertPointsToVoxels(Surface::SurfaceParams& params, const Bifrost::API::PointComponent& pointComponent, Bifrost::API::VoxelComponent& component){
    Bifrost::API::Object object(pointComponent.object());
    Bifrost::API::Dictionary dict = object.dictionary();

    Bifrost::API::LevelSetOptions levelSetOptions;
    levelSetOptions.dropletRevealFactor = params.levelset_droplet_reveal_factor;
    levelSetOptions.surfaceRadius = params.levelset_surface_radius;
    levelSetOptions.dropletRadius = params.levelset_droplet_radius;
    levelSetOptions.resolutionFactor = params.levelset_resolution_factor;
    levelSetOptions.maxVolumeOfHolesToClose = params.levelset_max_volume_of_holes_to_close;
    levelSetOptions.isAdaptive = dict.hasValue("enableSpatialAdapt")? dict.value<bool>("enableSpatialAdapt") : false;
    levelSetOptions.dropletThreshold = dict.hasValue("dropletThreshold")? dict.value<float>("dropletThreshold") : 1;
    levelSetOptions.doMorphologicalDilation = false;
    levelSetOptions.doErodeSheetsAndDroplets = false;

    Bifrost::API::StateServer ss = Bifrost::API::ObjectModel().stateServer(pointComponent.stateID());
    component = ss.createComponent(Bifrost::API::VoxelComponentType, (object.name()+"-volume"), Bifrost::API::Layout(pointComponent.layout()).voxelScale());
    Bifrost::API::StringArray particleChannels, voxelChannels;
    particleChannels.add(Bifrost::API::Channel(pointComponent.findChannel("position")).partialPathName());
    particleChannels.add(Bifrost::API::Channel(pointComponent.findChannel("droplet")).partialPathName());
    voxelChannels.add(Bifrost::API::Channel(ss.createChannel(component,Bifrost::API::DataType::FloatType,"distance")).partialPathName());
    params.distance_channel = "distance";
    params.velocity_channels = { "velocity" };

    Bifrost::API::RefArray allChannels = pointComponent.channels();
    for(unsigned int i = 0; i < allChannels.count(); ++i){
        Bifrost::API::Channel pointChannel(allChannels[i]);
        Bifrost::API::String name = pointChannel.name();
        if(name == "position" || name == "droplet") continue;
        if(pointChannel.dataType() != Bifrost::API::DataType::FloatType && pointChannel.dataType() != Bifrost::API::DataType::FloatV2Type && pointChannel.dataType() != Bifrost::API::DataType::FloatV3Type){
            continue;
        }
        Bifrost::API::VoxelChannel voxelChannel = ss.createChannel(component,pointChannel.dataType(),pointChannel.name());
        if(voxelChannel.valid()){
            particleChannels.add(pointChannel.partialPathName());
            voxelChannels.add(voxelChannel.partialPathName());
        }else{
            AiMsgWarning("[BIFROST] Failed to create voxel channel '%s'.", pointChannel.name().c_str());
        }
    }
    if(!Bifrost::API::createLevelSet( levelSetOptions, pointComponent,                 /* component holding the particles from which the level set is generated */
                                                       particleChannels,               /* must exist on point: particles, droplet, etc... */
                                                       Bifrost::API::VoxelComponent(), /* optional, component holding the solid distance */
                                                       Bifrost::API::String(),         /* optional, must exist on voxel solid */
                                                       component,                      /* component holding the output voxel channels */
                                                       voxelChannels                   /* must exist on outputVoxel: distance, etc... */
                                                       ).succeeded()){
        AiMsgError("[BIFROST] createLevelSet call failed...");
        return false;
    }
    return true;
}

}

namespace Surface{

bool initialize(SurfaceParams& params, Bifrost::API::VoxelComponent& component){
    Bifrost::API::Component tmp;
    if(!getComponent(params,tmp)){
        return false;
    }

    if(tmp.type() == Bifrost::API::PointComponentType){
        if(!convertPointsToVoxels(params, tmp, component))
            return false;
    }else{
        component = tmp;
    }

    if(params.channels.size() == 1 && params.channels[0] == "all"){
        params.channels.clear();
        Bifrost::API::RefArray channels = component.channels();
        for(unsigned int i = 0; i < channels.count(); ++i){
            params.channels.push_back(Bifrost::API::Channel(channels[i]).name().c_str());
        }
    }

    Bifrost::API::VoxelChannel distance = component.findChannel(params.distance_channel.c_str());
    if(!distance.valid() || distance.dataType() != Bifrost::API::DataType::FloatType){
        AiMsgError("[BIFROST] Invalid distance channel \"%s\". Available channels are:\n%s", params.distance_channel.c_str(),
                   availableChannels(component, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        return false;
    }

    if(params.dilate != 0)
        Bifrost::Processing::DilateFilter<float>(params.dilate).filter(distance, distance);
    if(params.smooth > 0 && params.smooth_iterations > 0)
        Bifrost::Processing::SmoothFilter<float>((Bifrost::Processing::SmoothFilter<float>::Mode) params.smooth_mode, params.smooth_iterations, params.smooth).filter(distance, distance);
    if(params.erode != 0)
        Bifrost::Processing::ErodeFilter<float>(params.erode).filter(distance, distance);

    if(params.enable_infinite_blending){
        Bifrost::Processing::extend(distance, params.infinite_blending_height, params.infinite_blending_center, params.infinite_blending_dimension, params.infinite_blending_radius, distance);
        Bifrost::API::VoxelChannel uvs = component.findChannel(params.uv_channel.c_str());
        if(uvs.valid()){
            Bifrost::Processing::extendUVs(params.infinite_blending_center, params.infinite_blending_dimension, uvs);
        }else{
            AiMsgWarning("[BIFROST] Invalid uv channel \"%s\". Available channels are:\n%s", params.uv_channel.c_str(),
                       availableChannels(component, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        }
    }

    if(params.debug > 0){ // debug
        DUMP("SAVING");
        Bifrost::API::ObjectModel om;
        Bifrost::API::Runtime::ActiveGraph ag = om.createActiveGraph( "myAG" );
        Bifrost::API::Runtime::CacheResource resource = ag.createCacheResource(35, "/home/beauchc/bifrost_debug/bifrostLiquidContainer1/voxel_liquid", Bifrost::API::Object(component.object()).name() );
        DUMP(resource.save(component.object(), Bifrost::API::BIF::Compression::Level0).succeeded());
        om.removeActiveGraph(ag.name());
    }

    return true;
}

}
