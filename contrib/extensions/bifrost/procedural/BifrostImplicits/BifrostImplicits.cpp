#include <ai.h>
#include <Implicit.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

AI_VOLUME_NODE_EXPORT_METHODS(BifrostImplicitsMtd)

using namespace Bifrost::RenderCore;

namespace
{

struct Samplers {
    Bifrost::API::VoxelChannel channel;
    std::vector<Bifrost::API::VoxelSampler*> samplers;
    const unsigned int size;
    AtCritSec lock;

    Samplers(Bifrost::API::VoxelChannel channel, unsigned int size) : channel(channel), size(size) {
        AiCritSecInit(&lock);
    }
    virtual ~Samplers(){
        for(unsigned int i = 0; i < samplers.size(); ++i)
            if(samplers[i]) delete samplers[i];
        AiCritSecClose(&lock);
    }
    Bifrost::API::VoxelSampler* operator[](unsigned int tid){
        if(samplers.empty())
        { // first time this channel is queried, allocate array for thread safe queries
            AiCritSecEnter(&lock);
            if(samplers.empty())
                samplers.resize(size, NULL);
            AiCritSecLeave(&lock);
        }
        if(!samplers[tid])
            samplers[tid] = new Bifrost::API::VoxelSampler(channel.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
        return samplers[tid];
    }
};

struct UserData{
    Bifrost::API::VoxelComponent component;
    std::map<std::string, Samplers*> samplers; // channel name => samplers/thread
    AtCritSec lock;
    Bifrost::API::String tmpFolder;
    float spf; // seconds per frame

    UserData(const Bifrost::API::VoxelComponent& component, const Bifrost::API::StringArray& channelNames, const Bifrost::API::String& tmpFolder, float spf)
        : component(component), tmpFolder(tmpFolder), spf(spf) {
        for(unsigned int i = 0; i < channelNames.count(); ++i){
            Bifrost::API::Ref ref = component.findChannel(channelNames[i].c_str());
            samplers[channelNames[i].c_str()] = ref.valid()? new Samplers(Bifrost::API::VoxelChannel(ref), AI_MAX_THREADS) : NULL;
        }
    }
    ~UserData(){
        for(auto it = samplers.begin(); it != samplers.end(); ++it)
            if(it->second) delete it->second;
        samplers.clear();
        if(!tmpFolder.empty()) { Bifrost::API::File::deleteFolder(tmpFolder); }
    }
    Samplers* operator[](const std::string& channel) {
        return samplers[channel];
    }
};

inline Bifrost::API::VoxelSampler* GetThreadSampler(const AtVolumeData* data, const char* channel, uint16_t tid, int interp){
    Samplers* samplers = (*((UserData*) data->private_info))[channel];
    return samplers? (*samplers)[tid] : NULL;
}

} // namespace

node_parameters
{
    AiParameterFlt("narrowBandThicknessInVoxels",0);
    AiParameterFlt("liquidStepSize",0);
    ImplicitNodeDeclareParameters(params, nentry);
}

volume_create
{
    data->private_info = NULL;

    ImplicitsInputData inData;
    FrameData frameData;

    // Implicit specific inputs
    inData.narrowBandThicknessInVoxels = AiNodeGetFlt(node, "narrowBandThicknessInVoxels");
    inData.stepSize = AiNodeGetFlt(node, "liquidStepSize");
    inData.sampleRate = AiNodeGetFlt(node, "samples");
    getNodeParameters(&inData, node);

    if(!InitializeImplicit(&inData, &frameData, &data->bbox)){
        if(frameData.empty){
            data->bbox.min.x = data->bbox.min.y = data->bbox.min.z = 0;
            data->bbox.max.x = data->bbox.max.y = data->bbox.max.z = 0;
            return true;
        }
        AiMsgError("[BIFROST] Failed to initialize implicit data on node '%s'", AiNodeGetName(node));
        return false;
    }
    if(inData.error){
       AiMsgError("[BIFROST] Invalid input data on node '%s'", AiNodeGetName(node));
       return false;
    }

    UserData* userData = new UserData(frameData.inSS.components()[0], frameData.loadChannelNames, frameData.tmpFolder, inData.motionBlur? 1./inData.fps : 0);
    data->private_info = userData;
    data->auto_step_size = inData.stepSize;

    if(inData.motionBlur){
        // make sure velocity samplers are available, disable motion blur otherwise
        if(!GetThreadSampler(data, "velocity_u", 0, 0) || !GetThreadSampler(data, "velocity_v", 0, 0) || !GetThreadSampler(data, "velocity_w", 0, 0)){
            AiMsgWarning("[bifrost implicit liquid] Could not find the velocity channels (velocity_u, velocity_v, velocity_z). Disabling motion blur...");
            userData->spf = 0;
        }
    }
    return true;
}

inline AtVector GetPosition(AtVector in, const AtVolumeData* data, uint16_t tid, int interp, float time, float maxstep){
    while(time > 0){
        amino::Math::vec3f position(in.x, in.y, in.z);
        float dt = time < maxstep? time : maxstep;
        in.x -= GetThreadSampler(data, "velocity_u", tid, interp)->sample<float>(position)*dt;
        in.y -= GetThreadSampler(data, "velocity_v", tid, interp)->sample<float>(position)*dt;
        in.z -= GetThreadSampler(data, "velocity_w", tid, interp)->sample<float>(position)*dt;
        time -= dt;
    }
    return in;
}

#define MS 1.f

volume_sample
{
    if(!data->private_info) return false;
    Bifrost::API::VoxelSampler* sampler = GetThreadSampler(data, channel, sg->tid, interp);
    if(!sampler) return false;
    *type = AI_TYPE_FLOAT;

    const float spf = ((UserData*) data->private_info)->spf;
    AtVector p = GetPosition(sg->P, data, sg->tid, interp, sg->time*spf, MS*spf);
    value->FLT() = sampler->sample<float>(amino::Math::vec3f(p.x, p.y, p.z));
	return true;
}

volume_gradient
{
    if(!data->private_info) return false;
    Bifrost::API::VoxelSampler* sampler = GetThreadSampler(data, channel, sg->tid, interp);
    if(!sampler) return false;

    const float spf = ((UserData*) data->private_info)->spf;
    AtVector p = GetPosition(sg->P, data, sg->tid, interp, sg->time*spf, MS*spf);
    amino::Math::vec3f normal;
    sampler->sampleGradient<float>(amino::Math::vec3f(p.x,p.y,p.z), normal);
    for(unsigned int i = 0; i < 3; ++i)
        (*gradient)[i] = normal[i];
	return true;
}

volume_ray_extents
{
    if(data->private_info) AiVolumeAddIntersection(info, t0, t1);
}

volume_update
{
    return true;
}

volume_cleanup
{
    if(data->private_info) delete ((UserData*) data->private_info);
    data->private_info = NULL;
    return true;
}
