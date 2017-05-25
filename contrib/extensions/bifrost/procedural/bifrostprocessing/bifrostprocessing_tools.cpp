#include <bifrostprocessing/bifrostprocessing_tools.h>
#include <bifrostapi/bifrost_visitor.h>

namespace{

class BBoxVisitor : public Bifrost::API::Visitor{
public:
    BBoxVisitor() : min(amino::Math::vec3i(INT_MAX)), max(amino::Math::vec3i(INT_MIN)) {}
    //BBoxVisitor(const BBoxVisitor& o) : min(o.min), max(o.max) {}
    Bifrost::API::Visitor* copy() const override{ return new BBoxVisitor(); }

    void beginTile(const Bifrost::API::TileAccessor &accessor, const Bifrost::API::TreeIndex &index) override{
        const Bifrost::API::Tile& tile  = accessor.tile(index);
        const Bifrost::API::TileCoord& coord = tile.coord();
        amino::Math::vec3i nmin(coord.i, coord.j, coord.k);
        update(nmin, amino::Math::vec3i(nmin + amino::Math::vec3i(tile.info().dimInfo.tileWidth)));

    }
    void join(const Visitor &visitor) override{
        const BBoxVisitor& o = static_cast<const BBoxVisitor&>(visitor);
        update(o.min, o.max);
    }
    inline void update(const amino::Math::vec3i& nmin, const amino::Math::vec3i& nmax){
        for(unsigned int i = 0; i < 3; ++i){
            if(nmin[i] < min[i]) min[i] = nmin[i];
            if(nmax[i] > max[i]) max[i] = nmax[i];
        }
    }

    amino::Math::bboxi bbox() const{
        return amino::Math::bboxi(min,max);
    }
private:
    amino::Math::vec3i min, max;
};

}

namespace Bifrost{
namespace Processing{

amino::Math::bboxf computeBBox(const Bifrost::API::Layout& layout){
    BBoxVisitor visitor;
    Bifrost::API::Layout(layout).traverse(visitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirstBottomUp, layout.maxDepth(), layout.maxDepth());
    amino::Math::bboxi bbox = visitor.bbox();
    float dx = layout.voxelScale();
    return amino::Math::bboxf( amino::Math::vec3f(bbox.min()[0]*dx, bbox.min()[1]*dx, bbox.min()[2]*dx),
                               amino::Math::vec3f(bbox.max()[0]*dx, bbox.max()[1]*dx, bbox.max()[2]*dx) );
}

}} // Bifrost::Processing
