#include <vector>
#include <stdint.h>

#include <ai.h>

#include <bifrostrendercore/bifrostrender_defs.h>
#include <bifrostrendercore/bifrostrender_types.h>
#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_primvars.h>
#include <bifrostrendercore/bifrostrender_visitors.h>
#include <bifrostrendercore/bifrostrender_meshing.h>
#include <bifrostrendercore/bifrostrender_meshingtools.h>
#include <bifrostrendercore/bifrostrender_filters.h>
#include <bifrostrendercore/bifrostrender_objectuserdata.h>
#include <bifrostrendercore/bifrostrender_types.h>

#include <Tools.h>
#include <Implicit.h>

template<class STR> inline char* StringToChar(const STR& str){
    char *dest = (char *) malloc((str.length()+1) * sizeof(char));
    strcpy(dest, str.c_str());
    return dest;
}

using namespace Bifrost::RenderCore;

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl
#define INIT_ASSERT(condition) if(!condition){ AiMsgWarning("%s:%d condition failed: %s", __FILENAME__, __LINE__, #condition); printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics ); return false; }
#define ERROR_ASSERT(condition) if(!condition){ AiMsgError("[BIFROST] Critical condition failed: (%s) (%s:%d)", #condition, __FILENAME__, __LINE__); return false; }

void ImplicitNodeDeclareParameters(AtList* params, AtNodeEntry* nentry){
    AiParameterBool("cullSidesOn", false);
    AiParameterVec2("cullSidesRange", 0, 1);
    AiParameterFlt("cullDepthAtStartInVoxels", 0);

    AiParameterFlt("velocityScale", 1);
    AiParameterFlt("spaceScale", 1); // Why?
    AiParameterFlt("fps", 24);

    // Filter attributes
    AiParameterFlt("dilateAmount", 0);
    AiParameterFlt("erodeAmount", 0);
    AiParameterInt("smoothMode" , 0);
    AiParameterInt("smoothKernelSize" , 0);
    AiParameterInt("smoothIterations" , 0);
    AiParameterFlt("smoothWeight" , 0); // ???
    AiParameterVec2("smoothRemapRange", 0, 1);
    AiParameterBool("smoothRemapInvert", false);

    AiParameterBool("clipOn", 0);
    AiParameterVec("clipMin", 0, 0, 0);
    AiParameterVec("clipMax", 1, 1, 1);

    AiParameterBool("infCubeBlendingOn", false);
    AiParameterInt("infCubeOutputType", OUTPUT_SIMONLY);
    AiParameterVec("infCubeTopCenter", 0, 0, 0);
    AiParameterVec("infCubeDim", 100, 100, 100);
    AiParameterInt("blendType", kLinear);
    AiParameterVec2("infCubeBlendRange", 0, 1);
    AiParameterVec2("blendingChannelRemapRange", 0, 1);
    AiParameterBool("blendingChannelRemapInvert", false);

    AiParameterFlt("implicitResolutionFactor", 0);
    AiParameterFlt("implicitDropletRevealFactor", 0);
    AiParameterFlt("implicitSurfaceRadius", 0);
    AiParameterFlt("implicitDropletRadius", 0);
    AiParameterFlt("implicitMaxVolumeOfHolesToClose", 0);
    AiParameterBool("doMorphologicalDilation", false);
    AiParameterBool("doErodeSheetsAndDroplets", false);

    AiParameterInt("debug" , 1);
    AiParameterInt("silent" , 0);

    AiParameterStr("bifFilename", "");
    AiParameterStr("distanceChannel", "");
    AiParameterStr("filterBlendingChannel", "");
    AiParameterStr("infiniteSurfaceBlendingChannel", "");
    AiParameterStr("primVarNames", "");
    AiParameterStr("bifrostObjectName", "");

    // arnold specific parameters // WTF
    AiParameterBool("motionBlur", true);
    AiParameterFlt("shutterStart" , 0);
    AiParameterFlt("shutterEnd", 1);

    AiParameterBool("exportUVs", 0);
    AiParameterArray("disp_map", AiArrayAllocate(0, 1, AI_TYPE_NODE));
    AiParameterFlt("disp_padding", 0);
    AiParameterFlt("disp_height", 1);
    AiParameterFlt("disp_zero_value", 0);
    AiParameterBool("disp_autobump", false);
}

bool getNodeParameters(ImplicitsInputData *inData, const AtNode *node)
{
    inData->cullSides.on = AiNodeGetBool( node, "cullSidesOn" );
    AtVector2 cullSidesRange = AiNodeGetVec2(node, "cullSidesRange");
    inData->cullSides.start = cullSidesRange.x;
    inData->cullSides.end = cullSidesRange.y;
    inData->cullSides.depthAtStartInVoxels = AiNodeGetFlt(node, "cullDepthAtStartInVoxels");

    inData->velocityScale = AiNodeGetFlt(node, "velocityScale");
    inData->fps = AiNodeGetFlt(node, "fps");
    inData->spaceScale = AiNodeGetFlt(node, "spaceScale");

    inData->dilateAmount = AiNodeGetFlt(node, "dilateAmount");
    inData->erodeAmount = AiNodeGetFlt(node, "erodeAmount");
    inData->smooth.mode = (SmoothFilterType) AiNodeGetInt( node, "smoothMode" );
    inData->smooth.kernelSize = AiNodeGetInt( node, "smoothKernelSize" );
    inData->smooth.iterations = AiNodeGetInt( node, "smoothIterations" );
    inData->smooth.weight = AiNodeGetFlt( node, "smoothWeight" );
    AtVector2 smoothRemapRange = AiNodeGetVec2(node, "smoothRemapRange");
    inData->smooth.remapMin = smoothRemapRange.x;
    inData->smooth.remapMax = smoothRemapRange.y;
    inData->smooth.remapInvert = AiNodeGetBool(node, "smoothRemapInvert");

    inData->clip.on = AiNodeGetBool( node, "clipOn" );
    AtVector clipMin = AiNodeGetVec(node, "clipMin"), clipMax = AiNodeGetVec(node, "clipMax");
    inData->clip.minX = clipMin.x;
    inData->clip.minY = clipMin.y;
    inData->clip.minZ = clipMin.z;
    inData->clip.maxX = clipMax.x;
    inData->clip.maxY = clipMax.y;
    inData->clip.maxZ = clipMax.z;

    inData->infCube.on = AiNodeGetBool( node, "infCubeBlendingOn" );
    inData->infCube.outputType = (InfCubeOutputType) AiNodeGetInt( node, "infCubeOutputType" );
    AtVector infCubeTopCenter = AiNodeGetVec(node, "infCubeTopCenter");
    inData->infCube.topCenterX = infCubeTopCenter.x;
    inData->infCube.topCenterY = infCubeTopCenter.y;
    inData->infCube.topCenterZ = infCubeTopCenter.z;
    AtVector infCubeDim = AiNodeGetVec(node, "infCubeDim");
    inData->infCube.dimX = infCubeDim.x;
    inData->infCube.dimY = infCubeDim.y;
    inData->infCube.dimZ = infCubeDim.z;
    inData->infCube.blendType = (FalloffType) AiNodeGetInt( node, "blendType" );
    AtVector2 infCubeBlendRange = AiNodeGetVec2(node, "infCubeBlendRange");
    inData->infCube.blendStart = infCubeBlendRange.x;
    inData->infCube.blendEnd = infCubeBlendRange.y;
    AtVector2 blendChannelRemapRange = AiNodeGetVec2(node, "blendingChannelRemapRange");
    inData->infCube.remapMin = blendChannelRemapRange.x;
    inData->infCube.remapMax = blendChannelRemapRange.y;
    inData->infCube.remapInvert = AiNodeGetBool( node, "blendingChannelRemapInvert" );

    inData->resolutionFactor = AiNodeGetFlt(node, "implicitResolutionFactor");
    inData->dropletRevealFactor = AiNodeGetFlt(node, "implicitDropletRevealFactor");
    inData->surfaceRadius = AiNodeGetFlt(node, "implicitSurfaceRadius");
    inData->dropletRadius = AiNodeGetFlt(node, "implicitDropletRadius");
    inData->maxVolumeOfHolesToClose = AiNodeGetFlt(node, "implicitMaxVolumeOfHolesToClose");
    inData->doMorphologicalDilation = AiNodeGetBool( node, "doMorphologicalDilation" );
    inData->doErodeSheetsAndDroplets = AiNodeGetBool( node, "doErodeSheetsAndDroplets" );

    inData->diagnostics.DEBUG = AiNodeGetInt(node, "debug");
    inData->diagnostics.silent = AiNodeGetInt(node, "silent");

    inData->bifFilename = StringToChar(AiNodeGetStr(node, "bifFilename"));
    inData->inputChannelName = StringToChar(AiNodeGetStr(node, "distanceChannel"));
    inData->smooth.channelName = StringToChar(AiNodeGetStr(node, "filterBlendingChannel"));
    inData->infCube.channelName = StringToChar(AiNodeGetStr(node, "infiniteSurfaceBlendingChannel"));
    inData->primVarNames = StringToChar(AiNodeGetStr(node, "primVarNames"));
    inData->bifrostObjectName = StringToChar(AiNodeGetStr(node, "bifrostObjectName"));
    inData->exportUVs = AiNodeGetBool( node, "exportUVs" );

    // arnold specific parameters
    inData->motionBlur = AiNodeGetBool( node, "motionBlur" );
    inData->shutterStart = AiNodeGetFlt( node, "shutterStart" );
    inData->shutterEnd = AiNodeGetFlt( node, "shutterEnd" );

    // check parameters
    inData->checkParameters();

    return inData->error;
}

void PostProcessVoxels(ImplicitsInputData *inData, FrameData *frameData) {
    const Bifrost::API::StateServer& inSS = frameData->inSS;
    //
    // POST PROCESSING CHANNEL DATA
    //
    if (inData->dilateAmount != 0.0f || inData->erodeAmount != 0.0f || ( inData->smooth.kernelSize > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 )) {
        IFNOTSILENT {
            printf("\nPost Processing %s channel...\n", inData->inputChannelName);
            printf("\tPost processing parameters:\n");
            if ( inData->dilateAmount != 0.0f ) {
                printf("\t\tDilate by: %.3f...\n", inData->dilateAmount);
            }
        }

        Bifrost::API::String filterType ("None");
        switch (inData->smooth.mode ) {
            case kMeanValue: filterType = "MeanValue"; break;
            case kGaussian: filterType = "Gaussian"; break;
            case kMedianValue: filterType = "Median"; break;
            case kCurvatureFlow: filterType = "CurvatureFlow"; break;
            case kLaplacianFlow: filterType = "LaplacianFlow"; break;
            default:break;
        }

        IFNOTSILENT {
            if ( inData->smooth.kernelSize > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) {
                printf("\t\tSmoothing FilterType: %s KernelSize: %d Iterations: %d Weight: %f FilterChannel: %s\n",
                       filterType.c_str(), inData->smooth.kernelSize, inData->smooth.iterations, inData->smooth.weight, inData->smooth.channelName );
            }
            if ( inData->erodeAmount != 0.0f ) {
                printf("\t\tErode by: %.3f...\n", inData->erodeAmount);
            }
        }

        // first dilate the channel
        if (inData->dilateAmount != 0.0 ) {
            IFNOTSILENT { printf("\tDilating...\n"); }
            dilateChannel( frameData, inData->dilateAmount, IMPLICITSURFACE );
        }

        // Run the smoothing filter
        if ( inData->smooth.kernelSize > 0 && inData->smooth.iterations > 0 && inData->smooth.weight > 0.0 ) {
            IFNOTSILENT { printf("\tSmoothing...\n"); }

            // Run the smoothing filter
            smoothChannel(inData->smooth, inSS, frameData->orgInputChannel, frameData, inData->diagnostics, IMPLICITSURFACE );

            IFNOTSILENT { printf("\n"); }
        } // end smooth check

        // finally erode the channel
        if ( inData->erodeAmount != 0.0f ) {
            IFNOTSILENT { printf("\tEroding...\n"); }
            erodeChannel( frameData, inData->erodeAmount, IMPLICITSURFACE );
        }

        IFNOTSILENT { printf("\n"); }
    }
}


CoreObjectUserData *createCoreObjectUserData(Bifrost::API::String& json, Bifrost::API::String& filename, const ClipParams& clip, Bifrost::API::String& tmpFolder){
    CoreObjectUserData* out = NULL;
    CoreObjectUserData tmpObj(json, filename);

    if(tmpObj.objectExists()){
        // write in memory data to a temp file
        // TODO: get component in a more robust way....
        Bifrost::API::String componentName = strstr( filename.c_str(), "volume" ) != NULL? "voxel_liquid-volume" : "voxel_liquid-particle";
        filename = writeHotDataToDisk(tmpObj, filename, componentName, tmpFolder);
    }

    // TODO: only import required channels (frameData->loadChannelNames)
    // Now we have the right file name
    Bifrost::API::Status loadState;
    Bifrost::API::ObjectModel om;
    Bifrost::API::FileIO fio = om.createFileIO(Bifrost::API::File::forwardSlashes(filename));
    Bifrost::API::StateServer ss = om.createStateServer();

    // timer for diagnostics
    if(clip.on){
        loadState = fio.load(ss, amino::Math::bboxf( amino::Math::vec3f(clip.minX, clip.minY, clip.minZ),
                                                     amino::Math::vec3f(clip.maxX, clip.maxY, clip.maxZ)));
    }else{
        loadState = fio.load(ss);
    }
    if(loadState == Bifrost::API::Status::Success){
        Bifrost::API::RefArray objects = ss.objects();
        if(objects.count() == 1){
            json = CoreObjectUserData::writeDictObject(ss.stateID(), Bifrost::API::Object(objects[0]).name()).saveJSON();
            out = new CoreObjectUserData(json, filename);
            if(!out->objectExists()){
                delete out;
                out = NULL;
            }
        }else{
            AiMsgWarning("[BIFROST] Can't find bif object in file '%s' (%d objects exist).", filename.c_str(), objects.count());
        }
    }else{
        AiMsgError("[BIFROST] Failed to load bif file '%s'.", filename.c_str());
    }

    return out;
}

bool InitializeImplicit(ImplicitsInputData* inData, FrameData* frameData, AtBBox* bounds)
{
    inData->error = false;

    //
    // INIT
    //
    // log start
    printEndOutput( "[BIFROST POLYMESH] START OUTPUT", inData->diagnostics );

    Bifrost::API::String tmpFolder;
    { // init in memory class
        Bifrost::API::String obj = inData->bifrostObjectName, file = inData->bifFilename;
        inData->inMemoryRef = createCoreObjectUserData(obj, file, inData->clip, tmpFolder);
        ERROR_ASSERT(inData->inMemoryRef != NULL);

        // realloc for the new name
        free(inData->bifFilename); inData->bifFilename = StringToChar(file);
        free(inData->bifrostObjectName); inData->bifrostObjectName = StringToChar(obj);
    }
    //
    // PRELOADING AND INFO GATHERING
    //

    // init FrameData struct that holds information specific to the frame we are rendering
    frameData->init();
    frameData->pluginType = PLUGIN_IMPLICITS;
    frameData->presenceNeeded = inData->cullSides.on;

    // process which channels to load
    initAndGetFrameData(	frameData,
                            ( void * ) inData,
                            inData->bifFilename,
                            inData->inputChannelName,
                            inData->primVarNames,
                            inData->smooth.channelName,
                            inData->infCube.channelName,
                            IMPLICITSURFACE,
                            inData->diagnostics,
                            getASSData );

    if(inData->exportUVs){
        frameData->uvNeeded = true;
        frameData->loadChannelNames.addUnique("uv");
        frameData->primVarNames.addUnique("uv");
    }

    frameData->tmpFolder = tmpFolder;

    if ( frameData->error ) {
        printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics );
        return false;
    }

    if ( frameData->empty ){
        printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics );
        AiMsgWarning("[bifrost liquid] Ignoring empty liquid data...");
        return false;
    }

    Bifrost::API::StateServer inSS = Bifrost::API::ObjectModel().stateServer(inData->inMemoryRef->bifrostObject().stateID());
    frameData->inSS = inSS;

    //
    // FILE CLASS GATHERING
    //
    // now that we have a bif file loaded
    // let's declare some variables for keeping track of what information we need during rendering
    Bifrost::API::RefArray objects = inSS.objects();
    Bifrost::API::Object inObj = objects[0];
    frameData->inObj = inObj;
    Bifrost::API::RefArray components = inSS.components();
    Bifrost::API::Component component = components[0];
    component = components[components.findFirstByName(strstr( inData->bifFilename, "volume" ) != NULL? "voxel_liquid-volume" : "voxel_liquid-particle")];

    //for(unsigned int i = 0; i < components.count(); ++i){
    //    DUMP(Bifrost::API::Component(components[i]).name());
    //}

    Bifrost::API::Layout layout = component.layout();
    Bifrost::API::RefArray channels = component.channels();

    // we may need to get the metadata once more because of older version files
    if ( frameData->metaDataLoaded == 0 ) {
        //getMetaDataFlags ( fio, frameData );
    }

    // print parameters
    IFNOTSILENT { inData->printParameters( frameData->isPointCache ); }

    // print load time
    IFNOTSILENT {
        if ( inData->diagnostics.DEBUG > 0 ) {
            //printf("\nLoad time:%f secs\n", duration);
        }
    }

    // print file info to the console
    IFNOTSILENT {
        std::cerr << "\nFile Info:" << std::endl;
        //std::cerr << "\tfilename: " << fio.filename() << std::endl;
        //std::cerr << "\tframe: " << fio.info().frame << " version: " << fio.info().version << " compression: " << fio.info().compression << std::endl << std::endl;
        std::cout << "\tComponent " << components.count() << ": " << component.name() << std::endl;
        std::cout << "\tLayout: " << layout.name() << std::endl;
        std::cout << "\t\tVoxelScale: " << layout.voxelScale() << std::endl;
        std::cout << "\t\tMaxDepth: " << layout.maxDepth() << std::endl;
        std::cout << "\t\tTotal TileCount: " << layout.tileCount() << std::endl;
        if ( frameData->isPointCache ) {
            for ( int i = 0; i <= layout.maxDepth(); i++ ) {
                std::cout << "\t\t\tTileCount " << i << ": " << layout.tileCount(i) << "->" << Bifrost::API::PointChannel(channels[0]).elementCount(i) << std::endl;
            }
        } else {
            for ( int i = 0; i <= layout.maxDepth(); i++ ) {
                std::cout << "\t\t\tTileCount " << i << ": " << layout.tileCount(i) << std::endl;
            }
        }
        if ( inData->diagnostics.DEBUG > 1 ) {
            std::cout << "\tChannel count: " << channels.count() << std::endl;
            for ( size_t i=0; i<channels.count(); i++ ) {
                std::cout << "\t\tChannel: " << Bifrost::API::Base(channels[i]).name() << std::endl;
            }
        }
        if ( frameData->dropletThreshold > 0 ) {
            printf("\nADAPTIVE SIM: ");
            frameData->isAdaptive ? printf("ON\n") : printf("OFF\n");
            printf("\tdropletThreshold: %f\n", frameData->dropletThreshold);
        }
        // dump data before channel processing
        if ( inData->diagnostics.DEBUG > 1 ) {
            dumpStateServer( inSS, "BEFORE PROCESSING" );
        }
    }

    // decide on what type of rendering we are doing
    CacheType cacheType = component.type()==Bifrost::API::PointComponentType? PARTICLE : VOXEL;
    IFNOTSILENT { printf("\n\nTHIS IS A %s CACHE!\n", cacheType==PARTICLE? "PARTICLE" : "VOXEL"); }

    //
    // OPTIMIZE CACHE FOR RENDERING
    //
    // TODO: do it? necessary? would need to compute frameData->hullCorners
    // Bifrost::API::Channel inChannel = component.findChannel( Bifrost::API::String( inData->inputChannelName ) );
    // optimizeInputForRendering( frameData, inChannel, inData->diagnostics );

    //
    // CONVERT PARTICLES TO VOXELS IF NEEDED
    //
    if ( cacheType == PARTICLE ) {
        IFNOTSILENT { printf("\nConverting particles to voxels...\n"); }

        component = convertParticlesToVoxels(component, inData, frameData, inSS, frameData->loadChannelNames );

        // update layout and channels as we now have another component
        layout = component.layout();
        channels = component.channels();

        IFNOTSILENT {
            if ( inData->diagnostics.DEBUG > 1 ) {
                dumpStateServer( inSS, "AFTER PARTICLE CONVERSION" );
            }
        }
    }

    // check whether we have a channel named as the input after conversion
    frameData->orgInputChannel = component.findChannel( Bifrost::API::String( inData->inputChannelName ) );
    if( !frameData->orgInputChannel.valid()){
        IFNOTSILENT ( printf( "\n\nNo valid channel found with the name: %s\nCan not continue!\n", inData->inputChannelName ) );
        return false;
    } else {
        IFNOTSILENT ( printf( "\nRendering Channel is: %s...\n", frameData->orgInputChannel.fullPathName().c_str() ) );
    }

    //
    // AT THIS POINT EVERYTHING IS VOXELIZED. PARTICLE CACHES ARE TRANSFORMED INTO A VOXEL REP BY CALCULATING A LEVEL SET OUT OF PARTICLES.
    // VOXEL CACHES ARE JUST PASSED THROUGH.
    // SO WE CAN ASSUME THAT THINGS ARE ALL IN VOXELS.
    //

    //
    // CREATE EXTRA RUNTIME CHANNELS
    //
    if ( frameData->speedNeeded ) {
        IFNOTSILENT { printf("\nCreating primvar: \n\tspeed\n"); }
        // create a speed channel out of velocity channels
        createSpeedChannel( inSS, frameData->orgInputChannel, component, inData->diagnostics );
    }
    if ( frameData->uvNeeded ) {
        IFNOTSILENT { printf("\nCreating primvar: \n\tu\n\tv\n"); }
        // separate uv channel to u1 and v1
        createUVChannels( inSS, frameData->orgInputChannel, component, inData->diagnostics );
    }

    //
    // CREATE PRIMVARINFO STRUCTURE
    //
    initPVarInfo( frameData );

    IFNOTSILENT { printf("\nProcessing PRIMVARS List...\n"); }

    checkPrimVars( frameData, inSS, component, inData->inputChannelName, inData->diagnostics );

    // print out primVars
    IFNOTSILENT {
        if (frameData->primVars.size() > 0 ) {
            printf("\tFinal list of Primvars to Export:\n");
            for (unsigned i = 0; i < frameData->primVars.size(); i++) {
                printf("\t\t%s\n", frameData->primVars[i].name.c_str() );
            }
        } else {
            printf ( "\tNo primvars found to export!\n" );
        }
    }

    //
    // NOW THAT WE HAVE EVERYTHING, INIT REST OF FRAMEDATA STRUCTURE
    //
    frameData->layout = layout;
    frameData->bifInfo.maxDepth = layout.maxDepth();
    frameData->voxelScale = layout.voxelScale();
    frameData->bboxSim.voxelScale = layout.voxelScale();
    frameData->bboxInf.voxelScale = layout.voxelScale();
    frameData->orgVoxelScale = layout.voxelScale();
    frameData->bifInfo.calcTileVolumes( layout );
    frameData->srcChannel = frameData->orgInputChannel;
    frameData->airDistanceChannel = component.findChannel( Bifrost::API::String( "airDistance" ) );
    frameData->inSS = inSS;
    frameData->exportNormals = false;
    frameData->sampleRate = inData->sampleRate;

    // init level lookup counts and depthWidths
    for (int i = 0; i <= layout.maxDepth(); i++) {
        frameData->bifInfo.levelDepthWidth[i] = layout.tileDimInfo(i).depthWidth;
        frameData->bifInfo.levelLookupCount[i] = 0;
        frameData->bifInfo.fastLookups[i] = 0;
        frameData->bifInfo.tilesWithDataCount[i] = 0;
        frameData->bifInfo.tilesWithChildTiles[i] = 0;
    }

    // check whether we need to export motion blur
    frameData->velocityScale = 0.0f;
    frameData->shutterSize = 0.0f;
    IFNOTSILENT { printf("\nMotionBlur: "); }

    if ( frameData->shutter[0] < frameData->shutter[1] ) {
        if ( frameData->velocityExists ) {
            frameData->motionBlur = true;
            frameData->velocityScale = inData->velocityScale;
            frameData->shutterSize = frameData->shutter[1] - frameData->shutter[0];

            IFNOTSILENT { printf("ON\n\tShutter: %.2f %.2f\n\n", (frameData->shutter)[0], (frameData->shutter)[1]); }

            frameData->velocityChannelU = component.findChannel( "velocity_u" );
            frameData->velocityChannelV = component.findChannel( "velocity_v" );
            frameData->velocityChannelW = component.findChannel( "velocity_w" );
            frameData->reportChannels.push_back( frameData->velocityChannelU );
            frameData->reportChannels.push_back( frameData->velocityChannelV );
            frameData->reportChannels.push_back( frameData->velocityChannelW );
        } else {
            IFNOTSILENT { printf("OFF because no valid velocity channel found!\n\n"); }
        }
    } else {
        IFNOTSILENT { printf("OFF\n\n"); }
    }

    // POST PROCESS : dilate, smooth, erode
    PostProcessVoxels(inData, frameData);

    //
    // INFCUBE BLENDING
    //
    if ( inData->infCube.on ) {
        IFNOTSILENT { printf("Applying inf cube blending...\n"); }

        // get channel
        frameData->infCubeBlendChannel = component.findChannel( Bifrost::API::String ( inData->infCube.channelName ) );

        displaceChannel( inData->infCube, inSS, frameData, inData->diagnostics );

        // if output type is ALL extend the sim
        if ( inData->infCube.outputType == OUTPUT_ALL ) {
            extendChannel( inData->infCube, inSS, frameData, inData->diagnostics );
        }

        IFNOTSILENT { printf("\tDone...\n\n"); }
    }

    // if we have an adaptive sim
    // make a copy of distance channel as we need the original values for presence
    // do it here since we need the data processed with the filtering operators
    if ( frameData->presenceNeeded ) {
        copyDistanceChannel( frameData, inSS, component );
    }

    // turn off infcubeblending as its effect is baked into the data
    inData->infCube.on = false;

    //
    // CALC BBOX
    //
    double bboxMin[3] = { 0.0, 0.0, 0.0 };
    double bboxMax[3] = { 0.0, 0.0, 0.0 };
    computeIsosurfaceBounds( frameData->srcChannel, bboxMin, bboxMax);

    // set bounds for the proc
    bounds->min[0] = frameData->bboxSim.world.min[0] = (float) bboxMin[0];
    bounds->min[1] = frameData->bboxSim.world.min[1] = (float) bboxMin[1];
    bounds->min[2] = frameData->bboxSim.world.min[2] = (float) bboxMin[2];
    bounds->max[0] = frameData->bboxSim.world.max[0] = (float) bboxMax[0];
    bounds->max[1] = frameData->bboxSim.world.max[1] = (float) bboxMax[1];
    bounds->max[2] = frameData->bboxSim.world.max[2] = (float) bboxMax[2];
    frameData->bboxSim.world.calcFromMinMax();

    //
    // REPORTING AND OPTIMIZATION
    //
    // initialise arrays for various optimization calcs
    initOptimizationArrays ( frameData, inData->diagnostics );

    // report channel ranges in the log
    // add existing input channels to the final list
    if ( frameData->smoothChannelExists ) {
        frameData->smoothChannel = component.findChannel( inData->smooth.channelName );
        frameData->reportChannels.push_back( frameData->smoothChannel );
    }

    if ( frameData->infCubeBlendChannelExists ) {
        frameData->infCubeBlendChannel = component.findChannel( inData->infCube.channelName );
        frameData->reportChannels.push_back( frameData->infCubeBlendChannel );
    }
    reportChannelRanges ( frameData, component, inData->diagnostics );

    IFNOTSILENT {
        if ( inData->diagnostics.DEBUG > 1 ) {
            dumpStateServer( inSS, "AFTER ALL OPS" );
        }
    }
    return true;
}
