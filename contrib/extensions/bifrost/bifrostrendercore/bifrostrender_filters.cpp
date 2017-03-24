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

#include <bifrostrendercore/bifrostrender_filters.h>

namespace Bifrost{
namespace RenderCore{

void dilateChannel( FrameData *frameData, float dilateAmount, BifrostRenderType renderType )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	OffsetVoxelsVisitor inward( frameData->srcChannel, -dilateAmount );
	layout.traverse( inward, Bifrost::API::TraversalMode::ParallelBreadthFirst );

	if ( renderType == IMPLICITSURFACE ) {
		OffsetVoxelsVisitor inward2( frameData->airDistanceChannel, dilateAmount );
		layout.traverse( inward2, Bifrost::API::TraversalMode::ParallelBreadthFirst );
	}
}

void erodeChannel( FrameData *frameData, float erodeAmount, BifrostRenderType renderType )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();

	OffsetVoxelsVisitor inward( frameData->srcChannel, erodeAmount );
	layout.traverse( inward, Bifrost::API::TraversalMode::ParallelBreadthFirst );

	if ( renderType == IMPLICITSURFACE ) {
		OffsetVoxelsVisitor inward2( frameData->airDistanceChannel, -erodeAmount );
		layout.traverse( inward2, Bifrost::API::TraversalMode::ParallelBreadthFirst );
	}
}

void smoothChannel(	SmoothFilterParams smooth, 
					Bifrost::API::StateServer srcSS,
					Bifrost::API::VoxelChannel inChannel,
					FrameData *frameData,
					DiagnosticsParams diagnostics,
					BifrostRenderType renderType )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();
	Bifrost::API::Component component = frameData->srcChannel.component();

	// if we need to blend smoothing later we would need the original data
	// so let's make copies of them if we are blending
	Bifrost::API::VoxelChannel filterChannel = component.findChannel( Bifrost::API::String( smooth.channelName ) );

	if ( filterChannel.valid() || smooth.weight > FLT_EPSILON ) {
		// create a backup of the original channels for filterblending purposes
		Bifrost::API::VoxelChannel orgChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "orgDistance" );
		orgChannel.setOffsetType ( frameData->srcChannel.offsetType() );
		orgChannel.setBackgroundValue <float> ( frameData->srcChannel.backgroundValue<float>() );
		CopyChannelVisitor copier( frameData->srcChannel, orgChannel );
		layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );

		if ( renderType == IMPLICITSURFACE ) {
			Bifrost::API::VoxelChannel orgAirDistanceChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "orgAirDistance" );
			orgAirDistanceChannel.setOffsetType ( frameData->airDistanceChannel.offsetType() );
			orgAirDistanceChannel.setBackgroundValue <float> ( frameData->airDistanceChannel.backgroundValue<float>() );
			CopyChannelVisitor copier2( frameData->airDistanceChannel, orgAirDistanceChannel );
			layout.traverse( copier2, Bifrost::API::TraversalMode::ParallelBreadthFirst );
		}
	}

	// proceed to smoothing

	// Create temporary float channels for smoothing
	Bifrost::API::VoxelChannel tmpChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "tmpChannel" );
	tmpChannel.setOffsetType ( frameData->srcChannel.offsetType() );
	tmpChannel.setBackgroundValue <float> ( frameData->srcChannel.backgroundValue<float>() );

	Bifrost::API::VoxelChannel tmpAirDistanceChannel;
	if ( renderType == IMPLICITSURFACE ) {
		tmpAirDistanceChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "tmpAirDistance" );
		tmpAirDistanceChannel.setOffsetType ( frameData->airDistanceChannel.offsetType() );
		tmpAirDistanceChannel.setBackgroundValue <float> ( frameData->airDistanceChannel.backgroundValue<float>() );
	}

	// handle gaussian as multiples of mean value filter
	int gaussianExtraIteration = 1; 
	if ( smooth.mode == kGaussian ) {
		smooth.mode = kMeanValue; 
		gaussianExtraIteration = 4;
	}

	Bifrost::API::VoxelChannel airDistanceChannel;
	if ( renderType == IMPLICITSURFACE ) {
		airDistanceChannel = component.findChannel( Bifrost::API::String( "airDistance" ) );
	}

	// apply filter
	for ( int i = 0; i < smooth.iterations * gaussianExtraIteration; i++ ) {
		SmoothVoxelsVisitor visitor	(
										i % 2 ? tmpChannel : inChannel,
										i % 2 ? inChannel : tmpChannel,
										int( smooth.amount ),
										smooth.mode
									);
		layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
		frameData->srcChannel = i % 2 ? inChannel : tmpChannel;

		if ( renderType == IMPLICITSURFACE ) {
			SmoothVoxelsVisitor visitor2(	
											i % 2 ? tmpAirDistanceChannel : airDistanceChannel,
											i % 2 ? airDistanceChannel : tmpAirDistanceChannel,
											int( smooth.amount ),
											smooth.mode
										);
			layout.traverse(visitor2, Bifrost::API::TraversalMode::ParallelBreadthFirst);
			frameData->airDistanceChannel = i % 2 ? frameData->airDistanceChannel : tmpAirDistanceChannel;
		}
	}

	// copy tmpChannels to inChannels if srcChannels are tmpChannels
	// and delete tmpchannels from memory
	if ( frameData->srcChannel == tmpChannel ) {
		CopyChannelVisitor copier( tmpChannel, inChannel );
		layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );
		frameData->srcChannel = inChannel;
	}

	if ( renderType == IMPLICITSURFACE ) {
		if ( frameData->airDistanceChannel == tmpAirDistanceChannel ) {
			CopyChannelVisitor copier( tmpAirDistanceChannel, airDistanceChannel );
			layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );
			frameData->airDistanceChannel = airDistanceChannel;
		}
	}

	// delete tmp channels from mem
	srcSS.removeChannel( tmpChannel.fullPathName() );
	if ( renderType == IMPLICITSURFACE ) {
		srcSS.removeChannel( tmpAirDistanceChannel.fullPathName() );
	}

	// blend with original if necessary

	if ( filterChannel.valid() || smooth.weight > FLT_EPSILON ) {
		Bifrost::API::VoxelChannel tmpSmoothBlendChannel;
		if ( filterChannel.valid() ) {
			if ( diagnostics.silent == 0 ) {
				printf("\t\tFilter Blending using %s channel...\n", filterChannel.partialPathName().c_str() );
				printf("\t\tMin: %f Max: %f Invert: %d\n", smooth.remapMin, smooth.remapMax, smooth.remapInvert);
			}

			// we need to make a copy of the blending channel because we are going to remap it and it maybe used as a primvar which would requires the original data
			tmpSmoothBlendChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "tmpSmoothBlend" );
			tmpSmoothBlendChannel.setOffsetType ( filterChannel.offsetType() );
			tmpSmoothBlendChannel.setBackgroundValue <float> ( filterChannel.backgroundValue<float>() );
			CopyChannelVisitor copier3( filterChannel, tmpSmoothBlendChannel );
			layout.traverse( copier3, Bifrost::API::TraversalMode::ParallelBreadthFirst );

			// perform remapping
			RemapVoxelsVisitor remapper ( tmpSmoothBlendChannel, smooth.remapMin, smooth.remapMax, smooth.remapInvert ); 
			layout.traverse(remapper, Bifrost::API::TraversalMode::ParallelBreadthFirst);
		}

		Bifrost::API::VoxelChannel orgChannel = component.findChannel( Bifrost::API::String( "orgDistance" ) );
		Bifrost::API::VoxelChannel orgAirDistanceChannel;
		if ( renderType == IMPLICITSURFACE ) {
			orgAirDistanceChannel = component.findChannel( Bifrost::API::String( "orgAirDistance" ) );
		}

		// now do the blending
		BlendVoxelsVisitor blender ( smooth.weight, orgChannel, tmpSmoothBlendChannel, frameData->srcChannel ); 
		layout.traverse( blender, Bifrost::API::TraversalMode::ParallelBreadthFirst );

		if ( renderType == IMPLICITSURFACE ) {
			BlendVoxelsVisitor blender2( smooth.weight, orgAirDistanceChannel, tmpSmoothBlendChannel, frameData->airDistanceChannel ); 
			layout.traverse( blender2, Bifrost::API::TraversalMode::ParallelBreadthFirst );
		}

		// we are done clear tmpChannels
		srcSS.removeChannel( orgChannel.fullPathName() );
		if ( renderType == IMPLICITSURFACE ) {
			srcSS.removeChannel( orgAirDistanceChannel.fullPathName() );
		}

		if ( tmpSmoothBlendChannel.valid() ) {
			srcSS.removeChannel( tmpSmoothBlendChannel.fullPathName() );
		}
	} else {
		if ( diagnostics.silent == 0 ) {
			if ( smooth.weight < FLT_EPSILON ) {
				printf("\tWeight is 0\n");
			} else {
				if ( strcmp( smooth.channelName, "none" ) != 0 ) {
					printf("\tCan not find the specified filter channel: %s\n", smooth.channelName);
				}
			}
		}
	} // end filter blending check
}

void displaceChannel( InfCubeParams infCube, Bifrost::API::StateServer srcSS, FrameData *frameData, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();
	Bifrost::API::Component component = frameData->srcChannel.component();

	if ( frameData->infCubeBlendChannel.valid() ) {
		IFNOTSILENTINFUNCTION { printf("\tBlending with an infinite plane at y = %f using %s channel...\n\n", infCube.topCenterY, frameData->infCubeBlendChannel.partialPathName().c_str() ); }

		// we need to make a copy of the blending channel because we are going to remap it and it maybe used as a primvar which requires the original data
		Bifrost::API::VoxelChannel tmpBlendChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, "tmpBlend" );
		tmpBlendChannel.setOffsetType ( frameData->infCubeBlendChannel.offsetType() );
		tmpBlendChannel.setBackgroundValue <float> ( frameData->infCubeBlendChannel.backgroundValue<float>() );
		CopyChannelVisitor copier( frameData->infCubeBlendChannel, tmpBlendChannel );
		layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );

		RemapVoxelsVisitor blendRemapper ( tmpBlendChannel, infCube.remapMin, infCube.remapMax, infCube.remapInvert ); 
		layout.traverse(blendRemapper, Bifrost::API::TraversalMode::ParallelBreadthFirst);

		frameData->infCubeBlendChannel = tmpBlendChannel;
	} else {
		IFNOTSILENTINFUNCTION { printf("\tBlending with an infinite plane at y= %f...\n", infCube.topCenterY); }
	}

	// compute sim boundary in world space
	computeImplicitsBounds( frameData, infCube, diagnostics );

	// displace the srcchannel
	DisplaceVoxelsVisitor displacer (
										frameData->srcChannel,
										frameData->airDistanceChannel,
										frameData->infCubeBlendChannel,
										&frameData->primVars,
										frameData->bboxSim,
										infCube
									);

	layout.traverse(displacer, Bifrost::API::TraversalMode::ParallelBreadthFirst, 0, layout.maxDepth());

	// we are done with the tmp channel so release it if we created one
	if ( frameData->infCubeBlendChannel.valid() ) {
		srcSS.removeChannel( (frameData->infCubeBlendChannel).fullPathName() );
	}
}

bool checkAndAddTile ( int i, int j, int k, int depthWidth, float voxelScale, FrustumPlane planes[6] )
{
	amino::Math::vec3f tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = (float) i;
	tileBoundsMin[1] = (float) j;
	tileBoundsMin[2] = (float) k;
	tileBoundsMax[0] = (float) i + depthWidth;
	tileBoundsMax[1] = (float) j + depthWidth;
	tileBoundsMax[2] = (float) k + depthWidth;

	// convert to worldspace
	tileBoundsMin *= voxelScale;
	tileBoundsMax *= voxelScale;

	CvPoint corners[8];
	corners[0][0] = tileBoundsMin[0];
	corners[0][1] = tileBoundsMin[1];
	corners[0][2] = tileBoundsMin[2];

	corners[1][0] = tileBoundsMax[0];
	corners[1][1] = tileBoundsMin[1];
	corners[1][2] = tileBoundsMin[2];

	corners[2][0] = tileBoundsMin[0];
	corners[2][1] = tileBoundsMax[1];
	corners[2][2] = tileBoundsMin[2];

	corners[3][0] = tileBoundsMax[0];
	corners[3][1] = tileBoundsMax[1];
	corners[3][2] = tileBoundsMin[2];

	corners[4][0] = tileBoundsMin[0];
	corners[4][1] = tileBoundsMin[1];
	corners[4][2] = tileBoundsMax[2];

	corners[5][0] = tileBoundsMax[0];
	corners[5][1] = tileBoundsMin[1];
	corners[5][2] = tileBoundsMax[2];

	corners[6][0] = tileBoundsMin[0];
	corners[6][1] = tileBoundsMax[1];
	corners[6][2] = tileBoundsMax[2];

	corners[7][0] = tileBoundsMax[0];
	corners[7][1] = tileBoundsMax[1];
	corners[7][2] = tileBoundsMax[2];

	return boundIntersectsWithFrustum ( corners, planes );
}

void checkTilesInBetweenLimits (	int iStart, int iEnd, int jStart, int jEnd, int kStart, int kEnd,
									int depthWidth, float voxelScale, FrustumPlane planes[6],
									Bifrost::API::TileAccessor acc, Bifrost::API::TreeIndexArray& tileIndexes )
{
	Bifrost::API::Layout layout = acc.layout();

	for ( int k = kStart; k < kEnd; k+=5 ) {
		for ( int j = jStart; j < jEnd; j+=5 ) {
			for ( int i = iStart; i < iEnd; i+=5 ) {
				if ( checkAndAddTile ( i, j, k, depthWidth, voxelScale, planes ) ) {
					Bifrost::API::Tile tile = acc.tile( i, j, k, layout.maxDepth() );
					if ( tile.info().depth != layout.maxDepth() ) {
						// There is a tile there but it is not at the highest level so add one at the highest level
						Bifrost::API::TreeIndex tileIndex = acc.addTile( i, j, k, layout.maxDepth() );
						tileIndexes.add( tileIndex );
					}
				}
			}
		}
	}
}

void getFillTileIds( InfCubeParams infCube, FrameData *frameData, Bifrost::API::TreeIndexArray& tileIndexes )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();
	Bifrost::API::TileAccessor acc = layout.tileAccessor();
	const Bifrost::API::TileDimInfo info = layout.tileDimInfo( layout.maxDepth() );
	Bifrost::API::Component component = frameData->srcChannel.component();

	float infCubeTopYInTileSpace = infCube.topCenterY / frameData->voxelScale;
	int yStart = int ( infCubeTopYInTileSpace ) - 5;
	int yEnd = yStart + 10;

	checkTilesInBetweenLimits	(	(int) frameData->bboxSim.tile.min[ 0 ], (int) frameData->bboxSim.tile.max[ 0 ],
								yStart, yEnd,
								(int) frameData->bboxSim.tile.min[ 2 ], (int) frameData->bboxSim.tile.max[ 2 ],
								info.depthWidth, frameData->voxelScale, frameData->planes,
								acc, tileIndexes
							);
}

void getInfCubeTileIds( InfCubeParams infCube, FrameData *frameData, Bifrost::API::TreeIndexArray& tileIndexes )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();
	Bifrost::API::TileAccessor acc = layout.tileAccessor();
	const Bifrost::API::TileDimInfo info = layout.tileDimInfo( layout.maxDepth() );
	Bifrost::API::Component component = frameData->srcChannel.component();

	float infCubeTopYInTileSpace = infCube.topCenterY / frameData->voxelScale;
	int yStart = int ( infCubeTopYInTileSpace ) - 10;
	int yEnd = yStart + 20;

	checkTilesInBetweenLimits	(	(int) frameData->bboxInf.tile.min[ 0 ], (int) frameData->bboxSim.tile.min[ 0 ],
									yStart, yEnd,
									(int) frameData->bboxInf.tile.min[ 2 ], (int) frameData->bboxInf.tile.max[ 2 ],
									info.depthWidth, frameData->voxelScale, frameData->planes,
									acc, tileIndexes
								);

	checkTilesInBetweenLimits	(	(int) frameData->bboxSim.tile.max[ 0 ], (int) frameData->bboxInf.tile.max[ 0 ],
									yStart, yEnd,
									(int) frameData->bboxInf.tile.min[ 2 ], (int) frameData->bboxInf.tile.max[ 2 ],
									info.depthWidth, frameData->voxelScale, frameData->planes,
									acc, tileIndexes
								);

	checkTilesInBetweenLimits	(	(int) frameData->bboxSim.tile.min[ 0 ], (int) frameData->bboxSim.tile.max[ 0 ],
									yStart, yEnd,
									(int) frameData->bboxInf.tile.min[ 2 ], (int) frameData->bboxSim.tile.min[ 2 ],
									info.depthWidth, frameData->voxelScale, frameData->planes,
									acc, tileIndexes
								);

	checkTilesInBetweenLimits	(	(int) frameData->bboxSim.tile.min[ 0 ], (int) frameData->bboxSim.tile.max[ 0 ],
									yStart, yEnd,
									(int) frameData->bboxSim.tile.max[ 2 ], (int) frameData->bboxInf.tile.max[ 2 ],
									info.depthWidth, frameData->voxelScale, frameData->planes,
									acc, tileIndexes
								);
}

void extendChannel( InfCubeParams infCube, Bifrost::API::StateServer srcSS, FrameData *frameData, DiagnosticsParams diagnostics )
{
	Bifrost::API::Layout layout = frameData->srcChannel.layout();
	Bifrost::API::TileAccessor acc = layout.tileAccessor();
	Bifrost::API::Component component = frameData->srcChannel.component();

	IFNOTSILENTINFUNCTION { printf("\tExtending the sim space...\n"); }

	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			printf( "\t\tTile count before extending: %d\n", layout.tileCount( layout.maxDepth() ) );
		}
	}

	// first get tiles that are missing in the sim bbox and fill them
	Bifrost::API::TreeIndexArray tileIndexes;
	getFillTileIds( infCube, frameData, tileIndexes );
	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			printf("\t\tFill count: %d\n", tileIndexes.count());
		}
	}

	// now fill the surrounding areas
	getInfCubeTileIds( infCube, frameData, tileIndexes );
	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			printf("\t\tCube count: %d\n", tileIndexes.count());
		}
	}

	IFNOTSILENTINFUNCTION {
		if ( diagnostics.DEBUG > 0 ) {
			printf( "\t\tTile count after extending: %d\n", layout.tileCount( layout.maxDepth() ) );
		}
	}

	for( int i = 0; i < tileIndexes.count(); i++ ) {
		Bifrost::API::Tile tile = acc.tile( tileIndexes[ i ] );
		//std::cerr << tile.info().i << "," << tile.info().j << "," << tile.info().k << "," << tile.info().depth << std::endl;
		Bifrost::API::TileData<float> srcTileData = frameData->srcChannel.tileData<float>( tileIndexes[ i ] );
		const Bifrost::API::TileCoord coord = tile.coord();

		for ( size_t element = 0; element < srcTileData.count(); element++ ) {
			int voxelJ = ( int ( element / 5) ) % 5;

			float finalYCoord = ( coord.j + voxelJ + 0.5f ) * frameData->voxelScale;
			srcTileData[ element ] = finalYCoord - infCube.topCenterY;
		}
	}
}

}}
