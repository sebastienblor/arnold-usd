//*****************************************************************************
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

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
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_dictionary.h>
#include <bifrostapi/bifrost_haloedcache.h>
#include <bifrostapi/bifrost_levelset.h>
#include <bifrostapi/bifrost_voxelcomponent.h>
#include <bifrostapi/bifrost_pointcomponent.h>

#include <ai.h>

#include <bifrostrendercore/bifrostrender_defs.h>
#include <bifrostrendercore/bifrostrender_types.h>
#include <bifrostrendercore/bifrostrender_math.h>
#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_primvars.h>
#include <bifrostrendercore/bifrostrender_visitors.h>
#include <Tools.h>

#include <bifrostrendercore/bifrostrender_filters.h>

#include <bifrostrendercore/bifrostrender_objectuserdata.h>

using namespace Bifrost::RenderCore;

namespace
{
void cleanVoxelSamplers(Bifrost::API::VoxelSampler **samplers)
{
	for (int i = 0; i < AI_MAX_THREADS; ++i) {
		if ( samplers[i] ) {
			delete samplers[i];
			samplers[i] = 0;
		}
	}
}

struct BifrostImplicitsUserData {
	CoreObjectUserData *objectRef;

	Bifrost::API::VoxelSampler **channelSamplers;
	std::map<AtString, int> channelSamplerIndexes;
	int srcChannelSamplerIndexStart;

	Bifrost::API::Component voxelComponent;

	ImplicitsInputData *inputData;
	FrameData *frameData;

	std::string objectName;
	std::string file;
	AtBBox bbox;
	int maxSteps;

	bool shadowing;
	float shadowingStepSize;
	int shadowingMaxSteps;
};

bool getNodeParameters( ImplicitsInputData *inData, const AtNode *parentNode )
{
	bool error = false;

	// get numeric data
	inData->narrowBandThicknessInVoxels = AiNodeGetFlt(parentNode, "narrowBandThicknessInVoxels");
	inData->stepSize = AiNodeGetFlt(parentNode, "liquidStepSize");

	inData->cullSides.on = AiNodeGetBool( parentNode, "cullSidesOn" );
	inData->cullSides.start = AiNodeGetFlt(parentNode, "cullSidesStart");
	inData->cullSides.end = AiNodeGetFlt(parentNode, "cullSidesEnd");
	inData->cullSides.depthAtStartInVoxels = AiNodeGetFlt(parentNode, "cullDepthAtStartInVoxels");

	inData->velocityScale = AiNodeGetFlt(parentNode, "velocityScale");
	inData->fps = AiNodeGetFlt(parentNode, "fps");
	inData->spaceScale = AiNodeGetFlt(parentNode, "spaceScale");

	inData->dilateAmount = AiNodeGetFlt(parentNode, "dilateAmount");
	inData->erodeAmount = AiNodeGetFlt(parentNode, "erodeAmount");

	inData->smooth.on = AiNodeGetBool( parentNode, "smoothOn" );
	inData->smooth.mode = (SmoothFilterType) AiNodeGetInt( parentNode, "smoothMode" );
	inData->smooth.amount = AiNodeGetInt( parentNode, "smoothAmount" );
	inData->smooth.iterations = AiNodeGetInt( parentNode, "smoothIterations" );
	inData->smooth.remapMin = AiNodeGetFlt(parentNode, "smoothRemapMin");
	inData->smooth.remapMax = AiNodeGetFlt(parentNode, "smoothRemapMax");
	inData->smooth.remapInvert = AiNodeGetBool(parentNode, "smoothRemapInvert");

	inData->clip.on = AiNodeGetBool( parentNode, "clipOn" );
	inData->clip.minX = AiNodeGetFlt(parentNode, "clipMinX");
	inData->clip.maxX = AiNodeGetFlt(parentNode, "clipMaxX");
	inData->clip.minY = AiNodeGetFlt(parentNode, "clipMinY");
	inData->clip.maxY = AiNodeGetFlt(parentNode, "clipMaxY");
	inData->clip.minZ = AiNodeGetFlt(parentNode, "clipMinZ");
	inData->clip.maxZ = AiNodeGetFlt(parentNode, "clipMaxZ");

	inData->infCube.on = AiNodeGetBool( parentNode, "infCubeBlendingOn" );
	inData->infCube.outputType = (InfCubeOutputType) AiNodeGetInt( parentNode, "infCubeOutputType" );
	inData->infCube.simWaterLevel = AiNodeGetFlt(parentNode, "simWaterLevel");
	inData->infCube.topCenterX = AiNodeGetFlt(parentNode, "infCubeTopCenterX");
	inData->infCube.topCenterY = AiNodeGetFlt(parentNode, "infCubeTopCenterY");
	inData->infCube.topCenterZ = AiNodeGetFlt(parentNode, "infCubeTopCenterZ");
	inData->infCube.dimX = AiNodeGetFlt(parentNode, "infCubeDimX");
	inData->infCube.dimZ = AiNodeGetFlt(parentNode, "infCubeDimZ");
	inData->infCube.blendType = (FalloffType) AiNodeGetInt( parentNode, "blendType" );
	inData->infCube.blendStart = AiNodeGetFlt(parentNode, "infCubeBlendStart");
	inData->infCube.blendEnd = AiNodeGetFlt(parentNode, "infCubeBlendEnd");
	inData->infCube.remapMin = AiNodeGetFlt(parentNode, "blendingChannelRemapMin");
	inData->infCube.remapMax = AiNodeGetFlt(parentNode, "blendingChannelRemapMax");
	inData->infCube.remapInvert = AiNodeGetBool( parentNode, "blendingChannelRemapInvert" );

	inData->resolutionFactor = AiNodeGetFlt(parentNode, "implicitResolutionFactor");
	inData->dropletRevealFactor = AiNodeGetFlt(parentNode, "implicitDropletRevealFactor");
	inData->surfaceRadius = AiNodeGetFlt(parentNode, "implicitSurfaceRadius");
	inData->dropletRadius = AiNodeGetFlt(parentNode, "implicitDropletRadius");
	inData->maxVolumeOfHolesToClose = AiNodeGetFlt(parentNode, "implicitMaxVolumeOfHolesToClose");
	inData->doMorphologicalDilation = AiNodeGetBool( parentNode, "doMorphologicalDilation" );
	inData->doErodeSheetsAndDroplets = AiNodeGetBool( parentNode, "doErodeSheetsAndDroplets" );

	inData->diagnostics.DEBUG = AiNodeGetInt( parentNode, "debug" );

	inData->hotData = AiNodeGetBool( parentNode, "hotData" );

	// get string data
	const AtString bifFilenameParam("bifFilename");
	const AtString bifFilename = AiNodeGetStr(parentNode, bifFilenameParam );
	size_t inputLen = bifFilename.length();
	inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifFilename, bifFilename.c_str() );

	const AtString distanceChannelNameParam("distanceChannel");
	const AtString distanceChannelName = AiNodeGetStr(parentNode, distanceChannelNameParam );
	inputLen = distanceChannelName.length();
	inData->inputChannelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->inputChannelName, distanceChannelName.c_str() );

	const AtString filterBlendingChannelNameParam("filterBlendingChannel");
	const AtString filterBlendingChannelName = AiNodeGetStr(parentNode, filterBlendingChannelNameParam );
	inputLen = filterBlendingChannelName.length();
	inData->smooth.channelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->smooth.channelName, filterBlendingChannelName.c_str() );

	const AtString infiniteSurfaceBlendingChannelNameParam("infiniteSurfaceBlendingChannel");
	const AtString infiniteSurfaceBlendingChannelName = AiNodeGetStr(parentNode, infiniteSurfaceBlendingChannelNameParam );
	inputLen = infiniteSurfaceBlendingChannelName.length();
	inData->infCube.channelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->infCube.channelName, infiniteSurfaceBlendingChannelName.c_str() );

	const AtString primVarNamesParam("primVarNames");
	const AtString primVarNames = AiNodeGetStr(parentNode, primVarNamesParam );
	inputLen = primVarNames.length();
	inData->primVarNames = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->primVarNames, primVarNames.c_str() );

	const AtString objectNameParam("bifrostObjectName");
	const AtString objectName = AiNodeGetStr(parentNode, objectNameParam );
	inputLen = objectName.length();
	inData->bifrostObjectName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifrostObjectName, objectName.c_str() );

	// arnold specific parameters
	inData->motionBlur = AiNodeGetBool( parentNode, "motionBlur" );
	inData->shutterStart = AiNodeGetFlt( parentNode, "shutterStart" );
	inData->shutterEnd = AiNodeGetFlt( parentNode, "shutterEnd" );

	// check parameters
	inData->checkParameters( PLUGIN_IMPLICITS );

	return error;
}


bool BifrostImplicitsInit(void** user_ptr)
{
	BifrostImplicitsUserData *data = new BifrostImplicitsUserData;
	data->objectRef = 0;
	data->srcChannelSamplerIndexStart = -1;
   
	*user_ptr = data;

	return true;
}

bool BifrostImplicitsCleanup(void* user_ptr)
{
	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*)user_ptr;
  
	if ( userData ) {
		FrameData *frameData = userData->frameData;
		ImplicitsInputData *inData = userData->inputData;

		if ( frameData ) {
			if ( inData->hotData ) {
				Bifrost::API::File::deleteFolder( frameData->tmpFolder );
			}
		}

		if ( inData ) {
			free( inData->bifFilename );
			free( inData->inputChannelName );
			free( inData->smooth.channelName );
			free( inData->infCube.channelName );
			free( inData->primVarNames );
			free( inData->bifrostObjectName );
		}

		if ( userData->objectRef ) {
			delete userData->objectRef;
		}

		delete userData;
	}

	return true;
}

bool BifrostImplicitsCreate(	void* user_ptr,
								const char* user_config,
								const AtNode* node,
								AtVolumeData* out_data )
{
	//
	//
	// DECLARATIONS
	//
	//
	BifrostImplicitsUserData *data = (BifrostImplicitsUserData *) user_ptr;

	//
	//
	// INIT
	//
	//

	// init ImplicitsInputData
	struct ImplicitsInputData *inData = (struct ImplicitsInputData *) new ( struct ImplicitsInputData);
	data->inputData = inData;

    // log start
	printEndOutput( "[BIFROST IMPLICITS] START OUTPUT", inData->diagnostics );

	// get input data
	bool error = getNodeParameters( inData, node );

	// init in memory class
	inData->inMemoryRef = new CoreObjectUserData( inData->bifrostObjectName, inData->bifFilename );

	// init user data stuff
	data->objectRef = inData->inMemoryRef;
	data->objectName = inData->bifrostObjectName;
	data->file = inData->bifFilename;

	out_data->private_info = user_ptr;

	//data->maxSteps = 1000;
	//data->shadowing = true;
	//data->shadowingStepSize = 0.5f;
	//data->shadowingMaxSteps = 100;

	// set step size
	out_data->auto_step_size = inData->stepSize;

	//
	//
	// CHECK INPUT FILE
	//
	//
	Bifrost::API::String writeToFolder;
	if ( inData->hotData ) {
		// write in memory data to a temp file
		Bifrost::API::String writeToFile;
		if ( strstr( inData->bifFilename, "volume" ) != NULL ) {
			writeToFile = writeHotDataToDisk( *(inData->inMemoryRef), inData->bifFilename, "voxel_liquid-volume", writeToFolder );
		} else {
			writeToFile = writeHotDataToDisk( *(inData->inMemoryRef), inData->bifFilename, "voxel_liquid-particle", writeToFolder );
		}

		// realloc for the new name
		size_t inputLen = writeToFile.length();
		free ( inData->bifFilename );
		inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
		strcpy( inData->bifFilename, writeToFile.c_str() );
	}

	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes(inData->bifFilename);

	// check for existence of files if we don't have hot data
	if ( !inData->hotData ) {
		if (FILE *file = fopen(correctedFilename.c_str(), "r")) {
			fclose(file);
		} else {
			error = true;
			IFNOTSILENT {
				printf("BifFile: %s does not exists!\n", correctedFilename.c_str() );
				printf("\tExiting...\n");
			}
		}

		// if there is any error exit
		if ( error ) {
			printEndOutput( "[BIFROST IMPLICITS] END OUTPUT", inData->diagnostics );
			return false;
		}
	}

	// if there is any error exit
	if ( error ) {
		printEndOutput( "[BIFROST IMPLICITS] END OUTPUT", inData->diagnostics );
		return NULL;
	}

	IFNOTSILENT {
		inData->printParameters( correctedFilename.find("particle") != Bifrost::API::String::npos );
	}

	//
	//
	// PRELOADING AND INFO GATHERING
	//
	//
	//

	// init FrameData struct that holds information specific to the frame we are rendering
	FrameData *frameData = (FrameData *) new (FrameData);
	frameData->init();
	frameData->pluginType = PLUGIN_IMPLICITS;
	if ( inData->cullSides.on ) {
		frameData->presenceNeeded = true;
	}
	frameData->hotData = inData->hotData;
	frameData->tmpFolder = writeToFolder;

	data->frameData = frameData;

	// process which channels to load
	initAndGetFrameData(	frameData,
							( void * ) inData,
							inData->bifFilename,
							inData->inputChannelName,
							inData->primVarNames,
							inData->smooth.channelName,
							inData->infCube.channelName,
							IMPLICITSURFACE,
							inData->diagnostics,
							getASSData );

	if ( frameData->error ) {
		printEndOutput( "[BIFROST IMPLICITS] END OUTPUT", inData->diagnostics );
		return NULL;
	}

	//
	//
	// SETUP CLIPPING
	//
	//
	//
	
	amino::Math::bboxf clipBox;
	if ( inData->clip.on ) {
		amino::Math::vec3f min ( inData->clip.minX, inData->clip.minY, inData->clip.minZ );
		amino::Math::vec3f max ( inData->clip.maxX, inData->clip.maxY, inData->clip.maxZ );

		clipBox = amino::Math::bboxf( min, max );
	}

	//
	//
	// FILE LOADING
	//
	//
	//
    Bifrost::API::ObjectModel om;
	Bifrost::API::FileIO fio = om.createFileIO( correctedFilename );

	Bifrost::API::Status loadSt;
	Bifrost::API::StateServer inSS = om.createStateServer();

	// timer for diagnostics
	std::time_t start, end;

	start = std::time(NULL);

	if ( inData->clip.on ) {
		loadSt = fio.load( inSS, frameData->loadChannelNames, clipBox);
	} else {
		loadSt = fio.load( inSS, frameData->loadChannelNames);
	}

	end = std::time(NULL);
	double duration = difftime( end, start );

    if( loadSt == Bifrost::API::Status::Failure ) {
		IFNOTSILENT { printf("Bif file can not be loaded, please check the file!\n"); }
		return NULL;
    }

	//
	//
	// FILE CLASS GATHERING
	//
	//
	//

	// now that we have a bif file loaded
	// let's declare some variables for keeping track of what information we need during rendering
	Bifrost::API::RefArray objects = inSS.objects();
	Bifrost::API::Object inObj = objects[0];
	frameData->inObj = inObj;
	Bifrost::API::RefArray components = inSS.components();
	Bifrost::API::Component component = components[0];
	Bifrost::API::Layout layout = component.layout();
	Bifrost::API::RefArray channels = component.channels();

	// we may need to get the metadata once more because of older version files
	if ( frameData->metaDataLoaded == 0 ) {
		getMetaDataFlags ( fio, frameData );
	}

	// print parameters
	//IFNOTSILENT { inData->printParameters( frameData->isPointCache ); }

	// print load time
	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 0 ) {
			printf("\nLoad time:%f secs\n", duration);
		}
	}

	// print file info to the console
	IFNOTSILENT {
		std::cerr << "\nFile Info:" << std::endl;
		std::cerr << "\tfilename: " << fio.filename() << std::endl;
		std::cerr << "\tframe: " << fio.info().frame << " version: " << fio.info().version << " compression: " << fio.info().compression << std::endl << std::endl;
		std::cout << "\tComponent " << components.count() << ": " << component.name() << std::endl;
		std::cout << "\tLayout: " << layout.name() << std::endl;
		std::cout << "\t\tVoxelScale: " << layout.voxelScale() << std::endl;
		std::cout << "\t\tMaxDepth: " << layout.maxDepth() << std::endl;
		std::cout << "\t\tTotal TileCount: " << layout.tileCount() << std::endl;
		if ( frameData->isPointCache ) {
			for ( int i = 0; i <= layout.maxDepth(); i++ ) {
				std::cout << "\t\t\tTileCount " << i << ": " << layout.tileCount(i) << "->" << Bifrost::API::PointChannel(channels[0]).elementCount(i) << std::endl;
			}
		} else {
			for ( int i = 0; i <= layout.maxDepth(); i++ ) {
				std::cout << "\t\t\tTileCount " << i << ": " << layout.tileCount(i) << std::endl;
			}
		}

		if ( inData->diagnostics.DEBUG > 1 ) {
			std::cout << "\tChannel count: " << channels.count() << std::endl;
			for ( size_t i=0; i<channels.count(); i++ ) {
				std::cout << "\t\tChannel: " << Bifrost::API::Base(channels[i]).name() << std::endl;
			}
		}
	}

	IFNOTSILENT {
		if ( frameData->dropletThreshold > 0 ) {
			printf("\nADAPTIVE SIM: ");
			frameData->isAdaptive ? printf("ON\n") : printf("OFF\n");
			printf("\tdropletThreshold: %f\n", frameData->dropletThreshold);
		}
	}

	// dump data before channel processing
	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 1 ) {
			dumpStateServer( inSS, "BEFORE PROCESSING" );
		}
	}

	//
	//
	// GET CACHE CHANNELS
	//
	//

	// get channels for common channels
	const Bifrost::API::Channel& posChannel = component.findChannel( Bifrost::API::String( "position" ) );
	const Bifrost::API::Channel& velChannel = component.findChannel( Bifrost::API::String( "velocity" ) );
	const Bifrost::API::Channel& velUChannel = component.findChannel( Bifrost::API::String( "velocity_u") );
	const Bifrost::API::Channel& velVChannel = component.findChannel( Bifrost::API::String( "velocity_v") );
	const Bifrost::API::Channel& velWChannel = component.findChannel( Bifrost::API::String( "velocity_w") );
	const Bifrost::API::Channel& idChannel = component.findChannel( Bifrost::API::String( "id64") );
	const Bifrost::API::Channel& dropletChannel = component.findChannel( Bifrost::API::String( "droplet") );
	const Bifrost::API::Channel& airDistanceChannel = component.findChannel( Bifrost::API::String( "airDistance") );

	//
	// decide on what type of rendering we are doing
	//
	CacheType cacheType = VOXEL;
	if ( component.type() == Bifrost::API::PointComponentType ) {
		cacheType = PARTICLE;
		IFNOTSILENT { printf("\n\nTHIS IS A PARTICLE CACHE!\n"); }
	} else {
		IFNOTSILENT { printf("\nTHIS IS A VOXEL CACHE!\n"); }
	}

	//
	//
	// OPTIMIZE CACHE FOR RENDERING
	//
	//
	Bifrost::API::Channel inChannel = component.findChannel( Bifrost::API::String( inData->inputChannelName ) );
	optimizeInputForRendering ( frameData, inChannel, inData->diagnostics );

	//
	//
	// CONVERT PARTICLES TO VOXELS IF NEEDED
	//
	//
	//
	if ( cacheType == PARTICLE ) {
		IFNOTSILENT { printf("\nConverting particles to voxels...\n"); }

		component = convertParticlesToVoxels(	component,
												inData,
												frameData,
												inSS,
												frameData->loadChannelNames );

		// update layout and channels as we now have another component
		layout = component.layout();
		channels = component.channels();

		IFNOTSILENT {
			if ( inData->diagnostics.DEBUG > 1 ) {
				dumpStateServer( inSS, "AFTER PARTICLE CONVERSION" );
			}
		}
	}

	// check whether we have a channel named as the input after conversion
	frameData->orgInputChannel = component.findChannel( Bifrost::API::String( inData->inputChannelName ) );

	if ( frameData->orgInputChannel.valid() == false ) {
		IFNOTSILENT ( printf( "\n\nNo valid channel found with the name: %s\nCan not continue!\n", inData->inputChannelName ) );
		return NULL;
	} else {
		IFNOTSILENT ( printf( "\nRendering Channel is: %s...\n", frameData->orgInputChannel.fullPathName().c_str() ) );
	}

	//
	//
	//
	// AT THIS POINT EVERYTHING IS VOXELIZED. PARTICLE CACHES ARE TRANSFORMED INTO A VOXEL REP BY CALCULATING A LEVEL SET OUT OF PARTICLES.
	// VOXEL CACHES ARE JUST PASSED THROUGH.
	// SO WE CAN ASSUME THAT THINGS ARE ALL IN VOXELS.
	//
	//
	//

	//
	//
	// CREATE EXTRA RUNTIME CHANNELS
	//
	//
	//
	if ( frameData->speedNeeded ) {
		IFNOTSILENT { printf("\nCreating primvar: \n\tspeed\n"); }
		// create a speed channel out of velocity channels
		createSpeedChannel( inSS, frameData->orgInputChannel, component, inData->diagnostics );
	}

	// do we need uv
	if ( frameData->uvNeeded ) {
		IFNOTSILENT {
			if ( frameData->speedNeeded ) {
				printf("\tu\n\tv\n");
			} else {
				printf("\nCreating primvar: \n\tu\n\tv\n");
			}
		}

		// separate uv channel to u1 and v1
		createUVChannels( inSS, frameData->orgInputChannel, component, inData->diagnostics );
	}

	//
	//
	// CREATE PRIMVARINFO STRUCTURE
	//
	//
	initPVarInfo( frameData );

	IFNOTSILENT { printf("\nProcessing PRIMVARS List...\n"); }

	checkPrimVars( frameData, inSS, component, inData->inputChannelName, inData->diagnostics );

	// print out primVars
	IFNOTSILENT {
		if (frameData->primVars.size() > 0 ) {
			printf("\tFinal list of Primvars to Export:\n");
			for (unsigned i = 0; i < frameData->primVars.size(); i++) {
				printf("\t\t%s\n", frameData->primVars[i].name.c_str() );
			}
		} else {
			printf ( "\tNo primvars found to export!\n" );
		}
	}

	//
	//
	// NOW THAT WE HAVE EVERYTHING, INIT REST OF FRAMEDATA STRUCTURE
	//
	//

	frameData->layout = layout;
	frameData->bifInfo.maxDepth = layout.maxDepth();
	frameData->voxelScale = layout.voxelScale();
	frameData->bboxSim.voxelScale = layout.voxelScale();
	frameData->bboxInf.voxelScale = layout.voxelScale();
	frameData->orgVoxelScale = layout.voxelScale();
	frameData->bifInfo.calcTileVolumes( layout );
	frameData->srcChannel = frameData->orgInputChannel;
	frameData->airDistanceChannel = component.findChannel( Bifrost::API::String( "airDistance" ) );

	// init level lookup counts and depthWidths
	for (int i = 0; i <= layout.maxDepth(); i++) {
		frameData->bifInfo.levelDepthWidth[i] = layout.tileDimInfo(i).depthWidth;
		frameData->bifInfo.levelLookupCount[i] = 0;
		frameData->bifInfo.fastLookups[i] = 0;
		frameData->bifInfo.tilesWithDataCount[i] = 0;
		frameData->bifInfo.tilesWithChildTiles[i] = 0;
	}

	IFNOTSILENT { printf( "\nSHADING RATE: %.3f\n", frameData->shadingRate ); }

	// check whether we need to export motion blur
	frameData->velocityScale = 0.0f;
	frameData->shutterSize = 0.0f;
	IFNOTSILENT { printf("\nMotionBlur: "); }

	if ( frameData->shutter[0] < frameData->shutter[1] ) {
		if ( frameData->velocityExists ) {
			frameData->motionBlur = true;
			frameData->velocityScale = inData->velocityScale;
			frameData->shutterSize = frameData->shutter[1] - frameData->shutter[0];

			IFNOTSILENT { printf("ON\n\tShutter: %.2f %.2f\n\n", (frameData->shutter)[0], (frameData->shutter)[1]); }

			frameData->velocityChannelU = component.findChannel( "velocity_u" );
			frameData->velocityChannelV = component.findChannel( "velocity_v" );
			frameData->velocityChannelW = component.findChannel( "velocity_w" );
			frameData->reportChannels.push_back( frameData->velocityChannelU );
			frameData->reportChannels.push_back( frameData->velocityChannelV );
			frameData->reportChannels.push_back( frameData->velocityChannelW );
		} else {
			IFNOTSILENT { printf("OFF because no valid velocity channel found!\n\n"); }
		}	
	} else {
		IFNOTSILENT { printf("OFF\n\n"); }
	}

	//
	//
	// POST PROCESSING CHANNEL DATA
	//
	//
	//
	if (	inData->dilateAmount != 0.0f ||
			inData->erodeAmount != 0.0f ||
			( inData->smooth.on && inData->smooth.amount > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) 
	) {
		IFNOTSILENT {
			printf("\nPost Processing %s channel...\n", inData->inputChannelName);
			printf("\tPost processing parameters:\n");

			if ( inData->dilateAmount != 0.0f ) {
				printf("\t\tDilate by: %.3f...\n", inData->dilateAmount);
			}
		}

		Bifrost::API::String filterType ("None");
		switch (inData->smooth.mode ) {
			case kMeanValue:
				filterType = "MeanValue";
				break;
			case kGaussian:
				filterType = "Gaussian";
				break;
			case kMedianValue:
				filterType = "Median";
				break;
			case kCurvatureFlow:
				filterType = "CurvatureFlow";
				break;
			case kLaplacianFlow:
				filterType = "LaplacianFlow";
				break;
			//case kMentalRay:
			//	filterType = "MentalRay";
			//	break;
			default:
				break;
		}

		IFNOTSILENT {
			if ( inData->smooth.on && inData->smooth.amount > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) {
				printf("\t\tSmoothing FilterType: %s KernelSize: %d Iterations: %d Weight: %f FilterChannel: %s\n", filterType.c_str(),
																													inData->smooth.amount,
																													inData->smooth.iterations,
																													inData->smooth.weight,
																													inData->smooth.channelName );
			}

			if ( inData->erodeAmount != 0.0f ) {
				printf("\t\tErode by: %.3f...\n", inData->erodeAmount);
			}
		}

		// first dilate the channel
		if (inData->dilateAmount != 0.0 ) {
			IFNOTSILENT { printf("\tDilating...\n"); }
			dilateChannel( frameData, inData->dilateAmount, IMPLICITSURFACE );
		}

		// Run the smoothing filter
		if ( inData->smooth.on && inData->smooth.amount > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) {
			IFNOTSILENT { printf("\tSmoothing...\n"); }

			// Run the smoothing filter
			smoothChannel(	inData->smooth,
							inSS,
							frameData->orgInputChannel,
							frameData,
							inData->diagnostics,
							IMPLICITSURFACE );

			IFNOTSILENT { printf("\n"); }
		} // end smooth check

		// finally erode the channel
		if ( inData->erodeAmount != 0.0f ) {
			IFNOTSILENT { printf("\tEroding...\n"); }
			erodeChannel( frameData, inData->erodeAmount, IMPLICITSURFACE );
		}

		IFNOTSILENT { printf("\n"); }
	} // end post processing check

	//
	//
	// INFCUBE BLENDING
	//
	//
	if ( inData->infCube.on ) {
		IFNOTSILENT { printf("Applying inf cube blending...\n"); }

		// get channel
		frameData->infCubeBlendChannel = component.findChannel( Bifrost::API::String ( inData->infCube.channelName ) );

		displaceChannel( inData->infCube, inSS, frameData, inData->diagnostics );
		
		//Bifrost::API::Layout dispLayout = frameData->displacementChannel.layout();
		//for ( int i = 0; i <= dispLayout.maxDepth(); i++ ) {
		//	std::cout << "\t\t\tTileCount " << i << ": " << dispLayout.tileCount(i) << std::endl;
		//}

		// if output type is ALL extend the sim
		if ( inData->infCube.outputType == OUTPUT_ALL ) {
			extendChannel( inData->infCube, inSS, frameData, inData->diagnostics );
		}

		IFNOTSILENT { printf("\tDone...\n\n"); }
	}

	// if we have an adaptive sim
	// make a copy of distance channel as we need the original values for presence
	// do it here since we need the data processed with the filtering operators
	if ( frameData->presenceNeeded ) {
		copyDistanceChannel( frameData, inSS, component );
	}

	//
	//
	// CONVERT SURFACE TO NARROWBAND FOG IF WE ARE RENDERING A SURFACE
	//
	//

	// This is not needed in Arnold - FOR NOW
	//IFNOTSILENT {
	//	printf("Converting sdf to FogVolume density field...\n");
	//	printf("\tFogVolume narrow band thickness is: %.3f voxels...\n\n", inData->narrowBandThicknessInVoxels);
	//}

	//channelToNarrowBandFog ( frameData->srcChannel, inData->narrowBandThicknessInVoxels, frameData, true, inData->diagnostics );

	// turn off infcubeblending as its effect is baked into the data
	inData->infCube.on = false;

	//
	//
	// CALC BBOX
	//
	//
	double bboxMin[3] = { 0.0, 0.0, 0.0 };
	double bboxMax[3] = { 0.0, 0.0, 0.0 };
	computeIsosurfaceBounds( frameData->srcChannel, bboxMin, bboxMax);
	data->bbox.min.x = (float) bboxMin[0];
	data->bbox.min.y = (float) bboxMin[1];
	data->bbox.min.z = (float) bboxMin[2];
	data->bbox.max.x = (float) bboxMax[0];
	data->bbox.max.y = (float) bboxMax[1];
	data->bbox.max.z = (float) bboxMax[2];
   
	out_data->bbox = data->bbox;

	//
	//
	// REPORTING AND OPTIMIZATION
	//
	//

	// initialise arrays for various optimization calcs
	initOptimizationArrays ( frameData, inData->diagnostics );

	// report channel ranges in the log
	// add existing input channels to the final list
	if ( frameData->smoothChannelExists ) {
		frameData->smoothChannel = component.findChannel( inData->smooth.channelName );
		frameData->reportChannels.push_back( frameData->smoothChannel );
	}

	if ( frameData->infCubeBlendChannelExists ) {
		frameData->infCubeBlendChannel = component.findChannel( inData->infCube.channelName );
		frameData->reportChannels.push_back( frameData->infCubeBlendChannel );
	}

	reportChannelRanges ( frameData, component, inData->diagnostics );

	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 1 ) {
			dumpStateServer( inSS, "AFTER ALL OPS" );
		}
	}

	//
	//
	// ARNOLD SPECIFIC
	//
	//

	// now allocate space for samplers
	data->voxelComponent = component;
	int samplerChannelCount = 0;

	for ( int i = 0; i < channels.count(); i++ ) {
		Bifrost::API::Channel channel = (Bifrost::API::Channel) channels[i];
		AtString tmpString ( channel.name().c_str() );
		int startIndex = samplerChannelCount * AI_MAX_THREADS;
		data->channelSamplerIndexes[ tmpString ] = startIndex;

		if ( tmpString == Bifrost::API::String( inData->inputChannelName ) ) {
			data->srcChannelSamplerIndexStart = startIndex;
		}

		if ( inData->diagnostics.DEBUG > 0 ) {
			printf("%s - %d - %d\n", channel.name().c_str(), data->channelSamplerIndexes[ tmpString ], data->srcChannelSamplerIndexStart );
		}

		samplerChannelCount++;		
	}

	data->channelSamplers = ( Bifrost::API::VoxelSampler ** ) malloc( samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );
	memset( data->channelSamplers, 0, samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );

	//
	//
	// FINISH
	//
	//
	printEndOutput( "[BIFROST IMPLICITS] END OUTPUT", inData->diagnostics );

	return true;
}

bool BifrostImplicitsCleanupVolume(void* user_ptr, AtVolumeData* data, const AtNode* node)
{
	if (!user_ptr) {
		return false;
	}

	// AiAddMemUsage(-((AtInt64)grid->memoryUsage()), "BifrostAero volume plugin data");

	data->private_info = NULL;
	return true;
}

bool BifrostImplicitsSample(void* user_ptr,
                            const AtVolumeData* data,
                            const AtString channel,
                            const AtShaderGlobals* sg,
                            int interp,
                            AtParamValue *value,
                            AtByte *type)
{
	if (!data->private_info) return false;

	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*) data->private_info;
	FrameData *frameData = userData->frameData;
	ImplicitsInputData *inData = userData->inputData;

	amino::Math::vec3f pos;
	pos[0] = sg->P.x;
	pos[1] = sg->P.y;
	pos[2] = sg->P.z;

	int samplerIndexStart = userData->channelSamplerIndexes[ channel ];
	Bifrost::API::VoxelSampler *threadSampler = userData->channelSamplers[ samplerIndexStart + sg->tid ];

	if (threadSampler == 0) {
		if ( inData->diagnostics.DEBUG > 0 ) {
			printf( "Creating a new sampler for channel %s and thread %d...\n", channel.c_str(), sg->tid );
		}
		Bifrost::API::VoxelChannel bifChannel = userData->voxelComponent.findChannel( channel.c_str() );

		threadSampler = new Bifrost::API::VoxelSampler( bifChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace ) );
		userData->channelSamplers[ samplerIndexStart + sg->tid ] = threadSampler;
	}

	*type = AI_TYPE_FLOAT;
	if ( samplerIndexStart >= userData->srcChannelSamplerIndexStart && samplerIndexStart < userData->srcChannelSamplerIndexStart + AI_MAX_THREADS ) {
		// this is sampling the input channel so we need to apply the scalar
		//value->FLT = inData->channelScale * threadSampler->sample<float>(pos);
		value->FLT = threadSampler->sample<float>(pos);
	} else {
		value->FLT = threadSampler->sample<float>(pos);
	}

	//if ( value->FLT < 0.1f ) {
	//	printf("%f\n", value->FLT);
	//}

	return true;

	//if (!strcmp(channel, "velocity")) {
	//	Bifrost::API::VoxelSampler *threadSampler = userData->primVarSamplers[sg->tid];
	//	if (threadSampler == 0) {
	//		threadSampler = userData->velocity_samplers[sg->tid] = new Bifrost::API::VoxelSampler(userData->velocity_channel.createSampler(
	//		Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
	//	}
	//	*type = AI_TYPE_RGB;
	//	amino::Math::vec3f col = threadSampler->sample<amino::Math::vec3f>(pos);

	//	// velocity is expressed in seconds, need to convert to frames
	//	value->RGB.r = col[0]; 
	//	value->RGB.g = col[1]; 
	//	value->RGB.b = col[2]; 

	//	value->RGB *= userData->inv_fps;
 //    
	//	return true;
	//}
}

bool BifrostImplicitsGradient(void* user_ptr,
                            const AtVolumeData* data,
                            const AtString channel,
                            const AtShaderGlobals* sg,
                            int interp,
                            AtVector *gradient)
{
	if (!data->private_info) return false;

	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*) data->private_info;
	FrameData *frameData = userData->frameData;
	ImplicitsInputData *inData = userData->inputData;

	amino::Math::vec3f pos;
	pos[0] = sg->P.x;
	pos[1] = sg->P.y;
	pos[2] = sg->P.z;

	int samplerIndexStart = userData->channelSamplerIndexes[ channel ];
	Bifrost::API::VoxelSampler *threadSampler = userData->channelSamplers[ samplerIndexStart + sg->tid ];

	if (threadSampler == 0) {
		if ( inData->diagnostics.DEBUG > 0 ) {
			printf( "Creating a new sampler for channel %s and thread %d...\n", channel.c_str(), sg->tid );
		}
		Bifrost::API::VoxelChannel bifChannel = userData->voxelComponent.findChannel( channel.c_str() );

		threadSampler = new Bifrost::API::VoxelSampler( bifChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace ) );
		userData->channelSamplers[ samplerIndexStart + sg->tid ] = threadSampler;
	}

	amino::Math::vec3f normal;
	if ( samplerIndexStart >= userData->srcChannelSamplerIndexStart && samplerIndexStart < userData->srcChannelSamplerIndexStart + AI_MAX_THREADS ) {
		// this is sampling the input channel so we need to apply the scalar
		//value->FLT = inData->channelScale * threadSampler->sample<float>(pos);
		threadSampler->sampleGradient<float>(pos, normal);
	} else {
		threadSampler->sampleGradient<float>(pos, normal);
	}

	(*gradient)[0] = normal[0];
	(*gradient)[1] = normal[1];
	(*gradient)[2] = normal[2];

	return true;

	//if (!strcmp(channel, "velocity")) {
	//	Bifrost::API::VoxelSampler *threadSampler = userData->primVarSamplers[sg->tid];
	//	if (threadSampler == 0) {
	//		threadSampler = userData->velocity_samplers[sg->tid] = new Bifrost::API::VoxelSampler(userData->velocity_channel.createSampler(
	//		Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
	//	}
	//	*type = AI_TYPE_RGB;
	//	amino::Math::vec3f col = threadSampler->sample<amino::Math::vec3f>(pos);

	//	// velocity is expressed in seconds, need to convert to frames
	//	value->RGB.r = col[0]; 
	//	value->RGB.g = col[1]; 
	//	value->RGB.b = col[2]; 

	//	value->RGB *= userData->inv_fps;
 //    
	//	return true;
	//}
}

void BifrostImplicitsRayExtents(void* user_ptr,
                                const AtVolumeData* data,
                                const AtVolumeIntersectionInfo* info,
                                AtByte tid,
                                float time,
                                const AtPoint* origin,
                                const AtVector* direction,
                                float t0,
                                float t1)
{
	//if (!data->private_info) return;
	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*)user_ptr;
	if (userData == 0) return;

	//float tmin, tmax;

	//float step = userData->inputData->stepSize;
	//tmin = t0;
	//tmax = t1;
	//int i = 0;
	//for (float t = tmin; t < tmax && i < userData->maxSteps; t+= step, i++) {
	//	AiVolumeAddIntersection(info, t, MIN(t+step, tmax));
	//}
	AiVolumeAddIntersection(info, t0, t1);
}

bool BifrostImplicitsUpdate (void *user_ptr, const char *user_string, const AtNode *node, AtVolumeData *data)
{
	return true;
}

} // namespace


// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif

AI_EXPORT_LIB bool VolumePluginLoader(AtVolumePluginVtable* vtable)
{
	vtable->Init			= BifrostImplicitsInit;
	vtable->Cleanup			= BifrostImplicitsCleanup;
	vtable->CreateVolume	= BifrostImplicitsCreate;
	vtable->CleanupVolume	= BifrostImplicitsCleanupVolume;
	vtable->UpdateVolume	= BifrostImplicitsUpdate;
	vtable->Sample			= BifrostImplicitsSample;
#ifdef BIFROST2018
	vtable->Gradient		= BifrostImplicitsGradient;
#endif
	vtable->RayExtents		= BifrostImplicitsRayExtents;
	strcpy(vtable->version, AI_VERSION);
	return true;
}

#ifdef __cplusplus
}
#endif
