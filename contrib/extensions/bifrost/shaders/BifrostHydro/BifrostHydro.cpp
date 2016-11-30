#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <vector>

#include <ai.h>

#include "HydroTypes.h"
#include "Hydro3rdParty.h"
#include "HydroMIS.h"

AI_SHADER_NODE_EXPORT_METHODS(  BifrostHydroMethod  )

static const char* distributionNames[] = {"beckmann", "ggx", NULL};
static const char* interiorTypes[] = {"None", "Attenuation+ScatterWeight", "Scattering+Absorption", NULL};

enum BifrostHydroAIParams
{
	// diffuse
	pDiffuseWeight = 0,
	pDiffuseColor,
	pDiffuseRoughness,
	pDiffuseExtraSamples,
	pDiffuseIndirectScale,
	pDiffuseIndirectClamp,
	pDiffuseNormal,

	// emission
	pEmissionWeight,
	pEmissionColor,

	// specular
	pSpecularWeight,
	pSpecularColor,
	pSpecularRoughness,
	pSpecularAnisotropy,
	pSpecularRotation,
	pSpecularRoughnessDepthScale,
	pSpecularExtraSamples,
	pSpecularNormal,
	pSpecularIndirectScale,
	pSpecularIndirectClamp,
	pSpecularInternalDirect,
	pSpecularDistribution,

	// transmission
	pTransmissionWeight,
	pTransmissionColor,
	pTransmissionRoughness,
	pTransmissionIor,
	pTransmissionRoughnessDepthScale,
	pTransmissionEnableInternalReflections,
    pTransmissionExtraSamples,
	pTransmissionClamp,
	pTransmissionDoDirect,
	pTransmissionNormal,

	// scattering
	pSSInteriorType,
	pSSAttenuationColor,
	pSSScatteringWeight,
	pSSScattering,
	pSSAbsorption,
	pSSDensityScale,
	pSSDirection,

	pAiEnableMatte,
	pAiMatteColor,
	pAiMatteColorA,

	pOpacity,

	pRR,
	pRRTransmissionDepth,
	
	pAovDiffuseColor,
	pAovDirectDiffuse,
	pAovDirectDiffuseRaw,
	pAovIndirectDiffuse,
	pAovIndirectDiffuseRaw,
	pAovDirectSpecular,
	pAovIndirectSpecular,
	pAovSingleScatter,
	pAovRefraction,
	pAovEmission,
	pAovUv,
	pAovDepth,

	pStandardAovs,
	pTransmitAovs,

	pTraceSetAll,
	pTraceSetShadows,
	pTraceSetDiffuse,
	pTraceSetSpecular,
	pTraceSetTransmission,

	pDebug,

	pBump
};

node_parameters
{
	AiParameterFLT( "diffuseWeight", 0.0f );
	AiParameterRGB( "diffuseColor", 0.0f, 0.05f, 0.144f );
	AiParameterFLT( "diffuseRoughness", 0.0f );
    AiParameterINT( "aiDiffuseExtraSamples", 0 );
	AiParameterFLT( "diffuseIndirectScale", 1.0f );
	AiParameterFLT( "diffuseIndirectClamp", 0.0f );
	AiParameterVec( "diffuseNormal", 0, 0, 0 );

	AiParameterFLT( "emissionWeight", 0.0f );
	AiParameterRGB( "emissionColor", 1.0f, 1.0f, 1.0f );

	AiParameterFLT( "specularWeight", 1.0f );
	AiParameterRGB( "specularColor", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "specularRoughness", 0.0f );
	AiParameterFLT( "specularAnisotropy", 0.5f );
	AiParameterFLT( "specularRotation", 0.0f );
	AiParameterFLT( "specularRoughnessDepthScale", 1.0f );
    AiParameterINT( "aiSpecularExtraSamples", 0 );
	AiParameterVec( "specularNormal", 0, 0, 0 );
	AiParameterFLT( "specularIndirectScale", 1.0f );
	AiParameterFLT( "specularIndirectClamp", 0.0f );
	AiParameterBOOL( "specularInternalDirect", true );
    AiParameterENUM( "aiSpecularDistribution", AI_MICROFACET_GGX, distributionNames );

	AiParameterFLT( "transmissionWeight", 0.7f );
	AiParameterRGB( "transmissionColor", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "transmissionRoughness", 0.f );
	AiParameterFLT( "transmissionIor", 1.35f );
	AiParameterFLT( "transmissionRoughnessDepthScale", 1.0f );
	AiParameterBOOL( "transmissionEnableInternalReflections", true );
    AiParameterINT( "aiTransmissionExtraSamples", 0 );
	AiParameterFLT( "transmissionClamp", 0.0f );
	AiParameterBOOL( "transmissionDoDirect", false );
	AiParameterVec( "transmissionNormal", 0, 0, 0 );

	AiParameterENUM( "ssInteriorType", 0, interiorTypes );
	AiParameterRGB( "ssAttenuationColor", 0.85f, 1.0f, 1.0f );
	AiParameterFLT( "ssScatteringWeight", 1.0f );
	AiParameterRGB( "ssScattering", 1.0f, 1.0f, 1.0f );
	AiParameterRGB( "ssAbsorption", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "ssDensityScale", 1.0f );
	AiParameterFLT( "ssDirection", 0.75f );

	AiParameterBOOL( "aiEnableMatte", false );
	AiParameterRGB( "aiMatteColor", 0.0f, 0.0f, 0.0f );
	AiParameterFlt( "aiMatteColorA", 0.0f );

	AiParameterRGB( "opacity", 1.0f, 1.0f, 1.0f );

    AiParameterBool( "aiRussianRoulette", true );
    AiParameterInt( "aiRrTransmissionDepth", 1 );

	// AOVs
    AiParameterStr( "aiAovDiffuseColor", "diffuseColor" );
    AiParameterStr( "aiAovDirectDiffuse", "directDiffuse" );
    AiParameterStr( "aiAovDirectDiffuseRaw", "directDiffuseRaw" );
    AiParameterStr( "aiAovIndirectDiffuse", "indirectDiffuse" );
    AiParameterStr( "aiAovIndirectDiffuseRaw", "indirectDiffuseRaw" );
    AiParameterStr( "aiAovDirectSpecular", "directSpecular" );
    AiParameterStr( "aiAovIndirectSpecular", "indirectSpecular" );
    AiParameterStr( "aiAovSingleScatter", "singleScatter" );

    AiParameterStr( "aiAovRefraction", "refraction" );
    AiParameterStr( "aiAovEmission", "emission" );
    AiParameterStr( "aiAovUv", "uv" );
    AiParameterStr( "aiAovDepth", "depth" );

    AiParameterBool( "aiStandardCompatibleAOVs", false );
    AiParameterBool( "aiTransmitAovs", false );

	// Sets
    AiParameterSTR( "aiTraceSetAll", "" );
    AiParameterSTR( "aiTraceSetShadows", "" );
    AiParameterSTR( "aiTraceSetDiffuse", "" );
    AiParameterSTR( "aiTraceSetSpecular", "" );
    AiParameterSTR( "aiTraceSetTransmission", "" );

    AiParameterInt( "aiDebug", 0 );

    AiMetaDataSetStr(mds, NULL, "maya.name", "bifrostHydro");
}

#ifdef MSVC
#define CRTSECURENOWARNINGS 1
#endif

node_initialize
{
	ShaderData* data = new ShaderData(  );
	AiNodeSetLocalData( node, data );
	data->diffuseSampler = NULL;
	data->specularSampler = NULL;
	data->refractionSampler = NULL;
	data->permTable = NULL;
	data->permTableDiffuse = NULL;
	data->permTableSpecular = NULL;
};

node_finish
{
	if ( AiNodeGetLocalData( node ) ) {
		ShaderData* data = ( ShaderData* )AiNodeGetLocalData( node );
		AiSamplerDestroy( data->diffuseSampler );
		AiSamplerDestroy( data->specularSampler );
		AiSamplerDestroy( data->refractionSampler );
		delete[] data -> permTable;
		delete[] data -> permTableDiffuse;
		delete[] data -> permTableSpecular;

		AiNodeSetLocalData( node, NULL );
		delete data;
	}
}

node_update
{
	ShaderData* data = (  ShaderData* )AiNodeGetLocalData(  node );

	// set up AOVs
	REGISTER_AOVS

	data->standardAovs = params[  pStandardAovs  ].BOOL;
	data->transmitAovs = params[  pTransmitAovs  ].BOOL;

	// store some options we'll reuse later
	AtNode* options = AiUniverseGetOptions(  );
	data->giDiffuseDepth = AiNodeGetInt(  options, "GI_diffuse_depth"  );
	data->giReflectionDepth = AiNodeGetInt(  options, "GI_reflection_depth"  );
	data->giRefractionDepth = AiNodeGetInt(  options, "GI_refraction_depth"  );
	data->giSpecularDepth = AiNodeGetInt(  options, "GI_glossy_depth"  );

	data->giSpecularSamples = AiNodeGetInt(  options, "GI_glossy_samples" ) + params[ pSpecularExtraSamples ].INT;
	data->giSpecularSamplesSQR = SQR(  data->giSpecularSamples );

	data->giDiffuseSamples = AiNodeGetInt( options, "GI_diffuse_samples" ) + params[ pDiffuseExtraSamples ].INT;
	data->giDiffuseSamplesSQR = SQR( data->giDiffuseSamples );

	data->giRefractionSamples = AiNodeGetInt( options, "GI_refraction_samples" ) + params[ pTransmissionExtraSamples ].INT;
	data->giRefractionSamplesSQR = SQR( data->giRefractionSamples );

	// setup samplers
	if ( data->diffuseSampler ) {
		AiSamplerDestroy( data->diffuseSampler );
	}
	if ( data->specularSampler ) {
		AiSamplerDestroy( data->specularSampler );
	}
	if ( data->refractionSampler ) {
		AiSamplerDestroy( data->refractionSampler );
	}

	data->diffuseSampler = AiSampler( data->giDiffuseSamples, 2 );
	data->specularSampler = AiSampler( data->giSpecularSamples, 2 );
	data->refractionSampler = AiSampler( data->giRefractionSamples, 2 );

	// Get all the light nodes in the scene and try and find their light group parameter
	// we'll store this based on the light pointer for fast access during rendering
	AtNodeIterator* it = AiUniverseGetNodeIterator( AI_NODE_LIGHT );
	while ( !AiNodeIteratorFinished( it ) ) {
		AtNode* light = AiNodeIteratorGetNext( it );
		if ( AiNodeLookUpUserParameter( light, "lightGroup" ) )
			data->lightGroups[ light ] = AiNodeGetInt( light, "lightGroup" ) - 1;
		else
			data->lightGroups[ light ] = -1;

		data->shadowDensities[ light ] = AiNodeGetFlt( light, "shadowDensity" );
	}
	AiNodeIteratorDestroy( it );

	// check whether the normal parameters are connected or not
	data->specularNormalConnected = AiNodeIsLinked( node, "specularNormal" );
	data->diffuseNormalConnected = AiNodeIsLinked( node, "diffuseNormal" );
	data->transmissionNormalConnected = AiNodeIsLinked( node, "transmissionNormal" );

	// check clamp stuff
	data->specularIndirectClamp = params[ pSpecularIndirectClamp ].FLT;
	if ( data->specularIndirectClamp == 0.0f ) {
		data->specularIndirectClamp = AI_INFINITE;
	}

	data->transmissionClamp = params[ pTransmissionClamp ].FLT;
	if ( data->transmissionClamp == 0.0f ) {
		data->transmissionClamp = AI_INFINITE;
	}

	data->diffuseIndirectClamp = params[ pDiffuseIndirectClamp ].FLT;
	if ( data->diffuseIndirectClamp == 0.0f ) {
		data->diffuseIndirectClamp = AI_INFINITE;
	}

	data->transmissionDoDirect = params[ pTransmissionDoDirect ].BOOL;

	// Set up info for RR
	data->doRR = params[ pRR ].BOOL;
	data->rrTransmissionDepth = params[ pRRTransmissionDepth ].INT;
	data->AASamples = SQR(  std::max(  1, AiNodeGetInt(  options, "AA_samples"  )  )  );
	data->AASamplesInv = 1.0f / float(  data->AASamples  );

	data->totalDepth = AiNodeGetInt( options, "GI_total_depth" ) + 1;
	delete[] data -> permTable;
	data->permTable = new int[ data->AASamples * data->totalDepth ];
	delete[] data -> permTableDiffuse;
	data->permTableDiffuse = new int[ data->AASamples * data->giDiffuseSamplesSQR * data->totalDepth ];
	delete[] data -> permTableSpecular;
	data->permTableSpecular = new int[ data->AASamples * data->giSpecularSamplesSQR * data->totalDepth ];

	// seed the random number generator
	srand( 12419 );

	// generate the permutation table for RR
	for ( int d = 0; d < data->totalDepth; ++d ) {
		permute( &( data->permTable[ d * data->AASamples ] ), data->AASamples );
	}

	srand( 4567 );
	// generate the permutation table for diffuse RR
	if ( data->giDiffuseSamplesSQR > 0 ) {
		for ( int d = 0; d < data->totalDepth; ++d ) {
			permute( &( data->permTableDiffuse[ d * data->AASamples * data->giDiffuseSamplesSQR ] ), data->AASamples * data->giDiffuseSamplesSQR );
		}
	}

	srand( 453 );
	// generate the permutation table for specular RR
	if ( data->giSpecularSamplesSQR > 0 ) {
		for ( int d = 0; d < data->totalDepth; ++d ) {
			permute( &( data->permTableSpecular[ d * data->AASamples * data->giSpecularSamplesSQR ] ), data->AASamples * data->giSpecularSamplesSQR );
		}
	}

	data->xres = AiNodeGetInt( options, "xres" );

	// Trace sets setup
	data->traceSetAllEnabled = false;
	data->traceSetShadowsEnabled = false;
	data->traceSetDiffuseEnabled = false;
	data->traceSetSpecularEnabled = false;
	data->traceSetTransmissionEnabled = false;

	if ( strlen( params[ pTraceSetAll ].STR ) ) {
		std::string tmp( params[ pTraceSetAll ].STR );
		data->traceSetAllEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetAllInclusive = false;
			data->traceSetAll = tmp.substr( 1 );
		} else {
			data->traceSetAllInclusive = true;
			data->traceSetAll = tmp;
		}
	}

	if ( strlen( params[ pTraceSetShadows ].STR ) ) {
		std::string tmp( params[ pTraceSetShadows ].STR );
		data->traceSetShadowsEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetShadowsInclusive = false;
			data->traceSetShadows = tmp.substr( 1 );
		} else {
			data->traceSetShadowsInclusive = true;
			data->traceSetShadows = tmp;
		}
	}

	if ( strlen( params[ pTraceSetDiffuse ].STR ) ) {
		std::string tmp( params[ pTraceSetDiffuse ].STR );
		data->traceSetDiffuseEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetDiffuseInclusive = false;
			data->traceSetDiffuse = tmp.substr( 1 );
		} else {
			data->traceSetDiffuseInclusive = true;
			data->traceSetDiffuse = tmp;
		}
	}

	if ( strlen( params[ pTraceSetSpecular ].STR ) ) {
		std::string tmp( params[ pTraceSetSpecular ].STR );
		data->traceSetSpecularEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetSpecularInclusive = false;
			data->traceSetSpecular = tmp.substr( 1 );
		} else {
			data->traceSetSpecularInclusive = true;
			data->traceSetSpecular = tmp;
		}
	}

	if ( strlen( params[ pTraceSetTransmission ].STR ) ) {
		std::string tmp( params[ pTraceSetTransmission ].STR );
		data->traceSetTransmissionEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetTransmissionInclusive = false;
			data->traceSetTransmission = tmp.substr( 1 );
		} else {
			data->traceSetTransmissionInclusive = true;
			data->traceSetTransmission = tmp;
		}
	}

	data->debug = params[ pDebug ].INT;

}

shader_evaluate
{
	ShaderData* data = ( ShaderData* )AiNodeGetLocalData( node );

	// get params
	float ior = AiShaderEvalParamFlt( pTransmissionIor );
	float roughness = AiShaderEvalParamFlt( pSpecularRoughness );

	AtRGB diffuseColor = AiShaderEvalParamRGB( pDiffuseColor ) * AiShaderEvalParamFlt( pDiffuseWeight );
	float diffuseRoughness = AiShaderEvalParamFlt( pDiffuseRoughness );
	float diffuseIndirectScale = AiShaderEvalParamFlt( pDiffuseIndirectScale );

	AtRGB specularColor = AiShaderEvalParamRGB( pSpecularColor ) * AiShaderEvalParamFlt( pSpecularWeight );
	float specularRoughnessDepthScale =	AiShaderEvalParamFlt( pSpecularRoughnessDepthScale );
	float specularIndirectScale = AiShaderEvalParamFlt( pSpecularIndirectScale );
	bool specularInternalDirect = AiShaderEvalParamBool( pSpecularInternalDirect );
	int specularDistribution = AiShaderEvalParamInt( pSpecularDistribution );

	float transmissionRoughness = AiShaderEvalParamFlt( pTransmissionRoughness );
	AtRGB transmissionColor = AiShaderEvalParamRGB( pTransmissionColor ) * AiShaderEvalParamFlt( pTransmissionWeight );
	float transmissionRoughnessDepthScale = AiShaderEvalParamFlt( pTransmissionRoughnessDepthScale );
	bool transmissionEnableInternalReflections = AiShaderEvalParamBool( pTransmissionEnableInternalReflections );


	AtRGB ssAttenuationColor = AiShaderEvalParamRGB( pSSAttenuationColor );
	float ssScatteringWeight = AiShaderEvalParamFlt( pSSScatteringWeight );
	float ssDensityScale = AiShaderEvalParamFlt( pSSDensityScale );
	float ssDirection = AiShaderEvalParamFlt( pSSDirection );
	AtRGB ssScattering = AiShaderEvalParamRGB( pSSScattering );
	AtRGB ssAbsorption = AiShaderEvalParamRGB( pSSAbsorption );
	InteriorType ssInteriorType = (InteriorType) AiShaderEvalParamInt( pSSInteriorType );

	AtRGB emissionColor = AiShaderEvalParamRGB( pEmissionColor ) * AiShaderEvalParamFlt( pEmissionWeight );
	AtRGB opacity = AiShaderEvalParamRGB( pOpacity );

	// precalc values
	float eta = 1.0f / ior;
	roughness *= roughness;
	transmissionRoughness *= transmissionRoughness;

	// balance diffuse, transmission
	// shouldn't we do this after energy conservation?
	float tSum = maxh( transmissionColor ) + maxh( diffuseColor );
	if ( tSum > 1.0f ) {
		float tSumInv = 1.0f / tSum;
		transmissionColor *= tSumInv;
		diffuseColor *= tSumInv;
	}

	// if the attenuation color is anything but white, make sure we don't have some channels left at pure white
	if ( minh( ssAttenuationColor ) < 1.0f ) {
		ssAttenuationColor = clamp( ssAttenuationColor, AI_RGB_BLACK, rgb( 0.999f ) );
	}

	// precalculate scattering coefficients as we'll need them for shadows etc.
	AtRGB sigmaT = AI_RGB_BLACK;
	AtRGB sigmaS = AI_RGB_BLACK;
	AtRGB sigmaA = AI_RGB_BLACK;
	AtRGB sigmaSPrime = AI_RGB_BLACK;
	AtRGB sigmaTPrime = AI_RGB_BLACK;
	bool doAttenuation = false;
	bool doScattering = false;
	if ( (minh( ssAttenuationColor ) < 1 || ssScatteringWeight > 0 || ssInteriorType == COEFFICIENTS) && ssInteriorType != NONE ) {
		if (  ssInteriorType == COEFFICIENTS ) {
			sigmaS = ssScattering * ssDensityScale;
			sigmaA = ssAbsorption * ssDensityScale;
			sigmaT = sigmaS + sigmaA;

			if ( maxh( sigmaT ) > 0 ) {
				doAttenuation = true;
			}
			
			if ( maxh( sigmaS ) > 0 ) {
				doScattering = true;
			}
		} else {
			sigmaA = ( AI_RGB_WHITE - ssAttenuationColor ) * ssDensityScale;
			// if we're doing a scattering medium, then make sure absorption is not zero
			if ( ssScatteringWeight > 0 ) {
				sigmaA = max( sigmaA, rgb( 0.0001f ) );
			}

			sigmaS = ssAttenuationColor * ssScatteringWeight * ssDensityScale;
			sigmaT = sigmaS + sigmaA;

			if ( maxh( sigmaA ) > 0 ) {
				doAttenuation = true;
			}

			if ( maxh( sigmaS ) > 0 ) {
				doScattering = true;
			}
		}

		sigmaSPrime = sigmaS * ( 1.0f - ssDirection );
		sigmaTPrime = ( sigmaSPrime + sigmaA );
	}

	// if this is a camera ray, prepare the temporary storage for deep groups
	AtRGB* deepGroupPtr = NULL;
	AtRGB resultDirectGroup[ NUMLIGHTGROUPS ];
	bool doDeepGroups = !data->standardAovs;
	bool transmitAovs =	data->transmitAovs && ( !data->standardAovs ) && ( !doDeepGroups );

	for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
		resultDirectGroup[ i ] = AI_RGB_BLACK;
	}

	if ( doDeepGroups && ( sg->Rt & AI_RAY_CAMERA ) ) {
		// if this is a camera ray allocate the group storage
		deepGroupPtr = (AtRGB *) AiShaderGlobalsQuickAlloc( sg, sizeof( AtRGB ) * NUMLIGHTGROUPS );
		memset( deepGroupPtr, 0, sizeof( AtRGB ) * NUMLIGHTGROUPS );
		AiStateSetMsgPtr( "HydroDeepGroupPtr", deepGroupPtr );
	} else if ( doDeepGroups ) {
		// secondary ray hit - get the pointer from the state
		// if the pointer hasn't been set we're being called from a BSDF that doesn't have deep group support
		// so don't try and do it or we'll be in ( crashy ) trouble!
		if ( !AiStateGetMsgPtr( "HydroDeepGroupPtr", (void **) &deepGroupPtr ) ) {
			doDeepGroups = false;
		}
	}

	AtRGB* transmittedAovPtr = NULL;
	if ( transmitAovs && ( sg->Rt & AI_RAY_CAMERA ) ) {
		transmittedAovPtr =	(AtRGB *) AiShaderGlobalsQuickAlloc( sg, sizeof( AtRGB ) * NUMAOVs );
		memset( transmittedAovPtr, 0, sizeof( AtRGB ) * NUMAOVs );
		AiStateSetMsgPtr( "HydroTransmittedAovPtr", transmittedAovPtr );
	} else if ( transmitAovs ) {
		if ( !AiStateGetMsgPtr( "HydroTransmittedAovPtr", (void **) &transmittedAovPtr ) ) {
			transmitAovs = false;
		}
	}

	// get path throughput so far
	AtRGB pathThroughput = AI_RGB_WHITE;
	if ( data->doRR && sg->Rr > 0 ) {
		AiStateGetMsgRGB( "HydroThroughput", &pathThroughput );
	}

	// if it's a shadow ray, handle shadow colouring through absorption
	if ( sg->Rt & AI_RAY_SHADOW ) {
		// if the object is transmissive and
		AtRGB out_opacity = AI_RGB_WHITE;
		if ( maxh( transmissionColor ) ) {
			// check transmission through the surface
			float costheta = AiV3Dot( sg->Nf, -sg->Rd );
			float kt = 1.0f - fresnel( costheta, eta ); // for dielectric

			// if surface is fully reflective
			if ( kt >= IMPORTANCE_EPS )  {
				if ( maxh( sigmaT ) > 0.0f ) {
					AtPoint HydroPreviousIntersection;
					AtRGB HydroSigmaT = sigmaTPrime;
					if ( AiStateGetMsgPnt( "HydroPreviousIntersection", &HydroPreviousIntersection )  ) {
						AiStateGetMsgRGB( "HydroPreviousSigmaT", &HydroSigmaT );
						bool doExtinction = false;
						if ( AiV3Dot( sg->N, sg->Rd ) < 0.0f ) {
							// ray is entering a closed volume
							bool HydroInside;
							AiStateGetMsgBool( "HydroInside", &HydroInside );

							if ( HydroInside ) {
								// ray is entering an embedded volume
								doExtinction = true;
							} else {
								// shouldn't get here
							}
						} else {
							// ray is exiting a closed volume
							doExtinction = true;
						}

						if ( doExtinction ) {
							float z = AiV3Dist( sg->P, HydroPreviousIntersection );
							out_opacity.r = fast_exp( -z * HydroSigmaT.r );
							out_opacity.g = fast_exp( -z * HydroSigmaT.g );
							out_opacity.b = fast_exp( -z * HydroSigmaT.b );
							out_opacity = -log( out_opacity );
						}

						// 'add' the extra opacity due to transmission colour and fresnel
						out_opacity = 1.0f - ( ( 1.0f - out_opacity ) * kt * transmissionColor );
					} else {
						// first intersection
						// tell the next shader invocation that we're now inside the surface and what our extinction coefficient is
						AiStateSetMsgRGB( "HydroPreviousSigmaT", sigmaTPrime );
						AiStateSetMsgBool( "HydroInside", true );
						out_opacity = 1.0f - kt * transmissionColor;
					}
				} else { // no extinction, shadows are fresnel only.
					AiStateSetMsgRGB( "HydroPreviousSigmaT", AI_RGB_BLACK );
					out_opacity = 1.0f - kt * transmissionColor;
				}
			}
		}

		// store intersection position
		AiStateSetMsgPnt( "HydroPreviousIntersection", sg->P );
		sg->out_opacity = out_opacity * opacity;
		return;
	}

	if ( maxh( opacity ) < IMPORTANCE_EPS ) {
		sg->out.RGB = AI_RGB_BLACK;
		sg->out_opacity = AI_RGB_BLACK;
	}
	sg->out_opacity = opacity;

	// early out if we're fully transparent or the object is matte
	if ( AiColorIsZero( opacity ) || ( AiShaderGlobalsIsObjectMatte( sg ) && ( sg->Rt & AI_RAY_CAMERA ) ) ) {
		return;
	}

	// Evaluate bump;
	// AtRGB bump = AiShaderEvalParamRGB( pBump );

	// set ray type to undefined
	int HydroRayType = HYDRO_RAY_UNDEFINED;
	AiStateSetMsgInt( "HydroRayType", HYDRO_RAY_UNDEFINED );

	// Initialize parameter temporaries
	// TODO: reorganize this so we're not evaluating upstream when we don't need
	// the parameters, e.g. in shadow rays
	AtVector specularNormal = sg->Nf;
	if ( data->specularNormalConnected ) {
		specularNormal = AiV3Normalize( AiShaderEvalParamVec( pSpecularNormal ) );
	}

	AtVector diffuseNormal = sg->Nf;
	if ( data->diffuseNormalConnected ) {
		diffuseNormal = AiV3Normalize( AiShaderEvalParamVec( pDiffuseNormal ) );
	}

	AtVector transmissionNormal = sg->Nf;
	if ( data->transmissionNormalConnected ) {
		transmissionNormal = AiV3Normalize( AiShaderEvalParamVec( pTransmissionNormal ) );
	}

	// adapt the roughness values for anisotropy
	float specularAnisotropy = CLAMP( AiShaderEvalParamFlt( pSpecularAnisotropy ), 0.0f, 1.0f );
	float roughnessX = roughness;
	float roughnessY = roughness;
	if ( specularAnisotropy != 0.5f ) {
		float anisoT = SQR( fabsf( 2.0f * specularAnisotropy - 1.0f ) );
		roughnessX = ( specularAnisotropy >= 0.5f ) ? roughness : LERP( anisoT, MAX( roughness, 0.0025f ), 1.0f );
		roughnessY = ( specularAnisotropy <= 0.5f )	? roughness : LERP( anisoT, MAX( roughness, 0.0025f ), 1.0f );
	}

	// Grab the roughness from the previous surface and make sure we're slightly
	// rougher than it to avoid specular-specular fireflies
	float HydroPreviousRoughness = 0.0f;
	if ( sg->Rr > 0 ) {
		AiStateGetMsgFlt( "HydroPreviousRoughness", &HydroPreviousRoughness );
		if ( data->doRR ) {
			roughnessX = std::max( roughnessX, HydroPreviousRoughness * specularRoughnessDepthScale );
			roughnessY = std::max( roughnessY, HydroPreviousRoughness * specularRoughnessDepthScale );
			transmissionRoughness =	std::max( transmissionRoughness, HydroPreviousRoughness * transmissionRoughnessDepthScale );
		}
	}

	// clamp roughnesses
	// roughness = std::max( 0.000001f, roughness );

	// View direction
	AtVector wo = -sg->Rd;

	// Initialize result temporaries
	AtRGB resultDiffuseDirect = AI_RGB_BLACK;
	AtRGB resultDiffuseDirectRaw = AI_RGB_BLACK;
	AtRGB resultSpecularDirect = AI_RGB_BLACK;
	AtRGB resultDiffuseIndirect = AI_RGB_BLACK;
	AtRGB resultDiffuseIndirectRaw = AI_RGB_BLACK;
	AtRGB resultSpecularIndirect = AI_RGB_BLACK;
	AtRGB resultSs = AI_RGB_BLACK;
	AtColor resultTransmission = AI_RGB_BLACK;
	AtColor resultTransmissionDirect = AI_RGB_BLACK;
	AtColor resultEmission = AI_RGB_BLACK;

	// Set up flags to early out of calculations based on where we are in the ray tree
	bool doDiffuse = true;
	bool doSpecular = true;
	bool doSS = true;
	bool doTransmission = true;

	// disable diffuse if contribution is small
	if ( maxh( diffuseColor ) < IMPORTANCE_EPS  ) {
		doDiffuse = false;
	}

	if (	( sg->Rr_diff > 0 ) || // disable specular->diffuse caustics
			maxh( specularColor ) < IMPORTANCE_EPS || // disable specular if contribution is small
			( sg->Rr_refr > 0 && !transmissionEnableInternalReflections ) || // disable specular->transmitted caustics
			roughness > 1.0f ||
			HydroRayType == HYDRO_RAY_HAIR ||
			( HydroPreviousRoughness > 0.0f && roughness ==0.0f ) ) // kill specular if roughness has been scaled up too far
	{
		doSpecular = false;
	}

	// if the user has chosen not to do internal direct reflections, turn off the
	// direct specular calculation if we're backfacing
	bool doSpecularDirect = doSpecular && ( ( AiV3Dot( sg->Nf, sg->Ng ) > 0 ) || specularInternalDirect );

	if ( ( sg->Rr_diff > 0 ) || // disable transmitted caustics
		maxh( transmissionColor ) < IMPORTANCE_EPS ||
		HydroRayType == HYDRO_RAY_HAIR )  // disable transmission if contribution is small
	{
		doTransmission = false;
	}

	// prepare temporaries for light group calculation
	AtRGB lightGroupsDirect[ NUMLIGHTGROUPS ];
	memset( lightGroupsDirect, 0, sizeof( AtRGB ) * NUMLIGHTGROUPS );

	// Decide whether to calculate shadow groups or not.
	bool doShadowGroups = ( sg->Rt & AI_RAY_CAMERA );

	// Accumulator for transmission integrated according to the specular brdf.
	// Will be used to attenuate diffuse and transmission
	float kti = 1.0f;
	float kti2 = 1.0f;

	// storage for all deepgroup contributions
	AtRGB deepGroupsSpecular[ NUMLIGHTGROUPS ];
	AtRGB deepGroupsDiffuse[ NUMLIGHTGROUPS ];
	AtRGB deepGroupsTransmission[ NUMLIGHTGROUPS ];
	memset( deepGroupsSpecular, 0, sizeof( AtRGB ) * NUMLIGHTGROUPS );
	memset( deepGroupsDiffuse, 0, sizeof( AtRGB ) * NUMLIGHTGROUPS );
	memset( deepGroupsTransmission, 0, sizeof( AtRGB ) * NUMLIGHTGROUPS );
	int count = 0;

	//
	//
	// ILLUMINATION CALCULATION
	//
	//

	// Create the BRDF data structures for MIS
	//
	AtVector Nold = sg->N;
	AtVector Nfold = sg->Nf;
	sg->N = sg->Nf = specularNormal;

	// build local coordinate space for sampling
	AtVector U, V;
	if ( !AiV3isZero( sg->dPdu ) && AiV3Exists( sg->dPdu ) ) {
		// we have valid a valid dPdu derivative, construct V
		AtVector Utmp = AiV3Normalize( sg->dPdu );
		V = AiV3Normalize( AiV3Cross( sg->Nf, Utmp ) );
		U = AiV3Cross( V, sg->Nf );
	} else {
		AiBuildLocalFramePolar( &U, &V, &sg->Nf );
	}

	// rotated frames for anisotropy
	AtVector U1 = U, U2 = U;
	AtVector V1 = V, V2 = V;

	if ( specularNormal != sg->Nf ) {
		if ( !AiV3isZero( sg->dPdu ) && AiV3Exists( sg->dPdu ) ) {
			// we have valid a valid dPdu derivative, construct V
			AtVector Utmp = AiV3Normalize( sg->dPdu );
			V1 = AiV3Normalize( AiV3Cross( specularNormal, Utmp ) );
			U1 = AiV3Cross( V1, specularNormal );
		} else {
			AiBuildLocalFramePolar( &U1, &V1, &specularNormal );
		}
	}

	if ( specularAnisotropy != 0.5f ) {
		float specularRotation = AiShaderEvalParamFlt( pSpecularRotation );
		const float cosPhi = cosf( specularRotation * AI_PI );
		const float sinPhi = sinf( specularRotation * AI_PI );
		U1 = cosPhi * U1 - sinPhi * V1;
		V1 = cosPhi * V1 + sinPhi * U1;
	}

	void* mis;
	mis = AiMicrofacetMISCreateData( sg, specularDistribution, &U1, 0, roughnessX, roughnessY );

	// init dilelectric
	FresnelDielectric* fr = ( FresnelDielectric* ) AiShaderGlobalsQuickAlloc( sg, sizeof( FresnelDielectric ) );
	new ( fr ) FresnelDielectric( eta );

	// init brdf data
	BrdfDataWrap brdfw;
	brdfw.brdfData = mis;
	brdfw.sg = sg;
	brdfw.fr = fr;
	brdfw.eta = eta;
	brdfw.V = wo;
	brdfw.N = specularNormal;
	brdfw.kr = 0.0f;

	sg->N = Nold;
	sg->Nf = Nfold;

	sg->N = sg->Nf = diffuseNormal;
	void* dmis = AiOrenNayarMISCreateData( sg, diffuseRoughness );

	sg->fhemi = true;
	sg->Nf = Nfold;
	sg->N = Nold;

	AtRGBA shadowGroups[ NUMLIGHTGROUPS ];
	memset( shadowGroups, 0, sizeof( AtRGBA ) * NUMLIGHTGROUPS );

	// set the global trace set if it's defined
	// this will potentially be overriden by each component as we go through the shader
	if ( data->traceSetAllEnabled ) {
		AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
	}

	// set the shadows trace set if it's defined
	if ( data->traceSetShadowsEnabled ) {
		AiShaderGlobalsSetTraceSet( sg, data->traceSetShadows.c_str(), data->traceSetShadowsInclusive );
	}

	AtVector Nforig = sg->Nf;
	AtVector Norig = sg->N;
	if ( doSpecularDirect ) {
		if ( data->specularNormalConnected ) {
			sg->N = sg->Nf = specularNormal;
		}

		AiLightsPrepare( sg );

		while ( AiLightsGetSample( sg ) ) {
			if ( AiLightGetAffectSpecular( sg->Lp ) ){
				// get the group assigned to this light from the hash table using
				// the light's pointer
				int lightGroup = data->lightGroups[ sg->Lp ];

				// per-light specular and diffuse Weight multipliers
				float specularWeight = AiLightGetSpecular( sg->Lp );

				brdfw.ibs = false;
				AtRGB LspecularDirect = AiEvaluateLightSample( sg, &brdfw, AiMicrofacetMISSampleWrap, AiMicrofacetMISBRDFWrap, AiMicrofacetMISPDFWrap ) * specularWeight * specularColor;
				// if the light is assigned a valid group number, add this sample's contribution to that light group
				if ( lightGroup >= 0 && lightGroup < NUMLIGHTGROUPS ) {
					lightGroupsDirect[ lightGroup ] += LspecularDirect;
				}
				// accumulate the result
				resultSpecularDirect += LspecularDirect;
			}
		}

		sg->Nf = Nforig;
		sg->N = Norig;
		if ( data->specularNormalConnected ) {
			AiLightsResetCache( sg );
		}

		if ( brdfw.ns > 0.0f ) {
			kti = 1.0f - ( maxh( brdfw.krInt ) / brdfw.ns * maxh( specularColor ) );
			assert( AiIsFinite( kti ) );
		}
	}

	if ( doDiffuse ) {
		if ( data->diffuseNormalConnected ) {
			sg->N = sg->Nf = diffuseNormal;
		}

		AiLightsPrepare( sg );
		AtRGB LdiffuseDirect = AI_RGB_BLACK;

		while ( AiLightsGetSample( sg ) ) {
			if ( AiLightGetAffectDiffuse( sg->Lp ) ) {
				// get the group assigned to this light from the hash table using the light's pointer
				int lightGroup = data->lightGroups[ sg->Lp ];
				// get diffuse Weight multiplier
				float diffuseWeight = AiLightGetDiffuse( sg->Lp );

				LdiffuseDirect = AiEvaluateLightSample( sg, dmis, AiOrenNayarMISSample, AiOrenNayarMISBRDF, AiOrenNayarMISPDF ) * diffuseWeight * kti;

				if ( lightGroup >= 0 && lightGroup < NUMLIGHTGROUPS ) {
					lightGroupsDirect[ lightGroup ] += LdiffuseDirect * diffuseColor;
					shadowGroups[ lightGroup ].rgb() += sg->Liu * sg->we * std::max( 0.0f, AiV3Dot( sg->Nf, sg->Ld ) ) * sg->Lo * kti * diffuseColor * AI_ONEOVERPI;
					shadowGroups[ lightGroup ].a += maxh( sg->Lo ) * sg->we;
				}

				resultDiffuseDirect += LdiffuseDirect;
				assert( AiIsFinite( resultDiffuseDirect ) );
			}
		}
		sg->Nf = Nforig;
		sg->N = Norig;
	}

	// only do direct transmission if it's enabled and the ray is leaving a volume
	if ( doTransmission && data->transmissionDoDirect && AiV3Dot( sg->Rd, sg->N ) > 0.0f ) {
		sg->fhemi = false;
		AiLightsPrepare( sg );
		AtRGB LtransmissionDirect;

		float t_eta = ior;
		if ( AiV3Dot( sg->N, sg->Rd ) > 0.0f ) {
			t_eta = 1.0f / t_eta;
		}
		MicrofacetTransmission* mft = MicrofacetTransmission::create( sg, transmissionRoughness, transmissionRoughness, t_eta, sg->Nf, U, V );
		while ( AiLightsGetSample( sg ) ) {
			if ( AiLightGetAffectSpecular( sg->Lp ) ) {
				// get the group assigned to this light from the hash table using the light's pointer
				int lightGroup = data->lightGroups[ sg->Lp ];
				float specularWeight = AiLightGetSpecular( sg->Lp );

				LtransmissionDirect = AiEvaluateLightSample( sg, mft, MicrofacetTransmission::Sample, MicrofacetTransmission::BTDF, MicrofacetTransmission::PDF );
				LtransmissionDirect *= transmissionColor * specularWeight * kti;

				if ( doDeepGroups || sg->Rt & AI_RAY_CAMERA ) {
					if ( lightGroup >= 0 && lightGroup < NUMLIGHTGROUPS ) {
						lightGroupsDirect[ lightGroup ] += LtransmissionDirect;
					}
				}
				resultTransmissionDirect += LtransmissionDirect;
			}
		}
		AiLightsResetCache( sg );
		sg->fhemi = true;
	}

	// unset the shadows trace set
	if ( data->traceSetShadowsEnabled ) {
		// if we defined a global trace set, re-set this, otherwise, unset
		if ( data->traceSetAllEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
		} else {
			AiShaderGlobalsUnsetTraceSet( sg );
		}
	}

	// Multiply by the colors
	resultDiffuseDirectRaw = resultDiffuseDirect;
	resultDiffuseDirect *= diffuseColor;

	// Sample BRDFS
	float samples[ 2 ];
	AtRay wiRay;
	AtVector wi;
	AtScrSample scrs;
	AtVector H;
	float kr = 1, kt = 1;
	kti = 1.0f;

	// figure out whether to choose specular or transmission for russian roulette
	// TODO: unify all the IOR calculations
	bool inside = false;
	if ( AiV3Dot( sg->N, sg->Rd ) > 0.0f ) {
		inside = true;
	}

	float n1 = 1.0f;
	float n2 = ior;

	if ( inside ) {
		n1 = ior;
		n2 = 1.0f;
	}

	AiMakeRay( &wiRay, AI_RAY_REFRACTED, &sg->P, NULL, AI_BIG, sg );
	bool tir = ( !AiRefractRay( &wiRay, &sg->Nf, n1, n2, sg ) ) && inside;
	bool rrTransmission = ( doSpecular && doTransmission && sg->Rr >= data->rrTransmissionDepth && !tir && roughness == 0.0f );
	bool rrSpecular = false;
	if ( rrTransmission ) {
		kr = fresnel( AiV3Dot( -sg->Rd, sg->Nf ), eta );

		// get a permuted, stratified random number
		float u = ( float( data->permTable[ sg->Rr * data->AASamples + sg->si ] ) +
					sampleTEAFloat( sg->Rr * data->AASamples + sg->si, TEA_STREAM_ALSURFACE_RR_JITTER ) ) *
					data->AASamplesInv;
		// offset based on pixel
		float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_ALSURFACE_RR_OFFSET );
		u = fmodf( u + offset, 1.0f );

		if ( u < kr ) {
			doSpecular &= true;
			doTransmission &= false;
		} else {
			doSpecular &= false;
			doTransmission &= true;
		}
	}

	// indirectSpecular
	// -----------------
	if ( doSpecular && specularIndirectScale > 0.0f ) {
		// set the specular trace set if it's defined
		if ( data->traceSetSpecularEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetSpecular.c_str(), data->traceSetSpecularInclusive );
		}

		AtSamplerIterator* sampit = AiSamplerIterator( data->specularSampler, sg );
		// if we have perfect specular reflection, fall back to a single sample along the reflection direction
		if ( roughness == 0.0f ) {
			if ( !tir ) {
				AiStateSetMsgFlt( "HydroPreviousRoughness", std::max( roughnessX, roughnessY ) );
				sg->Nf = specularNormal;
				AiMakeRay( &wiRay, AI_RAY_GLOSSY, &sg->P, NULL, AI_BIG, sg );
				AiReflectRay( &wiRay, &sg->Nf, sg );
				AtRGB kr;

				if ( !rrTransmission ) {
					kr = brdfw.fr->kr( std::max( 0.0f, AiV3Dot( wiRay.dir, sg->Nf ) ) );
					kti = maxh( kr );
				} else {
					kr = 1.0f;
					kti = 1.0f;
				}
				// Previously we pulled the sampler here as an optimization. This
				// nets us about a 10-30%
				// speedup in the case of pure dielectrics, but severely fucks up
				// sss, both on the surface
				// being cast, and in reflected surfaces.
				// Remove this for now until we can figure out exactly what's going
				// on
				// AiSamplerGetSample( sampit, samples );

				AtRGB f = kr * specularColor * specularIndirectScale;
				bool cont = true;
				AtRGB throughput = pathThroughput * f;
				float rrP = 1.0f;

				rrP = std::min( 1.0f, sqrtf( maxh( throughput ) / maxh( pathThroughput ) ) );
				if ( data->doRR && sg->Rr > 0 ) {
					cont = false;
					// get a permuted, stratified random number
					int pb = data->AASamples * data->giSpecularSamplesSQR;
					int pi = data->giSpecularSamplesSQR * sg->si;
					float u = ( float( data->permTableSpecular[ sg->Rr * pb + pi ] ) +
								sampleTEAFloat( sg->Rr * data->AASamples + sg->si, TEA_STREAM_ALSURFACE_RR_SPEC_JITTER ) ) /
								float( pb );
					// offset based on pixel
					float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_ALSURFACE_RR_SPEC_OFFSET );
					u = fmodf( u + offset, 1.0f );

					if ( u < rrP ) {
						cont = true;
						rrP = 1.0f / rrP;
						throughput *= rrP;
						f *= rrP;
					}
				}

				if ( cont ) {
					AiStateSetMsgRGB( "HydroThroughput", throughput );
					if ( maxh( kr ) > IMPORTANCE_EPS ) {
						bool hit = AiTrace( &wiRay, &scrs );
						resultSpecularIndirect = min( scrs.color * f, rgb( data->specularIndirectClamp ) );

						if ( doDeepGroups && hit ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsSpecular[ i ] += min( deepGroupPtr[ i ] * f, rgb( data->specularIndirectClamp ) );
							}
						}
					}
				}
				sg->Nf = Nfold;
				kti = 1.0f - kti * maxh( specularColor ) * specularIndirectScale;
			}
		} else {
			AiMakeRay( &wiRay, AI_RAY_GLOSSY, &sg->P, NULL, AI_BIG, sg );
			kti = 0.0f;
			AiStateSetMsgFlt( "HydroPreviousRoughness", std::max( roughnessX, roughnessY ) );
			sg->Nf = specularNormal;
			AtRGB kr;
			int ssi = 0;
			while ( AiSamplerGetSample( sampit, samples ) ) {
				wi = AiMicrofacetMISSample( mis, float( samples[ 0 ] ), float( samples[ 1 ] ) );
				if ( AiV3Dot( wi, specularNormal ) > 0.0f ) {
					// get half-angle vector for fresnel
					wiRay.dir = wi;
					AiV3Normalize( H, wi + brdfw.V );
					kr = brdfw.fr->kr( std::max( 0.0f, AiV3Dot( H, wi ) ) );
					kti += maxh( kr );
					if ( maxh( kr ) > IMPORTANCE_EPS ) {  // only trace a ray if it's going to matter
						AtRGB brdf = AiMicrofacetMISBRDF( mis, &wi );
						float pdf = AiMicrofacetMISPDF( mis, &wi );
						if ( pdf <= 0.0f ) continue;
						AtRGB f = brdf / pdf * kr;
						AtRGB throughput = pathThroughput * f * specularColor * specularIndirectScale;
						bool cont = true;
						float rrP = 1.0f;

						rrP = std::min( 1.0f, sqrtf( maxh( throughput ) / maxh( pathThroughput ) ) );
						if ( data->doRR && sg->Rr > 0 ) {
							cont = false;
							// get a permuted, stratified random number
							int pb = data->AASamples * data->giSpecularSamplesSQR;
							int pi = data->giSpecularSamplesSQR * sg->si + ssi;
							int idx = sg->Rr * pb + pi;
							float u =	( float( data->permTableSpecular[ idx ] ) +	sampleTEAFloat( idx, TEA_STREAM_ALSURFACE_RR_SPEC_JITTER ) ) / float( pb );
							// offset based on pixel
							float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_ALSURFACE_RR_SPEC_OFFSET );
							u = fmodf( u + offset, 1.0f );

							if ( u < rrP ) {
								cont = true;
								rrP = 1.0f / rrP;
								throughput *= rrP;
								f *= rrP;
							}
						}

						AiStateSetMsgRGB( "HydroThroughput", throughput );
						// if we're in a camera ray, pass the sample index down to the
						// child SG
						if ( cont ) {
							bool hit = AiTrace( &wiRay, &scrs );
							f *= specularColor * specularIndirectScale;
							resultSpecularIndirect += min( scrs.color * f, rgb( data->specularIndirectClamp ) );

							// accumulate the lightgroup contributions calculated by
							// the child shader
							if ( doDeepGroups && hit ) {
								for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
									deepGroupsSpecular[ i ] += min( deepGroupPtr[ i ] * f, rgb( data->specularIndirectClamp ) );
								}
							}
						}
					}
				}
				ssi++;
			}  // END while( samples )

			sg->Nf = Nfold;
			resultSpecularIndirect *= AiSamplerGetSampleInvCount( sampit );
			kti *= AiSamplerGetSampleInvCount( sampit );
			kti = 1.0f - kti * maxh( specularColor );

			if ( doDeepGroups ) {
				for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
					deepGroupsSpecular[ i ] *= AiSamplerGetSampleInvCount( sampit );
				}
			}
		}

		// unset the specular trace set
		if ( data->traceSetSpecularEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(),
					data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}
	}  // if ( doSpecular )

	// indirectDiffuse
	// ----------------
	if ( doDiffuse && kti * kti2 * maxh( diffuseColor ) * diffuseIndirectScale > IMPORTANCE_EPS ) {
		// set the diffuse trace set if it's defined
		if ( data->traceSetDiffuseEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetDiffuse.c_str(), data->traceSetDiffuseInclusive );
		}

		float kr = kti * kti2;
		AtSamplerIterator* sampit = AiSamplerIterator( data->diffuseSampler, sg );
		AiMakeRay( &wiRay, AI_RAY_DIFFUSE, &sg->P, NULL, AI_BIG, sg );
		int ssi = 0;
		while ( AiSamplerGetSample( sampit, samples ) ) 		{
			// cosine hemisphere sampling as O-N sampling does not work outside of
			// a light loop
			float stheta = sqrtf( float( samples[ 0 ] ) );
			float phi = float( AI_PITIMES2 * samples[ 1 ] );
			wi.x = stheta * cosf( phi );
			wi.y = stheta * sinf( phi );
			wi.z = sqrtf( 1.0f - float( samples[ 0 ] ) );
			AiV3RotateToFrame( wi, U, V, sg->Nf );

			float cosTheta = AiV3Dot( wi, sg->Nf );
			if ( cosTheta <= 0.0f ) {
				continue;
			}

			float p = cosTheta * float( AI_ONEOVERPI );

			// trace the ray
			wiRay.dir = wi;
			AtRGB f = kr * AiOrenNayarMISBRDF( dmis, &wi ) / p;
			AtRGB throughput = pathThroughput * f * diffuseColor * diffuseIndirectScale;
			float rrP = 1.0f;
			bool cont = true;

			if ( data->doRR && sg->Rr > 0 ) {
				cont = false;
				// get a permuted, stratified random number
				int pb = data->AASamples * data->giDiffuseSamplesSQR;
				int pi = data->giDiffuseSamplesSQR * sg->si + ssi;
				int idx = sg->Rr * pb + pi;
				float u = ( float( data->permTableDiffuse[ idx ] ) + sampleTEAFloat( idx, TEA_STREAM_ALSURFACE_RR_DIFF_JITTER ) ) / float( pb );
				// offset based on pixel
				float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_ALSURFACE_RR_DIFF_OFFSET );
				u = fmodf( u + offset, 1.0f );

				rrP = std::min( 1.0f, sqrtf( maxh( throughput ) / maxh( pathThroughput ) ) );
				if ( u < rrP ) {
					cont = true;
					rrP = 1.0f / rrP;
					throughput *= rrP;
					f *= rrP;
				}
			}

			if ( cont ) {
				AiStateSetMsgRGB( "HydroThroughput", throughput );
				bool hit = AiTrace( &wiRay, &scrs );

				resultDiffuseIndirectRaw += min( scrs.color * f, rgb( data->diffuseIndirectClamp ) );

				// accumulate the lightgroup contributions calculated by the child shader
				if ( doDeepGroups && hit ) {
					for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
						deepGroupsDiffuse[ i ] +=
							min( deepGroupPtr[ i ] * f, rgb( data->diffuseIndirectClamp ) );
					}
				}
			}

			ssi++;
		}
		float invns = AiSamplerGetSampleInvCount( sampit );
		resultDiffuseIndirectRaw *= invns * diffuseIndirectScale;
		resultDiffuseIndirect = resultDiffuseIndirectRaw * diffuseColor;

		if ( doDeepGroups ) {
			for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
				deepGroupsDiffuse[ i ] *=	invns * diffuseColor * diffuseIndirectScale;
			}
		}

		// unset the diffuse trace set
		if ( data->traceSetDiffuseEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}

	}  // if ( doDiffuse )

	// refraction
	// ----------
	AtRGB childAovs[ NUMAOVs ];
	memset( childAovs, 0, sizeof( AtRGB ) * NUMAOVs );

	if ( doTransmission ) {
		float samples[ 2 ];
		float kt;
		AtRay wiRay;
		// sg->N = sg->Nf = transmissionNormal;
		AiMakeRay( &wiRay, AI_RAY_REFRACTED, &sg->P, NULL, AI_BIG, sg );
		AtVector wi, R;
		AtScrSample sample;

		AtRGB mfp = AI_RGB_WHITE / sigmaTPrime;

		// set the transmission trace set if it's defined
		if ( data->traceSetTransmissionEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetTransmission.c_str(), data->traceSetTransmissionInclusive );
		}

		float invNs = 1.0f;
		AtSamplerIterator* sampit = AiSamplerIterator( data->refractionSampler, sg );
		if ( transmissionRoughness == 0.0f ) {
			if ( !rrTransmission ) {
				kt = 1.0f - fresnel( ior, sg->N, wo, R, wi, inside );
			} else {
				kt = 1.0f;
			}

			float n1, n2;
			if ( inside ) {
				n1 = ior;
				n2 = 1.0f;
			} else {
				n1 = 1.0f;
				n2 = ior;
			}

			bool refraction = AiRefractRay( &wiRay, &sg->Nf, n1, n2, sg );
			if ( refraction ) {
				// commented this sampler pull out to match the spec. This will make
				// stuff slower, but less noisy. Need more testing before we release.
				// AiSamplerGetSample( sampit, samples );
				AtRGB throughput = pathThroughput * kti;
				AiStateSetMsgRGB( "HydroThroughput", throughput );

				if ( sg->Rr_refr < data->giRefractionDepth ) {
					if ( kti * kti2 > IMPORTANCE_EPS ) {
						AiStateSetMsgFlt( "HydroPreviousRoughness", 0.0f );
						AiTrace( &wiRay, &sample );
						AtRGB transmittance = AI_RGB_WHITE;

						if ( maxh( sigmaT ) > 0.0f && !inside ) {
							transmittance.r = fast_exp( float( -sample.z ) * sigmaT.r );
							transmittance.g = fast_exp( float( -sample.z ) * sigmaT.g );
							transmittance.b = fast_exp( float( -sample.z ) * sigmaT.b );
						}

						AtRGB f = transmittance;

						resultTransmission += min( sample.color * f, rgb( data->transmissionClamp ) );
						assert( AiIsFinite( resultTransmission ) );

						// accumulate the lightgroup contributions calculated by the child shader
						if ( doDeepGroups ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsTransmission[ i ] += min( deepGroupPtr[ i ] * f, rgb( data->transmissionClamp ) );
							}
						}

						if ( transmitAovs ) {
							for ( int i = 0; i < NUMAOVs; ++i ) {
								childAovs[ i ] += transmittedAovPtr[ i ] * f;
							}
						}

						// single scattering
						if ( doAttenuation && maxh( sigmaSPrime ) > 0.0f && !inside && doScattering ) {
							resultSs += AiSSSTraceSingleScatter(  sg, bssrdfbrdf( sigmaSPrime / sigmaTPrime ), mfp, ssDirection, ior );
						}

						if ( minh( sample.opacity ) < 1.0f ) {
							AiTraceBackground( &wiRay, &sample );
							resultTransmission +=	min( sample.color, rgb( data->transmissionClamp ) ) * ( AI_RGB_WHITE - sample.opacity );
							assert( AiIsFinite( resultTransmission ) );
						}
					}
				} else { // trace the background if we've hit the refraction depth limit
					AiTraceBackground( &wiRay, &sample );
					resultTransmission += min( sample.color, rgb( data->transmissionClamp ) );
				}
			} else { // total internal reflection
				AtRGB throughput = pathThroughput * kti;
				AiStateSetMsgRGB( "HydroThroughput", throughput );
				bool hit = AiTrace( &wiRay, &sample );

				AtRGB transmittance = AI_RGB_WHITE;
				if ( maxh( sigmaT ) > 0.0f && !inside ) {
					transmittance.r = fast_exp( float( -sample.z ) * sigmaT.r );
					transmittance.g = fast_exp( float( -sample.z ) * sigmaT.g );
					transmittance.b = fast_exp( float( -sample.z ) * sigmaT.b );
				}
				resultTransmission += min( sample.color * transmittance, rgb( data->transmissionClamp ) );
				assert( AiIsFinite( resultTransmission ) );
				// accumulate the lightgroup contributions calculated by the child shader
				if ( doDeepGroups && hit ) {
					for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
						deepGroupsTransmission[ i ] +=
							min( deepGroupPtr[ i ] * transmittance,
							rgb( data->transmissionClamp ) );
					}
				}

				if ( transmitAovs ) {
					for ( int i = 0; i < NUMAOVs; ++i ) {
						childAovs[ i ] += transmittedAovPtr[ i ] * transmittance;
					}
				}
			}
		} else {
			tir = false;

			float t_eta = ior;
			if ( AiV3Dot( sg->N, sg->Rd ) > 0.0f ) {
				t_eta = 1.0f / t_eta;
			}

			AtVector U, V;
			AiBuildLocalFramePolar( &U, &V, &sg->N );
			MicrofacetTransmission* mft = MicrofacetTransmission::create( sg, transmissionRoughness, transmissionRoughness, t_eta, sg->Nf, U, V );

			while ( AiSamplerGetSample( sampit, samples ) ) {
				AtVector m = mft->sampleMicrofacetNormal( samples[ 0 ], samples[ 1 ] );
				bool refracted = refraction( sg->Rd, m, t_eta, wi );
				wiRay.dir = wi;
				AtRGB brdf;
				float pdf;
				if ( refracted ) {
					brdf = mft->btdf( wiRay.dir );
					pdf = mft->pdf( wiRay.dir );
				} else {
					brdf = AI_RGB_WHITE;
					pdf = 1.0f;
				}

				if ( AiIsFinite( wiRay.dir ) && pdf > 1e-7f ) {
					AtRGB f = brdf / pdf;

					AtRGB throughput = pathThroughput * kti * f;
					AiStateSetMsgRGB( "HydroThroughput", throughput );
					if ( sg->Rr_refr < data->giRefractionDepth ) {
						AiStateSetMsgFlt( "HydroPreviousRoughness", transmissionRoughness );
						AiTrace( &wiRay, &sample );
						AtRGB transmittance = AI_RGB_WHITE;
						if ( maxh( sigmaT ) > 0.0f && !inside ) {
							transmittance.r = fast_exp( float( -sample.z ) * sigmaT.r );
							transmittance.g = fast_exp( float( -sample.z ) * sigmaT.g );
							transmittance.b = fast_exp( float( -sample.z ) * sigmaT.b );
							f *= transmittance;
						}

						resultTransmission += min( sample.color * f, rgb( data->transmissionClamp ) );
						assert( AiIsFinite( resultTransmission ) );
						// accumulate the lightgroup contributions calculated by the
						// child shader
						if ( doDeepGroups ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsTransmission[ i ] += min( 
									deepGroupPtr[ i ] * f, rgb( data->transmissionClamp ) );
							}
						}

						if ( transmitAovs ) {
							for ( int i = 0; i < NUMAOVs; ++i ) {
								childAovs[ i ] += transmittedAovPtr[ i ] * f;
							}
						}

						// single scattering
						if ( doAttenuation && maxh( sigmaSPrime ) > 0.0f && !inside && doScattering ) {
							AtVector N = sg->N;
							sg->N = m;
							resultSs += AiSSSTraceSingleScatter(  sg, bssrdfbrdf( sigmaSPrime / sigmaTPrime ), mfp, ssDirection, ior );
							sg->N = N;
						}

						if ( minh( sample.opacity ) < 1.0f ) {
							AiTraceBackground( &wiRay, &sample );
							resultTransmission += min( sample.color * f, rgb( data->transmissionClamp ) ) * ( AI_RGB_WHITE - sample.opacity );
							assert( AiIsFinite( resultTransmission ) );
						}
					} else { // trace the background if we've hit nothing
						AiTraceBackground( &wiRay, &sample );
						resultTransmission += min( sample.color * f, rgb( data->transmissionClamp ) );
						assert( AiIsFinite( resultTransmission ) );
					}
				}
			}

			invNs = AiSamplerGetSampleInvCount( sampit );
		}

		if ( rrTransmission || tir ) {
			kti = kti2 = 1.0f;
		}

		resultTransmission *= invNs * transmissionColor * kti * kti2;
		assert( AiIsFinite( resultTransmission ) );
		resultSs *= invNs * transmissionColor * kti * kti2;

		if ( doDeepGroups ) {
			for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
				deepGroupsTransmission[ i ] *= invNs * transmissionColor * kti * kti2;
			}
		}

		if ( transmitAovs ) {
			for ( int i = 0; i < NUMAOVs; ++i ) {
				childAovs[ i ] *= invNs * transmissionColor * kti * kti2;
			}
		}

		// unset the transmission trace set
		if ( data->traceSetTransmissionEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}

		// reset the normal
		sg->N = Norig;
		sg->Nf = Nforig;

	}  // if ( doTransmission )

	resultTransmission += resultTransmissionDirect;
	assert( AiIsFinite( resultTransmission ) );

	// Emission
	resultEmission = emissionColor * kti * kti2;

	// Now accumulate the deep group brdf results onto the relevant samples
	if ( sg->Rt & AI_RAY_CAMERA ) {
		if ( data->standardAovs ) {
			AtRGB tmp;
			tmp = resultDiffuseDirect;
			if ( tmp != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDirectDiffuse ].c_str(), tmp );
			}

			tmp = resultDiffuseIndirect;
			if ( tmp != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kIndirectDiffuse ].c_str(), tmp );
			}

			tmp = resultSpecularDirect;
			if ( tmp != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDirectSpecular ].c_str(), tmp );
			}

			tmp = resultSpecularIndirect;
			if ( tmp != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kIndirectSpecular ].c_str(), tmp );
			}

			tmp = resultTransmission + resultSs;
			if ( tmp != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kRefraction ].c_str(), tmp );
			}

			if ( resultEmission != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kEmission ].c_str(), resultEmission );
			}
		} else if ( transmitAovs && doTransmission ) {
			for ( int i = 0; i < NUMAOVs; ++i ) {
				if ( i == kRefraction ) {
					if ( resultTransmission != AI_RGB_BLACK ) {
						AiAOVSetRGB( sg, data->aovs[ kRefraction ].c_str(), resultTransmission );
					}
					continue;
				} else if ( i == kDirectSpecular ) {
					AtRGB tmp = resultSpecularDirect + transmittedAovPtr[ kDirectSpecular ];
					if ( tmp != AI_RGB_BLACK ) {
						AiAOVSetRGB( sg, data->aovs[ kDirectSpecular ].c_str(), tmp );
					}
					continue;
				} else if ( i == kIndirectSpecular ) {
					AtRGB tmp = resultSpecularIndirect + transmittedAovPtr[ kIndirectSpecular ];
					if ( tmp != AI_RGB_BLACK ) {
						AiAOVSetRGB( sg, data->aovs[ kIndirectSpecular ].c_str(), tmp );
					}
					continue;
				}

				if ( transmittedAovPtr[ i ] != AI_RGB_BLACK ) {
					AiAOVSetRGB( sg, data->aovs[ i ].c_str(), transmittedAovPtr[ i ] );
				}
			}
		} else {
			//diffuseColor = min( diffuseColor, rgb( data->aovDiffuseColorClamp ) );
			//resultDiffuseDirect = min( resultDiffuseDirect, rgb( data->aovDirectDiffuseClamp ) );
			//resultDiffuseDirectRaw = min( resultDiffuseDirectRaw, rgb( data->aovDirectDiffuseRawClamp ) );
			//resultSpecularDirect = min( resultSpecularDirect, rgb( data->aovDirectSpecularClamp ) );
			//resultDiffuseIndirect = min( resultDiffuseIndirect, rgb( data->aovIndirectDiffuseClamp ) );
			//resultDiffuseIndirectRaw = min( resultDiffuseIndirectRaw, rgb( data->aovIndirectDiffuseRawClamp ) );
			//resultSpecularIndirect = min( resultSpecularIndirect, rgb( data->aovIndirectSpecularClamp ) );
			//resultSs = min( resultSs, rgb( data->aovSingleScatterClamp ) );
			//resultTransmission = min( resultTransmission, rgb( data->aovRefractionClamp ) );
			//resultEmission = min( resultEmission, rgb( data->aovEmissionClamp ) );

			if ( diffuseColor != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDiffuseColor ].c_str(), diffuseColor );
			}

			if ( resultDiffuseDirect != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDirectDiffuse ].c_str(), resultDiffuseDirect );
			}

			if ( resultDiffuseDirectRaw != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDirectDiffuseRaw ].c_str(),resultDiffuseDirectRaw );
			}

			if ( resultSpecularDirect != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kDirectSpecular ].c_str(), resultSpecularDirect );
			}

			if ( resultDiffuseIndirect != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kIndirectDiffuse ].c_str(), resultDiffuseIndirect );
			}

			if ( resultDiffuseIndirectRaw != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kIndirectDiffuseRaw ].c_str(), resultDiffuseIndirectRaw );
			}

			if ( resultSpecularIndirect != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kIndirectSpecular ].c_str(), resultSpecularIndirect );
			}

			if ( resultSs != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kSingleScatter ].c_str(), resultSs );
			}

			if ( resultTransmission != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kRefraction ].c_str(), resultTransmission );
			}

			if ( resultEmission != AI_RGB_BLACK ) {
				AiAOVSetRGB( sg, data->aovs[ kEmission ].c_str(), resultEmission );
			}

			// write data AOVs
			AtRGB uv = AiColorCreate( sg->u, sg->v, 0.0f );
			AiAOVSetRGB( sg, data->aovs[ kUv ].c_str(), uv );
			AtRGB depth = AiColorCreate( float( sg->Rl ), AiV3Dot( sg->Nf, wo ), sg->P.y );
			AiAOVSetRGB( sg, data->aovs[ kDepth ].c_str(), depth );
		}
	} else { // we're in a secondary ray //
		if ( doDeepGroups ) {
			for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
				deepGroupPtr[ i ] =	deepGroupsDiffuse[ i ] +
									deepGroupsSpecular[ i ] +
									deepGroupsTransmission[ i ] +
									lightGroupsDirect[ i ];
			}
		} else if ( transmitAovs ) {
			if ( doTransmission ) {
				for ( int i = 0; i < NUMAOVs; ++i ) {
					transmittedAovPtr[ i ] = childAovs[ i ];
				}
			} else {
				transmittedAovPtr[ kDiffuseColor ] = diffuseColor;
				transmittedAovPtr[ kDirectDiffuse ] = resultDiffuseDirect;
				transmittedAovPtr[ kDirectDiffuseRaw ] = resultDiffuseDirectRaw;
				transmittedAovPtr[ kIndirectDiffuse ] = resultDiffuseIndirect;
				transmittedAovPtr[ kIndirectDiffuseRaw ] =	resultDiffuseIndirectRaw;
				transmittedAovPtr[ kDirectSpecular ] = resultSpecularDirect;
				transmittedAovPtr[ kIndirectSpecular ] = resultSpecularIndirect;
				transmittedAovPtr[ kSingleScatter ] = resultSs;
				transmittedAovPtr[ kRefraction ] = resultTransmission;
				transmittedAovPtr[ kEmission ] = resultEmission;
				transmittedAovPtr[ kUv ] = rgb( sg->u, sg->v, 0.0f );
				transmittedAovPtr[ kDepth ] = rgb( float( sg->Rl ), AiV3Dot( sg->Nf, wo ), 0.0f );
			}
		}
	}

	assert( AiIsFinite( resultDiffuseDirect ) );
	assert( AiIsFinite( resultSpecularDirect ) );
	assert( AiIsFinite( resultDiffuseIndirect ) );
	assert( AiIsFinite( resultSpecularIndirect ) );
	assert( AiIsFinite( resultSs ) );
	assert( AiIsFinite( resultTransmission ) );
	assert( AiIsFinite( resultEmission ) );

	// Sum final result from temporaries
	//
	sg->out.RGB =	resultDiffuseDirect + resultDiffuseIndirect + 
					resultSpecularDirect + resultSpecularIndirect +
					resultSs +
					resultTransmission + resultEmission;

	assert( AiIsFinite( sg->out.RGB ) );
}
