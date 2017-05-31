#include <bifrostprocessing/bifrostprocessing_intersector.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_layout.h>
#include "defs.h"

#define MIN(a,b) (a)<(b)?(a):(b)

namespace{

typedef Bifrost::Processing::Interval Interval;

struct Ray{
    amino::Math::vec3f org, dir, invDir;
    Ray(){}
    Ray(const amino::Math::vec3f& org, const amino::Math::vec3f& dir)
        : org(org), dir(dir), invDir(1./dir[0],1./dir[1],1./dir[2]){}
    amino::Math::vec3f operator()(float t) const{ return org + t*dir; }
};

inline int minIndex(const float v[3]){
    if(v[2] < v[0] && v[2] < v[1]) return 2;
    if(v[1] < v[0]) return 1;
    return 0;
}

class DDA
{
public:
    DDA(int DIM) : DIM(DIM), t0(0), t1(0) { assert(DIM != 0); }

    inline void init(const Ray& ray, float tmin, float tmax){
        t0 = tmin; t1 = tmax;
        const amino::Math::vec3f &pos = ray(t0), &dir = ray.dir, &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i)
            _voxel[i] = floor(pos.v[i]);
        /*
        if(DIM != 1){
            for(unsigned int i = 0; i < 3; ++i)
                if(_voxel[i] > 0) _voxel[i] -= _voxel[i] % DIM;
                else _voxel[i] += (_voxel[i] % DIM);
        }
        */
        for (int axis = 0; axis < 3; ++axis) {
            if ( dir.v[axis] < 1e-20 && dir.v[axis] > -1e-20) { //handles dir = +/- 0
                _step[axis]  = 0; //dummy value
                _next[axis]  = std::numeric_limits<float>::max(); //i.e. disabled!
                _delta[axis] = std::numeric_limits<float>::max(); //dummy value
            } else if( inv[axis] > 0) {
                _step[axis]  = DIM;
                _next[axis]  = t0 + (_voxel[axis] + DIM - pos.v[axis]) * inv.v[axis];
                _delta[axis] = _step[axis] * inv.v[axis];
            } else {
                _step[axis]  = -DIM;
                _next[axis]  = t0 + (_voxel[axis] - pos.v[axis]) * inv.v[axis];
                _delta[axis] = _step[axis] * inv.v[axis];
            }
        }
    }
    inline bool done() const{ return t0 >= t1; }
    inline void terminate(){ t0 = t1; }
    inline bool step() {
        const int stepAxis = minIndex(_next);
        t0 = _next[stepAxis];
        _next[stepAxis]  += _delta[stepAxis];
        _voxel[stepAxis] += _step[stepAxis];
        return !done();
    }
    inline const int* voxel() const { return _voxel; }
    inline float time() const { return t0; }
    inline float maxTime() const { return t1;  }
    inline float next() const { return MIN(MIN(t1, _next[0]), MIN(_next[1], _next[2])); }
private:
    float t0, t1;
    int DIM;
    int _voxel[3], _step[3];
    float _delta[3], _next[3];
};

inline int powi(int n, int exp){
    int v = 1;
    while(exp-- > 0) v *= n;
    return v;
}

class HDDA{
public:
    HDDA(Bifrost::API::TileAccessor& accessor, int tileWidth, int maxDepth, int depth)
        : depth(depth), accessor(accessor),
          dda(powi(tileWidth, maxDepth-depth)),
          hdda((depth==maxDepth)? nullptr : new HDDA(accessor, tileWidth, maxDepth, depth+1)){}
    ~HDDA(){ if(hdda) delete hdda; }

    void init(const Ray &ray, float t0, float t1){
        this->terminate();
        this->ray = ray;
        dda.init(ray, t0, t1);
    }

    Interval next(Interval current=Interval()){
        if(this->done()){
            return current;
        }
        if(isLeaf()){
            if(current.t0 < 0) current.t0 = dda.time();
            current.t1 = dda.maxTime();
            dda.terminate();
            return current;
        }
        if(!hdda->done()){
            current = hdda->next(current);
            if(hdda->done()) dda.step();
            if(current.valid() && current.t1+EPS < dda.time()) return current;
        }
        if(dda.done()) return current;
        do{
            if(dda.time() >= dda.next()) continue;
            assert(current.t0 <= dda.time());
            int d = accessor.index(dda.voxel()[0], dda.voxel()[1], dda.voxel()[2], depth).depth;
            if(d < depth){ // tile doesn't exist at the given depth
                if(current.valid()){
                    dda.step();
                    return current;
                }
                continue;
            }
            // found tile at expected depth
            hdda->init(ray, dda.time(), dda.next());
            current = hdda->next(current); // give child current interval to continue
            if(current.valid() && current.t1+EPS < dda.time()){
                dda.step();
                return current;
            }
        }while(dda.step());

        return current;
    }

    inline void terminate(){ dda.terminate(); if(!isLeaf()) hdda->terminate(); }
    inline bool done() const{ return dda.done(); }

public:
    inline bool isLeaf() const{ return hdda == nullptr; }

    Bifrost::API::TileAccessor& accessor;
    Ray ray;
    int depth;
    DDA dda; // one depth further
    HDDA* hdda; // one depth further
};

struct IntersectorImpl{
    IntersectorImpl(const Bifrost::API::Layout& layout, const amino::Math::vec3f &origin, const amino::Math::vec3f& direction, float t0, float t1){
        float invDx = 1./layout.voxelScale();
        int n = layout.tileDimInfo().tileWidth;
        int dist = (int) ((t1-t0)*invDx)/n;
        int depth = layout.maxDepth();
        while((dist /= n) > 0){ --depth; }
        accessor = layout.tileAccessor();
        hdda = new HDDA(accessor, layout.tileDimInfo().tileWidth, layout.maxDepth(), depth);
        hdda->init(Ray(origin*invDx, direction*invDx), t0, t1);
    }
    ~IntersectorImpl(){
        accessor.reset();
        delete hdda;
    }

    inline bool done() const{ return hdda->done(); }
    inline Interval next(){ return hdda->next(); }

    HDDA* hdda;
    Bifrost::API::TileAccessor accessor;
};

}

namespace Bifrost{
namespace Processing{

Intersector::Intersector(const Bifrost::API::Layout& layout, const amino::Math::vec3f &origin, const amino::Math::vec3f& direction, float t0, float t1)
    : impl(new IntersectorImpl(layout, origin, direction, t0, t1)){}
Intersector::~Intersector(){
    delete static_cast<IntersectorImpl*>(impl);
}

bool Intersector::done() const{ return static_cast<IntersectorImpl*>(impl)->done(); }
Interval Intersector::next(){ return static_cast<IntersectorImpl*>(impl)->next(); }

}} // Bifrost::Processing
