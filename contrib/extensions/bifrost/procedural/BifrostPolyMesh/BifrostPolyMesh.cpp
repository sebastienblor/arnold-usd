//*****************************************************************************
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

#include <vector>
#include <stdint.h>

#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_types.h>
#include <bifrostapi/bifrost_status.h>
//#include <bifrostapi/bifrost_refarray.h>
//#include <bifrostapi/bifrost_stringarray.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>
#include <bifrostapi/bifrost_tile.h>
#include <bifrostapi/bifrost_tileiterator.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_mesh.h>
#include <bifrostapi/bifrost_all.h>

#include <ai.h>

#include <bifrostrendercore/CoreTypes.h>
#include <bifrostrendercore/CoreTools.h>
#include <bifrostrendercore/CorePrimVars.h>
#include <AITypes.h>
#include <AITools.h>

#if defined(_Windows) || defined(_WIN32)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif 

using namespace Bifrost::RenderCore;

// Calculate magnitude of velocity visitor
struct CalcVelocityMagnitudeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel&	m_vel;
	const Bifrost::API::PointChannel&	m_dst;
	Bifrost::API::TileAccessor			m_acc;
	float								m_minVal;
	float								m_maxVal;

	CalcVelocityMagnitudeVisitor(
		const Bifrost::API::PointChannel&	in_vel,
		const Bifrost::API::PointChannel&	in_dst
	)
		: m_vel(in_vel), m_dst(in_dst)
	{
		// A new accessor for this thread
		Bifrost::API::Layout layout = m_vel.layout();
		m_acc = layout.tileAccessor();
		m_minVal = std::numeric_limits<float>::max();
		m_maxVal = -std::numeric_limits<float>::max();
	}

	CalcVelocityMagnitudeVisitor(const CalcVelocityMagnitudeVisitor& other)
		: m_vel(other.m_vel), m_dst(other.m_dst)
	{
		m_minVal = std::numeric_limits<float>::max();
		m_maxVal = -std::numeric_limits<float>::max();
		// A new accessor for this thread
		Bifrost::API::Layout layout = m_vel.layout();
		m_acc = layout.tileAccessor();
	}

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index)
	{
		Bifrost::API::TileData<amino::Math::vec3f> inVelData = m_vel.tileData<amino::Math::vec3f>( index );
		Bifrost::API::TileData<float> outData = m_dst.tileData<float>( index );
		for ( size_t k = 0; k < inVelData.count(); k++ ) {
			float speed = sqrt( inVelData[k][0] * inVelData[k][0] + inVelData[k][1] * inVelData[k][1] + inVelData[k][2] * inVelData[k][2] );
			outData[k] = speed;
			m_minVal = std::min(m_minVal, speed);
			m_maxVal = std::max(m_maxVal, speed);
		}
	}

	Bifrost::API::Visitor* copy() const
	{
		return new CalcVelocityMagnitudeVisitor(*this);
	}

	void join(const Bifrost::API::Visitor& visitor)
	{
		const CalcVelocityMagnitudeVisitor& other = dynamic_cast<const CalcVelocityMagnitudeVisitor&>(visitor);
		m_minVal = std::min(m_minVal, other.m_minVal);
		m_maxVal = std::max(m_maxVal, other.m_maxVal);
	}
};

void createSpeedChannel(Bifrost::API::StateServer& inSS)
{
	// get the component
	Bifrost::API::PointComponent point = inSS.findComponent( "voxel_liquid-particle" );
	Bifrost::API::Layout layout = point.layout();
	Bifrost::API::PointChannel velocityChannel = point.findChannel( "velocity" );

	// speed is magnitude of velocity so get velocity channel and calc and put magnitude in a channel called speed
	Bifrost::API::PointChannel speedChannel = inSS.createChannel ( point, Bifrost::API::FloatType, "speed" );
	CalcVelocityMagnitudeVisitor magVisitor ( velocityChannel, speedChannel);
	layout.traverse(magVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst);
}

bool ProcSubdivide( AIProcNodeData *nodeData, PolyMeshInputData *inData )
{
	Bifrost::API::String pointFilename = inData->particleFilename;
	Bifrost::API::String voxelFilename = inData->voxelFilename;
	//
	//
	// CHECK INPUT FILE
	//
	//
	Bifrost::API::String writeToFolderParticle;
	Bifrost::API::String writeToFolderVoxel;
	if ( inData->hotData ) {
		// declare State Server
		Bifrost::API::StateServer hotServer( nodeData->objectRef->stateServer() );

		// write in memory volume data to a temp file
		Bifrost::API::String writeToFile;
		writeToFile = writeHotDataToDisk( hotServer, inData->voxelFilename, "voxel_liquid-volume", inData->diagnostics, writeToFolderParticle );

		// realloc for the new name
		size_t inputLen = writeToFile.length();
		free ( inData->voxelFilename );
		inData->voxelFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
		strcpy( inData->voxelFilename, writeToFile.c_str() );

		// write in memory particle data to a temp file
		writeToFile = writeHotDataToDisk( hotServer, inData->particleFilename, "voxel_liquid-particle", inData->diagnostics, writeToFolderVoxel );

		// realloc for the new name
		inputLen = writeToFile.length();
		free ( inData->particleFilename );
		inData->particleFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
		strcpy( inData->particleFilename, writeToFile.c_str() );
	}

	Bifrost::API::String msg = "[BifrostPolyMesh] Bif file does not exists!";
	Bifrost::API::String correctedPointFilename = Bifrost::API::File::forwardSlashes( inData->particleFilename );
	Bifrost::API::String correctedVoxelFilename = Bifrost::API::File::forwardSlashes( inData->voxelFilename );

	printf("\n---------------------------------------\n");
	printf("START OUTPUT FOR BIFROSTPOLYMESH\n");
	printf("---------------------------------------\n\n");

	// check for existence of files
	int pointFileExists = false;
	int voxelFileExists = false;
    if (FILE *file = fopen(correctedPointFilename.c_str(), "r")) {
        fclose(file);
		pointFileExists = true;
	}
    if (FILE *file = fopen(correctedVoxelFilename.c_str(), "r")) {
        fclose(file);
		voxelFileExists = true;
	}

	// bail out if one of the files does not exist
	if ( ! (pointFileExists && voxelFileExists) ) {
		printf("\n");
		if ( !pointFileExists ) {
			printf ("BifFile: %s does not exists!\n", correctedPointFilename.c_str() );
		}
		if ( !voxelFileExists ) {
			printf ("BifFile: %s does not exists!\n", correctedVoxelFilename.c_str() );
		}

		printf("\tExiting...\n\n");
		printf("\n---------------------------------------\n");
		printf("END OUTPUT FOR BIFROSTPOLYMESH\n");
		printf("---------------------------------------\n\n");
		return false;
	}

 	inData->printParameters();

	//
	//
	// PRELOADING AND INFO GATHERING
	//
	//
	//

	// init FrameData struct that holds information specific to the frame we are rendering
	PolyMeshFrameData *frameData = (PolyMeshFrameData *) new( PolyMeshFrameData );
	frameData->init();
	frameData->hotData = inData->hotData;
	frameData->tmpFolderParticle = writeToFolderParticle;
	frameData->tmpFolderVoxel = writeToFolderVoxel;
	nodeData->frameData = (void *) frameData;
	frameData->pluginType = PLUGIN_MESH;

	// process which channels to load
	//initAndGetPrimitivesFrameData(	frameData, inData, getASSData );

	if ( frameData->error ) {
		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );
		return false;
	}

	//
	//
	// Lazy Loading
	//
	//
	//

	// get information from bif file
	Bifrost::API::ObjectModel OM;
	Bifrost::API::FileIO fio = OM.createFileIO( correctedPointFilename );
	Bifrost::API::BIF::FileInfo fInfo = fio.info();

	// let's get avaliable channel names
	Bifrost::API::StringArray inputChannelNames;

	if ( inData->diagnostics.debug > 0 ) {
		printf("\nAvaliable channels:\n");
	}

	for ( int i = 0; i < fInfo.channelCount; i++ ) {
		inputChannelNames.addUnique( fio.channelInfo(i).name );
		if ( inData->diagnostics.debug > 0 ) {
			printf("\t%s\n", inputChannelNames[i].c_str());
		}
	}

	// now check all the required channels and assemble a loadChannelNames list
	Bifrost::API::String inputPrimVars ( inData->primVarNames );
	frameData->primVarNames = inputPrimVars.split(",");

	// velocity is always needed for polymesh
	size_t channelExists = inputChannelNames.findFirstByName( "velocity" );
	if ( channelExists != inputChannelNames.npos ) {
		frameData->loadChannelNames.addUnique( "velocity" );
	} else {
		printf("Can not find velocity channel for speed calculation. Exiting...\n");
		return false;
	}

	// first lets check whether we have speed specified anywhere
	// if yes check existence of velocity and if it is there add corresponding channels to the loadChannelNames
	if ( frameData->primVarNames.findFirstByName("speed") != inputChannelNames.npos ) {
		// if we get here that means speed is avaliable and needed
		frameData->speedNeeded = true;
	}

	//
	// now process all the input data
	//

	// now process primvars list
	printf("\nProcessing PRIMVARS List...\n");
	// check primVars for matches
	for ( int i = 0; i < frameData->primVarNames.count(); i++) {
		// do not do anything for speed as it will be calculated later
		if ( frameData->primVarNames[i] != "speed" ) { 
			size_t channelExists = inputChannelNames.findFirstByName( frameData->primVarNames[i] );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( frameData->primVarNames[i] );
			} else {
				printf( "\tCan not find primVar %s in file, skipping!\n", frameData->primVarNames[i].c_str() );
				frameData->primVarNames.remove( i );
				i--;
			}
		}
	}

	// now add cache type specific channels to the loadChannelNames
	channelExists = inputChannelNames.findFirstByName( "position" );
	if ( channelExists != inputChannelNames.npos ) {
		frameData->loadChannelNames.addUnique( "position" );
	}
	channelExists = inputChannelNames.findFirstByName( "droplet" );
	if ( channelExists != inputChannelNames.npos ) {
		frameData->loadChannelNames.addUnique( "droplet" );
	}

	// check motion blur is on and if it is export velocity channel(s)
	// get shutter times to see whether we are exporting motion blur or not
	AtNode* camNode = AiUniverseGetCamera();
    frameData->shutter[0] = inData->shutterStart;
	frameData->shutter[1] = inData->shutterEnd;
	bool motionBlur = false;
	bool velocityExists = true;

	// check velocity
	if ( frameData->shutter[1] - frameData->shutter[0] > FLT_EPSILON ) {
		motionBlur = true;

		// we need to export motion blur do we have velocity in bif?
		channelExists = inputChannelNames.findFirstByName( "velocity" );
		if ( channelExists != inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( "velocity" );
		} else {
			velocityExists = false;
		}
	} 

	printf("\nMotionBlur: ");
	if ( motionBlur ) {
		if ( velocityExists ) {
			printf("ON\n\tShutter: %.2f %.2f\n", frameData->shutter[0], frameData->shutter[1]);
			frameData->motionBlur = true;
		} else {
			printf("OFF because no valid velocity channel found!\n");
			frameData->motionBlur = false;
		}	
	} else {
		printf("OFF\n");
		frameData->motionBlur = false;
	}

	//
	// we are done with channel processing so output results
	//

	// print out primVars
	if (frameData->primVarNames.count() > 0 ) {
		printf("\nFinal list of Primvars to Export:\n");
		for (int i = 0; i < frameData->primVarNames.count(); i++) {
			printf("\t%s\n", frameData->primVarNames[i].c_str() );
		}
	} else {
		printf ( "\tNo primvars found to export!\n" );
	}

	// print out loadChannelNames
	if ( inData->diagnostics.debug > 0 ) {
		if (frameData->loadChannelNames.count() > 0 ) {
			printf("\nFinal list of channels to load:\n");
			for (int i = 0; i < frameData->loadChannelNames.count(); i++) {
				printf("\t%s\n", frameData->loadChannelNames[i].c_str() );
			}
		}
	}

	// define a state server and load the channels
	printf ("\nLoading bifrost caches...\n");
	Bifrost::API::StateServer SS = OM.createStateServer();

	printf ("\tLoading %s...\n", correctedPointFilename.data());
	Bifrost::API::Status loadStPoint = fio.load( SS, frameData->loadChannelNames);

	printf ("\tLoading %s...\n", correctedVoxelFilename.data());
	fio = OM.createFileIO( correctedVoxelFilename );
	Bifrost::API::StringArray loadVoxelChannelNames;
	loadVoxelChannelNames.add("velocity_u");
	loadVoxelChannelNames.add("velocity_v");
	loadVoxelChannelNames.add("velocity_w");

	Bifrost::API::Status loadStVoxel = fio.load( SS, loadVoxelChannelNames);

	if ( loadStPoint == Bifrost::API::Status::Success && loadStVoxel == Bifrost::API::Status::Success && SS.valid() ) {
		printf("\tFiles are loaded and valid!\n");

		Bifrost::API::RefArray objects = SS.objects();
		Bifrost::API::Object inObj = objects[0];
		Bifrost::API::PointComponent point = SS.findComponent( "voxel_liquid-particle" );
		Bifrost::API::VoxelComponent voxel = SS.findComponent( "voxel_liquid-volume" );
		Bifrost::API::RefArray pointChannels = point.channels();
		Bifrost::API::RefArray voxelChannels = voxel.channels();

		// set paramaters that are stored in bif dictionary
		// read the bif dictionary and get dropletThreshold and adaptive flag value
		Bifrost::API::Dictionary dict = inObj.dictionary();
		Bifrost::API::StringArray keys = dict.keys();
		inData->dropletThreshold = dict.value<float>("dropletThreshold");
		inData->isAdaptive = dict.value<bool>("enableSpatialAdapt");
		printf("\nADAPTIVE SIM: ");
		inData->isAdaptive ? printf("ON\n") : printf("OFF\n");
		printf("dropletThreshold: %f\n\n", inData->dropletThreshold);

		if ( inData->diagnostics.debug > 0 ) {
			printf("\tLoaded channels:\n");
			for ( size_t i=0; i<pointChannels.count(); i++ ) {
				std::cerr << "\t\tPointChannel: " << Bifrost::API::Base(pointChannels[i]).name() << std::endl;
			}
			printf("\n");
			for ( size_t i=0; i<voxelChannels.count(); i++ ) {
				std::cerr << "\t\tVoxelChannel: " << Bifrost::API::Base(voxelChannels[i]).name() << std::endl;
			}
			printf("\n");
		}

		if ( inData->diagnostics.debug > 0 ) {
			Bifrost::API::String ssDump = SS.dump();
			printf("\n");
			printf("%s", ssDump.c_str() );
			printf("\n");
		}

		Bifrost::API::PointChannel position = point.findChannel( "position" );
		Bifrost::API::PointChannel velocity = point.findChannel( "velocity" );
		Bifrost::API::PointChannel droplet = point.findChannel( "droplet" );
		Bifrost::API::VoxelChannel voxelVelocityU = voxel.findChannel( "velocity_u" );
		Bifrost::API::VoxelChannel voxelVelocityV = voxel.findChannel( "velocity_v" );
		Bifrost::API::VoxelChannel voxelVelocityW = voxel.findChannel( "velocity_w" );


		// get and store primvars to export
		initPVarInfo<PolyMeshFrameData>( frameData );

		// primvars to transfer: this is for the mesher call
		Bifrost::API::RefArray primVarChannels;

		printf("\nProcessing PRIMVARS List...\n");
		// check primVar vector for matches
		for ( unsigned i = 0; i < frameData->primVars.size(); i++) {
			// if speed is specified as a primvar, create a channel for it.
			if (frameData->primVars[i].name == "speed" ) {
				createSpeedChannel(SS);
			}

			Bifrost::API::PointChannel primVarChannel = point.findChannel( frameData->primVars[i].name.c_str() );
			if ( primVarChannel.valid() ) {
				frameData->primVars[i].channel = primVarChannel;
				primVarChannels.add( primVarChannel );
			} else {
				printf("\tCan not find primVar %s in file, skipping!\n", frameData->primVars[i].name.c_str() );
				frameData->primVars.erase( frameData->primVars.begin() + i);
				i = i - 1;
			}
		}

		// print out primVars
		if (frameData->primVars.size() > 0 ) {
			printf("\n\tFinal list of Primvars to Export:\n");
			for (unsigned i = 0; i < frameData->primVars.size(); i++) {
				printf("\t\t%s\n", frameData->primVars[i].name.c_str() );
			}
		} else {
			printf ( "\tNo primvars found to export!\n" );
		}

		// now that we know which channels we would use
		// clean up the in-memory bif
		//printf("\nCleaning up unnecessary bifrost channels from memory:\n");
		//for ( unsigned i = 0; i<pointChannels.count(); i++ ) {
		//	bool match = false;

		//	// check primvars
		//	for (size_t var = 0; var < primVars.size(); var++) {
		//		if ( Bifrost::API::Base(primVars[var].channel) == Bifrost::API::Base(pointChannels[i]) ) {
		//			match = true;
		//		}
		//	}

		//	// also check default channels
		//	if (	( pointChannels[i] != Bifrost::API::Base(position) ) &&
		//			( pointChannels[i] != Bifrost::API::Base(velocity) ) &&
		//			( pointChannels[i] != Bifrost::API::Base(droplet) ) &&
		//			( match != true ) ) {
		//			// remove channel from state server		
		//			printf("\t%s\n", Bifrost::API::Channel(pointChannels[i]).name().c_str() );
		//			SS.removeChannel ( Bifrost::API::Channel(pointChannels[i]).fullPathName() );
		//	}
		//}

		Bifrost::API::Mesh::Options options;
		options.dropletRevealFactor = inData->dropletRevealFactor;
		options.surfaceRadius = inData->surfaceRadius;
		options.dropletRadius = inData->dropletRadius;
		options.dropletThreshold = inData->dropletThreshold;
		options.kernelFactor = inData->kernelFactor;
		options.resolutionFactor = inData->resolutionFactor;
		options.smoothing = inData->smoothing;
		options.minimumFeatureAngle = inData->minimumFeatureAngle;
		options.isAdaptive = inData->isAdaptive;
		options.bbox = amino::Math::bboxf (	-10000.0f, -10000.0f, -10000.0f,
						10000.0f, 10000.0f, 10000.0f );

		printf("\nCreating the mesh...\n");

		// now create the mesh
		Bifrost::API::VoxelChannel voxelSolidDistance;

		Bifrost::API::Mesh meshData = Bifrost::API::Mesh::build(	options,
																	position, velocity,
																	voxelVelocityU, voxelVelocityV, voxelVelocityW,
																	droplet, voxelSolidDistance, primVarChannels );
		printf("\tDone\n");

		// emit primitives
		amino::Math::vec3f wp, vel;
		printf("\nStarting subdivision\n");

		// access to topology data
		size_t vertexCount = 0;
		const amino::Math::vec3f* vertexPos = meshData.vertices(vertexCount);

		size_t velocityCount = 0;
		const amino::Math::vec3f* vertexVel = meshData.velocities(velocityCount);

		size_t triangleCount = 0;
		const amino::Math::vec3i* triangles = meshData.triangles(triangleCount);

		if ( triangleCount == 0 ) {
			printf("\tNo triangles are received from the mesher!\n");
			printf("\tExiting...\n\n");

			printf("\n---------------------------------------\n");
			printf("END OUTPUT FOR BIFROSTPOLYMESH\n");
			printf("---------------------------------------\n\n");
			return false;
		} else {
			printf("\t%d triangles and %d vertices are received from the mesher!\n", triangleCount, vertexCount);
		}

		// init arrays that hold poly data
		CvInt *vertices = NULL;
		CvFloat *pos = NULL;
		CvFloat *motionPos = NULL;

		CvInt polyCount = (CvInt) triangleCount;

		vertices = (CvInt *) malloc ( 3 * sizeof(CvInt) * polyCount );
		frameData->mem.push_back( (void *) vertices );
		pos = (CvFloat *) malloc ( 3 * vertexCount * sizeof(CvFloat));
		frameData->mem.push_back( (void *) pos );
		if ( motionBlur ) {
			motionPos = (CvFloat *) malloc ( 3 * vertexCount * sizeof(CvFloat));
			frameData->mem.push_back( (void *) motionPos );
		}

		//define tokens and ptrs for export
		std::vector<CvToken> toks;
		std::vector<CvPointer> ptr;
		std::vector<int> varIndex;

		if ( frameData->primVars.size() > 0 ) {
			declarePrimVars( frameData->primVars, toks, ptr, varIndex, (int) vertexCount, frameData->mem, TOKCLASS_VARYING, getPrimVarTokenAI );

			// assign primvar data
			for ( unsigned var = 0; var < frameData->primVars.size(); var++ ) {
				size_t count = 0;
				Bifrost::API::PointChannel primVarChannel = frameData->primVars[var].channel;
				// get data according to type
				switch ( primVarChannel.dataType() ) {
					case Bifrost::API::FloatType:
						frameData->primVars[var].inDataArray = (void *) meshData.channel1f( Bifrost::API::String( frameData->primVars[var].name.c_str() ), count);
						break;
					case Bifrost::API::FloatV2Type:
						frameData->primVars[var].inDataArray = (void *) meshData.channel2f( Bifrost::API::String( frameData->primVars[var].name.c_str() ), count);
						break;
					case Bifrost::API::FloatV3Type:
						frameData->primVars[var].inDataArray = (void *) meshData.channel3f( Bifrost::API::String( frameData->primVars[var].name.c_str() ), count);
						break;
					default:
						assert(0); // type not supported
						break;
				}
			}
		}

		printf("\tPreparing triangles...\n");

		for (int i = 0; i < polyCount; i++) {
			int index = 3* i;
			// fill the vertices array
			vertices[ index ] = triangles[ i ][ 0 ];
			vertices[ index + 1 ] = triangles[ i ][ 1 ];
			vertices[ index + 2 ] = triangles[ i ][ 2 ];
		}

		printf("\tPreparing positions...\n");
		for (int i = 0; i < vertexCount; i++) {
			if ( motionBlur ) {
				// output positions taking shutter times into account
				pos[3 * i]		= vertexPos[i][0] + inData->velocityScale * vertexVel[i][0] * frameData->shutter[0];
				pos[3 * i + 1]	= vertexPos[i][1] + inData->velocityScale * vertexVel[i][1] * frameData->shutter[0];
				pos[3 * i + 2]	= vertexPos[i][2] + inData->velocityScale * vertexVel[i][2] * frameData->shutter[0];

				motionPos[3 * i]		= vertexPos[i][0] + inData->velocityScale * vertexVel[i][0] * frameData->shutter[1];
				motionPos[3 * i + 1]	= vertexPos[i][1] + inData->velocityScale * vertexVel[i][1] * frameData->shutter[1];
				motionPos[3 * i + 2]	= vertexPos[i][2] + inData->velocityScale * vertexVel[i][2] * frameData->shutter[1];
			} else {
				// just output position
				pos[3 * i] = vertexPos[i][0];
				pos[3 * i + 1] = vertexPos[i][1];
				pos[3 * i + 2] = vertexPos[i][2];
			}
		}

		// fill primvar arrays
		if ( frameData->primVars.size() > 0 ) {
			for ( unsigned var = 0; var < frameData->primVars.size(); var++ ) {
				printf("\tPreparing PrimVar: %s...\n", frameData->primVars[var].name.c_str());
				Bifrost::API::PointChannel primVarChannel = frameData->primVars[var].channel;
				for (int i = 0; i < vertexCount; i++) {
					switch ( primVarChannel.dataType() ) {
						case Bifrost::API::FloatType:
							frameData->primVars[var].primDataArray[i] = ( (float *) frameData->primVars[var].inDataArray )[i];
							frameData->primVars[var].min = std::min<float>( frameData->primVars[var].min,  frameData->primVars[var].primDataArray[i] );
							frameData->primVars[var].max = std::max<float>( frameData->primVars[var].max,  frameData->primVars[var].primDataArray[i] );
							break;
						case Bifrost::API::FloatV2Type:
							frameData->primVars[var].primDataArray[2 * i] = ( (amino::Math::vec2f *) frameData->primVars[var].inDataArray )[i][0];
							frameData->primVars[var].primDataArray[2 * i + 1] = ( (amino::Math::vec2f *) frameData->primVars[var].inDataArray )[i][1];
							break;
						case Bifrost::API::FloatV3Type:
							frameData->primVars[var].primDataArray[3 * i] = ( (amino::Math::vec3f *) frameData->primVars[var].inDataArray )[i][0];
							frameData->primVars[var].primDataArray[3 * i + 1] = ( (amino::Math::vec3f *) frameData->primVars[var].inDataArray )[i][1];
							frameData->primVars[var].primDataArray[3 * i + 2] = ( (amino::Math::vec3f *) frameData->primVars[var].inDataArray )[i][2];
							break;
						default:
							// type not supported
							//assert(0);
							break;
					}
				}

				printf( "\t\tDone...\n" );
			}
		}

		// export polygons
		printf("\tExporting Polygons...\n");

		AtNode *newNode = AiNode( "polymesh" );
		nodeData->createdNodes.push_back( newNode );
		nodeData->nofNodesCreated++;

		std::string nodeName ( "BifrostPolyMeshExport1" );
		AiNodeSetStr( newNode, "name", nodeName.c_str() );
		AiNodeSetBool( newNode, "opaque", false );

		// put spaceScale into action
		if ( inData->spaceScale != 1.0f ) {
			AtVector scaleVec = AiVector( inData->spaceScale, inData->spaceScale, inData->spaceScale );
			AtMatrix scaleM4;
			AiM4Scaling( scaleM4, &scaleVec );
			AiNodeSetMatrix ( newNode, "matrix", scaleM4 );
		}

		// create array to hold poly data
		AtArray *posArray = AiArrayAllocate( (AtUInt32) vertexCount, motionBlur ? 2 : 1, AI_TYPE_POINT );
		AtArray *nSidesArray = AiArrayAllocate( polyCount, 1, AI_TYPE_UINT );
		AtArray *vertexArray = AiArrayAllocate( polyCount * 3, 1, AI_TYPE_UINT );

		// get vertex data and convert to Arnold - BAD
		for ( int i = 0; i < vertexCount; i++ ) {
			// set position data
			int index = i * 3;
			AtPoint tmpPoint = AiPoint ( pos[ index ], pos[ index + 1 ], pos[ index + 2 ] );
			AiArraySetPnt( posArray, i, tmpPoint );

			if ( motionBlur ) {
				tmpPoint = AiPoint ( motionPos[ index ], motionPos[ index + 1 ], motionPos[ index + 2 ] );
				AiArraySetPnt( posArray, (AtUInt32) (i + vertexCount), tmpPoint );
			}
		}

		// get triangle data
		for ( int i = 0; i < polyCount; i++ ) {
			// set position data
			int index = i * 3;
			AiArraySetUInt( nSidesArray, i, 3 );
			AiArraySetUInt( vertexArray, index, vertices[ index ] );
			AiArraySetUInt( vertexArray, index + 1, vertices[ index + 1 ] );
			AiArraySetUInt( vertexArray, index + 2, vertices[ index + 2 ] );
		}

		AiNodeSetArray( newNode, "vlist", posArray );
		AiNodeSetArray( newNode, "nsides", nSidesArray );
		AiNodeSetArray( newNode, "vidxs", vertexArray );

		// export primvars too
		int exported = 0;
		for ( int var = 0 ; var < frameData->primVars.size(); var++ ) {
			if ( frameData->primVars[ var ].exportToRIB ) {
				int currentArrayIndex = frameData->primVars[ var ].exportArraysIndex;

				// declare primvar
				Bifrost::API::String exportType = toks[ currentArrayIndex ];
				exportType.trimLeft ( frameData->primVars[ var ].name.c_str() );
				AiNodeDeclare( newNode, frameData->primVars[ var ].name.c_str(), exportType.c_str() );

				// get the data and poupulate arnold array
				float *primVarData = (float *)ptr[ currentArrayIndex ];
				AtArray *primVarArray = AiArrayAllocate( (AtUInt32) vertexCount, 1, AI_TYPE_FLOAT );

				for ( int i = 0; i < vertexCount; i++ ) {
					AiArraySetFlt(primVarArray, i, primVarData[ i ]);
				}

				AiNodeSetArray( newNode, frameData->primVars[ var ].name.c_str(), primVarArray );
			}
		}

		printf("\t\tFinished exporting\n\n");

		// report min max for float types
		int nofFloatPrimVars = 0;
		for ( int i = 0; i < frameData->primVars.size(); i++ ) {
			if ( frameData->primVars[i].channel.dataType() == Bifrost::API::FloatType ||  frameData->primVars[i].name == "speed" ) {
				if (nofFloatPrimVars == 0 ) {
					printf("\nMin-Max ranges for float primvars:\n");
				}

				printf( "\t%s:\n", frameData->primVars[i].name.c_str() );
				printf( "\t\tMin: %f\n", frameData->primVars[i].min );
				printf( "\t\tMax: %f\n", frameData->primVars[i].max );
				nofFloatPrimVars = 1;
			}
		}
	}  else {
		printf("\tThere is a problem loading files! Please check!\n");
	} // end ss.valid

	// free bif memory
	if ( SS.valid() ) {
		SS.clear();
	}

	// free the assembled data arrays
	for (unsigned i = 0; i < frameData->mem.size(); i++) {
		free ( frameData->mem[i] );
	}

	return true;
	printf("\n---------------------------------------\n");
	printf("END OUTPUT FOR BIFROSTPOLYMESH\n");
	printf("---------------------------------------\n\n");
}

// we read the UI parameters into their global vars
static int ProcInit( AtNode *myNode, void **user_ptr )
{
	// create nodeData
	AIProcNodeData *nodeData = new AIProcNodeData();
	nodeData->objectRef = 0;

	std::string objectName = AiNodeLookUpUserParameter(myNode, "objectName") ? AiNodeGetStr(myNode, "objectName") : "";
	std::string particleFilename = AiNodeLookUpUserParameter(myNode, "particleFilename") ? AiNodeGetStr(myNode, "particleFilename") : "";
	if (true || objectName != nodeData->objectName || particleFilename != nodeData->file) // in case we change the frame ?
	{
		// need to update objet_ref
		if (nodeData->objectRef) {
			delete nodeData->objectRef;
		}

		nodeData->objectRef = new CoreObjectUserData(objectName, particleFilename);
		nodeData->objectName = objectName;
		nodeData->file = particleFilename;
	}

	// create Input Data
	PolyMeshInputData *inData = (PolyMeshInputData *) malloc( sizeof( PolyMeshInputData ) );
	float debug = 0;

	// store values
	nodeData->proceduralNode = myNode;
	AiNodeGetMatrix ( myNode, "matrix", nodeData->world2Obj );
	AiM4Invert ( nodeData->world2Obj, nodeData->obj2World );

	nodeData->camNode = AiUniverseGetCamera();
	AiWorldToCameraMatrix( nodeData->camNode, 0.0f, nodeData->world2Cam );
	AiM4Mult( nodeData->obj2Cam, nodeData->obj2World, nodeData->world2Cam );

	nodeData->inData = (void *) inData;
	nodeData->bifrostCtx = AiShaderGlobals();
	nodeData->samplerPool.clear();
	nodeData->nofNodesCreated = 0;

	// make a copy of the nodeData
	*user_ptr = (void *) nodeData;

	printf( "\n" );
	inData->error = false;

	inData->velocityScale = AiNodeGetFlt(myNode, "velocityScale");
	inData->fps = AiNodeGetFlt(myNode, "fps");
	inData->spaceScale = AiNodeGetFlt(myNode, "spaceScale");

	//inData->clip.on = AiNodeGetBool(myNode, "clipOn");
	//inData->clip.minX = AiNodeGetFlt(myNode, "clipMinX");
	//inData->clip.maxX = AiNodeGetFlt(myNode, "clipMaxX");
	//inData->clip.minY = AiNodeGetFlt(myNode, "clipMinY");
	//inData->clip.maxY = AiNodeGetFlt(myNode, "clipMaxY");
	//inData->clip.minZ = AiNodeGetFlt(myNode, "clipMinZ");
	//inData->clip.maxZ = AiNodeGetFlt(myNode, "clipMaxZ");

	inData->dropletRevealFactor = AiNodeGetFlt(myNode, "dropletRevealFactor");
	inData->surfaceRadius = AiNodeGetFlt(myNode, "surfaceRadius");
	inData->dropletRadius = AiNodeGetFlt(myNode, "dropletRadius");
	inData->kernelFactor = AiNodeGetFlt(myNode, "kernelFactor");
	inData->resolutionFactor = AiNodeGetFlt(myNode, "resolutionFactor");
	inData->smoothing = AiNodeGetInt(myNode, "smoothing");
	inData->minimumFeatureAngle = AiNodeGetFlt(myNode, "minimumFeatureAngle");

	inData->diagnostics.debug = AiNodeGetInt(myNode, "debug");
	inData->diagnostics.silent = 0;

	inData->hotData = AiNodeGetBool(myNode, "hotData");

	const AtString primVarNamesParam("primVarNames");
	const AtString primVarNames = AiNodeGetStr(myNode, primVarNamesParam );
	size_t inputLen = primVarNames.length();
	inData->primVarNames = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->primVarNames, primVarNames.c_str() );

	const AtString particleFileParam("particleFilename");
	const AtString particleFile = AiNodeGetStr(myNode, particleFileParam );
	inputLen = particleFile.length();
	inData->particleFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->particleFilename, particleFile.c_str() );

	const AtString voxelFileParam("voxelFilename");
	const AtString voxelFile = AiNodeGetStr(myNode, voxelFileParam );
	inputLen = voxelFile.length();
	inData->voxelFilename = (char *) malloc ( ( inputLen + 1 ) * sizeof( char ) );
	strcpy( inData->voxelFilename, voxelFile.c_str() );

	// arnold specific parameters
	inData->motionBlur = AiNodeGetBool( myNode, "motionBlur" );
	inData->shutterStart = AiNodeGetFlt( myNode, "shutterStart" );
	inData->shutterEnd = AiNodeGetFlt( myNode, "shutterEnd" );

	// if there is something fishy error out
	//if ( inData->clip.on ) {
	//	if ( inData->clip.maxX <= inData->clip.minX || inData->clip.maxY <= inData->clip.minY || inData->clip.maxZ <= inData->clip.minZ ) {
	//		printf("[BIFROST PRIMITIVES] ClipBox coordinates are wrong: one or more of MaxXYZ is smaller than MinXYZ\n");
	//		inData->error = true;
	//	}
	//}

	if ( inData->error ) {
		return false;
	} else {
		// now do creation of nodes
		return ProcSubdivide( nodeData, inData );
	}
}

// we will create one node
static int ProcNumNodes( void *user_ptr )
{
	return 1;
}

// this is the function that Arnold calls to request the nodes
// that this procedural creates.
static AtNode *ProcGetNode(void *user_ptr, int i)
{
	AIProcNodeData *nodeData = (AIProcNodeData *) user_ptr;

	if ( i >= 0 && i < (int) nodeData->createdNodes.size() )
	{
		const char* nodeName = AiNodeGetName( nodeData->createdNodes[i] );

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
		PolyMeshInputData *inData = (PolyMeshInputData *)  nodeData->inData;
		PolyMeshFrameData *frameData = (PolyMeshFrameData *) nodeData->frameData;

		if ( frameData ) {
			if ( inData->hotData ) {
				Bifrost::API::File::deleteFolder( frameData->tmpFolderParticle );
				Bifrost::API::File::deleteFolder( frameData->tmpFolderVoxel );
			}
		}

		if ( nodeData->inData ) {
			free( inData->primVarNames );
			free( inData->particleFilename );
			free( inData->voxelFilename );
			free( inData );
		}

		if ( nodeData->objectRef ) {
			delete nodeData->objectRef;
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
