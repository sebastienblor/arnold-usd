#include <bifrostprocessing/bifrostprocessing_extend.h>
#include <bifrostprocessing/bifrostprocessing_visitors.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_layout.h>
#include <limits>
#include "defs.h"
#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_component.h>

namespace{

class ExtendBlendVistor : public Bifrost::API::Visitor {
public:
    ExtendBlendVistor(const Bifrost::API::VoxelChannel& sdf, float h, float radius, Bifrost::API::VoxelChannel& out)
        : sdf(sdf), dx(Bifrost::API::Layout(sdf.layout()).voxelScale()), h(h/dx), radius(radius/dx), out(out){}
    ExtendBlendVistor(const ExtendBlendVistor& o)
        : sdf(o.sdf), dx(o.dx), h(o.h), radius(o.radius), out(o.out){}
    Bifrost::API::Visitor* copy() const override{ return new ExtendBlendVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& const_accessor, const Bifrost::API::TreeIndex& index) override{
        Bifrost::API::TileAccessor accessor(const_accessor);

        const Bifrost::API::Tile& tile = accessor.tile(index);
        const Bifrost::API::TileCoord& coord = tile.coord();

        Bifrost::API::TileData<float> data = sdf.tileData<float>(index);
        for(size_t e = 0; e < data.count(); ++e){
            int j = (int (e / 5)) % 5;
            float simH = data[e], planeH = (coord.j + j - .5f)-h;
            float r = fabs(planeH);

            if(simH == sdf.backgroundValue<float>()){
                data[e] = planeH;
            }else if(r > radius || (planeH < 0 && simH > 0)){
                data[e] = simH;
            }else{
                data[e] = fabs(simH) < fabs(planeH)? simH : planeH;
            }
        }
    }

private:
    const Bifrost::API::VoxelChannel sdf;
    Bifrost::API::VoxelChannel out;
    float dx, h, radius;
};

std::ostream& operator<<(std::ostream& stream, const Bifrost::API::TileCoord& coord){
    return stream << "Coord[" << coord.i << ", " << coord.j << ", " << coord.k << "]";
}

void crawl(Bifrost::API::VoxelChannel &sdf, Bifrost::API::Tile current, int i, int k, float h){
    Bifrost::API::Layout layout(sdf.layout());
    Bifrost::API::TileCoord coord = current.coord();
    if(!current.valid() || coord.j > h) return;

    Bifrost::API::TileAccessor accessor = Bifrost::API::Layout(sdf.layout()).tileAccessor();
    int step = current.info().dimInfo.depthWidth;
    int depth = current.info().depth;

    if(current.info().depth == layout.maxDepth()){
        //std::cerr << coord << ", " << current.info().depth << std::endl;
        Bifrost::API::TileData<float> data = sdf.tileData<float>(current.index());

        int j = 0;
        while(data(i,j,k) >= 0){
            float tmph = (coord.j + j - .5f)-h;
            if(tmph > 0) return;
            data(i,j,k) = tmph;
            j = (j+1)%5;

            if(j==0){
                coord.j += step;
                current = accessor.tile(coord, current.info().depth);
                if(current.info().depth < depth) current = Bifrost::API::Tile();
                if(!current.valid()) break;
                data = sdf.tileData<float>(current.index());
            }
        }
    }else{
        int j = 0;
        while(!current.child(i,j,k).valid()){
            if(coord.j > h) return;
            j = (j+1)%5;
            if(j==0){
                coord.j += step;
                current = accessor.tile(coord, depth);
                if(current.info().depth < depth) current = Bifrost::API::Tile();
                if(!current.valid()) break;
            }
        }
        if(current.valid() && current.child(i,j,k).valid()){
            Bifrost::API::Tile child = accessor.tile(current.child(i,j,k));
            if(child.info().depth == current.info().depth) return;
            for(int ii = 0; ii < 5; ++ii){
                for(int kk = 0; kk < 5; ++kk){
                    crawl(sdf,child,ii,kk, h);
                }
            }
        }
    }
}

void cullBottom(Bifrost::API::VoxelChannel &sdf, float h){
    Bifrost::API::Layout layout(sdf.layout());
    const Bifrost::API::Tile& root = *layout.tileIterator();
    float tileSpaceH = h/layout.voxelScale();
    DUMP("CULL STARTS");

    for(int i = 0; i < 5; ++i)
        for(int k = 0; k < 5; ++k)
            crawl(sdf,root,i,k, tileSpaceH);
    DUMP("CULL ENDS");
}

class ExtendUVVistor : public Bifrost::API::Visitor {
public:
    ExtendUVVistor(const amino::Math::vec2f& min, const amino::Math::vec2f& max, Bifrost::API::VoxelChannel& out) : out(out) {
        float dx = Bifrost::API::Layout(out.layout()).voxelScale();
        mat =   amino::Math::mat33f(dx,0,0,  0,dx,0,  0,0,1) *
                amino::Math::mat33f(1,0,-min[0],  0,1,-min[1],  0,0,1) *
                amino::Math::mat33f(1./(max[0]-min[0]),0,0,  0,1./(max[1]-min[1]),0,  0,0,1);
    }
    ExtendUVVistor(const ExtendUVVistor& o)
        : mat(o.mat), out(o.out) {}
    Bifrost::API::Visitor* copy() const override{ return new ExtendUVVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        const Bifrost::API::Tile& tile = accessor.tile(index);
        const Bifrost::API::TileCoord& coord = tile.coord();
        Bifrost::API::TileData<amino::Math::vec2f> data = out.tileData<amino::Math::vec2f>(index);

        const int w = tile.info().dimInfo.depthWidth;
        FOR_IJK(i,j,k,w){
            amino::Math::vec3f uv = mat * amino::Math::vec3f(coord.i+i, coord.k+k, 1);
            data(i,j,k) = amino::Math::vec2f(uv[0],uv[1]);
        }
    }

private:
    amino::Math::mat33f mat;
    Bifrost::API::VoxelChannel out;
};

}

namespace Bifrost {
namespace Processing {

void extend(const API::VoxelChannel &sdf, float height, const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, float radius, Bifrost::API::VoxelChannel& out){
    Bifrost::API::Layout layout(sdf.layout());
    std::cerr << sdf.valid() << height << dimensions[0] << radius << out.valid() << std::endl;

    //if(sdf.valid() || !sdf.valid()){ return; }
    cullBottom(out, height);
    //if(sdf.valid() || !sdf.valid()){ return; }

    amino::Math::bboxf bbox(amino::Math::vec3f(center[0] - dimensions[0]*.5, height, center[1] - dimensions[1]*.5),
                            amino::Math::vec3f(center[0] + dimensions[0]*.5, height, center[1] + dimensions[1]*.5));

    {// add tile
        float invDx = 1./layout.voxelScale();
        const amino::Math::bboxi ranges(
                    amino::Math::vec3i((int)floor((bbox.min()[0]-radius)*invDx), (int)floor((bbox.max()[1]-radius)*invDx), (int)floor((bbox.min()[2]-radius)*invDx)),
                    amino::Math::vec3i( (int)ceil((bbox.max()[0]+radius)*invDx),  (int)ceil((bbox.max()[1]+radius)*invDx),  (int)ceil((bbox.max()[2]+radius)*invDx)));

        int n = layout.tileDimInfo().tileWidth;
        Bifrost::API::TileAccessor accessor = layout.tileAccessor();
        for(int j = ranges.min()[1]; j <= ranges.max()[1]+n; j+=n){
            for(int i = ranges.min()[0]; i <= ranges.max()[0]+n; i+=n){
                for(int k = ranges.min()[2]; k <= ranges.max()[2]+n; k+=n){
                    accessor.addTile(i,j,k,layout.maxDepth());
                }
            }
        }
    }
    ExtendBlendVistor visitor(sdf, height, radius, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
}

void extendUVs(const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, API::VoxelChannel &out){
    Bifrost::API::Layout layout(out.layout());
    ExtendUVVistor visitor(center-dimensions*.5, center+dimensions*.5, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::BreadthFirst, layout.maxDepth(), layout.maxDepth());
}

}}
