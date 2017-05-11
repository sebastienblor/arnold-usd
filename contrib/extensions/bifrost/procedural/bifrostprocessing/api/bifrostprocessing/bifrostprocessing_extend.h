#include <bifrostapi/bifrost_voxelchannel.h>

namespace Bifrost{
namespace Processing {

void extend(const Bifrost::API::VoxelChannel& sdf, float height, const amino::Math::vec2f& center, const amino::Math::vec2f& dimensions, float radius, Bifrost::API::VoxelChannel &out);

void extendUVs(const amino::Math::vec2f& center, const amino::Math::vec2f& dimensions, Bifrost::API::VoxelChannel& out);

}}
