//*

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

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl
AI_VOLUME_NODE_EXPORT_METHODS(BifrostVolumeMtd)

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

struct BifrostVolumeUserData {
	CoreObjectUserData *objectRef;

	Bifrost::API::VoxelSampler **channelSamplers;
	std::map<AtString, int> channelSamplerIndexes;
	int srcChannelSamplerIndexStart;

	Bifrost::API::Component voxelComponent;

	VolumeInputData *inputData;
	FrameData *frameData;

	std::string objectName;
	std::string file;
	AtBBox bbox;
	float stepSize;
	int maxSteps;

	bool shadowing;
	float shadowingStepSize;
	int shadowingMaxSteps;

};

bool getNodeParameters( VolumeInputData *inData, const AtNode *node )
{
	inData->error = false;

	// get numeric data
    inData->channelScale = AiNodeGetFlt(node, "channelScale");
    inData->velocityScale = AiNodeGetFlt(node, "velocityScale");
    inData->fps = AiNodeGetFlt(node, "fps");
    inData->spaceScale = AiNodeGetFlt(node, "spaceScale");

    inData->smooth.mode = (SmoothFilterType) AiNodeGetInt( node, "smoothMode" );
    inData->smooth.kernelSize = AiNodeGetInt( node, "smoothKernelSize" );
    inData->smooth.iterations = AiNodeGetInt( node, "smoothIterations" );
    inData->smooth.weight = AiNodeGetFlt( node, "smoothWeight" );
    inData->smooth.remapMin = AiNodeGetFlt(node, "smoothRemapMin");
    inData->smooth.remapMax = AiNodeGetFlt(node, "smoothRemapMax");
    inData->smooth.remapInvert = AiNodeGetBool(node, "smoothRemapInvert");

    inData->clip.on = AiNodeGetBool( node, "clipOn" );
    inData->clip.minX = AiNodeGetFlt(node, "clipMinX");
    inData->clip.maxX = AiNodeGetFlt(node, "clipMaxX");
    inData->clip.minY = AiNodeGetFlt(node, "clipMinY");
    inData->clip.maxY = AiNodeGetFlt(node, "clipMaxY");
    inData->clip.minZ = AiNodeGetFlt(node, "clipMinZ");
    inData->clip.maxZ = AiNodeGetFlt(node, "clipMaxZ");

    inData->skip = std::max(1, AiNodeGetInt( node, "skip" ) );
    inData->splatSamples = AiNodeGetInt( node, "splatSamples" );
    inData->splatMinRadius = AiNodeGetFlt(node, "splatMinRadius");
    inData->splatMaxRadius = AiNodeGetFlt(node, "splatMaxRadius");
    inData->splatSurfaceAttract = AiNodeGetFlt(node, "splatSurfaceAttract");
    inData->splatFalloffType = (FalloffType) AiNodeGetInt( node, "splatFalloffType" );
    inData->splatFalloffStart = AiNodeGetFlt(node, "splatFalloffStart");
    inData->splatFalloffEnd = AiNodeGetFlt(node, "splatFalloffEnd");
    inData->splatDisplacement = AiNodeGetFlt(node, "splatDisplacement");
    inData->splatNoiseFreq = AiNodeGetFlt(node, "splatNoiseFreq");

    inData->diagnostics.DEBUG = AiNodeGetInt( node, "debug" );

	// get string data
	const AtString bifFilenameParam("bifFilename");
    const AtString bifFilename = AiNodeGetStr(node, bifFilenameParam );
	size_t inputLen = bifFilename.length();
	inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifFilename, bifFilename.c_str() );

	const AtString inputChannelNameParam("inputChannelName");
    const AtString inputChannelName = AiNodeGetStr(node, inputChannelNameParam );
	inputLen = inputChannelName.length();
	inData->inputChannelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->inputChannelName, inputChannelName.c_str() );

	const AtString smoothChannelNameParam("smoothChannelName");
    const AtString smoothChannelName = AiNodeGetStr(node, smoothChannelNameParam );
	inputLen = smoothChannelName.length();
	inData->smooth.channelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->smooth.channelName, smoothChannelName.c_str() );

    const AtString primVarNamesParam("primVarNames");
    const AtString primVarNames = AiNodeGetStr(node, primVarNamesParam );
	inputLen = primVarNames.length();
	inData->primVarNames = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->primVarNames, primVarNames.c_str() );

	const AtString bifrostObjectNameParam("bifrostObjectName");
    const AtString bifrostObjectName = AiNodeGetStr(node, bifrostObjectNameParam );
	inputLen = bifrostObjectName.length();
	inData->bifrostObjectName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifrostObjectName, bifrostObjectName.c_str() );

	// arnold specific parameters
    inData->motionBlur = AiNodeGetBool( node, "motionBlur" );
    inData->shutterStart = AiNodeGetFlt( node, "shutterStart" );
    inData->shutterEnd = AiNodeGetFlt( node, "shutterEnd" );

	inData->checkParameters();

	return inData->error;
}

} // namespace

node_parameters
{
    // get numeric data
    AiParameterFlt("channelScale", 0);
    AiParameterFlt("velocityScale", 0);
    AiParameterFlt("fps", 0);
    AiParameterFlt("spaceScale", 0);

    AiParameterBool( "smoothOn" , 0);
    AiParameterInt( "smoothMode" , 0);
    AiParameterInt( "smoothKernelSize" , 0);
    AiParameterInt( "smoothIterations" , 0);
    AiParameterFlt( "smoothWeight" , 0);
    AiParameterFlt("smoothRemapMin", 0);
    AiParameterFlt("smoothRemapMax", 0);
    AiParameterBool("smoothRemapInvert", 0);

    AiParameterBool( "clipOn" , 0);
    AiParameterFlt("clipMinX", 0);
    AiParameterFlt("clipMaxX", 0);
    AiParameterFlt("clipMinY", 0);
    AiParameterFlt("clipMaxY", 0);
    AiParameterFlt("clipMinZ", 0);
    AiParameterFlt("clipMaxZ", 0);

    AiParameterInt( "skip" , 0);
    AiParameterInt( "splatSamples" , 0);
    AiParameterFlt("splatMinRadius", 0);
    AiParameterFlt("splatMaxRadius", 0);
    AiParameterFlt("splatSurfaceAttract", 0);
    AiParameterInt( "splatFalloffType" , 0);
    AiParameterFlt("splatFalloffStart", 0);
    AiParameterFlt("splatFalloffEnd", 0);
    AiParameterFlt("splatDisplacement", 0);
    AiParameterFlt("splatNoiseFreq", 0);

    AiParameterInt( "debug" , 0);
    AiParameterBool( "hotData" , 0);

    AiParameterStr("bifFilename" , "");
    AiParameterStr("inputChannelName" , "");
    AiParameterStr("smoothChannelName" , "");
    AiParameterStr("primVarNames" , "");
    AiParameterStr("bifrostObjectName" , "");

    // arnold specific parameters
    AiParameterBool( "motionBlur" , 0);
    AiParameterFlt( "shutterStart" , 0);
    AiParameterFlt( "shutterEnd" , 0);
}

volume_create
{
    BifrostVolumeUserData *pdata = new BifrostVolumeUserData;
    data->private_info = pdata;
	//
	//
	// INIT
	//
	//
    pdata->objectRef = 0;
    pdata->srcChannelSamplerIndexStart = -1;

	// init VolumeInputData
	struct VolumeInputData *inData = (struct VolumeInputData *) new ( struct VolumeInputData);
    pdata->inputData = inData;

    // log start
	printEndOutput( "[BIFROST VOLUME] START OUTPUT", inData->diagnostics );

    // get input pdata
	bool error = getNodeParameters( inData, node );

    // init in memory class
    inData->inMemoryRef = new CoreObjectUserData( inData->bifrostObjectName, inData->bifFilename );

    bool hotData = inData->inMemoryRef->objectExists();

    // init user pdata stuff
    pdata->objectRef = inData->inMemoryRef;
    pdata->objectName = inData->bifrostObjectName;
    pdata->file = inData->bifFilename;

	// FIX THIS
	//if(shader) {
    //	pdata->stepSize = AiNodeGetFlt(shader, "aiStepSize");
    //	pdata->maxSteps = AiNodeGetInt(shader, "aiMaxSteps");
    //	pdata->shadowing = AiNodeGetBool(shader, "aiShadowing");
    //	pdata->shadowingStepSize = AiNodeGetFlt(shader, "aiShadowingStepSize");
    //	pdata->shadowingMaxSteps = AiNodeGetInt(shader, "aiShadowingMaxSteps");
	//} else {
        pdata->stepSize = 0.5f;
        pdata->maxSteps = 100;
        pdata->shadowing = true;
        pdata->shadowingStepSize = 0.5f;
        pdata->shadowingMaxSteps = 100;
	//}

    data->auto_step_size = pdata->stepSize;

	//
	//
	// CHECK INPUT FILE
	//
    //
	Bifrost::API::String writeToFolder;
    if ( hotData ) {
        // write in memory pdata to a temp file
		Bifrost::API::String writeToFile;
		if ( strstr( inData->bifFilename, "volume" ) != NULL ) {
			writeToFile = writeHotDataToDisk( *(inData->inMemoryRef), inData->bifFilename, "AeroObject-volume", writeToFolder );
		} else {
			writeToFile = writeHotDataToDisk( *(inData->inMemoryRef), inData->bifFilename, "AeroObject-particle", writeToFolder );
		}

		// realloc for the new name
		size_t inputLen = writeToFile.length();
		free ( inData->bifFilename );
		inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
		strcpy( inData->bifFilename, writeToFile.c_str() );
	}

	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes(inData->bifFilename);

    // check for existence of files if we don't have hot pdata
    if ( !hotData ) {
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
			printEndOutput( "[BIFROST VOLUME] END OUTPUT", inData->diagnostics );
			return false;
		}
	}

	//
	//
	// PRELOADING AND INFO GATHERING
	//
	//

	// init FrameData struct that holds information specific to the frame we are rendering
	FrameData *frameData = (FrameData *) new (FrameData);
	frameData->init();
	frameData->pluginType = PLUGIN_VOLUME;
	frameData->tmpFolder = writeToFolder;

    pdata->frameData = frameData;

    inData->diagnostics.DEBUG = 1;
	// process which channels to load
	initAndGetFrameData(	frameData,
							(void *) inData,
							inData->bifFilename,
							inData->inputChannelName,
							inData->primVarNames,
							inData->smooth.channelName,
							NULL,
							VOLUME,
							inData->diagnostics,
							getASSData );

	if ( frameData->error ) {
		printEndOutput( "[BIFROST VOLUME] END OUTPUT", inData->diagnostics );
		return false;
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
    frameData->loadChannelNames.add(inData->inputChannelName);

	if ( inData->clip.on ) {
		loadSt = fio.load( inSS, frameData->loadChannelNames, clipBox);
	} else {
		loadSt = fio.load( inSS, frameData->loadChannelNames);
	}

	end = std::time(NULL);
	double duration = difftime( end, start );

    if( loadSt == Bifrost::API::Status::Failure ) {
		IFNOTSILENT { printf( "Bif file %s can not be loaded!\nPlease check the file!\n", inData->bifFilename ); }
		return false;
    }

	//
	//
	// FILE CLASS GATHERING
	//
	//
	//

    // declare an SS for holding pdata if we do particle splatting
	Bifrost::API::StateServer srcSS = inSS;

	// now that we have a bif file loaded
	// let's declare some variables for keeping track of what information we need during rendering
	Bifrost::API::RefArray objects = inSS.objects();
	Bifrost::API::Object inObj = objects[0];
	frameData->inObj = inObj;
	Bifrost::API::RefArray components = inSS.components();
	Bifrost::API::Component component = components[0];
	Bifrost::API::Layout layout = component.layout();
	Bifrost::API::RefArray channels = component.channels();

    // we may need to get the metapdata once more because of older version files
	if ( frameData->metaDataLoaded == 0 ) {
		getMetaDataFlags ( fio, frameData );
	}

	// print parameters
	IFNOTSILENT { inData->printParameters( frameData->isPointCache ); }

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

    // dump pdata before channel processing
	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 1 ) {
			dumpStateServer( srcSS, "BEFORE PROCESSING" );
		}
	}

	//
	//
	// GET CACHE CHANNELS
	//
	//

	const Bifrost::API::Channel idChannel = component.findChannel( Bifrost::API::String( "id64") );
	const Bifrost::API::Channel dropletChannel = component.findChannel( Bifrost::API::String( "droplet") );
	const Bifrost::API::Channel airDistanceChannel = component.findChannel( Bifrost::API::String( "airDistance") );

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
	// DO SPLATTING
	//
	//
	//

	if ( cacheType == PARTICLE ) {
		Bifrost::API::Component outComponent = splatParticlesToVoxels( inObj, component, inData, srcSS, calcNoiseAI );

		component = outComponent;
		layout = outComponent.layout();
		channels = outComponent.channels();

		// we are done with original cache release memory associated with it
		clearStateServer( inSS );
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
	// AT THIS POINT EVERYTHING IS VOXELIZED. PARTICLE CACHES ARE TRANSFORMED INTO A VOXEL REP BY SPLATTING.
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
		createSpeedChannel( srcSS, frameData->orgInputChannel, component, inData->diagnostics );
	}

	if ( frameData->uvNeeded ) {
		IFNOTSILENT {
			if ( frameData->speedNeeded ) {
				printf("\tu\n\tv\n");
			} else {
				printf("\nCreating primvar: \n\tu\n\tv\n");
			}
		}

		// separate uv channel to u1 and v1
		createUVChannels( srcSS, frameData->orgInputChannel, component, inData->diagnostics );
	}

	//
	//
	// CREATE PRIMVARINFO STRUCTURE
	//
	//
	initPVarInfo( frameData );

	IFNOTSILENT { printf("\nProcessing PRIMVARS List...\n"); }

	checkPrimVars( frameData, srcSS, component, inData->inputChannelName, inData->diagnostics );

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

	// reset layout voxelscale to accomodate spaceScale
	layout.setVoxelScale( layout.voxelScale() * inData->spaceScale );
	frameData->layout = layout;
	frameData->bifInfo.maxDepth = layout.maxDepth();
	frameData->voxelScale = layout.voxelScale();
	frameData->orgVoxelScale = layout.voxelScale() / inData->spaceScale;
	frameData->bifInfo.calcTileVolumes( layout );
	frameData->srcChannel = frameData->orgInputChannel;

	// init level lookup counts and depthWidths
	for (int i = 0; i <= layout.maxDepth(); i++) {
		frameData->bifInfo.levelDepthWidth[i] = layout.tileDimInfo(i).depthWidth;
		frameData->bifInfo.levelLookupCount[i] = 0;
		frameData->bifInfo.fastLookups[i] = 0;
		frameData->bifInfo.tilesWithDataCount[i] = 0;
		frameData->bifInfo.tilesWithChildTiles[i] = 0;
	}

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
    if ( inData->smooth.kernelSize > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) {
		IFNOTSILENT {
			printf("\nPost Processing %s channel...\n", inData->inputChannelName);
			printf("\tPost processing parameters:\n");
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
			printf("\t\tSmoothing FilterType: %s KernelSize: %d Iterations: %d Weight: %f FilterChannel: %s\n", filterType.c_str(),
																												inData->smooth.kernelSize,
																												inData->smooth.iterations,
																												inData->smooth.weight,
																												inData->smooth.channelName );
		}

		IFNOTSILENT { printf("\tSmoothing...\n"); }

		// Run the smoothing filter
		smoothChannel(	inData->smooth,
						srcSS,
						frameData->srcChannel,
						frameData,
						inData->diagnostics,
						VOLUME );

		IFNOTSILENT { printf("\n"); }
	} // end smooth check

	//
	//
	// CALC BBOX
	//
	//
	double bboxMin[3] = { 0.0, 0.0, 0.0 };
	double bboxMax[3] = { 0.0, 0.0, 0.0 };
	computeAeroBounds( frameData->srcChannel, bboxMin, bboxMax);
    pdata->bbox.min.x = (float) bboxMin[0];
    pdata->bbox.min.y = (float) bboxMin[1];
    pdata->bbox.min.z = (float) bboxMin[2];
    pdata->bbox.max.x = (float) bboxMax[0];
    pdata->bbox.max.y = (float) bboxMax[1];
    pdata->bbox.max.z = (float) bboxMax[2];

    data->bbox = pdata->bbox;

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

	reportChannelRanges ( frameData, component, inData->diagnostics );

	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 1 ) {
			dumpStateServer( srcSS, "AFTER ALL OPS" );
		}
	}

	printEndOutput( "[BIFROST VOLUME] END OUTPUT", inData->diagnostics );

	// now allocate space for samplers
    pdata->voxelComponent = component;
	int samplerChannelCount = 0;

    for ( unsigned int i = 0; i < channels.count(); i++ ) {
		Bifrost::API::Channel channel = (Bifrost::API::Channel) channels[i];
		AtString tmpString ( channel.name().c_str() );
		int startIndex = samplerChannelCount * AI_MAX_THREADS;
        pdata->channelSamplerIndexes[ tmpString ] = startIndex;

		if ( tmpString == Bifrost::API::String( inData->inputChannelName ) ) {
            pdata->srcChannelSamplerIndexStart = startIndex;
		}

		if ( inData->diagnostics.DEBUG > 0 ) {
            printf("%s - %d - %d\n", channel.name().c_str(), pdata->channelSamplerIndexes[ tmpString ], pdata->srcChannelSamplerIndexStart );
		}

		samplerChannelCount++;		
	}

    pdata->channelSamplers = ( Bifrost::API::VoxelSampler ** ) malloc( samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );
    memset( pdata->channelSamplers, 0, samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );

	return true;
}

volume_cleanup
{
    BifrostVolumeUserData *volData = (BifrostVolumeUserData*) data->private_info;
    if(!volData){ return  false; }

    FrameData *frameData = volData->frameData;
    VolumeInputData *inData = volData->inputData;

    if(frameData){
        if(!frameData->tmpFolder.empty()){
            Bifrost::API::File::deleteFolder( frameData->tmpFolder );
        }
        delete frameData;
    }

    if(inData){
        free( inData->bifFilename );
        free( inData->inputChannelName );
        free( inData->smooth.channelName );
        free( inData->primVarNames );
        free( inData->bifrostObjectName );

        if ( inData->inMemoryRef ) {
            delete inData->inMemoryRef;
        }
        delete inData;
    }

    delete volData;
	data->private_info = NULL;
	return true;
}

volume_gradient
{
    *gradient = AI_V3_ZERO;
    return false;
}

volume_sample
{
	if (!data->private_info) return false;

	BifrostVolumeUserData *volData = (BifrostVolumeUserData*) data->private_info;
	VolumeInputData *inData = volData->inputData;

	amino::Math::vec3f pos;
	pos[0] = sg->P.x;
	pos[1] = sg->P.y;
	pos[2] = sg->P.z;

    std::map<AtString, int>::iterator channelIt = volData->channelSamplerIndexes.find(channel);
    if (channelIt == volData->channelSamplerIndexes.end())
    {
        value->FLT() = 0.f;
        return false;
    }

    int samplerIndexStart = channelIt->second;
	
	Bifrost::API::VoxelSampler *threadSampler = volData->channelSamplers[ samplerIndexStart + sg->tid ];

	if (threadSampler == 0) {
		Bifrost::API::VoxelChannel bifChannel = volData->voxelComponent.findChannel( channel.c_str() );

		threadSampler = new Bifrost::API::VoxelSampler( bifChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace ) );
		volData->channelSamplers[ samplerIndexStart + sg->tid ] = threadSampler;
	}

	*type = AI_TYPE_FLOAT;
	if ( samplerIndexStart >= volData->srcChannelSamplerIndexStart && samplerIndexStart < volData->srcChannelSamplerIndexStart + AI_MAX_THREADS ) {
		// this is sampling the input channel so we need to apply the scalar
        value->FLT() = inData->channelScale * threadSampler->sample<float>(pos);
	} else {
        value->FLT() = threadSampler->sample<float>(pos);
	}

	return true;
}

volume_ray_extents
{
	//if (!data->private_info) return;
    BifrostVolumeUserData *volData = (BifrostVolumeUserData*)data->private_info;
	if (volData == 0) return;


	float tmin, tmax;

	float step = volData->stepSize;
	tmin = t0;
	tmax = t1;
	int i = 0;
	for (float t = tmin; t < tmax && i < volData->maxSteps; t+= step, i++) {
        AiVolumeAddIntersection(info, t, (t+step < tmax)? t+step : tmax);
    }
}

volume_update
{
    return true;
}

//*/