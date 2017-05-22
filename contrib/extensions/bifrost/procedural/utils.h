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
#include "defs.h"

bool getMotion(float& shutter_start, float& shutter_end);

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter=[](const Bifrost::API::Channel&){ return true; });

std::ostream& operator<<(std::ostream& out, const AtMatrix& m);

void exportChannel(AtNode *node, const Bifrost::API::Array<amino::Math::vec3f> &positions, const Bifrost::API::VoxelChannel &channel);
