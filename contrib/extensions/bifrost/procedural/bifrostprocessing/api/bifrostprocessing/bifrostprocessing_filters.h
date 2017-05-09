#pragma once

#include <bifrostapi/bifrost_voxelchannel.h>

namespace Bifrost{
namespace Processing{

class Filter{
public:
    virtual ~Filter(){}
    virtual void filter(const Bifrost::API::VoxelChannel& in, Bifrost::API::VoxelChannel& out)=0;
    void filter(Bifrost::API::VoxelChannel& inOut){ this->filter(inOut, inOut); }
};

template<typename T>
class OffsetFilter : public Filter{
public:
    OffsetFilter(const T& amount);
    void filter(const Bifrost::API::VoxelChannel& in, Bifrost::API::VoxelChannel& out) override;

private:
    T _amount;
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
    void filter(const Bifrost::API::VoxelChannel& in, Bifrost::API::VoxelChannel& out) override;

private:
    Mode _mode;
    unsigned int _iterations;
    float blend;
};

}} // namespace Bifrost::Processing
