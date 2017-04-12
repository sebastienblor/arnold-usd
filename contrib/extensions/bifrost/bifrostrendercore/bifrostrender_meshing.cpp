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

#include <bifrostrendercore/bifrostrender_meshing.h>
// #include <bifrostrendercore/bifrostrender_meshingtables.h>

namespace Bifrost{
namespace RenderCore{

MarchingCubes::MarchingCubes( FrameData *frameData )
	: CoreMesherInterface( frameData )
{
    c_ids.rehash ( 12 * 125 * frameData->sampleRate * frameData->sampleRate * frameData->sampleRate );
}

float CoreMesherInterface::getFieldValue( amino::Math::vec3f position )
{
	return c_sampler->sample<float>( position );
}

size_t MarchingCubes::calcVoxel( float *gridVals, amino::Math::vec3i& voxelCornerPos )
{
	// define vars
	int edges[ 12 ];
	int cubeIndex = 0;

	for ( int corner = 0; corner < 8; corner++ ) {
		cubeIndex |= ( gridVals[corner] > 0.0f ) ? 1 << corner : 0;
	}

	// compute vertices
	if ( c_edgeTable[cubeIndex] == 0 ) {
		return 0;
	}

	size_t vertexCount = 0;
	for ( int count = 0; count < 12; count++ ) {
		if ( ( c_edgeTable[cubeIndex] & ( 1 << count) ) == 0 ) {
			continue;
		}

		// calc a new vertex
		amino::Math::vec3f newVertex (0, 0, 0);
		amino::Math::vec2i e ( c_edgeIndex[count][0], c_edgeIndex[count][1] );

        amino::Math::vec3i p0 ( c_cubeVerts[e[0]] );
        amino::Math::vec3i p1 ( c_cubeVerts[e[1]] );

		float a = gridVals[ e[0] ];
		float b = gridVals[ e[1] ];
		float d = a - b;

		float t = 0.0f;
		if ( std::fabs( d ) > FLT_EPSILON ) {
			t = a / d;
		}

		for ( int coord = 0; coord < 3; coord++ ) {
			// calc interpolated vertex position in tile space
            newVertex[ coord ] = (voxelCornerPos[coord] + p0[coord]) + t * ( p1[coord] - p0[coord] );
            newVertex[ coord ] /= c_frameData->sampleRate;
		}

		// check whether this point is generated before
		auto positionInserted = c_ids.find( newVertex );
        if ( positionInserted != c_ids.end() ) {
			edges[count] = (int) positionInserted->second;
		} else {
			size_t vertexID = getVertexCount();
			edges[count] = (int) vertexID;
			addVertexPosition( newVertex );
			c_ids[newVertex] = vertexID;
		}
		vertexCount += 1;
	}

	for ( int count = 0; c_triTable[cubeIndex][count]!=-1; count+= 3 ) {
		amino::Math::vec3i triangle (	edges[ c_triTable[ cubeIndex ][ count ] ],
										edges[ c_triTable[ cubeIndex ][ count + 1 ] ],
										edges[ c_triTable[ cubeIndex ][ count + 2 ] ]		);
		addTriangle( triangle );
	}

	return vertexCount;
}

// init static variables

bool SurfaceNets::c_tablesCreated = false;
int SurfaceNets::c_cubeEdges[ 24 ];
int SurfaceNets::c_edgeTable[ 256 ];

void SurfaceNets::createTables()
{
	int edgesCount = 0;
	for ( int i = 0; i < 8; i++ ) {
		for( int j = 1; j <= 4; j <<= 1 ) {
			int p = i ^ j;
			if ( i <= p ) {
				c_cubeEdges[edgesCount++] = i;
				c_cubeEdges[edgesCount++] = p;
			}
		}
	}

	for ( int i = 0; i < 256; ++i ) {
		int em = 0;
		for ( int j = 0; j < 24; j += 2) {
			bool a = ( i & ( 1 << c_cubeEdges[j] ) ) == 0;
			bool b = ( i & ( 1 << c_cubeEdges[j + 1] ) ) == 0;
			em |= ( a != b ) ? ( 1 << ( j >> 1 ) ) : 0;
		}
		c_edgeTable[i] = em;
	}
}

// surfacenets methods
SurfaceNets::SurfaceNets( FrameData *frameData )
	: CoreMesherInterface( frameData )
{
	if ( !c_tablesCreated ) {
		createTables();
		c_tablesCreated = true;
	}
}

void MarchingTets::calcMesh( amino::Math::vec3i resolution, amino::Math::bboxf bbox )
{
	printf("meshing using marching tets\n");
}

}}
