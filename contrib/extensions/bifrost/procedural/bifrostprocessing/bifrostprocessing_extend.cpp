#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_voxelchannel.h>
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
#include "tbb.h"

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

class ExtendUVVisitor : public Bifrost::API::Visitor {
public:
    ExtendUVVisitor(const amino::Math::vec2f& min, const amino::Math::vec2f& max, Bifrost::API::VoxelChannel& out) : out(out) {
        float dx = Bifrost::API::Layout(out.layout()).voxelScale();
        mat =   amino::Math::mat33f(1./(max[0]-min[0]),0,0,  0,1./(max[1]-min[1]),0,  0,0,1) *
                amino::Math::mat33f(1,0,-min[0],  0,1,-min[1],  0,0,1) *
                amino::Math::mat33f(dx,0,0,  0,dx,0,  0,0,1);
    }
    ExtendUVVisitor(const ExtendUVVisitor& o)
        : mat(o.mat), out(o.out) {}
    Bifrost::API::Visitor* copy() const override{ return new ExtendUVVisitor(*this); }

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

class PlaneSdfVisitor : public Bifrost::API::Visitor {
public:
    PlaneSdfVisitor(float h, Bifrost::API::VoxelChannel& out)
        : dx(Bifrost::API::Layout(out.layout()).voxelScale()), h(h/dx), out(out){}
    PlaneSdfVisitor(const PlaneSdfVisitor& o)
        : dx(o.dx), h(o.h), out(o.out){}
    Bifrost::API::Visitor* copy() const override{ return new PlaneSdfVisitor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        const Bifrost::API::TileCoord& coord = accessor.tile(index).coord();
        Bifrost::API::TileData<float> data = out.tileData<float>(index);
        int tileWidth = accessor.tileInfo(index).dimInfo.tileWidth;
        int W = accessor.tileInfo(index).dimInfo.depthWidth / tileWidth;
        float w = W * .5;
        for(size_t e = 0; e < data.count(); ++e){
            int j = ((int (e / tileWidth)) % tileWidth);
            data[e] = ((coord.j + j*W + w) - h)*dx; // in world space
        }
    }
private:
    Bifrost::API::VoxelChannel out;
    float dx, h; // h is in tile space
};
inline void createOceanPlane(Bifrost::API::VoxelChannel& sdf, float height){
    PROFILER("CREATE OCEAN PLANE");
    Bifrost::API::Layout layout(sdf.layout());
    PlaneSdfVisitor visitor(height, sdf);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
}


// ***** FLOODING ALGORITHM *****

struct FloodTile{
    Bifrost::API::Tile tile;
    float H;
    const Bifrost::API::VoxelChannel sdfChannel, alphaChannel;
    const Bifrost::API::TileData<float> _sdf;
    Bifrost::API::TileData<float> _alpha;

    FloodTile(const Bifrost::API::Tile& tile, const Bifrost::API::VoxelChannel& sdf, Bifrost::API::VoxelChannel& alpha, int H)
        : tile(tile), H(H), sdfChannel(sdf), alphaChannel(alpha),
          _sdf(tile.valid()? sdf.tileData<float>(tile.index()) : Bifrost::API::TileData<float>()),
          _alpha(tile.valid()? alpha.tileData<float>(tile.index()) : Bifrost::API::TileData<float>()) {}

    inline bool valid() const{ return tile.valid(); }
    inline const float& sdf(int i, int j, int k) const{ return _sdf(i,j,k); }
    inline float& alpha(int i, int j, int k){ return _alpha(i,j,k); }

    inline bool flood(int i, int j, int k){
        bool ret = (tile.coord().j <= H) && (alpha(i,j,k) == 0) && (sdf(i,j,k) >= 0);
        if(ret) alpha(i,j,k) = 1;
        return ret;
    }
};

class FloodVisitor : public Bifrost::API::Visitor {
public:
    FloodVisitor(Bifrost::API::VoxelChannel& sdf, float height, Bifrost::API::VoxelChannel& alpha)
        : sdf(sdf), alpha(alpha), dx(Bifrost::API::Layout(alpha.layout()).voxelScale()),
          H(height/dx),
          N(Bifrost::API::Layout(sdf.layout()).tileDimInfo().tileWidth), n(N-1),
          maxDepth(Bifrost::API::Layout(sdf.layout()).maxDepth()){}
    FloodVisitor(const FloodVisitor& o)
        : sdf(o.sdf), alpha(o.alpha), dx(o.dx), H(o.H), N(o.N), n(N-1), maxDepth(o.maxDepth) {}
    Bifrost::API::Visitor* copy() const override{ return new FloodVisitor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        indexes.insert(index.tile);
        Bifrost::Private::SweepAccessor sweeper(accessor);

        std::unordered_set<int>::iterator it;
        while((it = indexes.begin()) != indexes.end()){
            Bifrost::API::TreeIndex center(*it, maxDepth);
            indexes.erase(it);
            assert(center.valid());

            Bifrost::API::Tile tile = accessor.tile(center);
            coord = tile.coord();
            sdfData = sdf.tileData<float>(center);
            alphaData = alpha.tileData<float>(center);

            // flood current tile
            FOR_IJK(i,j,k,N){
                if(alphaData(i,j,k) == 1)
                    flood(i, j, k);
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
    float dx;
    float H; // ocean height in tile space

    std::unordered_set<int> indexes;
    Bifrost::API::TileCoord coord;
    Bifrost::API::TileData<float> sdfData, alphaData;
    int N, n, maxDepth;

    void flood(int i, int j, int k){
        if(coord.j + j + .5 > H) { // over ocean level
            for(; j<N && sdfData(i,j,k) > 0 && alphaData(i,j,k) != 1; ++j)
                alphaData(i,j,k) = 1;
            return;
        }
        //if(sdfData(i,j,k) < ((coord.j + j + 1.5 >= H)? -dx*0.8660254f : 0)) return; // completely inside water 0.8660254 = sqrt(3)/2 (half unit cube diagonal)
        if(sdfData(i,j,k) < -dx*0.8660254f) return; // completely inside water 0.8660254 = sqrt(3)/2 (half unit cube diagonal)
        alphaData(i,j,k) = 1;

        if(i>0 && alphaData(i-1,j,k) != 1) flood( i-1, j, k );
        if(i<n && alphaData(i+1,j,k) != 1) flood( i+1, j, k );
        if(j>0 && alphaData(i,j-1,k) != 1) flood( i, j-1, k );
        if(j<n && alphaData(i,j+1,k) != 1) flood( i, j+1, k );
        if(k>0 && alphaData(i,j,k-1) != 1) flood( i, j, k-1 );
        if(k<n && alphaData(i,j,k+1) != 1) flood( i, j, k+1 );
    }
};

class FloodRadiusVisitor : public Bifrost::API::Visitor {
public:
    FloodRadiusVisitor(Bifrost::API::VoxelChannel& sdf, float height, float radius, Bifrost::API::VoxelChannel& alpha)
        : sdf(sdf), alpha(alpha),
          H(floor(height/Bifrost::API::Layout(alpha.layout()).voxelScale())+1),
          R(radius/Bifrost::API::Layout(alpha.layout()).voxelScale()), invR2(1./(R*R)),
          N(Bifrost::API::Layout(sdf.layout()).tileDimInfo().tileWidth),
          maxDepth(Bifrost::API::Layout(sdf.layout()).maxDepth()){ _haloCache.init(alpha); }
    FloodRadiusVisitor(const FloodRadiusVisitor& o)
        : sdf(o.sdf), alpha(o.alpha), H(o.H), R(o.R), invR2(o.invR2), N(o.N), maxDepth(o.maxDepth) { _haloCache.init(alpha); }
    Bifrost::API::Visitor* copy() const override{ return new FloodRadiusVisitor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        Bifrost::API::Tile tile = accessor.tile(index);
        int J = H - tile.coord().j;
        if(J < 0 || J > N) return;
        _haloCache.update(index);
        const Bifrost::API::HaloedCache<float>::Tile& halo = _haloCache.tile(index);

        alphaData = alpha.tileData<float>(index);
        sdfData = sdf.tileData<float>(index);

        // flood current tile
        for(int I = 0; I < N; ++I){
            for(int K = 0; K < N; ++K){
                if(alphaData(I,J,K) != 1 || sdfData(I,J,K) < 0) continue;
                bool contour = false;
                for(int i = -1; i <= 1 && !contour; i+=2){
                    for(int k = -1; k <= 1 && !contour; k+=2){
                        if(halo(I+i,J,K+k) != 1) contour = true;
                    }
                }
                if(!contour) continue;

                FOR_IJK_SYMMETRIC(i,j,k,R){
                    float dist = (i*i+j*j+k*k)*invR2;
                    if(dist > 1) continue;
                    dist = 1 - sqrt(dist);
                    Bifrost::API::TileCoord coord = tile.coord() + Bifrost::API::TileCoord(I+i, J+j, K+k);
                    Bifrost::API::Tile ctile = accessor.tile(coord, maxDepth);
                    if(ctile.index().depth != maxDepth) continue;
                    float& prev = alpha.tileData<float>(ctile.index())(coord.i-ctile.coord().i, coord.j-ctile.coord().j, coord.k-ctile.coord().k);
                    if(prev == 1) continue;
                    if(prev == 0 || prev < dist) prev = dist;
                }
            }
        }
    }
private:
    Bifrost::API::HaloedCache<float> _haloCache;
    Bifrost::API::VoxelChannel sdf, alpha;
    int H; // ocean height in tile space
    float R, invR2; // radius, 1/(radius^2) in tile space

    Bifrost::API::TileData<float> sdfData, alphaData;
    int N, maxDepth;
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
            TBB_FOR_ALL<int>(0,25, [&](int i){
                crawl(sdf,alpha,child,i/5,0,i%5, H);
            });
        }
    }
}

void computeAlpha(Bifrost::API::VoxelChannel &sdf, Bifrost::API::VoxelChannel& alpha, float height, float radius){
    if((0)) DUMP(radius);
    Bifrost::API::Layout layout(sdf.layout());
    int N = layout.tileDimInfo().tileWidth;
    {
        // TODO: make this parallel
        PROFILER("CRAWL");
        int H(floor(height/layout.voxelScale())+5);
        Bifrost::API::Tile root = layout.tileAccessor().tile(0,0,0,0);
        TBB_FOR_ALL<int>(0,N*N,[&](int i){
            crawl(sdf, alpha, root, i/N, 0, i%N, H);
        });
    }
    {
        PROFILER("FLOOD");
        FloodVisitor visitor(sdf, height, alpha);
        layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
        FloodRadiusVisitor visitor2(sdf, height, radius, alpha);
        layout.traverse(visitor2, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
    }
}

// ***** MERGE *****

class MergeVisitor : public Bifrost::API::Visitor {
public:
    MergeVisitor(const Bifrost::API::VoxelChannel& sdf, const Bifrost::API::VoxelChannel& ocean, const Bifrost::API::VoxelChannel& alpha, Bifrost::API::VoxelChannel& out)
        : sdf(sdf), ocean(ocean), alpha(alpha), out(out) {}
    MergeVisitor(const MergeVisitor& o)
        : sdf(o.sdf), ocean(o.ocean), alpha(o.alpha), out(o.out){}
    Bifrost::API::Visitor* copy() const override{ return new MergeVisitor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& , const Bifrost::API::TreeIndex& index) override{
        Bifrost::API::TileData<float> sdfData = sdf.tileData<float>(index);
        Bifrost::API::TileData<float> oceanData  = ocean.tileData<float>(index);
        Bifrost::API::TileData<float> alphaData = alpha.tileData<float>(index);
        Bifrost::API::TileData<float> outData = out.tileData<float>(index);

        float bg = sdf.backgroundValue<float>();

        for(size_t e = 0; e < outData.count(); ++e){
            float p = sdfData[e], q = oceanData[e], a = alphaData[e], &r = outData[e];
            //if(p == bg){ p = q; }
            if(a == 0){
                q = fmax(q,p); // reenforce holes
            }
            r = p + q - sqrt(p*p+q*q);
        }
    }
private:
    Bifrost::API::VoxelChannel sdf, ocean, alpha, out;
};

inline void merge(const Bifrost::API::VoxelChannel& in1, const Bifrost::API::VoxelChannel& in2, const Bifrost::API::VoxelChannel& alpha, Bifrost::API::VoxelChannel& out){
    PROFILER("BLEND OCEAN PLANE");
    assert(in1.layout() == in2.layout() && in2.layout() == alpha.layout() && alpha.layout() == out.layout());
    Bifrost::API::Layout layout(in1.layout());
    MergeVisitor visitor(in2, in1, alpha, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
}

}

namespace Bifrost {
namespace Processing {

ExtendFilter::ExtendFilter(float height, const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, float radius, const Bifrost::API::String& out_channel)
    : height(height), center(center), dimensions(dimensions), radius(radius), out_channel(out_channel) {}

void ExtendFilter::filter(const Bifrost::API::Channel in, Bifrost::API::Channel _out) const{
    Bifrost::API::VoxelChannel sdf(in), out(_out);
    Bifrost::API::Layout layout(sdf.layout());

    if(true){// add tiles
        // TODO: make this parallel
        PROFILER("ADDING OCEAN TILES");
        amino::Math::bboxf bbox(amino::Math::vec3f(center[0] - dimensions[0]*.5, height, center[1] - dimensions[1]*.5),
                                amino::Math::vec3f(center[0] + dimensions[0]*.5, height, center[1] + dimensions[1]*.5));

        float invDx = 1./layout.voxelScale();
        int N = layout.tileDimInfo().tileWidth;
        // TODO : could it be narrowed
        const amino::Math::bboxi ranges(
                    amino::Math::vec3i((int)floor((bbox.min()[0])*invDx), (int)floor((bbox.max()[1])*invDx)-N, (int)floor((bbox.min()[2])*invDx)),
                    amino::Math::vec3i( (int)ceil((bbox.max()[0])*invDx),  (int)ceil((bbox.max()[1])*invDx)+N,  (int)ceil((bbox.max()[2])*invDx)));

        int depth = layout.maxDepth();
        int jmin = ranges.min()[1], jmax = ranges.max()[1];
        Bifrost::API::TileAccessor accessor = layout.tileAccessor();

        for(int j = jmin; j <= jmax; j+=N){
            for(int i = ranges.min()[0]; i <= ranges.max()[0]; i+=N){
                for(int k = ranges.min()[2]; k <= ranges.max()[2]; k+=N){
                    accessor.addTile(i,j,k,depth);
                }
            }
        }
        //*
        int n2 = N*N;
        for(int i = ranges.min()[0]; i <= ranges.max()[0]; i+=n2){
            for(int k = ranges.min()[2]; k <= ranges.max()[2]; k+=n2){
                accessor.addTile(i,jmin-N,k,depth-1);
                accessor.addTile(i,jmax+N,k,depth-1);
            }
        }
        //*/
    }

    Bifrost::API::StateServer ss = Bifrost::API::ObjectModel().stateServer(sdf.stateID());
    Bifrost::API::Component component = sdf.component();

    Bifrost::API::VoxelChannel alpha = ss.createChannel(component, Bifrost::API::DataType::FloatType, (out_channel.empty()? "oceanTmp" : out_channel));
    computeAlpha(out, alpha, height, radius);

    if(sdf == out){
        sdf = ss.createChannel(component, Bifrost::API::DataType::FloatType, "tmp");
        createOceanPlane(sdf, height);
        merge(sdf, out, alpha, out);
        Bifrost::API::String name = sdf.fullPathName();
        sdf.reset();
        ss.removeChannel(name);
    }else{
        createOceanPlane(out, height);
        merge(out, sdf, alpha, out);
    }

    if(out_channel.empty()){
        // clear temporary channel
        Bifrost::API::String name = alpha.fullPathName();
        alpha.reset();
        ss.removeChannel(name);
    }
}

void ExtendFilter::uvs(Bifrost::API::Channel _out) const{
    Bifrost::API::VoxelChannel out(_out);
    PROFILER("EXTEND UVS");
    Bifrost::API::Layout layout(out.layout());
    ExtendUVVisitor visitor(center-dimensions*.5, center+dimensions*.5, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
}

}}
