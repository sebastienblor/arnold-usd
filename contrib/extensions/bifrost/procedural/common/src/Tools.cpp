#include "Tools.h"

using namespace Bifrost::RenderCore;

void getASSData ( void *frameData, void *inputData, PluginType pluginType )
{
	AtNode* camNode = AiUniverseGetCamera();

	if ( pluginType == PLUGIN_PRIMITIVES ) {
		((PrimitivesFrameData *)frameData)->shutter[0] = ((PrimitivesInputData *)inputData)->shutterStart;
		((PrimitivesFrameData *)frameData)->shutter[1] = ((PrimitivesInputData *)inputData)->shutterEnd;
	} else if ( pluginType == PLUGIN_IMPLICITS ) {
		((FrameData *)frameData)->shutter[0] = ((ImplicitsInputData *)inputData)->shutterStart;
		((FrameData *)frameData)->shutter[1] = ((ImplicitsInputData *)inputData)->shutterEnd;
	} else if ( pluginType == PLUGIN_MESH ) {
		((PolyMeshFrameData *)frameData)->shutter[0] = ((PolyMeshInputData *)inputData)->shutterStart;
		((PolyMeshFrameData *)frameData)->shutter[1] = ((PolyMeshInputData *)inputData)->shutterEnd;
	} else {
		((FrameData *)frameData)->shutter[0] = ((VolumeInputData *)inputData)->shutterStart;
		((FrameData *)frameData)->shutter[1] = ((VolumeInputData *)inputData)->shutterEnd;
	}

	// get screen window
	if ( pluginType != PLUGIN_PRIMITIVES && pluginType != PLUGIN_MESH ) {
		// renderState->GetAttribute( "ShadingRate", &( ((FrameData *)frameData)->shadingRate), sizeof( float ), &resultType, &resultCount );

		// get screenWindow
		AtPoint2 tmp = AiNodeGetPnt2( camNode, "screen_window_min" );
		( (FrameData *)frameData )->screenWindow[ 0 ] = tmp.x;
		( (FrameData *)frameData )->screenWindow[ 1 ] = tmp.y;

		tmp = AiNodeGetPnt2( camNode, "screen_window_max" );
		( (FrameData *)frameData )->screenWindow[ 2 ] = tmp.x;
		( (FrameData *)frameData )->screenWindow[ 3 ] = tmp.y;
	}
}

Bifrost::API::VoxelSampler * getAndSetThreadDataAI( std::string idString, Bifrost::API::VoxelChannel channel, void *usrData )
{
	Bifrost::API::VoxelSampler *channelSampler = NULL;
	std::map<AtByte, Bifrost::API::VoxelSampler *>::iterator it;
	AIProcNodeData *nodeData = (AIProcNodeData *) usrData;

	if ( ! nodeData->samplerPool.empty() && nodeData->samplerPool.count( nodeData->bifrostCtx->tid ) != 0 ) {
		// we already have a sampler for this
		channelSampler = nodeData->samplerPool[ nodeData->bifrostCtx->tid ];
	} else {
		//printf("creating new sampler for %d\n", (int ) nodeData->bifrostCtx->tid );
		channelSampler = new Bifrost::API::VoxelSampler( channel.createSampler( Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::SamplerSpace::WorldSpace ) );
		nodeData->samplerPool.insert( std::pair<AtByte, Bifrost::API::VoxelSampler *>( nodeData->bifrostCtx->tid, channelSampler ) );
	}

	return channelSampler;
}

CvFloat calcNoiseAI ( CvPoint3 noiseP )
{
	AtPoint evalPoint  = AiPoint( noiseP[0], noiseP[1], noiseP[2] );
	return AiPerlin4( evalPoint, 0.0f );
}

void transformPointsAI( CvPoint finalWp[2], float shutterStart, void *usrData )
{
	AIProcNodeData *nodeData = (AIProcNodeData *) usrData;

	AtPoint evalPoint  = AiPoint( finalWp[ 0 ][ 0 ], finalWp[ 1 ][ 1 ], finalWp[ 0 ][ 2 ] );
	AtPoint xformed;
	AiM4PointByMatrixMult ( &xformed, nodeData->obj2Cam, &evalPoint );
	finalWp[ 0 ][ 0 ] = xformed.x;
	finalWp[ 0 ][ 1 ] = xformed.y;
	finalWp[ 0 ][ 2 ] = xformed.z;


	evalPoint  = AiPoint( finalWp[ 1 ][ 0 ], finalWp[ 1 ][ 1 ], finalWp[ 1 ][ 2 ] );
	AiM4PointByMatrixMult ( &xformed, nodeData->obj2Cam, &evalPoint );
	finalWp[ 1 ][ 0 ] = xformed.x;
	finalWp[ 1 ][ 1 ] = xformed.y;
	finalWp[ 1 ][ 2 ] = xformed.z;
}

void setExportTokenAI( std::vector<CvToken>& toks, ExportTokens token )
{
	switch ( token ) {
		case TOK_POSITION:
			toks.push_back(const_cast<char*>("position uniform FLOAT"));
			break;
		case TOK_RADIUS:
			toks.push_back(const_cast<char*>("radius uniform FLOAT"));
			break;
		case TOK_TRANSPARENCY:
			toks.push_back(const_cast<char*>("transparency uniform FLOAT"));
			break;
		case TOK_NORMAL:
			toks.push_back(const_cast<char*>("N uniform VECTOR"));
			break;
		case TOK_GRADIENT_NORMAL:
			toks.push_back(const_cast<char*>("densityNormal uniform VECTOR"));
			break;
		case TOK_PRIMVAR:
			break;
	}
}

std::string getPrimVarTokenAI ( TokenClass tokenClass, Bifrost::API::DataType dataType, std::string primVarName )
{
	std::string classString;

	switch ( tokenClass ) {
		case TOKCLASS_VARYING:
			classString = "varying";
			break;
		case TOKCLASS_UNIFORM:
			classString = "uniform";
			break;
		case TOKCLASS_CONSTANT:
			classString = "constant";
	}

	if ( primVarName == "speed" ) {
		return primVarName + " " + classString + " FLOAT";
	} else {
		switch ( dataType ) {
			case Bifrost::API::FloatType:
				return primVarName + " " + classString + " FLOAT";
				break;
			case Bifrost::API::FloatV2Type:
				return primVarName + " " + classString + " POINT2";
				break;
			case Bifrost::API::FloatV3Type:
				return primVarName + " " + classString + " VECTOR";
				break;
			default:
				return "";
				break;
		}
	}
}
