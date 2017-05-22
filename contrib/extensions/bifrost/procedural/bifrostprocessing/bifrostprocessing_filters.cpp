#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_mixer.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_om.h>
#include "defs.h"
#include <vector>

#define SWAP(ch1, ch2) { API::Channel _tmp = ch1; ch1 = ch2; ch2 = _tmp; }

namespace{

template<typename T> inline T sqrt(const T& v){ return std::sqrt(v); }
template<> inline amino::Math::vec2f sqrt<amino::Math::vec2f>(const amino::Math::vec2f& v){ return amino::Math::vec2f(std::sqrt(v[0]), std::sqrt(v[1])); }
template<> inline amino::Math::vec2i sqrt<amino::Math::vec2i>(const amino::Math::vec2i& v){ return amino::Math::vec2i((int)std::sqrt(v[0]), (int)std::sqrt(v[1])); }
template<> inline amino::Math::vec3f sqrt<amino::Math::vec3f>(const amino::Math::vec3f& v){ return amino::Math::vec3f(std::sqrt(v[0]), std::sqrt(v[1]), std::sqrt(v[2])); }
template<> inline amino::Math::vec3i sqrt<amino::Math::vec3i>(const amino::Math::vec3i& v){ return amino::Math::vec3i((int)std::sqrt(v[0]), (int)std::sqrt(v[1]), (int)std::sqrt(v[2])); }

template<typename T>
class TransformVisitor : public Bifrost::API::Visitor {
public:
    TransformVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const Bifrost::Processing::TransformFilter<T>& filter)
        : in(in), out(out), filter(filter){}
    TransformVisitor(const TransformVisitor& o)
        : in(o.in), out(o.out), filter(o.filter) {}
    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) override{
        const Bifrost::API::TileData<T>& inData = in.tileData<T>(index);
        Bifrost::API::TileData<T> outData = out.tileData<T>(index);
        for(size_t k = 0; k < inData.count(); ++k) outData[k] = filter.compute(inData[k]);
    }
    Bifrost::API::Visitor* copy() const override{ return new TransformVisitor<T>(*this); }

private:
    const Bifrost::API::Channel in, out;
    const Bifrost::Processing::TransformFilter<T>& filter;
};

template<typename T>
class HaloVisitor : public Bifrost::API::Visitor {
public:
    HaloVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const Bifrost::Processing::HaloFilter<T>& filter)
        : in(in), out(out), filter(filter){
        assert(in != out);
        assert(in.layout() == out.layout());
        _haloCache.init(in);
    }
    HaloVisitor(const HaloVisitor& o)
        : in(o.in), out(o.out), filter(o.filter) { _haloCache.init(in); }
    Bifrost::API::Visitor* copy() const override{ return new HaloVisitor<T>(*this); }
    void beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index) override{
        const typename Bifrost::API::HaloedCache<T>::Tile& halo = this->_haloCache.tile(index);
        typename Bifrost::API::TileData<T> out = this->out.template tileData<T>(index);
        const int w = Bifrost::API::Layout(this->in.layout()).tileDimInfo().tileWidth;
        this->_haloCache.update(index);
        for(int i = 0; i < w; ++i)
            for(int j = 0; j < w; ++j)
                for(int k = 0; k < w; ++k)
                    out(i,j,k) = filter.compute(halo, i, j, k);
    }

protected:
    const Bifrost::API::Channel in, out;
    Bifrost::API::HaloedCache<T> _haloCache;
    const Bifrost::Processing::HaloFilter<T>& filter;
};

}

namespace Bifrost{
namespace Processing{

void Filter::filter(API::Channel &inOut) const{
    filter(inOut, inOut);
}
void Filter::setTraversalMode(API::TraversalMode mode){ _mode = mode; }
Bifrost::API::TraversalMode Filter::traversalMode() const{ return _mode; }
void Filter::setDepths(int min, int max){ _depths = amino::Math::vec2i(min, max); }
const amino::Math::vec2i& Filter::depths() const{ return _depths; }

template<typename T>
void TransformFilter<T>::filter(const API::Channel &in, API::Channel &out) const{
    TransformVisitor<T> visitor(in,out,*this);
    if(depths()[0] > 0 && depths()[1] > 0){
        Bifrost::API::Layout(in.layout()).traverse(visitor, traversalMode(), depths()[0], depths()[1]);
    }else{
        Bifrost::API::Layout(in.layout()).traverse(visitor, traversalMode());
    }
}

template<typename T>
T IdentityFilter<T>::compute(const T &in) const{
    return in;
}

template<typename T>
OffsetFilter<T>::OffsetFilter(const T &offset) : offset(offset) {}

template<typename T>
T OffsetFilter<T>::compute(const T& in) const{
    return in + offset;
}

template<typename T>
DilateFilter<T>::DilateFilter(const T& amount) : OffsetFilter<T>(-amount){}

template<typename T>
ErodeFilter<T>::ErodeFilter(const T& amount) : OffsetFilter<T>(amount){}

template<typename T>
ScaleFilter<T>::ScaleFilter(const T &scale) : scale(scale){}

template<typename T>
T ScaleFilter<T>::compute(const T& in) const{
    return in*scale;
}

template<typename T>
RemapFilter<T>::RemapFilter(const T &inMin, const T &inMax, const T &min, const T &max)
    : inMin(inMin), min(min), dir((max-min)/(inMax-inMin)) {}

template<typename T>
T RemapFilter<T>::compute(const T &in) const{
    return (in - inMin)*dir + min;
}

template<typename T>
void HaloFilter<T>::filter(const API::Channel &in, API::Channel &out) const{
    HaloVisitor<T> visitor(in,out,*this);
    if(depths()[0] > 0 && depths()[1] > 0){
        Bifrost::API::Layout(in.layout()).traverse(visitor, traversalMode(), depths()[0], depths()[1]);
    }else{
        Bifrost::API::Layout(in.layout()).traverse(visitor, traversalMode());
    }
}

template<typename T>
T MeanFilter<T>::compute(const typename API::HaloedCache<T>::Tile &in, int i, int j, int k) const{
    static float s = 1./27.;
    T out(0);
    for(int ii = -1; ii <= 1; ++ii)
        for(int jj = -1; jj <= 1; ++jj)
            for(int kk = -1; kk <= 1; ++kk)
                out += in(i+ii,j+jj,k+kk);
    return out*s;
}

template<typename T>
T MedianFilter<T>::compute(const typename API::HaloedCache<T>::Tile &in, int i, int j, int k) const{
    std::vector<T> tmp;
    tmp.reserve(27); // (-1,0,1)^3
    for(int ii = -1; ii <= 1; ++ii)
        for(int jj = -1; jj <= 1; ++jj)
            for(int kk = -1; kk <= 1; ++kk)
                tmp.push_back(in(i+ii,j+jj,k+kk));

    size_t midpoint = (tmp.size() - 1) >> 1;
    // Partially sort the vector until the median value is at the midpoint.
    std::nth_element(tmp.begin(), tmp.begin() + midpoint, tmp.end());

    return tmp[midpoint];
}

template<typename T>
T LaplacianFilter<T>::compute(const typename API::HaloedCache<T>::Tile &in, int i, int j, int k) const{
    static float s = 1./6.;
    return in(i,j,k) + s * (-6 * in(i,j,k) + in(i-1,j,k) + in(i+1,j,k) + in(i,j-1,k) + in(i,j+1,k) + in(i,j,k-1) + in(i,j,k+1));
}

template<typename T>
T CurvatureFilter<T>::compute(const typename API::HaloedCache<T>::Tile &in, int i, int j, int k) const{
    static T s(1./3.);
    T stencil[19] = {
        in( i+0, j+0, k+0),
        in( i-1, j+0, k+0), in( i+1, j+0, k+0),
        in( i+0, j-1, k+0), in( i+0, j+1, k+0),
        in( i+0, j+0, k-1), in( i+0, j+0, k+1),
        in( i-1, j-1, k+0), in( i+1, j-1, k+0), in( i-1, j+1, k+0), in( i+1, j+1, k+0),
        in( i-1, j+0, k-1), in( i+1, j+0, k-1), in( i-1, j+0, k+1), in( i+1, j+0, k+1),
        in( i+0, j-1, k-1), in( i+0, j+1, k-1), in( i+0, j-1, k+1), in( i+0, j+1, k+1)
    };

    const T
            Half(0.5), Quarter(0.25),
            Dx  = Half * (stencil[2] - stencil[1]), Dx2 = Dx * Dx,
            Dy  = Half * (stencil[4] - stencil[3]), Dy2 = Dy * Dy,
            Dz  = Half * (stencil[6] - stencil[5]), Dz2 = Dz * Dz,
            normGrad = Dx2 + Dy2 + Dz2;

    if (normGrad < T(0)) {
        return T(0);
    }

    const T
            Dxx = stencil[2] - 2 * stencil[0] + stencil[1],
            Dyy = stencil[4] - 2 * stencil[0] + stencil[3],
            Dzz = stencil[6] - 2 * stencil[0] + stencil[5],
            Dxy = Quarter * (stencil[10] - stencil[ 8] + stencil[7] - stencil[ 9]),
            Dxz = Quarter * (stencil[14] - stencil[12] + stencil[11] - stencil[13]),
            Dyz = Quarter * (stencil[18] - stencil[16] + stencil[15] - stencil[17]);
    T alpha = (Dx2*(Dyy+Dzz)+Dy2*(Dxx+Dzz)+Dz2*(Dxx+Dyy)-2*(Dx*(Dy*Dxy+Dz*Dxz)+Dy*Dz*Dyz));
    T beta  = sqrt(normGrad);

    return in(i, j, k) + s*(alpha/(2*beta*beta));
}

template<typename T>
SmoothFilter<T>::SmoothFilter(Mode mode, unsigned int iterations, float blend)
    : _mode(mode), _iterations(iterations), blend(blend) {}

namespace{

template<typename T>
Bifrost::API::VoxelChannel copy(const Bifrost::API::VoxelChannel& in, const Bifrost::API::String& name){
    Bifrost::API::Layout layout(in.layout());
    Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(in.stateID()));

    Bifrost::API::VoxelChannel out = ss.createChannel(Bifrost::API::Component(in.component()), in.dataType(), name);
    out.setOffsetType (in.offsetType());
    out.setBackgroundValue<T>(in.backgroundValue<T>());

    IdentityFilter<T>().filter(in,out);

    return out;
}

}

template<typename T>
void SmoothFilter<T>::filter(const API::Channel &_in, API::Channel &out) const{
    Bifrost::API::Channel in(_in); // create new reference for swapping
    assert(in.layout() == out.layout());

    Bifrost::API::Layout layout(in.layout());
    Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(in.stateID()));

    unsigned int n = _iterations;
    if(_mode == kGaussian) n *= 4; // gaussian approximation handled as 4*mean

    Bifrost::API::Channel tmp = copy<T>(in, "tmp");
    bool need_copy = (blend != 1 && in == out);
    if(need_copy) out = copy<T>(in, "tmp2");

    std::unique_ptr<Filter> filter;
    switch(_mode){
    case kMeanValue:
    case kGaussian: {// gaussian approximation handled as 4*mean
        filter = std::make_unique< MeanFilter<T> >();
        break;
    }case kMedianValue:{
        filter = std::make_unique< MedianFilter<T> >();
        break;
    }case kLaplacianFlow:{
        filter = std::make_unique< LaplacianFilter<T> >();
        break;
    }case kCurvatureFlow:{
        filter = std::make_unique< CurvatureFilter<T> >();
        break;
    }
    }

    if(n%2==1) SWAP(tmp,out); // => result in 'out' at the end of the loop
    for(unsigned int i = 0; i < n; ++i){
        SWAP(tmp,out);
        filter->filter(tmp,out);
    }
    float blend = need_copy? this->blend : 1-this->blend;
    if(need_copy) SWAP(in,out);

    if(blend != 1){
        AlphaMixer<T>(blend).mix(in,out,out);
    }

    // clear temporary channels
    tmp.reset();
    in.reset();
    ss.removeChannel("tmp");
    if(need_copy) ss.removeChannel("tmp2");
}

INSTANCIATE(TransformFilter);
INSTANCIATE(IdentityFilter);
INSTANCIATE(OffsetFilter);
INSTANCIATE(DilateFilter);
INSTANCIATE(ErodeFilter);
INSTANCIATE(ScaleFilter);
INSTANCIATE(RemapFilter);
INSTANCIATE(HaloFilter);
template class BIFROSTPROCESSING_API_DECL MeanFilter<float>;
template class BIFROSTPROCESSING_API_DECL MedianFilter<float>;
template class BIFROSTPROCESSING_API_DECL LaplacianFilter<float>;
template class BIFROSTPROCESSING_API_DECL CurvatureFilter<float>;
template class BIFROSTPROCESSING_API_DECL SmoothFilter<float>;

}} // namespace Bifrost::Processing
