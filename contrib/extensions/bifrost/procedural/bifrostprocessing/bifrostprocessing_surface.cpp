#include <bifrostprocessing/bifrostprocessing_surface.h>
#include <bifrostapi/bifrost_all.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostprocessing/bifrostprocessing_tools.h>
#include <bifrostapi/bifrost_cacheresource.h>
#include <aminomath/vec.h>
#include "utils.h"
#include "defs.h"

namespace Bifrost {
namespace Processing{

Bifrost::API::String SurfaceParameters::str() const{
    std::stringstream ss;
    ss << ShapeParameters::str();
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

namespace{

Status convertPointsToVoxels(const SurfaceParameters& params, const Bifrost::API::PointComponent& pointComponent, Bifrost::API::VoxelComponent& component, Status& status){
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

Surface::Surface(const SurfaceParameters& params) : Shape(params){
    if(!status()) return;
    scale(1./params.space_scale);

    distance_channel = params.distance_channel;
    if(params.render_component == RenderComponent::Particle){
        // TODO: pass _voxels directly
        Bifrost::API::VoxelComponent component;
        if(!convertPointsToVoxels(params, _points, component, _status))
            return;
        _voxels = component;
        distance_channel = "distance";
    }
    if(!voxels().valid()){
        _status.error("Invalid voxel component.");
        return;
    }

    Bifrost::API::VoxelChannel distance = voxels().findChannel(distance_channel);
    if(!distance.valid() || distance.dataType() != Bifrost::API::DataType::FloatType){
        _status.error("Invalid distance channel '%s'. Available channels are:\n%s", distance_channel.c_str(),
                   Bifrost::Private::availableChannels(_voxels, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        return;
    }

    if(params.dilate != 0)
        Bifrost::Processing::DilateFilter<float>(params.dilate).filter(distance, distance);
    if(params.smooth > 0 && params.smooth_iterations > 0)
        Bifrost::Processing::SmoothFilter<float>((Bifrost::Processing::SmoothFilter<float>::Mode) params.smooth_mode, params.smooth_iterations, params.smooth).filter(distance, distance);
    if(params.erode != 0)
        Bifrost::Processing::ErodeFilter<float>(params.erode).filter(distance, distance);

    if(params.enable_ocean_blending){
        Bifrost::Processing::ExtendFilter extend(params.ocean_blending_height, params.ocean_blending_center, params.ocean_blending_dimension, params.ocean_blending_radius);
        extend.filter(distance,distance);
        if(params.enable_ocean_blending_uvs){
            Bifrost::API::VoxelChannel uvs = _voxels.findChannel(params.uv_channel.c_str());
            if(uvs.valid()){
                extend.uvs(uvs);
            }else{
                _status.warn("Invalid uv channel '%s'. Available channels are:\n%s", params.uv_channel.c_str(),
                            Bifrost::Private::availableChannels(_voxels, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
            }
        }
    }

    if(params.export_laplacian){
        Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(_voxels.stateID()));
        Bifrost::API::VoxelChannel laplacian = ss.createChannel(_voxels, Bifrost::API::DataType::FloatType, "laplacian");
        laplacian.setBackgroundValue<float>(0);
        LaplacianFilter<float>().filter(distance, laplacian);
    }
    if(params.export_curvature){
        Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(_voxels.stateID()));
        Bifrost::API::VoxelChannel curvature = ss.createChannel(_voxels, Bifrost::API::DataType::FloatType, "curvature");
        curvature.setBackgroundValue<float>(0);
        CurvatureFilter<float>().filter(distance, curvature);
    }
    //optimize(voxels().layout());
    //Bifrost::API::VoxelChannel(_voxels.findChannel("distance")).setBackgroundValue<float>(.2);

    if(params.space_scale != 1){
        scale(params.space_scale);
        ScaleFilter<float>(params.space_scale).filter(distance,distance);
    }

    if(false){
        DUMP("SAVING");
        Bifrost::API::ObjectModel om;
        Bifrost::API::Runtime::ActiveGraph ag = om.createActiveGraph( "myAG" );
        Bifrost::API::Runtime::CacheResource resource = ag.createCacheResource(35, "/home/beauchc/bifrost_debug/bifrostLiquidContainer1/voxel_liquid", Bifrost::API::Object(_voxels.object()).name() );
        DUMP(resource.save(_voxels.object(), Bifrost::API::BIF::Compression::Level0).succeeded());
        om.removeActiveGraph(ag.name());
    }
}

void Surface::mesh(Bifrost::API::Array<amino::Math::vec3f> &vertices, Bifrost::API::Array<amino::Math::vec3i> &indices, unsigned int subdivisions) const{
    Bifrost::API::VoxelChannel distance = _voxels.findChannel(distance_channel);
    MarchingCubes().mesh(distance, vertices, indices, subdivisions);
}

}} // Bifrost::Processing
