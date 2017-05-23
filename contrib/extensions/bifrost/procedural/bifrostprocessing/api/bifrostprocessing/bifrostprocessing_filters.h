#pragma once

#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_haloedcache.h>

namespace Bifrost{
namespace Processing{

class Filter{
public:
    virtual ~Filter(){}

    virtual void filter(const Bifrost::API::Channel in, Bifrost::API::Channel out) const=0;
    void filter(Bifrost::API::Channel inOut) const;

    void setTraversalMode(Bifrost::API::TraversalMode mode);
    Bifrost::API::TraversalMode traversalMode() const;

    void setDepths(int min, int max);
    const amino::Math::vec2i& depths() const;

private:
    Bifrost::API::TraversalMode _mode = Bifrost::API::TraversalMode::ParallelBreadthFirst;
    amino::Math::vec2i _depths = amino::Math::vec2i(-1,-1);
};

template<typename T>
class TransformFilter : public Filter{
public:
    void filter(const API::Channel in, API::Channel out) const;
    virtual T compute(const T& in) const=0;
};

template<typename T>
class IdentityFilter : public TransformFilter<T>{
public:
    T compute(const T& in) const override;
};

template<typename T>
class OffsetFilter : public TransformFilter<T>{
public:
    OffsetFilter(const T& offset);
    T compute(const T& in) const override;
private:
    T offset;
};

template<typename T>
class DilateFilter : public OffsetFilter<T>{
public:
    DilateFilter(const T& amount);
};

template<typename T>
class ErodeFilter : public OffsetFilter<T>{
public:
    ErodeFilter(const T& amount);
};

template<typename T>
class ScaleFilter : public TransformFilter<T>{
public:
    ScaleFilter(const T& scale);
    T compute(const T& in) const override;
private:
    T scale;
};

template<typename T>
class RemapFilter : public TransformFilter<T>{
public:
    RemapFilter(const T& inMin, const T& inMax, const T& min, const T& max);
    T compute(const T& in) const override;
private:
    T inMin, min, dir;
};

template<typename T>
class HaloFilter : public Filter{
public:
    void filter(const API::Channel in, API::Channel out) const;
    virtual T compute(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) const=0;
};

template<typename T>
class MeanFilter : public HaloFilter<T>{
public:
    T compute(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) const override;
};

template<typename T>
class MedianFilter : public HaloFilter<T>{
public:
    T compute(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) const override;
};

template<typename T>
class LaplacianFilter : public HaloFilter<T>{
public:
    T compute(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) const override;
};

template<typename T>
class CurvatureFilter : public HaloFilter<T>{
public:
    T compute(const typename Bifrost::API::HaloedCache<T>::Tile& in, int i, int j, int k) const override;
};

template<typename T>
class SmoothFilter : public Filter{
public:
    enum Mode{
        kMeanValue=0,
        kGaussian,
        kMedianValue,
        kLaplacianFlow,
        kCurvatureFlow
    };
    SmoothFilter(Mode mode, unsigned int iterations, float blend);
    void filter(const Bifrost::API::Channel in, Bifrost::API::Channel out) const override;

private:
    Mode _mode;
    unsigned int _iterations;
    float blend;
};

class ExtendFilter : public Filter{
public:
    ExtendFilter(float height, const amino::Math::vec2f& center, const amino::Math::vec2f& dimensions, float radius);
    void filter(const Bifrost::API::Channel in, Bifrost::API::Channel out) const override;
    void uvs(Bifrost::API::Channel out) const;

private:
    float height;
    amino::Math::vec2f center, dimensions;
    float radius;
};

}} // namespace Bifrost::Processing
