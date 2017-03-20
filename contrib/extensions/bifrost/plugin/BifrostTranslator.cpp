#include <string>
#include <vector>
#include <iostream>

#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MFileObject.h>
#include <maya/MTime.h>

#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>

#include <ai.h>

#include "extension/Extension.h"

#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_objectuserdata.h>
#include "BifrostTranslator.h"



using namespace Bifrost::RenderCore;

// This is a replica of CDagTranslator::ExportMatrix to have spacescale injected into the output matrix attribute
void BifrostTranslator::ExportMatrixWithSpaceScale(AtNode* node, float spaceScale)
{
	AtMatrix matrix;
	GetMatrix(matrix);

	// inject our scaling values here
    AtVector s(spaceScale, spaceScale, spaceScale);
    AtMatrix scalingMatrix = AiM4Scaling(s);
	// apply scaling to input matrix
    matrix = AiM4Mult(scalingMatrix, matrix);

	if ( !IsExportingMotion() ) {
		if ( IsMotionBlurEnabled( MTOA_MBLUR_OBJECT ) && RequiresMotionData() ) {
			AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
			AiArraySetMtx(matrices, GetMotionStep(), matrix);
			AiNodeSetArray(node, "matrix", matrices);
		} else {
			AiNodeSetMatrix(node, "matrix", matrix);
		}
	} else if ( IsMotionBlurEnabled( MTOA_MBLUR_OBJECT ) && RequiresMotionData() ) {
		AtArray* matrices = AiNodeGetArray(node, "matrix");
		if (matrices) {
			int step = GetMotionStep();
            if ( step >= (int)(AiArrayGetNumKeys(matrices) * AiArrayGetNumElements(matrices)) ) {
				AiMsgError("Matrix AtArray steps not set properly for %s",  m_dagPath.partialPathName().asChar());
			} else {
				AiArraySetMtx(matrices, step, matrix);
			}
		}
	}
}

AtNode* BifrostTranslator::CreateArnoldNodes()
{
	c_renderType = NONE;
	MFnDagNode  bifrostDesc;
	MDagPath bifrostPath = m_dagPath;

	bifrostDesc.setObject( bifrostPath.node() );

	int renderType = (RenderType)bifrostDesc.findPlug("bifrostRenderType").asInt();

	switch ( renderType ) {
		case 0:
			{
				c_bifType = (BIFType)bifrostDesc.findPlug("aeroRenderData").asInt();
				c_renderType = AERO;
			}
			break;
		case 1:
		case 2:
			{
				c_bifType = (BIFType)bifrostDesc.findPlug("renderData").asInt();
				int renderMethod = bifrostDesc.findPlug("renderMethod").asInt();
				if ( renderMethod == 0 ) {
					c_renderType = LIQUID_POLYMESH;
				} else {
					c_renderType = LIQUID_IMPLICIT;
				}
			}
			break;
		case 3:
			{
				c_bifType = (BIFType)bifrostDesc.findPlug("pointRenderData").asInt();
				c_renderType = POINT;
			}
			break;
		default:
			c_renderType = NONE;
			break;
	}

	// if we don't know about this render type return null
	if ( c_renderType == NONE ) {
		return NULL;
	}

	MPlug objectPlug = bifrostDesc.findPlug("object");
	MString objStr = objectPlug.asString();
   
	c_object = std::string( objStr.asChar() );

	MFnDependencyNode bfContainer(objectPlug.source().node());

	switch (c_renderType) {
		{
			case AERO:
				MPlug containerPlug = bfContainer.findPlug("aeroCacheProperties");
				MFnDependencyNode bfAeroProps(containerPlug.source().node());
      
				MString cacheDir = bfAeroProps.findPlug("aeroCachePath").asString();
				MString cacheName = bfAeroProps.findPlug("aeroCacheFileName").asString();

				MString fullBifPath;
				if ( c_bifType == 0 ) {
					fullBifPath = cacheDir + cacheName + "/AeroObject/AeroObject_volume";
				} else {
					fullBifPath = cacheDir + cacheName + "/AeroObject/AeroObject_particle";
				}
				c_file = fullBifPath.asChar();
				break;
		}

		{
			case POINT:
				MPlug containerPlug = bfContainer.findPlug("foamCacheProperties");
				MFnDependencyNode bfPointProps(containerPlug.source().node());
      
				MString cacheDir = bfPointProps.findPlug("foamCachePath").asString();
				MString cacheName = bfPointProps.findPlug("foamCacheFileName").asString();

				MString fullBifPath = cacheDir + cacheName + "/Foam/Foam_particle";
				c_file = fullBifPath.asChar();
				break;
		}

		{
			case LIQUID_IMPLICIT:
			case LIQUID_POLYMESH:
				MPlug containerPlug = bfContainer.findPlug("liquidCacheProperties");
				MFnDependencyNode bfLiquidProps(containerPlug.source().node());
      
				MString cacheDir = bfLiquidProps.findPlug("liquidCachePath").asString();
				MString cacheName = bfLiquidProps.findPlug("liquidCacheFileName").asString();

				MString fullBifPath;
				if ( c_bifType == 0 ) {
					fullBifPath = cacheDir + cacheName + "/voxel_liquid/voxel_liquid_volume";
				} else {
					fullBifPath = cacheDir + cacheName + "/voxel_liquid/voxel_liquid_particle";
				}

				c_file = fullBifPath.asChar();
				break;
		}

		default:
			break;
	}

	switch (c_renderType) {
		case AERO:
            return AddArnoldNode( "bifrostVolume" );
		case LIQUID_POLYMESH:
            return AddArnoldNode( "bifrostPolymesh" );
		case LIQUID_IMPLICIT:
            return AddArnoldNode( "bifrostImplicits" );
		case POINT:
            return AddArnoldNode( "bifrostPrimitives" );
		default:
			AiMsgError( "[Bifrost AI Converter]: Type not implemented yet : %s", c_object.c_str() );
			break;
	}

	// for non-implemented Render Types
	return NULL;
}

void BifrostTranslator::Export( AtNode* instance )
{
	switch (c_renderType) {
		case AERO:
			ExportAero( instance );
			break;
		case LIQUID_POLYMESH:
			ExportLiquidPolyMesh( instance );
			break;
		case LIQUID_IMPLICIT:
			ExportLiquidImplicit( instance );
			break;
		case POINT:
			ExportPoint( instance );
			break;
		default:
			break;
	}
}

void BifrostTranslator::ExportPoint(AtNode *shape)
{
	CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );
	bool hotData = objectRef.objectExists();




	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist(frame);

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Point data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data
	AiNodeSetStr( shape, "bifrostObjectName", c_object.c_str() );

	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );

	// setup motion blur stuff

	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
		GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
	}


	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );


	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );


	// global params

	int intAttrVal = bifrostDesc.findPlug( "renderPrimitiveType" ).asInt();
	AiNodeSetInt( shape, "renderType", intAttrVal );


	float attrVal = bifrostDesc.findPlug( "pointChannelScale" ).asFloat();
	AiNodeSetFlt( shape, "channelScale", attrVal );


	bool boolAttrVal = bifrostDesc.findPlug( "exportNormalAsPrimvar" ).asBool();
	AiNodeSetBool( shape, "exportNormalAsPrimvar", boolAttrVal );


	attrVal = bifrostDesc.findPlug( "pointVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal );

	const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

	AiNodeSetFlt(shape, "fps", fps);


	attrVal = bifrostDesc.findPlug( "pointSpaceScale" ).asFloat();
	AiNodeSetFlt( shape, "spaceScale", attrVal );


	intAttrVal = bifrostDesc.findPlug( "pointSkip" ).asInt();
	AiNodeSetInt( shape, "skip", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "chunkSize" ).asInt();
	AiNodeSetInt( shape, "chunkSize", intAttrVal );


	// clip params

	boolAttrVal = bifrostDesc.findPlug( "pointClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );


	attrVal = bifrostDesc.findPlug( "pointClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );


	// radius params

	attrVal = bifrostDesc.findPlug( "pointRadius" ).asFloat();
	AiNodeSetFlt( shape, "pointRadius", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "useChannelToModulateRadius" ).asBool();
	AiNodeSetBool( shape, "useChannelToModulateRadius", boolAttrVal );


	boolAttrVal = bifrostDesc.findPlug( "camRadiusOn" ).asBool();
	AiNodeSetBool( shape, "camRadiusOn", boolAttrVal );


	attrVal = bifrostDesc.findPlug( "camRadiusStartDistance" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusStartDistance", attrVal );


	attrVal = bifrostDesc.findPlug( "camRadiusEndDistance" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusEndDistance", attrVal );


	attrVal = bifrostDesc.findPlug( "camRadiusStartFactor" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusStartFactor", attrVal );


	attrVal = bifrostDesc.findPlug( "camRadiusEndFactor" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusEndFactor", attrVal );


	attrVal = bifrostDesc.findPlug( "camRadiusFactorExponent" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusFactorExponent", attrVal );


	// multi sample params

	intAttrVal = bifrostDesc.findPlug( "mpSamples" ).asInt();
	AiNodeSetInt( shape, "mpSamples", intAttrVal );


	attrVal = bifrostDesc.findPlug( "mpMinRadius" ).asFloat();
	AiNodeSetFlt( shape, "mpMinRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "mpMaxRadius" ).asFloat();
	AiNodeSetFlt( shape, "mpMaxRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "mpSurfaceAttract" ).asFloat();
	AiNodeSetFlt( shape, "mpSurfaceAttract", attrVal );


	intAttrVal = bifrostDesc.findPlug( "mpFalloffType" ).asInt();
	AiNodeSetInt( shape, "mpFalloffType", intAttrVal );


	attrVal = bifrostDesc.findPlug( "mpFalloffStart" ).asFloat();
	AiNodeSetFlt( shape, "mpFalloffStart", attrVal );


	attrVal = bifrostDesc.findPlug( "mpFalloffEnd" ).asFloat();
	AiNodeSetFlt( shape, "mpFalloffEnd", attrVal );


	attrVal = bifrostDesc.findPlug( "mpDisplacementValue" ).asFloat();
	AiNodeSetFlt( shape, "mpDisplacementValue", attrVal );


	attrVal = bifrostDesc.findPlug( "mpDisplacementNoiseFrequency" ).asFloat();
	AiNodeSetFlt( shape, "mpDisplacementNoiseFrequency", attrVal );


    intAttrVal = bifrostDesc.findPlug( "debug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "silent" ).asInt();
	AiNodeSetInt( shape, "silent", intAttrVal );

	// Determine the final BIF file name
	const float frame = (float)MAnimControl::currentTime().value();

	std::string particleFilename;
	{
		const int frameNumber  = (int)floorf(frame);

		std::string path = c_file + ".#.bif";
		particleFilename = Bifrost::API::File::resolveFramePadding( path.c_str(), frameNumber ).c_str();
	}


	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );


	MString strAttrVal = bifrostDesc.findPlug( "pointPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );


	strAttrVal = bifrostDesc.findPlug( "pointChannel" ).asString();
	AiNodeSetStr( shape, "inputChannelName", strAttrVal.asChar() );

	// export matrix
	ExportMatrix( shape );
   
	// export shaders
	if ( RequiresShaderExport() ) {
		ExportBifrostShader();
	}

	// export lighting
	ExportLightLinking( shape );
}

void BifrostTranslator::ExportAero(AtNode *shape)
{
	CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );

	// do we have hot data
	bool hotData = objectRef.objectExists();

	// declare output variables



	// export hotdata status 
	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist(frame);

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Aero data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data
	AiNodeSetStr( shape, "bifrostObjectName", c_object.c_str() );

	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );

	// setup motion blur stuff

	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
		GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
	}


	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );


	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );

	// first export matrix
	float spaceScale = bifrostDesc.findPlug( "aeroSpaceScale" ).asFloat();
	ExportMatrixWithSpaceScale( shape, spaceScale );

	// global params

	float attrVal = bifrostDesc.findPlug( "aeroChannelScale" ).asFloat();
	AiNodeSetFlt( shape, "channelScale", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal );

	const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

	AiNodeSetFlt(shape, "fps", fps);

	// set space scale to 1 as it is incorporated into matrix attribute

	AiNodeSetFlt( shape, "spaceScale", 1 );

	// smooth params

	bool boolAttrVal = bifrostDesc.findPlug( "aeroSmoothOn" ).asBool();
	AiNodeSetBool( shape, "smoothOn", boolAttrVal );


	int intAttrVal = bifrostDesc.findPlug( "aeroSmoothMode" ).asInt();
	AiNodeSetInt( shape, "smoothMode", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "aeroSmoothAmount" ).asInt();
	AiNodeSetInt( shape, "smoothAmount", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "aeroSmoothIterations" ).asInt();
	AiNodeSetInt( shape, "smoothIterations", intAttrVal );


	attrVal = bifrostDesc.findPlug( "aeroSmoothWeight" ).asFloat();
	AiNodeSetFlt( shape, "smoothWeight", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroSmoothRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMin", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroSmoothRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMax", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "aeroSmoothRemapInvert" ).asBool();
	AiNodeSetBool( shape, "smoothRemapInvert", boolAttrVal );


	// clip params

	boolAttrVal = bifrostDesc.findPlug( "aeroClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );


	attrVal = bifrostDesc.findPlug( "aeroClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );


	// splat params

	attrVal = bifrostDesc.findPlug( "splatResolutionFactor" ).asFloat();
	AiNodeSetFlt( shape, "splatResolutionFactor", attrVal );


	intAttrVal = bifrostDesc.findPlug( "aeroSkip" ).asInt();
	AiNodeSetInt( shape, "skip", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "splatSamples" ).asInt();
	AiNodeSetInt( shape, "splatSamples", intAttrVal );


	attrVal = bifrostDesc.findPlug( "splatMinRadius" ).asFloat();
	AiNodeSetFlt( shape, "splatMinRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "splatMaxRadius" ).asFloat();
	AiNodeSetFlt( shape, "splatMaxRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "splatSurfaceAttract" ).asFloat();
	AiNodeSetFlt( shape, "splatSurfaceAttract", attrVal );


	intAttrVal = bifrostDesc.findPlug( "splatFalloffType" ).asInt();
	AiNodeSetInt( shape, "splatFalloffType", intAttrVal );


	attrVal = bifrostDesc.findPlug( "splatFalloffStart" ).asFloat();
	AiNodeSetFlt( shape, "splatFalloffStart", attrVal );


	attrVal = bifrostDesc.findPlug( "splatFalloffEnd" ).asFloat();
	AiNodeSetFlt( shape, "splatFalloffEnd", attrVal );


	attrVal = bifrostDesc.findPlug( "splatDisplacement" ).asFloat();
	AiNodeSetFlt( shape, "splatDisplacement", attrVal );


	attrVal = bifrostDesc.findPlug( "splatNoiseFreq" ).asFloat();
	AiNodeSetFlt( shape, "splatNoiseFreq", attrVal );


	// diagnotics params

    intAttrVal = bifrostDesc.findPlug( "debug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "silent" ).asInt();
	AiNodeSetInt( shape, "silent", intAttrVal );

	// Determine the final BIF file name
	const float frame = (float)MAnimControl::currentTime().value();

	std::string particleFilename;
	{
		const int frameNumber  = (int)floorf(frame);

		std::string path = c_file + ".#.bif";
		particleFilename = Bifrost::API::File::resolveFramePadding( path.c_str(), frameNumber ).c_str();
	}


	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );


	MString strAttrVal = bifrostDesc.findPlug( "aeroChannel" ).asString();
	AiNodeSetStr( shape, "inputChannelName", strAttrVal.asChar() );


	strAttrVal = bifrostDesc.findPlug( "aeroSmoothChannel" ).asString();
	AiNodeSetStr( shape, "smoothChannelName", strAttrVal.asChar() );


	strAttrVal = bifrostDesc.findPlug( "aeroPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );

	// export shaders
	if ( RequiresShaderExport() ) {
		ExportBifrostShader();

		// we need to hack this because a volume shader doesn't work with a 
		// MayaShadingGroup node in the middle, so I'm bypassing it
		AtNode *sgNode = (AtNode*)AiNodeGetPtr(shape, "shader");
		if (sgNode && AiNodeIsLinked(sgNode, "beauty")) {
			AtNode *shad = AiNodeGetLink(sgNode, "beauty");
			AiNodeSetPtr(shape, "shader", shad);
		}
	}

	// export lighting
	ExportLightLinking( shape );
}

void BifrostTranslator::getLiquidAttributes( MFnDagNode&  bifrostDesc, AtNode *shape )
{
	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );
	AiNodeSetByte( shape, "visibility", AI_RAY_ALL );

	float spaceScale = bifrostDesc.findPlug( "liquidSpaceScale" ).asFloat();
	ExportMatrixWithSpaceScale( shape, spaceScale );

	// setup motion blur stuff

	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
		GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
	}


	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );


	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	// culling params

	bool boolAttrVal = bifrostDesc.findPlug( "cullSidesOn" ).asBool();
	AiNodeSetBool( shape, "cullSidesOn", boolAttrVal );


	float attrVal = bifrostDesc.findPlug( "cullSidesStart" ).asFloat();
	AiNodeSetFlt( shape, "cullSidesStart", attrVal );


	attrVal = bifrostDesc.findPlug( "cullSidesEnd" ).asFloat();
	AiNodeSetFlt( shape, "cullSidesEnd", attrVal );


	attrVal = bifrostDesc.findPlug( "cullDepthAtStartInVoxels" ).asFloat();
	AiNodeSetFlt( shape, "cullDepthAtStartInVoxels", attrVal );

	// common attributes

	attrVal = bifrostDesc.findPlug( "liquidVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal );

	const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

	AiNodeSetFlt(shape, "fps", fps);

	// set space scale to 1 as it is incorporated into matrix attribute

	AiNodeSetFlt( shape, "spaceScale", 1 );


	// post process params

	attrVal = bifrostDesc.findPlug( "dilateAmount" ).asFloat();
	AiNodeSetFlt( shape, "dilateAmount", attrVal );


	attrVal = bifrostDesc.findPlug( "erodeAmount" ).asFloat();
	AiNodeSetFlt( shape, "erodeAmount", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "smoothOn" ).asBool();
	AiNodeSetBool( shape, "smoothOn", boolAttrVal );


	int intAttrVal = bifrostDesc.findPlug( "smoothMode" ).asInt();
	AiNodeSetInt( shape, "smoothMode", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "smoothAmount" ).asInt();
	AiNodeSetInt( shape, "smoothAmount", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "smoothIterations" ).asInt();
	AiNodeSetInt( shape, "smoothIterations", intAttrVal );


	attrVal = bifrostDesc.findPlug( "smoothWeight" ).asFloat();
	AiNodeSetFlt( shape, "smoothWeight", attrVal );


	attrVal = bifrostDesc.findPlug( "smoothRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMin", attrVal );


	attrVal = bifrostDesc.findPlug( "smoothRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMax", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "smoothRemapInvert" ).asBool();
	AiNodeSetBool( shape, "smoothRemapInvert", boolAttrVal );


	// clip params

	boolAttrVal = bifrostDesc.findPlug( "liquidClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );


	// infcube blending params

	boolAttrVal = bifrostDesc.findPlug( "infCubeBlendingOn" ).asBool();
	AiNodeSetBool( shape, "infCubeBlendingOn", boolAttrVal );


	intAttrVal = bifrostDesc.findPlug( "infCubeOutputType" ).asInt();
	AiNodeSetInt( shape, "infCubeOutputType", intAttrVal );


	attrVal = bifrostDesc.findPlug( "simWaterLevel" ).asFloat();
	AiNodeSetFlt( shape, "simWaterLevel", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeTopCenterX" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterX", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeTopCenterY" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterY", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeTopCenterZ" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterZ", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeDimX" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimX", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeDimY" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimY", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeDimZ" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimZ", attrVal );


	intAttrVal = bifrostDesc.findPlug( "blendType" ).asInt();
	AiNodeSetInt( shape, "blendType", intAttrVal );


	attrVal = bifrostDesc.findPlug( "infCubeBlendStart" ).asFloat();
	AiNodeSetFlt( shape, "infCubeBlendStart", attrVal );


	attrVal = bifrostDesc.findPlug( "infCubeBlendEnd" ).asFloat();
	AiNodeSetFlt( shape, "infCubeBlendEnd", attrVal );


	attrVal = bifrostDesc.findPlug( "blendingChannelRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "blendingChannelRemapMin", attrVal );


	attrVal = bifrostDesc.findPlug( "blendingChannelRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "blendingChannelRemapMax", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "blendingChannelRemapInvert" ).asBool();
	AiNodeSetBool( shape, "blendingChannelRemapInvert", boolAttrVal );


	// particle to voxel conversion params

	attrVal = bifrostDesc.findPlug( "implicitResolutionFactor" ).asFloat();
	AiNodeSetFlt( shape, "implicitResolutionFactor", attrVal );


	attrVal = bifrostDesc.findPlug( "implicitDropletRevealFactor" ).asFloat();
	AiNodeSetFlt( shape, "implicitDropletRevealFactor", attrVal );


	attrVal = bifrostDesc.findPlug( "implicitSurfaceRadius" ).asFloat();
	AiNodeSetFlt( shape, "implicitSurfaceRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "implicitDropletRadius" ).asFloat();
	AiNodeSetFlt( shape, "implicitDropletRadius", attrVal );


	attrVal = bifrostDesc.findPlug( "implicitMaxVolumeOfHolesToClose" ).asFloat();
	AiNodeSetFlt( shape, "implicitMaxVolumeOfHolesToClose", attrVal );


	boolAttrVal = bifrostDesc.findPlug( "doMorphologicalDilation" ).asBool();
	AiNodeSetBool( shape, "doMorphologicalDilation", boolAttrVal );


	boolAttrVal = bifrostDesc.findPlug( "doErodeSheetsAndDroplets" ).asBool();
	AiNodeSetBool( shape, "doErodeSheetsAndDroplets", boolAttrVal );


	// diagnostics params

    intAttrVal = bifrostDesc.findPlug( "debug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "silent" ).asInt();
	AiNodeSetInt( shape, "silent", intAttrVal );

	// Determine the final BIF file name
	const float frame = (float)MAnimControl::currentTime().value();

	std::string particleFilename;
	{
		const int frameNumber  = (int)floorf(frame);

		std::string path = c_file + ".#.bif";
		particleFilename = Bifrost::API::File::resolveFramePadding( path.c_str(), frameNumber ).c_str();
	}


	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );


	MString strAttrVal = bifrostDesc.findPlug( "distanceChannel" ).asString();
	AiNodeSetStr( shape, "distanceChannel", strAttrVal.asChar() );
    std::cerr << "EXPORT distanceChannel = " << strAttrVal << std::endl;


	strAttrVal = bifrostDesc.findPlug( "filterBlendingChannel" ).asString();
	AiNodeSetStr( shape, "filterBlendingChannel", strAttrVal.asChar() );


	strAttrVal = bifrostDesc.findPlug( "infiniteSurfaceBlendingChannel" ).asString();
	AiNodeSetStr( shape, "infiniteSurfaceBlendingChannel", strAttrVal.asChar() );


	strAttrVal = bifrostDesc.findPlug( "liquidPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );
}

void BifrostTranslator::ExportLiquidPolyMesh(AtNode *shape)
{
    std::cerr << "EXPORT LIQUID POLY" << std::endl;
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );
	CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );

	bool hotData = objectRef.objectExists();

	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
        const bool cacheExist = objectRef.checkCacheFileExist( frame );

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Liquid data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data

	AiNodeSetStr( shape, "bifrostObjectName", c_object.c_str() );

	// set user params

	int intAttrVal = bifrostDesc.findPlug( "mesherAlgo" ).asInt();
	AiNodeSetInt( shape, "mesherAlgo", intAttrVal );


	intAttrVal = bifrostDesc.findPlug( "sampleRate" ).asInt();
	AiNodeSetInt( shape, "sampleRate", intAttrVal );

	getLiquidAttributes( bifrostDesc, shape );
    //AiNodeSetStr(shape, "bifFilename", "/home/beauchc/bifrost_debug/whale/cache/bifrost/class04_whale_simulation_end/bifrostLiquidContainer1/voxel_liquid/voxel_liquid_volume.0035.bif");
    //AiNodeSetBool( shape, "hotData", 0 );AiNodeSetBool( shape, "hotData", 0 );


	// export shaders
	if ( RequiresShaderExport() ) {
		ExportBifrostShader();
	}

	// export lighting
	ExportLightLinking( shape );
}

void BifrostTranslator::ExportLiquidImplicit(AtNode *shape)
{
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );
	CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );

	bool hotData = objectRef.objectExists();


	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist( frame );

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Liquid data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data

	AiNodeSetStr( shape, "bifrostObjectName", c_object.c_str() );

	AiNodeSetStr( shape, "solver", "levelset" );
	AiNodeSetFlt( shape, "threshold", 0.0f );

	// add implicit specific attributes

	float attrVal = bifrostDesc.findPlug( "narrowBandThicknessInVoxels" ).asFloat();
	AiNodeSetFlt( shape, "narrowBandThicknessInVoxels", attrVal );


	attrVal = bifrostDesc.findPlug( "liquidStepSize" ).asFloat();
	AiNodeSetFlt( shape, "liquidStepSize", attrVal );

	MString strAttrVal = bifrostDesc.findPlug( "distanceChannel" ).asString();
	AiNodeSetStr(shape, "field_channel", strAttrVal.asChar() );

	int intAttrVal = bifrostDesc.findPlug( "sampleRate" ).asInt();
	AiNodeSetUInt( shape, "samples", (unsigned int) intAttrVal );

	// set user params
	getLiquidAttributes( bifrostDesc, shape );

	// export shaders
	if ( RequiresShaderExport() ) {
		ExportBifrostShader();
		// we need to hack this because a volume shader doesn't work with a 
		// MayaShadingGroup node in the middle, so I'm bypassing it
		AtNode *sgNode = (AtNode*)AiNodeGetPtr(shape, "shader");
		if (sgNode && AiNodeIsLinked(sgNode, "beauty")) {
			AtNode *shad = AiNodeGetLink(sgNode, "beauty");
			AiNodeSetPtr(shape, "shader", shad);
		}
	}

	ExportLightLinking(shape);
}

void BifrostTranslator::ExportMotion( AtNode* shape )
{
	// Check if motionblur is enabled and early out if it's not.
	if ( !IsMotionBlurEnabled() ) {
		return;
	}

	// Set transform matrix
	ExportMatrix( shape );
}

void BifrostTranslator::ExportBifrostShader()
{
	AtNode *node = GetArnoldNode();

	 MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
	 if ( !shadingGroupPlug.isNull() ) {
		AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
		if (rootShader != NULL) {
	       // Push the shader in the vector to be assigned later to mtoa_shading_groups
		   AiNodeSetPtr(node, "shader", rootShader);
		}
	}
}

void BifrostTranslator::RequestUpdate()
{
	SetUpdateMode( AI_RECREATE_NODE );
	CShapeTranslator::RequestUpdate();
}

// ***************
//  ALL THE CODE BELLOW SHOULD BE REMOVED WHEN THESE ATTRIBUTES BECOME PART OF
//  BIFROST SHAPE IN MAYA 2018
// ***************

#define ADD_DSTR(longName, value) \
    data.name = data.shortName = longName;\
    data.stringDefault = MString(value); \
    helper.MakeInputString(data);

#define ADD_DFLT(longName, value) \
    data.name = data.shortName = longName;\
    data.hasSoftMin = data.hasSoftMax = true; \
    data.softMin.FLT() = 0.f;\
    data.softMax.FLT() = 1.f; \
    data.hasMin = data.hasMax = false; \
    data.defaultValue.FLT() = value;\
    helper.MakeInputFloat(data);

#define ADD_DINT(longName, value) \
    data.name = data.shortName = longName;\
    data.hasSoftMin = data.hasSoftMax = true; \
    data.softMin.INT() = 0;\
    data.softMax.INT() = 1; \
    data.hasMin = data.hasMax = false; \
    data.defaultValue.INT() = value;\
    helper.MakeInputInt(data);

#define ADD_DBOOL(longName, value) \
    data.name = data.shortName = longName;\
    data.defaultValue.BOOL() = value;\
    helper.MakeInputBoolean(data);

#define ADD_DFLT3(longName, x, y, z) \
    data.name = data.shortName = longName;\
    data.defaultValue.VEC() = AtVector(x,y,z);\
    helper.MakeInputVector(data);

#define ADD_DSMOOTH_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("MeanValue");\
        enums.append("Gaussian");\
        enums.append("Median");\
        enums.append("LaplacianFlow");\
        enums.append("CurvatureFlow");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DFALLOFF_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Linear");\
        enums.append("Smooth");\
        enums.append("Smoother");\
        enums.append("None");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DDATA_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Voxels");\
        enums.append("Particles");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

void BifrostTranslator::NodeInitializer( CAbTranslator context )
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    // AERO
    ADD_DDATA_ENUM("aeroRenderData");
    ADD_DFLT("aeroChannelScale", 1.f);
    ADD_DFLT("aeroVelocityScale", 1.f);
    ADD_DFLT("aeroSpaceScale", 1.f);
    ADD_DINT("aeroSilent", 0);

    ADD_DSTR("aeroPrimVars", "");
    ADD_DSTR("aeroChannel", "smoke");
    ADD_DSTR("aeroSmoothChannel", "");

    ADD_DBOOL("aeroSmoothOn", false);
    ADD_DSMOOTH_ENUM("aeroSmoothMode");
    ADD_DINT("aeroSmoothAmount", 1);
    ADD_DINT("aeroSmoothIterations", 1);
    ADD_DFLT("aeroSmoothWeight", 1.f);
    ADD_DFLT("aeroSmoothRemapMin", 0.f);
    ADD_DFLT("aeroSmoothRemapMax", 1.f);
    ADD_DBOOL("aeroSmoothRemapInvert", false);

    ADD_DBOOL("aeroClipOn", false);
    ADD_DFLT3("aeroClipMin", 0.f, 0.f, 0.f);
    ADD_DFLT3("aeroClipMax", 1.f, 1.f, 1.f);

    ADD_DFLT("splatResolutionFactor", 1.f);
    ADD_DINT("aeroSkip", 1);
    ADD_DINT("splatSamples", 1);
    ADD_DFLT("splatMinRadius", 1.f);
    ADD_DFLT("splatMaxRadius", 1.f);
    ADD_DFLT("splatSurfaceAttract", 1.f);
    ADD_DFALLOFF_ENUM("splatFalloffType");
    ADD_DFLT("splatFalloffStart", 1.2f);
    ADD_DFLT("splatFalloffEnd", 1.2f);
    ADD_DFLT("splatDisplacement", 8.f);
    ADD_DFLT("splatNoiseFreq", 1);

    // LIQUID
    data.name = data.shortName = "renderMethod";
    {\
        MStringArray enums;\
        enums.append("BifrostMesher");\
        enums.append("Implicit");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);
    ADD_DDATA_ENUM("renderData");
    data.name = data.shortName = "mesherAlgo";
    {\
        MStringArray enums;\
        enums.append("Marching Cubes");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);
    ADD_DINT("sampleRate", 1);

    ADD_DFLT("liquidVelocityScale", 1.f);
    ADD_DFLT("liquidSpaceScale", 1.f);

    ADD_DSTR("liquidPrimVars", "vorticity");
    ADD_DSTR("distanceChannel", "distance");
    ADD_DSTR("filterBlendingChannel", "");
    ADD_DSTR("infiniteSurfaceBlendingChannel", "");

    ADD_DFLT("narrowBandThicknessInVoxels", 1.5f);
    ADD_DFLT("liquidStepSize", 0.05f);
    ADD_DBOOL("cullSidesOn", false);
    ADD_DFLT("cullSidesStart", 0.75f);
    ADD_DFLT("cullSidesEnd", 0.95f);
    ADD_DFLT("cullDepthAtStartInVoxels", 10.0f);

    ADD_DFLT("dilateAmount", 0.f);
    ADD_DFLT("erodeAmount", 0.f);
    ADD_DBOOL("smoothOn", false);
    ADD_DSMOOTH_ENUM("smoothMode");
    ADD_DINT("smoothAmount", 1);
    ADD_DINT("smoothIterations", 1);
    ADD_DFLT("smoothRemapMin", 0.f);
    ADD_DFLT("smoothRemapMax", 1.f);
    ADD_DBOOL("smoothRemapInvert", false);

    ADD_DBOOL("liquidClipOn", false);
    ADD_DFLT3("liquidClipMin", 0.f, 0.f, 0.f);
    ADD_DFLT3("liquidClipMax", 1.f, 1.f, 1.f);

    ADD_DBOOL("infCubeBlendingOn", false);
    data.name = data.shortName = "infCubeOutputType";
    {\
        MStringArray enums;\
        enums.append("SimOnly");\
        enums.append("All");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);
    ADD_DFLT("simWaterLevel", 0.f);
    ADD_DFLT3("infCubeTopCenter", 0.f, 0.f, 0.f);
    ADD_DFLT3("infCubeDim", 100.f, 100.f, 100.f);
    ADD_DFALLOFF_ENUM("blendType");
    ADD_DFLT("infCubeBlendStart", 0.5f);
    ADD_DFLT("infCubeBlendEnd", 0.99f);
    ADD_DFLT("blendingChannelRemapMin", 0.f);
    ADD_DFLT("blendingChannelRemapMax", 1.f);
    ADD_DBOOL("blendingChannelRemapInvert", false);

    ADD_DFLT("implicitResolutionFactor", 1.f);
    ADD_DFLT("implicitDropletRevealFactor", 3.0f);
    ADD_DFLT("implicitSurfaceRadius", 1.2f);
    ADD_DFLT("implicitDropletRadius", 1.2f);
    ADD_DFLT("implicitMaxVolumeOfHolesToClose", 8.f);
    ADD_DBOOL("doMorphologicalDilation", true);
    ADD_DBOOL("doErodeSheetsAndDroplets", true);

    // FOAM
    data.name = data.shortName = "renderPrimitiveType";
    {\
        MStringArray enums;\
        enums.append("Point");\
        enums.append("Sphere");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);
    ADD_DFLT("pointVelocityScale", 1.f);
    ADD_DFLT("pointSpaceScale", 1.f);
    ADD_DINT("pointSkip", 1);
    ADD_DINT("chunkSize", 100000);

    ADD_DSTR("pointPrimVars", "");

    ADD_DSTR("pointChannel", "density");
    ADD_DFLT("pointChannelScale", 1.f);
    ADD_DBOOL("exportNormalAsPrimvar", false);

    ADD_DFLT("pointClipOn", false);
    ADD_DFLT3("pointClipMin", 0.f, 0.f, 0.f);
    ADD_DFLT3("pointClipMax", 1.f, 1.f, 1.f);
    ADD_DFLT("pointRadius", 1.f);
    ADD_DBOOL("useChannelToModulateRadius", true);
    ADD_DBOOL("camRadiusOn", false);
    ADD_DFLT("camRadiusStartDistance", 0.f);
    ADD_DFLT("camRadiusEndDistance", 100.0f);
    ADD_DFLT("camRadiusStartFactor", 1.f);
    ADD_DFLT("camRadiusEndFactor", 2.0f);
    ADD_DFLT("camRadiusFactorExponent", 1.f);
    ADD_DINT("mpSamples", 1);
    ADD_DFLT("mpMinRadius", 0.f);
    ADD_DFLT("mpMaxRadius", 1.f);
    ADD_DFLT("mpSurfaceAttract", 0.f);
    ADD_DSMOOTH_ENUM("mpFalloffType");
    ADD_DFLT("mpFalloffStart", 0.8f);
    ADD_DFLT("mpFalloffEnd", 1.f);
    ADD_DFLT("mpDisplacementValue", 0.f);
    ADD_DFLT("mpDisplacementNoiseFrequency", 1.f);

    ADD_DINT("debug", 1);
    ADD_DINT("silent", 0);
}
