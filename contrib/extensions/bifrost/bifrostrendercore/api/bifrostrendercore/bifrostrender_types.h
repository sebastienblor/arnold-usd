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

#include <vector>
#include <stdint.h>
#include <cfloat>

#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_stateserver.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_basictypes.h"
#include "bifrostrender_objectuserdata.h"

namespace Bifrost{
namespace RenderCore{

//
//
// ENUMS
//
//

enum ExportTokens {
	TOK_POSITION = 0,
	TOK_RADIUS,
	TOK_TRANSPARENCY,
	TOK_NORMAL,
	TOK_GRADIENT_NORMAL,
	TOK_PRIMVAR
};

enum TokenClass {
	TOKCLASS_VARYING = 0,
	TOKCLASS_UNIFORM,
	TOKCLASS_CONSTANT
};

// enum for smoothing filter types
enum SmoothFilterType {
	kMeanValue=0,
	kGaussian,
	kMedianValue,
	kLaplacianFlow,
    kCurvatureFlow
}; 

// enum for blending types
enum FalloffType {
	kLinear=0,
	kSmoothStep,
	kSmootherStep,
	kNone
};

// enum for input cache type
enum CacheType {
	PARTICLE = 0,
	VOXEL,
	FOAM,
	LIQUID,
	AERO
};

// enum for input cache type
enum BifrostRenderType {
	IMPLICITSURFACE = 0,
	VOLUME,
};

enum PrimitivesRenderType {
	PRIM_POINT = 0,
	PRIM_SPHERE,
	PRIM_BLOB,
	PRIM_CURVE
};

// enum for input cache type
enum PluginType {
	PLUGIN_IMPLICITS = 0,
	PLUGIN_VOLUME,
    PLUGIN_PRIMITIVES
};

// enum for range calc strategy
enum RangeStrategy {
	SIMPLEBOUND = 0,
	BOUNDCACHE,
	LEVELPRECHECK,
	LEVELFULLCHECK
};

// enum for range calc strategy
enum Diagnostics {
	SIMONLY = 0,
	ALL
};

// enum for range calc strategy
enum InfCubeOutputType {
	OUTPUT_SIMONLY = 0,
	OUTPUT_ALL
};

// enum for sdf to fogvolume calc type
enum SDFType {
	HALFATSURFACE = 0,
	ZEROATSURFACE,
	SHELL,
	SHELL01
};

// enum for range calc strategy
enum MesherAlgorithm {
	MESH_MARCHINGCUBES = 0,
	MESH_MARCHINGTETS,
	MESH_NAIVESURFACENETS,
	MESH_BIFROST
};

//
//
// STRUCTS
//
//

struct BIFROSTRENDERAPI_DECL BBox {
	float bound[6];
	amino::Math::vec3f min;
	amino::Math::vec3f max;
	amino::Math::vec3f center;
	amino::Math::vec3f dim;

	void calcFromBounds();
	void calcFromMinMax();
	void dump( Bifrost::API::String bBoxName );
	void dumpSimple();
};

struct BIFROSTRENDERAPI_DECL BBoxData {
	BBox world;
	BBox tile;
	float voxelScale;

	void calcTileFromWorld();
	void calcWorldFromTile();
};

struct primVarInfo {
    std::string name;
    std::string channelName;
    std::string tokenName;
	Bifrost::API::Channel channel;
	void *inDataArray;
	size_t size;
	CvFloat *primDataArray;
	float min;
	float max;
	float defVal;
    bool exportToRIB;

	int exportArraysIndex;
	int samplerArrayIndex;
};

struct BIFROSTRENDERAPI_DECL BifRenderInfo {
	RangeStrategy rangeStrategy;
	double levelVolume[9];
	int levelLookupCount[8];
	int levelDepthWidth[8];
	int fastLookups[8];
	unsigned long long rangeTotalCount;
	unsigned long long rangeFrustumBailOut;
	unsigned long long rangeCacheReuse;
	unsigned long long rangeCount;
	unsigned long long rangeTileCountBailOut;
	unsigned long long rangeInfCubeBailOut;
	unsigned long long boxCacheReuse;
	unsigned long long boxCount;
	unsigned long long intersectWithSimCount;
	unsigned long long intersectWithInfCubeCount;
	unsigned long long insideInfCubeCount;
	unsigned long long calcInfCubeFieldCallCount;
	unsigned long long calcInfCubeFieldOutsideCount;
	unsigned long long calcInfCubeFieldInsideCount;
	unsigned long long calcInfCubeFieldIntersectCount;
	unsigned long long calcInfCubeGradientCallCount;

	float* levelMin[8];
	float* levelMax[8];
	float* velMinX[8];
	float* velMaxX[8];
	float* velMinY[8];
	float* velMaxY[8];
	float* velMinZ[8];
	float* velMaxZ[8];

	int maxDepth;
	int tilesWithDataCount[8];
	int tilesWithChildTiles[8];

	void calcTileVolumes(Bifrost::API::Layout& layout);
	void dump();
};

struct FrustumPlane {
	amino::Math::vec3f normal;
	float normalMag;
	float planeD;
};

struct BIFROSTRENDERAPI_DECL ThreadNames {
    std::string threadNameSrc;
    std::string threadNameVelU;
    std::string threadNameVelV;
    std::string threadNameVelW;

    std::string rangeMinBoundCacheName;
    std::string rangeMaxBoundCacheName;
    std::string rangeMinResultCacheName;
    std::string rangeMaxResultCacheName;
    std::string rangeCacheActiveName;

    std::string boxMinBoundCacheName;
    std::string boxMaxBoundCacheName;
    std::string boxMinXCacheName;
    std::string boxMaxXCacheName;
    std::string boxMinYCacheName;
    std::string boxMaxYCacheName;
    std::string boxMinZCacheName;
    std::string boxMaxZCacheName;
    std::string boxCacheActiveName;
};

// struct to hold frame data
struct BIFROSTRENDERAPI_DECL FrameData {
    Bifrost::API::Layout layout;
    Bifrost::API::VoxelChannel orgInputChannel;
	Bifrost::API::VoxelChannel srcChannel;
	Bifrost::API::VoxelChannel safeChannel;
	Bifrost::API::VoxelChannel airDistanceChannel;
	Bifrost::API::VoxelChannel smoothChannel;
	Bifrost::API::VoxelChannel infCubeBlendChannel;
	Bifrost::API::VoxelChannel velocityChannelU;
	Bifrost::API::VoxelChannel velocityChannelV;
	Bifrost::API::VoxelChannel velocityChannelW;
	Bifrost::API::VoxelChannel displacementChannel;

	Bifrost::API::StateServer inSS;

    Bifrost::API::Object inObj;

	std::vector<Bifrost::API::VoxelChannel> reportChannels;
	std::vector<primVarInfo> primVars;
	std::vector<float> channelMin;
	std::vector<float> channelMax;

	float dropletThreshold;
	bool isAdaptive;
	amino::Math::vec3f gravityDirection;
	int presenceFalloffAxises[2];
	int metaDataLoaded;

	bool motionBlur;
	int sampleRate;
	float shutter[2];
	float shutterSize;
	float screenWindow[4];
	float velocityScale;
	bool velocityExists;
    std::string idString;

	bool error;
	bool isPointCache;
	bool idExists;
	bool smoothChannelExists;
	bool infCubeBlendChannelExists;
	bool speedNeeded;
	bool uvNeeded;
	bool presenceNeeded;
	Bifrost::API::StringArray loadChannelNames;
	Bifrost::API::StringArray primVarNames;
	Bifrost::API::Int32Array primVarTypes;
	size_t maxElementCount;

	bool exportNormals;

	// size of finest tile in world space
	float tileWorldSize;
	// size of finest tile in world space
	float voxelScale;
	float orgVoxelScale;
	float cullDepthAtStart;

    BBoxData bboxSim;
    BBoxData bboxInf;

	CvPoint hullCorners[8];
	FrustumPlane planes[6];
	CvInt hullCount;

	BifRenderInfo bifInfo;

	Diagnostics diagnostics;

    PluginType pluginType;

	// for internal bookkeeping
	bool renderInfCube;
	float offsetSize;

	bool doNotRender;

	std::vector<void *> mem;

	void dump();
	void init();
};

struct BIFROSTRENDERAPI_DECL SmoothFilterParams {
    SmoothFilterType mode;
	int amount;
	int iterations;
	float weight;
	CvString channelName;
	float remapMin;
	float remapMax;
	bool remapInvert; // 7 params, 1 string
};

struct BIFROSTRENDERAPI_DECL ClipParams {
    bool on;
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ; // 7 params
};

struct BIFROSTRENDERAPI_DECL InfCubeParams {
	bool on;
	InfCubeOutputType outputType;
	float simWaterLevel;
	float topCenterX;
	float topCenterY;
	float topCenterZ;
    float dimX;
    float dimY; // ??
	float dimZ;
	FalloffType blendType;
	float blendStart;
	float blendEnd;
	CvString channelName;
	float remapMin;
	float remapMax;
	bool remapInvert; // 14 params, 1 string
};

struct BIFROSTRENDERAPI_DECL LevelSetParams {
	float resolutionFactor;
	float dropletRevealFactor;
	float surfaceRadius;			// Careful with this: Too small and you see a lot of speckles on the surface
	float dropletRadius;
	float maxVolumeOfHolesToClose;
	bool doMorphologicalDilation;  
	bool doErodeSheetsAndDroplets; // 7 params
};

struct BIFROSTRENDERAPI_DECL DiagnosticsParams {
	int silent;
	unsigned DEBUG;
};

struct BIFROSTRENDERAPI_DECL CullSidesParams {
	bool on;
	float start;
	float end;
	float range; // calculated
	float depthAtStartInVoxels; // 4 params
};

#define FREESTR(str) if(str) { free(str); str = NULL; }

// Input data for BifrostImplicits
struct BIFROSTRENDERAPI_DECL ImplicitsInputData {
    ImplicitsInputData () :
        bifFilename(NULL),
        inputChannelName(NULL),
        primVarNames(NULL),
        bifrostObjectName(NULL),
        inMemoryRef(NULL) {}

    ~ImplicitsInputData() {
        FREESTR(bifFilename);
        FREESTR(inputChannelName);
        FREESTR(primVarNames);
        FREESTR(bifrostObjectName);
    }

	CvString bifFilename;
	CvString inputChannelName;
	CvString primVarNames;
	CvString bifrostObjectName;

	// parameters for voxel evaluation
	MesherAlgorithm mesherAlgo;
	int sampleRate;
	float narrowBandThicknessInVoxels;
	float velocityScale;
	float fps;
	float spaceScale;  // 4 params

	CullSidesParams cullSides; // 8 params

	float dilateAmount;
	float erodeAmount;
	SmoothFilterParams smooth; // 18 params

	ClipParams clip; // 25 params

	InfCubeParams infCube; // 39 params

	// parameters for particle to voxel conversion
	float resolutionFactor;
	float dropletRevealFactor;
	float surfaceRadius;			// Careful with this: Too small and you see a lot of speckles on the surface
	float dropletRadius;
	float maxVolumeOfHolesToClose;
	bool doMorphologicalDilation;  
	bool doErodeSheetsAndDroplets; // 46 params

	// diagnostics parameters
	DiagnosticsParams diagnostics; // 47 params - silent is not get with the current system
    //bool hotData; // 47 params

	// for arnold
	float stepSize;
	bool motionBlur;
	float shutterStart;
	float shutterEnd;

	// for internal bookkeeping
	float surfaceThickness;
	bool renderInfCube;
	bool error;

	// in memory data structure
	CoreObjectUserData *inMemoryRef;

	static const int nofProcFloatParams = 46;
	static const int nofProcStringParams = 7;

	static const int nofImplicitFloatParams = 48;
	static const int nofImplicitStringParams = 6;

	static const int nofCubeFloatParams = 17;
	static const int nofCubeStringParams = 1;

	void printParameters( bool isParticleCache );
    void checkParameters();
};

struct BIFROSTRENDERAPI_DECL PrimitivesInputData {
	PrimitivesInputData () {
		this->inMemoryRef = NULL;
	}

	PrimitivesRenderType renderType;
	CvString inputChannelName;
	float channelScale;
	bool exportNormalAsPrimvar;

	float velocityScale;
	float fps;
	float spaceScale;
	int skip;
	int chunkSize;

	ClipParams clip;

	float radius;
	bool useChannelToModulateRadius;
	bool camRadius;
	float camRadiusStartDistance;
	float camRadiusEndDistance;
	float camRadiusDistanceRange;
	float camRadiusStartFactor;
	float camRadiusEndFactor;
	float camRadiusFactorRange;
	float camRadiusFactorExponent;

	int mpSamples;
	float mpMinRadius;
	float mpMaxRadius;
	float mpSurfaceAttract;
	FalloffType mpFalloffType;
	float mpFalloffStart;
	float mpFalloffEnd;
	float mpFalloffRange;
	float mpDisplacementValue;
	float mpDisplacementNoiseFrequency;

	CvString primVarNames;
	CvString bifFilename;
	CvString bifrostObjectName;

	DiagnosticsParams diagnostics;
	bool hotData;

	// in memory data structure
	CoreObjectUserData *inMemoryRef;

	// for arnold
	bool motionBlur;
	float shutterStart;
	float shutterEnd;

	bool error;

	void printParameters();
	void checkParameters();
};

// input data for BifrostVolume
struct BIFROSTRENDERAPI_DECL VolumeInputData {
	VolumeInputData () {
		this->inMemoryRef = NULL;
	}

	CvString bifFilename;
	CvString inputChannelName;
	CvString primVarNames;
	CvString bifrostObjectName;

	// parameters for voxel evaluation
	float channelScale;
	float velocityScale;
	float fps;
	float spaceScale;  // 4 params

	SmoothFilterParams smooth; // 12 params

	ClipParams clip; // 19 params

	// parameters for particle splating
	float splatResolutionFactor;
	int skip;
	int splatSamples;
	float splatMinRadius;
	float splatMaxRadius;
	float splatSurfaceAttract;
	FalloffType splatFalloffType;
	float splatFalloffStart;
	float splatFalloffEnd;
	float splatDisplacement;
	float splatNoiseFreq; // 30 params

	// state parameters
	DiagnosticsParams diagnostics; // 31 params
    //bool hotData; // 32 params

	// in memory data structure
	CoreObjectUserData *inMemoryRef;

	// for arnold
	float stepSize;
	float raySamples;

	bool motionBlur;
	float shutterStart;
	float shutterEnd;

	// for internal bookkeeping
	float splatFalloffRange;				// not counted
	bool error;

	static const int nofImplicitFloatParams = 32;
	static const int nofImplicitStringParams = 5;

	void printParameters( bool isParticleCache );
	void checkParameters();
};

struct BIFROSTRENDERAPI_DECL PrimitivesFrameData {
	std::vector<primVarInfo> primVars;
	Bifrost::API::StringArray primVarNames;
	Bifrost::API::StringArray inputChannelNames;
	Bifrost::API::StringArray loadChannelNames;
	std::vector<void *> mem;

	CacheType cacheType;
	bool speedNeeded;
	bool transparencyNeeded;
	bool idNeeded;
	bool motionBlur;
	bool velocityExists;
	float shutter[2];

	int velocityIndex;
	int velocityIndexU;
	int velocityIndexV;
	int velocityIndexW;

	unsigned long id;
    std::string idString;

	CvInt nofElements;
	CvInt nofBaseElements;
	bool isMultiPointing;
	bool isPointCache;
	float voxelScale;
	float minDistance;
	float maxDistance;
	int finalChunkSize;

	bool error;
	bool hotData;
	Bifrost::API::String tmpFolder;

	PluginType pluginType;

	void init();
};

}}
