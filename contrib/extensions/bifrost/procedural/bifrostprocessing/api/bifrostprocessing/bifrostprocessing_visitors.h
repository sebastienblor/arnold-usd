#pragma once

#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_haloedcache.h>
#include <bifrostapi/bifrost_layout.h>
#include <functional>

namespace Bifrost{
namespace Processing{

// *** BASE TRANSFORM (arbitrary functions) ***

template<typename T>
using TransformFunction = std::function<T(const T&)>;

template<typename T>
class TransformVisitor : public Bifrost::API::Visitor {
public:
    TransformVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const TransformFunction<T>& func)
        : _in(in), _out(out), _func(func) { assert(in.layout() == out.layout()); }
    TransformVisitor(Bifrost::API::Channel& channel, const std::function<T(const T&)>& func)
        : TransformVisitor<T>(channel, channel, func) {}
    TransformVisitor(const TransformVisitor& o)
        : _in(o._in), _out(o._out), _func(o._func) {}
    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) override{
        const Bifrost::API::TileData<T>& inData = _in.tileData<T>(index);
        Bifrost::API::TileData<T> outData = _out.tileData<T>(index);
        for(size_t k = 0; k < inData.count(); ++k) outData[k] = _func(inData[k]);
    }
    Bifrost::API::Visitor* copy() const override{ return new TransformVisitor<T>(*this); }

private:
    const Bifrost::API::Channel _in;
    Bifrost::API::Channel _out;
    TransformFunction<T> _func;
};

// *** COPY ***

template<typename T>
class CopyVisitor : public TransformVisitor<T> {
public:
    CopyVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out);
};

// *** MULTIPLY / OFFSET ***

template<typename T>
class OffsetVisitor : public TransformVisitor<T> {
public:
    OffsetVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& offset);
    OffsetVisitor(Bifrost::API::Channel& inOut, const T& offset);
};

template<typename T>
class MultiplyVisitor : public TransformVisitor<T> {
public:
    MultiplyVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& offset);
    MultiplyVisitor(Bifrost::API::Channel& inOut, const T& offset);
};

template<typename T>
class MultiplyOffsetVisitor : public TransformVisitor<T> {
public:
    MultiplyOffsetVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& multiply, const T& offset);
    MultiplyOffsetVisitor(Bifrost::API::Channel& inOut, const T& multiply, const T& offset);
};

// *** REMAP ***

template<typename T>
class RemapVisitor : public TransformVisitor<T> {
public:
    RemapVisitor(const Bifrost::API::Channel& in, Bifrost::API::Channel& out, const T& inMin, const T& inMax, const T& min, const T& max);
    RemapVisitor(Bifrost::API::Channel& channel, const T& inMin, const T& inMax, const T& min, const T& max);
};

template<typename T>
class BlendVisitor : public Bifrost::API::Visitor {
public:
    BlendVisitor(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, float alpha, Bifrost::API::Channel& out);
    BlendVisitor(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, const Bifrost::API::Channel& alpha, Bifrost::API::Channel& out);
    BlendVisitor(const BlendVisitor& o);
    Bifrost::API::Visitor* copy() const override;
    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) override;

private:
    const Bifrost::API::Channel _in1, _in2, _alphaChannel;
    Bifrost::API::Channel _out;
    float _alpha;
};

template<typename T>
using HaloFunction = std::function<T(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k)>;

template<typename T>
class HaloVisitor : public Bifrost::API::Visitor {
public:
    HaloVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out, const HaloFunction<T>& func)
        : _in(in), _out(out), _func(func){
        assert(in != out);
        assert(in.layout() == out.layout());
        _haloCache.init(in);
    }
    HaloVisitor(const HaloVisitor& o)
        : _in(o._in), _out(o._out), _func(o._func) { _haloCache.init(_in); }
    Bifrost::API::Visitor* copy() const override{ return new HaloVisitor<T>(*this); }
    void beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index) override{
        const typename Bifrost::API::HaloedCache<T>::Tile& in = this->_haloCache.tile(index);
        typename Bifrost::API::TileData<T> out = this->_out.template tileData<T>(index);
        const int w = Bifrost::API::Layout(this->_in.layout()).tileDimInfo().tileWidth;
        this->_haloCache.update(index);
        for(int i = 0; i < w; ++i)
            for(int j = 0; j < w; ++j)
                for(int k = 0; k < w; ++k)
                    out(i,j,k) = _func(in, i, j, k);
    }

protected:
    Bifrost::API::Channel _in, _out;
    Bifrost::API::HaloedCache<T> _haloCache;
    HaloFunction<T> _func;
};

template<typename T>
class MeanVisitor : public HaloVisitor<T> {
public:
    MeanVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out);
};

template<typename T>
class MedianVisitor : public HaloVisitor<T> {
public:
    MedianVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out);
};

template<typename T>
class LaplacianFlowVisitor : public HaloVisitor<T> {
public:
    LaplacianFlowVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out);
};

template<typename T>
class CurvatureFlowVisitor : public HaloVisitor<T> {
public:
    CurvatureFlowVisitor(const Bifrost::API::Channel& in, const Bifrost::API::Channel& out);
};

}} // namespace Bifrost::Processing
