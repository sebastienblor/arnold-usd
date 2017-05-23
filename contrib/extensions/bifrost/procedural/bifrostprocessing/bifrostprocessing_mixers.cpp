#include <bifrostprocessing/bifrostprocessing_mixers.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_layout.h>
#include "defs.h"

namespace{

template<typename T>
class BaseMixerVisitor : public Bifrost::API::Visitor {
public:
    BaseMixerVisitor(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, Bifrost::API::Channel& out, const Bifrost::Processing::BaseMixer<T>& mixer)
        : in1(in1), in2(in2), out(out), mixer(mixer){}
    BaseMixerVisitor(const BaseMixerVisitor& o)
        : in1(o.in1), in2(o.in2), out(o.out), mixer(o.mixer) {}
    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) override{
        const Bifrost::API::TileData<T>& in1Data = in1.tileData<T>(index);
        const Bifrost::API::TileData<T>& in2Data = in2.tileData<T>(index);
        Bifrost::API::TileData<T> outData = out.tileData<T>(index);
        for(size_t k = 0; k < outData.count(); ++k) outData[k] = mixer.compute(in1Data[k],in2Data[k]);
    }
    Bifrost::API::Visitor* copy() const override{ return new BaseMixerVisitor<T>(*this); }

private:
    const Bifrost::API::Channel in1, in2, out;
    const Bifrost::Processing::BaseMixer<T>& mixer;
};

template<typename T>
class AlphaMixerVisitor : public Bifrost::API::Visitor {
public:
    AlphaMixerVisitor(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, const Bifrost::API::Channel& alpha, Bifrost::API::Channel& out)
        : in1(in1), in2(in2), alpha(alpha), out(out) {}
    AlphaMixerVisitor(const AlphaMixerVisitor& o)
        : in1(o.in1), in2(o.in2), alpha(o.alpha), out(o.out) {}
    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) override{
        const Bifrost::API::TileData<T>& in1Data = in1.tileData<T>(index);
        const Bifrost::API::TileData<T>& in2Data = in2.tileData<T>(index);
        const Bifrost::API::TileData<T>& alphaData = alpha.tileData<T>(index);
        Bifrost::API::TileData<T> outData = out.tileData<T>(index);
        for(size_t k = 0; k < outData.count(); ++k) outData[k] = in2Data[k] + alphaData[k]*(in1Data[k] - in2Data[k]);
    }
    Bifrost::API::Visitor* copy() const override{ return new AlphaMixerVisitor<T>(*this); }

private:
    const Bifrost::API::Channel in1, in2, alpha, out;
};

}

namespace Bifrost{
namespace Processing{

void Mixer::setTraversalMode(API::TraversalMode mode){ _mode = mode; }
Bifrost::API::TraversalMode Mixer::traversalMode() const{ return _mode; }
void Mixer::setDepths(int min, int max){ _depths = amino::Math::vec2i(min, max); }
const amino::Math::vec2i& Mixer::depths() const{ return _depths; }

template<typename T>
void BaseMixer<T>::mix(const Bifrost::API::Channel& in1, const Bifrost::API::Channel& in2, Bifrost::API::Channel& out) const{
    BaseMixerVisitor<T> visitor(in1,in2,out,*this);
    if(depths()[0] > 0 && depths()[1] > 0){
        Bifrost::API::Layout(out.layout()).traverse(visitor, traversalMode(), depths()[0], depths()[1]);
    }else{
        Bifrost::API::Layout(out.layout()).traverse(visitor, traversalMode());
    }
}

template<typename T>
AlphaMixer<T>::AlphaMixer(float alpha) : alpha(alpha){}
template<typename T>
AlphaMixer<T>::AlphaMixer(const Bifrost::API::Channel& alpha) : alpha(1), alphaChannel(alpha){}

template<typename T>
void AlphaMixer<T>::mix(const API::Channel &in1, const API::Channel &in2, API::Channel &out) const{
    if(alphaChannel.valid()){
        AlphaMixerVisitor<T> visitor(in1,in2,alphaChannel,out);
        if(this->depths()[0] > 0 && this->depths()[1] > 0){
            Bifrost::API::Layout(out.layout()).traverse(visitor, this->traversalMode(), this->depths()[0], this->depths()[1]);
        }else{
            Bifrost::API::Layout(out.layout()).traverse(visitor, this->traversalMode());
        }
    }else{
        BaseMixer<T>::mix(in1,in2,out);
    }
}

template<typename T>
T AlphaMixer<T>::compute(const T& in1, const T& in2) const{
    return in2 + alpha*(in1 - in2);
}

template<typename T>
StepMixer<T>::StepMixer(float dt) : dt(dt){}

template<typename T>
T StepMixer<T>::compute(const T& in1, const T& in2) const{
    return in1 + dt*in2;
}

INSTANCIATE(BaseMixer);
INSTANCIATE(AlphaMixer);
INSTANCIATE(StepMixer);

}} // Bifrost::Processing
