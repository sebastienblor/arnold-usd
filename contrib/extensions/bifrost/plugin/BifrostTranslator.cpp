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

#define EXPORT_BOOL(name) AiNodeSetBool(shape, name, bifrostDesc.findPlug(name).asBool())
#define EXPORT_INT(name) AiNodeSetInt(shape, name, bifrostDesc.findPlug(name).asInt())
#define EXPORT_FLT(name) AiNodeSetFlt(shape, name, bifrostDesc.findPlug(name).asFloat())
#define EXPORT_FLT2(name) AiNodeSetVec2(shape, name, bifrostDesc.findPlug(name "X").asFloat(), bifrostDesc.findPlug(name "Y").asFloat())
#define EXPORT_FLT3(name) AiNodeSetVec(shape, name, bifrostDesc.findPlug(name "X").asFloat(), bifrostDesc.findPlug(name "Y").asFloat(), bifrostDesc.findPlug(name "Z").asFloat())
#define EXPORT_STR(name) AiNodeSetStr(shape, name, bifrostDesc.findPlug(name).asString().asChar())

#define EXPORT2_BOOL(aiName, name) AiNodeSetBool(shape, aiName, bifrostDesc.findPlug(name).asBool())
#define EXPORT2_INT(aiName, name) AiNodeSetInt(shape, aiName, bifrostDesc.findPlug(name).asInt())
#define EXPORT2_FLT(aiName, name) AiNodeSetFlt(shape, aiName, bifrostDesc.findPlug(name).asFloat())
#define EXPORT2_FLT3(aiName, name) AiNodeSetVec(shape, aiName, bifrostDesc.findPlug(name "X").asFloat(), bifrostDesc.findPlug(name "Y").asFloat(), bifrostDesc.findPlug(name "Z").asFloat())
#define EXPORT2_STR(aiName, name) AiNodeSetStr(shape, aiName, bifrostDesc.findPlug(name).asString().asChar())

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
    std::cerr << "OBJECT = " << c_object.c_str() << std::endl;

    MFnDependencyNode bfContainer(objectPlug.source().node());

    switch (c_renderType) {
    {
    case AERO:
            MPlug containerPlug = bfContainer.findPlug("aeroCacheProperties");
            MFnDependencyNode bfAeroProps(containerPlug.source().node());

            MString cacheDir = bfAeroProps.findPlug("aeroCachePath").asString();
            MString cacheName = bfAeroProps.findPlug("aeroCacheFileName").asString();

            MString fullBifPath = cacheDir + cacheName + "/AeroObject/AeroObject_" + (c_bifType==0? "volume":"particle");
            c_file = fullBifPath.asChar();
            break;
    }{
    case POINT:
            MPlug containerPlug = bfContainer.findPlug("foamCacheProperties");
            MFnDependencyNode bfPointProps(containerPlug.source().node());

            MString cacheDir = bfPointProps.findPlug("foamCachePath").asString();
            MString cacheName = bfPointProps.findPlug("foamCacheFileName").asString();

            MString fullBifPath = cacheDir + cacheName + "/Foam/Foam_particle";
            c_file = fullBifPath.asChar();
            break;
    }{
    case LIQUID_IMPLICIT:
    case LIQUID_POLYMESH:
            MPlug containerPlug = bfContainer.findPlug("liquidCacheProperties");
            MFnDependencyNode bfLiquidProps(containerPlug.source().node());

            MString cacheDir = bfLiquidProps.findPlug("liquidCachePath").asString();
            MString cacheName = bfLiquidProps.findPlug("liquidCacheFileName").asString();

            MString fullBifPath = cacheDir + cacheName + "/voxel_liquid/voxel_liquid_" + (c_bifType==0? "volume":"particle");
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
        AiMsgError( "[BIFROST TRANSLATOR]: Type not implemented yet : %s", c_object.c_str() );
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

    const float frame = (float)MAnimControl::currentTime().value();
    if (objectRef.objectExists() && objectRef.checkCacheFileExist(frame)) {
        AiMsgError("[BIFROST TRANSLATOR]: Point data %s not found", c_object.c_str());
        return;
    }
    // we have some data
    AiNodeSetStr(shape, "bifrostObjectName", c_object.c_str());
    AiNodeSetStr(shape, "bifFilename", Bifrost::API::File::resolveFramePadding((c_file+".#.bif").c_str(), (int)floorf(frame)).c_str());
    AiNodeSetBool( shape, "opaque", false );
    AiNodeSetBool( shape, "matte", false );

    // setup motion blur stuff
    AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled());
    double shutterStart, shutterEnd;
    shutterStart = shutterEnd = 0.0;
    if (  IsMotionBlurEnabled() ) {
        GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
    }
    AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );
    AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );

    const MTime sec(1.0, MTime::kSeconds);
    float fps = (float) sec.as(MTime::uiUnit());
    AiNodeSetFlt(shape, "fps", fps);

    // get params from the node
    MFnDagNode  bifrostDesc(m_dagPath.node());

    // global params
    EXPORT2_INT("renderType", "renderPrimitiveType");
    EXPORT2_FLT("channelScale", "pointChannelScale");
    EXPORT_BOOL("exportNormalAsPrimvar");
    EXPORT2_FLT("velocityScale", "pointVelocityScale");
    EXPORT2_FLT("spaceScale", "pointSpaceScale");
    EXPORT2_INT("skip", "pointSkip");
    EXPORT_INT("chunkSize");

    // clip params
    EXPORT2_BOOL("clipOn", "pointClipOn");
    // TODO: export vectors instead
    EXPORT2_FLT("clipMinX", "pointClipMinX");
    EXPORT2_FLT("clipMinY", "pointClipMinY");
    EXPORT2_FLT("clipMinZ", "pointClipMinZ");
    EXPORT2_FLT("clipMaxX", "pointClipMaxX");
    EXPORT2_FLT("clipMaxY", "pointClipMaxY");
    EXPORT2_FLT("clipMaxZ", "pointClipMaxZ");

    // radius params
    EXPORT_FLT("pointRadius");
    EXPORT_BOOL("useChannelToModulateRadius");
    EXPORT_BOOL("camRadiusOn");
    // TODO: Replace by ranges
    EXPORT_FLT("camRadiusStartDistance");
    EXPORT_FLT("camRadiusEndDistance");
    EXPORT_FLT("camRadiusStartFactor");
    EXPORT_FLT("camRadiusEndFactor");
    EXPORT_FLT("camRadiusFactorExponent");

    // multi sample params
    EXPORT_INT("mpSamples");
    EXPORT_FLT("mpMinRadius");
    EXPORT_FLT("mpMaxRadius");
    EXPORT_FLT("mpSurfaceAttract");
    EXPORT_INT("mpFalloffType");
    EXPORT_FLT("mpFalloffStart");
    EXPORT_FLT("mpFalloffEnd");
    EXPORT_FLT("mpDisplacementValue");
    EXPORT_FLT("mpDisplacementNoiseFrequency");
    EXPORT_INT("debug");
    EXPORT_INT("silent");
    EXPORT2_STR("primVarNames", "pointPrimVars");
    EXPORT2_STR("inputChannelName", "pointChannel");

    ExportMatrix( shape );
    if ( RequiresShaderExport() ) {
        ExportBifrostShader();
    }
    ExportLightLinking( shape );
}

void BifrostTranslator::ExportAero(AtNode *shape)
{
    CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );
    const float frame = (float)MAnimControl::currentTime().value();

    if(!objectRef.objectExists() && objectRef.checkCacheFileExist(frame)){
        AiMsgError("[BIFROST TRANSLATOR]: Aero data %s not found", c_object.c_str());
        return;
    }

    // we have some data
    AiNodeSetStr( shape, "bifrostObjectName", c_object.c_str() );
    AiNodeSetStr( shape, "bifFilename", Bifrost::API::File::resolveFramePadding((c_file+".#.bif").c_str(), (int)floorf(frame)).c_str());
    AiNodeSetByte( shape, "visibility", AI_RAY_ALL );
    AiNodeSetBool( shape, "opaque", false );
    AiNodeSetBool( shape, "matte", false );

    // setup motion blur stuff
    AiNodeSetBool( shape, "motionBlur", IsMotionBlurEnabled() );
    double shutterStart = 0, shutterEnd = 0;
    if ( IsMotionBlurEnabled()) {
        GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
    }
    AiNodeSetFlt( shape, "shutterStart", (float) shutterStart );
    AiNodeSetFlt( shape, "shutterEnd", (float) shutterEnd );
    const MTime sec(1.0, MTime::kSeconds);
    float fps = (float) sec.as(MTime::uiUnit());
    AiNodeSetFlt(shape, "fps", fps);


    // get params from the node
    MFnDagNode  bifrostDesc(m_dagPath.node());

    // first export matrix
    ExportMatrixWithSpaceScale(shape, bifrostDesc.findPlug( "aeroSpaceScale" ).asFloat());
    // set space scale to 1 as it is incorporated into matrix attribute
    AiNodeSetFlt(shape, "spaceScale", 1);

    // global params
    EXPORT2_FLT("channelScale", "aeroChannelScale");
    EXPORT2_FLT("velocityScale", "aeroVelocityScale");

    // smooth params
    EXPORT2_INT("smoothMode", "aeroSmoothMode");
    EXPORT2_INT("smoothAmount", "aeroSmoothAmount");
    EXPORT2_INT("smoothIterations", "aeroSmoothIterations");
    EXPORT2_FLT("smoothWeight", "aeroSmoothWeight");
    EXPORT2_FLT("smoothRemapMin", "aeroSmoothRemapMin");
    EXPORT2_FLT("smoothRemapMax", "aeroSmoothRemapMax");
    EXPORT2_BOOL("smoothRemapInvert", "aeroSmoothRemapInvert");

    // clip params
    EXPORT2_BOOL("clipOn", "aeroClipOn");
    EXPORT2_FLT("clipMinX","aeroClipMinX");
    EXPORT2_FLT("clipMinY","aeroClipMinY");
    EXPORT2_FLT("clipMinZ","aeroClipMinZ");
    EXPORT2_FLT("clipMaxX","aeroClipMaxX");
    EXPORT2_FLT("clipMaxY","aeroClipMaxY");
    EXPORT2_FLT("clipMaxZ","aeroClipMaxZ");

    // splat params
    EXPORT_FLT("splatResolutionFactor");
    EXPORT2_INT("skip", "aeroSkip");
    EXPORT_INT("splatSamples");
    EXPORT_FLT("splatMinRadius");
    EXPORT_FLT("splatMaxRadius");
    EXPORT_FLT("splatSurfaceAttract");
    EXPORT_INT("splatFalloffType");
    EXPORT_FLT("splatFalloffStart");
    EXPORT_FLT("splatFalloffEnd");
    EXPORT_FLT("splatDisplacement");
    EXPORT_FLT("splatNoiseFreq");

    // channels
    EXPORT2_STR("inputChannelName", "aeroChannel");
    EXPORT2_STR("smoothChannelName", "aeroSmoothChannel");
    EXPORT2_STR("primVarNames", "aeroPrimVars");

    // diagnotics params
    EXPORT_INT("debug");
    EXPORT_INT("silent");

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
    ExportLightLinking( shape );
}

void BifrostTranslator::ExportLiquidAttributes( MFnDagNode&  bifrostDesc, AtNode *shape )
{
    AiNodeSetBool( shape, "opaque", false );
    AiNodeSetBool( shape, "matte", false );
    AiNodeSetByte( shape, "visibility", AI_RAY_ALL );

    ExportMatrixWithSpaceScale(shape, bifrostDesc.findPlug( "liquidSpaceScale" ).asFloat());
    // setup motion blur stuff
    AiNodeSetBool(shape, "motionBlur", IsMotionBlurEnabled());
    double shutterStart, shutterEnd;
    shutterStart = shutterEnd = 0.0;
    if (  IsMotionBlurEnabled() ) {
        GetSessionOptions().GetMotionRange(shutterStart, shutterEnd);
    }
    AiNodeSetFlt(shape, "shutterStart", (float) shutterStart);
    AiNodeSetFlt(shape, "shutterEnd", (float) shutterEnd);

    // culling params
    EXPORT_BOOL("cullSidesOn");
    EXPORT_FLT2("cullSidesRange");
    EXPORT_INT("cullDepthAtStartInVoxels");
    EXPORT_FLT("cullDepthAtStartInVoxels");

    // common attributes
    EXPORT2_FLT("velocityScale", "liquidVelocityScale");
    const MTime sec(1.0, MTime::kSeconds);
    float fps = (float) sec.as(MTime::uiUnit());
    AiNodeSetFlt(shape, "fps", fps); // Why?

    // post process params
    EXPORT_FLT("dilateAmount");
    EXPORT_FLT("erodeAmount");
    EXPORT_INT("smoothMode");
    EXPORT_INT("smoothAmount");
    EXPORT_INT("smoothIterations");
    EXPORT_FLT("smoothWeight");
    EXPORT_FLT2("smoothRemapRange");
    EXPORT_BOOL("smoothRemapInvert");

    // clip params
    EXPORT_BOOL("clipOn");
    EXPORT2_FLT3("clipMin", "liquidClipMin");
    EXPORT2_FLT3("clipMin", "liquidClipMax");

    // infcube blending params
    EXPORT_BOOL("infCubeBlendingOn");
    EXPORT_INT("infCubeOutputType");
    EXPORT_FLT("simWaterLevel");
    EXPORT_FLT3("infCubeTopCenter");
    EXPORT_FLT3("infCubeDim");
    EXPORT_INT("blendType");
    EXPORT_FLT2("infCubeBlendRange");
    EXPORT_FLT2("blendingChannelRemapRange");
    EXPORT_BOOL("blendingChannelRemapInvert");

    // TODO: These param should be hidden in voxel mode
    // particle to voxel conversion params
    EXPORT_FLT("implicitResolutionFactor");
    EXPORT_FLT("implicitDropletRevealFactor");
    EXPORT_FLT("implicitSurfaceRadius");
    EXPORT_FLT("implicitDropletRadius");
    EXPORT_FLT("implicitMaxVolumeOfHolesToClose");
    EXPORT_BOOL("doMorphologicalDilation");
    EXPORT_BOOL("doErodeSheetsAndDroplets");

    // diagnostics params
    EXPORT_INT("debug");
    EXPORT_INT("silent");
    // TODO: remove
    AiNodeSetInt(shape, "debug", 1);
    AiNodeSetInt(shape, "silent", 0);

    // TODO: should either use file or in memory, do not export both
    // Determine the final BIF file name
    const int frame  = (int)floorf((float)MAnimControl::currentTime().value());
    std::string path = c_file+".#.bif";
    AiNodeSetStr(shape, "bifFilename", Bifrost::API::File::resolveFramePadding(path.c_str(), frame).c_str());

    CoreObjectUserData objectRef( c_object.c_str(), c_file.c_str() );
    if(objectRef.objectExists()){
        AiNodeSetStr(shape, "bifrostObjectName", c_object.c_str());
    }else{
        // The specified object doesn't exist in the current state server.
        // Try to load the object from the cache file.
        const float frame = (float)MAnimControl::currentTime().value();
        if(!objectRef.checkCacheFileExist(frame)){
            AiMsgError("[BIFROST AI TRANSLATOR]: Liquid data %s not found", c_object.c_str()); return;
        }
        AiNodeSetStr(shape, "bifrostObjectName", "");
    }

    EXPORT_STR("distanceChannel");
    EXPORT_STR("filterBlendingChannel");
    EXPORT_STR("infiniteSurfaceBlendingChannel");
    EXPORT2_STR("primVarNames", "liquidPrimVars");
}

void BifrostTranslator::ExportLiquidPolyMesh(AtNode *shape)
{
    MFnDagNode  bifrostDesc(m_dagPath.node());
    ExportLiquidAttributes(bifrostDesc, shape);

    // export mesh specific attributes
    EXPORT_INT("sampleRate");

    // export shaders
    if(RequiresShaderExport()){
        ExportBifrostShader();
    }
    ExportLightLinking( shape );
}

void BifrostTranslator::ExportLiquidImplicit(AtNode *shape)
{
    MFnDagNode  bifrostDesc(m_dagPath.node());
    ExportLiquidAttributes( bifrostDesc, shape );

    // we have some data
    AiNodeSetStr( shape, "solver", "levelset" );
    AiNodeSetFlt( shape, "threshold", 0.0f );

    // add implicit specific attributes
    EXPORT_FLT("narrowBandThicknessInVoxels");
    EXPORT_FLT("liquidStepSize");
    EXPORT2_INT("samples", "sampleRate");
    EXPORT2_STR("field_channel", "distanceChannel");

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
    if(IsMotionBlurEnabled()){
        ExportMatrix(shape);
    }
}

void BifrostTranslator::ExportBifrostShader()
{
    MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
    if ( !shadingGroupPlug.isNull() ) {
        AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
        if (rootShader != NULL) {
            // Push the shader in the vector to be assigned later to mtoa_shading_groups
            AiNodeSetPtr(GetArnoldNode(), "shader", rootShader);
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

#define ADD_DFLT2(longName, x, y) \
    data.name = data.shortName = longName;\
    data.defaultValue.VEC2() = AtVector2(x,y);\
    helper.MakeInputVector2(data);

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

namespace{
    void AddAeroAttributes(CExtensionAttrHelper& helper, CAttrData& data){
        ADD_DDATA_ENUM("aeroRenderData");
        ADD_DFLT("aeroChannelScale", 1.f);
        ADD_DFLT("aeroVelocityScale", 1.f);
        ADD_DFLT("aeroSpaceScale", 1.f);

        ADD_DSTR("aeroPrimVars", "temperature");
        ADD_DSTR("aeroChannel", "smoke");
        ADD_DSTR("aeroSmoothChannel", "");

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
    }

    void AddLiquidAttributes(CExtensionAttrHelper& helper, CAttrData& data){
        data.name = data.shortName = "renderMethod";
        {\
            MStringArray enums;\
            enums.append("Mesh");\
            enums.append("Implicit");\
            data.enums = enums;\
        }\
        helper.MakeInputEnum(data);
        ADD_DDATA_ENUM("renderData");
        ADD_DINT("sampleRate", 2);

        ADD_DFLT("liquidVelocityScale", 1.f);
        ADD_DFLT("liquidSpaceScale", 1.f);

        ADD_DSTR("liquidPrimVars", "vorticity");
        ADD_DSTR("distanceChannel", "distance");
        ADD_DSTR("filterBlendingChannel", "");
        ADD_DSTR("infiniteSurfaceBlendingChannel", "");

        ADD_DFLT("narrowBandThicknessInVoxels", 1.5f);
        ADD_DFLT("liquidStepSize", 0.05f);
        ADD_DBOOL("cullSidesOn", false);
        ADD_DFLT2("cullSidesRange", 0.75f, 0.95f);
        ADD_DFLT("cullDepthAtStartInVoxels", 10.0f);

        ADD_DFLT("dilateAmount", 0.f);
        ADD_DFLT("erodeAmount", 0.f);
        ADD_DSMOOTH_ENUM("smoothMode");
        ADD_DINT("smoothAmount", 1);
        ADD_DINT("smoothIterations", 1);
        ADD_DFLT("smoothWeight", 0.f);
        ADD_DFLT2("smoothRemapRange", 0.f, 1.f);
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
        ADD_DFLT2("infCubeBlendRange", 0.5f, .99f);
        ADD_DFLT2("blendingChannelRemapRange", 0.f, 1.f);
        ADD_DBOOL("blendingChannelRemapInvert", false);

        ADD_DFLT("implicitResolutionFactor", 1.f);
        ADD_DFLT("implicitDropletRevealFactor", 3.0f);
        ADD_DFLT("implicitSurfaceRadius", 1.2f);
        ADD_DFLT("implicitDropletRadius", 1.2f);
        ADD_DFLT("implicitMaxVolumeOfHolesToClose", 8.f);
        ADD_DBOOL("doMorphologicalDilation", true);
        ADD_DBOOL("doErodeSheetsAndDroplets", true);
    }

    void AddFoamAttributes(CExtensionAttrHelper& helper, CAttrData& data){
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
        ADD_DFLT("pointRadius", .05f);
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
    }
}

void BifrostTranslator::NodeInitializer( CAbTranslator context )
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    AddAeroAttributes(helper, data);
    AddLiquidAttributes(helper, data);
    AddFoamAttributes(helper, data);

    ADD_DINT("debug", 1);
    ADD_DINT("silent", 0);
}
