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

#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_layout.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_types.h"

// define a hasher for vec3fs
namespace std {
	template <>
		class hash<amino::Math::vec3f> {
			public :
				size_t operator() (const amino::Math::vec3f &position ) const
                {
					const size_t *x = (const size_t *) &( position[0] );
					const size_t *y = (const size_t *) &( position[1] );
					const size_t *z = (const size_t *) &( position[2] );
					size_t f = ( *x + *y * 11 - *z * 17 ) & 0x7fffffff;     // avoid problems with +-0
                    return ( f >> 22 ) ^ ( f >> 12 ) ^ ( f );
				}
		};
}

namespace Bifrost{
namespace RenderCore{

class BIFROSTRENDERAPI_DECL CoreMesherInterface {
	public:
		CoreMesherInterface ( FrameData *frameData )
			: c_frameData( frameData )
		{
			//c_sampler = new Bifrost::API::VoxelSampler( inChannel.createSampler( Bifrost::API::VoxelSamplerLinearType, Bifrost::API::SamplerSpace::TileSpace ) );
			c_sampler = new Bifrost::API::VoxelSampler( c_frameData->srcChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
            c_voxelOffset = c_frameData->srcChannel.offsetValue() / (float) c_frameData->sampleRate;
			c_exportVertexCount = 0;
		}

		~CoreMesherInterface() {
			delete c_sampler;
		}

		void createVoxelSampler();
		float getFieldValue( amino::Math::vec3f position );

		size_t getVertexCount() { return c_positions.size(); }
		void addVertexPosition( amino::Math::vec3f position ) { c_positions.push_back( position ); }
		void setVertexPosition( size_t vertexIndex, amino::Math::vec3f& position ) { c_positions[vertexIndex] = position; }
		amino::Math::vec3f getVertexPosition( size_t vertexIndex ) { return c_positions[ vertexIndex ]; }

		void setMergedVertexArraySize( size_t size ) { c_mergedPositions.reserve( size ); }
		size_t getExportVertexCount() { return c_exportVertexCount; }
		size_t getMergedVertexCount() { return  c_mergedPositions.size(); }
		void addMergedVertexPosition( amino::Math::vec3f position ) { c_mergedPositions.push_back( position ); }
		amino::Math::vec3f getMergedVertexPosition( size_t vertexIndex ) { return c_mergedPositions[ vertexIndex ]; }
		void setMergedVertexPosition( size_t vertexIndex, amino::Math::vec3f& position ) { c_mergedPositions[vertexIndex] = position; }

		void setNormalsArraySize( size_t size ) { c_normals.resize( size, amino::Math::vec3f ( 0.0f, 0.0f, 0.0f ) ); }
		void addVertexNormal( amino::Math::vec3f normal ) { c_normals.push_back( normal ); }
		amino::Math::vec3f getVertexNormal( size_t vertexIndex ) { return c_normals[ vertexIndex ]; } 
		void setVertexNormal( size_t vertexIndex, amino::Math::vec3f& normal ) { c_normals[vertexIndex] = normal; }

		void setVelocityArraySize( size_t size ) { c_velocities.resize( size, amino::Math::vec3f ( 0.0f, 0.0f, 0.0f ) ); }
		void addVertexVelocity( amino::Math::vec3f velocity ) { c_velocities.push_back( velocity ); }
		amino::Math::vec3f getVertexVelocity( size_t vertexIndex ) { return c_velocities[ vertexIndex ]; }
        void setVertexVelocity( size_t vertexIndex, const amino::Math::vec3f& velocity ) { c_velocities[vertexIndex] = velocity; }

		size_t getTriangleCount() { return c_triangles.size(); }
		void addTriangle( amino::Math::vec3i triangle ) { c_triangles.push_back( triangle ); }
		amino::Math::vec3i getTriangle( size_t triangleIndex ) { return c_triangles[ triangleIndex ]; }
		void setTriangle( size_t triangleIndex, amino::Math::vec3i& indexes ) { c_triangles[ triangleIndex ] = indexes; }

		void setPrimVarArraySizes( size_t size )
		{
			for ( size_t var = 0; var < c_frameData->primVars.size(); var++ ) {
                c_primVars[ c_frameData->primVars[var].name ] = std::vector<float>();
                c_primVars[ c_frameData->primVars[var].name ].resize( size, 0.0f );
			}
		}

		std::unordered_map< std::string, std::vector<float> > c_primVars;
		std::unordered_map<amino::Math::vec3f, size_t> c_ids;
		std::vector<size_t> c_remappedID;
		std::vector<size_t> c_exportID;
		size_t c_exportVertexCount;
		FrameData *c_frameData;

        amino::Math::vec3f c_voxelOffset;

	private:
		std::vector<amino::Math::vec3f> c_positions;
		std::vector<amino::Math::vec3f> c_mergedPositions;
		std::vector<amino::Math::vec3f> c_velocities;
		std::vector<amino::Math::vec3f> c_normals;
		std::vector<amino::Math::vec3i> c_triangles;

		Bifrost::API::VoxelSampler* c_sampler;
};

class BIFROSTRENDERAPI_DECL MarchingCubes : public CoreMesherInterface
{
	public:
        MarchingCubes( FrameData *frameData );

		~MarchingCubes() {}

        // voxelCornerPos is in scaled tile space (never use floats!)
        size_t calcVoxel(float *gridVals, amino::Math::vec3i &voxelCornerPos );

		static int c_edgeTable[256];
		static int c_triTable[256][16];
        static int c_cubeVerts[8][3];
		static int c_edgeIndex[12][2];
};

class BIFROSTRENDERAPI_DECL MarchingTets : public CoreMesherInterface
{
	public:
		void calcMesh( amino::Math::vec3i resolution, amino::Math::bboxf bbox );
};

class BIFROSTRENDERAPI_DECL SurfaceNets : public CoreMesherInterface
{
	public:
		SurfaceNets( FrameData *frameData );

		~SurfaceNets() {}

		static void createTables();

		static bool c_tablesCreated;

		static int c_cubeEdges[ 24 ];
		static int c_edgeTable[ 256 ];
};

}}
