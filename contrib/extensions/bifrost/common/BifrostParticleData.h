#pragma once
#include <string>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_refarray.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>
#include "ai.h"
namespace
{

// Particle channels to be translated to Arnold
struct ParticleChannels
{
   std::string             declaration;
   float                   radius;     // Global
   AtRGB                   color;      // Global
   Bifrost::API::PointChannel positionChannel;
   Bifrost::API::PointChannel velocityChannel;
   Bifrost::API::RefArray     extraChannels;
};


}