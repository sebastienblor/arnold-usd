#include <ai.h>
#include <vector>
#include "debug.h"
#include "tbb.h"
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostprocessing/bifrostprocessing_surface.h>
#include "bifrost_surface.h"
#include "utils.h"

namespace{
    class Polymesh : public Surface{
    public:
        unsigned int subdivisions = 1;
        bool smoothing = true;

        AtArray* disp_map = nullptr;
        float disp_padding = 0;
        float disp_height = 0;
        float disp_zero_value = 0;
        bool disp_autobump = true;

        Polymesh(){}
        Polymesh(const AtNode* node) : Surface(node){
            GET_UINT(subdivisions);
            GET_BOOL(smoothing);
            disp_map = AiArrayCopy(AiNodeGetArray(node, "disp_map"));
            GET_FLT(disp_padding);
            GET_FLT(disp_height);
            GET_FLT(disp_zero_value);
            GET_BOOL(disp_autobump);
        }

        void declare(AtList *params, AtNodeEntry *nentry){
            Surface::declare(params,nentry);
            PARAM_UINT(subdivisions);
            PARAM_BOOL(smoothing);

            PARAM_ARRAY(disp_map, AiArrayAllocate(0, 1, AI_TYPE_NODE));
            PARAM_FLT(disp_padding);
            PARAM_FLT(disp_height);
            PARAM_FLT(disp_zero_value);
            PARAM_BOOL(disp_autobump);
        }

        virtual Bifrost::API::String str() const override{
            std::stringstream ss;
            ss << Surface::str().c_str();
            DUMP_PARAM(subdivisions);
            DUMP_PARAM(smoothing);

            DUMP_PARAM(disp_padding);
            DUMP_PARAM(disp_height);
            DUMP_PARAM(disp_zero_value);
            DUMP_PARAM(disp_autobump);
            return ss.str().c_str();
        }

        AtNode* polymesh(){
            float shutter_start, shutter_end;
            bool motion = getMotion(shutter_start, shutter_end);
            bool need_velocity = motion || (shutter_start != 0);

            Bifrost::API::Component comp = component();

            // **** CREATE POLYMESH ****

            Bifrost::API::Array<amino::Math::vec3f> vertices;
            Bifrost::API::Array<amino::Math::vec3i> indices;
            mesh(vertices, indices, subdivisions);

            AtNode *node = AiNode("polymesh");
            AiNodeSetBool(node, "smoothing", smoothing);

            // compute velocities
            Bifrost::API::Array<amino::Math::vec3f> velocities;
            if(need_velocity){
                velocities.resize(vertices.count());

                if(velocity_channels.count() == 3){ // 3 float velocity channels
                    Bifrost::API::VoxelChannel velocity_u = comp.findChannel(velocity_channels[0].c_str());
                    Bifrost::API::VoxelChannel velocity_v = comp.findChannel(velocity_channels[1].c_str());
                    Bifrost::API::VoxelChannel velocity_w = comp.findChannel(velocity_channels[2].c_str());
                    TBB_FOR_ALL(0, vertices.count(), 100, [&vertices, &velocity_u, &velocity_v, &velocity_w, &velocities](size_t start, size_t end){
                        Bifrost::API::VoxelSampler sampler_u = velocity_u.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                        Bifrost::API::VoxelSampler sampler_v = velocity_v.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                        Bifrost::API::VoxelSampler sampler_w = velocity_w.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                        for(size_t i = start; i < end; ++i){
                            const amino::Math::vec3f& p = vertices[i];
                            velocities[i] = amino::Math::vec3f(sampler_u.sample<float>(p), sampler_v.sample<float>(p), sampler_w.sample<float>(p));
                        }
                    });
                }else if(velocity_channels.count()==1){ // one vec3f velocity channel
                    Bifrost::API::VoxelChannel velocity = comp.findChannel(velocity_channels[0].c_str());
                    TBB_FOR_ALL(0, vertices.count(), 100, [&vertices, &velocity, &velocities](size_t start, size_t end){
                        Bifrost::API::VoxelSampler sampler = velocity.createSampler(Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace);
                        for(size_t i = start; i < end; ++i){
                            velocities[i] = sampler.sample<amino::Math::vec3f>(vertices[i]);
                        }
                    });
                }else{
                    AiMsgWarning("[BIFROST] Invalid velocity channels count = %d. Need 1 vec3f or 3 float. Disabling motion blur...", (int) velocity_channels.count());
                    shutter_start = shutter_end = 0;
                    motion = false;
                }
                if(motion){
                    AiNodeSetFlt(node, "motion_start", shutter_start);
                    AiNodeSetFlt(node, "motion_end", shutter_end);
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
                AiNodeSetArray(node, "vlist", vlist);
            }

            AtArray *vidxs = AiArrayAllocate( indices.count()*3, 1, AI_TYPE_UINT );
            {// export faces
                AtArray *nsides = AiArrayAllocate( indices.count(), 1, AI_TYPE_UINT );
                TBB_FOR_ALL(0, indices.count(), 100, [vidxs, nsides, &indices](size_t i){
                    AiArraySetUInt(nsides, i, 3);
                    for(unsigned int j = 0; j < 3; ++j)
                        AiArraySetUInt(vidxs, i*3+j, indices[i][j]);
                });
                AiNodeSetArray(node, "nsides", nsides);
                AiNodeSetArray(node, "vidxs", vidxs);
            }

            // export channels
            for(unsigned int i = 0; i < channels.count(); ++i){
                Bifrost::API::VoxelChannel channel = comp.findChannel(channels[i].c_str());
                if(!channel.valid()){
                    AiMsgWarning("[BIFROST] Skipping invalid channel '%s'. Available channels are: \n%s", channels[i].c_str(), availableChannels(comp).c_str());
                    continue;
                }
                exportChannel(node, vertices, channel);
            }

            if(!uv_channel.empty()){// export principal uvs
                Bifrost::API::VoxelChannel uvs = comp.findChannel(uv_channel.c_str());
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
                    AiNodeSetArray( node, "uvlist", uvlist );
                    AiNodeSetArray( node, "uvidxs", AiArrayCopy(vidxs) );
                }else{
                    AiMsgWarning("[BIFROST] Skipping export invalid uv channel '%s'. Available channels are: \n%s", uv_channel.c_str(), availableChannels(comp, validUVs).c_str());
                }
            }

            // transfer displacement attributes
            AiNodeSetArray(node, "disp_map", disp_map);
            AiNodeSetFlt(node, "disp_padding", disp_padding);
            AiNodeSetFlt(node, "disp_height", disp_height);
            AiNodeSetFlt(node, "disp_zero_value", disp_zero_value);
            AiNodeSetBool(node, "disp_autobump", disp_autobump);

            return node;
        }
    };
}

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPolymeshMtds)

procedural_init
{
    Polymesh polymesh(node);
    if(!polymesh.initialize())
        return false;
    return (*user_ptr = polymesh.polymesh()) != nullptr;
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
