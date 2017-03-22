#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

#include <ai.h>

#include <bifrostrendercore/bifrostrender_defs.h>
#include <bifrostrendercore/bifrostrender_types.h>
#include <bifrostrendercore/bifrostrender_math.h>
#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_primvars.h>
#include <bifrostrendercore/bifrostrender_visitors.h>
#include <Tools.h>

#include <Implicit.h>

#include <bifrostrendercore/bifrostrender_filters.h>

#include <bifrostrendercore/bifrostrender_objectuserdata.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

AI_VOLUME_NODE_EXPORT_METHODS(BifrostImplicitsMtd)

using namespace Bifrost::RenderCore;

namespace
{
void cleanVoxelSamplers(Bifrost::API::VoxelSampler **samplers)
{
	for (int i = 0; i < AI_MAX_THREADS; ++i) {
		if ( samplers[i] ) {
			delete samplers[i];
			samplers[i] = 0;
		}
	}
}

struct BifrostImplicitsUserData {
	Bifrost::API::VoxelSampler **channelSamplers;
	std::map<AtString, int> channelSamplerIndexes;
	int srcChannelSamplerIndexStart;

    Bifrost::API::Component voxelComponent;

    ImplicitsInputData *inputData;
    FrameData *frameData;
};

} // namespace

node_parameters
{
    AiParameterFlt("narrowBandThicknessInVoxels",0);
    AiParameterFlt("liquidStepSize",0);
    ImplicitNodeDeclareParameters(params, nentry);
}

volume_create
{
    BifrostImplicitsUserData *pdata = new BifrostImplicitsUserData;

    ImplicitsInputData *inData = new ImplicitsInputData;
    DUMP(inData->narrowBandThicknessInVoxels = AiNodeGetFlt(node, "narrowBandThicknessInVoxels"));
    DUMP(inData->stepSize = AiNodeGetFlt(node, "liquidStepSize"));
    getNodeParameters(inData, node);
    FrameData* frameData = new FrameData;
    pdata->inputData = inData;
    pdata->frameData = frameData;

    InitializeImplicit(inData, frameData, &data->bbox);

    data->private_info = pdata;
    //data->bbox.min = AtVector(-10000,-10000,-10000);// = frameData->bboxSim;
    //data->bbox.max = AtVector(10000,10000,10000);// = frameData->bboxSim;
    data->auto_step_size = inData->stepSize;

    Bifrost::API::Component component = frameData->inSS.components()[0];
    DUMP(component.name());
    Bifrost::API::RefArray channels = component.channels();
    DUMP(channels.count());

	// now allocate space for samplers
    pdata->voxelComponent = component;
    pdata->srcChannelSamplerIndexStart = -1;
	int samplerChannelCount = 0;

    for ( unsigned int i = 0; i < channels.count(); i++ ) {
		Bifrost::API::Channel channel = (Bifrost::API::Channel) channels[i];
		AtString tmpString ( channel.name().c_str() );
		int startIndex = samplerChannelCount * AI_MAX_THREADS;
        pdata->channelSamplerIndexes[ tmpString ] = startIndex;

        if ( tmpString == Bifrost::API::String( inData->inputChannelName ) ) {
            pdata->srcChannelSamplerIndexStart = startIndex;
		}

        if ( inData->diagnostics.DEBUG > 0 ) {
            printf("%s - %d - %d\n", channel.name().c_str(), pdata->channelSamplerIndexes[ tmpString ], pdata->srcChannelSamplerIndexStart );
		}

		samplerChannelCount++;		
	}

    pdata->channelSamplers = ( Bifrost::API::VoxelSampler ** ) malloc( samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );
    memset( pdata->channelSamplers, 0, samplerChannelCount * AI_MAX_THREADS * sizeof( void * ) );

	//
	//
	// FINISH
	//
	//
    printEndOutput( "[BIFROST IMPLICITS] END OUTPUT", inData->diagnostics );

    return true;
}

volume_cleanup
{
    BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*) data->private_info;
    if(!userData){
        return false;
    }
    FrameData *frameData = userData->frameData;
    ImplicitsInputData *inData = userData->inputData;

    //if ( frameData && inData->hotData ) {
    //    Bifrost::API::File::deleteFolder( frameData->tmpFolder );
    //}
    if ( inData ) {
        free( inData->bifFilename );
        free( inData->inputChannelName );
        free( inData->smooth.channelName );
        free( inData->infCube.channelName );
        free( inData->primVarNames );
        free( inData->bifrostObjectName );
    }

    delete userData;
    data->private_info = NULL;
	return true;
}

volume_sample
{
	if (!data->private_info) return false;

	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*) data->private_info;
	ImplicitsInputData *inData = userData->inputData;

	amino::Math::vec3f pos;
	pos[0] = sg->P.x;
	pos[1] = sg->P.y;
	pos[2] = sg->P.z;

	int samplerIndexStart = userData->channelSamplerIndexes[ channel ];
	Bifrost::API::VoxelSampler *threadSampler = userData->channelSamplers[ samplerIndexStart + sg->tid ];

	if (threadSampler == 0) {
        if ( inData->diagnostics.DEBUG > 0 ) {
			printf( "Creating a new sampler for channel %s and thread %d...\n", channel.c_str(), sg->tid );
		}
		Bifrost::API::VoxelChannel bifChannel = userData->voxelComponent.findChannel( channel.c_str() );

		threadSampler = new Bifrost::API::VoxelSampler( bifChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace ) );
		userData->channelSamplers[ samplerIndexStart + sg->tid ] = threadSampler;
	}

	*type = AI_TYPE_FLOAT;
	if ( samplerIndexStart >= userData->srcChannelSamplerIndexStart && samplerIndexStart < userData->srcChannelSamplerIndexStart + AI_MAX_THREADS ) {
		// this is sampling the input channel so we need to apply the scalar
        //value->FLT = inData->channelScale * threadSampler->sample<float>(pos);
        value->FLT() = threadSampler->sample<float>(pos);
	} else {
        value->FLT() = threadSampler->sample<float>(pos);
	}
	return true;
}

volume_gradient
{
	if (!data->private_info) return false;

	BifrostImplicitsUserData *userData = (BifrostImplicitsUserData*) data->private_info;
	ImplicitsInputData *inData = userData->inputData;

	amino::Math::vec3f pos;
	pos[0] = sg->P.x;
	pos[1] = sg->P.y;
	pos[2] = sg->P.z;

	int samplerIndexStart = userData->channelSamplerIndexes[ channel ];
	Bifrost::API::VoxelSampler *threadSampler = userData->channelSamplers[ samplerIndexStart + sg->tid ];

	if (threadSampler == 0) {
        if ( inData->diagnostics.DEBUG > 0 ) {
			printf( "Creating a new sampler for channel %s and thread %d...\n", channel.c_str(), sg->tid );
		}
		Bifrost::API::VoxelChannel bifChannel = userData->voxelComponent.findChannel( channel.c_str() );
        // should use appropriate interpolation type?
		threadSampler = new Bifrost::API::VoxelSampler( bifChannel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace ) );
		userData->channelSamplers[ samplerIndexStart + sg->tid ] = threadSampler;
	}

	amino::Math::vec3f normal;
	if ( samplerIndexStart >= userData->srcChannelSamplerIndexStart && samplerIndexStart < userData->srcChannelSamplerIndexStart + AI_MAX_THREADS ) {
		// this is sampling the input channel so we need to apply the scalar
		threadSampler->sampleGradient<float>(pos, normal);
	} else {
		threadSampler->sampleGradient<float>(pos, normal);
	}

    for(unsigned int i = 0; i < 3; ++i)
        (*gradient)[i] = normal[i];

	return true;
}

volume_ray_extents
{
    if(data->private_info) AiVolumeAddIntersection(info, t0, t1);
}
volume_update
{
	return true;
}
