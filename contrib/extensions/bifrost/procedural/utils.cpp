#include "utils.h"
#include <ai_msg.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include "tbb.h"
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <ai_universe.h>

std::string typeName(uint8_t type){
    switch (type) {
    case AI_TYPE_FLOAT:   return "FLOAT";
    case AI_TYPE_VECTOR2: return "VECTOR2";
    case AI_TYPE_VECTOR:  return "VECTOR";
    case AI_TYPE_INT:     return "INT";
    default: break;
    }
    return "";
}

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

std::string ArrayToString(const Bifrost::API::StringArray& strings){
    std::stringstream ss;
    ss << "{ ";
    for(unsigned int i = 0; i < strings.count()-1; ++i)
        ss << "\"" << strings[i] << "\", ";
    ss << "\"" << strings[strings.count()-1] << "\"}";
    return ss.str();
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
            out << m[i][j] << " ";
        }
        out << "]";
    }
    out << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const AtBBox& bbox){
    out << "AtBBox[[" << bbox.min.x << ", " << bbox.min.y << ", " << bbox.min.z << "],";
    return out << "[" << bbox.max.x << ", " << bbox.max.y << ", " << bbox.max.z << "]]";
}

std::ostream& operator<<(std::ostream& out, const AtVector& vec){
    return out << "AtVector(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}
