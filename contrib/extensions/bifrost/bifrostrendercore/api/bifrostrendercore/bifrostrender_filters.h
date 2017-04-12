//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2015
// Written by Erdem Taylan

#pragma once

#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_component.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_types.h"
#include "bifrostrender_visitors.h"

namespace Bifrost{
namespace RenderCore{

// dilate the channel
void BIFROSTRENDERAPI_DECL dilateChannel( FrameData *frameData, float dilateAmount, BifrostRenderType renderType );

// erode the channel
void BIFROSTRENDERAPI_DECL erodeChannel( FrameData *frameData, float erodeAmount, BifrostRenderType renderType );

// apply smoothing filter to a channel
void BIFROSTRENDERAPI_DECL smoothChannel(	SmoothFilterParams smooth, 
											Bifrost::API::StateServer srcSS,
											Bifrost::API::VoxelChannel inChannel,
											FrameData *frameData,
											DiagnosticsParams diagnostics,
											BifrostRenderType renderType );

// displace distance channels
void BIFROSTRENDERAPI_DECL displaceChannel( InfCubeParams infCube, Bifrost::API::StateServer srcSS, FrameData *frameData, DiagnosticsParams diagnostics );

// extend bounds of the sim domain
void BIFROSTRENDERAPI_DECL extendChannel( InfCubeParams infCube, Bifrost::API::StateServer srcSS, FrameData *frameData, DiagnosticsParams diagnostics );

}}

