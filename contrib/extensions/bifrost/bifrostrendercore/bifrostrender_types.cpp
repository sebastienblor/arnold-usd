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

#include <bifrostrendercore/bifrostrender_types.h>

namespace Bifrost{
namespace RenderCore{

void BBox::calcFromBounds()
{
	this->min[0] = this->bound[0];
	this->max[0] = this->bound[1];
	this->min[1] = this->bound[2];
	this->max[1] = this->bound[3];
	this->min[2] = this->bound[4];
	this->max[2] = this->bound[5];

	this->dim[0] = this->max[0] - this->min[0];
	this->dim[1] = this->max[1] - this->min[1];
	this->dim[2] = this->max[2] - this->min[2];

	this->center[0] = this->min[0] + this->dim[0] / 2.0f;
	this->center[1] = this->min[1] + this->dim[1] / 2.0f;
	this->center[2] = this->min[2] + this->dim[2] / 2.0f;
}

void BBox::calcFromMinMax()
{
	this->bound[0] = this->min[0];
	this->bound[1] = this->max[0];
	this->bound[2] = this->min[1];
	this->bound[3] = this->max[1];
	this->bound[4] = this->min[2];
	this->bound[5] = this->max[2];

	this->dim[0] = this->max[0] - this->min[0];
	this->dim[1] = this->max[1] - this->min[1];
	this->dim[2] = this->max[2] - this->min[2];

	this->center[0] = this->min[0] + this->dim[0] / 2.0f;
	this->center[1] = this->min[1] + this->dim[1] / 2.0f;
	this->center[2] = this->min[2] + this->dim[2] / 2.0f;
}

void BBox::dump( Bifrost::API::String bBoxName )
{
	printf("%s Min: \t%.2f %.2f %.2f\n%s Max: \t%.2f %.2f %.2f\n%sDim: \t%.2f %.2f %.2f\n\n",
			bBoxName.c_str(),
			this->min[0], this->min[1], this->min[2],
			bBoxName.c_str(),
			this->max[0], this->max[1], this->max[2],
			bBoxName.c_str(),
			this->dim[0], this->dim[1], this->dim[2]);
}

void BBox::dumpSimple()
{
	printf("BBox:\n\tMinX: %.2f\tMaxX: %.2f\n\tMinY: %.2f\tMaxY: %.2f\n\tMinZ: %.2f\tMaxZ: %.2f\n",
		this->min[0], this->max[0],
		this->min[1], this->max[1],
		this->min[2], this->max[2]);
}

void BBoxData::calcTileFromWorld()
{
	this->tile.min[0] = this->world.bound[0] / this->voxelScale;
	this->tile.min[1] = this->world.bound[2] / this->voxelScale;
	this->tile.min[2] = this->world.bound[4] / this->voxelScale;
	this->tile.max[0] = this->world.bound[1] / this->voxelScale;
	this->tile.max[1] = this->world.bound[3] / this->voxelScale;
	this->tile.max[2] = this->world.bound[5] / this->voxelScale;

	this->tile.calcFromMinMax();
}

void BBoxData::calcWorldFromTile()
{
	this->world.min[0] = this->tile.bound[0] * this->voxelScale;
	this->world.min[1] = this->tile.bound[2] * this->voxelScale;
	this->world.min[2] = this->tile.bound[4] * this->voxelScale;
	this->world.max[0] = this->tile.bound[1] * this->voxelScale;
	this->world.max[1] = this->tile.bound[3] * this->voxelScale;
	this->world.max[2] = this->tile.bound[5] * this->voxelScale;

	this->world.calcFromMinMax();
}

void BifRenderInfo::calcTileVolumes(Bifrost::API::Layout& layout)
{
	int maxDepth = layout.maxDepth();
	double voxelScale = layout.voxelScale();

	// calc volume of each tile in world space
	for (int depth = 0; depth <= maxDepth; depth++) {
		int depthWidth = layout.tileDimInfo(depth).depthWidth;
		double tileWorldSize = float(depthWidth) * voxelScale;
		this->levelVolume[depth] = tileWorldSize * tileWorldSize * tileWorldSize;
		// printf("Volume for level %d: %.2f %d\n", depth, this->levelVolume[depth], depthWidth);
	}

	// store volume of voxel as well
	this->levelVolume[8] = voxelScale * voxelScale * voxelScale;
	// printf("Volume for level 8: %.2f 1\n", this->levelVolume[8]);
}

void BifRenderInfo::dump()
{
	printf("\nBifRenderInfo Dump:\n");
	printf("\tRangeStrategy: %d\n", this->rangeStrategy);

	printf("\n\tLevelVolume:\n");
	for (unsigned i = 0; i < 9; i++ ) {
		printf("\t\t%d: %f\n", i, this->levelVolume[i]);
	}

	printf("\n\tmaxDepth: %d\n", this->maxDepth);

	printf("\n");
}

void FrameData::dump()
{
	printf("\nFrameData Dump:\n");
	printf("\tLayout: %s\n", this->layout.name().c_str());
	printf("\tsrcChannel: %s\n", this->srcChannel.name().c_str());
	if ( this->infCubeBlendChannel.valid() ) {
		printf("\tinfiniteBlendingChannel: %s\n", this->infCubeBlendChannel.name().c_str());
	}
	printf("\tvelU: %s\n", this->velocityChannelU.name().c_str());
	printf("\tvelV: %s\n", this->velocityChannelV.name().c_str());
	printf("\tvelW: %s\n", this->velocityChannelW.name().c_str());

	printf("\n\tPrimvars:\n");
	for (unsigned i = 0; i < reportChannels.size(); i++ ) {
		printf("\t\t%s\n", this->reportChannels[i].name().c_str());
	}

	printf("\n\tmotionBlur: %u\n", this->motionBlur);
	printf("\tshutter: %f %f\n", this->shutter[0], this->shutter[1]);
	printf("\tvelocityScale: %f\n", this->velocityScale);
	printf("\tvelocityExists: %u\n", this->velocityExists);

	printf("\ttileWorldSize: %f\n", this->tileWorldSize);
	printf("\tvoxelScale: %f\n", this->voxelScale);

	printf("\n\tBbox:\n");
	printf("\t\tSim World: \t%.2f %.2f %.2f   %.2f %.2f %.2f\n",	this->bboxSim.world.min[0], this->bboxSim.world.min[1], this->bboxSim.world.min[2],
																	this->bboxSim.world.max[0], this->bboxSim.world.max[1], this->bboxSim.world.max[2]);
	printf("\t\tSim Tile : \t%.2f %.2f %.2f   %.2f %.2f %.2f\n",	this->bboxSim.tile.min[0], this->bboxSim.tile.min[1], this->bboxSim.tile.min[2],
																	this->bboxSim.tile.max[0], this->bboxSim.tile.max[1], this->bboxSim.tile.max[2]);
	printf("\n");
}

void FrameData::init()
{
    this->hullCount = 8;
	this->offsetSize = 0.0f;
	this->bifInfo.rangeStrategy = LEVELFULLCHECK;
	this->diagnostics = ALL;

	this->metaDataLoaded = -1;
	this->dropletThreshold = -1;
	this->isAdaptive = false;

	// init counters
	this->bifInfo.rangeTotalCount = 0;
	this->bifInfo.rangeFrustumBailOut = 0;
	this->bifInfo.rangeCacheReuse = 0;
	this->bifInfo.rangeCount = 0;
	this->bifInfo.rangeInfCubeBailOut = 0;
	this->bifInfo.rangeTileCountBailOut = 0;
	this->bifInfo.boxCacheReuse = 0;
	this->bifInfo.boxCount = 0;
	this->bifInfo.intersectWithSimCount = 0;
	this->bifInfo.intersectWithInfCubeCount = 0;
	this->bifInfo.insideInfCubeCount = 0;
	this->bifInfo.calcInfCubeFieldCallCount = 0;
	this->bifInfo.calcInfCubeFieldOutsideCount = 0;
	this->bifInfo.calcInfCubeFieldInsideCount = 0;
	this->bifInfo.calcInfCubeFieldIntersectCount = 0;
	this->bifInfo.calcInfCubeGradientCallCount = 0;

	this->idExists = false;
	this->smoothChannelExists = false;
	this->infCubeBlendChannelExists = false;
	this->error = false;
	this->uvNeeded = false;
	this->speedNeeded = false;
	this->maxElementCount = 0;
	this->presenceNeeded = false;

	this->doNotRender = false;
}

void PrimitivesFrameData::init()
{
	this->error = false;
	this->hotData = false;
}

//
// ImplicitsInputData functions
//

void ImplicitsInputData::printParameters( bool isParticleCache )
{
	printf("\nParameters for the Plugin:\n");
	
	// get string data
    //if ( this->hotData ) {
    //	printf("\tUsing in memory data!\n");
    //} else {
    //	printf("\tfilename: %s\n", this->bifFilename);
    //}
	printf("\tdistanceChannel: %s\n", this->inputChannelName);
	printf("\tprimVarNames: %s\n", this->primVarNames);

	printf("\n\tnarrowBandThicknessInVoxels: %f\n", this->narrowBandThicknessInVoxels);
	//printf("\tstepSize: %f\n", this->stepSize);
	//printf("\n\tsampleRate: %d\n", this->sampleRate);
	printf("\n\tvelocityScale: %f fps: %f\n", this->velocityScale, this->fps);

	if ( this->cullSides.on ) {
		printf("\n\tcullSidesStart: %f\n", this->cullSides.start);
		printf("\tcullSidesEnd: %f\n", this->cullSides.end);
		printf("\tcullDepthAtStartInVoxels: %f\n\n", this->cullSides.depthAtStartInVoxels);
	} else {
		printf("\n\tcullSides: OFF\n\n");
	}

	printf("\tdilateAmount: %f\n", this->dilateAmount);
	printf("\terodeAmount: %f\n", this->erodeAmount);
    if ( this->smooth.amount > 0 ) {
		printf("\tsmoothKernelSize: %d\n", this->smooth.amount);
	
		switch (this->smooth.mode ) {
			case kMeanValue:
				printf("\tsmoothMode: Mean Value\n");
				break;
			case kGaussian:
				printf("\tsmoothMode: Gaussian\n");
				break;
			case kMedianValue:
				printf("\tsmoothMode: Median\n");
				break;
			case kCurvatureFlow:
				printf("\tsmoothMode: Curvature Flow\n");
				break;
			case kLaplacianFlow:
                printf("\tsmoothMode: LaplacianFlow\n");
				break;
			default:
				break;
		}
		printf("\tsmoothIterations: %d\n", this->smooth.iterations);
		printf("\tsmoothWeight: %f\n", this->smooth.weight);
		printf("\tsmoothBlendChannel: %s\n", this->smooth.channelName);
		printf("\tsmoothRemapMin: %f\n", this->smooth.remapMin);
		printf("\tsmoothRemapMax: %f\n", this->smooth.remapMax);
		if ( this->smooth.remapInvert ) {
			printf("\tsmoothRemapInvert: ON\n\n");
		} else {
			printf("\tsmoothRemapInvert: OFF\n\n");
		}
	} else {
		printf("\tsmoothing: OFF\n\n");
	}

	if ( this->clip.on ) {
		printf("\tclipBox: ON\n");
		printf("\tMinX: %f\n", this->clip.minX);
		printf("\tMaxX: %f\n", this->clip.maxX);
		printf("\tMinY: %f\n", this->clip.minY);
		printf("\tMaxY: %f\n", this->clip.maxY);
		printf("\tMinZ: %f\n", this->clip.minZ);
		printf("\tMaxZ: %f\n\n", this->clip.maxZ);
	} else {
		printf("\tclipBox: OFF\n\n");
	}


	if ( this->infCube.on ) {
		printf("\tinfCubeBlending: ON\n");
		printf("\tinfCubeBlendingChannel: %s\n", this->infCube.channelName);
		if ( this->infCube.outputType == OUTPUT_ALL ) {
			printf("\tinfCubeOutputType: ALL\n");
		} else {
			printf("\tinfCubeOutputType: SIMONLY\n");
		}

		printf("\tsimWaterLevel: %f\n", this->infCube.simWaterLevel);

		printf("\tinfCubeTopCenterX: %f\n", this->infCube.topCenterX);
		printf("\tinfCubeTopCenterY: %f\n", this->infCube.topCenterY);
		printf("\tinfCubeTopCenterZ: %f\n", this->infCube.topCenterZ);
		printf("\tinfCubeDimX: %f\n", this->infCube.dimX);
		printf("\tinfCubeDimZ: %f\n", this->infCube.dimZ);
		printf("\tblendType: %d\n", this->infCube.blendType);
		printf("\tinfCubeBlendStart: %f\n", this->infCube.blendStart);
		printf("\tinfCubeBlendEnd: %f\n", this->infCube.blendEnd);
		printf("\tblendingChannelRemapMin: %f\n", this->infCube.remapMin);
		printf("\tblendingChannelRemapMax: %f\n", this->infCube.remapMax);
		if ( this->infCube.remapInvert ) {
			printf("\tblendingChannelRemapInvert: ON\n\n");
		} else {
			printf("\tblendingChannelRemapInvert: OFF\n\n");
		}
	} else {
		printf("\tinfCubeBlending: OFF\n\n");
	}

	if ( isParticleCache ) {
		printf("\tresolutionFactor: %f\n", this->resolutionFactor);
		printf("\tdropletRevealFactor: %f\n", this->dropletRevealFactor);
		printf("\tsurfaceRadius: %f\n", this->surfaceRadius);
		printf("\tdropletRadius: %f\n", this->dropletRadius);
		printf("\tmaxVolumeOfHolesToClose: %f\n", this->maxVolumeOfHolesToClose);
		if ( this->doMorphologicalDilation ) {
			printf("\tdoMorphologicalDilation: ON\n");
		} else {
			printf("\tdoMorphologicalDilation: OFF\n");
		}
		if ( this->doErodeSheetsAndDroplets ) {
			printf("\tdoErodeSheetsAndDroplets: ON\n\n");
		} else {
			printf("\tdoErodeSheetsAndDroplets: OFF\n\n");
		}
	}

	printf("\tDEBUG: %u\n\n", this->diagnostics.DEBUG);
}

//
// VolumeData functions
//

void VolumeInputData::printParameters( bool isParticleCache )
{
	printf("\nParameters for the Plugin:\n");
	
	// get string data
    //if ( this->hotData ) {
    //	printf("\tUsing in memory data!\n");
    //} else {
    //	printf("\tfilename: %s\n", this->bifFilename);
    //}
	printf("\tinputChannel: %s\n", this->inputChannelName);
	printf("\tprimVarNames: %s\n", this->primVarNames);

	printf("\tchannelScale: %f\n", this->channelScale);
	printf("\tvelocityScale: %f fps: %f\n", this->velocityScale, this->fps);
	printf("\tspaceScale: %f\n", this->spaceScale);

    if ( this->smooth.amount > 0 ) {
		printf("\n\tsmoothKernelSize: %d\n", this->smooth.amount);
	
		switch (this->smooth.mode ) {
			case kMeanValue:
				printf("\tsmoothMode: Mean Value\n");
				break;
			case kGaussian:
				printf("\tsmoothMode: Gaussian\n");
				break;
			case kMedianValue:
				printf("\tsmoothMode: Median\n");
				break;
			case kCurvatureFlow:
				printf("\tsmoothMode: Curvature Flow\n");
				break;
			case kLaplacianFlow:
				printf("\tsmoothMode: LaplacianFlow\n");
				break;
			default:
				break;
		}
		printf("\tsmoothBlendChannel: %s\n", this->smooth.channelName);
		printf("\tsmoothIterations: %d\n", this->smooth.iterations);
		printf("\tsmoothWeight: %f\n", this->smooth.weight);
		printf("\tsmoothRemapMin: %f\n", this->smooth.remapMin);
		printf("\tsmoothRemapMax: %f\n", this->smooth.remapMax);
		if ( this->smooth.remapInvert ) {
			printf("\tsmoothRemapInvert: ON\n\n");
		} else {
			printf("\tsmoothRemapInvert: OFF\n\n");
		}
	} else {
		printf("\n\tsmoothing: OFF\n\n");
	}

	if ( this->clip.on ) {
		printf("\tclipBox: ON\n");
        printf("\tMinX: %f\n", this->clip.minX);
        printf("\tMaxX: %f\n", this->clip.maxX);
        printf("\tMinY: %f\n", this->clip.minY);
        printf("\tMaxY: %f\n", this->clip.maxY);
        printf("\tMinZ: %f\n", this->clip.minZ);
        printf("\tMaxZ: %f\n\n", this->clip.maxZ);
	} else {
		printf("\tclipBox: OFF\n\n");
	}

	if ( isParticleCache ) {
		printf("\tsplatResolutionFactor: %f\n", this->splatResolutionFactor);
		printf("\tskip: %d\n", this->skip);
		printf("\tsplatSamples: %d\n", this->splatSamples);	
		printf("\tsplatMinRadius: %f\n", this->splatMinRadius);
		printf("\tsplatMaxRadius: %f\n", this->splatMaxRadius);
		printf("\tsplatSurfaceAttract: %f\n", this->splatSurfaceAttract);
		printf("\tsplatFalloffType: %d\n", this->splatFalloffType);
		printf("\tsplatFalloffStart: %f\n", this->splatFalloffStart);
		printf("\tsplatFalloffEnd: %f\n", this->splatFalloffEnd);
		printf("\tsplatDisplacement: %f\n", this->splatDisplacement);
		printf("\tsplatNoiseFreq: %f\n\n", this->splatNoiseFreq);
	}

	printf("\tDEBUG: %u\n\n", this->diagnostics.DEBUG);
}

void PrimitivesInputData::printParameters()
{
	printf("\nInput Parameters:\n");

	// string args
	if ( this->hotData ) {
		printf("\tUsing in memory data!\n");
	} else {
		printf("\tfilename: %s\n", this->bifFilename);
	}

	// numeric args
	if (this->renderType == PRIM_POINT) {
		printf("\trenderType: POINT\n");
	} else if (this->renderType == PRIM_SPHERE) {
		printf("\trenderType: SPHERE\n");
	} else if (this->renderType == PRIM_BLOB) {
		printf("\trenderType: BLOB\n");
	} else if (this->renderType == PRIM_CURVE) {
		printf("\trenderType: CURVE\n");
	} else {
		printf("\trenderType: NOT SUPPORTED\n");
	}
	printf("\n");

	printf("\trenderChannelName: %s\n", this->inputChannelName);
	printf("\trenderChannelScale: %f\n", this->channelScale);
	if ( this->exportNormalAsPrimvar ) {
		printf("\texportNormalAsPrimvar: ON\n\n");
	} else {
		printf("\texportNormalAsPrimvar: OFF\n\n");
	}

	if ( this->fps > 0.0f ) {
		printf( "\tvelocityScale: %f fps: %f\n", this->velocityScale * this->fps, this->fps );
	} else {
		printf( "\tvelocityScale: %f\n", this->velocityScale );
	}
	printf("\tspaceScale: %f\n\n", this->spaceScale);

	printf("\tskip: %d\n", this->skip);
	printf("\tchunkSize: %d\n\n", this->chunkSize);

	if ( this->clip.on ) {
		printf("\tclipBox: ON\n");
        printf("\tMinX: %f\n", this->clip.minX);
        printf("\tMaxX: %f\n", this->clip.maxX);
        printf("\tMinY: %f\n", this->clip.minY);
        printf("\tMaxY: %f\n", this->clip.maxY);
        printf("\tMinZ: %f\n", this->clip.minZ);
        printf("\tMaxZ: %f\n\n", this->clip.maxZ);
	} else {
		printf("\tclipBox: OFF\n\n");
	}

	printf("\tradius: %f\n", this->radius);
	if ( this->useChannelToModulateRadius ) {
		printf("\tuseChannelToModulateRadius: ON\n");
	} else {
		printf("\tuseChannelToModulateRadius: OFF\n");
	}

	if ( this->camRadius ) {
		printf("\tcamRadius: ON\n");
		printf("\t\tcamRadiusStartDistance: %f\n", this->camRadiusStartDistance);
		printf("\t\tcamRadiusEndDistance: %f\n", this->camRadiusEndDistance);
		printf("\t\tcamRadiusStartFactor: %f\n", this->camRadiusStartFactor);
		printf("\t\tcamRadiusEndFactor: %f\n", this->camRadiusEndFactor);
		printf("\t\tcamRadiusFactorExponent: %f\n", this->camRadiusFactorExponent);
	} else {
		printf("\tcamRadius: OFF\n");
	}

	printf("\n\tmpSamples: %d\n", this->mpSamples);
	if ( this->mpSamples > 1 ) {
		printf("\tmpMinRadius: %f\n", this->mpMinRadius);
		printf("\tmpMaxRadius: %f\n", this->mpMaxRadius);
		printf("\tmpSurfaceAttract: %f\n", this->mpSurfaceAttract);
		printf("\tmpFalloffType: %d\n", this->mpFalloffType);
		printf("\tmpFalloffStart: %f\n", this->mpFalloffStart);
		printf("\tmpFalloffEnd: %f\n", this->mpFalloffEnd);
		printf("\tmpdisplacementValue: %f\n", this->mpDisplacementValue);
		printf("\tmpdisplacementNoiseFrequency: %f\n", this->mpDisplacementNoiseFrequency);
	}

	printf("\n\tprimVarNames: %s\n", this->primVarNames);

	if ( this->diagnostics.DEBUG > 0) {
		printf("\n\tDEBUG: ON\n");
	} else {
		printf("\n\tDEBUG: OFF\n");
	}
	printf("\n");
}

void PrimitivesInputData::checkParameters()
{
	if (this->camRadiusEndDistance <= this->camRadiusStartDistance) {
		printf("[BIFROST PRIMITIVES] CameraBasedRadius endDistance is smaller than startDistance!\n"); 
		this->error = true;
	} else {
		this->camRadiusDistanceRange = this->camRadiusEndDistance - this->camRadiusStartDistance;
		this->camRadiusFactorRange = this->camRadiusEndFactor - this->camRadiusStartFactor;
	}

	this->mpFalloffRange = this->mpFalloffEnd - this->mpFalloffStart;

	// check skip bound
	if ( this->skip < 1 ) {
		this->skip = 1;
	}

	if ( this->mpSamples < 1 ) {
		printf("[BIFROST PRIMITIVES] samples per particle is less than 1!\n"); 
		this->error = true;
	}

	// if there is something fishy error out
	if ( this->clip.on ) {
        if ( this->clip.maxX <= this->clip.minX || this->clip.maxY <= this->clip.minY || this->clip.maxZ <= this->clip.minZ ) {
			printf("[BIFROST PRIMITIVES] ClipBox coordinates are wrong: one or more of MaxXYZ is smaller than MinXYZ\n");
			this->error = true;
		}
	}

	// calc velocityScale according to fps
	if ( this->fps > FLT_EPSILON ) {
		// if fps is 0.0 assume absolute velocityScale
		// if not calc correct velocity scale
		this->velocityScale = this->velocityScale / this->fps;
	}

	if ( this->hotData && strcmp ( this->bifrostObjectName, "none" ) == 0 ) {
		// although hotdata is true, there is no in memory bif structure
		// so reset hotdata
		this->hotData = false;
		printf("[BIFROST PRIMITIVES] HotData is on but there is no in memory bif!\n");
	}
}

void ImplicitsInputData::checkParameters()
{
	this->cullSides.range = this->cullSides.end - this->cullSides.start;
	this->diagnostics.silent = 0;

    if ( this->narrowBandThicknessInVoxels < 1.0f ) {
		printf("\t\tnarrowBandThickness can not be smaller than a voxel!\n\t\tThis will be reset to default 1.5 voxels!\n");
		this->narrowBandThicknessInVoxels = 1.5f;
	}

	if ( this->cullSides.start < 0.0f || this->cullSides.start > 1.0f || this->cullSides.start > this->cullSides.end ) {
		printf("CullSideStart value is not valid. It is either:\n\tLess than zero!\n\tGreater than one!\n\tGreater than cullSidesEnd value!\n");
		this->error = true;
	}
	if ( this->cullSides.end < 0.0f || this->cullSides.end > 1.0f || this->cullSides.end < this->cullSides.start ) {
		printf("CullSideEnd value is not valid. It is either:\n\tLess than zero!\n\tGreater than one!\n\tLess than cullSidesStart value!\n");
		this->error = true;
	}

	if ( this->clip.on ) {
        if ( this->clip.maxX <= this->clip.minX || this->clip.maxY <= this->clip.minY || this->clip.maxZ <= this->clip.minZ ) {
			printf("ClipBox coordinates are wrong: one or more of MaxXYZ is smaller than MinXYZ\n");
			this->error = true;
		}
	}

	// calc velocityScale according to fps
	if ( this->fps > FLT_EPSILON ) {
		// if fps is 0.0 assume absolute velocityScale
		// if not calc correct velocity scale
		this->velocityScale = this->velocityScale / this->fps;
	}

//	if ( this->hotData && strcmp ( this->bifrostObjectName, "none" ) == 0 ) {
//		// although hotdata is true, there is no in memory bif structure
//		// so reset hotdata
//		this->hotData = false;
//		printf("HotData is on but there is no in memory bif!\n");
//	}
}


void VolumeInputData::checkParameters()
{
	this->splatFalloffRange = this->splatFalloffEnd - this->splatFalloffStart;
	this->diagnostics.silent = 0;

	if ( this->clip.on ) {
        if ( this->clip.maxX <= this->clip.minX || this->clip.maxY <= this->clip.minY || this->clip.maxZ <= this->clip.minZ ) {
			printf("ClipBox coordinates are wrong: one or more of MaxXYZ is smaller than MinXYZ\n");
			this->error = true;
		}
	}

	// calc velocityScale according to fps
	if ( this->fps > FLT_EPSILON ) {
		// if fps is 0.0 assume absolute velocityScale
		// if not calc correct velocity scale
		this->velocityScale = this->velocityScale / this->fps;
	}

//	if ( this->hotData && strcmp ( this->bifrostObjectName, "none" ) == 0 ) {
//		// although hotdata is true, there is no in memory bif structure
//		// so reset hotdata
//		this->hotData = false;
//		printf("HotData is on but there is no in memory bif!\n");
//	}
}

}}
