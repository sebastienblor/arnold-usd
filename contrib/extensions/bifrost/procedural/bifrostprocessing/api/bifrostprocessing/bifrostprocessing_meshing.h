#pragma once

#include <bifrostapi/bifrost_voxelchannel.h>

namespace Bifrost{
namespace Processing {

class Mesher{
public:
    virtual void mesh(const Bifrost::API::VoxelChannel& distance, Bifrost::API::Array<amino::Math::vec3f>& vertices, Bifrost::API::Array<amino::Math::vec3i>& indices, unsigned int subdivisions=1) const=0;
};

class MarchingCubes : public Mesher{
public:
    virtual void mesh(const API::VoxelChannel &distance, Bifrost::API::Array<amino::Math::vec3f> &vertices, Bifrost::API::Array<amino::Math::vec3i> &indices, unsigned int subdivisions) const override;
};

}} // Bifrost::Processing
