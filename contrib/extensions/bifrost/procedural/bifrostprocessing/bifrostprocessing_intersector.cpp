#include <bifrostprocessing/bifrostprocessing_intersector.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_layout.h>
#include <vector>
#include "defs.h"

#define MIN(a,b) (a)<(b)?(a):(b)
#define INV(v) (v)==0? std::numeric_limits<float>::max() : 1./(v)

namespace{

typedef Bifrost::Processing::Interval Interval;
//std::ostream& operator <<(std::ostream& out, const Interval& interval){ return out << "Interval(" << interval.t0 << ", " << interval.t1 << ")"; }

struct Ray{
    amino::Math::vec3f org, dir, invDir;
    Ray(){}
    Ray(const amino::Math::vec3f& org, const amino::Math::vec3f& dir)
        : org(org), dir(dir), invDir(INV(dir[0]),INV(dir[1]),INV(dir[2])){}
    Ray(const Ray& o) : org(o.org), dir(o.dir), invDir(o.invDir){}
    inline amino::Math::vec3f operator()(float t) const{ return org + t*dir; }
};

inline int minIndex(const float v[3]){
    if(v[2] < v[0] && v[2] < v[1]) return 2;
    if(v[1] < v[0]) return 1;
    return 0;
}

class DDA
{
public:
    DDA() : DIM(0), t0(0), t1(0) {}
    DDA(int DIM) : DIM(DIM), t0(0), t1(0) { assert(DIM > 0); }
    DDA(const DDA& o)  : DIM(o.DIM), t0(0), t1(0){ assert(DIM > 0); }

    inline void init(const Ray &ray){
        t0 = t1 = -1;
        this->ray = ray;
        const amino::Math::vec3f &dir = ray.dir, &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i) {
            if(dir.v[i] == 0) _states[i] = ZERO; //handles dir = +/- 0
            else if(inv[i] > 0) _states[i] = POSITIVE;
            else _states[i] = NEGATIVE;
        }
        for(unsigned int i = 0; i < 3; ++i) {
            switch(_states[i]){
            case ZERO:     _step[i] =    0; break; // dummy value
            case POSITIVE: _step[i] =  DIM; break;
            case NEGATIVE: _step[i] = -DIM; break;
            }
            _delta[i] = _step[i]? (_step[i] * inv.v[i]) : std::numeric_limits<float>::max();
        }
    }

    inline void update(float t0, float t1){
        this->t0 = t0; this->t1 = t1;
        const amino::Math::vec3f &pos = ray(t0), &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i){
            _voxel[i] = (int)floor(pos.v[i]);
        }
        assert(DIM > 0);
        if(DIM != 1){
            for(unsigned int i = 0; i < 3; ++i){
                if(_voxel[i] >= 0) _voxel[i] -= _voxel[i] % DIM;
                else _voxel[i] -= (DIM + (_voxel[i] % DIM)) % DIM;
            }
        }
        for(unsigned int i = 0; i < 3; ++i) {
            switch(_states[i]){
            case ZERO:     _next[i] = std::numeric_limits<float>::max(); break;
            case POSITIVE: _next[i] = t0 + (_voxel[i] - pos.v[i] + DIM) * inv.v[i]; break;
            case NEGATIVE: _next[i] = t0 + (_voxel[i] - pos.v[i]) * inv.v[i]; break;
            }
        }
    }

    inline bool done() const{ return t0 >= t1; }
    inline bool step() {
        const int stepAxis = minIndex(_next);
        t0 = _next[stepAxis];
        _next[stepAxis]  += _delta[stepAxis];
        _voxel[stepAxis] += _step[stepAxis];
        return !done();
    }
    inline Bifrost::API::TileCoord coord() const{ return Bifrost::API::TileCoord(_voxel[0], _voxel[1], _voxel[2]); }
    inline float time() const { return t0; }
    inline float next() const { return MIN(MIN(t1, _next[0]), MIN(_next[1], _next[2])); }
private:
    Ray ray;
    float t0, t1;
    int DIM;
    enum{ ZERO, POSITIVE, NEGATIVE } _states[3];
    int _voxel[3], _step[3];
    float _delta[3], _next[3];
};

struct IntersectorImpl{
    IntersectorImpl(const Bifrost::API::Layout& layout)
        : invDx(1./layout.voxelScale()), accessor(layout.tileAccessor()), index(-1) {
        ddas.resize(layout.maxDepth());
        int tileWidth = layout.tileDimInfo().tileWidth;
        int DIM = tileWidth;
        for(int i = (int) ddas.size()-1; i >= 0; --i){
            ddas[i] = DDA(DIM);
            DIM *= tileWidth;
        }
    }
    IntersectorImpl(const IntersectorImpl& o)
        : invDx(o.invDx), accessor(o.accessor), index(-1){
        ddas.reserve(o.ddas.size());
        for(const DDA& dda : o.ddas){
            ddas.push_back(dda);
        }
    }

    inline void init(const amino::Math::vec3f& org, const amino::Math::vec3f& dir, float t0, float t1){
        Ray ray(org*invDx, dir*invDx);
        this->t0 = t0; this->t1 = t1;
        index = ddas.size()-1;
        for(DDA& dda : ddas)
            dda.init(ray);
        ddas[index].update(t0,t1);
    }

    inline Interval next(){
        Interval current;
        if(index < 0) return current;

        const int maxDepth = ddas.size();
        int depth;
        DDA* dda = &ddas[index];
        bool loop = true;
        while(loop && t0 < t1){
            depth = accessor.index(dda->coord(), maxDepth).depth;
            if(depth == maxDepth){
                if(!current.valid()) current.t0 = t0;
                current.t1 = dda->next();
            }else if(current.valid()){
                loop = false;
            }
            if(depth > maxDepth-1) depth = maxDepth-1;
            while(dda->time() <= t0)
                dda->step();
            if(depth != index){
                index = depth;
                if(index < 0) break; // outside tiles region
                dda = &ddas[index];
                if(dda->time() != t0)
                    dda->update(t0, t1);
                while(dda->time() <= t0)
                    dda->step();
            }
            t0 = dda->time();
        }
        return current;
    }

    float t0, t1;
    int index;
    float invDx;
    Bifrost::API::TileAccessor accessor;
    std::vector<DDA> ddas;
};

}

namespace Bifrost{
namespace Processing{

Intersector::Intersector(const Bifrost::API::Layout &layout)
    : impl(new IntersectorImpl(layout)){}

Intersector::Intersector(const Bifrost::Processing::Intersector &o)
    : impl(new IntersectorImpl(*static_cast<IntersectorImpl*>(o.impl))) {}

Intersector::~Intersector(){
    if(impl) delete static_cast<IntersectorImpl*>(impl);
}

void Intersector::init(const amino::Math::vec3f &origin, const amino::Math::vec3f &direction, float t0, float t1){
    static_cast<IntersectorImpl*>(impl)->init(origin, direction, t0, t1);
    /*
    TODO: debug the offset
    Interval current;
    amino::Math::vec3f orig(1.88, -8.82, -29.14);
    amino::Math::vec3f end(6.38, 10.30,21.22);
    //amino::Math::vec3f orig(0.59, 7.37, -30.36);
    //amino::Math::vec3f end(7.67, -1.82, 22.44);
    amino::Math::vec3f dir = end-orig;
    t0 = 0; t1 = 1;
    DUMP("***********");
    IntersectorImpl impl(*static_cast<IntersectorImpl*>(this->impl));
    impl.init(orig, dir,t0,t1);
    const Ray& ray = impl.ddas[0].ray;
    while((current = impl.next()).valid()){
        amino::Math::vec3f start(ray(current.t0)), end(ray(current.t1));
        std::cerr << "setAttr pSphere6.translate " << start.v[0] << " " << start.v[1] << " " << start.v[2] << "; "
                  << "setAttr pSphere7.translate " << end.v[0] << " " << end.v[1] << " " << end.v[2] << "; " << std::endl;
    }
    //*/
}

Interval Intersector::next(){ return impl? static_cast<IntersectorImpl*>(impl)->next() : Interval(); }

}} // Bifrost::Processing
