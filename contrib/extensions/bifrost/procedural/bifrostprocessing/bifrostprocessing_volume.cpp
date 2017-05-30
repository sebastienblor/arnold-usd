#include <bifrostprocessing/bifrostprocessing_volume.h>
#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include "utils.h"
#include "defs.h"

namespace Bifrost{
namespace Processing{

Bifrost::API::String VolumeParameters::str() const{
    std::stringstream ss;
    ss << ShapeParameters::str().c_str();
    DUMP_PARAM(density_channel);
    DUMP_PARAM(smooth);
    DUMP_PARAM(smooth_iterations);
    return ss.str().c_str();
}

Volume::Volume(const VolumeParameters& params) : Shape(params), density_channel(params.density_channel){
    Bifrost::API::VoxelChannel density = voxels().findChannel(density_channel);
    if(!density.valid() || density.dataType() != Bifrost::API::DataType::FloatType){
        _status.error("Invalid distance channel '%s'. Available channels are:\n%s", density_channel.c_str(),
                   Bifrost::Private::availableChannels(_voxels, [](const Bifrost::API::Channel& c){ return c.dataType() == Bifrost::API::DataType::FloatType; }).c_str());
        return;
    }
    if(params.smooth > 0 && params.smooth_iterations > 0){
        Bifrost::Processing::SmoothFilter<float>(Bifrost::Processing::SmoothFilter<float>::Mode::kLaplacianFlow, params.smooth_iterations, params.smooth).filter(density, density);
    }
}

}} // Bifrost::Processing


