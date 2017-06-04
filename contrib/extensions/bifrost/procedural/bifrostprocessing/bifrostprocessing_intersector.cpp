#include <bifrostprocessing/bifrostprocessing_intersector.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_layout.h>
#include <vector>
#include "defs.h"

//#define NDEBUG
#ifdef NDEBUG
#define assert_le(x,y)
#define assert_l(x,y)
#define assert_ge(x,y)
#define assert_g(x,y)
#define assert_e(x,y)
#else
#define assert_le(x,y) if((x)>(y)) { std::cerr << __LINE__ << ": " << x << ">" << y << std::endl; assert((x) <= (y)); }
#define assert_l(x,y) if((x)>=(y)) { std::cerr << __LINE__ << ": " << x << ">=" << y << std::endl; assert((x) < (y)); }
#define assert_ge(x,y) if((x)<(y)) { std::cerr << __LINE__ << ": " << x << "<" << y << std::endl; assert((x) >= (y)); }
#define assert_g(x,y) if((x)<=(y)) { std::cerr << __LINE__ << ": " << x << "<=" << y << std::endl; assert((x) > (y)); }
#define assert_e(x,y) if((x)!=(y)) { std::cerr << __LINE__ << ": " << x << "!=" << y << std::endl; assert((x) == (y)); }
#endif
//#define assert(a)
#define MIN(a,b) (a)<(b)?(a):(b)
#define INV(v) (v)==0? std::numeric_limits<float>::max() : 1./(v)

namespace{

typedef Bifrost::Processing::Interval Interval;
//std::ostream& operator <<(std::ostream& out, const Interval& interval){ return out << "Interval(" << interval.t0 << ", " << interval.t1 << ")"; }
std::ostream& operator <<(std::ostream& out, const Bifrost::API::TileCoord& coord){ return out << "Coordl(" << coord.i << ", " << coord.j << ", " << coord.k << ")"; }
struct DDA;
std::ostream& operator <<(std::ostream& out, const DDA& dda);

std::ostream& operator <<(std::ostream& out, const int v[3]){
    std::stringstream ss;
    ss << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    return out << ss.str();
}
std::ostream& operator <<(std::ostream& out, const float v[3]){
    std::stringstream ss;
    ss << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    return out << ss.str();
}

enum{ ZERO = 0, POSITIVE = 1, NEGATIVE = -1 };
struct Ray{
    amino::Math::vec3f org, dir, invDir;
    amino::Math::vec3i sign;
    Ray(){}
    Ray(const amino::Math::vec3f& org, const amino::Math::vec3f& dir)
        : org(org), dir(dir), invDir(INV(dir[0]),INV(dir[1]),INV(dir[2])){
        for(unsigned int i = 0; i < 3; ++i)
            sign[i] = dir[i] > 0? POSITIVE : dir[i] < 0? NEGATIVE : ZERO;
    }
    Ray(const Ray& o) : org(o.org), dir(o.dir), invDir(o.invDir), sign(o.sign){}
    inline amino::Math::vec3f operator()(float t) const{ return org + t*dir; }
};

inline int minIndex(const float v[3]){
    if(v[2] < v[0] && v[2] < v[1]) return 2;
    if(v[1] < v[0]) return 1;
    return 0;
}

struct DDA
{
    DDA() : DIM(0), t0(0), t1(0) {}
    DDA(int DIM, int OFFSET) : DIM(DIM), OFFSET(OFFSET), t0(0), t1(0) { assert(DIM > 0); }
    DDA(const DDA& o)  : DIM(o.DIM), OFFSET(o.OFFSET), t0(0), t1(0){ assert(DIM > 0); }

    inline void init(const Ray &ray){
        t0 = t1 = -1;
        this->ray = ray;
        const amino::Math::vec3f &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i) {
            switch(ray.sign.v[i]){
            case ZERO:     _step[i] =    0; break; // dummy value
            case POSITIVE: _step[i] =  DIM; break;
            case NEGATIVE: _step[i] = -DIM; break;
            }
            _delta[i] = _step[i]? (_step[i] * inv.v[i]) : std::numeric_limits<float>::max();
            assert(_delta[i] >= 0);
        }
    }

    inline void update(float t0, float t1){
        this->t0 = t0; this->t1 = t1;
        assert_l(t0, t1);
        const amino::Math::vec3f &pos = ray(t0), &inv = ray.invDir;
        for(unsigned int i = 0; i < 3; ++i){
            _voxel[i] = (int)floor(pos.v[i]) - OFFSET;
        }
        assert_g(DIM, 0);
        if(DIM != 1){
            for(unsigned int i = 0; i < 3; ++i){
                if(_voxel[i] >= 0) _voxel[i] -= _voxel[i] % DIM;
                else _voxel[i] -= (DIM + (_voxel[i] % DIM)) % DIM;
            }
        }
        for(unsigned int i = 0; i < 3; ++i){
            _voxel[i] += OFFSET;
        }
        for(unsigned int i = 0; i < 3; ++i) {
            switch(ray.sign.v[i]){
            case ZERO:     _next[i] = std::numeric_limits<float>::max(); break;
            case POSITIVE: _next[i] = (_voxel[i] - pos.v[i] + DIM) * inv.v[i]; break;
            case NEGATIVE: _next[i] = (_voxel[i] - pos.v[i]) * inv.v[i]; break;
            }
            if(_next[i] < 0) DUMP(*this);
            _next[i] += t0;
            assert_ge(_next[i], t0);
        }

    }

    inline bool done() const{ return t0 >= t1; }
    inline void step() {
        const int stepAxis = minIndex(_next);
        t0 = _next[stepAxis];
        _next[stepAxis]  += _delta[stepAxis];
        _voxel[stepAxis] += _step[stepAxis];
    }
    inline Bifrost::API::TileCoord voxel() const{ return Bifrost::API::TileCoord(_voxel[0], _voxel[1], _voxel[2]); }
    inline float time() const { return t0; }
    inline float next() const { return MIN(MIN(t1, _next[0]), MIN(_next[1], _next[2])); }

    Ray ray;
    float t0, t1;
    int DIM, OFFSET;
    int _voxel[3], _step[3];
    float _delta[3], _next[3];
};

std::ostream& operator <<(std::ostream& out, const DDA& dda){
    std::stringstream ss;
    ss << "DDA( DIM = " << dda.DIM << ", OFFSET = " << dda.OFFSET << " (" << dda.t0 << ", " << dda.t1 << ")" << std::endl;
    ss << "     orig = " << dda.ray.org << std::endl;
    ss << "      dir = " << dda.ray.dir << std::endl;
    ss << "    voxel = " << dda._voxel << std::endl;
    ss << "     next = " << dda._next << std::endl;
    ss << "    delta = " << dda._delta << std::endl;
    ss << "     step = " << dda._step << std::endl;
    ss << ")";
    return out << ss.str();
}

void test(const DDA* dda, float dx){
    Ray ray(dda->ray.org*dx, dda->ray.dir*dx); // world space
    amino::Math::vec3f start(ray(dda->time())), end(ray(dda->next()));
    std::cerr << "setAttr pSphere6.translate " << start.v[0] << " " << start.v[1] << " " << start.v[2] << "; "
              << "setAttr pSphere7.translate " << end.v[0] << " " << end.v[1] << " " << end.v[2] << "; // " << dda->DIM << ", " << dda->time() << std::endl;
}

struct IntersectorImpl{
    IntersectorImpl(const Bifrost::API::Layout& layout)
        : invDx(1./layout.voxelScale()), accessor(layout.tileAccessor()), index(-1) {
        ddas.resize(layout.maxDepth());
        int tileWidth = layout.tileDimInfo().tileWidth;
        int DIM = tileWidth; // minimum stepping
        for(int i = (int) ddas.size()-1; i >= 0; --i, DIM *= tileWidth)
            ddas[i] = DDA(DIM, -(DIM>>1));
    }
    IntersectorImpl(const IntersectorImpl& o)
        : invDx(o.invDx), accessor(o.accessor), index(-1){
        ddas.reserve(o.ddas.size());
        for(const DDA& dda : o.ddas)
            ddas.push_back(dda);
    }

    inline void init(const amino::Math::vec3f& org, const amino::Math::vec3f& dir, float t0, float t1, bool debug=false){
        Ray ray(org*invDx, dir*invDx);
        //Ray ray(amino::Math::vec3f(2.5,2.5,2.5), amino::Math::vec3f(1,1,1));
        ray.org += amino::Math::vec3f(.5);
        this->t0 = t0; this->t1 = t1;
        index = ddas.size()-1;
        for(DDA& dda : ddas)
            dda.init(ray);
        ddas[index].update(t0,t1);
        count = 0;
        this->debug = debug;
        this->ray = ray;
    }

    inline Bifrost::API::TileCoord nextVoxel(){
        DDA& last = ddas[ddas.size()-1];
        if(index==(int)ddas.size()-1) return last.voxel();
        if(last.time() < t0){
            last.update(t0+EPS,t1);
            last.step();
        }
        return last.voxel();
    }

    inline Interval next(){
        Interval current;
        if(index < 0) return current;

        const int maxDepth = ddas.size();
        int depth;
        DDA* dda = &ddas[index];
        bool loop = true;
        while(loop && !dda->done()){
            assert_e(t0, dda->time());
            depth = accessor.index(nextVoxel(), maxDepth).depth;

            if(depth == maxDepth){
                if(!current.valid()) {
                    if(!mute) DUMP("!!!!!!!!!!!!!!!!!!!!");
                    current.t0 = ddas[ddas.size()-1].time();
                }
                current.t1 = dda->next();
            }else if(current.valid()){
                loop = false;
            }
            if(depth > maxDepth-1) depth = maxDepth-1;

            if(!debug && depth != index){
                assert_e(dda->time(), t0);
                assert(dda->done() || dda->next() >= dda->time());
                dda->step();
                index = depth;
                if(index < 0) break; // outside tiles region
                if(debug) index = 6;
                dda = &ddas[index];
                //assert_le(dda->time(), t0);
                if(dda->time() < t0){
                    dda->update(t0, t1);
                }
            }
            assert(dda->done() || dda->next() >= dda->time());
            dda->step();
            assert(dda->done() || dda->time() >= t0);
            t0 = dda->time();
            if(!mute) ++count;
        }
        //if(!mute) DUMP(count);
        return current;
    }

    Ray ray;
    unsigned int count;
    float t0, t1;
    int index;
    float invDx;
    Bifrost::API::TileAccessor accessor;
    std::vector<DDA> ddas;
    bool mute = true;
    bool debug = false;
};

}

namespace Bifrost{
namespace Processing{

Intersector::Intersector(const Bifrost::API::Layout &layout)
    : impl(new IntersectorImpl(layout)){
    return;
    float t0 = 0, t1 = 1;
    //*
    //TODO: debug the offset
    Interval current;

    amino::Math::vec3f orig(-39.43, 18.29, -5.46);
    amino::Math::vec3f end(-12.76,-12.74,-16.13);
    //amino::Math::vec3f orig(5.10,-8.03,48.56);
    //amino::Math::vec3f end(13.38, -1.08, -4.41);
    //amino::Math::vec3f orig(1.88, -8.82, -29.14);
    //amino::Math::vec3f end(6.38, 10.30,21.22);
    //amino::Math::vec3f orig(0.59, 7.37, -30.36);
    //amino::Math::vec3f end(7.67, -1.82, 22.44);

    amino::Math::vec3f dir = end-orig;

    orig = amino::Math::vec3f(-27.2573, 34.2993, -0.472827);
    dir = amino::Math::vec3f(0.566183, -0.817673, -0.104153);
    t0 = 33.2643;
    t1 = 55.278;
    end = orig + t1*dir;
    if(false){
        amino::Math::vec3f tmp; tmp = orig; orig = end; end = tmp; dir=-dir;
        t0 = 0;
        orig -= 30*dir;
    }


    IntersectorImpl impl(*static_cast<IntersectorImpl*>(this->impl));
    impl.mute = true;
    impl.init(orig, dir,t0,t1);
    while((current = impl.next()).valid());
    impl.mute = true;
    impl.init(orig, dir,t0,t1,false);
    Ray ray(orig, dir); // world space
    DUMP("***********");
    while((current = impl.next()).valid()){
        amino::Math::vec3f start(ray(current.t0)), end(ray(current.t1));
        std::cerr << "setAttr pSphere6.translate " << start.v[0] << " " << start.v[1] << " " << start.v[2] << "; "
                  << "setAttr pSphere7.translate " << end.v[0] << " " << end.v[1] << " " << end.v[2] << "; " << std::endl;
        amino::Math::vec3f ab = end-start;
        float dist = sqrt(ab[0]*ab[0] + ab[1]*ab[1] + ab[2]*ab[2]);
        DUMP(dist);
    }
    DUMP(end);
    DUMP("***********");
    Bifrost::API::TileAccessor accessor(layout.tileAccessor());
    int ofs = 2;
    int tile = accessor.index(-ofs,-25-ofs,-ofs,7).tile;
    FOR_IJK(i,j,k,5){
        Bifrost::API::TreeIndex index = accessor.index(i-ofs,j-25-ofs,k-ofs,7);
        assert(index.depth==7);
        assert(index.tile == tile);
    }
    //*/
}

Intersector::Intersector(const Bifrost::Processing::Intersector &o)
    : impl(new IntersectorImpl(*static_cast<IntersectorImpl*>(o.impl))) {}

Intersector::~Intersector(){
    if(impl) delete static_cast<IntersectorImpl*>(impl);
}

void Intersector::init(const amino::Math::vec3f &origin, const amino::Math::vec3f &direction, float t0, float t1, bool debug){
    static_cast<IntersectorImpl*>(impl)->init(origin, direction, t0, t1, debug);
}

//Interval Intersector::next(){ return Interval(); }
Interval Intersector::next(){ return impl? static_cast<IntersectorImpl*>(impl)->next() : Interval(); }

}} // Bifrost::Processing
