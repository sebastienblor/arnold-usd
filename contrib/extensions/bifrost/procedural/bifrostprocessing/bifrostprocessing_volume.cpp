#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "defs.h"

namespace Bifrost{
namespace Processing{

Status Volume::initialize(){
    return Shape::initialize();
}

Bifrost::API::String Volume::str() const{
    std::stringstream ss;
    ss << Shape::str().c_str();
    DUMP_PARAM(density_channel);
    return ss.str().c_str();
}

}} // Bifrost::Processing
