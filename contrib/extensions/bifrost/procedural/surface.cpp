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

namespace Surface{

bool initialize(SurfaceParams& params, Bifrost::API::VoxelComponent& component){

    Bifrost::API::Status status;
    Bifrost::API::ObjectModel om;
    Bifrost::API::FileIO fio = om.createFileIO(Bifrost::API::File::forwardSlashes(params.cache_file.c_str()));
    Bifrost::API::StateServer ss = om.createStateServer();
    Bifrost::API::Object object;

    status = params.clip? fio.load(ss, params.clip_bbox) : fio.load(ss);
    if(status != Bifrost::API::Status::Success){
        AiMsgError("[BIFROST] Failed to load bif file '%s'.", params.cache_file.c_str());
        return false;
    }

    Bifrost::API::RefArray objects = ss.objects();
    if(objects.count() != 1) {
        AiMsgError("[BIFROST] Can't find bif object in file '%s' (%d objects exist).", params.cache_file.c_str(), objects.count());
        return false;
    }
    object = objects[0];

    Bifrost::API::String componentName = Bifrost::API::Component(object.components()[0]).name();

    if(params.render_component == PARTICLES){
        Bifrost::API::PointComponent tmp = object.findComponent(componentName);
        if(!tmp.valid()){
            AiMsgError("[BIFROST] Invalid point component '%s'.", componentName.c_str());
            return false;
        }
        AiMsgError("[BIFROST] Particle rendering not implemented yet.");
        return false;
    }else{
        component =  object.findComponent(componentName);
        if(!component.valid()){
            AiMsgError("[BIFROST] Invalid voxel component '%s'.", componentName.c_str());
            return false;
        }
    }

    if(params.channels.size() == 1 && params.channels[0] == "all"){
        params.channels.clear();
        Bifrost::API::RefArray channels = component.channels();
        for(unsigned int i = 0; i < channels.count(); ++i)
            params.channels.push_back(Bifrost::API::Channel(channels[i]).name().c_str());
    }

    Bifrost::API::VoxelChannel distance = component.findChannel(params.distance_channel.c_str());
    if(!distance.valid() || distance.dataType() != Bifrost::API::DataType::FloatType){
        AiMsgError("[BIFROST] Invalid distance channel \"%s\". Available channels are:\n%s", params.distance_channel.c_str(),
                   availableChannels(component, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        return false;
    }

    if(params.dilate != 0)
        Bifrost::Processing::DilateFilter<float>(params.dilate).filter(distance, distance);
    if(params.smooth > 0 && params.smooth_iterations > 0){
        Bifrost::Processing::SmoothFilter<float>((Bifrost::Processing::SmoothFilter<float>::Mode) params.smooth_mode, params.smooth_iterations, params.smooth).filter(distance, distance);
        // TODO: smooth blending
    }
    if(params.erode != 0)
        Bifrost::Processing::ErodeFilter<float>(params.erode).filter(distance, distance);

    if(params.enable_infinite_blending){
        Bifrost::Processing::extend(distance, params.infinite_blending_height, params.infinite_blending_center, params.infinite_blending_dimension, params.infinite_blending_radius, distance);
    }

    std::cerr << ("EXTEND DONE") << std::endl;

    { // debug
        Bifrost::API::Runtime::ActiveGraph ag = om.createActiveGraph( "myAG" );
        Bifrost::API::Runtime::CacheResource resource = ag.createCacheResource(35, "/home/beauchc/bifrost_debug/bifrostLiquidContainer1/voxel_liquid", Bifrost::API::Object(component.object()).name() );
        DUMP(resource.save(component.object(), Bifrost::API::BIF::Compression::Level0).succeeded());
        om.removeActiveGraph(ag.name());
    }

    return true;
}

}
