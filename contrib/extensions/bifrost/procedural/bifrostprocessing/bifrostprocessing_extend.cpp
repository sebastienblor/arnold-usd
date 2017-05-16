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

class AlphaVistor : public Bifrost::API::Visitor {
public:
    AlphaVistor(float h, Bifrost::API::VoxelChannel& out)
        : h(h/Bifrost::API::Layout(out.layout()).voxelScale()+1), out(out){}
    AlphaVistor(const AlphaVistor& o)
        : h(o.h), out(o.out){}
    Bifrost::API::Visitor* copy() const override{ return new AlphaVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        const Bifrost::API::TileCoord& coord = accessor.tile(index).coord();
        Bifrost::API::TileData<float> data = out.tileData<float>(index);
        for(size_t e = 0; e < data.count(); ++e){
            int j = ((int (e / 5)) % 5);
            data[e] = ((coord.j + j - .5f)-h) > 0? 0.f : 1.f;
        }
    }

private:
    Bifrost::API::VoxelChannel out;
    float h; // tile space
};

inline Bifrost::API::Tile left(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.i -= 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
inline Bifrost::API::Tile right(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.i += 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
inline Bifrost::API::Tile closer(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.k -= 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
inline Bifrost::API::Tile further(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.k += 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
inline Bifrost::API::Tile bottom(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.j -= 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}
inline Bifrost::API::Tile top(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::Tile& tile){
    Bifrost::API::TileCoord coord = tile.coord();
    coord.j += 5;
    Bifrost::API::Tile out = accessor.tile(coord, tile.info().depth);
    return out.info().depth == tile.info().depth? out : Bifrost::API::Tile();
}

void blend(const Bifrost::API::VoxelChannel& sdf, Bifrost::API::VoxelChannel& alpha, const Bifrost::API::Tile& current, int i, int j, int k, const Bifrost::API::TileCoord& coord, float radius2){
    {
        if(!current.valid()) return;
        amino::Math::vec3i ab(current.coord().i+i-coord.i, current.coord().j+j-coord.j, current.coord().k+k-coord.k);
        float dist2 = ab[0]*ab[0] + ab[1]*ab[1] + ab[2]*ab[2];
        if(dist2 > radius2) return; // outside blend cylinder

        if(ab.nonzero()){// && sdf.tileData<float>(current.index())(i,j,k) > 0){
            float& v = alpha.tileData<float>(current.index())(i,j,k);
            if(v == 1 || v == -1) return; // ocean only
            //float a =  1 - dist2 / radius2;
            float a =  (radius2-dist2) / radius2;
            if(sdf.tileData<float>(current.index())(i,j,k) < 0){
                a = 1e-90;
            }
            if(a > v) v = a;
            else return;
        }
    }
    int n = Bifrost::API::Layout(sdf.layout()).tileDimInfo().tileWidth;
    Bifrost::API::TileAccessor accessor(Bifrost::API::Layout(sdf.layout()).tileAccessor());
    blend( sdf, alpha,  (i>0)? current :    left(accessor,current), (i+n-1)%n,         j,         k, coord, radius2 );
    blend( sdf, alpha,(i+1<n)? current :   right(accessor,current),   (i+1)%n,         j,         k, coord, radius2 );
    blend( sdf, alpha,  (j>0)? current :  bottom(accessor,current),         i, (j+n-1)%n,         k, coord, radius2 );
    blend( sdf, alpha,(j+1<n)? current :     top(accessor,current),         i,   (j+1)%n,         k, coord, radius2 );
    blend( sdf, alpha,  (k>0)? current :  closer(accessor,current),         i,         j, (k+n-1)%n, coord, radius2 );
    blend( sdf, alpha,(k+1<n)? current : further(accessor,current),         i,         j,   (k+1)%n, coord, radius2 );
}

struct FloodSharedData{
    const Bifrost::API::VoxelChannel sdf;
    Bifrost::API::VoxelChannel alpha;
    Bifrost::API::TileAccessor accessor;
    int H;
    int N;
};

void flood(const FloodSharedData& shared, const Bifrost::API::Tile& current, int i, int j, int k){
    {
        if(!current.valid()) return;
        float& alphaValue = shared.alpha.tileData<float>(current.index())(i,j,k);
        if(alphaValue == 1) return; // already visited

        float sdfValue = shared.sdf.tileData<float>(current.index())(i,j,k);
        if(current.coord().j + j > shared.H) return; // over ocean level

        if(sdfValue < 0) return; // already inside water
        alphaValue = 1;
    }
    const int n = shared.N;
    flood( shared,   (i>0)? current :    left(shared.accessor,current), (i+n-1)%n,         j,         k );
    flood( shared, (i+1<n)? current :   right(shared.accessor,current),   (i+1)%n,         j,         k );
    flood( shared,   (j>0)? current :  bottom(shared.accessor,current),         i, (j+n-1)%n,         k );
    flood( shared, (j+1<n)? current :     top(shared.accessor,current),         i,   (j+1)%n,         k );
    flood( shared,   (k>0)? current :  closer(shared.accessor,current),         i,         j, (k+n-1)%n );
    flood( shared, (k+1<n)? current : further(shared.accessor,current),         i,         j,   (k+1)%n );
}

class FloodVistor : public Bifrost::API::Visitor {
public:
    FloodVistor(Bifrost::API::VoxelChannel& sdf, float height, Bifrost::API::VoxelChannel& alpha)
        : sdf(sdf), H(floor(height/Bifrost::API::Layout(alpha.layout()).voxelScale())+2), alpha(alpha){}
    FloodVistor(const FloodVistor& o)
        : sdf(o.sdf), H(o.H), alpha(o.alpha){}
    Bifrost::API::Visitor* copy() const override{ return new FloodVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        const Bifrost::API::Tile& current = accessor.tile(index);
        Bifrost::API::TileData<float> alphaData = alpha.tileData<float>(index);
        int n = current.info().dimInfo.tileWidth;
        FloodSharedData shared = { sdf, alpha, accessor, H, n };
        FOR_IJK(i,j,k,n){
            if(alphaData(i,j,k) == 1){
                alphaData(i,j,k) = 0;
                flood(shared, current, i, j, k);
                alphaData(i,j,k) = 1;
            }
        }
    }
private:
    Bifrost::API::VoxelChannel sdf, alpha;
    int H;
};

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

class CrawlVistor : public Bifrost::API::Visitor {
// prerequisite : all tiles must contain leaves
public:
    CrawlVistor(Bifrost::API::VoxelChannel& sdf, float h, Bifrost::API::VoxelChannel& alpha)
        : sdf(sdf), H(floor(h/Bifrost::API::Layout(alpha.layout()).voxelScale())+2), alpha(alpha){}
    CrawlVistor(const CrawlVistor& o)
        : sdf(o.sdf), H(o.H), alpha(o.alpha){}
    Bifrost::API::Visitor* copy() const override{ return new CrawlVistor(*this); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        current = accessor.tile(index);
        if(index.depth != Bifrost::API::Layout(sdf.layout()).maxDepth())
            return;
        const int n = current.info().dimInfo.tileWidth;
        for(int i = 0; i < n; ++i)
            for(int k = 0; k < n; ++k)
                crawl(sdf, alpha, current,i, 0, k, H);
    }
    bool visitVoxel(const Bifrost::API::TileAccessor&, int i, int j, int k){
        const int depth = current.info().depth+1;

        for(int jj = j-1; jj >= 0; --jj){
            if(current.child(i,jj,k).depth == depth){
                std::cerr << i << ", " << j << ", " << k << " (" << depth << ")" << std::endl;
                DUMP(jj);
                DUMP(current.child(i,jj,k).depth);
                DUMP(depth);
                return false;
            }
        }
        return true;
    }

private:
    Bifrost::API::VoxelChannel sdf, alpha;
    int H;
    Bifrost::API::Tile current;
};

class CullVistor : public Bifrost::API::Visitor {
public:
    CullVistor() : hasLeaves(false){}
    Bifrost::API::Visitor* copy() const override{ return new CullVistor(); }

    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index) override{
        depth = index.depth;
        hasLeaves = accessor.tile(index).info().depth == Bifrost::API::Layout(accessor.layout()).maxDepth();
    }
    void join(const Visitor &visitor){
        const CullVistor& o = static_cast<const CullVistor&>(visitor);
        hasLeaves |= ((depth+1 == o.depth) && o.hasLeaves);
    }
    void endTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex &index){
        if(hasLeaves) return;
        Bifrost::API::Tile tile = accessor.tile(index);
        FOR_IJK(i,j,k,5){
            Bifrost::API::TileAccessor(accessor).removeTile(tile.child(i,j,k)).succeeded();
        }
        //if(!hasLeaves) Bifrost::API::TileAccessor(accessor).removeTile(index);
    }

private:
    int depth;
    bool hasLeaves;
};


void computeAlpha(Bifrost::API::VoxelChannel &sdf, Bifrost::API::VoxelChannel& alpha, float height, float radius){
    Bifrost::API::Layout layout(sdf.layout());
    //CullVistor culler;
    //layout.traverse(culler, Bifrost::API::TraversalMode::ParallelReduceDepthFirst);
    DUMP("COMPUTE ALPHA START");
    int H(floor(height/Bifrost::API::Layout(alpha.layout()).voxelScale())+2);
    Bifrost::API::Tile root = layout.tileAccessor().tile(0,0,0,0);
    for(int i = 0; i < 5; ++i){
        for(int k = 0; k < 5; ++k){
            crawl(sdf, alpha, root, i, 0, k, H);
        }
    }
    //CrawlVistor crawler(sdf, height, alpha);
    //layout.traverse(crawler, Bifrost::API::TraversalMode::DepthFirst);
    DUMP("COMPUTE ALPHA ENDS");
    DUMP("FLOOD STARTS");
    FloodVistor visitor(sdf, height, alpha);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirstBottomUp, layout.maxDepth(), layout.maxDepth());
    DUMP("FLOOD ENDS");
}

}

namespace Bifrost {
namespace Processing {

void extend(const Bifrost::API::VoxelChannel& _sdf, float height, const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, float radius, Bifrost::API::VoxelChannel& out){
    Bifrost::API::VoxelChannel sdf(_sdf);
    Bifrost::API::Layout layout(sdf.layout());
    std::cerr << sdf.valid() << height << dimensions[0] << radius << out.valid() << std::endl;

    {// add tiles
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
    if(false){// alpha plane
        AlphaVistor visitor(height, alpha);
        layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());
    }else{
        computeAlpha(out, alpha, height, radius);
    }

    if(sdf == out){
        sdf = ss.createChannel(component, Bifrost::API::DataType::FloatType, "tmp");

        //PlaneSdfVistor visitor1(height+layout.voxelScale(), out);
        PlaneSdfVistor visitor1(height, sdf);
        layout.traverse(visitor1, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());

        BlendVisitor<float> visitor2(sdf, out, alpha, out); // sdf = plane, out = new sdf
        layout.traverse(visitor2, Bifrost::API::TraversalMode::ParallelBreadthFirst);

        Bifrost::API::String name = sdf.fullPathName();
        sdf.reset();
        ss.removeChannel(name);
    }else{
        PlaneSdfVistor visitor1(height, out);
        layout.traverse(visitor1, Bifrost::API::TraversalMode::ParallelBreadthFirst, layout.maxDepth(), layout.maxDepth());

        BlendVisitor<float> visitor2(out, sdf, alpha, out);
        layout.traverse(visitor2, Bifrost::API::TraversalMode::ParallelBreadthFirst);
    }

    //Bifrost::API::String name = alpha.fullPathName();
    //alpha.reset();
    //ss.removeChannel(name);
}

void extendUVs(const amino::Math::vec2f &center, const amino::Math::vec2f &dimensions, API::VoxelChannel &out){
    Bifrost::API::Layout layout(out.layout());
    ExtendUVVistor visitor(center-dimensions*.5, center+dimensions*.5, out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::BreadthFirst, layout.maxDepth(), layout.maxDepth());
}

}}
