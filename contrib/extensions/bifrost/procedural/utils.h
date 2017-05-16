#pragma once

#include <aminomath/vec.h>
#include <ai_vector.h>
#include <string>
#include <sstream>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_channel.h>
#include <functional>
#include <ai_matrix.h>

inline amino::Math::vec2f AtVector2ToAminoVec2f(const AtVector2& v){ return amino::Math::vec2f(v.x, v.y); }
inline amino::Math::vec3f AtVectorToAminoVec3f(const AtVector& v){ return amino::Math::vec3f(v.x, v.y, v.z); }
inline AtVector2 AminoVec2fToAtVector2(const amino::Math::vec2f& v){ return AtVector2(v[0],v[1]); }
inline AtVector AminoVec3fToAtVector(const amino::Math::vec3f& v){ return AtVector(v[0],v[1],v[2]); }
inline AtVector2 AminoVec2iToAtVector2(const amino::Math::vec2i& v){ return AtVector2(v[0],v[1]); }
inline AtVector AminoVec3iToAtVector(const amino::Math::vec3i& v){ return AtVector(v[0],v[1],v[2]); }

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter=[](const Bifrost::API::Channel&){ return true; });

std::ostream& operator<<(std::ostream& out, const AtMatrix& m);
