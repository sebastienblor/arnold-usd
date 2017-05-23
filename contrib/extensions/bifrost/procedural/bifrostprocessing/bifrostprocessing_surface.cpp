#include <bifrostprocessing/bifrostprocessing_surface.h>
#include <bifrostapi/bifrost_all.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostapi/bifrost_cacheresource.h>
#include <aminomath/vec.h>
#include "utils.h"
#include "defs.h"

namespace Bifrost {
namespace Processing{

namespace{

Status convertPointsToVoxels(Surface& surface, const Bifrost::API::PointComponent& pointComponent, Bifrost::API::VoxelComponent& component, Status& status){
    Bifrost::API::Object object(pointComponent.object());
    Bifrost::API::Dictionary dict = object.dictionary();

    Bifrost::API::LevelSetOptions levelSetOptions;
    levelSetOptions.dropletRevealFactor = surface.levelset_droplet_reveal_factor;
    levelSetOptions.surfaceRadius = surface.levelset_surface_radius;
    levelSetOptions.dropletRadius = surface.levelset_droplet_radius;
    levelSetOptions.resolutionFactor = surface.levelset_resolution_factor;
    levelSetOptions.maxVolumeOfHolesToClose = surface.levelset_max_volume_of_holes_to_close;
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
    surface.distance_channel = "distance";
    surface.velocity_channels.clear();
    surface.velocity_channels.add("velocity");

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
            status.error("Failed to create voxel channel '%s'.", pointChannel.name().c_str());
            return status;
        }
    }
    if(!Bifrost::API::createLevelSet( levelSetOptions, pointComponent,                 /* component holding the particles from which the level set is generated */
                                                       particleChannels,               /* must exist on point: particles, droplet, etc... */
                                                       Bifrost::API::VoxelComponent(), /* optional, component holding the solid distance */
                                                       Bifrost::API::String(),         /* optional, must exist on voxel solid */
                                                       component,                      /* component holding the output voxel channels */
                                                       voxelChannels                   /* must exist on outputVoxel: distance, etc... */
                                                       ).succeeded()){
        status.error("createLevelSet call failed...");
        return status;
    }
    return status;
}

}

Status Surface::initialize(){
    Status status;
    if(!(status = Shape::initialize())){
        return status;
    }
    if(_component.type() == Bifrost::API::PointComponentType){
        Bifrost::API::VoxelComponent component;
        if(!convertPointsToVoxels(*this, _component, component, status))
            return status;
        _component = component;
    }

    if(channels.count() == 1 && channels[0] == "all"){
        channels.clear();
        Bifrost::API::RefArray channels = _component.channels();
        for(unsigned int i = 0; i < channels.count(); ++i){
            this->channels.addUnique(Bifrost::API::Channel(channels[i]).name());
        }
    }

    Bifrost::API::VoxelChannel distance = _component.findChannel(distance_channel.c_str());
    if(!distance.valid() || distance.dataType() != Bifrost::API::DataType::FloatType){
        status.error("Invalid distance channel \"%s\". Available channels are:\n%s", distance_channel.c_str(),
                   Bifrost::Private::availableChannels(_component, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        return status;
    }

    if(dilate != 0)
        Bifrost::Processing::DilateFilter<float>(dilate).filter(distance, distance);
    if(smooth > 0 && smooth_iterations > 0)
        Bifrost::Processing::SmoothFilter<float>((Bifrost::Processing::SmoothFilter<float>::Mode) smooth_mode, smooth_iterations, smooth).filter(distance, distance);
    if(erode != 0)
        Bifrost::Processing::ErodeFilter<float>(erode).filter(distance, distance);

    if(enable_ocean_blending){
        Bifrost::Processing::ExtendFilter extend(ocean_blending_height, ocean_blending_center, ocean_blending_dimension, ocean_blending_radius);
        extend.filter(distance,distance);
        if(enable_ocean_blending_uvs){
            Bifrost::API::VoxelChannel uvs = _component.findChannel(uv_channel.c_str());
            if(uvs.valid()){
                extend.uvs(uvs);
            }else{
                status.warn("Invalid uv channel \"%s\". Available channels are:\n%s", uv_channel.c_str(),
                            Bifrost::Private::availableChannels(_component, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
            }
        }
    }
    return status;
}

void Surface::mesh(Bifrost::API::Array<amino::Math::vec3f> &vertices, Bifrost::API::Array<amino::Math::vec3i> &indices, unsigned int subdivisions) const{
    Bifrost::API::VoxelChannel distance = _component.findChannel(distance_channel);
    MarchingCubes().mesh(distance, vertices, indices, subdivisions);
}

Bifrost::API::String Surface::str() const{
    std::stringstream ss;
    ss << Shape::str();
    DUMP_PARAM(render_component);

    DUMP_PARAM(distance_channel);

    DUMP_PARAM(levelset_droplet_reveal_factor);
    DUMP_PARAM(levelset_surface_radius);
    DUMP_PARAM(levelset_droplet_radius);
    DUMP_PARAM(levelset_resolution_factor);
    DUMP_PARAM(levelset_max_volume_of_holes_to_close);

    DUMP_PARAM(dilate);
    DUMP_PARAM(erode);
    DUMP_PARAM(smooth);
    DUMP_PARAM(smooth_mode);
    DUMP_PARAM(smooth_iterations);

    DUMP_PARAM(enable_ocean_blending);
    DUMP_PARAM(ocean_blending_height);
    DUMP_PARAM(ocean_blending_center);
    DUMP_PARAM(ocean_blending_dimension);
    DUMP_PARAM(ocean_blending_radius);
    DUMP_PARAM(ocean_blending_blend);
    DUMP_PARAM(enable_ocean_blending_uvs);

    return ss.str().c_str();
}

}} // Bifrost::Processing
