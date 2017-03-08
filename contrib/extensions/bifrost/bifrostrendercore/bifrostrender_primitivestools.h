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

#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_types.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_visitor.h>

#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>
#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_dictionary.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#ifndef _DARWIN
#include <malloc.h>
#endif
#include <ctime>

#include "bifrostrender_decl.h"
#include "bifrostrender_types.h"
#include "bifrostrender_tools.h"
#include "bifrostrender_visitors.h"
#include "bifrostrender_primvars.h"

namespace Bifrost{
namespace RenderCore{

float BIFROSTRENDERAPI_DECL calcPointDistance(CvPoint start, CvPoint end);
float BIFROSTRENDERAPI_DECL calcCamDistance(CvPoint location);
float BIFROSTRENDERAPI_DECL calcCamMulFactor( PrimitivesInputData *inData, float distance);

void BIFROSTRENDERAPI_DECL checkPrimitivesPrimVars( PrimitivesFrameData *frameData, Bifrost::API::Component component );

bool BIFROSTRENDERAPI_DECL checkSpeed(	PrimitivesInputData *inData, PrimitivesFrameData *frameData, Bifrost::API::BIF::FileInfo& fInfo );
void BIFROSTRENDERAPI_DECL checkVelocity( PrimitivesFrameData *frameData, Bifrost::API::BIF::FileInfo& fInfo );
void BIFROSTRENDERAPI_DECL checkInputChannel( const Bifrost::API::Channel channel, float& minVal, float& maxVal );

void BIFROSTRENDERAPI_DECL reportPrimVars( PrimitivesFrameData *frameData );
void BIFROSTRENDERAPI_DECL reportIdRange( const Bifrost::API::Channel& idChan );
void BIFROSTRENDERAPI_DECL reportPrimitivesBounds( Bifrost::API::Channel channel, float voxelScale, DiagnosticsParams diagnostics );

void BIFROSTRENDERAPI_DECL initPrimitivesPVarInfo( PrimitivesFrameData *frameData, Bifrost::API::Component component );

void BIFROSTRENDERAPI_DECL initAndGetPrimitivesFrameData( PrimitivesFrameData *frameData, PrimitivesInputData *inData, void getRenderDataCB( void *, void *, PluginType ) );

int BIFROSTRENDERAPI_DECL dumpPrimitives	(	PrimitivesInputData *inData,
												PrimitivesFrameData *frameData,
												Bifrost::API::Component component,
												const Bifrost::API::Channel& posChan,
												const Bifrost::API::Channel& inputChan,
												const Bifrost::API::Channel& velChan,
												const Bifrost::API::Channel& velChanU,
												const Bifrost::API::Channel& velChanV,
												const Bifrost::API::Channel& velChanW,
												const Bifrost::API::Channel& idChan,
												const Bifrost::API::Channel& smokeChan,
												const Bifrost::API::Channel& vorticityChan,
												const Bifrost::API::Channel& distanceChan,
												void *usrData,
												CvFloat calcNoiseCB( CvPoint3 ),
												void exportPrimitivesCB(	PrimitivesInputData *,
																			PrimitivesFrameData *,
																			int, std::vector<CvToken>&, std::vector<CvPointer>&, std::vector<int>&,
																			CvFloat *, CvFloat *, CvFloat *, CvFloat *, CvInt *, CvInt *,
																			void * ),
												void transformPointsCB( CvPoint [2], CvFloat, void * ),
												Bifrost::API::VoxelSampler *getAndSetThreadDataCB( std::string, Bifrost::API::VoxelChannel, void * ),
												void setExportTokenCB( std::vector<CvToken>&, ExportTokens ),
												std::string getPrimVarTokenCB( TokenClass, Bifrost::API::DataType, std::string )
											);

void BIFROSTRENDERAPI_DECL assignPrimitivePrimVars( std::vector<primVarInfo>& primVars, int xExport, int dataIndex, int samples, bool isPointCache, PrimitivesInputData *inData, PrimitivesFrameData *frameData);
CvFloat BIFROSTRENDERAPI_DECL *getPrimitivePrimVar( std::vector<primVarInfo>& primVars, std::vector<int>& varIndex, int var, int dataIndex);

}}

