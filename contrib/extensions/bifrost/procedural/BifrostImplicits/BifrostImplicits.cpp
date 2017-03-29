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

    Samplers(Bifrost::API::VoxelChannel channel, unsigned int size) : channel(channel) {
        samplers.resize(size, NULL);
    }
    virtual ~Samplers(){
        for(unsigned int i = 0; i < samplers.size(); ++i)
            if(samplers[i]) delete samplers[i];
    }
    Bifrost::API::VoxelSampler* operator[](unsigned int tid){
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

    UserData(const Bifrost::API::VoxelComponent& component, const Bifrost::API::String& tmpFolder)
        : component(component), tmpFolder(tmpFolder) {
        AiCritSecInit(&lock);
    }
    ~UserData(){
        for(auto it = samplers.begin(); it != samplers.end(); ++it)
            if(it->second) delete it->second;
        samplers.clear();
        if(!tmpFolder.empty()) { Bifrost::API::File::deleteFolder(tmpFolder); }
    }
    Samplers* operator[](const std::string& channel) {
        if(samplers.find(channel) == samplers.end())
        { // first time this channel is queried
            AiCritSecEnter(&lock);
            if(samplers.find(channel) == samplers.end()){
                Bifrost::API::Ref ref = component.findChannel(channel.c_str());
                samplers[channel] = ref.valid()? new Samplers(Bifrost::API::VoxelChannel(ref), AI_MAX_THREADS) : NULL;
            }
            AiCritSecLeave(&lock);
        }
        return samplers[channel];
    }
};

inline Bifrost::API::VoxelSampler* GetThreadSampler(const AtVolumeData* data, const AtString& channel, uint16_t tid, int interp){
    Samplers* samplers = (*((UserData*) data->private_info))[channel.c_str()];
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

    UserData* userData = new UserData(frameData.inSS.components()[0], frameData.tmpFolder);
    data->private_info = userData;
    data->auto_step_size = inData.stepSize;
    return true;
}

volume_sample
{
    if(!data->private_info) return false;
    Bifrost::API::VoxelSampler* sampler = GetThreadSampler(data, channel, sg->tid, interp);
    if(!sampler) return false;
    *type = AI_TYPE_FLOAT;
    value->FLT() = sampler->sample<float>(amino::Math::vec3f(sg->P.x,sg->P.y,sg->P.z));
	return true;
}

volume_gradient
{
    if(!data->private_info) return false;
    Bifrost::API::VoxelSampler* sampler = GetThreadSampler(data, channel, sg->tid, interp);
    if(!sampler) return false;
    amino::Math::vec3f normal;
    sampler->sampleGradient<float>(amino::Math::vec3f(sg->P.x,sg->P.y,sg->P.z), normal);
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
