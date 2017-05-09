#include <bifrostapi/bifrost_voxelchannel.h>

namespace Bifrost{
namespace Processing {

void createMarchingCubeMesh(const Bifrost::API::VoxelChannel& distance, Bifrost::API::Array<amino::Math::vec3f>& vertices, Bifrost::API::Array<amino::Math::vec3i>& indices, unsigned int subdivisions=1);

}} // Bifrost::Processing
