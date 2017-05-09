#include <bifrostprocessing/bifrostprocessing_visitors.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_haloedcache.h>
#include <aminomath/vec.h>
#include <vector>
#include <algorithm>
#include <float.h>
#include "defs.h"

namespace {

template<typename T>
std::function<T(const T&)> RemapFunction(const T& inMin, const T& inMax, const T& min, const T& max){
    T dir = (max-min)/(inMax-inMin);
    return [inMin,min,dir](const T& in){ return (in - inMin)*dir + min; };
}

template<typename T> inline T sqrt(const T& v){ return std::sqrt(v); }
template<> inline amino::Math::vec2f sqrt<amino::Math::vec2f>(const amino::Math::vec2f& v){ return amino::Math::vec2f(std::sqrt(v[0]), std::sqrt(v[1])); }
template<> inline amino::Math::vec2i sqrt<amino::Math::vec2i>(const amino::Math::vec2i& v){ return amino::Math::vec2i((int)std::sqrt(v[0]), (int)std::sqrt(v[1])); }
template<> inline amino::Math::vec3f sqrt<amino::Math::vec3f>(const amino::Math::vec3f& v){ return amino::Math::vec3f(std::sqrt(v[0]), std::sqrt(v[1]), std::sqrt(v[2])); }
template<> inline amino::Math::vec3i sqrt<amino::Math::vec3i>(const amino::Math::vec3i& v){ return amino::Math::vec3i((int)std::sqrt(v[0]), (int)std::sqrt(v[1]), (int)std::sqrt(v[2])); }

} // namespace

namespace Bifrost{
namespace Processing{

template<typename T>
CopyVisitor<T>::CopyVisitor(const Bifrost::API::Channel &in, Bifrost::API::Channel &out)
    : TransformVisitor<T>(in, out, [](const T& in) { return in; }) { assert(in != out); }


template<typename T>
OffsetVisitor<T>::OffsetVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& offset)
    : TransformVisitor<T>(in, out, [offset](const T& in) { return in + offset; }) {}
template<typename T>
OffsetVisitor<T>::OffsetVisitor(Bifrost::API::Channel& inOut, const T& offset)
    : TransformVisitor<T>(inOut, [offset](const T& in) { return in + offset; }) {}


template<typename T>
MultiplyVisitor<T>::MultiplyVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& multiply)
    : TransformVisitor<T>(in, out, [multiply](const T& in) { return in * multiply; }) {}
template<typename T>
MultiplyVisitor<T>::MultiplyVisitor(Bifrost::API::Channel& inOut, const T& multiply)
    : TransformVisitor<T>(inOut, [multiply](const T& in) { return in * multiply; }) {}


template<typename T>
MultiplyOffsetVisitor<T>::MultiplyOffsetVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& multiply, const T& offset)
    : TransformVisitor<T>(in, out, [multiply, offset](const T& in) { return multiply*in + offset; }) {}
template<typename T>
MultiplyOffsetVisitor<T>::MultiplyOffsetVisitor(Bifrost::API::Channel& inOut, const T& multiply, const T& offset)
    : TransformVisitor<T>(inOut, [multiply, offset](const T& in) { return multiply*in + offset; }) {}


template<typename T>
RemapVisitor<T>::RemapVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& inMin, const T& inMax, const T& min, const T& max)
    : TransformVisitor<T>(in, out, RemapFunction(inMin, inMax, min, max)) {}
template<typename T>
RemapVisitor<T>::RemapVisitor(Bifrost::API::Channel& channel, const T& inMin, const T& inMax, const T& min, const T& max)
    : RemapVisitor<T>(channel, channel, inMin, inMax, min, max) {}


template<typename T>
BlendVisitor<T>::BlendVisitor(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, float alpha, Bifrost::API::Channel& out) :
    _in1(in1), _in2(in2), _alpha(alpha), _out(out) {
    assert(in1.layout() == in2.layout() && in2.layout() == out.layout() );
}
template<typename T>
BlendVisitor<T>::BlendVisitor(const BlendVisitor& o) :
    _in1(o._in1), _in2(o._in2), _alpha(o._alpha), _out(o._out) {}
template<typename T>
Bifrost::API::Visitor* BlendVisitor<T>::copy() const{ return new BlendVisitor<T>(*this); }

template<typename T>
void BlendVisitor<T>::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ){
    const Bifrost::API::TileData<T> &inData1 = _in1.tileData<T>(index), &inData2 = _in2.tileData<T>(index);
    Bifrost::API::TileData<T> outData = _out.tileData<T>(index);
    for(size_t k = 0; k < inData1.count(); ++k) outData[k] = inData2[k] + _alpha*(inData1[k] - inData2[k]);
}

template<typename T>
MeanVisitor<T>::MeanVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out)
    : HaloVisitor<T>(in, out, [](const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) {
        static float s = 1./27.;
        T out(0);
        for(int ii = -1; ii <= 1; ++ii)
            for(int jj = -1; jj <= 1; ++jj)
                for(int kk = -1; kk <= 1; ++kk)
                    out += in(i+ii,j+jj,k+kk);
        return out*s;
    }){}

template<typename T>
MedianVisitor<T>::MedianVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out)
    : HaloVisitor<T>(in, out, [](const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) {
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
    }){}

template<typename T>
LaplacianFlowVisitor<T>::LaplacianFlowVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out)
    : HaloVisitor<T>(in, out, [](const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) {
        static float s = 1./6.;
        return in(i,j,k) + s * (-6 * in(i,j,k) + in(i-1,j,k) + in(i+1,j,k) + in(i,j-1,k) + in(i,j+1,k) + in(i,j,k-1) + in(i,j,k+1));
    }){}

template<typename T>
CurvatureFlowVisitor<T>::CurvatureFlowVisitor(const API::Channel &in, const API::Channel &out)
    : HaloVisitor<T>(in, out, [](const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) {
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
    }){}

INSTANCIATE(TransformVisitor);
INSTANCIATE(CopyVisitor);
INSTANCIATE(OffsetVisitor);
INSTANCIATE(MultiplyVisitor);
INSTANCIATE(MultiplyOffsetVisitor);
INSTANCIATE(RemapVisitor);
INSTANCIATE(MeanVisitor);
INSTANCIATE(MedianVisitor);
INSTANCIATE(LaplacianFlowVisitor);
INSTANCIATE(CurvatureFlowVisitor);
INSTANCIATE(BlendVisitor);

}} // namespace Bifrost::Processing
