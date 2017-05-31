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

    inline void init(const Ray &ray){
        t0 = t1 = -1;
        this->ray = ray;
        const amino::Math::vec3f &dir = ray.dir, &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i) {
            if(dir.v[i] < 1e-20 && dir.v[i] > -1e-20) _states[i] = ZERO; //handles dir = +/- 0
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
        for(unsigned int i = 0; i < 3; ++i)
            _voxel[i] = floor(pos.v[i]);
        /*if(DIM != 1){
            for(unsigned int i = 0; i < 3; ++i)
                if(_voxel[i] > 0) _voxel[i] -= _voxel[i] % DIM;
                else _voxel[i] += (_voxel[i] % DIM);
        }*/
        for(unsigned int i = 0; i < 3; ++i) {
            switch(_states[i]){
            case ZERO:     _next[i] = std::numeric_limits<float>::max(); break;
            case POSITIVE: _next[i] = t0 + (_voxel[i] - pos.v[i] + DIM) * inv.v[i]; break;
            case NEGATIVE: _next[i] = t0 + (_voxel[i] - pos.v[i]) * inv.v[i]; break;
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
    inline float next() const { return MIN(MIN(t1, _next[0]), MIN(_next[1], _next[2])); }
private:
    Ray ray;
    float t0, t1;
    int DIM;
    enum{ ZERO, POSITIVE, NEGATIVE } _states[3];
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
    HDDA(const Bifrost::API::TileAccessor& accessor, int DIM, int tileWidth, int maxDepth, int depth)
        : depth(depth), accessor(accessor), dda(DIM),
          hdda((depth==maxDepth)? nullptr : new HDDA(accessor, DIM/tileWidth, tileWidth, maxDepth, depth+1)){}
    ~HDDA(){ if(hdda) delete hdda; }

    void update(float t0, float t1){
        this->terminate();
        _current = isLeaf()? Interval(t0,t1) : Interval();
        dda.update(t0,t1);
    }
    void init(const Ray &ray){
        if(isLeaf()) return;
        this->terminate();
        dda.init(ray);
        hdda->init(ray);
    }
    void init(const Ray &ray, float t0, float t1){
        init(ray);
        update(t0,t1);
    }

    Interval next(){
        Interval current = _current;
        _current = Interval();
        if(isLeaf()) return current;

        Interval child;
        while(!done() || !hdda->done()){
            if(!dda.done() && dda.time() >= dda.next()) { dda.step(); continue; }
            while((child = hdda->next()).valid()){
                if(!current.valid()) {
                    current = child;
                }else if(current.t1 + EPS >= child.t0) { // extend current
                    current.t1 = child.t1;
                }else { // current is valid but not extendable by child => keep it for next time.
                    _current = child;
                    return current;
                }
            }
            if(accessor.index(dda.voxel()[0], dda.voxel()[1], dda.voxel()[2], depth).depth >= depth){ // tile doesn't exist at the given depth
                hdda->update(dda.time(), dda.next());
            }
            dda.step();
        }
        return current;
    }

    inline void terminate(){ dda.terminate(); if(!isLeaf()) hdda->terminate(); }
    inline bool done() const{ return isLeaf()? !_current.valid() : dda.done(); }

public:
    inline bool isLeaf() const{ return hdda == nullptr; }

    const Bifrost::API::TileAccessor& accessor;
    Interval _current;
    int depth;
    DDA dda;    // one depth further
    HDDA* hdda; // one depth further
};

}

namespace Bifrost{
namespace Processing{

Intersector::Intersector(const Bifrost::API::Layout &layout)
    : tileWidth(layout.tileDimInfo().tileWidth), maxDepth(layout.maxDepth()), invDx(1./layout.voxelScale()), accessor(layout.tileAccessor()), hdda(nullptr){}

Intersector::Intersector(const Bifrost::Processing::Intersector &o)
    : tileWidth(o.tileWidth), maxDepth(o.maxDepth), invDx(o.invDx), accessor(o.accessor), hdda(nullptr){
}
Intersector::~Intersector(){
    if(hdda) delete static_cast<HDDA*>(hdda);
}

void Intersector::init(const amino::Math::vec3f &origin, const amino::Math::vec3f &direction, float t0, float t1){
    int dist = (int) ((t1-t0)*invDx)/tileWidth;
    int depth = maxDepth-1;
    while((dist /= tileWidth) > 0){ --depth; }
    int DIM = powi(tileWidth, maxDepth-depth);
    if(!hdda || static_cast<HDDA*>(hdda)->depth != depth){
        if(hdda) delete static_cast<HDDA*>(hdda);
        hdda = new HDDA(accessor, DIM, tileWidth, maxDepth, depth);
    }
    static_cast<HDDA*>(hdda)->init(Ray(origin*invDx, direction*invDx), t0, t1);
}

Interval Intersector::next(){ return hdda? static_cast<HDDA*>(hdda)->next() : Interval(); }

}} // Bifrost::Processing
