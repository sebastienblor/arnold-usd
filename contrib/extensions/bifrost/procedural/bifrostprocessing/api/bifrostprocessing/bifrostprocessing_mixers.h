#pragma once

#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_array.h>

namespace Bifrost{
namespace Processing{

class Mixer{
public:
    virtual ~Mixer(){}

    virtual void mix(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, Bifrost::API::Channel& out) const=0;

    void setTraversalMode(Bifrost::API::TraversalMode mode);
    Bifrost::API::TraversalMode traversalMode() const;

    void setDepths(int min, int max);
    const amino::Math::vec2i& depths() const;

private:
    Bifrost::API::TraversalMode _mode = Bifrost::API::TraversalMode::ParallelBreadthFirst;
    amino::Math::vec2i _depths = amino::Math::vec2i(-1,-1);
};

template<typename T>
class BaseMixer : public Mixer{
public:
    virtual void mix(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, Bifrost::API::Channel& out) const override;
    virtual T compute(const T& in1, const T& in2) const=0;
};

template<typename T>
class AlphaMixer : public BaseMixer<T>{
public:
    AlphaMixer(float alpha);
    AlphaMixer(const Bifrost::API::Channel& alpha);
    void mix(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, Bifrost::API::Channel& out) const override;
    T compute(const T& in1, const T& in2) const override;
private:
    float alpha;
    Bifrost::API::Channel alphaChannel;
};

template<typename T>
class StepMixer : public BaseMixer<T>{
public:
    StepMixer(float dt);
    T compute(const T& in1, const T& in2) const override;
private:
    float dt;
};

}} // Bifrost::Processing
