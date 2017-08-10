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
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>
#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_dictionary.h>
#include <bifrostapi/bifrost_array.h>
#include <bifrostapi/bifrost_haloedcache.h>
#include <bifrostapi/bifrost_voxelcomponent.h>
#include <bifrostapi/bifrost_pointcomponent.h>
#include <bifrostapi/bifrost_levelset.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_defs.h"
#include "bifrostrender_types.h"
#include "bifrostrender_objectuserdata.h"

namespace Bifrost{
namespace RenderCore{

// utilities
void BIFROSTRENDERAPI_DECL assembleStringFromArray( Bifrost::API::StringArray inArray, Bifrost::API::String& out );
void BIFROSTRENDERAPI_DECL printStringArray( Bifrost::API::StringArray strArr );
void BIFROSTRENDERAPI_DECL printEndOutput( const char *msg, DiagnosticsParams diagnostics );
void BIFROSTRENDERAPI_DECL dumpStateServer( Bifrost::API::StateServer srcSS, const char *inMsg );

// get voxel offset value
amino::Math::vec3f BIFROSTRENDERAPI_DECL getVoxelOffset ( Bifrost::API::VoxelChannel inputChan );

// This function calculates bounding box of the bifrost cache
void BIFROSTRENDERAPI_DECL computeImplicitsBounds( FrameData *frameData, InfCubeParams infCube, DiagnosticsParams diagnostics );
void BIFROSTRENDERAPI_DECL computeVolumeBounds( float bbox[6], FrameData *frameData, DiagnosticsParams diagnostics );
void BIFROSTRENDERAPI_DECL computeIsosurfaceBounds( const Bifrost::API::Channel& channel, double bboxMin[3], double bboxMax[3] );
void BIFROSTRENDERAPI_DECL computeAeroBounds( const Bifrost::API::Channel& channel, double bboxMin[3], double bboxMax[3] );

// this function calculates a frustum bound
void BIFROSTRENDERAPI_DECL computeHullCorners( FrameData *frameData );

// this function converts a SDF channel into a narrow band fog representation
void BIFROSTRENDERAPI_DECL channelToNarrowBandFog ( Bifrost::API::VoxelChannel &inChannel, float narrowBandThicknessInVoxels, FrameData *frameData, bool countStats, DiagnosticsParams diagnostics );

// clear stateserver
void BIFROSTRENDERAPI_DECL clearStateServer( Bifrost::API::StateServer inSS );

// cleanup object in a stateserver
void BIFROSTRENDERAPI_DECL deleteObject( Bifrost::API::StateServer& stateServer, const Bifrost::API::Object& object );

// hash stuff
unsigned long BIFROSTRENDERAPI_DECL hash( const uint8_t *bufPtr, unsigned long range = 65535 );
unsigned long BIFROSTRENDERAPI_DECL imod( const unsigned long a, const unsigned long b );
std::string BIFROSTRENDERAPI_DECL toString( unsigned long a );
std::string BIFROSTRENDERAPI_DECL toStringPtr( uintptr_t a );

// allocate arrays for min max data
void BIFROSTRENDERAPI_DECL allocMinMaxArray(float **min, float **max, int i, int nofTiles);

// calc array structures for min max data
void BIFROSTRENDERAPI_DECL calcMinMaxArray(Bifrost::API::Layout& layout, Bifrost::API::VoxelChannel& channel, float **min, float **max, DiagnosticsParams diagnostics);

// calc coords of hull corners
// order is top left, top right, bottom right, botom left ( same for near and far planes)
void BIFROSTRENDERAPI_DECL calcHullCornerCoords( CvPoint corners[8], int hullCount, float screenWindow[4], void xformCB( const char *, const char *, int, CvPoint *, int) );

// calc normals corresponding to 6 surface of the viewing frustum
void BIFROSTRENDERAPI_DECL calcHullNormals( CvPoint hullCorners[8], FrustumPlane planes[6] );

// return whether the rangeCorners are inside the hullCorners
bool BIFROSTRENDERAPI_DECL boundIntersectsWithFrustum ( const CvPoint corners[8], FrustumPlane planes[6] );

// return whether a point is inside the hullCorners
bool BIFROSTRENDERAPI_DECL pointInFrustum ( amino::Math::vec3f wp, FrustumPlane planes[6] );

// calc a falloff factor depending on distance from sim center
float BIFROSTRENDERAPI_DECL calcPositionFalloff ( amino::Math::vec3f pos, FrameData *frameData );

// calc a depth for pruning
float BIFROSTRENDERAPI_DECL calcCullingDepth ( amino::Math::vec3f pos, FrameData *frameData, CullSidesParams cullSides );

// write hot data to disk
Bifrost::API::String BIFROSTRENDERAPI_DECL writeHotDataToDisk(	CoreObjectUserData& objectRef,
																Bifrost::API::String filename,
																Bifrost::API::String componentName,
																Bifrost::API::String& writeToFolder );

// process various things about frame we are rendering
void BIFROSTRENDERAPI_DECL initAndGetFrameData(		FrameData *frameData,
													void *inData,
													char *bifFilename,
													char *inputChannelName,
													char *primVarNames,
													char *smoothingChannelName,
													char *infCubeBlendChannelName,
													BifrostRenderType renderType,
													DiagnosticsParams diagnostics,
													void getRenderDataCB( void *, void *, PluginType ) );

// splats particles into voxels
Bifrost::API::Component BIFROSTRENDERAPI_DECL splatParticlesToVoxels(	Bifrost::API::Object inObject,
																		Bifrost::API::Component inComponent,
																		VolumeInputData *inData,
																		Bifrost::API::StateServer& srcSS,
																		CvFloat calcNoiseCB ( CvPoint3 ) );

// decide whether we have a point or voxel cache
bool BIFROSTRENDERAPI_DECL pointCacheStatusFromComponent( Bifrost::API::Component );
bool BIFROSTRENDERAPI_DECL pointCacheStatusFromFileInfo ( Bifrost::API::BIF::FileInfo fInfo );

// get stuff from MetaData
//void BIFROSTRENDERAPI_DECL getAdaptive ( Bifrost::API::Object inObj, void *inputData, RenderType renderType );
void BIFROSTRENDERAPI_DECL getMetaDataFlags ( Bifrost::API::FileIO fio, FrameData *frameData );

// set falloff axis vector for infcube blending
void BIFROSTRENDERAPI_DECL setFalloffAxis( FrameData *frameData );

// init arrays for optimization
void BIFROSTRENDERAPI_DECL allocOptimizationArrays( FrameData *frameData, DiagnosticsParams diagnostics );
void BIFROSTRENDERAPI_DECL initOptimizationArrays ( FrameData *frameData, DiagnosticsParams diagnostics );

// create a speed channel out of velocity channels
void BIFROSTRENDERAPI_DECL createSpeedChannel( Bifrost::API::StateServer srcSS, Bifrost::API::VoxelChannel inputChannel, Bifrost::API::Component component, DiagnosticsParams diagnostics );
void BIFROSTRENDERAPI_DECL createPointSpeedChannel( Bifrost::API::StateServer& inSS );

// create u1 and v1 channels from uv channel
void BIFROSTRENDERAPI_DECL createUVChannels( Bifrost::API::StateServer srcSS, Bifrost::API::VoxelChannel inputChannel, Bifrost::API::Component component, DiagnosticsParams diagnostics );

// report flat range for channels
void BIFROSTRENDERAPI_DECL reportChannelRanges ( FrameData *frameData, Bifrost::API::Component component, DiagnosticsParams Diagnostics );

// convert particles to voxels for liquids
Bifrost::API::Component BIFROSTRENDERAPI_DECL convertParticlesToVoxels( Bifrost::API::Component component,
																		ImplicitsInputData *inData,
																		FrameData *frameData,
																		Bifrost::API::StateServer& srcSS,
																		Bifrost::API::StringArray& loadChannelNames );

// do various optimizations
void BIFROSTRENDERAPI_DECL optimizeInputForRendering ( FrameData *frameData, Bifrost::API::Channel inChannel, DiagnosticsParams diagnostics );

// copy distance channel to another
void BIFROSTRENDERAPI_DECL copyDistanceChannel( FrameData *frameData, Bifrost::API::StateServer srcSS, Bifrost::API::Component component );

}}
