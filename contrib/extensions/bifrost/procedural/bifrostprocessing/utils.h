#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_tileaccessor.h>

namespace Bifrost{
namespace Private{

class SweepAccessor{
public:
    SweepAccessor(const Bifrost::API::TileAccessor& accessor);
    Bifrost::API::Tile    left(const Bifrost::API::Tile& tile) const;
    Bifrost::API::Tile   right(const Bifrost::API::Tile& tile) const;
    Bifrost::API::Tile  bottom(const Bifrost::API::Tile& tile) const;
    Bifrost::API::Tile     top(const Bifrost::API::Tile& tile) const;
    Bifrost::API::Tile  closer(const Bifrost::API::Tile& tile) const;
    Bifrost::API::Tile further(const Bifrost::API::Tile& tile) const;
private:
    Bifrost::API::TileAccessor accessor;
};

}} // namespace Bifrost::Private

namespace{

std::ostream& operator<<(std::ostream& stream, const Bifrost::API::TileCoord& coord){
    return stream << "Coord[" << coord.i << ", " << coord.j << ", " << coord.k << "]";
}

}
