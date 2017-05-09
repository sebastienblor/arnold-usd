#pragma once

#include <aminomath/vec.h>
#include <ai_vector.h>
#include <string>
#include <sstream>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_channel.h>
#include <functional>

inline amino::Math::vec2f AtVector2ToAminoVec2(const AtVector2& v){ return amino::Math::vec2f(v.x, v.y); }
inline amino::Math::vec3f AtVectorToAminoVec3(const AtVector& v){ return amino::Math::vec3f(v.x, v.y, v.z); }
inline AtVector2 AminoVec2ToAtVector(const amino::Math::vec2f& v){ return AtVector2(v[0],v[1]); }
inline AtVector AminoVec3ToAtVector(const amino::Math::vec3f& v){ return AtVector(v[0],v[1],v[2]); }

inline std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter=[](const Bifrost::API::Channel&){ return true; }){
    Bifrost::API::RefArray channels = component.channels();
    std::stringstream ss;
    for(unsigned int i = 0; i < channels.count(); ++i)
        if(filter(channels[i]))
            ss << "    " << Bifrost::API::Channel(channels[i]).name() << std::endl;
    return ss.str();
}

