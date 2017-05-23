#include <ai.h>
#include <bifrostprocessing/bifrostprocessing_points.h>
#include <bifrostapi/bifrost_pointcomponent.h>
#include "defs.h"
#include "utils.h"
#include "debug.h"

#include "bifrost_shape.h"
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_pointchannel.h>

namespace{

class Points : public Bifrost::Processing::Points{
public:
    float step_size = .025;
    unsigned int chunk_size = 100000;
    enum Mode { Disk=0, Sphere=1 } mode = Sphere;

    Points() : Bifrost::Processing::Points(amino::Math::vec3f(0)) {}

    Points(const AtNode* node) : Bifrost::Processing::Points(getCameraPosition()) {
        GET_SHAPE();
        GET_FLT(radius);
        GET_BOOL(enable_radius_channel);
        GET_STR(radius_channel);

        GET_BOOL(enable_camera_radius);
        GET_VEC2(camera_distance);
        GET_VEC2(camera_factor);
        GET_FLT(camera_factor_exponent);

        GET_BOOL(enable_multi_pointing);
        GET_INT(mp_samples);
        GET_VEC2(mp_radius);
        GET_FLT(mp_surface_attract);
        GET_INT(mp_falloff_type);
        GET_VEC2(mp_falloff_range);
        GET_FLT(mp_displacement);
        GET_FLT(mp_displacement_noise_frequency);

        GET_INT(skip);

        GET_FLT(step_size);
        GET_UINT(chunk_size);
        GET_ENUM(mode, Points::Mode);
    }

    virtual Bifrost::Processing::Status initialize() override{
        return report(Bifrost::Processing::Points::initialize());
    }

    void declare(AtList* params, AtNodeEntry* nentry){
        PARAM_SHAPE(({}), ({ "velocity" }));

        PARAM_FLT(radius);
        PARAM_BOOL(enable_radius_channel);
        PARAM_STR(radius_channel);

        PARAM_BOOL(enable_camera_radius);
        PARAM_VEC2(camera_distance);
        PARAM_VEC2(camera_factor);
        PARAM_FLT(camera_factor_exponent);

        PARAM_BOOL(enable_multi_pointing);
        PARAM_INT(mp_samples);
        PARAM_VEC2(mp_radius);
        PARAM_FLT(mp_surface_attract);
        PARAM_INT(mp_falloff_type);
        PARAM_VEC2(mp_falloff_range);
        PARAM_FLT(mp_displacement);
        PARAM_FLT(mp_displacement_noise_frequency);

        PARAM_INT(skip);

        PARAM_FLT(step_size);
        PARAM_UINT(chunk_size);
        static const char* mode_enums[] = { "disk", "sphere" };
        PARAM_ENUM(mode, mode_enums);
    }

    virtual Bifrost::API::String str() const override{
        std::stringstream ss;
        ss << Bifrost::Processing::Points::str().c_str();

        DUMP_PARAM(step_size);
        DUMP_PARAM(chunk_size);
        DUMP_PARAM(mode);

        return ss.str().c_str();
    }

    bool nodes(std::vector<AtNode*>& nodes){
        float shutter_start, shutter_end;
        bool motion = getMotion(shutter_start, shutter_end);
        bool need_velocity = motion || (shutter_start != 0);

        Bifrost::API::PointComponent comp(component());

        Bifrost::API::Layout layout(comp.layout());
        AtNode *points_node = AiNode("points");

        unsigned int N = comp.elementCount();
        {// export positions
            AtArray* points_array = AiArrayAllocate(N, motion? 2 : 1, AI_TYPE_VECTOR);

            Bifrost::API::TileIterator it = layout.tileIterator(layout.maxDepth(), layout.maxDepth(), Bifrost::API::TraversalMode::BreadthFirst);
            Bifrost::API::PointChannel positions, velocities;
            positions = comp.findChannel("position");
            if(need_velocity) velocities = comp.findChannel("velocity");
            Bifrost::API::TileData<amino::Math::vec3f> positionData, velocityData;
            unsigned int i = 0;
            for(;it;++it){
                positionData = positions.tileData<amino::Math::vec3f>((*it).index());
                if(need_velocity) velocityData = velocities.tileData<amino::Math::vec3f>((*it).index());
                for(unsigned int e = 0; e < positionData.count(); ++e, ++i){
                    if(motion){
                        AiArraySetVec(points_array,   i, AminoVec3fToAtVector((positionData[e]*layout.voxelScale() + shutter_start*velocityData[e])));
                        AiArraySetVec(points_array, i+N, AminoVec3fToAtVector((positionData[e]*layout.voxelScale() + shutter_end*velocityData[e])));
                    }else if(need_velocity){
                        AiArraySetVec(points_array,   i, AminoVec3fToAtVector((positionData[e]*layout.voxelScale() + shutter_start*velocityData[e])));
                    }else{
                        AiArraySetVec(points_array,   i, AminoVec3fToAtVector((positionData[e])*layout.voxelScale()));
                    }
                }
            }
            AiNodeSetArray(points_node, "points", points_array);
        }

        {// export radius
            AtArray* radius_array = AiArrayAllocate(comp.elementCount(), 2, AI_TYPE_FLOAT);
            for(unsigned int i = 0; i < comp.elementCount()<<1; ++i)
                AiArraySetFlt(radius_array, i, .025);
            AiNodeSetArray(points_node, "radius", radius_array);
        }

        {// export channels

        }

        AiNodeSetInt(points_node, "mode", mode);
        AiNodeSetFlt(points_node, "step_size", step_size);

        nodes.push_back(points_node);
        return true;
    }
};

}

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPointsMtds)

procedural_init
{
    Points points(node);
    if(!points.initialize())
        return false;
    std::vector<AtNode*>* nodes = new std::vector<AtNode*>();
    if(!points.nodes(*nodes) || nodes->size() == 0){
        delete nodes;
        return false;
    }
    *user_ptr = nodes;
    return true;
}
procedural_cleanup
{
    if(user_ptr) delete static_cast< std::vector<AtNode*>* >(user_ptr);
    return true;
}
procedural_num_nodes
{
    return user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr)->size() : 0;
}
procedural_get_node
{
    std::vector<AtNode*>* nodes = user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr) : nullptr;
    return (nodes && i >= 0 && i < nodes->size())? (*nodes)[i] : nullptr;
}
node_parameters
{
    Points().declare(params,nentry);
}

