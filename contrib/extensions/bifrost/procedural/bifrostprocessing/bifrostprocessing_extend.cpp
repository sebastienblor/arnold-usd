#include <bifrostprocessing/bifrostprocessing_extend.h>
#include <bifrostprocessing/bifrostprocessing_visitors.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_layout.h>
#include <limits>
#include "defs.h"
#include "utils.h"
#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_component.h>
#include <unordered_set>

#define FLOOD(direction, i1, j1, k1, i2, j2, k2) \
{\
    bool eval = false;\
    for(int s = 0; s < N && !eval; ++s) for(int t = 0; t < N && !eval; ++t) eval |= alphaData(i1,j1,k1) == 1;\
    if(eval){\
        FloodTile neighbor(sweeper.direction(tile), sdf, alpha, H);\
        if(neighbor.valid()){\
            eval = false;\
            for(int s = 0; s < N; ++s)\
                for(int t = 0; t < N; ++t)\
                    eval |= (alphaData(i1,j1,k1) == 1 && neighbor.flood(i2,j2,k2));\
            if(eval) indexes.insert(neighbor.tile.index().tile);\
        }\
    }\
}

namespace{

// ***** OCEAN UVS *****

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


// ***** OCEAN SDF *****

class PlaneSdfVistor : public Bifrost::API::Visitor {
public:
    PlaneSdfVistor(float h, Bifrost::API::VoxelChannel& out)
        : h(h/Bifrost::API::Layout(out.layout()).voxelScale()), out(out){}
    PlaneSdfVistor(const PlaneSdfVistor& o)
        : h(o.h), out(o.out){}
    Bifrost::API::Visitor* copy() const override{ return new PlaneSdfVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        const Bifrost::API::TileCoord& coord = accessor.tile(index).coord();
        Bifrost::API::TileData<float> data = out.tileData<float>(index);
        for(size_t e = 0; e < data.count(); ++e)
            data[e] = (coord.j + ((int (e / 5)) % 5) - .5f)-h;
    }
private:
    Bifrost::API::VoxelChannel out;
    float h;
};
inline void createOceanPlane(Bifrost::API::VoxelChannel& sdf, float height){
    PROFILER("CREATE OCEAN PLANE");
    Bifrost::API::Layout layout(sdf.layout());
    PlaneSdfVistor visitor(height, sdf);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
}


// ***** FLOODING ALGORITHM *****

struct FloodTile{
    Bifrost::API::Tile tile;
    int H;
    const Bifrost::API::VoxelChannel sdfChannel, alphaChannel;
    const Bifrost::API::TileData<float> _sdf;
    Bifrost::API::TileData<float> _alpha;

    FloodTile(const Bifrost::API::Tile& tile, const Bifrost::API::VoxelChannel& sdf, Bifrost::API::VoxelChannel& alpha, int H)
        : tile(tile), H(H), sdfChannel(sdf), alphaChannel(alpha),
          _sdf(tile.valid()? sdf.tileData<float>(tile.index()) : Bifrost::API::TileData<float>()),
          _alpha(tile.valid()? alpha.tileData<float>(tile.index()) : Bifrost::API::TileData<float>()) {}

    inline bool valid() const{ return tile.valid(); }
    inline const float& sdf(int i, int j, int k) const{ return _sdf(i,j,k); }
    inline const float& alpha(int i, int j, int k) const{ return _alpha(i,j,k); }
    inline float& alpha(int i, int j, int k){ return _alpha(i,j,k); }

    inline bool flood(int i, int j, int k){
        bool ret = (tile.coord().j + j <= H) && (alpha(i,j,k) == 0) && (sdf(i,j,k) >= 0);
        if(ret) alpha(i,j,k) = 1;
        return ret;
    }
};

struct FloodSharedData{
    Bifrost::API::TileData<float> sdfData;
    int H, n, J;
};
void flood(const FloodSharedData& shared, Bifrost::API::TileData<float>& alphaData, int i, int j, int k){
    if(shared.J + j > shared.H) return; // over ocean level
    if(shared.sdfData(i,j,k) < 0) return; // already inside water
    alphaData(i,j,k) = 1;

    const int& n = shared.n;
    if(i>0 && alphaData(i-1,j,k) != 1) flood( shared, alphaData, i-1, j, k );
    if(i<n && alphaData(i+1,j,k) != 1) flood( shared, alphaData, i+1, j, k );
    if(j>0 && alphaData(i,j-1,k) != 1) flood( shared, alphaData, i, j-1, k );
    if(j<n && alphaData(i,j+1,k) != 1) flood( shared, alphaData, i, j+1, k );
    if(k>0 && alphaData(i,j,k-1) != 1) flood( shared, alphaData, i, j, k-1 );
    if(k<n && alphaData(i,j,k+1) != 1) flood( shared, alphaData, i, j, k+1 );
}

class FloodVisitor : public Bifrost::API::Visitor {
public:
    FloodVisitor(Bifrost::API::VoxelChannel& sdf, float height, Bifrost::API::VoxelChannel& alpha)
        : sdf(sdf), H(floor(height/Bifrost::API::Layout(alpha.layout()).voxelScale())+1), alpha(alpha){}
    FloodVisitor(const FloodVisitor& o)
        : sdf(o.sdf), H(o.H), alpha(o.alpha){}
    Bifrost::API::Visitor* copy() const override{ return new FloodVisitor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        indexes.insert(index.tile);
        const int depth = Bifrost::API::Layout(sdf.layout()).maxDepth();
        const int N = accessor.tile(index).info().dimInfo.tileWidth;
        const int n = N-1;
        Bifrost::Private::SweepAccessor sweeper(accessor);

        std::unordered_set<int>::iterator it;
        while((it = indexes.begin()) != indexes.end()){
            Bifrost::API::TreeIndex center(*it, depth);
            indexes.erase(it);
            assert(center.valid());

            Bifrost::API::Tile tile = accessor.tile(center);
            Bifrost::API::TileData<float> alphaData = alpha.tileData<float>(center);
            FloodSharedData shared = { sdf.tileData<float>(center), H, n, tile.coord().j };

            // flood current tile
            FOR_IJK(i,j,k,N){
                if(alphaData(i,j,k) == 1)
                    flood(shared, alphaData, i, j, k);
            }
            // add neighboring indexes if we need to flood them
            FLOOD(   left, 0,s,t, n,s,t);
            FLOOD(  right, n,s,t, 0,s,t);
            FLOOD( bottom, s,0,t, s,n,t);
            FLOOD(    top, s,n,t, s,0,t);
            FLOOD( closer, s,t,0, s,t,n);
            FLOOD(further, s,t,n, s,t,0);
        }
    }
private:
    Bifrost::API::VoxelChannel sdf, alpha;
    int H;
    std::unordered_set<int> indexes;
};


// ***** CRAWLING ALGORITHM *****

void crawl(const Bifrost::API::VoxelChannel &sdf, Bifrost::API::VoxelChannel& alpha, Bifrost::API::Tile current, int i, int j, int k, int H){
    Bifrost::API::Layout layout(sdf.layout());
    Bifrost::API::TileCoord coord = current.coord();
    if(!current.valid() || coord.j > H) return;

    Bifrost::API::TileAccessor accessor = Bifrost::API::Layout(sdf.layout()).tileAccessor();
    int step = current.info().dimInfo.depthWidth;
    int depth = current.info().depth;

    if(current.info().depth == layout.maxDepth()){
        Bifrost::API::TileData<float> sdfData = sdf.tileData<float>(current.index());
        Bifrost::API::TileData<float> alphaData = alpha.tileData<float>(current.index());

        while(sdfData(i,j,k) >= 0 && (coord.j + j <= H)){ // under ocean level
            alphaData(i,j,k) = 1;
            j = (j+1)%5;
            if(j==0){
                coord.j += step;
                current = accessor.tile(coord, current.info().depth);
                if(current.info().depth < depth) current = Bifrost::API::Tile();
                if(!current.valid()) break; // NO GOOD
                sdfData = sdf.tileData<float>(current.index());
                alphaData = alpha.tileData<float>(current.index());
            }
        }
    }else{
        while(!current.child(i,j,k).valid()){
            if(coord.j > H) return;
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
                    crawl(sdf,alpha,child,ii,0,kk, H);
                }
            }
        }
    }
}

void computeAlpha(Bifrost::API::VoxelChannel &sdf, Bifrost::API::VoxelChannel& alpha, float height, float radius){
    Bifrost::API::Layout layout(sdf.layout());
    {
        // TODO: make this parallel
        PROFILER("CRAWL");
        int H(ceil(height/Bifrost::API::Layout(alpha.layout()).voxelScale())+1);
        Bifrost::API::Tile root = layout.tileAccessor().tile(0,0,0,0);
        for(int i = 0; i < 5; ++i){
            for(int k = 0; k < 5; ++k){
                crawl(sdf, alpha, root, i, 0, k, H);
            }
        }
    }
    {
        PROFILER("FLOOD");
        FloodVisitor visitor(sdf, height, alpha);
        layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
    }
}


// ***** ALPHA BLENDING *****

inline void blend(const Bifrost::API::VoxelChannel& in1, const Bifrost::API::VoxelChannel& in2, const Bifrost::API::VoxelChannel& alpha, Bifrost::API::VoxelChannel& out){
    PROFILER("BLEND OCEAN PLANE");
    assert(in1.layout() == in2.layout() && in2.layout() == alpha.layout() && alpha.layout() == out.layout());
    Bifrost::API::Layout layout(in1.layout());
    Bifrost::Processing::BlendVisitor<float> visitor(in1, in2, alpha, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
}

}

namespace Bifrost {
namespace Processing {

void extend(const Bifrost::API::VoxelChannel& _sdf, float height, const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, float radius, Bifrost::API::VoxelChannel& out){
    Bifrost::API::VoxelChannel sdf(_sdf);
    Bifrost::API::Layout layout(sdf.layout());

    {// add tiles
        // TODO: make this parallel
        PROFILER("ADDING OCEAN TILES");
        amino::Math::bboxf bbox(amino::Math::vec3f(center[0] - dimensions[0]*.5, height, center[1] - dimensions[1]*.5),
                                amino::Math::vec3f(center[0] + dimensions[0]*.5, height, center[1] + dimensions[1]*.5));

        float invDx = 1./layout.voxelScale();
        const amino::Math::bboxi ranges(
                    amino::Math::vec3i((int)floor((bbox.min()[0]-radius)*invDx), (int)floor((bbox.max()[1]-radius)*invDx), (int)floor((bbox.min()[2]-radius)*invDx)),
                    amino::Math::vec3i( (int)ceil((bbox.max()[0]+radius)*invDx),  (int)ceil((bbox.max()[1]+radius)*invDx),  (int)ceil((bbox.max()[2]+radius)*invDx)));

        int n = layout.tileDimInfo().tileWidth;
        Bifrost::API::TileAccessor accessor = layout.tileAccessor();
        for(int j = ranges.min()[1]; j <= ranges.max()[1]+2*n; j+=n){
            for(int i = ranges.min()[0]; i <= ranges.max()[0]+n; i+=n){
                for(int k = ranges.min()[2]; k <= ranges.max()[2]+n; k+=n){
                    accessor.addTile(i,j,k,layout.maxDepth());
                }
            }
        }
    }

    Bifrost::API::StateServer ss = Bifrost::API::ObjectModel().stateServer(sdf.stateID());
    Bifrost::API::Component component = sdf.component();

    Bifrost::API::VoxelChannel alpha = ss.createChannel(component, Bifrost::API::DataType::FloatType, "alpha");
    computeAlpha(out, alpha, height, radius);

    if(sdf == out){
        sdf = ss.createChannel(component, Bifrost::API::DataType::FloatType, "tmp");
        createOceanPlane(sdf, height);
        blend(sdf, out, alpha, out);
        Bifrost::API::String name = sdf.fullPathName();
        sdf.reset();
        ss.removeChannel(name);
    }else{
        createOceanPlane(out, height);
        blend(out, sdf, alpha, out);
    }

    //Bifrost::API::String name = alpha.fullPathName();
    //alpha.reset();
    //ss.removeChannel(name);
}

void extendUVs(const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, API::VoxelChannel &out){
    PROFILER("EXTEND UVS");
    Bifrost::API::Layout layout(out.layout());
    ExtendUVVistor visitor(center-dimensions*.5, center+dimensions*.5, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
}

}}
