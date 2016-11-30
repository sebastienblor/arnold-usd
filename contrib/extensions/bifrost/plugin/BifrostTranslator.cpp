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

#include "utils/time.h"
#include "scene/MayaScene.h"
#include "session/SessionOptions.h"
#include "extension/Extension.h"
#include "scene/MayaScene.h"

#include <bifrostrendercore/CoreTools.h>
#include <bifrostrendercore/CoreObjectUserData.h>
#include "BifrostTranslator.h"
#include <TranslatorUtils.h>

using namespace Bifrost::RenderCore;

AtNode* BifrostShapeTranslator::CreateArnoldNodes()
{
	c_renderType = NONE;
	MFnDagNode  bifrostDesc;
	MDagPath bifrostPath = m_dagPath;

	bifrostDesc.setObject( bifrostPath.node() );

	int renderType = (RenderType)bifrostDesc.findPlug("bifrostRenderType").asInt();
	switch ( renderType ) {
		case 0:
			c_bifType = (BIFType)bifrostDesc.findPlug("aeroRenderData").asInt();
			c_renderType = AERO;
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
			c_bifType = (BIFType)bifrostDesc.findPlug("pointRenderData").asInt();
			c_renderType = POINT;
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
			case LIQUID_POLYMESH:
				MPlug containerPlug = bfContainer.findPlug("liquidCacheProperties");
				MFnDependencyNode bfLiquidProps(containerPlug.source().node());

      
				MString cacheDir = bfLiquidProps.findPlug("liquidCachePath").asString();
				MString cacheName = bfLiquidProps.findPlug("liquidCacheFileName").asString();

				MString fullBifPath = cacheDir + cacheName + "/voxel_liquid/voxel_liquid";
				c_file = fullBifPath.asChar();
				break;
		}

		{
			case LIQUID_IMPLICIT:
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
			return AddArnoldNode("volume");
		case LIQUID_POLYMESH:
			return AddArnoldNode("procedural");
		case LIQUID_IMPLICIT:
			return AddArnoldNode("implicit");
		case POINT:
			return AddArnoldNode("procedural");
		default:
			AiMsgError("[Bifrost AI Convertor]: type not implemented yet : %s", c_object.c_str());
			break;
	}

	// for non-implemented Render Types
	return NULL;
}

void BifrostShapeTranslator::Export( AtNode* instance )
{
	Update( instance );
}

void BifrostShapeTranslator::Update( AtNode* node )
{
	switch (c_renderType) {
		case AERO:
			UpdateAero( node );
			break;
		case LIQUID_POLYMESH:
			UpdateLiquidPolyMesh( node );
			break;
		case LIQUID_IMPLICIT:
			UpdateLiquidImplicit( node );
			break;
		case POINT:
			UpdatePoint( node );
			break;
		default:
			break;
	}
}

void BifrostShapeTranslator::UpdatePoint(AtNode *shape)
{
	CoreObjectUserData objectRef(c_object, c_file);
	bool hotData = objectRef.objectExists();

	AiNodeDeclare( shape, "hotData", "constant BOOL" );
	AiNodeDeclare(shape, "objectName", "constant STRING");

	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist(frame);

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: point data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data
	AiNodeSetStr( shape, "objectName", c_object.c_str() );

	static const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

    static std::string strDSO = std::string( getenv( "MTOA_PATH" ) ) + std::string("/procedurals/BifrostPrimitives.dll");
	AiNodeSetStr(shape, "dso", strDSO.c_str());
	AiNodeSetBool( shape, "load_at_init", true );

	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );

	// setup motion blur stuff
	AiNodeDeclare( shape, "motionBlur", "constant BOOL" );
	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
        CArnoldSession* session = CMayaScene::GetArnoldSession();
		session->GetMotionRange( shutterStart, shutterEnd );
	}

	AiNodeDeclare( shape, "shutterStart", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );

	AiNodeDeclare( shape, "shutterEnd", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );

	AiNodeDeclare(shape, "fps", "constant FLOAT");
	AiNodeSetFlt(shape, "fps", fps);

	AiNodeDeclare( shape, "renderType", "constant INT" );
	int intAttrVal = bifrostDesc.findPlug( "renderPrimitiveType" ).asInt();
	AiNodeSetInt( shape, "renderType", intAttrVal );

	AiNodeDeclare( shape, "channelScale", "constant FLOAT" );
	float attrVal = bifrostDesc.findPlug( "pointChannelScale" ).asFloat();
	AiNodeSetFlt( shape, "channelScale", attrVal );

	AiNodeDeclare( shape, "useChannelGradientAsNormal", "constant BOOL" );
	bool boolAttrVal = bifrostDesc.findPlug( "useChannelGradientAsNormal" ).asBool();
	AiNodeSetBool( shape, "useChannelGradientAsNormal", boolAttrVal );

	AiNodeDeclare( shape, "exportNormalAsPrimvar", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "exportNormalAsPrimvar" ).asBool();
	AiNodeSetBool( shape, "exportNormalAsPrimvar", boolAttrVal );

	AiNodeDeclare( shape, "velocityScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal / fps );

	AiNodeDeclare( shape, "spaceScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointSpaceScale" ).asFloat();
	AiNodeSetFlt( shape, "spaceScale", attrVal );

	AiNodeDeclare( shape, "skip", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "pointSkip" ).asInt();
	AiNodeSetInt( shape, "skip", intAttrVal );

	AiNodeDeclare( shape, "chunkSize", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "chunkSize" ).asInt();
	AiNodeSetInt( shape, "chunkSize", intAttrVal );

	AiNodeDeclare( shape, "clipOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "pointClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );

	AiNodeDeclare( shape, "clipMinX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );

	AiNodeDeclare( shape, "clipMaxX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );

	AiNodeDeclare( shape, "clipMinY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );

	AiNodeDeclare( shape, "clipMaxY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );

	AiNodeDeclare( shape, "clipMinZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );

	AiNodeDeclare( shape, "clipMaxZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );

	AiNodeDeclare( shape, "pointRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointRadius" ).asFloat();
	AiNodeSetFlt( shape, "pointRadius", attrVal );

	AiNodeDeclare( shape, "pointRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "pointRadius" ).asFloat();
	AiNodeSetFlt( shape, "pointRadius", attrVal );

	AiNodeDeclare( shape, "useChannelToModulateRadius", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "useChannelToModulateRadius" ).asBool();
	AiNodeSetBool( shape, "useChannelToModulateRadius", boolAttrVal );

	AiNodeDeclare( shape, "camRadiusOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "camRadiusOn" ).asBool();
	AiNodeSetBool( shape, "camRadiusOn", boolAttrVal );

	AiNodeDeclare( shape, "camRadiusStartDistance", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "camRadiusStartDistance" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusStartDistance", attrVal );

	AiNodeDeclare( shape, "camRadiusEndDistance", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "camRadiusEndDistance" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusEndDistance", attrVal );

	AiNodeDeclare( shape, "camRadiusStartFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "camRadiusStartFactor" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusStartFactor", attrVal );

	AiNodeDeclare( shape, "camRadiusEndFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "camRadiusEndFactor" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusEndFactor", attrVal );

	AiNodeDeclare( shape, "camRadiusFactorExponent", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "camRadiusFactorExponent" ).asFloat();
	AiNodeSetFlt( shape, "camRadiusFactorExponent", attrVal );

	AiNodeDeclare( shape, "mpSamples", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "mpSamples" ).asInt();
	AiNodeSetInt( shape, "mpSamples", intAttrVal );

	AiNodeDeclare( shape, "mpMinRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpMinRadius" ).asFloat();
	AiNodeSetFlt( shape, "mpMinRadius", attrVal );

	AiNodeDeclare( shape, "mpMaxRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpMaxRadius" ).asFloat();
	AiNodeSetFlt( shape, "mpMaxRadius", attrVal );

	AiNodeDeclare( shape, "mpSurfaceAttract", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpSurfaceAttract" ).asFloat();
	AiNodeSetFlt( shape, "mpSurfaceAttract", attrVal );

	AiNodeDeclare( shape, "mpFalloffType", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "mpFalloffType" ).asInt();
	AiNodeSetInt( shape, "mpFalloffType", intAttrVal );

	AiNodeDeclare( shape, "mpFalloffStart", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpFalloffStart" ).asFloat();
	AiNodeSetFlt( shape, "mpFalloffStart", attrVal );

	AiNodeDeclare( shape, "mpFalloffEnd", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpFalloffEnd" ).asFloat();
	AiNodeSetFlt( shape, "mpFalloffEnd", attrVal );

	AiNodeDeclare( shape, "mpDisplacementValue", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpDisplacementValue" ).asFloat();
	AiNodeSetFlt( shape, "mpDisplacementValue", attrVal );

	AiNodeDeclare( shape, "mpDisplacementNoiseFrequency", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "mpDisplacementNoiseFrequency" ).asFloat();
	AiNodeSetFlt( shape, "mpDisplacementNoiseFrequency", attrVal );

	AiNodeDeclare( shape, "smoothing", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "meshingSmoothing" ).asInt();
	AiNodeSetInt( shape, "smoothing", intAttrVal );

	AiNodeDeclare( shape, "debug", "constant INT" );
    intAttrVal = bifrostDesc.findPlug( "aiDebug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );

	AiNodeDeclare( shape, "silent", "constant INT" );
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

	AiNodeDeclare(shape, "bifFilename", "constant STRING");
	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );

	AiNodeDeclare(shape, "primVarNames", "constant STRING");
	MString strAttrVal = bifrostDesc.findPlug( "pointPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );

	AiNodeDeclare(shape, "inputChannelName", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "pointChannel" ).asString();
	AiNodeSetStr( shape, "inputChannelName", strAttrVal.asChar() );

	// export matrix
    ExportMatrix( shape );
   
	// export shaders
	if (	(CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER ) ||
			CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines() ) {
		ExportBifrostShader();
	}

	// export lighting
	ExportLightLinking( shape );
}

void BifrostShapeTranslator::UpdateAero(AtNode *shape)
{
	CoreObjectUserData objectRef(c_object, c_file);
	bool hotData = objectRef.objectExists();

	AiNodeDeclare( shape, "hotData", "constant BOOL" );
	AiNodeDeclare(shape, "objectName", "constant STRING");

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
	AiNodeSetStr( shape, "objectName", c_object.c_str() );

	static const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

    static std::string strDSO = std::string( getenv( "MTOA_PATH" ) ) + std::string("/procedurals/BifrostVolume.dll");
	AiNodeSetStr(shape, "dso", strDSO.c_str());
	AiNodeSetBool( shape, "load_at_init", true );

	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );

	// setup motion blur stuff
	AiNodeDeclare( shape, "motionBlur", "constant BOOL" );
	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
        CArnoldSession* session = CMayaScene::GetArnoldSession();
		session->GetMotionRange( shutterStart, shutterEnd );
	}

	AiNodeDeclare( shape, "shutterStart", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );

	AiNodeDeclare( shape, "shutterEnd", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

	ExportMatrix(shape);

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );

	AiNodeDeclare(shape, "fps", "constant FLOAT");
	AiNodeSetFlt(shape, "fps", fps);

	AiNodeDeclare( shape, "channelScale", "constant FLOAT" );
	float attrVal = bifrostDesc.findPlug( "aeroChannelScale" ).asFloat();
	AiNodeSetFlt( shape, "channelScale", attrVal );

	AiNodeDeclare( shape, "velocityScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal / fps );

	AiNodeDeclare( shape, "spaceScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroSpaceScale" ).asFloat();
	AiNodeSetFlt( shape, "spaceScale", attrVal );

	AiNodeDeclare( shape, "smoothOn", "constant BOOL" );
	bool boolAttrVal = bifrostDesc.findPlug( "aeroSmoothOn" ).asBool();
	AiNodeSetBool( shape, "smoothOn", boolAttrVal );

	AiNodeDeclare( shape, "smoothMode", "constant INT" );
	int intAttrVal = bifrostDesc.findPlug( "aeroSmoothMode" ).asInt();
	AiNodeSetInt( shape, "smoothMode", intAttrVal );

	AiNodeDeclare( shape, "smoothAmount", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "aeroSmoothAmount" ).asInt();
	AiNodeSetInt( shape, "smoothAmount", intAttrVal );

	AiNodeDeclare( shape, "smoothIterations", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "aeroSmoothIterations" ).asInt();
	AiNodeSetInt( shape, "smoothIterations", intAttrVal );

	AiNodeDeclare( shape, "smoothRemapMin", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroSmoothRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMin", attrVal );

	AiNodeDeclare( shape, "smoothRemapMax", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroSmoothRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMax", attrVal );

	AiNodeDeclare( shape, "smoothRemapInvert", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "aeroSmoothRemapInvert" ).asBool();
	AiNodeSetBool( shape, "smoothRemapInvert", boolAttrVal );

	AiNodeDeclare( shape, "clipOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "aeroClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );

	AiNodeDeclare( shape, "clipMinX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );

	AiNodeDeclare( shape, "clipMaxX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );

	AiNodeDeclare( shape, "clipMinY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );

	AiNodeDeclare( shape, "clipMaxY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );

	AiNodeDeclare( shape, "clipMinZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );

	AiNodeDeclare( shape, "clipMaxZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "aeroClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );

	AiNodeDeclare( shape, "splatResolutionFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatResolutionFactor" ).asFloat();
	AiNodeSetFlt( shape, "splatResolutionFactor", attrVal );

	AiNodeDeclare( shape, "skip", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "skip" ).asInt();
	AiNodeSetInt( shape, "skip", intAttrVal );

	AiNodeDeclare( shape, "densityMult", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "densityMult" ).asFloat();
	AiNodeSetFlt( shape, "densityMult", attrVal );

	AiNodeDeclare( shape, "splatSamples", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "splatSamples" ).asInt();
	AiNodeSetInt( shape, "splatSamples", intAttrVal );

	AiNodeDeclare( shape, "splatMinRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatMinRadius" ).asFloat();
	AiNodeSetFlt( shape, "splatMinRadius", attrVal );

	AiNodeDeclare( shape, "splatMaxRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatMaxRadius" ).asFloat();
	AiNodeSetFlt( shape, "splatMaxRadius", attrVal );

	AiNodeDeclare( shape, "splatSurfaceAttract", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatSurfaceAttract" ).asFloat();
	AiNodeSetFlt( shape, "splatSurfaceAttract", attrVal );

	AiNodeDeclare( shape, "splatFalloffType", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "splatFalloffType" ).asInt();
	AiNodeSetInt( shape, "splatFalloffType", intAttrVal );

	AiNodeDeclare( shape, "splatFalloffStart", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatFalloffStart" ).asFloat();
	AiNodeSetFlt( shape, "splatFalloffStart", attrVal );

	AiNodeDeclare( shape, "splatFalloffEnd", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatFalloffEnd" ).asFloat();
	AiNodeSetFlt( shape, "splatFalloffEnd", attrVal );

	AiNodeDeclare( shape, "splatDisplacement", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatDisplacement" ).asFloat();
	AiNodeSetFlt( shape, "splatDisplacement", attrVal );

	AiNodeDeclare( shape, "splatNoiseFreq", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "splatNoiseFreq" ).asFloat();
	AiNodeSetFlt( shape, "splatNoiseFreq", attrVal );

	AiNodeDeclare( shape, "debug", "constant INT" );
    intAttrVal = bifrostDesc.findPlug( "aiDebug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );

	AiNodeDeclare( shape, "silent", "constant INT" );
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

	AiNodeDeclare(shape, "bifFilename", "constant STRING");
	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );

	AiNodeDeclare(shape, "inputChannelName", "constant STRING");
	MString strAttrVal = bifrostDesc.findPlug( "aeroChannel" ).asString();
	AiNodeSetStr( shape, "inputChannelName", strAttrVal.asChar() );

	AiNodeDeclare(shape, "smoothChannelName", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "aeroSmoothChannel" ).asString();
	AiNodeSetStr( shape, "smoothChannelName", strAttrVal.asChar() );

	AiNodeDeclare(shape, "primVarNames", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "aeroPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );

	if (	( CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) ||
			CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines() ) {
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

void BifrostShapeTranslator::UpdateLiquidPolyMesh(AtNode *shape)
{
    AiMsgInfo("[BIFROST AI TRANSLATOR]: Translating %s as liquid", c_object.c_str());
	CoreObjectUserData objectRef(c_object, c_file);
	bool hotData = objectRef.objectExists();

	AiNodeDeclare( shape, "hotData", "constant BOOL" );
	AiNodeDeclare(shape, "objectName", "constant STRING");

	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist(frame);

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Liquid data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data
	AiNodeSetStr( shape, "objectName", c_object.c_str() );

	static const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

    static std::string strDSO = std::string( getenv( "MTOA_PATH" ) ) + std::string("/procedurals/BifrostPolyMesh.dll");
	AiNodeSetStr(shape, "dso", strDSO.c_str());
	AiNodeSetBool( shape, "load_at_init", true );

	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );

	// setup motion blur stuff
	AiNodeDeclare( shape, "motionBlur", "constant BOOL" );
	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
        CArnoldSession* session = CMayaScene::GetArnoldSession();
		session->GetMotionRange( shutterStart, shutterEnd );
	}

	AiNodeDeclare( shape, "shutterStart", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );

	AiNodeDeclare( shape, "shutterEnd", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

	ExportMatrix(shape);

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );

	AiNodeDeclare(shape, "fps", "constant FLOAT");
	AiNodeSetFlt(shape, "fps", fps);

	AiNodeDeclare( shape, "velocityScale", "constant FLOAT" );
	float attrVal = bifrostDesc.findPlug( "liquidVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal / fps );

	AiNodeDeclare( shape, "spaceScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidSpaceScale" ).asFloat();
	AiNodeSetFlt( shape, "spaceScale", attrVal );

	AiNodeDeclare( shape, "dropletRevealFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "meshingDropletRevealFactor" ).asFloat();
	AiNodeSetFlt( shape, "dropletRevealFactor", attrVal );

	AiNodeDeclare( shape, "surfaceRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "meshingSurfaceRadius" ).asFloat();
	AiNodeSetFlt( shape, "surfaceRadius", attrVal );

	AiNodeDeclare( shape, "dropletRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "meshingDropletRadius" ).asFloat();
	AiNodeSetFlt( shape, "dropletRadius", attrVal );

	AiNodeDeclare( shape, "kernelFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "meshingKernelFactor" ).asFloat();
	AiNodeSetFlt( shape, "kernelFactor", attrVal );

	AiNodeDeclare( shape, "resolutionFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "meshingResolutionFactor" ).asFloat();
	AiNodeSetFlt( shape, "resolutionFactor", attrVal );

	AiNodeDeclare( shape, "minimumFeatureAngle", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "minimumFeatureAngle" ).asFloat();
	AiNodeSetFlt( shape, "minimumFeatureAngle", attrVal );

	AiNodeDeclare( shape, "smoothing", "constant INT" );
	int intAttrVal = bifrostDesc.findPlug( "meshingSmoothing" ).asInt();
	AiNodeSetInt( shape, "smoothing", intAttrVal );

	AiNodeDeclare( shape, "debug", "constant INT" );
    intAttrVal = bifrostDesc.findPlug( "aiDebug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );

	AiNodeDeclare( shape, "silent", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "silent" ).asInt();
	AiNodeSetInt( shape, "silent", intAttrVal );

	// Determine the final BIF file name
	const float frame = (float)MAnimControl::currentTime().value();

	std::string particleFilename;
	std::string voxelFilename;
	{
		const int frameNumber  = (int)floorf(frame);

		std::string path = c_file + "_particle.#.bif";
		particleFilename = Bifrost::API::File::resolveFramePadding( path.c_str(), frameNumber ).c_str();

		path = c_file + "_volume.#.bif";
		voxelFilename = Bifrost::API::File::resolveFramePadding( path.c_str(), frameNumber ).c_str();
	}

	AiNodeDeclare(shape, "particleFilename", "constant STRING");
	AiNodeSetStr( shape, "particleFilename", particleFilename.c_str() );

	AiNodeDeclare(shape, "voxelFilename", "constant STRING");
	AiNodeSetStr( shape, "voxelFilename", voxelFilename.c_str() );

	AiNodeDeclare(shape, "primVarNames", "constant STRING");
	MString strAttrVal = bifrostDesc.findPlug( "liquidPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );

	// export shaders
	if (	(CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER ) ||
			CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines() ) {
		ExportBifrostShader();
	}

	// export lighting
	ExportLightLinking( shape );
}

void BifrostShapeTranslator::UpdateLiquidImplicit(AtNode *shape)
{
	CoreObjectUserData objectRef(c_object, c_file);
	bool hotData = objectRef.objectExists();

	AiNodeDeclare( shape, "hotData", "constant BOOL" );
	AiNodeDeclare( shape, "objectName", "constant STRING" );

	if ( hotData ) {
		AiNodeSetBool( shape, "hotData", 1 );
	} else {
		// The specified object doesn't exist in the current state server.
		// Try to load the object from the cache file.

		const float frame = (float)MAnimControl::currentTime().value();
		const bool cacheExist = objectRef.checkCacheFileExist( frame );

		if ( !cacheExist ) {
			AiMsgError("[BIFROST AI TRANSLATOR]: Aero data %s not found", c_object.c_str());
			return;
		}

		AiNodeSetBool( shape, "hotData", 0 );
	}

	// we have some data
	AiNodeSetStr( shape, "objectName", c_object.c_str() );

	static const MTime sec(1.0, MTime::kSeconds);
	float fps = (float) sec.as(MTime::uiUnit());

    static std::string strDSO = std::string( getenv( "MTOA_PATH" ) ) + std::string("/procedurals/BifrostImplicits.dll");
	AiNodeSetStr(shape, "dso", strDSO.c_str());
	AiNodeSetStr(shape, "solver", "levelset");
	AiNodeSetFlt(shape, "threshold", 0.0f);
	AiNodeSetBool( shape, "load_at_init", true );
	AiNodeSetBool( shape, "opaque", false );
	AiNodeSetBool( shape, "matte", false );
	AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

	ExportMatrix(shape);

	// setup motion blur stuff
	AiNodeDeclare( shape, "motionBlur", "constant BOOL" );
	AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );

	double shutterStart, shutterEnd;
	shutterStart = shutterEnd = 0.0;
	if (  IsMotionBlurEnabled() ) {
        CArnoldSession* session = CMayaScene::GetArnoldSession();
		session->GetMotionRange( shutterStart, shutterEnd );
	}

	AiNodeDeclare( shape, "shutterStart", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );

	AiNodeDeclare( shape, "shutterEnd", "constant FLOAT" );
	AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

	// get params from the node
	MFnDagNode  bifrostDesc;
	bifrostDesc.setObject( m_dagPath.node() );

	// set default paras
	MString strAttrVal = bifrostDesc.findPlug( "distanceChannel" ).asString();
	AiNodeSetStr(shape, "field_channel", strAttrVal.asChar() );

	int intAttrVal = bifrostDesc.findPlug( "liquidRaySamples" ).asInt();
	AiNodeSetUInt( shape, "samples", (unsigned int) intAttrVal );

	// set user params
	AiNodeDeclare(shape, "fps", "constant FLOAT");
	AiNodeSetFlt(shape, "fps", fps);

	AiNodeDeclare( shape, "velocityScale", "constant FLOAT" );
	float attrVal = bifrostDesc.findPlug( "liquidVelocityScale" ).asFloat();
	AiNodeSetFlt( shape, "velocityScale", attrVal / fps );

	AiNodeDeclare( shape, "spaceScale", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidSpaceScale" ).asFloat();
	AiNodeSetFlt( shape, "spaceScale", attrVal );

	AiNodeDeclare( shape, "narrowBandThicknessInVoxels", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "narrowBandThicknessInVoxels" ).asFloat();
	AiNodeSetFlt( shape, "narrowBandThicknessInVoxels", attrVal );

	AiNodeDeclare( shape, "liquidStepSize", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidStepSize" ).asFloat();
	AiNodeSetFlt( shape, "liquidStepSize", attrVal );

	AiNodeDeclare( shape, "cullSidesOn", "constant BOOL" );
	bool boolAttrVal = bifrostDesc.findPlug( "cullSidesOn" ).asBool();
	AiNodeSetBool( shape, "cullSidesOn", boolAttrVal );

	AiNodeDeclare( shape, "cullSidesStart", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "cullSidesStart" ).asFloat();
	AiNodeSetFlt( shape, "cullSidesStart", attrVal );

	AiNodeDeclare( shape, "cullSidesEnd", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "cullSidesEnd" ).asFloat();
	AiNodeSetFlt( shape, "cullSidesEnd", attrVal );

	AiNodeDeclare( shape, "cullDepthAtStartInVoxels", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "cullDepthAtStartInVoxels" ).asFloat();
	AiNodeSetFlt( shape, "cullDepthAtStartInVoxels", attrVal );


	AiNodeDeclare( shape, "dilateAmount", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "dilateAmount" ).asFloat();
	AiNodeSetFlt( shape, "dilateAmount", attrVal );

	AiNodeDeclare( shape, "erodeAmount", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "erodeAmount" ).asFloat();
	AiNodeSetFlt( shape, "erodeAmount", attrVal );

	AiNodeDeclare( shape, "smoothOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "smoothOn" ).asBool();
	AiNodeSetBool( shape, "smoothOn", boolAttrVal );

	AiNodeDeclare( shape, "smoothMode", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "smoothMode" ).asInt();
	AiNodeSetInt( shape, "smoothMode", intAttrVal );

	AiNodeDeclare( shape, "smoothAmount", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "smoothAmount" ).asInt();
	AiNodeSetInt( shape, "smoothAmount", intAttrVal );

	AiNodeDeclare( shape, "smoothIterations", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "smoothIterations" ).asInt();
	AiNodeSetInt( shape, "smoothIterations", intAttrVal );

	AiNodeDeclare( shape, "smoothRemapMin", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "smoothRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMin", attrVal );

	AiNodeDeclare( shape, "smoothRemapMax", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "smoothRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "smoothRemapMax", attrVal );

	AiNodeDeclare( shape, "smoothRemapInvert", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "smoothRemapInvert" ).asBool();
	AiNodeSetBool( shape, "smoothRemapInvert", boolAttrVal );

	AiNodeDeclare( shape, "clipOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "liquidClipOn" ).asBool();
	AiNodeSetBool( shape, "clipOn", boolAttrVal );

	AiNodeDeclare( shape, "clipMinX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMinX" ).asFloat();
	AiNodeSetFlt( shape, "clipMinX", attrVal );

	AiNodeDeclare( shape, "clipMaxX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMaxX" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxX", attrVal );

	AiNodeDeclare( shape, "clipMinY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMinY" ).asFloat();
	AiNodeSetFlt( shape, "clipMinY", attrVal );

	AiNodeDeclare( shape, "clipMaxY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMaxY" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxY", attrVal );

	AiNodeDeclare( shape, "clipMinZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMinZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMinZ", attrVal );

	AiNodeDeclare( shape, "clipMaxZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "liquidClipMaxZ" ).asFloat();
	AiNodeSetFlt( shape, "clipMaxZ", attrVal );


	AiNodeDeclare( shape, "infCubeBlendingOn", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "infCubeBlendingOn" ).asBool();
	AiNodeSetBool( shape, "infCubeBlendingOn", boolAttrVal );

	AiNodeDeclare( shape, "infCubeOutputType", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "infCubeOutputType" ).asInt();
	AiNodeSetInt( shape, "infCubeOutputType", intAttrVal );

	AiNodeDeclare( shape, "simWaterLevel", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "simWaterLevel" ).asFloat();
	AiNodeSetFlt( shape, "simWaterLevel", attrVal );

	AiNodeDeclare( shape, "infCubeTopCenterX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeTopCenterX" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterX", attrVal );

	AiNodeDeclare( shape, "infCubeTopCenterY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeTopCenterY" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterY", attrVal );

	AiNodeDeclare( shape, "infCubeTopCenterZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeTopCenterZ" ).asFloat();
	AiNodeSetFlt( shape, "infCubeTopCenterZ", attrVal );

	AiNodeDeclare( shape, "infCubeDimX", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeDimX" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimX", attrVal );

	AiNodeDeclare( shape, "infCubeDimY", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeDimY" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimY", attrVal );

	AiNodeDeclare( shape, "infCubeDimZ", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeDimZ" ).asFloat();
	AiNodeSetFlt( shape, "infCubeDimZ", attrVal );

	AiNodeDeclare( shape, "blendType", "constant INT" );
	intAttrVal = bifrostDesc.findPlug( "blendType" ).asInt();
	AiNodeSetInt( shape, "blendType", intAttrVal );

	AiNodeDeclare( shape, "infCubeBlendStart", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeBlendStart" ).asFloat();
	AiNodeSetFlt( shape, "infCubeBlendStart", attrVal );

	AiNodeDeclare( shape, "infCubeBlendEnd", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "infCubeBlendEnd" ).asFloat();
	AiNodeSetFlt( shape, "infCubeBlendEnd", attrVal );

	AiNodeDeclare( shape, "blendingChannelRemapMin", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "blendingChannelRemapMin" ).asFloat();
	AiNodeSetFlt( shape, "blendingChannelRemapMin", attrVal );

	AiNodeDeclare( shape, "blendingChannelRemapMax", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "blendingChannelRemapMax" ).asFloat();
	AiNodeSetFlt( shape, "blendingChannelRemapMax", attrVal );

	AiNodeDeclare( shape, "blendingChannelRemapInvert", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "blendingChannelRemapInvert" ).asBool();
	AiNodeSetBool( shape, "blendingChannelRemapInvert", boolAttrVal );


	AiNodeDeclare( shape, "implicitResolutionFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "implicitResolutionFactor" ).asFloat();
	AiNodeSetFlt( shape, "implicitResolutionFactor", attrVal );

	AiNodeDeclare( shape, "implicitDropletRevealFactor", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "implicitDropletRevealFactor" ).asFloat();
	AiNodeSetFlt( shape, "implicitDropletRevealFactor", attrVal );

	AiNodeDeclare( shape, "implicitSurfaceRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "implicitSurfaceRadius" ).asFloat();
	AiNodeSetFlt( shape, "implicitSurfaceRadius", attrVal );

	AiNodeDeclare( shape, "implicitDropletRadius", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "implicitDropletRadius" ).asFloat();
	AiNodeSetFlt( shape, "implicitDropletRadius", attrVal );

	AiNodeDeclare( shape, "implicitMaxVolumeOfHolesToClose", "constant FLOAT" );
	attrVal = bifrostDesc.findPlug( "implicitMaxVolumeOfHolesToClose" ).asFloat();
	AiNodeSetFlt( shape, "implicitMaxVolumeOfHolesToClose", attrVal );

	AiNodeDeclare( shape, "doMorphologicalDilation", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "doMorphologicalDilation" ).asBool();
	AiNodeSetBool( shape, "doMorphologicalDilation", boolAttrVal );

	AiNodeDeclare( shape, "doErodeSheetsAndDroplets", "constant BOOL" );
	boolAttrVal = bifrostDesc.findPlug( "doErodeSheetsAndDroplets" ).asBool();
	AiNodeSetBool( shape, "doErodeSheetsAndDroplets", boolAttrVal );


	AiNodeDeclare( shape, "debug", "constant INT" );
    intAttrVal = bifrostDesc.findPlug( "aiDebug" ).asInt();
	AiNodeSetInt( shape, "debug", intAttrVal );

	AiNodeDeclare( shape, "silent", "constant INT" );
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

	AiNodeDeclare(shape, "bifFilename", "constant STRING");
	AiNodeSetStr( shape, "bifFilename", particleFilename.c_str() );

	AiNodeDeclare(shape, "distanceChannel", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "distanceChannel" ).asString();
	AiNodeSetStr( shape, "distanceChannel", strAttrVal.asChar() );

	AiNodeDeclare(shape, "filterBlendingChannel", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "filterBlendingChannel" ).asString();
	AiNodeSetStr( shape, "filterBlendingChannel", strAttrVal.asChar() );

	AiNodeDeclare(shape, "infiniteSurfaceBlendingChannel", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "infiniteSurfaceBlendingChannel" ).asString();
	AiNodeSetStr( shape, "infiniteSurfaceBlendingChannel", strAttrVal.asChar() );

	AiNodeDeclare(shape, "primVarNames", "constant STRING");
	strAttrVal = bifrostDesc.findPlug( "liquidPrimVars" ).asString();
	AiNodeSetStr( shape, "primVarNames", strAttrVal.asChar() );

	if (	( CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) ||
			CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines() ) {
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

void BifrostShapeTranslator::ExportMotion(AtNode* shape)
{
	// Check if motionblur is enabled and early out if it's not.
	if (!IsMotionBlurEnabled()) return;

	// Set transform matrix
	ExportMatrix(shape);
}

void BifrostShapeTranslator::NodeInitializer( CAbTranslator context )
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;
    // debug
    ADD_INT_MIN("aiDebug", "aidbg", 0, 0, 5);
}

void BifrostShapeTranslator::ExportBifrostShader()
{
    AtNode *node = GetArnoldNode();

	MPlug shadingGroupPlug = GetNodeShadingGroup( m_dagPath.node(), 0 );
	if ( !shadingGroupPlug.isNull() ) {
        AtNode *rootShader = ExportConnectedNode( shadingGroupPlug );

		if ( rootShader != NULL ) {
			// Push the shader in the vector to be assigned later to mtoa_shading_groups
			AiNodeSetPtr(node, "shader", rootShader);
		}
	}
}
