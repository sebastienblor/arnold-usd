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

#include <vector>
#include <string>

#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_visitors.h>
#include <bifrostapi/bifrost_bifutils.h>


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

namespace Bifrost{
namespace RenderCore{

void assembleStringFromArray( Bifrost::API::StringArray inArray, Bifrost::API::String& out )
{
	out.clear();
	size_t i = 0;

	if ( inArray.count() > 0 ) {
		for ( ; i < inArray.count() - 1; i++ ) {
			out += inArray[i];
			out += ",";
		}

		out += inArray[i];
	}
}

void printStringArray( Bifrost::API::StringArray strArr )
{
	for ( int i = 0; i < strArr.count(); i++ ) {
		printf( "%s, ", strArr[ i ].c_str() );
	}
	printf("\n");
}

void printEndOutput ( const char *msg, DiagnosticsParams diagnostics )
{
	IFNOTSILENTINFUNCTION {
		printf( "\n---------------------------------------\n" );
		printf( "%s\n", msg );
		printf( "---------------------------------------\n\n" );
	}
}

void dumpStateServer( Bifrost::API::StateServer srcSS, const char *inMsg )
{
	Bifrost::API::String ssDump = srcSS.dump();
	printf("\nSTATE SERVER %s\n\n", inMsg);
	printf("%s\n\n", ssDump.c_str() );
	printf("STATE SERVER DUMP END\n\n");
}

amino::Math::vec3f getVoxelOffset ( Bifrost::API::VoxelChannel inputChan )
{
	Bifrost::API::VoxelOffsetType offsetType = inputChan.offsetType();

	switch ( offsetType ) {
		case Bifrost::API::VoxelOffsetType::Center :
			return amino::Math::vec3f( 0.5f, 0.5f, 0.5f );
			break;
		case Bifrost::API::VoxelOffsetType::XFace :
			return amino::Math::vec3f( 0.0f, 0.5f, 0.5f );
			break;
		case Bifrost::API::VoxelOffsetType::YFace :
			return amino::Math::vec3f( 0.5f, 0.0f, 0.5f );
			break;
		case Bifrost::API::VoxelOffsetType::ZFace :
			return amino::Math::vec3f( 0.5f, 0.5f, 0.0f );
			break;
		case Bifrost::API::VoxelOffsetType::XEdge :
			return amino::Math::vec3f( 0.5f, 0.0f, 0.0f );
			break;
		case Bifrost::API::VoxelOffsetType::YEdge :
			return amino::Math::vec3f( 0.0f, 0.5f, 0.0f );
			break;
		case Bifrost::API::VoxelOffsetType::ZEdge :
			return amino::Math::vec3f( 0.0f, 0.0f, 0.5f );
			break;
		case Bifrost::API::VoxelOffsetType::Corner :
			return amino::Math::vec3f( 0.0f, 0.0f, 0.0f );
			break;
		default:
			return amino::Math::vec3f( 0.0f, 0.0f, 0.0f );
			break;
	}
}

bool pointCacheStatusFromComponent( Bifrost::API::Component component )
{
	return ( component.type() == Bifrost::API::VoxelComponentType ) ? false : true;
}

bool pointCacheStatusFromFileInfo ( Bifrost::API::BIF::FileInfo fInfo )
{
	return ( fInfo.componentType == Bifrost::API::VoxelComponentType ) ? false : true;
}

void computeAeroBounds( const Bifrost::API::Channel& channel, double bboxMin[3], double bboxMax[3] )
{
	// Find the maximum depth that contains the finest tiles
	Bifrost::API::Layout layout = channel.layout();
	const Bifrost::API::TreeIndex::Depth maxDepth = (Bifrost::API::TreeIndex::Depth)layout.maxDepth();

	// Visit the finest and coarse tiles to compute a bounding box that cover all of them
	bboxMin[0] = bboxMin[1] = bboxMin[2] = std::numeric_limits<double>::max();
	bboxMax[0] = bboxMax[1] = bboxMax[2] = -std::numeric_limits<double>::max();
	{
		AeroVoxelBoundsVisitor visitor(maxDepth, channel);
		layout.traverse(visitor, Bifrost::API::ParallelReduceBreadthFirst, 0, maxDepth);

		if (visitor.m_tileBoundsMin[0] >= visitor.m_tileBoundsMax[0] ||
			visitor.m_tileBoundsMin[1] >= visitor.m_tileBoundsMax[1] ||
			visitor.m_tileBoundsMin[2] >= visitor.m_tileBoundsMax[2]) {
			return;
		}

		// Translate to world space size.
		const float scaling = layout.voxelScale();
		for (int i = 0; i < 3; i++) {
			bboxMin[i] = (float)visitor.m_tileBoundsMin[i] * scaling;
			bboxMax[i] = (float)visitor.m_tileBoundsMax[i] * scaling;
		}
	}
}

void computeIsosurfaceBounds( const Bifrost::API::Channel& channel, double bboxMin[3], double bboxMax[3] )
{
	// Find the maximum depth that contains the finest tiles
	Bifrost::API::Layout layout = channel.layout();
	const Bifrost::API::TreeIndex::Depth maxDepth = (Bifrost::API::TreeIndex::Depth)layout.maxDepth();

	// Visit the finest tiles to compute a bounding box that cover all of them
	bboxMin[0] = bboxMin[1] = bboxMin[2] = std::numeric_limits<double>::max();
	bboxMax[0] = bboxMax[1] = bboxMax[2] = -std::numeric_limits<double>::max();
	{
		TileBoundsVisitor visitor;
		layout.traverse(visitor, Bifrost::API::ParallelReduceBreadthFirst, maxDepth, maxDepth);

		if (visitor.c_tileBoundsMin[0] >= visitor.c_tileBoundsMax[0] ||
			visitor.c_tileBoundsMin[1] >= visitor.c_tileBoundsMax[1] ||
			visitor.c_tileBoundsMin[2] >= visitor.c_tileBoundsMax[2]) {
			return;
		}

		// Translate to world space size.
		const float scaling = layout.voxelScale();
		for (int i = 0; i < 3; i++) {
			bboxMin[i] = (float)visitor.c_tileBoundsMin[i] * scaling;
			bboxMax[i] = (float)visitor.c_tileBoundsMax[i] * scaling;
		}
	}
}

void computeVolumeBounds( float bbox[6], FrameData *frameData, DiagnosticsParams diagnostics )
{
	// Find the maximum depth that contains the finest tiles
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	// Visit the finest tiles to compute a bounding box that covers all of them
	bbox[0] = bbox[2] = bbox[4] = frameData->bboxSim.world.bound[0] = frameData->bboxSim.world.bound[2] = frameData->bboxSim.world.bound[4] = std::numeric_limits<float>::max();
	bbox[1] = bbox[3] = bbox[5] = frameData->bboxSim.world.bound[1] = frameData->bboxSim.world.bound[3] = frameData->bboxSim.world.bound[5] = -std::numeric_limits<float>::max();

	{
		TileBoundsVisitor2 visitor2( frameData->srcChannel );

		layout.traverse( visitor2, Bifrost::API::ParallelReduceBreadthFirst, layout.maxDepth() - 1, layout.maxDepth() - 1);

		// Translate to world space size.
		const float scaling = layout.voxelScale();

		bbox[0] = frameData->bboxSim.world.bound[0] = (float)visitor2.c_tileBoundsMin[0] * scaling;
		bbox[2] = frameData->bboxSim.world.bound[2] = (float)visitor2.c_tileBoundsMin[1] * scaling;
		bbox[4] = frameData->bboxSim.world.bound[4] = (float)visitor2.c_tileBoundsMin[2] * scaling;
		bbox[1] = frameData->bboxSim.world.bound[1] = (float)visitor2.c_tileBoundsMax[0] * scaling;
		bbox[3] = frameData->bboxSim.world.bound[3] = (float)visitor2.c_tileBoundsMax[1] * scaling;
		bbox[5] = frameData->bboxSim.world.bound[5] = (float)visitor2.c_tileBoundsMax[2] * scaling;
	}

	// calc sim min, max, dim
	frameData->bboxSim.world.calcFromBounds();

	// calc tile space bboxdata from world
	frameData->bboxSim.calcTileFromWorld();

	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			frameData->bboxSim.world.dump( "BBox Sim World" );
			frameData->bboxSim.tile.dump( "BBox Sim Tile" );
		} else {
			frameData->bboxSim.world.dumpSimple();
		}
	}
}

void computeImplicitsBounds( FrameData *frameData, InfCubeParams infCube, DiagnosticsParams diagnostics )
{
	// Find the maximum depth that contains the finest tiles
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	// Visit the finest tiles to compute a bounding box that cover all of them
	frameData->bboxSim.world.min[0] = frameData->bboxSim.world.min[1] = frameData->bboxSim.world.min[2] = std::numeric_limits<float>::max();
	frameData->bboxSim.world.max[0] = frameData->bboxSim.world.max[1] = frameData->bboxSim.world.max[2] = -std::numeric_limits<float>::max();
	{
		TileBoundsVisitor2 visitor( frameData->srcChannel );

		layout.traverse( visitor, Bifrost::API::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );

		if (visitor.c_tileBoundsMin[0] >= visitor.c_tileBoundsMax[0] ||
			visitor.c_tileBoundsMin[1] >= visitor.c_tileBoundsMax[1] ||
			visitor.c_tileBoundsMin[2] >= visitor.c_tileBoundsMax[2])
			return;

		// Translate to world space size.
		const float scaling = layout.voxelScale();
		frameData->bboxSim.world.min[0] = (float)visitor.c_tileBoundsMin[0] * scaling;
		frameData->bboxSim.world.min[1] = (float)visitor.c_tileBoundsMin[1] * scaling;
		frameData->bboxSim.world.min[2] = (float)visitor.c_tileBoundsMin[2] * scaling;
		frameData->bboxSim.world.max[0] = (float)visitor.c_tileBoundsMax[0] * scaling;
		frameData->bboxSim.world.max[1] = (float)visitor.c_tileBoundsMax[1] * scaling;
		frameData->bboxSim.world.max[2] = (float)visitor.c_tileBoundsMax[2] * scaling;
	}

	frameData->bboxSim.world.calcFromMinMax();
	frameData->bboxSim.calcTileFromWorld();

	if ( infCube.on ) {
		// calc infinite cube bounds
		frameData->bboxInf.world.bound[0] = infCube.topCenterX - infCube.dimX / 2.0f;
		frameData->bboxInf.world.bound[1] = infCube.topCenterX + infCube.dimX / 2.0f;
		frameData->bboxInf.world.bound[2] = infCube.topCenterY - frameData->bboxSim.world.dim[1];
		frameData->bboxInf.world.bound[3] = infCube.topCenterY;
		frameData->bboxInf.world.bound[4] = infCube.topCenterZ - infCube.dimZ / 2.0f;
		frameData->bboxInf.world.bound[5] = infCube.topCenterZ + infCube.dimZ / 2.0f;
	}

	frameData->bboxInf.world.calcFromBounds();
	frameData->bboxInf.calcTileFromWorld();

	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			frameData->bboxSim.world.dump( "\tBBox Sim World" );
			frameData->bboxSim.tile.dump( "\tBBox Sim Tile" );
		} else {
			frameData->bboxSim.world.dumpSimple();
		}
	}
}

void channelToNarrowBandFog ( Bifrost::API::VoxelChannel &inChannel, float narrowBandThicknessInVoxels, FrameData *frameData, bool countStats, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = inChannel.layout();
	SDFType sdfType = HALFATSURFACE;

	// go over tile levels
	Bifrost::API::TileIterator tileIterator = layout.tileIterator(	0,
																	layout.maxDepth(),
																	Bifrost::API::TraversalMode::BreadthFirst );

	// calculate narrow band thickness in world space
	float levelNarrowBandThickness = narrowBandThicknessInVoxels * frameData->orgVoxelScale;

	// go over tiles
	while ( tileIterator ) {
		// get the tile
		const Bifrost::API::Tile& tile = *tileIterator;
		Bifrost::API::TreeIndex tindex = tile.index();

		if ( !inChannel.elementCount( tindex ) ) {
			// nothing here
			continue;
		}

		Bifrost::API::TileData<float> distanceTileData = inChannel.tileData<float>( tindex );

		for ( size_t i = 0; i < distanceTileData.count(); i++ ) {
			// we do not care about coarser levels
			if ( tile.info().depth < layout.maxDepth() ) {
				distanceTileData[ i ] = 0.0f;
			} else {
					switch ( sdfType ) {
						case HALFATSURFACE:
							if ( distanceTileData[i] > levelNarrowBandThickness ) {
								distanceTileData[i] = 0.0f;
							} else {
								if (distanceTileData[i]  < -levelNarrowBandThickness) {
									distanceTileData[i] = 1.0f;
								} else {
									distanceTileData[i] = clip ( (0.5f - 0.5f * distanceTileData[i] / levelNarrowBandThickness) , 0.0f, 1.0f );
								}
							}
							break;
						case SHELL01:
							if ( distanceTileData[i] > levelNarrowBandThickness ) {
								distanceTileData[i] = 0.0f;
							} else {
								if (distanceTileData[i] < -2 * levelNarrowBandThickness) {
									distanceTileData[i] = 0.0f;
								} else if (distanceTileData[i] < -levelNarrowBandThickness) {
									distanceTileData[i] = clip ( 2 + distanceTileData[i] / levelNarrowBandThickness , 0.0f, 1.0f );
								} else {
									distanceTileData[i] = clip ( (0.5f - 0.5f * distanceTileData[i] / levelNarrowBandThickness) , 0.0f, 1.0f );
								}
							}
							break;
						case SHELL:
							if ( distanceTileData[i] > levelNarrowBandThickness ) {
								distanceTileData[i] = 0.0f;
							} else {
								if (distanceTileData[i] < -levelNarrowBandThickness) {
									distanceTileData[i] = 0.0f;
								} else if (distanceTileData[i] >= 0.0f ) {
									distanceTileData[i] = clip ( 0.5f - 0.5f * distanceTileData[i] / levelNarrowBandThickness , 0.0f, 0.5f );
								} else {
									distanceTileData[i] = clip ( 0.5f + 0.5f * distanceTileData[i] / levelNarrowBandThickness , 0.0f, 0.5f );
								}
							}
							break;
						default:
							// don't touch the data
							break;
					} // end of switch
				//}
			} // end of level check
		} // end of tiledata loop

		++tileIterator;
	} // end of tile iterator

	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 2 && countStats ) {
			for (int i = 0; i <= 7; i++) {
				printf("%d: total:%d cleared:%d\n", i, frameData->bifInfo.tilesWithDataCount[i], frameData->bifInfo.tilesWithChildTiles[i]);
			}
		}
	}
}

void deleteObject( Bifrost::API::StateServer& stateServer, const Bifrost::API::Object& object )
{
	const Bifrost::API::RefArray& componentArray =  object.components();

	for( size_t componentIndex = 0; componentIndex < componentArray.count(); ++componentIndex) {
		const Bifrost::API::Component& component = componentArray[componentIndex];
		const Bifrost::API::RefArray& channelArray = component.channels();

		for( size_t channelIndex = 0; channelIndex < channelArray.count(); ++channelIndex) {
			const Bifrost::API::Channel& channel = channelArray[channelIndex];

			stateServer.removeChannel(channel.fullPathName());
		}

		stateServer.removeLayout( Bifrost::API::Base(component.layout()).name() );
		stateServer.removeComponent(component.name());
	}

	stateServer.removeObject(object.name());
}

void clearStateServer( Bifrost::API::StateServer inSS )
{
	Bifrost::API::RefArray objects = inSS.objects();

	for ( int i = 0; i < objects.count(); i++ ) {
		Bifrost::API::Object obj = objects[i];
		deleteObject( inSS, obj );
	}

	inSS.clear();
}

unsigned long hash( const uint8_t *bufPtr, unsigned long range )
{
    unsigned long hash = 5381;
    int c;

    while (c = *bufPtr++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return imod(hash, range);
}

unsigned long imod( const unsigned long a, const unsigned long b )
{
    unsigned long n = a/b;
    return (a - n*b);
}

std::string toString ( unsigned long a )
{
//	int length = snprintf( NULL, 0, "%lu", a );
//	assert( length > 0 );
//	char *buffer = new char[ length + 1 ];
	char *buffer = new char[ 256 ];
//	snprintf ( buffer, length + 1, "%lu", a );
	sprintf ( buffer, "%lu", a );
	std::string str ( buffer );
	delete[] buffer;
	return str;
}

std::string toStringPtr ( uintptr_t a )
{
	//int length = snprintf( NULL, 0, "%" PRIuPTR, a );
	//assert( length > 0 );
	//char *buffer = new char[ length + 1 ];
	char *buffer = new char[ 256 ];
	//snprintf ( buffer, length + 1, "%" PRIuPTR, a );
	sprintf ( buffer, "%Iu", a );
	std::string str ( buffer );
	delete[] buffer;
	return str;
}

// allocate arrays for min max data
void allocMinMaxArray(float **min, float **max, int i, int nofTiles)
{
	min[i] = (float *) malloc( nofTiles * sizeof(float) );
	max[i] = (float *) malloc( nofTiles * sizeof(float) );
	for (int tile = 0; tile < nofTiles; tile++) {
		min[i][tile] = 0.0f;
		max[i][tile] = 0.0f;
	}
}

// calc array structures for min max data
void calcMinMaxArray(Bifrost::API::Layout& layout, Bifrost::API::VoxelChannel& channel, float **min, float **max, DiagnosticsParams diagnostics)
{
	Bifrost::API::TileIterator tileIterator;
	for (int depth = layout.maxDepth(); depth >= 0; depth--) {
		if ( diagnostics.DEBUG >= 3 ) {
			printf("loop level %d\n",depth);
		}
		tileIterator = layout.tileIterator( depth, depth, Bifrost::API::TraversalMode::BreadthFirst );
		while (tileIterator) {
			// init min max
			float tileMin = std::numeric_limits<float>::max();
			float tileMax = -std::numeric_limits<float>::max();

			// get the tile
			const Bifrost::API::Tile& tile = *tileIterator;
			// Get tile info to access children
			Bifrost::API::TileInfo info = tile.info(); 
			int tileIndex = tile.index().tile;

			// go over tiles and store data
			int count =0;
			if ( info.hasChildren || depth == layout.maxDepth() ) {
				if ( depth == layout.maxDepth() ) {
					if (tileIndex == 0 && diagnostics.DEBUG >= 3) {
						printf("proc level 7\n");
					}

					// we are at the finest level go through the voxels and calc min max

					// get tile data
					Bifrost::API::TileData<float> channelTileData = channel.tileData<float>( tile.index() );
					// calc min max
					for ( size_t i = 0; i < channelTileData.count(); i++ ) {
						tileMin = std::min ( tileMin, channelTileData[i] );
						tileMax = std::max ( tileMax, channelTileData[i] );
					}
					( (float *) min[depth] )[tileIndex] = tileMin;
					( (float *) max[depth] )[tileIndex] = tileMax;
				} else {
					if (tileIndex == 0 && diagnostics.DEBUG >= 3) {
						printf("proc level %d\n", tile.index().depth);
					}
					// we are at coarser levels
					// get values from the child tiles and calc a min max
					for (int i=0; i < info.dimInfo.tileWidth; i++ ) {
						for (int j=0; j < info.dimInfo.tileWidth; j++ ) {
							for (int k=0; k < info.dimInfo.tileWidth; k++ ) {
								Bifrost::API::TreeIndex childIndex = tile.child( i, j, k );
								if ( childIndex.valid() ) {
									count++;
									tileMin = std::min ( tileMin, ( (float *) min[childIndex.depth] )[childIndex.tile] );
									tileMax = std::max ( tileMax, ( (float *) max[childIndex.depth] )[childIndex.tile] );
									( (float *) min[depth] )[tileIndex] = tileMin;
									( (float *) max[depth] )[tileIndex] = tileMax;
									if (depth == 2 && diagnostics.DEBUG >= 3) {
										printf("%d,%d:%f %f   %d,%d:%f %f\n", childIndex.tile, childIndex.depth,
											( (float *) min[childIndex.depth] )[childIndex.tile],
											( (float *) max[childIndex.depth] )[childIndex.tile],
											tileIndex, depth,
											( (float *) min[depth] )[tileIndex],
											( (float *) max[depth] )[tileIndex] );
									}
								}
							}
						}
					}
				}

				if (count > 0 && diagnostics.DEBUG >= 3) {
					printf("Tile %d,%d has %d children and min: %f max:%f\n", tileIndex, depth, count, tileMin, tileMax);
				}
			} // end children check
			++tileIterator;
		}
	}
}

void calcHullCornerCoords( CvPoint corners[8], int hullCount, float screenWindow[4], void xformCB( const char *, const char *, int, CvPoint *, int) )
{
	float offset = 0.05f;

	// NEAR
 	// left top
 	corners[0][0] = screenWindow[0] - offset;
	corners[0][1] = screenWindow[2] + offset;
	corners[0][2] = 0.0f;

 	// right top
 	corners[1][0] = screenWindow[1] + offset;
	corners[1][1] = screenWindow[2] + offset;
	corners[1][2] = 0.0f;

 	// right bottom
 	corners[2][0] = screenWindow[1] + offset;
	corners[2][1] = screenWindow[3] - offset;
	corners[2][2] = 0.0f;

 	// left bottom
 	corners[3][0] = screenWindow[0] - offset;
	corners[3][1] = screenWindow[3] - offset;
	corners[3][2] = 0.0f;

	// FAR
 	// left top
 	corners[4][0] = screenWindow[0] - offset;
	corners[4][1] = screenWindow[2] + offset;
	corners[4][2] = 1.0f;

 	// right top
 	corners[5][0] = screenWindow[1] + offset;
	corners[5][1] = screenWindow[2] + offset;
	corners[5][2] = 1.0f;

 	// right bottom
 	corners[6][0] = screenWindow[1] + offset;
	corners[6][1] = screenWindow[3] - offset;
	corners[6][2] = 1.0f;

 	// left bottom
 	corners[7][0] = screenWindow[0] - offset;
	corners[7][1] = screenWindow[3] - offset;
	corners[7][2] = 1.0f;

	xformCB( (const char *) "screen", (const char *) "object", hullCount , corners, 0 );
}

void calcHullNormals( CvPoint hullCorners[8], FrustumPlane planes[6] )
{
	// calc normal for all 6 planes
	amino::Math::vec3f normal, vec1, vec2, pointOnPlane;

	// near plane
	vec1[0] = hullCorners[1][0] - hullCorners[0][0];
	vec1[1] = hullCorners[1][1] - hullCorners[0][1];
	vec1[2] = hullCorners[1][2] - hullCorners[0][2];
	vec2[0] = hullCorners[3][0] - hullCorners[0][0];
	vec2[1] = hullCorners[3][1] - hullCorners[0][1];
	vec2[2] = hullCorners[3][2] - hullCorners[0][2];
	planes[0].normal = amino::Math::cross ( vec2, vec1);
	planes[0].normalMag = sqrt( amino::Math::dot ( planes[0].normal, planes[0].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[0][0];
	pointOnPlane[1] = hullCorners[0][1];
	pointOnPlane[2] = hullCorners[0][2];
	planes[0].planeD = -1 * amino::Math::dot ( planes[0].normal, pointOnPlane );

	// left plane
	vec1[0] = hullCorners[0][0] - hullCorners[4][0];
	vec1[1] = hullCorners[0][1] - hullCorners[4][1];
	vec1[2] = hullCorners[0][2] - hullCorners[4][2];
	vec2[0] = hullCorners[7][0] - hullCorners[4][0];
	vec2[1] = hullCorners[7][1] - hullCorners[4][1];
	vec2[2] = hullCorners[7][2] - hullCorners[4][2];
	planes[1].normal = amino::Math::cross ( vec2, vec1);
	planes[1].normalMag = sqrt( amino::Math::dot ( planes[1].normal, planes[1].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[0][0];
	pointOnPlane[1] = hullCorners[0][1];
	pointOnPlane[2] = hullCorners[0][2];
	planes[1].planeD = -1 * amino::Math::dot ( planes[1].normal, pointOnPlane );

	// right plane
	vec1[0] = hullCorners[5][0] - hullCorners[1][0];
	vec1[1] = hullCorners[5][1] - hullCorners[1][1];
	vec1[2] = hullCorners[5][2] - hullCorners[1][2];
	vec2[0] = hullCorners[2][0] - hullCorners[1][0];
	vec2[1] = hullCorners[2][1] - hullCorners[1][1];
	vec2[2] = hullCorners[2][2] - hullCorners[1][2];
	planes[2].normal = amino::Math::cross ( vec2, vec1);
	planes[2].normalMag = sqrt( amino::Math::dot ( planes[2].normal, planes[2].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[1][0];
	pointOnPlane[1] = hullCorners[1][1];
	pointOnPlane[2] = hullCorners[1][2];
	planes[2].planeD = -1 * amino::Math::dot ( planes[2].normal, pointOnPlane );

	// far plane
	vec1[0] = hullCorners[4][0] - hullCorners[5][0];
	vec1[1] = hullCorners[4][1] - hullCorners[5][1];
	vec1[2] = hullCorners[4][2] - hullCorners[5][2];
	vec2[0] = hullCorners[6][0] - hullCorners[5][0];
	vec2[1] = hullCorners[6][1] - hullCorners[5][1];
	vec2[2] = hullCorners[6][2] - hullCorners[5][2];
	planes[3].normal = amino::Math::cross ( vec2, vec1);
	planes[3].normalMag = sqrt( amino::Math::dot ( planes[3].normal, planes[3].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[4][0];
	pointOnPlane[1] = hullCorners[4][1];
	pointOnPlane[2] = hullCorners[4][2];
	planes[3].planeD = -1 * amino::Math::dot ( planes[3].normal, pointOnPlane );

	// top plane
	vec1[0] = hullCorners[4][0] - hullCorners[0][0];
	vec1[1] = hullCorners[4][1] - hullCorners[0][1];
	vec1[2] = hullCorners[4][2] - hullCorners[0][2];
	vec2[0] = hullCorners[1][0] - hullCorners[0][0];
	vec2[1] = hullCorners[1][1] - hullCorners[0][1];
	vec2[2] = hullCorners[1][2] - hullCorners[0][2];
	planes[4].normal = amino::Math::cross ( vec2, vec1);
	planes[4].normalMag = sqrt( amino::Math::dot ( planes[4].normal, planes[4].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[0][0];
	pointOnPlane[1] = hullCorners[0][1];
	pointOnPlane[2] = hullCorners[0][2];
	planes[4].planeD = -1 * amino::Math::dot ( planes[4].normal, pointOnPlane );

	// bottom plane
	vec1[0] = hullCorners[2][0] - hullCorners[3][0];
	vec1[1] = hullCorners[2][1] - hullCorners[3][1];
	vec1[2] = hullCorners[2][2] - hullCorners[3][2];
	vec2[0] = hullCorners[7][0] - hullCorners[3][0];
	vec2[1] = hullCorners[7][1] - hullCorners[3][1];
	vec2[2] = hullCorners[7][2] - hullCorners[3][2];
	planes[5].normal = amino::Math::cross ( vec2, vec1);
	planes[5].normalMag = sqrt( amino::Math::dot ( planes[5].normal, planes[5].normal ) );
	//amino::Math::normalize ( normal );
	pointOnPlane[0] = hullCorners[3][0];
	pointOnPlane[1] = hullCorners[3][1];
	pointOnPlane[2] = hullCorners[3][2];
	planes[5].planeD = -1 * amino::Math::dot ( planes[5].normal, pointOnPlane );
}

bool boundIntersectsWithFrustum ( const CvPoint corners[8], FrustumPlane planes[6] )
{
	// given a normal for each face of the frustum
	// do a dot product of normal vector and the position we are given
	// if it is negative we are inside the frustum
	// 
	// if we have at least one point inside we return true
	// if all is outside we return false

	bool result = false;
	for ( int corner = 0; corner < 8; corner++ ) {
		amino::Math::vec3f pos;

		// keep track of outside status
		bool outside = false;

		// position vector
		pos[0] = corners[ corner ][ 0 ];
		pos[1] = corners[ corner ][ 1 ];
		pos[2] = corners[ corner ][ 2 ];

		for ( int plane = 0; plane < 6; plane++ ) {
			// calc signed distance to plane
			// since we have a normalized normal vector distance calc simplifies down to N.P + planeD
			float distance = ( amino::Math::dot ( planes[ plane ].normal, pos ) + planes[ plane ].planeD ) / planes[ plane ].normalMag;
			
			// if signed distance is positive, since we have normals pointing outwards, the point is outside
			if ( distance > 0.0f ) {
				// this point is outside of at least one plane -> so it is outside of frustum
				outside = true;
				break;
			}
		}

		// if outside is false here that means the point we last processed is inside
		// as one point is inside the frustum, our bounds is intersecting with the frustum
		// so we can bail out here
		if ( outside == false ) {
			result = true;
			break;
		}
	}

	// nothing is inside the frustum so return false
	return result;
}

bool pointInFrustum ( amino::Math::vec3f wp, FrustumPlane planes[6] )
{
	bool result = true;

	for ( int plane = 0; plane < 6; plane++ ) {
		// calc signed distance to plane
		float distance = ( amino::Math::dot ( planes[ plane ].normal, wp ) + planes[ plane ].planeD ) / planes[ plane ].normalMag;
			
		// if signed distance is positive, since we have normals pointing outwards, the point is outside
		if ( distance > 0.0f ) {
			// this point is outside of at least one plane -> so it is outside of frustum
			result = false;
			break;
		}
	}

	return result;
}

float calcPositionFalloff ( amino::Math::vec3f pos, FrameData *frameData )
{
	// we will calc a rectangular falloff
	float blendX = fabs ( 2 * ( pos[ frameData->presenceFalloffAxises[0] ] - frameData->bboxSim.world.center[ frameData->presenceFalloffAxises[0] ] ) );
	float blendZ = fabs ( 2 * ( pos[ frameData->presenceFalloffAxises[1] ] - frameData->bboxSim.world.center[ frameData->presenceFalloffAxises[1] ] ) );

	// take correctionfactor into account
	if ( frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[1] ] > frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[0] ] ) {
		// z is greater than x
		blendX /= frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[0] ];
		blendZ = std::max(0.0f, blendZ - frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[1] ] + frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[0] ]);
		blendZ /= frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[0] ];
	} else {
		// x is greater than z
		blendX = std::max(0.0f, blendX - frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[0] ] + frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[1] ]);
		blendX /= frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[1] ];
		blendZ /= frameData->bboxSim.world.dim[ frameData->presenceFalloffAxises[1] ];
	}

	// calc and return finalblend val
	return clip<float> ( std::max( blendX, blendZ ), 0.0f, 1.0f);
}

float calcCullingDepth ( amino::Math::vec3f pos, FrameData *frameData, CullSidesParams cullSides )
{
	float result = -cullSides.depthAtStartInVoxels;

	// calc a falloff depending on distance from sim center
	float falloff = calcPositionFalloff ( pos, frameData );

	// linearly interpolate from cull depth of cullDepthAtStart to zero
	if ( falloff >= cullSides.start ) {
		// if we are at a position closer than falloffend
		if ( falloff <= cullSides.end ) {
			result *= 1.0f - (falloff - cullSides.start) / cullSides.range;
		} else {
			result = 0.0f;
		}
	}

	return result;
}

Bifrost::API::String writeHotDataToDisk(	CoreObjectUserData& objectRef,
											Bifrost::API::String filename,
											Bifrost::API::String componentName,
											Bifrost::API::String& writeToFolder )
{
	// declare State Server
    DUMP("AAAAAAAAAAAAAAAAAA");
    Bifrost::API::StateServer hotServer( objectRef.stateServer() );

    DUMP(objectRef.file());
    DUMP(filename);

    writeToFolder = Bifrost::API::File::createTempFolder();
	writeToFolder.trimRight( "/" );

	Bifrost::API::String filenameOnly;
	Bifrost::API::File::extractFilename( Bifrost::API::String( filename ), filenameOnly );
    filenameOnly = Bifrost::API::String("/").append(filenameOnly);
    Bifrost::API::String writeToFile = Bifrost::API::String(writeToFolder).append(filenameOnly);

    { // REMOVE
        Bifrost::API::String hotFile = "/home/beauchc/bifrost_debug/tmp/";
        hotFile += filenameOnly;
        Bifrost::API::ObjectModel OM;
        Bifrost::API::FileIO fio = OM.createFileIO( hotFile );
        Bifrost::API::Component component = hotServer.findComponent( componentName );
        fio.save(component, Bifrost::API::BIF::Compression::Level0, 0 );
    }
    Bifrost::API::ObjectModel OM;
    Bifrost::API::FileIO fio = OM.createFileIO( writeToFile );
    Bifrost::API::Component component = hotServer.findComponent( componentName );

    fio.save(component, Bifrost::API::BIF::Compression::Level0, 0 );
    DUMP(fio.result().status == Bifrost::API::BIF::JobResult::Success);

    DUMP(writeToFile);

    return writeToFile;
}

void initAndGetFrameData(	FrameData *frameData,
							void *inData,
							char *bifFilename,
							char *inputChannelName,
							char *primVarNames,
							char *smoothingChannelName,
							char *infCubeBlendChannelName,
							BifrostRenderType renderType,
							DiagnosticsParams diagnostics,
							void getRenderDataCB( void *, void *, PluginType ) )
{
	// define an offset for cutting inwards to get rid of side faces
	frameData->offsetSize = 0.4f;

    // get information from data
	Bifrost::API::ObjectModel OM;
	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes( bifFilename );
	Bifrost::API::FileIO fio = OM.createFileIO( correctedFilename );
	Bifrost::API::BIF::FileInfo fInfo = fio.info();

	getMetaDataFlags( fio, frameData );

	// let's get avaliable channel names
	Bifrost::API::StringArray inputChannelNames;
	Bifrost::API::Int32Array inputChannelTypes;

	if ( diagnostics.DEBUG > 0 ) {
		printf("\nAvaliable channels:\n");
	}

	for ( int i = 0; i < fInfo.channelCount; i++ ) {
		Bifrost::API::Status status = inputChannelNames.addUnique( fio.channelInfo(i).name );

		// if we added the channel add its type too
		if ( status == Bifrost::API::Status::Success ) {
			inputChannelTypes.add ( fio.channelInfo(i).dataType );

			if ( diagnostics.DEBUG > 0 ) {
				printf("\t%s, %d\n", inputChannelNames[i].c_str(), inputChannelTypes[i]);
			}
		}

	}

	frameData->isPointCache = pointCacheStatusFromFileInfo ( fInfo );

	// check for inputChannelName in the channels list
	// if it is a point cache we may have that channel post particle->voxel conversion
	size_t channelExists = inputChannelNames.findFirstByName( inputChannelName );
	if ( !frameData->isPointCache && ( channelExists == inputChannelNames.npos ) ) {
		printf( "No valid channel found with the name: %s\nPlease check!\n", inputChannelName );
		frameData->error = true;
		return;
	}

	// check number of tiles and elements
	if ( channelExists != inputChannelNames.npos ) {
		if ( fio.channelInfo( channelExists ).tileCount == 0 ) {
			printf( "No tiles in file Diego! Please check!\n" );
			frameData->error = true;
			return;
		} else {
			if ( fio.channelInfo( channelExists ).elementCount == 0 ) {
				printf( "No elements in file Diego! Please check!\n" );
				frameData->error = true;
				return;
			}
		}
	}

	// so we have the channel for lookup
	// now check all the required channels and assemble a loadChannelNames list
	Bifrost::API::String inputPrimVars ( primVarNames );
	frameData->primVarNames = inputPrimVars.split(",");

	// first lets check whether we have speed specified anywhere
	// if yes check existence of velocity and if it is there add corresponding channels to the loadChannelNames
	if (	( inputChannelName != NULL && strcmp ( inputChannelName, "speed" ) == 0 ) ||
			( smoothingChannelName != NULL && strcmp ( smoothingChannelName, "speed" ) == 0 ) ||
			( infCubeBlendChannelName != NULL && strcmp ( infCubeBlendChannelName, "speed") == 0 ) ||
			frameData->primVarNames.findFirstByName("speed") != inputChannelNames.npos ) {

		// we need speed channel
		frameData->speedNeeded = true;

		// check existence of velocity channels

		// check what type of component we are processing
		if ( fInfo.componentType == Bifrost::API::VoxelComponentType ) {
			channelExists = inputChannelNames.findFirstByName( "velocity_u" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_u" );
			} else {
				printf("Can not find velocity_u channel for speed calculation.\n");
				frameData->error = true;
				frameData->speedNeeded = false;
				return;
			}

			channelExists = inputChannelNames.findFirstByName( "velocity_v" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_v" );
			} else {
				printf("Can not find velocity_v channel for speed calculation.\n");
				frameData->error = true;
				frameData->speedNeeded = false;
				return;
			}

			channelExists = inputChannelNames.findFirstByName( "velocity_w" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_w" );
			} else {
				printf("Can not find velocity_w channel for speed calculation.\n");
				frameData->error = true;
				frameData->speedNeeded = false;
				return;
			}
		} else if ( fInfo.componentType == Bifrost::API::PointComponentType ) {
			channelExists = inputChannelNames.findFirstByName( "velocity" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity" );
			} else {
				printf("Can not find velocity channel for speed calculation.\n");
				frameData->error = true;
				frameData->speedNeeded = false;
				return;
			}
		} else {
			printf("Can not determine cache type for speed channel creation! Please check your bif file\n");
			frameData->error = true;
			frameData->speedNeeded = false;
			return;
		}
	}

	// now process primvars list
	// this should be done before adding channels for various inputs as otherwise primvars list would not be correct
	if ( diagnostics.DEBUG > 0 ) {
		printf("\nProcessing PRIMVARS List...\n");
	}

	// check primVars for matches
	for ( int i = 0; i < frameData->primVarNames.count(); i++) {
		if ( frameData->primVarNames[i] == "speed" ) {
			// speed need to be there for all cases
			frameData->primVarTypes.add ( Bifrost::API::FloatType );
			frameData->maxElementCount = std::max<size_t> ( frameData->maxElementCount, 1 );
		} else {
			size_t channelIndex = inputChannelNames.findFirstByName( frameData->primVarNames[i] );

			if ( channelIndex != inputChannelNames.npos ) {
				Bifrost::API::Status status = frameData->loadChannelNames.addUnique( frameData->primVarNames[i] );

				// if we added something to the list do other things
				if ( status == Bifrost::API::Status::Success ) {
					frameData->primVarTypes.add ( inputChannelTypes[channelIndex] );

					// flag uv
					// we calc max elements in channels if it is not uv as it will be separated later in BifrostImplicits
					if ( frameData->primVarNames[i] == "uv" ) {
						frameData->uvNeeded = true;
					} else {
						frameData->maxElementCount = std::max<size_t> ( frameData->maxElementCount, getPrimVarElementCount ( frameData->primVarTypes[i] ) );
					}
				}
			} else {
				if ( diagnostics.DEBUG > 0 ) {
					if ( frameData->primVarNames[i] != "none" ) {
						printf( "\tCan not find primVar %s in file, skipping!\n", frameData->primVarNames[i].c_str() );
					}
				}
				frameData->primVarNames.remove( i );
				i--;
			}
		}
	}

	if ( renderType == IMPLICITSURFACE && frameData->presenceNeeded ) {
		// we need presence for single sided surface
		frameData->primVarNames.add ( "presence" );
		frameData->primVarTypes.add ( Bifrost::API::FloatType );
		frameData->maxElementCount = std::max<size_t> ( frameData->maxElementCount, 1 );
	}

	// if we need uv we need to remove uv from the prim var list and add u and v to the list
	// because uv is added to the load channel list above, uv channel is loaded in BifrostImplicits
	// and then it gets separated into u and v channels
	if ( frameData->uvNeeded ) {
		// first find the index for uv
		size_t uvIndex = frameData->primVarNames.findFirstByName( "uv" );

		// remove it from both arrays
		frameData->primVarNames.remove( uvIndex );
		frameData->primVarTypes.remove( uvIndex );

		// now add u and v
		frameData->primVarNames.add ( "u1" );
		frameData->primVarTypes.add ( Bifrost::API::FloatType );
		frameData->primVarNames.add ( "v1" );
		frameData->primVarTypes.add ( Bifrost::API::FloatType );

		frameData->maxElementCount = std::max<size_t> ( frameData->maxElementCount, getPrimVarElementCount ( frameData->primVarTypes.last() ) );
	}

	// print out primVars
	if ( diagnostics.DEBUG > 0 ) {
		if (frameData->primVarNames.count() > 0 ) {
			printf("\n\tFinal list of Primvars to Export:\n");
			for (int i = 0; i < frameData->primVarNames.count(); i++) {
				printf("\t\t%s, %d\n", frameData->primVarNames[i].c_str(), int( frameData->primVarTypes[i] ) );
			}
		} else {
			printf ( "\tNo primvars found to export!\n" );
		}
	}

	if ( diagnostics.DEBUG > 0 ) {
		printf( "\n\tMaxElementCount: %d\n\n", frameData->maxElementCount );
	}

	//
	// now process all the input data
	//

	// add lookup channel
	channelExists = inputChannelNames.findFirstByName( inputChannelName );
	if ( channelExists != inputChannelNames.npos ) {
		frameData->loadChannelNames.addUnique( inputChannelName );
	}

	// add smoothingChannel if specified and it exists
	channelExists = inputChannelNames.findFirstByName( smoothingChannelName );
	if ( channelExists != inputChannelNames.npos ) {
		frameData->loadChannelNames.addUnique( smoothingChannelName );
		frameData->smoothChannelExists = true;
	}

	// add infCubeBlendChannel if specified and it exists
	if ( renderType == IMPLICITSURFACE ) {
		channelExists = inputChannelNames.findFirstByName( infCubeBlendChannelName );
		if ( channelExists != inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( infCubeBlendChannelName );
			frameData->infCubeBlendChannelExists = true;
		}
	}

	//
	// now add cache type specific channels to the loadChannelNames
	//
	if ( fInfo.componentType == Bifrost::API::PointComponentType ) {
		// this is a particle cache
		channelExists = inputChannelNames.findFirstByName( "position" );
		if ( channelExists != inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( "position" );
		}

		// if we need id64 load that as well
		if ( renderType == VOLUME ) {
			channelExists = inputChannelNames.findFirstByName( "id64" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "id64" );
			}
		}

		if ( renderType == IMPLICITSURFACE ) {
			channelExists = inputChannelNames.findFirstByName( "droplet" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "droplet" );
			}
		}
	}

	if ( renderType == IMPLICITSURFACE ) {
		// airDistance is always needed
		channelExists = inputChannelNames.findFirstByName( "airDistance" );
		if ( channelExists != inputChannelNames.npos ) {
			frameData->loadChannelNames.addUnique( "airDistance" );
		}
	}

	// check whether motion blur is on and if it is, export velocity channel(s)
	// get shutter times to see whether we are exporting motion blur or not
	frameData->motionBlur = false;
	frameData->velocityExists = true;

	getRenderDataCB ( (void *) frameData, (void *) inData, frameData->pluginType );

	// check velocity
	if ( frameData->shutter[1] > frameData->shutter[0] ) {
		frameData->motionBlur = true;

		// we need to export motion blur do we have velocity in bif?
		if ( fInfo.componentType == Bifrost::API::VoxelComponentType ) {
			channelExists = inputChannelNames.findFirstByName( "velocity_u" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_u" );
			} else {
				frameData->velocityExists = false;
			}

			channelExists = inputChannelNames.findFirstByName( "velocity_v" );
			if ( frameData->velocityExists && channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_v" );
			} else {
				frameData->velocityExists = false;
			}

			channelExists = inputChannelNames.findFirstByName( "velocity_w" );
			if ( frameData->velocityExists && channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity_w" );
			} else {
				frameData->velocityExists = false;
			}
		} else {
			// this is a particle cache
			channelExists = inputChannelNames.findFirstByName( "velocity" );
			if ( channelExists != inputChannelNames.npos ) {
				frameData->loadChannelNames.addUnique( "velocity" );
			} else {
				frameData->velocityExists = false;
			}
		}
	} 

	if ( frameData->motionBlur ) {
		if ( frameData->velocityExists ) {
			frameData->motionBlur = true;
		} else {
			frameData->motionBlur = false;
		}	
	} else {
		frameData->motionBlur = false;
	}

	//
	// we are done with channel processing so output results
	//

	// print out loadChannelNames
	if ( diagnostics.DEBUG > 0 ) {
		if (frameData->loadChannelNames.count() > 0 ) {
			printf("\n\tFinal list of channels to load:\n");
			for (int i = 0; i < frameData->loadChannelNames.count(); i++) {
				printf("\t\t%s\n", frameData->loadChannelNames[i].c_str() );
			}
		}
	}
}



void concatChannelNames( Bifrost::API::StringArray channelNames, char *concat )
{
	concat = (char *) malloc ( 1024 * sizeof( char ) ); // alloc big
	for (int i = 0; i < channelNames.count(); i++) {
		if ( i != 0 ) {
			strcat ( concat, ",");
		}
		strcat ( concat, channelNames[i].c_str() );
	}
}

Bifrost::API::Component splatParticlesToVoxels( Bifrost::API::Object inObject,
												Bifrost::API::Component inComponent,
												VolumeInputData *inData,
												Bifrost::API::StateServer& srcSS,
												CvFloat calcNoiseCB ( CvPoint3) )
{
	// get common channels
	const Bifrost::API::Channel& posChannel = inComponent.findChannel( Bifrost::API::String( "position" ) );
	const Bifrost::API::Channel& inputChannel = inComponent.findChannel( Bifrost::API::String( inData->inputChannelName ) );
	const Bifrost::API::Channel& velChannel = inComponent.findChannel( Bifrost::API::String( "velocity" ) );
	const Bifrost::API::Channel& idChannel = inComponent.findChannel( Bifrost::API::String( "id64" ) );

	bool idExists = false;
	if ( idChannel.valid() ) {
		idExists = true;
	}

	Bifrost::API::RefArray channels = inComponent.channels();
	Bifrost::API::Layout layout = inComponent.layout();

	// if id channel is valid and there is skip
	// we need to figure out number of particles we will export
	size_t nofFinalParticles = 0;
	size_t nofSplatParticles = 0;
	size_t nofParticles = posChannel.elementCount( layout.maxDepth() );

	// get number of final particles
	if ( idExists && inData->skip > 1 ) {
		CalcNofParticlesVisitor nofParticlesVisitor ( idChannel, inData->skip );
		layout.traverse(nofParticlesVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );
		nofFinalParticles = nofParticlesVisitor.m_nofParticles;
	} else {
		nofFinalParticles = inData->skip > 1 ? ( 1 + nofParticles / inData->skip ) : nofParticles;
	}

	nofSplatParticles = nofFinalParticles * inData->splatSamples;

	bool isMultiPointing = inData->splatSamples > 1 ? true : false;
	IFNOTSILENT {
		printf( "\nConverting particles to voxels by splatting...\n" );
		if (isMultiPointing) {
			printf("\tWe will splat %d total elements for %d original elements...\n", nofSplatParticles, nofFinalParticles );	
		} else {
			if ( inData->skip > 1 ) {
				printf("\tWe will splat %d elements after skipping...\n", nofSplatParticles );
			} else {
				printf("\tWe will splat %d elements...\n", nofSplatParticles );
			}
		}
	}

	// create a new object model
	Bifrost::API::ObjectModel outOM;

	// create state server
	Bifrost::API::StateServer outSS = outOM.createStateServer();

	// create top object
	Bifrost::API::Object outObject = outSS.createObject("Particle2Voxel", "voxel_splat");
	Bifrost::Math::Similarity inWorld = inObject.world();
	outObject.setWorld ( inWorld );

	// create a component
	Bifrost::API::Component outComponent = outSS.createComponent( Bifrost::API::VoxelComponentType, "voxel_splat-volume", layout.voxelScale() );

	// get layout
	Bifrost::API::Layout outLayout = outComponent.layout();

	// add it to the main object
	outObject.addComponent(outComponent);

	//
	//
	// CREATE CHANNELS CORRESPONDING TO LOADED CHANNELS AS THEY ARE MARKED NEEDED BY THE PRELOADER
	// DONT CREATE CHANNELS FOR POSITION AND ID64
	//
	//
	for ( size_t i = 0; i < channels.count(); i++ ) {
		if ( Bifrost::API::Base(channels[i]).name() != "position" && Bifrost::API::Base(channels[i]).name() != "id64" ) {
			if ( Bifrost::API::Base( channels[i] ).name() == "velocity" ) {
				// create 3 channels to hold velocity vector
				outSS.createChannel( outComponent, Bifrost::API::FloatType, "velocity_u" );
				outSS.createChannel( outComponent, Bifrost::API::FloatType, "velocity_v" );
				outSS.createChannel( outComponent, Bifrost::API::FloatType, "velocity_w" );
			} else if ( Bifrost::API::Base( channels[i] ).name() == "uv" ) {
				outSS.createChannel( outComponent, Bifrost::API::FloatType, "u1" );
				outSS.createChannel( outComponent, Bifrost::API::FloatType, "v1" );
			} else {
				outSS.createChannel( outComponent, Bifrost::API::FloatType, Bifrost::API::Base( channels[i] ).name() );
			}
		}
	}

	// create an extra channel for keeping track of number of samples in a voxel
	Bifrost::API::VoxelChannel nofSamplesChannel = outSS.createChannel( outComponent, Bifrost::API::UInt64Type, "nofSamples" );

	// get output channel corresponding to input channel
	Bifrost::API::VoxelChannel outInputChannel = outComponent.findChannel( Bifrost::API::String( inData->inputChannelName ) );

	// keep track off output channels
	Bifrost::API::RefArray outChannels = outComponent.channels();

	// set values for channels
	for ( size_t i = 0; i < outChannels.count(); i++ ) {
		switch ( Bifrost::API::VoxelChannel ( outChannels[ i ] ).dataType() ) {
			case Bifrost::API::FloatType:
				Bifrost::API::VoxelChannel ( outChannels[ i ] ).setBackgroundValue( 0.0f );
				break;
			case Bifrost::API::UInt64Type:
				Bifrost::API::VoxelChannel ( outChannels[ i ] ).setBackgroundValue( 0 );
				break;
			default:
				break;
		}

		Bifrost::API::VoxelChannel ( outChannels[ i ] ).setOffsetType( Bifrost::API::VoxelOffsetType::Center );
	}

	// create the root of the tile tree
	Bifrost::API::TileAccessor outTacc = outLayout.tileAccessor();
	Bifrost::API::TreeIndex outRoot = outTacc.addRoot();

    int initStart = 0;
    int initEnd = 1;
    float dx = layout.voxelScale();
	int tileWidth = outLayout.tileDimInfo( outLayout.maxDepth() ).tileWidth;
	int tileWidthIndex = tileWidth - 1;

    if ( inData->splatSamples != 1 ) {
		// we need to calc how many tilewidths our maximum radius can cover
		// so that we can add those tiles if they are not in the bif tile tree
        int haloWidth = (int) ( ceil( inData->splatMaxRadius / dx ) / (float) tileWidth ) + 1;
        initStart = -haloWidth;
        initEnd = haloWidth;

        IFNOTSILENT { printf("\t\tExtra voxel width: %d\n", haloWidth); }
    }

	// add tiles in input cache to voxel cache
	Bifrost::API::TileIterator inTileIter = layout.tileIterator( layout.maxDepth(), layout.maxDepth(), Bifrost::API::TraversalMode::BreadthFirst );
	while ( inTileIter ) {
		const Bifrost::API::Tile& inTile = *inTileIter;

        for ( int k = initStart; k < initEnd; k++ ) {
            for ( int j = initStart; j < initEnd; j++ ) {
                for ( int i = initStart; i < initEnd; i++ ) {
					Bifrost::API::TreeIndex outTileIndex = outTacc.addTile(inTile.info().i + i, inTile.info().j + j, inTile.info().k + k, inTile.index().depth);
                    Bifrost::API::Tile outTile = outTacc.tile(outTileIndex);
                    Bifrost::API::TileData<float> outTileData = outInputChannel.tileData<float>( outTileIndex );

					//init tile data
					for ( int tk = 0; tk < 5; tk++ ) {
						for ( int tj = 0; tj < 5; tj++ ) {
							for ( int ti = 0; ti < 5; ti++ ) {
								outTileData(ti, tj, tk) = 0.0f;
							}
						}
					} // end set voxel data 
				}
			}
		} // end tile vicinity loop

		++inTileIter;
	}

	// calc frequency for export reporting
	int nofElements = (int) layout.tileCount( layout.maxDepth() );
	int freq =  nofElements > 10 ? 10 : nofElements; 
	int reportFreq = (int) layout.tileCount( layout.maxDepth() ) / freq;
	if (reportFreq == 0) {
		reportFreq = 1;
	}

	IFNOTSILENT { printf("\tSplating...\n"); }

	int exportTile = 0;
	uint64_t id = 0;
	int emptyTiles = 0;
	int addedTiles = 0;

	inTileIter = layout.tileIterator( layout.maxDepth(), layout.maxDepth(), Bifrost::API::TraversalMode::BreadthFirst );
	while ( inTileIter ) {
		// get tile coords
		const Bifrost::API::Tile& inTile = *inTileIter;

		// report progress
		IFNOTSILENT {
			if ( exportTile % reportFreq == 0 ) {
				std::cerr << "\t\tSplatting particles in tile " << exportTile << "..." << std::endl;
			}
		}

		// loop through the tile data
		// and fill output tile data according to splat params
		Bifrost::API::TileData<amino::Math::vec3f> tilePosData = posChannel.tileData<amino::Math::vec3f>( inTile.index() );
		Bifrost::API::TileData<uint64_t> tileIdData = idChannel.tileData<uint64_t>( inTile.index() );

		if ( (int) posChannel.elementCount( inTile.index() ) == 0 ) {
			emptyTiles++;
		} 

		for (int particle = 0; particle < (int) posChannel.elementCount( inTile.index() ); particle++, id++) {
			// if id channel exists we skip according to that which is the correct way
			uint64_t particleId = idExists ? tileIdData[ particle ] : id;
			bool skipping = ( particleId % ( (uint64_t) inData->skip ) == (uint64_t) 0 );
			if ( !skipping ) {
				continue;
			}

			// get original position
			amino::Math::vec3f particlePos = tilePosData[particle];

			// get a random radius for this particle
			srand48( particleId );
			float splatRadius = inData->splatMinRadius + (inData->splatMaxRadius - inData->splatMinRadius) * (float) drand48();
			float sqrRadius = splatRadius * splatRadius;

			for ( int splatIndex = 0; splatIndex < inData->splatSamples; splatIndex++ ) {
				float distanceToOriginal = 0.0f;
				float falloff = 1.0f;
				amino::Math::vec3f newWP;

				if ( inData->splatSamples > 1 ) {
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
					if ( inData->splatSurfaceAttract != 0.0f ) {
						amino::Math::vec3f sphereWP = newWP / distanceToOriginal;

						newWP = sphereWP * inData->splatSurfaceAttract + newWP * (1.0f - inData->splatSurfaceAttract);
					}

					// scale back upto splatRadius
					// if we are displacing, do that as well
					if ( inData->splatDisplacement != 0.0f ) {
						CvPoint3 noiseP;
						noiseP[0] = ( newWP[0] + particlePos[0] ) * inData->splatNoiseFreq;
						noiseP[1] = ( newWP[1] + particlePos[1] ) * inData->splatNoiseFreq;
						noiseP[2] = ( newWP[2] + particlePos[2] ) * inData->splatNoiseFreq;

						CvFloat radNoise = calcNoiseCB ( noiseP );
						radNoise = ( 1.0f - inData->splatDisplacement ) + inData->splatDisplacement * radNoise;

						newWP *= radNoise * splatRadius;						
					} else {
						newWP *= splatRadius; 
					}
							
					falloff = 1.0f - calcBlendVal ( inData->splatFalloffType, inData->splatFalloffStart, inData->splatFalloffEnd, distanceToOriginal / splatRadius);
					newWP += particlePos;
				} else {
					newWP = particlePos;
				}

				// get out tile
				Bifrost::API::TreeIndex outTileIndex = outLayout.treeIndex( (int) newWP[0], (int) newWP[1], (int) newWP[2], outLayout.maxDepth() );
				Bifrost::API::Tile outTile = outTacc.tile( outTileIndex );

				// splat data
				if ( outTile.info().depth == outLayout.maxDepth() ) {
					int tileDataI = clip <int> ( (int) floor( newWP[0] - outTile.info().i ), 0, tileWidthIndex );
					int tileDataJ = clip <int> ( (int) floor( newWP[1] - outTile.info().j ), 0, tileWidthIndex );
					int tileDataK = clip <int> ( (int) floor( newWP[2] - outTile.info().k ), 0, tileWidthIndex );

					for ( size_t channel = 0; channel < outChannels.count(); channel++ ) {
						Bifrost::API::Channel currentOutChannel = Bifrost::API::Channel( outChannels[ channel ] );
					
						if ( currentOutChannel != nofSamplesChannel ) {
							if ( currentOutChannel == outInputChannel ) {
								Bifrost::API::TileData<float> inputTileData = inputChannel.tileData<float>( inTile.index() );
								Bifrost::API::TileData<float> outTileData = currentOutChannel.tileData<float>( outTileIndex );

								// this is the input channel so accumulate according to scalars and splat samples
								outTileData( tileDataI, tileDataJ, tileDataK ) += inputTileData[ particle ] * falloff / (float) inData->splatSamples;
							} else {
								// process arbitrary channels
								if (	currentOutChannel.name() == "velocity_u" ||
										currentOutChannel.name() == "velocity_v" ||
										currentOutChannel.name() == "velocity_w" ) {
									// get velocity data
									Bifrost::API::TileData<amino::Math::vec3f> velocityTileData = velChannel.tileData<amino::Math::vec3f>( inTile.index() );
									Bifrost::API::TileData<float> outTileData = currentOutChannel.tileData<float>( outTileIndex );

									// get input velocity
									amino::Math::vec3f vel = velocityTileData[ particle ];

									if ( currentOutChannel.name() == "velocity_u" ) {
										outTileData( tileDataI, tileDataJ, tileDataK ) += vel[ 0 ];
									} else if ( currentOutChannel.name() == "velocity_v" ) {
										outTileData( tileDataI, tileDataJ, tileDataK ) += vel[ 1 ];
									} else {
										outTileData( tileDataI, tileDataJ, tileDataK ) += vel[ 2 ];
									}
								} else if ( currentOutChannel.name() == "u1" ||
										currentOutChannel.name() == "v1" ) {
									// find uv channel
									Bifrost::API::Channel tmpChannel = inComponent.findChannel( "uv" );
									// get this channels input data
									Bifrost::API::TileData<amino::Math::vec2f> uvTileData = tmpChannel.tileData<amino::Math::vec2f>( inTile.index() );
									Bifrost::API::TileData<float> outTileData = currentOutChannel.tileData<float>( outTileIndex );

									// get input uv
									amino::Math::vec2f uv = uvTileData[ particle ];

									if ( currentOutChannel.name() == "u1" ) {
										outTileData( tileDataI, tileDataJ, tileDataK ) += uv[ 0 ];
									} else {
										outTileData( tileDataI, tileDataJ, tileDataK ) += uv[ 1 ];
									}
								} else {
                                    Bifrost::API::Channel inputChannel2 = inComponent.findChannel( currentOutChannel.name() );
                                    Bifrost::API::TileData<float> inputTileData = inputChannel2.tileData<float>( inTile.index() );
									Bifrost::API::TileData<float> outTileData = currentOutChannel.tileData<float>( outTileIndex );
									// this is arbitrary data so accumulate
									float voxelData = inputTileData[particle];
									outTileData( tileDataI, tileDataJ, tileDataK ) += voxelData;
								}
							}
						} else {
							// this is samples counter per tile so add 1
							Bifrost::API::TileData<uint64_t> outTileData = currentOutChannel.tileData<uint64_t>( outTileIndex );

							outTileData( tileDataI, tileDataJ, tileDataK ) += (uint64_t) 1;
						}
					} // end channel loop
				} else {
					// printf("here\n");
				}
			} // end splat loop
		} // tile particle loop

		// we are finished with this tile
		++inTileIter;
		++exportTile;
	}

	IFNOTSILENT {
		printf( "\t\t%d extra tiles created during splating...\n", addedTiles );
		printf( "\t\tOut of %d tiles, %d tiles does not have any particles...\n", exportTile, emptyTiles );
	}

	//Bifrost::API::String outDump = outSS.dump();
	//printf("%s", outDump.c_str() );

	// we are finished with all tiles
	// now do averaging on channels
	Bifrost::API::TileIterator outTileIter = outLayout.tileIterator( layout.maxDepth(), layout.maxDepth(), Bifrost::API::TraversalMode::BreadthFirst );
	while ( outTileIter ) {
		const Bifrost::API::Tile& outTile = *outTileIter;
		Bifrost::API::TreeIndex outTileIndex = outTile.index();
		Bifrost::API::TileData<uint64_t> nofSamplesData = nofSamplesChannel.tileData<uint64_t>( outTileIndex );

		// loop over channel dta
		for ( size_t i = 0; i < outChannels.count(); i++ ) {
			Bifrost::API::Channel currentOutChannel = Bifrost::API::Channel( outChannels[ i ] );
			if ( currentOutChannel.name() != "nofSamples" && currentOutChannel.name() != inData->inputChannelName ) {
				// we divide by nofsamples as every channel except the input channel is averaging
				Bifrost::API::TileData<float> outTileData = currentOutChannel.tileData<float>( outTileIndex );

				for (int voxel = 0; voxel < (int) currentOutChannel.elementCount( outTileIndex ); voxel++) {
					if ( nofSamplesData[ voxel ] != (uint64_t) 0 ) {
						outTileData[ voxel ] /= (float) nofSamplesData[ voxel ];
					}
				} // end tile element loop
			} // end channel name check
		} // end channels loop

		++outTileIter;
	}

	IFNOTSILENT { printf("\t\tDone\n"); }

	srcSS = outSS;

	return outComponent;
}

void getMetaDataFlags ( Bifrost::API::FileIO fio, FrameData *frameData  )
{
	// because older version files does not support obj level metadata loading
	// we may need to visit this once more later
	Bifrost::API::Dictionary objDict;
	if ( frameData->metaDataLoaded < 0 ) {
		// this is the first time we run this
		const Bifrost::API::Dictionary& metaData = fio.metadata();
		objDict = metaData.value<Bifrost::API::Dictionary>( Bifrost::API::ObjectMetadataKey );
	} else {
		// this is the second time we run this
		objDict = frameData->inObj.dictionary();
	}

	if ( objDict.hasValue( "dropletThreshold" ) ) {
		frameData->dropletThreshold = objDict.value<float>("dropletThreshold");
	} else {
		frameData->metaDataLoaded = 0;
	}

	if ( objDict.hasValue("enableSpatialAdapt" ) ) {
		frameData->isAdaptive = objDict.value<bool>("enableSpatialAdapt");
	}

	if ( objDict.hasValue("gravityDirection" ) ) {
		frameData->gravityDirection = objDict.value<amino::Math::vec3f>("gravityDirection");
	}

	// set falloff axis direction
	setFalloffAxis( frameData );
}

void allocOptimizationArrays( FrameData *frameData, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	// now initialise arrays that hold min max values
	for (int i = 0; i <= layout.maxDepth(); i++) {
		int nofTiles = int( layout.tileCount(i) );

		IFNOTSILENTINFUNCTION {
			if ( diagnostics.DEBUG >= 2 ) {
				printf("Allocating %d elements for level %d\n", nofTiles, i);
			}
		}

		allocMinMaxArray(frameData->bifInfo.levelMin, frameData->bifInfo.levelMax, i, nofTiles);
		allocMinMaxArray(frameData->bifInfo.velMinX, frameData->bifInfo.velMaxX, i, nofTiles);
		allocMinMaxArray(frameData->bifInfo.velMinY, frameData->bifInfo.velMaxY, i, nofTiles);
		allocMinMaxArray(frameData->bifInfo.velMinZ, frameData->bifInfo.velMaxZ, i, nofTiles);
	}
}

void initOptimizationArrays ( FrameData *frameData, DiagnosticsParams diagnostics )
{
	// first alloc arrays
	 allocOptimizationArrays( frameData, diagnostics );

	// now calc
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	calcMinMaxArray( layout, frameData->srcChannel, frameData->bifInfo.levelMin, frameData->bifInfo.levelMax, diagnostics );
	if (frameData->velocityExists) {
		calcMinMaxArray( layout, frameData->velocityChannelU, frameData->bifInfo.velMinX, frameData->bifInfo.velMaxX, diagnostics );
		calcMinMaxArray( layout, frameData->velocityChannelV, frameData->bifInfo.velMinY, frameData->bifInfo.velMaxY, diagnostics );
		calcMinMaxArray( layout, frameData->velocityChannelW, frameData->bifInfo.velMinZ, frameData->bifInfo.velMaxZ, diagnostics );

		IFNOTSILENTINFUNCTION {
			if ( diagnostics.DEBUG >= 2 ) {
				printf("Vel Array Data Limits: %f %f %f %f %f %f\n",	frameData->bifInfo.velMinX[0][0], frameData->bifInfo.velMaxX[0][0],
																		frameData->bifInfo.velMinY[0][0], frameData->bifInfo.velMaxY[0][0],
																		frameData->bifInfo.velMinZ[0][0], frameData->bifInfo.velMaxZ[0][0]);
			}
		}
	}
}

void createSpeedChannel( Bifrost::API::StateServer srcSS, Bifrost::API::VoxelChannel inputChannel, Bifrost::API::Component component, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = component.layout();

	// we are processing a voxel component type so there is vel_u, vel_v, vel_w channels
	Bifrost::API::VoxelChannel speedChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "speed" );
	speedChannel.setOffsetType ( inputChannel.offsetType() );
	speedChannel.setBackgroundValue <float> ( 0.0f );

	// find velocity channels
	const Bifrost::API::Channel& velUChannel = component.findChannel( Bifrost::API::String( "velocity_u" ) );
	const Bifrost::API::Channel& velVChannel = component.findChannel( Bifrost::API::String( "velocity_v" ) );
	const Bifrost::API::Channel& velWChannel = component.findChannel( Bifrost::API::String( "velocity_w" ) );

	CalcVelocityMagnitudeVisitor speedVisitor ( velUChannel, velVChannel, velWChannel, speedChannel);
	layout.traverse(speedVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst);
}

void createPointSpeedChannel( Bifrost::API::StateServer& inSS )
{
	// get the component
	Bifrost::API::PointComponent point = inSS.findComponent( "voxel_liquid-particle" );
	Bifrost::API::Layout layout = point.layout();
	Bifrost::API::PointChannel velocityChannel = point.findChannel( "velocity" );

	// speed is magnitude of velocity so get velocity channel and calc and put magnitude in a channel called speed
	Bifrost::API::PointChannel speedChannel = inSS.createChannel ( point, Bifrost::API::FloatType, "speed" );
	CalcPointVelocityMagnitudeVisitor magVisitor ( velocityChannel, speedChannel);
	layout.traverse(magVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst);
}

void createUVChannels( Bifrost::API::StateServer srcSS, Bifrost::API::VoxelChannel inputChannel, Bifrost::API::Component component, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = component.layout();

	Bifrost::API::VoxelChannel uChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "u1" );
	Bifrost::API::VoxelChannel vChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "v1" );	
	uChannel.setOffsetType ( inputChannel.offsetType() );
	vChannel.setOffsetType ( inputChannel.offsetType() );

	uChannel.setBackgroundValue <float> ( 0.0f );
	vChannel.setBackgroundValue <float> ( 0.0f );

	// find uv channel
	const Bifrost::API::VoxelChannel& uvChannel = component.findChannel( Bifrost::API::String( "uv" ) );

	SeparateUVVisitor uvVisitor ( uvChannel, uChannel, vChannel);
	layout.traverse(uvVisitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);

	// we don't need uv channel anymore so clear it from memory
	srcSS.removeChannel( uvChannel.fullPathName() );
}

void reportChannelRanges ( FrameData *frameData, Bifrost::API::Component component, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = component.layout();

	// assemble a unique channel names array to report a range for in the log
	Bifrost::API::StringArray reportChannelNames;
	Bifrost::API::Channel tmpChannel;

	// add primvar channels to the report list
	for (int i = 0; i < frameData->primVars.size(); i++) {
		if (	frameData->primVars[i].channel.valid() &&
				frameData->primVars[i].channel.dataType() == Bifrost::API::FloatType &&
				frameData->primVars[i].name != "presence" ) {
			reportChannelNames.addUnique ( frameData->primVars[i].channel.name() );
		}
	}

	// add stuff that are already in the report channel list to the report list
	for (int i = 0; i < frameData->reportChannels.size(); i++) {
		reportChannelNames.addUnique ( frameData->reportChannels[i].name() );
	}

	// now we have a unique list so clear and store them in framedata->reportChannels
	frameData->reportChannels.clear();
	for (int i = 0; i < reportChannelNames.count(); i++) {
		tmpChannel = component.findChannel( reportChannelNames[i] );
		frameData->reportChannels.push_back( tmpChannel );
	}

	// now go over frameData->reportChannels and also store their min max values in frameData
	frameData->channelMin.clear();
	frameData->channelMax.clear();
    IFNOTSILENTINFUNCTION {
        if(frameData->reportChannels.size() > 0){
            printf("\nChannel Min-Max Values:\n");
        }
    }
	for ( int count = 0; count < frameData->reportChannels.size(); count++) {
        ChannelValueRangeVisitor valueRangeVisitor ( frameData->reportChannels[count] );
		layout.traverse(valueRangeVisitor, Bifrost::API::TraversalMode::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );
        frameData->channelMin.push_back( valueRangeVisitor.m_minVal );
        frameData->channelMax.push_back( valueRangeVisitor.m_maxVal );
		IFNOTSILENTINFUNCTION {
			printf("\tChannel: %s\n", ( frameData->reportChannels[ count ] ).name().c_str());
			printf("\tMin:%.2f, Max: %.2f\n\n", frameData->channelMin[ count ], frameData->channelMax[ count ]);
		}
	}
}

Bifrost::API::Component convertParticlesToVoxels( Bifrost::API::Component component, ImplicitsInputData *inData, FrameData *frameData, Bifrost::API::StateServer& srcSS, Bifrost::API::StringArray& loadChannelNames )
{
	Bifrost::API::Layout layout = component.layout();

	// we have a particle cache so first convert the particles into voxel rep and
	// than continue as normal with the new in memory voxels
	Bifrost::API::VoxelComponent voxelSolidComponent(srcSS.findComponent("Solids-volume"));
	Bifrost::API::String voxelSolidDistanceChannel("Solids-volume.distance");
	Bifrost::API::PointComponent pointComponent = component;

	Bifrost::API::Layout outputVoxelLayout(srcSS.createLayout("outputVoxel-volumeLayout"));
	outputVoxelLayout.setVoxelScale( layout.voxelScale() );

	Bifrost::API::Object outputVoxel(srcSS.createObject("ObjectType", "outputVoxel"));
	Bifrost::API::VoxelComponent outputVoxelComponent( srcSS.createComponent(Bifrost::API::VoxelComponentType, "outputVoxel-volume", outputVoxelLayout) );
	outputVoxel.addComponent(outputVoxelComponent);

	// create channels
	// since proc passes us the names of the necessary channels
	// we create channels according to that list

	// distance is not in the list of necessary channels as it is mandatory
	// create it
	Bifrost::API::VoxelChannel newDistanceChannel = srcSS.createChannel(outputVoxelComponent, Bifrost::API::FloatType, "distance");
	newDistanceChannel.setBackgroundValue( 0.0f );
	newDistanceChannel.setOffsetType( Bifrost::API::VoxelOffsetType::Center );

	// now go over the necessary channel names and pick what we need to create
	Bifrost::API::StringArray createdChannelNames;
	for ( int i = 0; i < loadChannelNames.count(); i++ ) {
		if (	loadChannelNames[i] != "position" &&
				loadChannelNames[i] != "id64" &&
				loadChannelNames[i] != "speed" &&
				loadChannelNames[i] != "droplet" ) {
			// handle velocity
			if ( loadChannelNames[i] == "velocity" ) {
				// proc says we have motion blur on or we need to calc speed AND we have the velocity channels in bif
				// so create corresponding channels for the voxel rep to hold velocity data
				srcSS.createChannel(outputVoxelComponent, Bifrost::API::FloatV3Type, "velocity");
				// create 3 extra channels for seperating velocity into three channels as in a cacheType of VOXEL
				srcSS.createChannel(outputVoxelComponent, Bifrost::API::FloatType, "velocity_u");
				srcSS.createChannel(outputVoxelComponent, Bifrost::API::FloatType, "velocity_v");
				srcSS.createChannel(outputVoxelComponent, Bifrost::API::FloatType, "velocity_w");
				// add it to created channel names array
				createdChannelNames.add ( loadChannelNames[i] );

				IFNOTSILENT { printf("\tCreating voxel channel %s\n", createdChannelNames.last().c_str() ); }
			} else {
				Bifrost::API::PointChannel channel = (Bifrost::API::PointChannel) component.findChannel ( loadChannelNames[i] );
				Bifrost::API::DataType dataType = channel.dataType();
				createPrimVarChannel(	loadChannelNames[i], dataType,
										srcSS, outputVoxelComponent,
										Bifrost::API::VoxelOffsetType::Center, 0.0f ); 
				// add it to created channel names array
				createdChannelNames.add ( loadChannelNames[i] );
				if ( loadChannelNames[ i ] != "airDistance" ) {
					IFNOTSILENT { printf("\tCreating voxel channel %s\n", createdChannelNames.last().c_str() ); }
				}
			} // end velocity check
		} // end channel name check
	} // end for loop

	// declare primvars name array for particle to voxel conversion
	Bifrost::API::StringArray pointChannels;

	// Mandatory channels (specific order required)
	pointChannels.add("voxel_liquid-particle.position");
	pointChannels.add("voxel_liquid-particle.droplet");

	// add extra channels
	for ( int i = 0; i < createdChannelNames.count(); i++) {
		Bifrost::API::String channelName = "voxel_liquid-particle.";
		channelName += Bifrost::API::String( createdChannelNames[i].c_str() );
		pointChannels.add( channelName );
	}

	// declare output channels
	Bifrost::API::StringArray outputVoxelChannels;

	// Mandatory channel (first must be 'distance')
	outputVoxelChannels.add( "outputVoxel-volume.distance" );

	// Optional output channels (order and type must be consistent with optional channels in the 'pointChannels' array)
	for ( int i = 0; i < createdChannelNames.count(); i++) {
		Bifrost::API::String channelName = "outputVoxel-volume.";
		channelName += Bifrost::API::String( createdChannelNames[i].c_str() );
		outputVoxelChannels.add( channelName );
	}

	IFNOTSILENT {
		if ( inData->diagnostics.DEBUG > 1 ) {
			dumpStateServer( srcSS, "AFTER PARTICLES TO VOXELS" );
		}
	}

	Bifrost::API::LevelSetOptions levelSetOptions;
	levelSetOptions.dropletRevealFactor = inData->dropletRevealFactor;
	levelSetOptions.surfaceRadius = inData->surfaceRadius;
	levelSetOptions.dropletRadius = inData->dropletRadius;
	levelSetOptions.resolutionFactor = inData->resolutionFactor;
	levelSetOptions.dropletThreshold = frameData->dropletThreshold;
	levelSetOptions.isAdaptive = frameData->isAdaptive;
	levelSetOptions.maxVolumeOfHolesToClose = inData->maxVolumeOfHolesToClose;
	levelSetOptions.doMorphologicalDilation = inData->doMorphologicalDilation;  
	levelSetOptions.doErodeSheetsAndDroplets = inData->doErodeSheetsAndDroplets;  

	IFNOTSILENT { printf("\tDoing conversion...\n"); }
	Bifrost::API::Status status = Bifrost::API::createLevelSet(	levelSetOptions, 
																component,					/* component holding the particles from which the level set is generated */ 
																pointChannels,				/* must exist on point: particles, droplet, etc... */
																voxelSolidComponent,		/* optional, component holding the solid distance */
																voxelSolidDistanceChannel,	/* optional, must exist on voxel solid */
																outputVoxelComponent,		/* component holding the output voxel channels */
																outputVoxelChannels			/* must exist on outputVoxel: distance, etc... */
																);
	if ( status == Bifrost::API::Status::Failure ) {
		IFNOTSILENT { printf("Error: createLevelSet call failed...\n"); }
	} else {
		IFNOTSILENT { printf("\t\tDone\n\n"); }
	}

	layout = outputVoxelComponent.layout();
	Bifrost::API::RefArray channels;
	channels = outputVoxelComponent.channels();

	srcSS = srcSS;

	// seperate the velocity vector into three channels a la VOXEL caches
	Bifrost::API::VoxelChannel outVelocityChannel = outputVoxelComponent.findChannel( Bifrost::API::String( "velocity" ) );
	Bifrost::API::VoxelChannel outVelocityChannelU = outputVoxelComponent.findChannel( Bifrost::API::String( "velocity_u" ) );
	Bifrost::API::VoxelChannel outVelocityChannelV = outputVoxelComponent.findChannel( Bifrost::API::String( "velocity_v" ) );
	Bifrost::API::VoxelChannel outVelocityChannelW = outputVoxelComponent.findChannel( Bifrost::API::String( "velocity_w" ) );

	SeperateVelocityVisitor seperator( outVelocityChannel, outVelocityChannelU, outVelocityChannelV, outVelocityChannelW );
	layout.traverse( seperator, Bifrost::API::TraversalMode::ParallelBreadthFirst );

	// converted velocity channel is not needed anymore so delete it
	srcSS.removeChannel( outVelocityChannel.fullPathName() );

	// we are done with point data so clear it from memory
	deleteObject ( srcSS, component.object() );

	return outputVoxelComponent;
}

void setFalloffAxis( FrameData *frameData ) {
	// what indexes we should use for a center falloff
	if ( fabs( frameData->gravityDirection[0] ) > 0.0f ) {
		// sim is oriented towards positive x axis
		frameData->presenceFalloffAxises[0] = 1;
		frameData->presenceFalloffAxises[1] = 2;
	} else if ( fabs( frameData->gravityDirection[1] ) > 0.0f ) {
		// sim is oriented towards positive y axis
		frameData->presenceFalloffAxises[0] = 0;
		frameData->presenceFalloffAxises[1] = 2;
	} else {
		// sim is oriented towards positive z axis
		frameData->presenceFalloffAxises[0] = 0;
		frameData->presenceFalloffAxises[1] = 1;
	}
}

void optimizeInputForRendering ( FrameData *frameData, Bifrost::API::Channel inChannel, DiagnosticsParams diagnostics )
{
	Bifrost::API::Component component = inChannel.component();
	Bifrost::API::Layout layout = inChannel.layout();

	// calc hull vertices
	calcHullNormals ( frameData->hullCorners, frameData->planes );

	// call the visitor
	Bifrost::API::Channel frustumChannel = inChannel;
	if ( frameData->isPointCache ) {
		// in this case orginputchannel does not exist so lets use position instead
		frustumChannel = component.findChannel( Bifrost::API::String( "position" ) );
	}

	FrustumCullVisitor culler( frustumChannel, frameData );
	layout.traverse( culler, Bifrost::API::TraversalMode::BreadthFirst, layout.maxDepth(), layout.maxDepth() );
	Bifrost::API::TileAccessor tmpAcc = layout.tileAccessor();
	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 1 ) {
			printf( "\nOptimizing %d tiles...\n", culler.c_cullIndexes.count() );
		}
	}
	tmpAcc.removeTiles ( culler.c_cullIndexes );
}

void copyDistanceChannel( FrameData *frameData, Bifrost::API::StateServer srcSS, Bifrost::API::Component component )
{
	Bifrost::API::Layout layout = component.layout();

	if ( frameData->isAdaptive ) {
		Bifrost::API::VoxelChannel safeChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "safeDistance" );
		safeChannel.setOffsetType ( frameData->srcChannel.offsetType() );
		safeChannel.setBackgroundValue <float> ( frameData->srcChannel.backgroundValue<float>() );
		CopyChannelVisitor copier( frameData->srcChannel, safeChannel );
		layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );
		frameData->safeChannel = safeChannel;
	}
}

}}
