#include <bifrostprocessing/bifrostprocessing_points.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tile.h>
#include <sstream>
#include "defs.h"

namespace Bifrost{
namespace Processing{

Bifrost::API::String PointsParameters::str() const{
    std::stringstream ss;
    ss << ShapeParameters::str().c_str();

    DUMP_PARAM(radius);
    DUMP_PARAM(enable_radius_channel);
    DUMP_PARAM(radius_channel);

    DUMP_PARAM(enable_camera_radius);
    DUMP_PARAM(camera_distance);
    DUMP_PARAM(camera_factor);
    DUMP_PARAM(camera_factor_exponent);

    DUMP_PARAM(enable_multi_pointing);
    DUMP_PARAM(mp_samples);
    DUMP_PARAM(mp_radius);
    DUMP_PARAM(mp_surface_attract);
    DUMP_PARAM(mp_falloff_type);
    DUMP_PARAM(mp_falloff_range);
    DUMP_PARAM(mp_displacement);
    DUMP_PARAM(mp_displacement_noise_frequency);

    DUMP_PARAM(skip);

    return ss.str().c_str();
}

Points::Points(const PointsParameters& params) : Shape(params) {}

bool Points::positions(Bifrost::API::Array<amino::Math::vec3f>& positions) const{
    if(!_points.valid()) return false;
    Bifrost::API::Layout layout(_points.layout());
    positions.resize(_points.elementCount());
    float dx = layout.voxelScale();
    Bifrost::API::TileIterator it = layout.tileIterator(layout.maxDepth(), layout.maxDepth(), Bifrost::API::TraversalMode::BreadthFirst);
    // TODO: expose position channel
    Bifrost::API::PointChannel channel = _points.findChannel("position");
    Bifrost::API::TileData<amino::Math::vec3f> data;
    unsigned int i = 0;
    for(;it;++it){
        data = channel.tileData<amino::Math::vec3f>((*it).index());
        for(unsigned int e = 0; e < data.count(); ++e){
            positions[i++] = (data[e]*dx);
        }
    }
    return true;
}

}} // Bifrost::Processing

