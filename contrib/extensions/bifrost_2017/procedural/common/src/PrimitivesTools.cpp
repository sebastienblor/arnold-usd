#include "PrimitivesTools.h"

using namespace Bifrost::RenderCore;

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl
//
//
// EXPORT STUFF
//
//
void exportPrimitivesAI	(	PrimitivesInputData *inData,
							PrimitivesFrameData *frameData,
							int totalExported,
							std::vector<CvToken>& toks,
							std::vector<CvPointer>& ptr,
							std::vector<int>& varIndex,
							CvFloat *cPos,
							CvFloat *cWidth,
							CvFloat *cTra,
							CvFloat *cNormal,
							CvInt *cVerts,
							CvInt *cCode,
							void *usrData
						)
{
	AIProcNodeData *nodeData = (AIProcNodeData *) usrData;

	// export primitives according to type
	if ( inData->renderType == PRIM_POINT  || inData->renderType == PRIM_SPHERE ) 
	{
		nodeData->nofNodesCreated++;
		std::string nodeName ( "BifrostExport" );
		nodeName += toString( (unsigned long) nodeData->nofNodesCreated ); 
		
		AtNode *newNode = AiNode( "points", nodeName.c_str(), nodeData->proceduralNode );
		nodeData->createdNodes.push_back( newNode );
	
		if ( inData->renderType == PRIM_POINT ) {
			AiNodeSetStr( newNode, "mode", "disk");
		} else {
			AiNodeSetStr( newNode, "mode", "sphere");
		}
		AiNodeSetFlt( newNode, "min_pixel_width", 0.0f );
		AiNodeSetBool( newNode, "opaque", false );
		AiNodeSetFlt(newNode, "motion_start", inData->shutterStart);
		AiNodeSetFlt(newNode, "motion_end", inData->shutterEnd);

		// create a one element array which will be pointed to our data structure
		float *posData = (float *)cPos;
        AtArray *pointArray = AiArrayAllocate( totalExported, frameData->motionBlur ? 2 : 1, AI_TYPE_VECTOR );

		float *widthData = (float *)cWidth;
		AtArray *radiusArray = AiArrayAllocate( totalExported, frameData->motionBlur ? 2 : 1, AI_TYPE_FLOAT );

		AtArray *traArray = NULL;
		float *traData = NULL;
		if ( frameData->transparencyNeeded ) {
			traArray = AiArrayAllocate( totalExported, frameData->motionBlur ? 2 : 1, AI_TYPE_FLOAT );
			traData = (float *)cTra;
		}

		AtArray *normalArray = NULL;
		float *normalData = NULL;
		if ( inData->exportNormalAsPrimvar && inData->renderType == PRIM_POINT ) {
			normalArray = AiArrayAllocate( totalExported, frameData->motionBlur ? 2 : 1, AI_TYPE_VECTOR );
			normalData = (float *)cNormal;
		}

		// pull stuff from arrays
		for ( int i = 0; i < totalExported; i++ ) {
			// set position data
			int index = i * 3;
            AtVector tmpPoint = AtVector ( posData[ index ], posData[ index + 1 ], posData[ index + 2 ] );
            AiArraySetVec(pointArray, i, tmpPoint);

			// set radius
            AiArraySetFlt(radiusArray, i, inData->renderType == PRIM_POINT? widthData[ i ] : widthData[ i ] * 2.0f);
	
			// set transparency
			if ( frameData->transparencyNeeded ) {
				AiArraySetFlt(traArray, i, traData[ i ]);
			}
		
			if ( frameData->motionBlur ) {
				int index = 3 * i + 3 * frameData->finalChunkSize;
                tmpPoint = AtVector ( posData[ index ], posData[ index + 1 ], posData[ index + 2 ] );
                AiArraySetVec(pointArray, i + totalExported, tmpPoint);

				AiArraySetFlt(radiusArray, i + totalExported, widthData[ i + frameData->finalChunkSize ]);

				// assign second transparency if needed
				if ( frameData->transparencyNeeded ) {
					AiArraySetFlt(traArray, i + totalExported, traData[ i + frameData->finalChunkSize ]);
				}
			}
		}

		AiNodeSetArray( newNode, "points", pointArray );
		AiNodeSetArray( newNode, "radius", radiusArray );
        //AiNodeSetFlt( newNode, "motion_start", 0 );
        //AiNodeSetFlt( newNode, "motion_end", 0 );

        AiNodeSetFlt(newNode, "step_size", inData->stepSize);

		// export transparency if needed
		if ( frameData->transparencyNeeded ) {
			AiNodeDeclare( newNode, "transparency", "uniform FLOAT" );
			AiNodeSetArray( newNode, "transparency", traArray );
		}

		// export gradient if needed
		if ( inData->exportNormalAsPrimvar && inData->renderType == PRIM_POINT ) {
			AiNodeDeclare( newNode, "densityNormal", "uniform VECTOR" );

			for ( int i = 0; i < totalExported; i++ ) {
				int index = i * 3;
                AtVector tmpVector = AtVector ( normalData[ index ], normalData[ index + 1 ], normalData[ index + 2 ] );
				AiArraySetVec(normalArray, i, tmpVector);

				if ( frameData->motionBlur ) {
                    AtVector tmpVector = AtVector ( normalData[ index ], normalData[ index + 1 ], normalData[ index + 2 ] );
					AiArraySetVec(normalArray, i + totalExported, tmpVector);
				}
			}

			AiNodeSetArray( newNode, "densityNormal", normalArray );
		}

		// export primvars too
        for (unsigned int var = 0 ; var < frameData->primVars.size(); var++ ) {
			if ( frameData->primVars[ var ].exportToRIB ) {
				int currentArrayIndex = frameData->primVars[ var ].exportArraysIndex;

				// declare primvar
				Bifrost::API::String exportType = toks[ currentArrayIndex ];
				exportType.trimLeft ( frameData->primVars[ var ].name.c_str() );
				AiNodeDeclare( newNode, frameData->primVars[ var ].name.c_str(), exportType.c_str() );

				// get the data and populate arnold array
				float *primVarData = (float *)ptr[ currentArrayIndex ];
				AtArray *primVarArray = AiArrayAllocate( totalExported, 1, AI_TYPE_FLOAT );

				for ( int i = 0; i < totalExported; i++ ) {
					AiArraySetFlt(primVarArray, i, primVarData[ i ]);
				}

				AiNodeSetArray( newNode, frameData->primVars[ var ].name.c_str(), primVarArray );
			}
		}
	}
}
