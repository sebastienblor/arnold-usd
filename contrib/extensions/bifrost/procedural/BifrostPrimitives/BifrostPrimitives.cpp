//*****************************************************************************
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2015
// Written by Erdem Taylan

#include <Types.h>
#include <Tools.h>
#include <PrimitivesTools.h>

using namespace Bifrost::RenderCore;

bool ProcSubdivide( AIProcNodeData *nodeData, PrimitivesInputData *inData )
{
	//
	//
	// DECLARATIONS
	//
	//

	//
	//
	// INIT
	//
	//

	// init in memory class
	inData->inMemoryRef = new CoreObjectUserData( inData->bifrostObjectName, inData->bifFilename );

	printEndOutput( "[BIFROST PRIMITIVES] START OUTPUT", inData->diagnostics );

	// check render type and exit if not known
	if ( inData->renderType > PRIM_CURVE ) {
		printf( "\nRender Type is set to something bad!\nPlease check!n", inData->inputChannelName );
		printf("\tExiting...\n");

		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

		return false;
	}

    CvFloat radius = inData->radius;

	//
	//
	// CHECK INPUT FILE
	//
	//
	//
	Bifrost::API::String writeToFolder;
	if ( inData->hotData ) {
		// write in memory volume data to a temp file
		Bifrost::API::String writeToFile;
		writeToFile = writeHotDataToDisk( *(inData->inMemoryRef), inData->bifFilename, "Foam-particle", writeToFolder );

		// realloc for the new name
		size_t inputLen = writeToFile.length();
		free ( inData->bifFilename );
		inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
		strcpy( inData->bifFilename, writeToFile.c_str() );
	}

	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes( inData->bifFilename );

	// check for existence of files
	if (FILE *file = fopen(correctedFilename.c_str(), "r")) {
		fclose(file);
	} else {
		printf("BifFile: %s does not exists!\n", correctedFilename.c_str() );
		printf("\tExiting...\n");

		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );
		return false;
	}

	// output parameters to console
 	inData->printParameters();

	//
	//
	// PRELOADING AND INFO GATHERING
	//
	//
	//

	// init FrameData struct that holds information specific to the frame we are rendering
	PrimitivesFrameData *frameData = (PrimitivesFrameData *) new( PrimitivesFrameData );
	frameData->init();
	frameData->pluginType = PLUGIN_PRIMITIVES;
	frameData->tmpFolder = writeToFolder;
	nodeData->frameData = frameData;

	// process which channels to load
	initAndGetPrimitivesFrameData(	frameData, inData, getASSData );

	if ( frameData->error ) {
		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );
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

	if ( inData->clip.on ) {
		loadSt = fio.load( inSS, frameData->loadChannelNames, clipBox);
	} else {
		loadSt = fio.load( inSS, frameData->loadChannelNames);
	}

	end = std::time(NULL);
	double duration = difftime( end, start );

    if( loadSt == Bifrost::API::Status::Failure ) {
		printf("Bif file can not be loaded, please check the file!\n");
		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

		return false;
    }

	//
	//
	// FILE CLASS GATHERING
	//
	//
	//
	Bifrost::API::RefArray components = inSS.components();
	Bifrost::API::RefArray objects = inSS.objects();
	Bifrost::API::Component component = components[0];
	Bifrost::API::Layout layout = component.layout();

	frameData->voxelScale = layout.voxelScale();

	Bifrost::API::RefArray channels = component.channels();
	std::cerr << "\tComponent count: " << components.count() << std::endl;
	std::cerr << "\tComponent: " << component.name() << std::endl;
	std::cerr << "\t\tElementCount: " << component.elementCount(7) << std::endl;
	std::cerr << "\tLayout: " << layout.name() << std::endl;
	std::cerr << "\t\tDepth: " << layout.maxDepth() << std::endl;
	std::cerr << "\t\tVoxelScale: " << frameData->voxelScale << std::endl;
	std::cerr << "\t\tTileCount: " << layout.tileCount() << std::endl;
	for ( size_t d=0; d < layout.depthCount(); d++ ) {
		std::cerr << "\t\tdepth " << d << ": " << layout.tileCount( (Bifrost::API::TreeIndex::Depth) d) << "->" << Bifrost::API::PointChannel(channels[0]).elementCount(d) << std::endl;
	} 
	std::cerr << "\tChannel count: " << channels.count() << std::endl;
	for ( size_t i=0; i<channels.count(); i++ ) {
		std::cerr << "\t\tChannel: " << Bifrost::API::Base(channels[i]).name() << std::endl;
	}

	if ( inData->diagnostics.DEBUG > 1 ) {
		dumpStateServer( inSS, "AFTER LOADING" );
	}

	//
	//
	// GET CACHE CHANNELS
	//
	//
	const Bifrost::API::Channel& posChan = component.findChannel("position");
	const Bifrost::API::Channel& inputChan = component.findChannel(inData->inputChannelName);
	const Bifrost::API::Channel& velChan = component.findChannel("velocity");
	const Bifrost::API::Channel& velChanU = component.findChannel("velocity_u");
	const Bifrost::API::Channel& velChanV = component.findChannel("velocity_v");
	const Bifrost::API::Channel& velChanW = component.findChannel("velocity_w");

	// get id
	const Bifrost::API::Channel idChan = component.findChannel("id64");

	// check for specific channels to decide whether we have a liquid or aero or voxel cache
	const Bifrost::API::Channel smokeChan = component.findChannel("smoke");
	const Bifrost::API::Channel vorticityChan = component.findChannel("vorticity");
	const Bifrost::API::Channel distanceChan = component.findChannel("distance");

	// keep track of what we are rendering
	frameData->cacheType = FOAM;

	if ( frameData->isPointCache ) {
		if ( frameData->inputChannelNames.findFirstByName("vorticity") != frameData->inputChannelNames.npos ) {
			frameData->cacheType = LIQUID;
			printf("\nThis is a LIQUID cache...\n");
		} else if ( frameData->inputChannelNames.findFirstByName("smoke") != frameData->inputChannelNames.npos ) {
			frameData->cacheType = AERO;
			printf("\nThis is an AERO cache...\n");
		} else {
			printf("\nThis is a FOAM cache...\n");
		}
	} else {
		frameData->cacheType = VOXEL;
		printf("\nThis is a VOXEL cache...\n");
	}

	// check input channel range
	if ( inputChan.valid() ) {
		float minVal;
		float maxVal;
		checkInputChannel ( inputChan, minVal, maxVal );

		if ( maxVal < FLT_EPSILON && minVal > -FLT_EPSILON && inData->useChannelToModulateRadius ) {
			printf("You have USECHANNELTOMODULATERADIUS on but your maximum render channel value is zero!\n");
			printf("This means that all your radius values will be zero which makes the primitives not showing up in the render!\n");
			printf("So nothing to render! Exiting...\n");

			printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

			return true;
		}
	}

	// alert user if this is a Liquid cache and  USECHANNELTOMODULATERADIUS is used
	if ( frameData->cacheType == LIQUID && inData->useChannelToModulateRadius && ( strcmp( inData->inputChannelName, const_cast<char*>("density") ) == 0 ) ) {
		printf( "\nBEWARE:\tThis is a LIQUID cache and you used density as the rendering channel!\n" );
		printf( "\tuseChannelToModulateRadius is also ON which means you may not see that much stuff if your density range is low!\n");
		printf( "\tYou can check your density range above\n");
	}

	//
	//
	// PRE EXPORT
	//
	//

	// decide how many items we will export
	if ( frameData->isPointCache ) {
		// position channel should be there
		frameData->nofBaseElements = (CvInt) posChan.elementCount( layout.maxDepth() );
	} else {
		// we have a voxel cache
		// get nof voxels from the component
		frameData->nofBaseElements = (CvInt) component.elementCount( layout.maxDepth() );
	}

	if ( frameData->nofBaseElements == 0 ) {
		// Diego wants to render 0 particles :)
		printf("\tNothing to render! Exiting...\n");

		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

		return true;
	} else {
		printf("\nThe cache file has %d elements...\n", frameData->nofBaseElements );
	}

	// if id channel is valid and there is skip
	// we need to figure out number of particles we will export
	CvInt multiBase = 0;
	if ( idChan.valid() && inData->skip > 1 ) {
		CalcNofParticlesVisitor nofParticlesVisitor ( idChan, inData->skip );
		layout.traverse(nofParticlesVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );
		multiBase = nofParticlesVisitor.m_nofParticles;
	} else {
		if ( frameData->isPointCache ) {
			multiBase = inData->skip > 1 ? ( 1 + frameData->nofBaseElements / inData->skip ) : frameData->nofBaseElements;
		} else {
			if ( inData->skip > 1 ) {
				multiBase = (frameData->nofBaseElements % inData->skip) == 0 ? frameData->nofBaseElements / inData->skip : frameData->nofBaseElements / inData->skip + 1;
			} else {
				multiBase = frameData->nofBaseElements;
			}
		}
	}

	frameData->nofElements = (CvInt) inData->mpSamples * multiBase;

	frameData->isMultiPointing = inData->mpSamples > 1 ? true : false;

	if (frameData->isMultiPointing) {
		printf("\tWill export %d total elements for %d original elements...\n", frameData->nofElements, frameData->nofBaseElements );	
	} else {
		if ( inData->skip > 1 ) {
			printf("\tWill export %d elements after skipping...\n", frameData->nofElements );
		} else {
			printf("\tWill export %d elements...\n", frameData->nofElements );
		}
	}

	// now calculate a new chunksize depending on mpSamples. it is easier this way
	// if mpSamples == 1, this would be the same as the input chunkSize
	frameData->finalChunkSize = inData->mpSamples * ( inData->chunkSize / inData->mpSamples );

	// report id range
	reportIdRange( idChan );

	//
	//
	// CREATE PRIMVARINFO STRUCTURE
	//
	//
	initPrimitivesPVarInfo( frameData, component );

	printf("\nProcessing PRIMVARS List...\n");
	
	checkPrimitivesPrimVars( frameData, component );

	// print out primVars
	if (frameData->primVars.size() > 0 ) {
		printf("\tFinal list of Primvars to Export:\n");
		for (unsigned i = 0; i < frameData->primVars.size(); i++) {
			if ( frameData->primVars[i].exportToRIB ) {
				printf("\t\t%s\n", frameData->primVars[i].name.c_str() );
			}
		}
	} else {
		printf ( "\tNo primvars found to export!\n" );
	}

	//
	//
	// NOW THAT WE HAVE EVERYTHING, PROCESS AND EXPORT
	//
	//
	frameData->minDistance = std::numeric_limits<float>::max();
	frameData->maxDistance = -std::numeric_limits<float>::max();

	int xMulti = dumpPrimitives	(	inData,
									frameData,
									component,
									posChan,
									inputChan,
									velChan,
									velChanU,
									velChanV,
									velChanW,
									idChan,
									smokeChan,
									vorticityChan,
									distanceChan,
									(void *) nodeData,
									calcNoiseAI,
									exportPrimitivesAI,
									transformPointsAI,
									getAndSetThreadDataAI,
									setExportTokenAI,
									getPrimVarTokenAI
								);
	//int xMulti = 0;

	if (frameData->isMultiPointing) {
		printf("\t\tFinished exporting %ld items including multipointing!\n", xMulti);	
	} else {
		printf("\t\tFinished exporting %ld items!\n", xMulti);	
	}

	//
	//
	// REPORT
	//
	//
	reportPrimVars( frameData );

	// report min max for distance for cam based radius
	if ( inData->camRadius ) {
		printf("\nMin-Max ranges for cam based radius distance:\n");
		printf( "\t\tMin: %f\n", frameData->minDistance );
		printf( "\t\tMax: %f\n\n", frameData->maxDistance );
	}

	reportPrimitivesBounds( inputChan, frameData->voxelScale, inData->diagnostics );

	//
	//
	// CLEAR AND RETURN MEMORY
	//
	//
	if ( inData->diagnostics.DEBUG > 1 ) {
		dumpStateServer( inSS, "BEFORE CLEARING" );
	}

	clearStateServer( inSS );

	if ( inData->diagnostics.DEBUG > 1 ) {
		dumpStateServer( inSS, "AFTER CLEARING" );
	}

	printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

	return true;
}

// we read the UI parameters into their global vars
static int ProcInit( AtNode *myNode, void **user_ptr )
{
	// create nodeData
	AIProcNodeData *nodeData = new AIProcNodeData();

	// create Input Data
	PrimitivesInputData *inData = (PrimitivesInputData *) malloc( sizeof( PrimitivesInputData ) );
	inData->diagnostics.silent = 0;
	nodeData->inData = inData;

	std::string objectName = AiNodeLookUpUserParameter(myNode, "bifrostObjectName") ? AiNodeGetStr(myNode, "bifrostObjectName") : "";
	std::string particleFilename = AiNodeLookUpUserParameter(myNode, "bifFilename") ? AiNodeGetStr(myNode, "bifFilename") : "";

	inData->inMemoryRef = new CoreObjectUserData(objectName.c_str(), particleFilename.c_str());
	nodeData->objectName = objectName;
	nodeData->file = particleFilename;

	// store values
	nodeData->proceduralNode = myNode;
	AiNodeGetMatrix ( myNode, "matrix", nodeData->world2Obj );
	AiM4Invert ( nodeData->world2Obj, nodeData->obj2World );

	nodeData->camNode = AiUniverseGetCamera();
	AiWorldToCameraMatrix( nodeData->camNode, 0.0f, nodeData->world2Cam );
	AiM4Mult( nodeData->obj2Cam, nodeData->obj2World, nodeData->world2Cam );

	nodeData->bifrostCtx = AiShaderGlobals();
	nodeData->samplerPool.clear();
	nodeData->nofNodesCreated = 0;

	// make a copy of the nodeData
	*user_ptr = (void *) nodeData;

	printf( "\n" );
	inData->error = false;

	inData->renderType = ( PrimitivesRenderType ) AiNodeGetInt(myNode, "renderType");
	inData->channelScale = AiNodeGetFlt(myNode, "channelScale");
	inData->exportNormalAsPrimvar = AiNodeGetBool(myNode, "exportNormalAsPrimvar");

	inData->velocityScale = AiNodeGetFlt(myNode, "velocityScale");
	inData->fps = AiNodeGetFlt(myNode, "fps");
	inData->spaceScale = AiNodeGetFlt(myNode, "spaceScale");
	inData->skip = AiNodeGetInt(myNode, "skip");
	inData->chunkSize = AiNodeGetInt(myNode, "chunkSize");

	inData->clip.on = AiNodeGetBool(myNode, "clipOn");
	inData->clip.minX = AiNodeGetFlt(myNode, "clipMinX");
	inData->clip.maxX = AiNodeGetFlt(myNode, "clipMaxX");
	inData->clip.minY = AiNodeGetFlt(myNode, "clipMinY");
	inData->clip.maxY = AiNodeGetFlt(myNode, "clipMaxY");
	inData->clip.minZ = AiNodeGetFlt(myNode, "clipMinZ");
	inData->clip.maxZ = AiNodeGetFlt(myNode, "clipMaxZ");

	inData->radius = AiNodeGetFlt(myNode, "pointRadius");
	inData->useChannelToModulateRadius = AiNodeGetBool(myNode, "useChannelToModulateRadius");

	inData->camRadius = AiNodeGetBool(myNode, "camRadiusOn");
	inData->camRadiusStartDistance = AiNodeGetFlt(myNode, "camRadiusStartDistance");
	inData->camRadiusEndDistance = AiNodeGetFlt(myNode, "camRadiusEndDistance");
	inData->camRadiusStartFactor = AiNodeGetFlt(myNode, "camRadiusStartFactor");
	inData->camRadiusEndFactor = AiNodeGetFlt(myNode, "camRadiusEndFactor");
	inData->camRadiusFactorExponent = AiNodeGetFlt(myNode, "camRadiusFactorExponent");

	inData->mpSamples =  AiNodeGetInt(myNode, "mpSamples");
	inData->mpMinRadius = AiNodeGetFlt(myNode, "mpMinRadius");
	inData->mpMaxRadius = AiNodeGetFlt(myNode, "mpMaxRadius");
	inData->mpSurfaceAttract = AiNodeGetFlt(myNode, "mpSurfaceAttract");
	inData->mpFalloffType = ( FalloffType ) AiNodeGetInt(myNode, "mpFalloffType");
	inData->mpFalloffStart = AiNodeGetFlt(myNode, "mpFalloffStart");
	inData->mpFalloffEnd = AiNodeGetFlt(myNode, "mpFalloffEnd");
	inData->mpDisplacementValue = AiNodeGetFlt(myNode, "mpDisplacementValue");
	inData->mpDisplacementNoiseFrequency = AiNodeGetFlt(myNode, "mpDisplacementNoiseFrequency");

	inData->diagnostics.DEBUG = AiNodeGetInt(myNode, "debug");

	inData->hotData = AiNodeGetBool(myNode, "hotData");

	const AtString bifFilenameParam("bifFilename");
	const AtString bifFilename = AiNodeGetStr(myNode, bifFilenameParam );
	size_t inputLen = bifFilename.length();
	inData->bifFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifFilename, bifFilename.c_str() );

	const AtString primVarNamesParam("primVarNames");
	const AtString primVarNames = AiNodeGetStr(myNode, primVarNamesParam );
	inputLen = primVarNames.length();
	inData->primVarNames = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->primVarNames, primVarNames.c_str() );

	const AtString inputChannelNameParam("inputChannelName");
	const AtString inputChannelName = AiNodeGetStr(myNode, inputChannelNameParam );
	inputLen = inputChannelName.length();
	inData->inputChannelName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->inputChannelName, inputChannelName.c_str() );

	const AtString bifrostObjectNameParam("bifrostObjectName");
	const AtString bifrostObjectName = AiNodeGetStr(myNode, bifrostObjectNameParam );
	inputLen = bifrostObjectName.length();
	inData->bifrostObjectName = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->bifrostObjectName, bifrostObjectName.c_str() );

	// arnold specific parameters
	inData->motionBlur = AiNodeGetBool( myNode, "motionBlur" );
	inData->shutterStart = AiNodeGetFlt( myNode, "shutterStart" );
	inData->shutterEnd = AiNodeGetFlt( myNode, "shutterEnd" );

	//check params
	inData->checkParameters();

	if ( inData->error ) {
		return false;
	} else {
		// now do creation of nodes
		return ProcSubdivide( nodeData, inData );
	}
}

// we will create one node per chunk as set in the UI
static int ProcNumNodes( void *user_ptr )
{
	AIProcNodeData *nodeData = (AIProcNodeData *) user_ptr;
	PrimitivesInputData *inData = (PrimitivesInputData *) nodeData->inData;

	if ( inData->diagnostics.DEBUG > 1 ) {
		printf( "%d nodes created\n", (int) nodeData->createdNodes.size() );
	}
	return (int) nodeData->createdNodes.size();
}

// this is the function that Arnold calls to request the nodes
// that this procedural creates.
static AtNode *ProcGetNode(void *user_ptr, int i)
{
	AIProcNodeData *nodeData = (AIProcNodeData *) user_ptr;
	PrimitivesInputData *inData = (PrimitivesInputData *) nodeData->inData;

	if ( i >= 0 && i < (int) nodeData->createdNodes.size() )
	{
		const char* nodeName = AiNodeGetName( nodeData->createdNodes[i] );

		if ( inData->diagnostics.DEBUG > 1 ) {
			printf("Creating node: %s\n", nodeName);
		}

		return nodeData->createdNodes[ i ];
	} else {
		return NULL;
	}
}

static int ProcCleanup( void *user_ptr )
{
	AIProcNodeData *nodeData = (AIProcNodeData *) user_ptr;

	// free mem
	if ( nodeData ) {
		PrimitivesInputData *inData = (PrimitivesInputData *)  nodeData->inData;
		PrimitivesFrameData *frameData = (PrimitivesFrameData *) nodeData->frameData;

		if ( frameData ) {
			if ( inData->hotData ) {
				Bifrost::API::File::deleteFolder( frameData->tmpFolder );
			}

			// free the assembled data arrays
			for (int i = 0; i < frameData->mem.size(); i++) {
				free ( frameData->mem[i] );
			}
		}

		if ( nodeData->inData ) {
			free( inData->inputChannelName );
			free( inData->primVarNames );
			free( inData->bifFilename );
			free( inData->bifrostObjectName );

			delete inData->inMemoryRef;

			free( inData );
		}
	}

	if ( nodeData->bifrostCtx ) {
		AiShaderGlobalsDestroy( nodeData->bifrostCtx );
	}

	delete nodeData;

	return 1;
}

extern "C"
{
	AI_EXPORT_LIB int ProcLoader(AtProcVtable *vtable)
	// vtable passed in by proc_loader macro define
	{
		vtable->Init     = ProcInit;
		vtable->Cleanup  = ProcCleanup;
		vtable->NumNodes = ProcNumNodes;
		vtable->GetNode  = ProcGetNode;
		strcpy(vtable->version, AI_VERSION);
		return 1;
	}
}
