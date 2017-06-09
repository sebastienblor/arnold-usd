#include "sampler.h"
#include "tbb.h"
#include <ai.h>
#include <vector>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include "utils.h"
#include "debug.h"

template<typename T>
inline T getData(const amino::Math::vec3f& wsPos, float invDx, const Bifrost::API::VoxelChannel& channel, const Bifrost::API::TileAccessor& accessor, int maxDepth, int N){
    amino::Math::vec3f pos = wsPos*invDx;\
    const Bifrost::API::Tile& tile = accessor.tile((int)pos[0], (int)pos[1], (int)pos[2], maxDepth);\
    if(tile.index().depth != maxDepth) return channel.backgroundValue<T>();
    const Bifrost::API::TileCoord& coord = tile.coord();\
    return channel.tileData<T>(tile.index())((int)(pos[0]-coord.i)%N, (int)(pos[1]-coord.j)%N, (int)(pos[2]-coord.k)%N);\
}

#define VOXEL_SAMPLER_TYPE Bifrost::API::VoxelSamplerQBSplineType
//#define VOXEL_SAMPLER_TYPE Bifrost::API::VoxelSamplerLinearType
#define VOXEL_SAMPLER_ARGS VOXEL_SAMPLER_TYPE, Bifrost::API::WorldSpace

#define ARRAY(T, AiArraySet, CAST) \
    const Bifrost::API::VoxelChannel& channel = channels[0];\
    AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type()); \
    TBB_FOR_ALL(0, positions.count(), 100, [&positions, &channel, &channelArray](size_t start, size_t end){ \
        Bifrost::API::VoxelSampler sampler = channel.createSampler(VOXEL_SAMPLER_ARGS); \
        for(size_t i = start; i < end; ++i){ \
             AiArraySet(channelArray, i, CAST(sampler.sample<T>(positions[i]))); \
        } \
    }); \
    return channelArray

#define ARRAY_CLOSEST(T, AiArraySet, CAST) \
    AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type());\
    const Bifrost::API::VoxelChannel& channel = channels[0];\
    Bifrost::API::Layout layout(channel.layout());\
    if(!accessor.valid()) accessor = layout.tileAccessor();\
    const Bifrost::API::TileAccessor& accessor = this->accessor;\
    int N = layout.tileDimInfo().tileWidth; \
    float invDx = this->invDx;\
    int maxDepth = layout.maxDepth();\
    TBB_FOR_ALL(0, positions.count(), 100, [&positions, &channel, &channelArray, &accessor, &invDx, &maxDepth, &N](size_t e){\
        AiArraySet(channelArray, e, CAST(getData<T>(positions[e], invDx, channel, accessor, maxDepth, N)));\
    });\
    return channelArray

namespace{

template<unsigned int N, typename T>
class ChannelSamplerT : public ChannelSamplerImpl{
public:
    ChannelSamplerT(const Bifrost::API::VoxelChannel (& inChannels) [N]);
    ChannelSamplerT(const ChannelSamplerT<N,T> &o);
    ChannelSamplerImpl* clone() const override;
    uint8_t type() const override;
    void sample(const AtVector &pos, AtParamValue *value) const override;
    bool sampleGradient(const AtVector &pos, AtVector& gradient) const override{ return false; }
    AtArray* array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const override;

private:
    int maxDepth, tileWidth;
    float invDx;
    Bifrost::API::VoxelChannel channels[N];
    Bifrost::API::VoxelSampler samplers[N];
    mutable Bifrost::API::TileAccessor accessor;
};

template<unsigned int N,typename T>
ChannelSamplerT<N,T>::ChannelSamplerT(const Bifrost::API::VoxelChannel (& inChannels) [N]){
    Bifrost::API::Layout layout(inChannels[0].layout());
    invDx = 1./layout.voxelScale();
    maxDepth = layout.maxDepth();
    tileWidth = layout.tileDimInfo().tileWidth;
    for(unsigned int i = 0; i < N; ++i){
        channels[i] = inChannels[i];
        samplers[i] = channels[i].createSampler(VOXEL_SAMPLER_ARGS);
    }
    accessor = layout.tileAccessor();
}
template<unsigned int N,typename T>
ChannelSamplerT<N,T>::ChannelSamplerT(const ChannelSamplerT<N,T> &o)
    : invDx(o.invDx), maxDepth(o.maxDepth), tileWidth(o.tileWidth), accessor(o.accessor){
    for(unsigned int i = 0; i < N; ++i){
        channels[i] = o.channels[i];
        samplers[i] = channels[i].createSampler(VOXEL_SAMPLER_ARGS);
    }
}
template<unsigned int N,typename T>
ChannelSamplerImpl* ChannelSamplerT<N,T>::clone() const{
    return new ChannelSamplerT<N,T>(*this);
}

// *** SPECIALIZATIONS ***

template<> uint8_t ChannelSamplerT<1,float>::type() const {
    return AI_TYPE_FLOAT;
}
template<> void ChannelSamplerT<1,float>::sample(const AtVector &pos, AtParamValue *value) const {
    value->FLT() = samplers[0].sample<float>(Convert(pos));
    //value->FLT() = getData<float>(Convert(pos), invDx, channels[0], accessor, maxDepth, tileWidth);
}
template<> bool ChannelSamplerT<1,float>::sampleGradient(const AtVector &pos, AtVector& gradient) const {
    amino::Math::vec3f g;
    samplers[0].sampleGradient<float>(Convert(pos), g);
    gradient = Convert(g);
    return true;
}
template<> AtArray* ChannelSamplerT<1,float>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY(float, AiArraySetFlt,);
    //ARRAY_CLOSEST(float, AiArraySetFlt,);
}


template<> uint8_t ChannelSamplerT<1,amino::Math::vec2f>::type() const {
    return AI_TYPE_VECTOR2;
}
template<> void ChannelSamplerT<1,amino::Math::vec2f>::sample(const AtVector &pos, AtParamValue *value) const {
    value->VEC2() = Convert(samplers[0].sample<amino::Math::vec2f>(Convert(pos)));
}
template<> AtArray* ChannelSamplerT<1,amino::Math::vec2f>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY(amino::Math::vec2f, AiArraySetVec2, Convert);
}


template<> uint8_t ChannelSamplerT<1,amino::Math::vec3f>::type() const {
    return AI_TYPE_VECTOR;
}
template<> void ChannelSamplerT<1,amino::Math::vec3f>::sample(const AtVector &pos, AtParamValue *value) const {
    value->VEC() = Convert(samplers[0].sample<amino::Math::vec3f>(Convert(pos)));
}
template<> AtArray* ChannelSamplerT<1,amino::Math::vec3f>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY(amino::Math::vec3f, AiArraySetVec, Convert);
}


template<> uint8_t ChannelSamplerT<2,float>::type() const {
    return AI_TYPE_VECTOR2;
}
template<> void ChannelSamplerT<2,float>::sample(const AtVector &pos, AtParamValue *value) const {
    amino::Math::vec3f p(Convert(pos));
    value->VEC2() = AtVector2(samplers[0].sample<float>(p), samplers[1].sample<float>(p));
}
template<> AtArray* ChannelSamplerT<2,float>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type());
    TBB_FOR_ALL(0, positions.count(), 100, [&positions, this, channelArray](size_t start, size_t end){
        ChannelSamplerT<2,float> s(*this);
        for(size_t i = start; i < end; ++i){ \
            amino::Math::vec3f p(positions[i]);
            AiArraySetVec2(channelArray, i, AtVector2(s.samplers[0].sample<float>(p), s.samplers[1].sample<float>(p)));
        }
    });
    return channelArray;
}


template<> uint8_t ChannelSamplerT<3,float>::type() const {
    return AI_TYPE_VECTOR;
}
template<> void ChannelSamplerT<3,float>::sample(const AtVector &pos, AtParamValue *value) const {
    amino::Math::vec3f p(Convert(pos));
    value->VEC() = AtVector(samplers[0].sample<float>(p), samplers[1].sample<float>(p), samplers[2].sample<float>(p));
}
template<> AtArray* ChannelSamplerT<3,float>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type());
    TBB_FOR_ALL(0, positions.count(), 100, [&positions, this, channelArray](size_t start, size_t end){
        ChannelSamplerT<3,float> s(*this);
        for(size_t i = start; i < end; ++i){ \
            amino::Math::vec3f p(positions[i]);
            AiArraySetVec(channelArray, i, AtVector(s.samplers[0].sample<float>(p), s.samplers[1].sample<float>(p), s.samplers[2].sample<float>(p)));
        }
    });
    return channelArray;
}


template<> uint8_t ChannelSamplerT<1,int>::type() const {
    return AI_TYPE_INT;
}
template<> void ChannelSamplerT<1,int>::sample(const AtVector &pos, AtParamValue *value) const {
    value->INT() = getData<int>(Convert(pos), invDx, channels[0], accessor, maxDepth, tileWidth);
}
template<> AtArray* ChannelSamplerT<1,int>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY_CLOSEST(int, AiArraySetInt,);
}


template<> uint8_t ChannelSamplerT<1,amino::Math::vec2i>::type() const {
    return AI_TYPE_VECTOR2;
}
template<> void ChannelSamplerT<1,amino::Math::vec2i>::sample(const AtVector &pos, AtParamValue *value) const {
    value->VEC2() = Convert(getData<amino::Math::vec2i>(Convert(pos), invDx, channels[0], accessor, maxDepth, tileWidth));
}
template<> AtArray* ChannelSamplerT<1,amino::Math::vec2i>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY_CLOSEST(amino::Math::vec2i, AiArraySetVec2, Convert);
}


template<> uint8_t ChannelSamplerT<1,amino::Math::vec3i>::type() const {
    return AI_TYPE_VECTOR2;
}
template<> void ChannelSamplerT<1,amino::Math::vec3i>::sample(const AtVector &pos, AtParamValue *value) const {
    value->VEC() = Convert(getData<amino::Math::vec3i>(Convert(pos), invDx, channels[0], accessor, maxDepth, tileWidth));
}
template<> AtArray* ChannelSamplerT<1,amino::Math::vec3i>::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    ARRAY_CLOSEST(amino::Math::vec3i, AiArraySetVec, Convert);
}


ChannelSamplerImpl* createSampler(const std::vector<Bifrost::API::VoxelChannel>& channels){
    if(channels.size()==0 || !channels[0].valid()) return nullptr;
    Bifrost::API::DataType dataType = channels[0].dataType();
    for(unsigned int i = 1; i < channels.size(); ++i){
        if(!channels[i].valid()) return nullptr;
        if(channels[i].dataType() != dataType) return nullptr; // heterogeneous data types...
    }
    switch (channels.size()) {
    case 1:{
        switch(dataType){
        case Bifrost::API::DataType::FloatType   : return new ChannelSamplerT<1,float>({channels[0]});
        case Bifrost::API::DataType::FloatV2Type : return new ChannelSamplerT<1,amino::Math::vec2f>({channels[0]});
        case Bifrost::API::DataType::FloatV3Type : return new ChannelSamplerT<1,amino::Math::vec3f>({channels[0]});
        case Bifrost::API::DataType::Int32Type   : return new ChannelSamplerT<1,int>({channels[0]});
        case Bifrost::API::DataType::Int32V2Type : return new ChannelSamplerT<1,amino::Math::vec2i>({channels[0]});
        case Bifrost::API::DataType::Int32V3Type : return new ChannelSamplerT<1,amino::Math::vec3i>({channels[0]});
        default: return nullptr;
        }
    }case 2:{
        return dataType==Bifrost::API::DataType::FloatType? new ChannelSamplerT<2,float>({channels[0],channels[1]}) : nullptr;
    }case 3:{
        return dataType==Bifrost::API::DataType::FloatType? new ChannelSamplerT<3,float>({channels[0],channels[1],channels[2]}) : nullptr;
    }default: break;
    }
    return nullptr;
}

}

ChannelSampler::ChannelSampler() : impl(nullptr) {}
ChannelSampler::ChannelSampler(const ChannelSampler &o) : impl(o.impl->clone()){}

ChannelSampler::ChannelSampler(const Bifrost::API::VoxelComponent &component, const Bifrost::API::String &channel){
    impl.reset(createSampler({component.findChannel(channel.c_str())}));
    if(valid()) return;
    impl.reset(createSampler({component.findChannel((channel+"_u").c_str()), component.findChannel((channel+"_v").c_str()), component.findChannel((channel+"_w").c_str()) }));
    if(valid()) return;
    impl.reset(createSampler({component.findChannel((channel+"_x").c_str()), component.findChannel((channel+"_y").c_str()), component.findChannel((channel+"_z").c_str()) }));
    if(valid()) return;
    impl.reset(createSampler({component.findChannel((channel+"_x").c_str()), component.findChannel((channel+"_y").c_str()) }));
}

ChannelSampler::ChannelSampler(const Bifrost::API::VoxelChannel channel) : impl(createSampler({channel})) {}
ChannelSampler::ChannelSampler(const std::vector<Bifrost::API::VoxelChannel> &channels) : impl(createSampler(channels)) {}

ChannelSampler& ChannelSampler::operator=(const ChannelSampler& o){ impl.reset(); if(o.impl != nullptr) impl.reset(o.impl->clone()); return *this; }

uint8_t ChannelSampler::type() const{
    return valid()? impl->type() : AI_TYPE_NONE;
}
void ChannelSampler::sample(const AtVector &pos, AtParamValue *value) const{
    if(valid()) impl->sample(pos,value);
}
bool ChannelSampler::sampleGradient(const AtVector &pos, AtVector& gradient) const{
    return valid() && impl->sampleGradient(pos,gradient);
}
AtArray* ChannelSampler::array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const{
    return valid()? impl->array(positions) : nullptr;
}

// *** THREAD SAMPLER ***

ThreadSampler::ThreadSampler(const Bifrost::API::VoxelComponent component) : component(component) {}
ThreadSampler::ThreadSampler(const ThreadSampler &o) : component(o.component) {}
const ChannelSampler& ThreadSampler::channelSampler(const std::string &channel, int) const{
    // TODO: consider interpolation mode
    if(samplers.find(channel) == samplers.end())
        samplers[channel] = ChannelSampler(component, channel.c_str());
    return samplers[channel];
}

// *** COMPONENT SAMPLER ***

Sampler::Sampler(const Bifrost::API::VoxelComponent component, unsigned int N){
    samplers.resize(N, ThreadSampler(component));
}

bool Sampler::sample(const AtString& channel, const AtVector& pos, uint16_t tid, AtParamValue *value, uint8_t *type, int interp, float time) const{
    const ChannelSampler& sampler = samplers[tid].channelSampler(std::string(channel.c_str()), interp);
    if(!sampler.valid()) return false;
    *type = sampler.type();
    sampler.sample((time==0)? pos : pos - time * velocity(pos, tid), value);
    return true;
}

bool Sampler::sampleGradient(const AtString& channel, const AtVector& pos, uint16_t tid, AtVector& gradient, int interp, float time) const{
    const ChannelSampler& sampler = samplers[tid].channelSampler(std::string(channel.c_str()), interp);
    return sampler.valid() && sampler.sampleGradient(((time==0)? pos : pos - time * velocity(pos, tid)), gradient);
}

AtVector Sampler::velocity(const AtVector& pos, uint16_t tid) const{
    const ChannelSampler& sampler = samplers[tid].channelSampler("velocity", AI_VOLUME_INTERP_TRICUBIC);
    if(!sampler.valid()) return AtVector(0,0,0);
    AtParamValue value;
    sampler.sample(pos, &value);
    return value.VEC();
}
