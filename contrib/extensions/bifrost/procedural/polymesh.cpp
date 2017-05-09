#include <ai.h>
#include <vector>
#include "debug.h"
#include "tbb.h"
#include "surface.h"
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostprocessing/bifrostprocessing_meshing.h>
#include "utils.h"

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPolymeshMtds)

namespace {

    template<typename T>
    void exportChannel(AtNode* polymesh, const Bifrost::API::Array<amino::Math::vec3f>& vertices, const Bifrost::API::VoxelChannel& channel);

#define EXPORT_CHANNEL(T, type, declaration, AiArraySet, CAST) \
    template<> void exportChannel<T>(AtNode* polymesh, const Bifrost::API::Array<amino::Math::vec3f>& vertices, const Bifrost::API::VoxelChannel& channel){ \
        AiNodeDeclare(polymesh, channel.name().c_str(), declaration); \
        AtArray *channelArray = AiArrayAllocate(vertices.count(), 1, type); \
        TBB_FOR_ALL(0, vertices.count(), 100, [vertices, channel, channelArray](size_t start, size_t end){ \
            Bifrost::API::VoxelSampler sampler = channel.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace); \
            for(size_t i = start; i < end; ++i){ \
                AiArraySet(channelArray, i, CAST(sampler.sample<T>(vertices[i]))); \
            } \
        }); \
        AiNodeSetArray(polymesh, channel.name().c_str(), channelArray); \
    }

    // export channel specialization per type
    EXPORT_CHANNEL(float, AI_TYPE_FLOAT, "varying FLOAT", AiArraySetFlt,)
    EXPORT_CHANNEL(amino::Math::vec2f, AI_TYPE_VECTOR2, "varying VECTOR2", AiArraySetVec2, AminoVec2ToAtVector)
    EXPORT_CHANNEL(amino::Math::vec3f, AI_TYPE_VECTOR, "varying VECTOR", AiArraySetVec, AminoVec3ToAtVector)

    inline void exportChannel(AtNode *polymesh, const Bifrost::API::Array<amino::Math::vec3f> &vertices, const Bifrost::API::VoxelChannel &channel){
        switch(channel.dataType()){
        case Bifrost::API::DataType::FloatType:   exportChannel<float>(polymesh, vertices, channel); return;
        case Bifrost::API::DataType::FloatV2Type: exportChannel<amino::Math::vec2f>(polymesh, vertices, channel); return;
        case Bifrost::API::DataType::FloatV3Type: exportChannel<amino::Math::vec3f>(polymesh, vertices, channel); return;
        default: AiMsgWarning("[BIFROST] Unknown conversion for exporting channel '%s' (type = %d)", channel.name().c_str(), channel.dataType());
        }
    }
}

procedural_init
{
    PROFILER("initialize mesh");

    float shutter_start, shutter_end;
    { // get shutter start / end
        AtNode* options = AiUniverseGetOptions();
        if(AiNodeGetBool(options, "ignore_motion_blur")){
            shutter_start = shutter_end = AiNodeGetFlt(options, "reference_time");
        }else{
            AtNode* camera = AiUniverseGetCamera();
            shutter_start = AiNodeGetFlt(camera, "shutter_start");
            shutter_end = AiNodeGetFlt(camera, "shutter_end");
        }
        if(shutter_end < shutter_start){ shutter_end = shutter_start; }
    }
    bool motion = shutter_start != shutter_end;

    Surface::PolymeshParams params(node);
    Bifrost::API::VoxelComponent component;
    if(!Surface::initialize(params, component)){
        return false;
    }
    DUMP(params.subdivisions);

    Bifrost::API::VoxelChannel distance = component.findChannel(params.distance_channel.c_str());

    // **** CREATE POLYMESH ****

    Bifrost::API::Array<amino::Math::vec3f> vertices;
    Bifrost::API::Array<amino::Math::vec3i> indices;
    Bifrost::Processing::createMarchingCubeMesh(distance, vertices, indices, params.subdivisions);

    AtNode *polymesh = AiNode("polymesh");
    AiNodeSetBool(polymesh, "smoothing", params.smoothing);
    AiNodeSetStr(polymesh, "name", (std::string(AiNodeGetName(node))+"_polymesh").c_str() );
    AiNodeSetMatrix(polymesh, "matrix", AiM4Scaling(AtVector(params.space_scale, params.space_scale, params.space_scale)));

    // compute velocities
    Bifrost::API::Array<amino::Math::vec3f> velocities;
    if(motion || shutter_start != 0){
        float velocity_scale = params.velocity_scale / params.fps;
        Bifrost::API::VoxelChannel velocity_u = component.findChannel("velocity_u");
        Bifrost::API::VoxelChannel velocity_v = component.findChannel("velocity_v");
        Bifrost::API::VoxelChannel velocity_w = component.findChannel("velocity_w");
        velocities.resize(vertices.count());
        TBB_FOR_ALL(0, vertices.count(), 100, [velocity_scale, vertices, velocity_u, velocity_v, velocity_w, &velocities](size_t start, size_t end){
            Bifrost::API::VoxelSampler sampler_u = velocity_u.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
            Bifrost::API::VoxelSampler sampler_v = velocity_v.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
            Bifrost::API::VoxelSampler sampler_w = velocity_w.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
            for(size_t i = start; i < end; ++i){
                const amino::Math::vec3f& p = vertices[i];
                velocities[i] = velocity_scale * amino::Math::vec3f(sampler_u.sample<float>(p), sampler_v.sample<float>(p), sampler_w.sample<float>(p));
            }
        });
        if(motion){
            AiNodeSetFlt(polymesh, "motion_start", shutter_start);
            AiNodeSetFlt(polymesh, "motion_end", shutter_end);
        }
    }

    {// export vertices
        AtArray *vlist = AiArrayAllocate( vertices.count(), motion? 2 : 1, AI_TYPE_VECTOR );
        if(shutter_start == 0){
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,vertices](size_t i){
                AiArraySetVec(vlist, i, AminoVec3ToAtVector(vertices[i]));
            });
        }else{
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,vertices,velocities,shutter_start](size_t i){
                AiArraySetVec(vlist, i, AminoVec3ToAtVector(vertices[i] + velocities[i]*shutter_start));
            });
        }
        if(motion){
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,vertices,velocities,shutter_end](size_t i){
                AiArraySetVec(vlist, vertices.count()+i, AminoVec3ToAtVector(vertices[i] + velocities[i]*shutter_end));
            });
        }
        AiNodeSetArray(polymesh, "vlist", vlist);
    }

    {// export faces
        AtArray *vidxs = AiArrayAllocate( indices.count()*3, 1, AI_TYPE_UINT );
        AtArray *nsides = AiArrayAllocate( indices.count(), 1, AI_TYPE_UINT );
        TBB_FOR_ALL(0, indices.count(), 100, [vidxs, nsides,indices](size_t i){
            AiArraySetUInt(nsides, i, 3);
            for(unsigned int j = 0; j < 3; ++j)
                AiArraySetUInt(vidxs, i*3+j, indices[i][j]);
        });
        AiNodeSetArray(polymesh, "nsides", nsides);
        AiNodeSetArray(polymesh, "vidxs", vidxs);
    }

    // export channels
    for(unsigned int i = 0; i < params.channels.size(); ++i){
        Bifrost::API::VoxelChannel channel = component.findChannel(params.channels[i].c_str());
        if(!channel.valid()){
            AiMsgWarning("[BIFROST] Skipping invalid channel '%s'. Available channels are: \n%s", params.channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
        exportChannel(polymesh, vertices, channel);
    }

    //AiNodeSetByte(polymesh, "sidedness", 0);
    //AiNodeSetBool(polymesh, "invert_normals", true);

    *user_ptr = polymesh;

    return true;
}

node_parameters
{
    Surface::PolymeshParams::declare(params, nentry);
}
procedural_cleanup
{
    return true;
}
procedural_num_nodes
{
    return 1;
}
procedural_get_node
{
    return i==0? (AtNode*)(user_ptr) : NULL;
}
