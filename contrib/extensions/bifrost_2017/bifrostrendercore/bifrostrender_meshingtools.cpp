//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2016
// Written by Erdem Taylan

#include <bifrostrendercore/bifrostrender_meshingtools.h>

namespace Bifrost{
namespace RenderCore{

void allocatePolyData( size_t vertexCount, size_t polyCount, FrameData *frameData, CvInt **vertices, CvFloat **pos, CvFloat **normal, CvFloat **motionPos)
{
	// allocate arrays for the data
	*vertices = (CvInt *) malloc ( 3 * sizeof(CvInt) * polyCount );
	frameData->mem.push_back( (void *) *vertices );

	*pos = (CvFloat *) malloc ( 3 * vertexCount * sizeof(CvFloat));
	frameData->mem.push_back( (void *) *pos );

	if ( frameData->exportNormals ) {
		*normal = (CvFloat *) malloc ( 3 * vertexCount * sizeof(CvFloat));
		frameData->mem.push_back( (void *) *normal );
	}

	if ( frameData->motionBlur ) {
		*motionPos = (CvFloat *) malloc ( 3 * vertexCount * sizeof(CvFloat));
		frameData->mem.push_back( (void *) *motionPos );
	}
}

void preparePolyData(	size_t vertexCount,
						size_t polyCount,
						MarchingCubes  *mesher,
						FrameData *frameData,
						ImplicitsInputData *inData,
						CvInt *vertices,
						CvFloat *pos,
						CvFloat *normal,
						CvFloat *motionPos )
{
	// assign primvar data
	for ( unsigned var = 0; var < frameData->primVars.size(); var++ ) {
		size_t count = 0;
		Bifrost::API::VoxelChannel primVarChannel = (Bifrost::API::VoxelChannel) frameData->primVars[var].channel;
		// get data according to type
		switch ( primVarChannel.dataType() ) {
			case Bifrost::API::FloatType:
				frameData->primVars[var].inDataArray = (void *) mesher->c_primVars[ frameData->primVars[var].name ].data();
				break;
			case Bifrost::API::FloatV2Type:
				frameData->primVars[var].inDataArray = (void *) mesher->c_primVars[ frameData->primVars[var].name ].data();
				break;
			case Bifrost::API::FloatV3Type:
				frameData->primVars[var].inDataArray = (void *) mesher->c_primVars[ frameData->primVars[var].name ].data();
				break;
			default:
				assert(0); // type not supported
				break;
		}
	}

	printf("\tPreparing triangles...\n");
				
	for (int i = 0; i < polyCount; i++) {
		int index = 3* i;

		// fill the vertices array
		amino::Math::vec3i poly = mesher->getTriangle( i );
		vertices[ index ] = poly[ 0 ];
		vertices[ index + 1 ] = poly[ 1 ];
		vertices[ index + 2 ] = poly[ 2 ];
	}

	printf("\tPreparing positions...\n");
	for (int i = 0; i < vertexCount; i++) {
		amino::Math::vec3f vertex = mesher->getMergedVertexPosition( i );

		if ( frameData->motionBlur ) {
			amino::Math::vec3f vel = mesher->getVertexVelocity( i );

			// output positions taking shutter times into account
			pos[3 * i]		= vertex[0] + inData->velocityScale * vel[0] * frameData->shutter[0];
			pos[3 * i + 1]	= vertex[1] + inData->velocityScale * vel[1] * frameData->shutter[0];
			pos[3 * i + 2]	= vertex[2] + inData->velocityScale * vel[2] * frameData->shutter[0];

			motionPos[3 * i]		= vertex[0] + inData->velocityScale * vel[0] * frameData->shutter[1];
			motionPos[3 * i + 1]	= vertex[1] + inData->velocityScale * vel[1] * frameData->shutter[1];
			motionPos[3 * i + 2]	= vertex[2] + inData->velocityScale * vel[2] * frameData->shutter[1];
		} else {
			// just output position
			pos[3 * i] = vertex[0];
			pos[3 * i + 1] = vertex[1];
			pos[3 * i + 2] = vertex[2];
		}

		// export normal
		if ( frameData->exportNormals ) {
			amino::Math::vec3f vertexNormal = mesher->getVertexNormal( i );
			normal[3 * i] = vertexNormal[0];
			normal[3 * i + 1] = vertexNormal[1];
			normal[3 * i + 2] = vertexNormal[2];
		}
	}

	// fill primvar arrays
	if ( frameData->primVars.size() > 0 ) {
		for ( unsigned var = 0; var < frameData->primVars.size(); var++ ) {
			printf("\tPreparing PrimVar: %s...\n", frameData->primVars[var].name.c_str());
			Bifrost::API::VoxelChannel primVarChannel = frameData->primVars[var].channel;
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
}

bool checkPolyCount( size_t vertexCount, size_t polyCount )
{
	if ( polyCount == 0 ) {
		printf( "\tNo polygons are received from the mesher!\n" );
		printf( "\tExiting...\n\n" );

		printf( "\n---------------------------------------\n" );
		printf( "END OUTPUT FOR BIFROSTPOLYMESH\n" );
		printf( "---------------------------------------\n\n" );
		return true;
	} else {
		printf( "\t%d polygons and %d vertices are received from the mesher!\n", polyCount, vertexCount );
		return false;
	}
}

}}
