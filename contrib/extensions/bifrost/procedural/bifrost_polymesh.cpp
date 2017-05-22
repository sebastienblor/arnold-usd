#include <ai.h>
#include <vector>
#include "debug.h"
#include "tbb.h"
#include "surface.h"
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostprocessing/bifrostprocessing_meshing.h>
#include <bifrostprocessing/bifrostprocessing_surface.h>
#include "utils.h"

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPolymeshMtds)

procedural_init
{
    float shutter_start, shutter_end;
    bool motion = getMotion(shutter_start, shutter_end);

    Polymesh surface(node);
    Bifrost::Processing::Status status = surface.initialize();
    for(unsigned int i = 0; i < status.warnings().count(); ++i){
        AiMsgWarning("[BIFROST] %s", status.warnings()[i].c_str());
    }
    if(!status){
        AiMsgError("[BIFROST] %s", status.error().c_str());
        return false;
    }
    DUMP(surface.str());
    Bifrost::API::Component component = surface.component();

    // **** CREATE POLYMESH ****

    Bifrost::API::Array<amino::Math::vec3f> vertices;
    Bifrost::API::Array<amino::Math::vec3i> indices;
    surface.mesh(vertices, indices, surface.subdivisions);

    AtNode *polymesh = AiNode("polymesh");
    AiNodeSetBool(polymesh, "smoothing", surface.smoothing);
    AiNodeSetStr(polymesh, "name", (std::string(AiNodeGetName(node))+"_polymesh").c_str() );

    // compute velocities
    Bifrost::API::Array<amino::Math::vec3f> velocities;
    if(motion || shutter_start != 0){
        velocities.resize(vertices.count());

        if(surface.velocity_channels.count() == 3){ // 3 float velocity channels
            Bifrost::API::VoxelChannel velocity_u = component.findChannel(surface.velocity_channels[0].c_str());
            Bifrost::API::VoxelChannel velocity_v = component.findChannel(surface.velocity_channels[1].c_str());
            Bifrost::API::VoxelChannel velocity_w = component.findChannel(surface.velocity_channels[2].c_str());
            TBB_FOR_ALL(0, vertices.count(), 100, [&vertices, &velocity_u, &velocity_v, &velocity_w, &velocities](size_t start, size_t end){
                Bifrost::API::VoxelSampler sampler_u = velocity_u.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                Bifrost::API::VoxelSampler sampler_v = velocity_v.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                Bifrost::API::VoxelSampler sampler_w = velocity_w.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                for(size_t i = start; i < end; ++i){
                    const amino::Math::vec3f& p = vertices[i];
                    velocities[i] = amino::Math::vec3f(sampler_u.sample<float>(p), sampler_v.sample<float>(p), sampler_w.sample<float>(p));
                }
            });
        }else if(surface.velocity_channels.count()==1){ // one vec3f velocity channel
            Bifrost::API::VoxelChannel velocity = component.findChannel(surface.velocity_channels[0].c_str());
            TBB_FOR_ALL(0, vertices.count(), 100, [&vertices, &velocity, &velocities](size_t start, size_t end){
                Bifrost::API::VoxelSampler sampler = velocity.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                for(size_t i = start; i < end; ++i){
                    velocities[i] = sampler.sample<amino::Math::vec3f>(vertices[i]);
                }
            });
        }else{
            AiMsgWarning("[BIFROST] Invalid velocity channels count = %d. Need 1 vec3f or 3 float. Disabling motion blur...", (int) surface.velocity_channels.count());
            shutter_start = shutter_end = 0;
            motion = false;
        }
        if(motion){
            AiNodeSetFlt(polymesh, "motion_start", shutter_start);
            AiNodeSetFlt(polymesh, "motion_end", shutter_end);
        }
    }

    {// export vertices
        AtArray *vlist = AiArrayAllocate( vertices.count(), motion? 2 : 1, AI_TYPE_VECTOR );
        if(shutter_start == 0){
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,&vertices](size_t i){
                AiArraySetVec(vlist, i, AminoVec3fToAtVector(vertices[i]));
            });
        }else{
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,&vertices,&velocities,shutter_start](size_t i){
                AiArraySetVec(vlist, i, AminoVec3fToAtVector(vertices[i] + velocities[i]*shutter_start));
            });
        }
        if(motion){
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,&vertices,&velocities,shutter_end](size_t i){
                AiArraySetVec(vlist, vertices.count()+i, AminoVec3fToAtVector(vertices[i] + velocities[i]*shutter_end));
            });
        }
        AiNodeSetArray(polymesh, "vlist", vlist);
    }

    AtArray *vidxs = AiArrayAllocate( indices.count()*3, 1, AI_TYPE_UINT );
    {// export faces
        AtArray *nsides = AiArrayAllocate( indices.count(), 1, AI_TYPE_UINT );
        TBB_FOR_ALL(0, indices.count(), 100, [vidxs, nsides, &indices](size_t i){
            AiArraySetUInt(nsides, i, 3);
            for(unsigned int j = 0; j < 3; ++j)
                AiArraySetUInt(vidxs, i*3+j, indices[i][j]);
        });
        AiNodeSetArray(polymesh, "nsides", nsides);
        AiNodeSetArray(polymesh, "vidxs", vidxs);
    }

    // export channels
    for(unsigned int i = 0; i < surface.channels.count(); ++i){
        Bifrost::API::VoxelChannel channel = component.findChannel(surface.channels[i].c_str());
        if(!channel.valid()){
            AiMsgWarning("[BIFROST] Skipping invalid channel '%s'. Available channels are: \n%s", surface.channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
        exportChannel(polymesh, vertices, channel);
    }

    if(!surface.uv_channel.empty()){// export principal uvs
        Bifrost::API::VoxelChannel uvs = component.findChannel(surface.uv_channel.c_str());
        const static std::function<bool(const Bifrost::API::Channel&)> validUVs = [](const Bifrost::API::Channel& c){
            return c.valid() && c.dataType() == Bifrost::API::DataType::FloatV2Type;
        };
        if(validUVs(uvs)){
            AtArray* uvlist = AiArrayAllocate( vertices.count(), 1, AI_TYPE_VECTOR2 );
            TBB_FOR_ALL(0, vertices.count(), 100, [&vertices, &uvs, uvlist](size_t start, size_t end){
                Bifrost::API::VoxelSampler sampler = uvs.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                for(size_t i = start; i < end; ++i){
                    AiArraySetVec2(uvlist, i, AminoVec2fToAtVector2(sampler.sample<amino::Math::vec2f>(vertices[i])));
                }
            });
            AiNodeSetArray( polymesh, "uvlist", uvlist );
            AiNodeSetArray( polymesh, "uvidxs", AiArrayCopy(vidxs) );
        }else{
            AiMsgWarning("[BIFROST] Skipping export invalid uv channel '%s'. Available channels are: \n%s", surface.uv_channel.c_str(), availableChannels(component, validUVs).c_str());
        }
    }

    // transfer displacement attributes
    AiNodeSetArray(polymesh, "disp_map", surface.disp_map);
    AiNodeSetFlt(polymesh, "disp_padding", surface.disp_padding);
    AiNodeSetFlt(polymesh, "disp_height", surface.disp_height);
    AiNodeSetFlt(polymesh, "disp_zero_value", surface.disp_zero_value);
    AiNodeSetBool(polymesh, "disp_autobump", surface.disp_autobump);

    *user_ptr = polymesh;

    return true;
}

node_parameters
{
    Polymesh().declare(params, nentry);
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
