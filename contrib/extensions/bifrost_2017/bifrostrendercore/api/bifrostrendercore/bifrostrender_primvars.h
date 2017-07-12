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

#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_string.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_component.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_types.h"

namespace Bifrost{
namespace RenderCore{

void BIFROSTRENDERAPI_DECL createPrimVarChannel(	Bifrost::API::String primVar, Bifrost::API::DataType dataType,
							Bifrost::API::StateServer& ss, Bifrost::API::Component& component,
							Bifrost::API::VoxelOffsetType offsetType, float defVal);

void BIFROSTRENDERAPI_DECL createPrimVarChannels( std::vector<primVarInfo>& primVars,
							Bifrost::API::StateServer& ss,
							Bifrost::API::Component& component,
							Bifrost::API::VoxelOffsetType offsetType,
							float defVal);

void BIFROSTRENDERAPI_DECL	declarePrimVars(std::vector<primVarInfo>& primVars,
											std::vector<CvToken>& toks,
											std::vector<CvPointer>& ptr,
											std::vector<int>& varIndex,
											int nofElements,
											std::vector<void *>& mem,
											TokenClass tokenClass,
											std::string getPrimVarTokenCB ( TokenClass, Bifrost::API::DataType, std::string ) );

void BIFROSTRENDERAPI_DECL assignPrimVars( std::vector<primVarInfo>& primVars, int xExport, int dataIndex, int samples);

void BIFROSTRENDERAPI_DECL outputPrimVarsToConsole ( std::vector<CvToken>& toks );

Bifrost::API::Channel BIFROSTRENDERAPI_DECL checkPrimVarAndMakeDuplicate( std::string channelName, Bifrost::API::StateServer srcSS, Bifrost::API::Component component, char *inputChannelName );

Bifrost::API::String BIFROSTRENDERAPI_DECL getPrimVarTypeString( int32_t dataType );

size_t BIFROSTRENDERAPI_DECL getPrimVarElementCount ( int32_t dataType );

template<typename T>
void initPVarInfo( T *frameData );

void BIFROSTRENDERAPI_DECL checkPrimVars( FrameData *frameData, Bifrost::API::StateServer srcSS, Bifrost::API::Component component, char *inputChannelName, DiagnosticsParams diagnostics );

// analyze the primvar input string and output a return a valid list
void BIFROSTRENDERAPI_DECL getFinalPrimVars(	Bifrost::API::String bifFilename,
												Bifrost::API::String primVarNames,
												int DEBUG,
												Bifrost::API::StringArray& finalPrimVarNames );

}}
