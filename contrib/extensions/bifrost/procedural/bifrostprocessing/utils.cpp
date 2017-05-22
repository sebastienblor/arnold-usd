#include "utils.h"

namespace Bifrost{
namespace Private{

SweepAccessor::SweepAccessor(const Bifrost::API::TileAccessor &accessor)
    : accessor(accessor){}

Bifrost::API::Tile SweepAccessor::left(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.i -= tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
Bifrost::API::Tile SweepAccessor::right(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.i += tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
Bifrost::API::Tile SweepAccessor::bottom(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.j -= tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
Bifrost::API::Tile SweepAccessor::top(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.j += tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
Bifrost::API::Tile SweepAccessor::closer(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.k -= tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
Bifrost::API::Tile SweepAccessor::further(const Bifrost::API::Tile& tile) const{
    Bifrost::API::TileCoord coord = tile.coord();
    coord.k += tile.info().dimInfo.tileWidth;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter){
    Bifrost::API::RefArray channels = component.channels();
    std::stringstream ss;
    for(unsigned int i = 0; i < channels.count(); ++i)
        if(filter(channels[i]))
            ss << "    " << Bifrost::API::Channel(channels[i]).name() << std::endl;
    return ss.str();
}

}} // Bifrost::Private
