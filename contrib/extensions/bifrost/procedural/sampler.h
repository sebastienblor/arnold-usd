#include <bifrostapi/bifrost_voxelcomponent.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <unordered_map>
#include <ai_vector.h>
#include <ai_nodes.h>
#include <memory>
#include <vector>

class ChannelSamplerImpl{
public:
    virtual ChannelSamplerImpl* clone() const=0;
    virtual uint8_t type() const=0;
    virtual void sample(const AtVector& pos, AtParamValue *value) const=0;
    virtual AtArray* array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const=0;
};

class ChannelSampler{
public:
    ChannelSampler();
    ChannelSampler(const ChannelSampler& o);
    ChannelSampler(const Bifrost::API::VoxelComponent& component, const Bifrost::API::String& channel);
    ChannelSampler(const Bifrost::API::VoxelChannel channel);
    ChannelSampler(const std::vector<Bifrost::API::VoxelChannel>& channels);
    ChannelSampler& operator=(const ChannelSampler& o);

    inline bool valid() const{ return impl != nullptr; }
    uint8_t type() const;
    void sample(const AtVector& pos, AtParamValue *value) const;
    AtArray* array(const Bifrost::API::Array<amino::Math::vec3f> &positions) const;

private:
    std::unique_ptr<ChannelSamplerImpl> impl;
};

class ComponentSampler{
public:
    ComponentSampler(const ComponentSampler& o);
    ComponentSampler(const Bifrost::API::VoxelComponent component);
    const ChannelSampler& channelSampler(const std::string& channel, int interp);

private:
    Bifrost::API::VoxelComponent component;
    std::unordered_map< std::string, ChannelSampler> samplers;
};
