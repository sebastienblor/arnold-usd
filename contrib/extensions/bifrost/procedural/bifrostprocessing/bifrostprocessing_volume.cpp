#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "defs.h"

namespace Bifrost{
namespace Processing{

Bifrost::API::String VolumeParameters::str() const{
    std::stringstream ss;
    ss << ShapeParameters::str().c_str();
    DUMP_PARAM(density_channel);
    return ss.str().c_str();
}

Volume::Volume(const VolumeParameters& params) : Shape(params){

}

}} // Bifrost::Processing


