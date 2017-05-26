#include "polymesh.h"
#include <ai.h>
#include <vector>
#include "debug.h"
#include "tbb.h"
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostprocessing/bifrostprocessing_meshers.h>
#include <bifrostprocessing/bifrostprocessing_surface.h>
#include "utils.h"
#include "sampler.h"

PolymeshParameters::PolymeshParameters() : SurfaceParameters() {}

PolymeshParameters::PolymeshParameters(const AtNode* node) : SurfaceParameters(node){
    GET_UINT(subdivisions);
    GET_BOOL(smoothing);
    disp_map = AiArrayCopy(AiNodeGetArray(node, "disp_map"));
    GET_FLT(disp_padding);
    GET_FLT(disp_height);
    GET_FLT(disp_zero_value);
    GET_BOOL(disp_autobump);
}

void PolymeshParameters::declare(AtList *params, AtNodeEntry *nentry){
    SurfaceParameters::declare(params,nentry);
    AiParameterStr("field_channel", distance_channel.c_str());
    PARAM_UINT(subdivisions);
    PARAM_BOOL(smoothing);

    PARAM_ARRAY(disp_map, AiArrayAllocate(0, 1, AI_TYPE_NODE));
    PARAM_FLT(disp_padding);
    PARAM_FLT(disp_height);
    PARAM_FLT(disp_zero_value);
    PARAM_BOOL(disp_autobump);
}

Bifrost::API::String PolymeshParameters::str() const{
    std::stringstream ss;
    ss << SurfaceParameters::str().c_str();
    DUMP_PARAM(subdivisions);
    DUMP_PARAM(smoothing);

    DUMP_PARAM(disp_padding);
    DUMP_PARAM(disp_height);
    DUMP_PARAM(disp_zero_value);
    DUMP_PARAM(disp_autobump);
    return ss.str().c_str();
}

AtNode* PolymeshParameters::node() const{
    Bifrost::Processing::Surface surface(*this);
    DUMP(this->str());
    report(surface.status());
    if(!surface.status()){
        return nullptr;
    }
    Bifrost::API::Component component = surface.voxels();

    float shutter_start, shutter_end;
    bool motion = getMotion(shutter_start, shutter_end);
    bool need_velocity = motion || (shutter_start != 0);

    // **** CREATE POLYMESH ****

    Bifrost::API::Array<amino::Math::vec3f> vertices;
    Bifrost::API::Array<amino::Math::vec3i> indices;
    surface.mesh(vertices, indices, subdivisions);

    AtNode *node = AiNode("polymesh");
    AiNodeSetBool(node, "smoothing", smoothing);

    {// export vertices
        ChannelSampler velocities;
        if(need_velocity){
            velocities = ChannelSampler(component, velocity_channel);
            if(!velocities.valid()){
                motion = need_velocity = false;
                shutter_start = shutter_end = 0;
                AiMsgWarning("[BIFROST] Invalid velocity channel: %s. Ignoring motion...", velocity_channel.c_str());
            }
        }

        AtArray *vlist = AiArrayAllocate( vertices.count(), motion? 2 : 1, AI_TYPE_VECTOR );
        if(need_velocity){
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,&vertices,&velocities,motion,shutter_start,shutter_end](size_t start, size_t end){
                ChannelSampler vsampler(velocities);
                AtParamValue value;
                for(size_t i = start; i < end; ++i){
                    AtVector pos(Convert(vertices[i]));
                    vsampler.sample(pos, &value);
                    AtVector vel(value.VEC());
                    AiArraySetVec(vlist, i, pos + shutter_start * vel);
                    if(motion) AiArraySetVec(vlist, vertices.count()+i, pos + shutter_end * vel);
                }
            });
            AiNodeSetFlt(node, "motion_start", shutter_start);
            AiNodeSetFlt(node, "motion_end", shutter_end);
        }else{
            TBB_FOR_ALL(0, vertices.count(), 100, [vlist,&vertices](size_t i){
                AiArraySetVec(vlist, i, Convert(vertices[i]));
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
        ChannelSampler sampler(component, channels[i]);
        if(!sampler.valid()){
            AiMsgWarning("[BIFROST] Don't know how to export channel '%s', Available channels are: %s", channels[i].c_str(), availableChannels(component).c_str());
            continue;
        }
        AiNodeDeclare(node, channels[i].c_str(), (std::string("varying ")+typeName(sampler.type())).c_str()); \
        AiNodeSetArray(node, channels[i].c_str(), sampler.array(vertices));
    }

    if(!uv_channel.empty()){// export principal uvs
        Bifrost::API::VoxelChannel uvs = component.findChannel(uv_channel.c_str());
        const static std::function<bool(const Bifrost::API::Channel&)> validUVs = [](const Bifrost::API::Channel& c){
            return c.valid() && c.dataType() == Bifrost::API::DataType::FloatV2Type;
        };
        if(validUVs(uvs)){
            AtArray* uvlist = ChannelSampler(uvs).array(vertices);
            AiNodeSetArray( node, "uvlist", uvlist );
            AiNodeSetArray( node, "uvidxs", AiArrayCopy(vidxs) );
        }else{
            AiMsgWarning("[BIFROST] Skipping export invalid uv channel '%s'. Available channels are: \n%s", uv_channel.c_str(), availableChannels(component, validUVs).c_str());
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

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPolymeshMtds)

node_parameters
{
    PolymeshParameters().declare(params, nentry);
}
procedural_init
{
    return (*user_ptr = PolymeshParameters(node).node()) != nullptr;
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
