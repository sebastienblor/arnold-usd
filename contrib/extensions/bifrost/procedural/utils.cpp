#include "utils.h"
#include <ai_msg.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include "tbb.h"
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <ai_universe.h>

Bifrost::Processing::Status report(const Bifrost::Processing::Status& status){
    for(unsigned int i = 0; i < status.warnings().count(); ++i){
        AiMsgWarning("[BIFROST] %s", status.warnings()[i].c_str());
    }
    if(!status){
        AiMsgError("[BIFROST] %s", status.error().c_str());
    }
    return status;
}

Bifrost::API::StringArray ArrayToStrings(const AtArray* array){
    Bifrost::API::StringArray strings;
    for(unsigned int i = 0; i < AiArrayGetNumElements(array); ++i)
        strings.add(AiArrayGetStr(array,i).c_str());
    return strings;
}

AtArray* CreateStringArray(const std::vector<std::string>& strings){
    AtArray* array = AiArrayAllocate(strings.size(),1,AI_TYPE_STRING);
    for(unsigned int i = 0; i < strings.size(); ++i)
        AiArraySetStr(array, i, strings[i].c_str());
    return array;
}

bool getMotion(float &shutter_start, float &shutter_end){
    AtNode* options = AiUniverseGetOptions();
    if(AiNodeGetBool(options, "ignore_motion_blur")){
        shutter_start = shutter_end = AiNodeGetFlt(options, "reference_time");
    }else{
        AtNode* camera = AiUniverseGetCamera();
        shutter_start = AiNodeGetFlt(camera, "shutter_start");
        shutter_end = AiNodeGetFlt(camera, "shutter_end");
    }
    if(shutter_end < shutter_start){ shutter_end = shutter_start; }
    return shutter_start != shutter_end;
}

amino::Math::vec3f getCameraPosition(){
    return amino::Math::vec3f();
}

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter){
    Bifrost::API::RefArray channels = component.channels();
    std::stringstream ss;
    for(unsigned int i = 0; i < channels.count(); ++i)
        if(filter(channels[i]))
            ss << "    " << Bifrost::API::Channel(channels[i]).name() << std::endl;
    return ss.str();
}

std::ostream& operator<<(std::ostream& out, const AtMatrix& m){
    out << "AtMatrix[";
    for(unsigned int i = 0; i < 4; ++i){
        out << "[";
        for(unsigned int j = 0; j < 4; ++j){
            out << m[i][j];
        }
        out << "]";
    }
    out << "]";
    return out;
}

namespace {

    template<typename T>
    void exportChannel(AtNode* node, const Bifrost::API::Array<amino::Math::vec3f>& positions, const Bifrost::API::VoxelChannel& channel);

#define EXPORT_CHANNEL(T, type, declaration, AiArraySet, CAST) \
    template<> void exportChannel<T>(AtNode* node, const Bifrost::API::Array<amino::Math::vec3f>& positions, const Bifrost::API::VoxelChannel& channel){ \
        AiNodeDeclare(node, channel.name().c_str(), declaration); \
        AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type); \
        TBB_FOR_ALL(0, positions.count(), 100, [positions, channel, channelArray](size_t start, size_t end){ \
            Bifrost::API::VoxelSampler sampler = channel.createSampler(Bifrost::API::VoxelSamplerLinearType, Bifrost::API::SamplerSpace::WorldSpace); \
            for(size_t i = start; i < end; ++i){ \
                 AiArraySet(channelArray, i, CAST(sampler.sample<T>(positions[i]))); \
            } \
        }); \
        AiNodeSetArray(node, channel.name().c_str(), channelArray); \
    }

#define EXPORT_ICHANNEL(T, type, declaration, AiArraySet, CAST) \
    template<> void exportChannel<T>(AtNode* node, const Bifrost::API::Array<amino::Math::vec3f>& positions, const Bifrost::API::VoxelChannel& channel){ \
        AiNodeDeclare(node, channel.name().c_str(), declaration); \
        AtArray *channelArray = AiArrayAllocate(positions.count(), 1, type);\
        Bifrost::API::Layout layout(channel.layout());\
        const Bifrost::API::TileAccessor accessor = layout.tileAccessor();\
        float invDx = 1./layout.voxelScale();\
        int N = layout.tileDimInfo().tileWidth; \
        TBB_FOR_ALL(0, positions.count(), 100, [positions, channel, channelArray, layout, accessor, invDx, N](size_t e){\
            amino::Math::vec3f pos = positions[e]*invDx;\
            const Bifrost::API::Tile& tile = accessor.tile(pos[0], pos[1], pos[2], layout.maxDepth());\
            const Bifrost::API::TileCoord& coord = tile.coord();\
            AiArraySet(channelArray, e, CAST(channel.tileData<T>(tile.index())((int)(pos[0]-coord.i)%N, (int)(pos[1]-coord.j)%N, (int)(pos[2]-coord.k)%N)));\
        });\
        AiNodeSetArray(node, channel.name().c_str(), channelArray);\
    }

    // export channel specialization per type
    EXPORT_CHANNEL(float, AI_TYPE_FLOAT, "varying FLOAT", AiArraySetFlt,)
    EXPORT_CHANNEL(amino::Math::vec2f, AI_TYPE_VECTOR2, "varying VECTOR2", AiArraySetVec2, AminoVec2fToAtVector2)
    EXPORT_CHANNEL(amino::Math::vec3f, AI_TYPE_VECTOR, "varying VECTOR", AiArraySetVec, AminoVec3fToAtVector)
    EXPORT_ICHANNEL(int, AI_TYPE_INT, "varying INT", AiArraySetInt, )
    EXPORT_ICHANNEL(amino::Math::vec2i, AI_TYPE_VECTOR2, "varying VECTOR2", AiArraySetVec2, AminoVec2iToAtVector2)
    EXPORT_ICHANNEL(amino::Math::vec3i, AI_TYPE_VECTOR, "varying VECTOR", AiArraySetVec, AminoVec3iToAtVector)

}

void exportChannel(AtNode *node, const Bifrost::API::Array<amino::Math::vec3f> &positions, const Bifrost::API::VoxelChannel &channel){
    switch(channel.dataType()){
    case Bifrost::API::DataType::FloatType:   exportChannel<float>(node, positions, channel); return;
    case Bifrost::API::DataType::FloatV2Type: exportChannel<amino::Math::vec2f>(node, positions, channel); return;
    case Bifrost::API::DataType::FloatV3Type: exportChannel<amino::Math::vec3f>(node, positions, channel); return;
    case Bifrost::API::DataType::Int32Type:   exportChannel<int>(node, positions, channel); return;
    case Bifrost::API::DataType::Int32V2Type: exportChannel<amino::Math::vec2i>(node, positions, channel); return;
    case Bifrost::API::DataType::Int32V3Type: exportChannel<amino::Math::vec3i>(node, positions, channel); return;
    default: AiMsgWarning("[BIFROST] Unknown conversion for exporting channel '%s' (type = %d)", channel.name().c_str(), channel.dataType());
    }
}
