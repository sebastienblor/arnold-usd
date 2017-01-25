//*****************************************************************************
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2016
// Written by Erdem Taylan

#ifndef _AITOOLS_H_
#define _AITOOLS_H_

#include <map>

#include <bifrostapi/bifrost_voxelsampler.h>

#include <ai.h>

#include <bifrostrendercore/bifrostrender_types.h>
#include <Types.h>

void getASSData ( void *frameData, void *inData, Bifrost::RenderCore::PluginType pluginType );
Bifrost::API::VoxelSampler * getAndSetThreadDataAI( std::string idString, Bifrost::API::VoxelChannel channel, void *usrData );
Bifrost::RenderCore::CvFloat calcNoiseAI( Bifrost::RenderCore::CvPoint3 noiseP );
void transformPointsAI( Bifrost::RenderCore::CvPoint finalWp[2], float shutterStart, void *usrData );
void setExportTokenAI( std::vector<Bifrost::RenderCore::CvToken>& toks, Bifrost::RenderCore::ExportTokens token );
std::string getPrimVarTokenAI ( Bifrost::RenderCore::TokenClass tokenClass, Bifrost::API::DataType dataType, std::string primVarName );

#endif
