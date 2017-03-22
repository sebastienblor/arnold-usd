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

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl
#define INIT_ASSERT(condition) if(!condition){ AiMsgWarning("%s:%d condition failed: %s", __FILENAME__, __LINE__, #condition); printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics ); return false; }
#define ERROR_ASSERT(condition) if(!condition){ AiMsgError("%s:%d condition failed: %s", __FILENAME__, __LINE__, #condition); }

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPolymeshMtd)

namespace {

AtNode* ProcSubdivide( ImplicitsInputData *inData, FrameData *frameData )
{
    printf("\nCreating the mesh...\n");
    Bifrost::API::StateServer SS = frameData->inSS;

    // declare arrays that hold poly data
    CvInt *vertices = NULL;
    CvFloat *pos = NULL;
    CvFloat *normal = NULL;
    CvFloat *motionPos = NULL;
    size_t polyCount;
    size_t vertexCount;
    size_t velocityCount;
    polyCount = vertexCount = velocityCount = 0;

    //define tokens and ptrs for export
    std::vector<CvToken> toks;
    std::vector<CvPointer> ptr;
    std::vector<int> varIndex;

    // apply meshing algo
    switch ( inData->mesherAlgo ) {
    case MESH_MARCHINGCUBES:
    {
        MarchingCubesVisitor visitor( frameData );
        Bifrost::API::Layout layout ( frameData->srcChannel.layout() );
        //layout.traverse( visitor, Bifrost::API::BreadthFirst, layout.maxDepth(), layout.maxDepth() );
        layout.traverse( visitor, Bifrost::API::ParallelReduceBreadthFirst, layout.maxDepth(), layout.maxDepth() );

        vertexCount = (CvInt) visitor.c_mesher->getExportVertexCount();
        polyCount = (CvInt) visitor.c_mesher->getTriangleCount();

        // check number of polys received, if none exit
        if ( checkPolyCount( vertexCount, polyCount ) ) {
            return NULL;
        }

        // we have some polys, allocate space for them
        allocatePolyData( vertexCount, polyCount, frameData, &vertices, &pos, &normal, &motionPos);
        // alloc space for primvars
        if ( frameData->primVars.size() > 0 ) {
            declarePrimVars( frameData->primVars, toks, ptr, varIndex, (int) vertexCount, frameData->mem, TOKCLASS_VARYING, getPrimVarTokenAI );
        }
        // prepare data for exporting
        preparePolyData( vertexCount, polyCount, visitor.c_mesher, frameData, inData, vertices, pos, normal, motionPos );

        break;
    }
    default:
        // the other types are not implemented
        break;
    }

    printf("\tDone\n");

    // export polygons
    printf("\tExporting Polygons...\n");

    AtNode *polymesh = AiNode( "polymesh" );

    std::string nodeName ( "BifrostPolyMeshExport1" );
    AiNodeSetStr( polymesh, "name", nodeName.c_str() );
    AiNodeSetBool( polymesh, "opaque", false );
    AiNodeSetBool( polymesh, "smoothing", true );

    // put spaceScale into action
    if ( inData->spaceScale != 1.0f ) {
        AtVector scaleVec = AtVector( inData->spaceScale, inData->spaceScale, inData->spaceScale );
        AtMatrix scaleM4 = AiM4Scaling( scaleVec );
        AiNodeSetMatrix ( polymesh, "matrix", scaleM4 );
    }

    // create array to hold poly data
    AtArray *posArray = AiArrayAllocate( (uint32_t) vertexCount, frameData->motionBlur ? 2 : 1, AI_TYPE_VECTOR );
    AtArray *nSidesArray = AiArrayAllocate( (uint32_t) polyCount, 1, AI_TYPE_UINT );
    AtArray *vertexArray = AiArrayAllocate( (uint32_t) polyCount * 3, 1, AI_TYPE_UINT );

    AtArray *normalArray = NULL;
    AtArray *vertexNormalArray = NULL;
    if ( frameData-> exportNormals ) {
        normalArray = AiArrayAllocate( (uint32_t) vertexCount, frameData->motionBlur ? 2 : 1, AI_TYPE_VECTOR );
        vertexNormalArray = AiArrayAllocate( (uint32_t) polyCount * 3, 1, AI_TYPE_UINT );
    }

    // get vertex data and convert to Arnold - BAD
    for ( unsigned int i = 0; i < vertexCount; i++ ) {
        // set position data
        int index = i * 3;
        AtVector tmpPoint = AtVector ( pos[index], pos[index + 1], pos[index + 2] );
        AiArraySetVec( posArray, i, tmpPoint );

        if ( frameData->exportNormals ) {
            AtVector tmpNormal = AtVector ( normal[index], normal[index + 1], normal[index + 2] );
            AiArraySetVec( normalArray, i, tmpNormal );
        }

        if ( frameData->motionBlur ) {
            tmpPoint = AtVector ( motionPos[index], motionPos[index + 1], motionPos[index + 2] );
            AiArraySetVec( posArray, (uint32_t) (i + vertexCount), tmpPoint );

            if ( frameData->exportNormals ) {
                AtVector tmpNormal = AtVector ( normal[index], normal[index + 1], normal[index + 2] );
                AiArraySetVec( normalArray, (uint32_t) (i + vertexCount), tmpNormal );
            }
        }
    }

    // get triangle data
    for ( unsigned int i = 0; i < polyCount; i++ ) {
        // set position data
        int index = i * 3;
        AiArraySetUInt( nSidesArray, i, 3 );
        AiArraySetUInt( vertexArray, index, vertices[index] );
        AiArraySetUInt( vertexArray, index + 1, vertices[index + 1] );
        AiArraySetUInt( vertexArray, index + 2, vertices[index + 2] );

        if ( frameData->exportNormals ) {
            AiArraySetUInt( vertexNormalArray, index, vertices[index] );
            AiArraySetUInt( vertexNormalArray, index + 1, vertices[index + 1] );
            AiArraySetUInt( vertexNormalArray, index + 2, vertices[index + 2] );
        }
    }

    AiNodeSetArray( polymesh, "vlist", posArray );
    if ( frameData->exportNormals ) {
        AiNodeSetArray( polymesh, "nlist", normalArray );
    }
    AiNodeSetArray( polymesh, "nsides", nSidesArray );
    AiNodeSetArray( polymesh, "vidxs", vertexArray );
    if ( frameData->exportNormals ) {
        AiNodeSetArray( polymesh, "nidxs", vertexNormalArray );
    }

    // export primvars too
    for ( unsigned int var = 0 ; var < frameData->primVars.size(); var++ ) {
        if ( frameData->primVars[ var ].exportToRIB ) {
            int currentArrayIndex = frameData->primVars[ var ].exportArraysIndex;

            // declare primvar
            Bifrost::API::String exportType = toks[ currentArrayIndex ];
            exportType.trimLeft ( frameData->primVars[ var ].name.c_str() );
            AiNodeDeclare( polymesh, frameData->primVars[ var ].name.c_str(), exportType.c_str() );

            // get the data and populate arnold array
            float *primVarData = (float *)ptr[ currentArrayIndex ];
            AtArray *primVarArray = AiArrayAllocate( (uint32_t) vertexCount, 1, AI_TYPE_FLOAT );

            for ( unsigned int i = 0; i < vertexCount; i++ ) {
                AiArraySetFlt(primVarArray, i, primVarData[ i ]);
            }
            AiNodeSetArray( polymesh, frameData->primVars[ var ].name.c_str(), primVarArray );
        }
    }

    printf("\t\tFinished exporting\n\n");

    // report min max for float types
    int nofFloatPrimVars = 0;
    for ( unsigned int i = 0; i < frameData->primVars.size(); i++ ) {
        if ( frameData->primVars[i].channel.dataType() == Bifrost::API::FloatType ||  frameData->primVars[i].name == "speed" ) {
            if (nofFloatPrimVars == 0 ) {
                printf("\nMin-Max ranges for float primvars after voxelization:\n");
            }
            printf( "\t%s:\n", frameData->primVars[i].name.c_str() );
            printf( "\t\tMin: %f\n", frameData->primVars[i].min );
            printf( "\t\tMax: %f\n", frameData->primVars[i].max );
            nofFloatPrimVars = 1;
        }
    }

    // free bif memory
    if ( SS.valid() ) {
        SS.clear();
    }

    // free the assembled data arrays
    for (unsigned i = 0; i < frameData->mem.size(); i++) {
        free ( frameData->mem[i] );
    }

    printf("\n---------------------------------------\n");
    printf("END OUTPUT FOR BIFROSTPOLYMESH\n");
    printf("---------------------------------------\n\n");
    return polymesh;
}

}

procedural_init
{
    ImplicitsInputData *inData = new ImplicitsInputData;
    DUMP(inData->mesherAlgo = (MesherAlgorithm) AiNodeGetInt(node, "mesherAlgo"));
    DUMP(inData->sampleRate = AiNodeGetInt(node, "sampleRate"));
    getNodeParameters(inData, node);

    FrameData* frameData = new FrameData;
    AtBBox bounds;
    InitializeImplicit(inData, frameData, &bounds);

    AtNode* polymesh = inData->error? NULL : ProcSubdivide(inData, frameData);
    *user_ptr = polymesh;

    printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics );

    if (frameData) {
        //if ( inData->hotData ) {
        //    Bifrost::API::File::deleteFolder( frameData->tmpFolder );
        //}
        delete frameData;
    }

    if(inData) { delete inData; }

    return polymesh != NULL;
}

procedural_num_nodes
{
	return 1;
}
procedural_get_node
{
    return (i==0)? (AtNode*)user_ptr : NULL;
}
procedural_cleanup
{
	return 1;
}
node_parameters
{
    AiParameterInt("mesherAlgo", 0);
    AiParameterInt("sampleRate", 1);
    ImplicitNodeDeclareParameters(params, nentry);
}
