#pragma once

#include <aminomath/vec.h>
#include <ai_vector.h>
#include <string>
#include <sstream>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <functional>
#include <ai_matrix.h>
#include <ai_nodes.h>
#include <ai.h>
#include <vector>
#include "defs.h"
#include <bifrostprocessing/bifrostprocessing_shape.h>

inline amino::Math::vec2f Convert(const AtVector2& v){ return amino::Math::vec2f(v.x, v.y); }
inline amino::Math::vec3f Convert(const AtVector& v){ return amino::Math::vec3f(v.x, v.y, v.z); }
inline AtVector2 Convert(const amino::Math::vec2f& v){ return AtVector2(v[0],v[1]); }
inline AtVector Convert(const amino::Math::vec3f& v){ return AtVector(v[0],v[1],v[2]); }
inline AtVector2 Convert(const amino::Math::vec2i& v){ return AtVector2(v[0],v[1]); }
inline AtVector Convert(const amino::Math::vec3i& v){ return AtVector(v[0],v[1],v[2]); }
inline AtBBox Convert(const amino::Math::bboxf& bbox){
    return AtBBox(Convert(bbox.min()), Convert(bbox.max()));
}
Bifrost::API::StringArray ArrayToStrings(const AtArray* array);
AtArray* CreateStringArray(const std::vector<std::string>& strings);
std::string ArrayToString(const Bifrost::API::StringArray& strings);

std::string typeName(uint8_t type);

Bifrost::Processing::Status report(const Bifrost::Processing::Status& status);

bool getMotion(float& shutter_start, float& shutter_end);

amino::Math::vec3f getCameraPosition();

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter=[](const Bifrost::API::Channel&){ return true; });

std::ostream& operator<<(std::ostream& out, const AtMatrix& m);
std::ostream& operator<<(std::ostream& out, const AtBBox& bbox);
