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

#include <bifrostrendercore/bifrostrender_tbb.h>

namespace Bifrost{
namespace RenderCore{

// paralel mesh value sampler
// Although it is designed to be run in parallel it can also be run as a serial process
// Look in Corevisitors.cpp where this is called. There is a commented out serial call for this in that file.
void MeshSampler::operator() ( const tbb::blocked_range<int>& r ) const
{
	// first define samplers
	Bifrost::API::VoxelSampler *gradientSampler = NULL;
	if ( c_mesher->c_frameData->exportNormals ) {
		gradientSampler = new Bifrost::API::VoxelSampler( c_mesher->c_frameData->srcChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
	}

	Bifrost::API::VoxelSampler *velXSampler, *velYSampler, *velZSampler;
	velXSampler = velYSampler = velZSampler = NULL;
	if ( c_mesher->c_frameData->motionBlur ) {
		velXSampler = new Bifrost::API::VoxelSampler( c_mesher->c_frameData->velocityChannelU.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
		velYSampler = new Bifrost::API::VoxelSampler( c_mesher->c_frameData->velocityChannelV.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
		velZSampler = new Bifrost::API::VoxelSampler( c_mesher->c_frameData->velocityChannelW.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
	}

	// create samplers for primvars too
	std::vector<Bifrost::API::VoxelSampler *> primVarSamplers;
	for ( unsigned var = 0; var < c_mesher->c_frameData->primVars.size(); var++ ) {
		Bifrost::API::VoxelChannel tmpChannel = ( Bifrost::API::VoxelChannel ) c_mesher->c_frameData->primVars[var].channel;
		Bifrost::API::VoxelSampler *tmpSampler = new Bifrost::API::VoxelSampler( tmpChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::TileSpace ) );
		primVarSamplers.push_back( tmpSampler );
	}

	for ( size_t i = r.begin(); i != r.end(); i++ ) {
		amino::Math::vec3f orgPos = c_mesher->getMergedVertexPosition( i );

		amino::Math::vec3f finalPos = c_dx * ( orgPos + c_mesher->c_voxelOffset );

		c_mesher->setMergedVertexPosition( i, finalPos );

		// get the normal
		if ( c_mesher->c_frameData->exportNormals ) {
			amino::Math::vec3f vertexNormal;
			gradientSampler->sampleGradient<float>( orgPos, vertexNormal );
			c_mesher->setVertexNormal( i, vertexNormal );
		}

		// process velocities if needed
		if ( c_mesher->c_frameData->motionBlur ) {
			float velX = velXSampler->sample<float>( orgPos );
			float velY = velYSampler->sample<float>( orgPos );
			float velZ = velZSampler->sample<float>( orgPos );
			c_mesher->setVertexVelocity( i, amino::Math::vec3f( velX, velY, velZ ) );
		}

		// process primvars
		for ( unsigned var = 0; var < c_mesher->c_frameData->primVars.size(); var++ ) {
			c_mesher->c_primVars[ c_mesher->c_frameData->primVars[var].name ][i] = primVarSamplers[var]->sample<float>( orgPos );
		}
	}
}

}}

