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


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

#include <bifrostrendercore/bifrostrender_primitivestools.h>

namespace Bifrost{
namespace RenderCore{

float calcPointDistance(CvPoint start, CvPoint end)
{
	float distance = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float temp = 0;

	x = start[0] - end[0];
	y = start[1] - end[1];
	z = start[2] - end[2];
	temp = x * x + y * y + z * z;

	if (temp > 0.0f) {
		distance = sqrt (temp);
	}

	return distance;
}

float calcCamDistance(CvPoint location)
{
	float distance = 0;
	float temp = 0;

	temp = location[0] * location[0] + location[1] * location[1] + location[2] * location[2];

	if (temp > 0.0f) {
		distance = sqrt (temp);
	}

	return distance;
}

float calcCamMulFactor( PrimitivesInputData *inData, float distance)
{
	// if exponent is zero always return 0
	if (inData->camRadiusFactorExponent == 0.0f) {
		return 1.0f;
	}

	if ( distance < inData->camRadiusStartDistance ) {
		return inData->camRadiusStartFactor;
	}

	if ( distance > inData->camRadiusEndDistance ) {
		return inData->camRadiusEndFactor;
	}

	float ratio = (distance - inData->camRadiusStartDistance) / inData->camRadiusDistanceRange;
	if (inData->camRadiusFactorExponent != 1.0f) {
		ratio = powf(ratio, inData->camRadiusFactorExponent);
	}

	return inData->camRadiusStartFactor + ratio * inData->camRadiusFactorRange;
}

bool checkSpeed(	PrimitivesInputData *inData,
					PrimitivesFrameData *frameData,
					Bifrost::API::BIF::FileInfo& fInfo )
{
	size_t channelExists;

	// first lets check whether we have speed specified anywhere
	// if yes check existence of velocity and if it is there add corresponding channels to the loadChannelNames
	if (	inData->inputChannelName == "speed" ||
			frameData->primVarNames.findFirstByName("speed") != frameData->inputChannelNames.npos ) {
		
		// we need speed channel
		// check existence of velocity channels

		// check what type of component we are processing
		if ( fInfo.componentType == Bifrost::API::VoxelComponentType ) {
			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_u" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_u" );
			} else {
				printf("Can not find velocity_u channel for speed calculation. Exiting...\n");
				return false;
			}

			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_v" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_v" );
			} else {
				printf("Can not find velocity_v channel for speed calculation. Exiting...\n");
				return false;
			}

			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_w" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_w" );
			} else {
				printf("Can not find velocity_w channel for speed calculation. Exiting...\n");
				return false;
			}
		} else if ( fInfo.componentType == Bifrost::API::PointComponentType ) {
			channelExists = frameData->inputChannelNames.findFirstByName( "velocity" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity" );
			} else {
				printf("Can not find velocity channel for speed calculation. Exiting...\n");
				return false;
			}
		} else {
			printf("Can not determine cache type for speed channel creation! Please check your bif file\n");
			return false;
		}
	}

	return true;
}

void reportPrimVars( PrimitivesFrameData *frameData )
{
	int nofFloatPrimVars = 0;
	for ( int i = 0; i < frameData->primVars.size(); i++ ) {
		if (	frameData->primVars[i].exportToRIB &&
				( frameData->primVars[i].channel.dataType() == Bifrost::API::FloatType ||  frameData->primVars[i].name == "speed" ) ) {
			// print a message for the first item
			if (nofFloatPrimVars == 0 ) {
				printf("\nMin-Max ranges for float primvars:\n");
			}

			printf( "\t%s:\n", frameData->primVars[i].name.c_str() );
			printf( "\t\tMin: %f\n", frameData->primVars[i].min );
			printf( "\t\tMax: %f\n", frameData->primVars[i].max );
			nofFloatPrimVars = 1;
		}
	}
}

void initPrimitivesPVarInfo( PrimitivesFrameData *frameData, Bifrost::API::Component component )
{
	for ( int i = 0; i < frameData->primVarNames.count(); i++ ) {
		if ( frameData->primVarNames[i] == "speed" ) {
			// do special processing for speed
			// this is for not allocating another channel in bif structure for speed
			// this would yield better memory consumption
			if ( component.type() == Bifrost::API::VoxelComponentType ) {
				// add all three velocity channels to primvars list with export set to false
				primVarInfo pVarInfo;
				pVarInfo.min = std::numeric_limits<float>::max();
				pVarInfo.max = -std::numeric_limits<float>::max();
				pVarInfo.name = std::string( "velocity_u" );
				pVarInfo.exportToRIB = false;
				frameData->primVars.push_back( pVarInfo );
				frameData->velocityIndexU = int( frameData->primVars.size() - 1 );

				pVarInfo.min = std::numeric_limits<float>::max();
				pVarInfo.max = -std::numeric_limits<float>::max();
				pVarInfo.name = std::string( "velocity_v" );
				pVarInfo.exportToRIB = false;
				frameData->primVars.push_back( pVarInfo );
				frameData->velocityIndexV = int( frameData->primVars.size() - 1);

				pVarInfo.min = std::numeric_limits<float>::max();
				pVarInfo.max = -std::numeric_limits<float>::max();
				pVarInfo.name = std::string( "velocity_w" );
				pVarInfo.exportToRIB = false;
				frameData->primVars.push_back( pVarInfo );
				frameData->velocityIndexW = int( frameData->primVars.size() - 1);
			} else {
				// this is a point cache so store only velocity
				primVarInfo pVarInfo;
				pVarInfo.min = 0;
				pVarInfo.max = 0;
				pVarInfo.name = std::string( "velocity" );
				pVarInfo.exportToRIB = false;
				frameData->primVars.push_back( pVarInfo );
				frameData->velocityIndex = int( frameData->primVars.size() - 1);
			}

			// now add speed to the list as well
			primVarInfo pVarInfo;
			pVarInfo.min = std::numeric_limits<float>::max();
			pVarInfo.max = -std::numeric_limits<float>::max();
			pVarInfo.name = std::string( frameData->primVarNames[i].c_str() );
			pVarInfo.exportToRIB = true;
			frameData->primVars.push_back( pVarInfo );
		} else {
			primVarInfo pVarInfo;
			pVarInfo.min = std::numeric_limits<float>::max();
			pVarInfo.max = -std::numeric_limits<float>::max();
			pVarInfo.name = std::string( frameData->primVarNames[i].c_str() );
			pVarInfo.exportToRIB = true;
			frameData->primVars.push_back( pVarInfo );
		}
	}
}

void checkVelocity( PrimitivesFrameData *frameData,
					Bifrost::API::BIF::FileInfo& fInfo )
{
	size_t channelExists;

	if ( frameData->shutter[1] - frameData->shutter[0] > FLT_EPSILON ) {
		frameData->motionBlur = true;

		// we need to export motion blur do we have velocity in bif?
		if ( fInfo.componentType == Bifrost::API::VoxelComponentType ) {
			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_u" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_u" );
			} else {
				frameData->velocityExists = false;
			}

			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_v" );
			if ( frameData->velocityExists && channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_v" );
			} else {
				frameData->velocityExists = false;
			}

			channelExists = frameData->inputChannelNames.findFirstByName( "velocity_w" );
			if ( frameData->velocityExists && channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_w" );
			} else {
				frameData->velocityExists = false;
			}
		} else {
			// this is a particle cache
			channelExists = frameData->inputChannelNames.findFirstByName( "velocity" );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity" );
			} else {
				frameData->velocityExists = false;
			}
		}
	} 
}

void checkInputChannel( const Bifrost::API::Channel channel, float& minVal, float& maxVal )
{
	Bifrost::API::Layout layout = channel.layout();

	ChannelValueRangeVisitor valueRangeVisitor ( channel );
	layout.traverse(valueRangeVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );
	minVal = valueRangeVisitor.m_minVal;
	maxVal = valueRangeVisitor.m_maxVal;
	std::cerr << std::endl << channel.name().c_str() << " Range Min: " << valueRangeVisitor.m_minVal << " Max: " << valueRangeVisitor.m_maxVal << std::endl;
}

void reportIdRange( const Bifrost::API::Channel& idChan )
{
	Bifrost::API::Layout layout = idChan.layout();

	if ( idChan.valid() ) {
		UInt64ValueRangeVisitor valueRangeVisitor ( idChan );
		layout.traverse(valueRangeVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );
		std::cerr << "\tId Range Min: " << valueRangeVisitor.m_minVal << " Max: " << valueRangeVisitor.m_maxVal << std::endl;
	}
}

void checkPrimitivesPrimVars( PrimitivesFrameData *frameData, Bifrost::API::Component component )
{
	// check primVars for matches
	for ( unsigned i = 0; i < frameData->primVars.size(); i++) {
		Bifrost::API::Channel primVarChannel = component.findChannel( frameData->primVars[i].name.c_str() );
		if ( primVarChannel.valid() ) {
			frameData->primVars[i].channel = primVarChannel;
		} else if (frameData->primVars[i].name == "speed" ) {
			// do not assign anything to the channel 
			// velocity channels are added to the primvars list with export turned off
			// we will act according to the type of component later
			//primVars[i].channel = velChan;
		} else {
			if ( frameData->primVars[i].name != std::string( "none") ) {
				printf("\tCan not find primVar %s in file, skipping!\n", frameData->primVars[i].name.c_str() );
			}
			frameData->primVars.erase( frameData->primVars.begin() + i);
			i = i - 1;
		}
	}
}

void reportPrimitivesBounds( Bifrost::API::Channel channel, float voxelScale, DiagnosticsParams diagnostics )
{
	BBoxData bboxSim;

    // Find the maximum depth that contains the finest tiles
    Bifrost::API::Layout layout = channel.layout();

    // Visit the finest tiles to compute a bounding box that covers all of them
	bboxSim.tile.bound[0] = bboxSim.tile.bound[2] = bboxSim.tile.bound[4] = std::numeric_limits<float>::max();
	bboxSim.tile.bound[1] = bboxSim.tile.bound[3] = bboxSim.tile.bound[5] = -std::numeric_limits<float>::max();
	bboxSim.world.bound[0] = bboxSim.world.bound[2] = bboxSim.world.bound[4] = std::numeric_limits<float>::max();
	bboxSim.world.bound[1] = bboxSim.world.bound[3] = bboxSim.world.bound[5] = -std::numeric_limits<float>::max();

	{
		TileBoundsVisitor visitor;
		layout.traverse( visitor, Bifrost::API::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );

		bboxSim.tile.bound[0] = (float)visitor.c_tileBoundsMin[0];
		bboxSim.tile.bound[2] = (float)visitor.c_tileBoundsMin[1];
		bboxSim.tile.bound[4] = (float)visitor.c_tileBoundsMin[2];
		bboxSim.tile.bound[1] = (float)visitor.c_tileBoundsMax[0];
		bboxSim.tile.bound[3] = (float)visitor.c_tileBoundsMax[1];
		bboxSim.tile.bound[5] = (float)visitor.c_tileBoundsMax[2];

		// Translate to world space size.
		bboxSim.world.bound[0] = (float)visitor.c_tileBoundsMin[0] * voxelScale;
		bboxSim.world.bound[2] = (float)visitor.c_tileBoundsMin[1] * voxelScale;
		bboxSim.world.bound[4] = (float)visitor.c_tileBoundsMin[2] * voxelScale;
		bboxSim.world.bound[1] = (float)visitor.c_tileBoundsMax[0] * voxelScale;
		bboxSim.world.bound[3] = (float)visitor.c_tileBoundsMax[1] * voxelScale;
		bboxSim.world.bound[5] = (float)visitor.c_tileBoundsMax[2] * voxelScale;
	}

	// calc various bbox reps
	bboxSim.tile.calcFromBounds();
	bboxSim.world.calcFromBounds();

	if ( diagnostics.DEBUG > 0 ) {
		printf("BBox Sim World: \t%.2f %.2f %.2f   %.2f %.2f %.2f\n",	bboxSim.world.min[0], bboxSim.world.min[1], bboxSim.world.min[2],
																		bboxSim.world.max[0], bboxSim.world.max[1], bboxSim.world.max[2]);
		printf("BBox Sim World Dim: \t%.2f %.2f %.2f\n", bboxSim.world.dim[0], bboxSim.world.dim[1], bboxSim.world.dim[2]);
		printf("\n");

		printf("BBox Sim Tile: \t\t%.2f %.2f %.2f   %.2f %.2f %.2f\n",	bboxSim.tile.min[0], bboxSim.tile.min[1], bboxSim.tile.min[2],
																		bboxSim.tile.max[0], bboxSim.tile.max[1], bboxSim.tile.max[2]);
		printf("BBox Sim Tile Dim: \t%.2f %.2f %.2f\n\n", bboxSim.tile.dim[0], bboxSim.tile.dim[1], bboxSim.tile.dim[2]);
		printf("\n");
	} else {
		printf("BBox:\n\tMinX: %.2f\tMaxX: %.2f\n\tMinY: %.2f\tMaxY: %.2f\n\tMinZ: %.2f\tMaxZ: %.2f\n", bboxSim.world.min[0], bboxSim.world.max[0],
																										bboxSim.world.min[1], bboxSim.world.max[1],
																										bboxSim.world.min[2], bboxSim.world.max[2]);
	}
}

//
//
// PRIMVAR STUFF
//
//

void assignPrimitivePrimVars( std::vector<primVarInfo>& primVars, int xExport, int dataIndex, int samples, bool isPointCache, PrimitivesInputData *inData, PrimitivesFrameData *frameData )
{
	int k = 0;

	for ( unsigned var = 0; var < primVars.size(); var++ ) {
		if ( primVars[var].exportToRIB ) {
			// first handle special cases
			if ( primVars[var].name == "speed" ) {
				// first get velocity vector
				CvVector velocity;
				if ( isPointCache ) {
					velocity[0] = ( (amino::Math::vec3f *) primVars[frameData->velocityIndex].inDataArray )[dataIndex][0];
					velocity[1] = ( (amino::Math::vec3f *) primVars[frameData->velocityIndex].inDataArray )[dataIndex][1];
					velocity[2] = ( (amino::Math::vec3f *) primVars[frameData->velocityIndex].inDataArray )[dataIndex][2];
				} else {
					velocity[0] = ( (float *) primVars[frameData->velocityIndexU].inDataArray )[dataIndex];
					velocity[1] = ( (float *) primVars[frameData->velocityIndexV].inDataArray )[dataIndex];
					velocity[2] = ( (float *) primVars[frameData->velocityIndexW].inDataArray )[dataIndex];
				}

				// calc speed
				float speed = sqrt ( velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2] );

				// now set the channel
				// we need to multiply with spaceScale here as velocity is scale dependent
				k = xExport;
				for (int sample = 0; sample < samples; sample++) {
					primVars[var].primDataArray[k++] = speed * inData->spaceScale;
				}

				// set min max for the channel
				primVars[var].min = std::min<float>( primVars[var].min,  speed );
				primVars[var].max = std::max<float>( primVars[var].max,  speed );
			} else {
				// get data according to type
				switch ( primVars[var].channel.dataType() ) {
					case Bifrost::API::FloatType:
						k = xExport;
						for (int sample = 0; sample < samples; sample++) {
							primVars[var].primDataArray[k++] = ( (float *) primVars[var].inDataArray )[dataIndex];
						}
						primVars[var].min = std::min<float>( primVars[var].min,  ( (float *) primVars[var].inDataArray )[dataIndex] );
						primVars[var].max = std::max<float>( primVars[var].max,  ( (float *) primVars[var].inDataArray )[dataIndex] );
						break;
					case Bifrost::API::FloatV2Type:
						k = 2 * xExport;
						for (int sample = 0; sample < samples; sample++) {
							primVars[var].primDataArray[k++] = ( (amino::Math::vec2f *) primVars[var].inDataArray )[dataIndex][0];
							primVars[var].primDataArray[k++] = ( (amino::Math::vec2f *) primVars[var].inDataArray )[dataIndex][1];
						}
						break;
					case Bifrost::API::FloatV3Type:
						k = 3 * xExport;
						for (int sample = 0; sample < samples; sample++) {
							primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][0];
							primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][1];
							primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][2];
						}
						break;
					default:
						assert(0); // type not supported
						break;
				} // end of switch
			} // end of speed check
		} // end of exportToRIB check
	} // end of primVars loop
}

CvFloat *getPrimitivePrimVar( std::vector<primVarInfo>& primVars, std::vector<int>& varIndex, int var, int dataIndex)
{
	// get data according to type
	Bifrost::API::DataType dataType;

	if ( primVars[ varIndex[var] ].name == "speed" ) {
		dataType = Bifrost::API::FloatType;
	} else {
		dataType = primVars[ varIndex[var] ].channel.dataType();
	}

	switch ( dataType ) {
		case Bifrost::API::FloatType:
			return &( primVars[ varIndex[var] ].primDataArray[ dataIndex ]);
			break;
		case Bifrost::API::FloatV2Type:
			return &( primVars[ varIndex[var] ].primDataArray[ 2 * dataIndex ] );
			break;
		case Bifrost::API::FloatV3Type:
			return &( primVars[ varIndex[var] ].primDataArray[ 3 * dataIndex ] );
			break;
		default:
			assert(0); // type not supported
			return (CvFloat *) NULL;
			break;
	} // end of switch
}

//
//
// INIT STUFF
//
//
void initAndGetPrimitivesFrameData(	PrimitivesFrameData *frameData, PrimitivesInputData *inData, void getRenderDataCB( void *, void *, PluginType ) )
{
    // get information from bif file
	Bifrost::API::ObjectModel OM;
	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes( inData->bifFilename );
	Bifrost::API::FileIO fio = OM.createFileIO( correctedFilename );
	Bifrost::API::BIF::FileInfo fInfo = fio.info();

	// let's get avaliable channel names
	if ( inData->diagnostics.DEBUG > 0 ) {
		printf("\nAvaliable channels:\n");
	}

	for ( int i = 0; i < fInfo.channelCount; i++ ) {
		frameData->inputChannelNames.addUnique( fio.channelInfo(i).name );
		if ( inData->diagnostics.DEBUG > 0 ) {
			printf("\t%s\n", frameData->inputChannelNames[i].c_str());
		}
	}

	frameData->isPointCache = pointCacheStatusFromFileInfo ( fInfo );

	// check for inputChannelName in the channels list
	size_t channelExists = frameData->inputChannelNames.findFirstByName( inData->inputChannelName );
	if ( channelExists == frameData->inputChannelNames.npos ) {
		printf( "\nNo valid channel found with the name: %s\n\tCan not create primitives!\n", inData->inputChannelName );
		printf("\tExiting...\n");

		printEndOutput( "[BIFROST PRIMITIVES] END OUTPUT", inData->diagnostics );

		frameData->error = true;
		return;
	}

    // check number of tiles and elements
    if(channelExists != frameData->inputChannelNames.npos &&
            (fio.channelInfo( channelExists ).tileCount == 0 ||
             fio.channelInfo( channelExists ).elementCount == 0)) {
        frameData->empty = true;
        return;
	}

	// so we have the channel for lookup
	// now check all the required channels and assemble a frameData->loadChannelNames list
	Bifrost::API::String inputPrimVars ( inData->primVarNames );
    frameData->primVarNames = inputPrimVars.split(" ");

	if( checkSpeed( inData, frameData, fInfo ) ) {
		// if we get here that means speed is avaliable
		// so add it to the avaliable channel lists as if it exists in the bif file
		// and send information about it to the implicits
		frameData->speedNeeded = true;
	} else {
		// There is an error
		return;
	}

	// check whether transparency is used
	if ( frameData->pluginType == PLUGIN_PRIMITIVES ) {
		frameData->transparencyNeeded = frameData->primVarNames.findFirstByName("transparency") != frameData->inputChannelNames.npos ? true : false;
	}

	//
	// now process all the input data
	//

	// add lookup channel
	if ( frameData->pluginType == PLUGIN_PRIMITIVES ) {
		channelExists = frameData->inputChannelNames.findFirstByName( inData->inputChannelName );
		if ( channelExists != frameData->inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( inData->inputChannelName );
		}
	}

	// now process primvars list
	printf("\nPruning PRIMVARS List...\n");

	// check primVars for matches
	for ( int i = 0; i < frameData->primVarNames.count(); i++) {
		// do not do anything for speed as it will be calculated later
		if ( frameData->primVarNames[i] != "speed" ) { 
			channelExists = frameData->inputChannelNames.findFirstByName( frameData->primVarNames[i] );
			if ( channelExists != frameData->inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( frameData->primVarNames[i] );
			} else {
				printf( "\tCan not find primVar %s in file, skipping!\n", frameData->primVarNames[i].c_str() );
				frameData->primVarNames.remove( i );
				i--;
			}
		}
	}

	// now add cache type specific channels to the frameData->loadChannelNames
	if ( fInfo.componentType == Bifrost::API::PointComponentType ) {
		// this is a particle cache
		channelExists = frameData->inputChannelNames.findFirstByName( "position" );
		if ( channelExists != frameData->inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( "position" );
		}

		// do we need id64?
		if ( frameData->pluginType == PLUGIN_PRIMITIVES ) {
			frameData->idNeeded = false;
			if ( inData->skip > 1 || inData->mpSamples > 1 ) {
				// we are either skipping or multipointing or both
				// we need id
				frameData->idNeeded = true;
				channelExists = frameData->inputChannelNames.findFirstByName( "id64" );
				if ( channelExists != frameData->inputChannelNames.npos ) {
					frameData->loadChannelNames.addUnique( "id64" );
				}
			}
		}
	}

	// check motion blur is on and if it is export velocity channel(s)
	// get shutter times to see whether we are exporting motion blur or not
	frameData->motionBlur = false;
	frameData->velocityExists = true;

	getRenderDataCB ( (void *) frameData, (void *) inData, frameData->pluginType );

	// check velocity
	checkVelocity( frameData, fInfo );

	printf("\nMotionBlur: ");
	if ( frameData->motionBlur ) {
		if ( frameData->velocityExists ) {
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
		printf("\n\tFinal list of Primvars to Export:\n");
		for (int i = 0; i < frameData->primVarNames.count(); i++) {
			printf("\t\t%s\n", frameData->primVarNames[i].c_str() );
		}
	} else {
		printf ( "\nNo primvars found to export!\n" );
	}

	// print out frameData->loadChannelNames
	if ( inData->diagnostics.DEBUG > 0 ) {
		if (frameData->loadChannelNames.count() > 0 ) {
			printf("\n\tFinal list of channels to load:\n");
			for (int i = 0; i < frameData->loadChannelNames.count(); i++) {
				printf("\t\t%s\n", frameData->loadChannelNames[i].c_str() );
			}
		}
	}
}


int dumpPrimitives	(	PrimitivesInputData *inData,
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
													CvFloat *cPos, CvFloat *cWidth, CvFloat *cTra, CvFloat *cNormal, CvInt *cVerts, CvInt *cCode,
													void * ),
						void transformPointsCB( CvPoint [2], CvFloat, void * ),
						Bifrost::API::VoxelSampler *getAndSetThreadDataCB( std::string, Bifrost::API::VoxelChannel, void * ),
						void setExportTokenCB( std::vector<CvToken>&, ExportTokens token ),
						std::string getPrimVarTokenCB( TokenClass, Bifrost::API::DataType, std::string )
					)
{
	Bifrost::API::Layout layout = component.layout();
	bool isPointCache = pointCacheStatusFromComponent( component );

	bool idExists = false;
	if ( idChan.valid() ) {
		idExists = true;
		printf("\n");
	} else {
		if ( frameData->idNeeded ) {
			printf("\tThere is no id per particle in this file!\n\tId based operations will use array index!\n\n");
		}
	}

	// if we are exporting all at once, do init
	CvInt *cVerts = NULL;
	CvInt *cCode = NULL;
	CvFloat *cPos = NULL;
	CvFloat *cWidth = NULL;
	CvFloat *cTra = NULL;
	CvFloat *cNormal = NULL;

	Bifrost::API::TileData<amino::Math::vec3f> posArray;
	Bifrost::API::TileData<amino::Math::vec3f> velArray;
	Bifrost::API::TileData<float> velArrayU;
	Bifrost::API::TileData<float> velArrayV;
	Bifrost::API::TileData<float> velArrayW;
	Bifrost::API::TileData<float> denArray;
	Bifrost::API::TileData<uint64_t> idArray;

	// voxel sampler stuff for gradient calculation
	Bifrost::API::VoxelSampler *channelSampler = NULL;
	Bifrost::API::VoxelChannel outChannel;

	//define tokens and ptrs for export
	std::vector<CvToken> toks;
	std::vector<CvPointer> ptr;
	std::vector<int> varIndex;

	// init stuff if necessary
	if ( inData->renderType == PRIM_POINT ) {
		// store two positions: one for shutter open, one for shutter close
		cPos = (CvFloat *) malloc ( 6 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cPos );

		// store two radiuses as it can change because of cam based radius
		cWidth = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cWidth );

		// put to the tokens list
		setExportTokenCB( toks, TOK_POSITION );
		setExportTokenCB( toks, TOK_RADIUS );
		ptr.push_back(cPos);
		ptr.push_back(cWidth);
			
		//
		// define primvar values
		//

		// transparency
		if ( frameData->transparencyNeeded ) {
			// store two transparencies as it can change because of radius
			cTra = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
			frameData->mem.push_back( (void *) cTra );

			setExportTokenCB( toks, TOK_TRANSPARENCY );
			ptr.push_back(cTra);
		}

		// other primvars
		declarePrimVars( frameData->primVars, toks, ptr, varIndex, frameData->finalChunkSize, frameData->mem, TOKCLASS_UNIFORM, getPrimVarTokenCB );

		// are we exporting normals
		if ( inData->exportNormalAsPrimvar ) {
			if ( frameData->cacheType != VOXEL ) {
				// we will export density gradient as normal
				cNormal = (CvFloat *) malloc ( 3 * sizeof(CvFloat) * frameData->finalChunkSize );
				frameData->mem.push_back( (void *) cNormal );

				setExportTokenCB( toks, TOK_GRADIENT_NORMAL );
				ptr.push_back(cNormal);

				// create a particle splatter for fast gradient calc
				// create voxel structure to hold the voxel representation
				printf("Converting to voxels for gradient calc...\n");

				// create a new object model
				Bifrost::API::ObjectModel outOM;

				// create state server
				Bifrost::API::StateServer outSS = outOM.createStateServer();

				// create top object
				Bifrost::API::Object outObject = outSS.createObject("Particle2Voxel", "voxel_channel");
				// create a component
				Bifrost::API::Component outComponent = outSS.createComponent( Bifrost::API::VoxelComponentType, "voxel_channel-volume", layout.voxelScale() );
				// add it to the main object
				outObject.addComponent(outComponent);
				// create a channel in the component
				outChannel = outSS.createChannel( outComponent, Bifrost::API::FloatType, inData->inputChannelName);
				// set values for channel
				outChannel.setBackgroundValue( 0.0f );
				outChannel.setOffsetType( Bifrost::API::VoxelOffsetType::Center );

				// set layout values
				Bifrost::API::Layout outLayout = outComponent.layout();

				// create the root of the tile tree
				Bifrost::API::TileAccessor outTacc = outLayout.tileAccessor();
				Bifrost::API::TreeIndex outRoot = outTacc.addRoot();

				// get tiles and initialise stuff
				Bifrost::API::TileIterator inTileIter = layout.tileIterator( 0, layout.maxDepth(), Bifrost::API::TraversalMode::ParallelBreadthFirst );

				while (inTileIter) {
					// get tile coords
					const Bifrost::API::Tile& inTile = *inTileIter;
					Bifrost::API::TileCoord inTileCoord = inTile.coord();
					Bifrost::API::TreeIndex inTileIndex = inTile.index();

					// create a tile at this coord and depth
					Bifrost::API::TreeIndex outTileIndex = outTacc.addTile(inTileCoord.i, inTileCoord.j, inTileCoord.k, inTile.index().depth);

					//init tile data
					Bifrost::API::TileData<float> outTileData = outChannel.tileData<float>( outTileIndex );
					for ( int tk = 0; tk < 5; tk++ ) {
						for ( int tj = 0; tj < 5; tj++ ) {
							for ( int ti = 0; ti < 5; ti++ ) {
								outTileData(ti, tj, tk) = 0.0;
							}
						}
					}

					// are there any elements in this tile
					int nofTileParticles = (int) posChan.elementCount( inTile.index() );
					if ( nofTileParticles > 0 ) {
						Bifrost::API::TileData<amino::Math::vec3f> tilePosData = posChan.tileData<amino::Math::vec3f>( inTileIndex );
						Bifrost::API::TileData<float> tileInputChannelData = inputChan.tileData<float>( inTileIndex );

						// fill tile data
						for (int particle = 0; particle < nofTileParticles; particle++) {
							amino::Math::vec3f particlePos = tilePosData[particle];
							float particleDensity = tileInputChannelData[particle];

							int tileDataI = clip <int> ( (int) floor( particlePos[0] - inTileCoord.i ), 0, 4);
							int tileDataJ = clip <int> ( (int) floor( particlePos[1] - inTileCoord.j ), 0, 4);
							int tileDataK = clip <int> ( (int) floor( particlePos[2] - inTileCoord.k ), 0, 4);
							outTileData(tileDataI, tileDataJ, tileDataK) += particleDensity;
						}
					}
					++inTileIter;
				} // end tile iterator
			} else {
			} // end cache type check
		} // end exportGradientAsPrimvar check
	}

	if ( inData->renderType == PRIM_SPHERE ) {
		// alloc pos with 6 elements per sphere: one for each shutter value
		cPos = (CvFloat *) malloc ( 6 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cPos );

		// store two radiuses as it can change because of cam based radius
		cWidth = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cWidth );

		//
		// define primvar values
		//

		// transparency
		if ( frameData->transparencyNeeded ) {
			// no need to add it to the toks and ptr here as it is exported with Riopacity
			// store two of them because of cam radius
			cTra = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
			frameData->mem.push_back( (void *) cTra );
		}

		// other primvars
		declarePrimVars( frameData->primVars, toks, ptr, varIndex, frameData->finalChunkSize, frameData->mem, TOKCLASS_CONSTANT, getPrimVarTokenCB );
	}

	if ( inData->renderType == PRIM_BLOB ) {
		// malloc for 2xnofElements elements + 2 op elements + nofElements ids
		cCode = (CvInt *) malloc ( sizeof(CvInt) * 3 * frameData->finalChunkSize + 2 );
		frameData->mem.push_back( (void *) cCode );

		// store an extra array for radius because of the cam based radius
		// array is arranged as such:
		// 1st frameData->finalChunkSize items -> radius at Shutter Open
		// 2rd, 3th, 4th -> position at shutter open
		// 5th frameData->finalChunkSize items -> radius at Shutter Close
		// 6rd, 7th, 8th -> position at shutter close
		cPos = (CvFloat *) malloc ( 8 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cPos );

		//
		// define primvar values
		//

		// transparency
		if ( frameData->transparencyNeeded ) {
			// store two transparencies as it can change because of cam based radius
			cTra = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
			frameData->mem.push_back( (void *) cTra );

			setExportTokenCB( toks, TOK_TRANSPARENCY );
			ptr.push_back(cTra);
		}

		// other primvars
		declarePrimVars( frameData->primVars, toks, ptr, varIndex, frameData->finalChunkSize, frameData->mem, TOKCLASS_VARYING, getPrimVarTokenCB );
	}

	if ( inData->renderType == PRIM_CURVE ) {
		cVerts = (CvInt *) malloc ( sizeof(CvInt) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cVerts );
		cPos = (CvFloat *) malloc ( 6 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cPos );
		cWidth = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
		frameData->mem.push_back( (void *) cWidth );

		setExportTokenCB( toks, TOK_POSITION );
		setExportTokenCB( toks, TOK_RADIUS );
		ptr.push_back(cPos);
		ptr.push_back(cWidth);

		//
		// define primvar values
		// we need two times the data as we have two vertices on the curve
		//

		// transparency
		if ( frameData->transparencyNeeded ) {
			cTra = (CvFloat *) malloc ( 2 * sizeof(CvFloat) * frameData->finalChunkSize );
			frameData->mem.push_back( (void *) cTra );

			setExportTokenCB( toks, TOK_TRANSPARENCY );
			ptr.push_back(cTra);
		}

		// other primvars
		declarePrimVars( frameData->primVars, toks, ptr, varIndex, frameData->finalChunkSize, frameData->mem, TOKCLASS_VARYING, getPrimVarTokenCB );
	}

	//
	// declarations are done
	// start the hard work
	//

	// emit primitives
	if (inData->renderType == PRIM_POINT) {
		printf("\nStarting Points export...\n");
	} else if (inData->renderType == PRIM_SPHERE) {
		printf("\nStarting Sphere export...\n");
	} else if (inData->renderType == PRIM_BLOB) {
		printf("\nStarting Blobby export...\n");
	} else {
		printf("\nStarting Curves export...\n");
	}

	// calc frequency for export reporting
	int freq = frameData->nofElements > 9 ? 9 : frameData->nofElements;
	int reportFreq = frameData->nofBaseElements / freq;
	if (reportFreq == 0) {
		reportFreq = 1;
	}

	if ( inData->diagnostics.DEBUG > 0 ) {
		printf("\nReport Freq: %d\n\n", reportFreq);
	}

	//
	//
	// start assembling export arrays
	//
	//

	printf("\tExporting %ld items...\n", frameData->nofElements);
	outputPrimVarsToConsole( toks );

	int xBase = 0;
	int xChunk = 0;
	int xMulti = 0;

	// get voxel offset if we are rendering a voxel cache
	amino::Math::vec3f voxelOffset ( 0.0f, 0.0f, 0.0f );
	if ( !isPointCache ) {
		voxelOffset = getVoxelOffset ( inputChan );
	}

	// get tile dim info stuff
	const Bifrost::API::TileDimInfo dimInfo = layout.tileDimInfo( layout.maxDepth() );

	// define a tile accessor to look for tiles
	Bifrost::API::TileAccessor tAcc = layout.tileAccessor();

	//
	// start iterating over tiles
	//
	for ( int t = 0; t < layout.tileCount( layout.maxDepth() ); t++ ) {
		// are we done exporting?
		if (xBase >= frameData->nofBaseElements) {
			break;
		}

		// get tile data
		Bifrost::API::TreeIndex tIndex(t, layout.maxDepth());
		Bifrost::API::Tile tile = tAcc.tile( tIndex );

		// declare some arrays
		posArray = posChan.tileData<amino::Math::vec3f>( tIndex );
		velArray = velChan.tileData<amino::Math::vec3f>( tIndex );
		velArrayU = velChanU.tileData<float>( tIndex );
		velArrayV = velChanV.tileData<float>( tIndex );
		velArrayW = velChanW.tileData<float>( tIndex );
		denArray = inputChan.tileData<float>( tIndex );
		if ( idExists ) {
			idArray = idChan.tileData<uint64_t>( tIndex );
		}

		// get tile primvar data
		for ( unsigned var = 0; var < frameData->primVars.size(); var++ ) {
			if ( frameData->primVars[var].name != "speed" ) {
				// get data according to type
				switch ( frameData->primVars[var].channel.dataType() ) {
					case Bifrost::API::FloatType:
						frameData->primVars[var].inDataArray = (void *) frameData->primVars[var].channel.tileDataPtr( tIndex, frameData->primVars[var].size );
						break;
					case Bifrost::API::FloatV2Type:
						frameData->primVars[var].inDataArray = (void *) frameData->primVars[var].channel.tileDataPtr( tIndex, frameData->primVars[var].size );
						break;
					case Bifrost::API::FloatV3Type:
						frameData->primVars[var].inDataArray = (void *) frameData->primVars[var].channel.tileDataPtr( tIndex, frameData->primVars[var].size );
						break;
					default:
						assert(0); // type not supported
						break;
				}
			}
		}

		//
		// loop over elements and gather data
		//
		int nofLoopElements = (int) (isPointCache ? posChan.elementCount( tIndex ) : inputChan.elementCount( tIndex ) );

		// loop
		for (int i = 0; i < nofLoopElements; i++, xBase++) {
			// print progress
			if ( xBase % reportFreq == 0 && xBase != 0) {
				printf("\t\tExporting for item: %d...\n", xMulti);
			}

			// are we exporting or skipping?
			// if id exists we skip according to that which is the correct way
			uint64_t skipId = idExists ? idArray[i] : (uint64_t) xBase;
			bool notSkipping = ( skipId % ( (uint64_t) inData->skip ) == (uint64_t) 0 );

			if ( (inData->skip == 1) || notSkipping ) {
				amino::Math::vec3f wp, vel;
				vel[0] = vel[1] = vel[2] = 0.0f;
				CvPoint finalWp[2];

				// get tile data
				if ( isPointCache ) {
					wp = posArray[i] * inData->spaceScale;
					if ( frameData->motionBlur ) {
						vel = velArray[i] * inData->spaceScale;
					}
				} else {
					// calc a pos from the voxel
					const Bifrost::API::TileCoord coord = tile.coord();

					amino::Math::vec3i tileBoundsMin;
					tileBoundsMin[0] = coord.i;
					tileBoundsMin[1] = coord.j;
					tileBoundsMin[2] = coord.k;

					// get voxel index
					int voxelI = int ( i % dimInfo.tileWidth);
					int voxelJ = ( int ( i / dimInfo.tileWidth) ) % dimInfo.tileWidth;
					int voxelK = int ( i / ( dimInfo.tileWidth * dimInfo.tileWidth ) );

					// we don't multiply with dx here: it is done below for all
					wp[0] = inData-> spaceScale * ( tileBoundsMin[0] + voxelI * dimInfo.voxelWidth + voxelOffset[0] );
					wp[1] = inData-> spaceScale * ( tileBoundsMin[1] + voxelJ * dimInfo.voxelWidth + voxelOffset[1] );
					wp[2] = inData-> spaceScale * ( tileBoundsMin[2] + voxelK * dimInfo.voxelWidth + voxelOffset[2] );
					if ( frameData->motionBlur ) {
						vel[0] = velArrayU[i] * inData-> spaceScale;
						vel[1] = velArrayV[i] * inData-> spaceScale;
						vel[2] = velArrayW[i] * inData-> spaceScale;
					}
				}

				float channelVal = std::min<float>(1.0, denArray[i]) * inData->channelScale;

				// init RNG
				srand48( skipId );
				// get a random radius for this particle
				float multiRadius = inData->mpMinRadius + (inData->mpMaxRadius - inData->mpMinRadius) * (float) drand48();
				// now create mpSamples points around the original point
				for ( int multi = 0; multi < inData->mpSamples; multi++, xMulti++, xChunk++) {
					// we will do an export for this particle
					// so check the chunksize if it is passed
					// export what we gathered, reset the counter and continue

					if ( xChunk == frameData->finalChunkSize ) {
						exportPrimitivesCB	(	inData,
												frameData,
												xChunk,
												toks,
												ptr,
												varIndex,
												cPos,
												cWidth,
												cTra,
												cNormal,
												cVerts,
												cCode,
												usrData
											);
						xChunk = 0;
					}

					//m_mutex->Unlock();

					// don't change original data if we are not multipointing
					float distanceToOriginal = 0.0f;
					float mpDistanceFalloff = 1.0f;
					amino::Math::vec3f newWP;

					// do multipointing if it is turned on
					if (frameData->isMultiPointing) {
						// create point in a unit sphere by rejection
						do {
							newWP[0] = 2.0f * (float) drand48() - 1;
							newWP[1] = 2.0f * (float) drand48() - 1;
							newWP[2] = 2.0f * (float) drand48() - 1;
							distanceToOriginal = newWP[0] * newWP[0] + newWP[1] * newWP[1] + newWP[2] * newWP[2];
						} while ( distanceToOriginal > 1 );

						// calc radius for the found sample
						distanceToOriginal = std::max ( 0.000000000000001f, std::sqrt(distanceToOriginal) );

						// do we attract to the sphere surface
						if ( inData->mpSurfaceAttract != 0.0f ) {
							amino::Math::vec3f sphereWP = newWP / distanceToOriginal;

							newWP = sphereWP * inData->mpSurfaceAttract + newWP * (1.0f - inData->mpSurfaceAttract);
						}

						// scale back upto splatRadius
						// if we are displacing, apply that as well
						if ( inData->mpDisplacementValue != 0.0f) {
							CvPoint3 noiseP;
							noiseP[0] = ( newWP[0] + wp[0] ) * inData->mpDisplacementNoiseFrequency;
							noiseP[1] = ( newWP[1] + wp[1] ) * inData->mpDisplacementNoiseFrequency;
							noiseP[2] = ( newWP[2] + wp[2] ) * inData->mpDisplacementNoiseFrequency;

							CvFloat radNoise = calcNoiseCB ( noiseP );
							radNoise = ( 1.0f - inData->mpDisplacementValue ) + inData->mpDisplacementValue * radNoise;
							newWP *= radNoise * multiRadius;						
						} else {
							newWP *= multiRadius; 
						}

						mpDistanceFalloff = 1.0f - calcBlendVal ( inData->mpFalloffType, inData->mpFalloffStart, inData->mpFalloffEnd, distanceToOriginal / multiRadius);
						newWP += wp;
					} else {
						newWP = wp;
					}

					// position is in tile space so let's multiply with dx
					newWP *= frameData->voxelScale;

					// calc data according to the type
					switch (inData->renderType) {
						case PRIM_POINT:
							{
								// position calc
								// assign both shutter open and close positions
								int k = 3 * xChunk;
								cPos[k] =		finalWp[0][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[0];
								cPos[k + 1] =	finalWp[0][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[0];
								cPos[k + 2] =	finalWp[0][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[0];

								k += 3 * frameData->finalChunkSize;
								cPos[k] =		finalWp[1][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[1];
								cPos[k + 1] =	finalWp[1][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[1];
								cPos[k + 2] =	finalWp[1][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[1];

								// width calc
								k = xChunk;
								if (inData->useChannelToModulateRadius) {
									cWidth[k] = mpDistanceFalloff * inData->radius * channelVal;
									cWidth[k + frameData->finalChunkSize] = cWidth[k];
								} else {
									cWidth[k] = mpDistanceFalloff * inData->radius;
									cWidth[k + frameData->finalChunkSize] = cWidth[k];
								}

								// apply cam based radius
								if (inData->camRadius) {
									// apply cam based radius to calculated radius
									// use calculated point from previous stage
									transformPointsCB( finalWp, (CvFloat) frameData->shutter[0], usrData );
									float distance1 = calcCamDistance (finalWp[0]);
									float distance2 = calcCamDistance (finalWp[1]);
									frameData->minDistance = std::min ( frameData->minDistance, distance1 );
									frameData->maxDistance = std::max ( frameData->maxDistance, distance1 );
									float radMulFactor = calcCamMulFactor(inData, distance1);
									cWidth[k] *= radMulFactor;
									radMulFactor = calcCamMulFactor(inData, distance2);
									cWidth[k + frameData->finalChunkSize] *= radMulFactor;
								}

								// transparency calc
								if ( frameData->transparencyNeeded ) {
									k = xChunk;
									cTra[k] = exp( -cWidth[k] );
									cTra[k + frameData->finalChunkSize] =  exp( -cWidth[k + frameData->finalChunkSize] );
								}

								// primvar calc
								assignPrimitivePrimVars( frameData->primVars, xChunk, i, 1, isPointCache, inData, frameData);

								// normal calc
								if ( inData->exportNormalAsPrimvar ) {
									// get sample pos
									k = 3 * xChunk;
									amino::Math::vec3f normal, newPos;
									// we need to divide by spacescale here as cPos is in scaled space but density lookup channel is in original space
									newPos[0] = cPos[k] / inData->spaceScale;
									newPos[1] = cPos[k + 1] / inData->spaceScale;
									newPos[2] = cPos[k + 2] / inData->spaceScale;

									if ( frameData->cacheType != VOXEL ) {
										channelSampler = getAndSetThreadDataCB( frameData->idString, outChannel, usrData );

										// sample the channel
										(*channelSampler).sampleGradient<float>( newPos, normal );
									} else {
										// for voxel caches we just calc gradient of the input
										channelSampler = getAndSetThreadDataCB( frameData->idString, outChannel, usrData );

										// sample channel
										(*channelSampler).sampleGradient<float>( newPos, normal );
									}

									// normalize vector
									float normalMag = sqrt( normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2] );
									if ( (normalMag - 0.0f) > FLT_EPSILON) {
										cNormal[k++] = normal[0] / normalMag;
										cNormal[k++] = normal[1] / normalMag;
										cNormal[k] = normal[2] / normalMag;
									} else {
										cNormal[k++] = 0.0;
										cNormal[k++] = 1.0;
										cNormal[k] = 0.0;
									}
								}

	 							break;
							}
						case PRIM_SPHERE:
							{
								// position calc
								// assign both shutter open and close positions
								int k = 3 * xChunk;
								cPos[k] =		finalWp[0][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[0];
								cPos[k + 1] =	finalWp[0][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[0];
								cPos[k + 2] =	finalWp[0][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[0];

								k += 3 * frameData->finalChunkSize;
								cPos[k] =		finalWp[1][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[1];
								cPos[k + 1] =	finalWp[1][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[1];
								cPos[k + 2] =	finalWp[1][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[1];

								// width calc
								k = xChunk;
								if (inData->useChannelToModulateRadius) {
									float radius = mpDistanceFalloff * inData->radius * channelVal / 2.0f;
									cWidth[k] = radius > 0.000001f ? radius : 0.000001f;
									cWidth[k + frameData->finalChunkSize] = cWidth[k];
								} else {
									float radius = mpDistanceFalloff * inData->radius / 2.0f;
									cWidth[k] = radius > 0.000001f ? radius : 0.000001f;
									cWidth[k + frameData->finalChunkSize] = cWidth[k];
								}

								// apply cam based radius
								if (inData->camRadius) {
									// apply cam based radius to calculated radius
									// use calculated point from previous stage
									transformPointsCB( finalWp, (CvFloat) frameData->shutter[0], usrData );
									float distance1 = calcCamDistance (finalWp[0]);
									float distance2 = calcCamDistance (finalWp[1]);
									frameData->minDistance = std::min ( frameData->minDistance, distance1 );
									frameData->maxDistance = std::max ( frameData->maxDistance, distance1 );
									float radMulFactor = calcCamMulFactor(inData, distance1);
									cWidth[k] *= radMulFactor;
									radMulFactor = calcCamMulFactor(inData, distance2);
									cWidth[k + frameData->finalChunkSize] *= radMulFactor;
								}

								// transparency calc
								if ( frameData->transparencyNeeded ) {
									k = xChunk;
									cTra[k] = exp(-cWidth[k]);
									cTra[k + frameData->finalChunkSize] =  exp(-cWidth[k + frameData->finalChunkSize]);
								}

								// primvar calc
								assignPrimitivePrimVars( frameData->primVars, xChunk, i, 1, isPointCache, inData, frameData);

								break;
							}
						case PRIM_BLOB:
							{
								// for opcode 1005

								// export code array
								int k = xChunk * 2;
								cCode[k++] = 1005;
								cCode[k] = 4 * xChunk;

								// calc final position for frameData->shutter open
								finalWp[0][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[0];
								finalWp[0][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[0];
								finalWp[0][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[0];
								// calc final position for frameData->shutter close
								finalWp[1][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[1];
								finalWp[1][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[1];
								finalWp[1][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[1];

								// export scale
								k = 4 * xChunk;
								int offset = 4 * frameData->finalChunkSize;
								if (inData->useChannelToModulateRadius) {
									cPos[k] =			mpDistanceFalloff * inData->radius * channelVal;
									cPos[k + offset] =	cPos[k];
								} else {
									cPos[k] =			mpDistanceFalloff * inData->radius;
									cPos[k + offset] =	cPos[k];
								}

								// export transform
								cPos[k + 1] = finalWp[0][0];
								cPos[k + 2] = finalWp[0][1];
								cPos[k + 3] = finalWp[0][2];

								cPos[k + 1 + offset] = finalWp[1][0];
								cPos[k + 2 + offset] = finalWp[1][1];
								cPos[k + 3 + offset] = finalWp[1][2];

								// apply cam based radius
								if (inData->camRadius) {
									// apply cam based radius to calculated radius
									// use calculated point from previous stage
									transformPointsCB( finalWp, (CvFloat) frameData->shutter[0], usrData );
									float distance1 = calcCamDistance (finalWp[0]);
									float distance2 = calcCamDistance (finalWp[1]);
									frameData->minDistance = std::min ( frameData->minDistance, distance1 );
									frameData->maxDistance = std::max ( frameData->maxDistance, distance1 );
									float radMulFactor = calcCamMulFactor(inData, distance1);
									cPos[k] *= radMulFactor;
									radMulFactor = calcCamMulFactor(inData, distance2);
									cPos[k + offset] *= radMulFactor;
								}

								// transparency calc
								if ( frameData->transparencyNeeded ) {
									k = xChunk;
									cTra[k] = exp( -cPos[k * 4] );
									cTra[k + frameData->finalChunkSize] = exp( -cPos[k * 4 + offset] );
								}

								// primvar calc
								assignPrimitivePrimVars( frameData->primVars, xChunk, i, 1, isPointCache, inData, frameData);
								break;
							}
						case PRIM_CURVE:
							{
								cVerts[xChunk] = 2;

								int k = 6 * xChunk;
								cPos[k++] = finalWp[0][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[0];
								cPos[k++] = finalWp[0][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[0];
								cPos[k++] = finalWp[0][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[0];
								if ( !frameData->motionBlur ) {
									// if there is no motionblur we modify the frameData->shutter so that we have at least some curve.
									frameData->shutter[1] = frameData->shutter[0] + 0.001f;
								}
								cPos[k++] = finalWp[1][0] = newWP[0] + vel[0] * inData->velocityScale * frameData->shutter[1];
								cPos[k++] = finalWp[1][1] = newWP[1] + vel[1] * inData->velocityScale * frameData->shutter[1];
								cPos[k]   = finalWp[1][2] = newWP[2] + vel[2] * inData->velocityScale * frameData->shutter[1];

								// radius calc
								k = 2 * xChunk;
								if (inData->useChannelToModulateRadius) {
									cWidth[k] = mpDistanceFalloff * inData->radius * channelVal / 2.0f;
									cWidth[k + 1] = cWidth[k];
								} else {
									cWidth[k] = mpDistanceFalloff * inData->radius / 2.0f;
									cWidth[k + 1] = cWidth[k];
								}

								// apply cam based radius
								if (inData->camRadius) {
									// apply cam based radius to calculated radius
									// use calculated point from previous stage
									transformPointsCB( finalWp, (CvFloat) frameData->shutter[0], usrData );
									float distance1 = calcCamDistance (finalWp[0]);
									float distance2 = calcCamDistance (finalWp[1]);
									frameData->minDistance = std::min ( frameData->minDistance, distance1 );
									frameData->maxDistance = std::max ( frameData->maxDistance, distance1 );
									float radMulFactor = calcCamMulFactor(inData, distance1);
									cWidth[k] *= radMulFactor;
									radMulFactor = calcCamMulFactor(inData, distance2);
									cWidth[k + 1] *= radMulFactor;
								}

								// transparency calc
								if ( frameData->transparencyNeeded ) {
									k = 2 * xChunk;
									cTra[k] = exp(-cWidth[k]);
									cTra[k + 1] =  exp(-cWidth[k + 1]);
								}

								// primvar calc
								assignPrimitivePrimVars( frameData->primVars, 2 * xChunk, i, 2, isPointCache, inData, frameData);

								break;
							}
						default:
							break;
					} // end type switch
				} // end multi
			} // end skip check
		} // end tiledata loop

		// are we done exporting?
		if (xBase >= frameData->nofBaseElements) {
			break;
		}

	} // end tile loop

	// export the remaining items
	if ( xChunk > 0 && xChunk < frameData->finalChunkSize ) {
		exportPrimitivesCB	(	inData,
								frameData,
								xChunk,
								toks,
								ptr,
								varIndex,
								cPos,
								cWidth,
								cTra,
								cNormal,
								cVerts,
								cCode,
								usrData
							);
	}

	return xMulti;
} 

}}
