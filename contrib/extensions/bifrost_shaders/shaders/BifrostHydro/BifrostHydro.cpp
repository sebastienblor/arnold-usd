#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <vector>

#include <ai.h>

#include "HydroTypes.h"
#include "Hydro3rdParty.h"
#include "HydroMIS.h"

AI_SHADER_NODE_EXPORT_METHODS(  BifrostHydroMtd  )

static const char* interiorTypes[] = {"None", "AttenuationColor+Weight", "TransmissionColor+Radius", NULL};

enum BifrostHydroAIParams
{
	// diffuse
	pDiffuseIntensity = 0,
	pDiffuseColor,
	pDiffuseRoughness,
	pDiffuseIndirectScale,

	// emission
	pEmissionWeight,
	pEmissionColor,

	// specular
	pIor,
	pSpecularRoughness,
	pReflectionIntensity,
	pReflectionColor,
	pRefractionIntensity,
	pRefractionColor,
	pSpecularIndirectScale,

	// single scattering
	pInteriorType,
	pDensityScale,
	pScatterDirection,
	pAttenuationColor,
	pAttenuationWeight,
	pTransmissionColor,
	pTransmissionRadius,

	pShadowTransmission,

	pRRDepth,
	pRoughnessDepthScale,
	
	pAovDiffuseColor,
	pAovDirectDiffuse,
	pAovDirectDiffuseRaw,
	pAovIndirectDiffuse,
	pAovIndirectDiffuseRaw,
	pAovDirectSpecular,
	pAovIndirectSpecular,
	pAovInterior,
	pAovRefraction,
	pAovEmission,
	pAovUv,
	pAovDepth,

	pStandardAovs,
	pTransmitAovs,

	pDebug,

	pBump
};

node_parameters
{
	AiParameterFLT( "diffuseIntensity", 0.0f );
	AiParameterRGB( "diffuseColor", 0.0f, 0.05f, 0.144f );
	AiParameterFLT( "diffuseRoughness", 0.0f );
	AiParameterFLT( "diffuseIndirectScale", 1.0f );

	AiParameterFLT( "emissionIntensity", 0.0f );
	AiParameterRGB( "emissionColor", 1.0f, 1.0f, 1.0f );

	AiParameterFLT( "ior", 1.35f );
	AiParameterFLT( "specularRoughness", 0.0f );
	AiParameterFLT( "reflectionIntensity", 1.0f );
	AiParameterRGB( "reflectionColor", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "refractionIntensity", 1.0f );
	AiParameterRGB( "refractionColor", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "specularIndirectScale", 1.0f );

	AiParameterENUM( "interiorType", 0, interiorTypes );
	AiParameterFLT( "densityScale", 1.0f );
	AiParameterFLT( "scatterDirection", 0.75f );
	AiParameterRGB( "attenuationColor", 0.85f, 1.0f, 1.0f );
	AiParameterFLT( "attenuationWeight", 1.0f );
	AiParameterRGB( "transmissionColor", 0.5f, 0.5f, 0.5f );
	AiParameterRGB( "transmissionRadius", 1.0f, 1.0f, 1.0f );

	AiParameterRGB( "shadowTransmission", 1.0f, 1.0f, 1.0f );

	AiParameterInt( "rrDepth", 1 );
	AiParameterFLT( "roughnessDepthScale", 1.0f );

	// AOVs
	AiParameterStr( "aovDiffuseColor", "diffuseColor" );
	AiParameterStr( "aovDirectDiffuse", "directDiffuse" );
	AiParameterStr( "aovDirectDiffuseRaw", "directDiffuseRaw" );
	AiParameterStr( "aovIndirectDiffuse", "indirectDiffuse" );
	AiParameterStr( "aovIndirectDiffuseRaw", "indirectDiffuseRaw" );
	AiParameterStr( "aovDirectSpecular", "directSpecular" );
	AiParameterStr( "aovIndirectSpecular", "indirectSpecular" );
	AiParameterStr( "aovInterior", "interior" );
	AiParameterStr( "aovRefraction", "refraction" );
	AiParameterStr( "aovEmission", "emission" );
	AiParameterStr( "aovUv", "uv" );
	AiParameterStr( "aovDepth", "depth" );

	AiParameterBool( "standardCompatibleAOVs", false );
	AiParameterBool( "transmitAovs", false );

	AiParameterInt( "debug", 0 );

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
}

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
	data->giRefractionDepth = AiNodeGetInt(  options, "GI_refraction_depth"  );

	data->giSpecularSamples = AiNodeGetInt(  options, "GI_glossy_samples" );
	data->giSpecularSamplesSQR = SQR(  data->giSpecularSamples );

	data->giDiffuseSamples = AiNodeGetInt( options, "GI_diffuse_samples" );
	data->giDiffuseSamplesSQR = SQR( data->giDiffuseSamples );

	data->giRefractionSamples = AiNodeGetInt( options, "GI_refraction_samples" );
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

	// Set up info for RR
	data->rrDepth = params[ pRRDepth ].INT;
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

    data->debug = params[ pDebug ].INT;

}

shader_evaluate
{
    ShaderData* data = ( ShaderData* )AiNodeGetLocalData( node );

	// get params
	float ior = AiShaderEvalParamFlt( pIor );
	float roughness = AiShaderEvalParamFlt( pSpecularRoughness );

	AtRGB diffuseColor = AiShaderEvalParamRGB( pDiffuseColor ) * AiShaderEvalParamFlt( pDiffuseIntensity );
	float diffuseRoughness = AiShaderEvalParamFlt( pDiffuseRoughness );
	float diffuseIndirectScale = AiShaderEvalParamFlt( pDiffuseIndirectScale );

	AtRGB reflectionColor = AiShaderEvalParamRGB( pReflectionColor ) * AiShaderEvalParamFlt( pReflectionIntensity );
	float roughnessDepthScale =	AiShaderEvalParamFlt( pRoughnessDepthScale );
	float specularIndirectScale = AiShaderEvalParamFlt( pSpecularIndirectScale );

	AtRGB refractionColor = AiShaderEvalParamRGB( pRefractionColor ) * AiShaderEvalParamFlt( pRefractionIntensity );

	InteriorType interiorType = (InteriorType) AiShaderEvalParamInt( pInteriorType );
	float densityScale = AiShaderEvalParamFlt( pDensityScale );
	densityScale = std::max<float>(0.0001f, densityScale);
	float scatterDirection = AiShaderEvalParamFlt( pScatterDirection );
	AtRGB attenuationColor = AiShaderEvalParamRGB( pAttenuationColor );
	float attenuationWeight = AiShaderEvalParamFlt( pAttenuationWeight );
	AtRGB transmissionColor = AiShaderEvalParamRGB( pTransmissionColor );
	AtRGB transmissionRadius = AiShaderEvalParamRGB( pTransmissionRadius ) / densityScale;

	AtRGB emissionColor = AiShaderEvalParamRGB( pEmissionColor ) * AiShaderEvalParamFlt( pEmissionWeight );

	// precalc values
	float eta = 1.0f / ior;
	roughness *= roughness;

	// balance diffuse, transmission
	// shouldn't we do this after energy conservation?
	float tSum = maxh( refractionColor ) + maxh( diffuseColor );
	if ( tSum > 1.0f ) {
		float tSumInv = 1.0f / tSum;
		refractionColor *= tSumInv;
		diffuseColor *= tSumInv;
	}

	// if the attenuation color is anything but white, make sure we don't have some channels left at pure white
	if ( minh( attenuationColor ) < 1.0f ) {
		attenuationColor = clamp( attenuationColor, AI_RGB_BLACK, rgb( 0.999f ) );
	}

	if ( minh( transmissionColor ) < 1.0f ) {
		transmissionColor = clamp( transmissionColor, AI_RGB_BLACK, rgb( 0.999f ) );
	}

	// precalculate scattering coefficients as we'll need them for shadows etc.
	AtRGB sigmaT = AI_RGB_BLACK;
	AtRGB sigmaS = AI_RGB_BLACK;
	AtRGB sigmaA = AI_RGB_BLACK;
	AtRGB sigmaSPrime = AI_RGB_BLACK;
	AtRGB sigmaTPrime = AI_RGB_BLACK;
	bool doAttenuation = false;
	bool doScattering = false;
	if ( (minh( attenuationColor ) < 1 || attenuationWeight > 0 || interiorType == COEFFICIENTS) && interiorType != NONE ) {
		if ( interiorType == COEFFICIENTS ) {
			// we are given albedo and dmfp so we need sigmaS and sigmaA from those
			alphaInversion( transmissionColor, transmissionRadius, sigmaSPrime, sigmaA );
			sigmaS = sigmaSPrime / (1.0f - scatterDirection);

			if ( maxh( sigmaA ) > 0 ) {
				doAttenuation = true;
			}

			if ( maxh( sigmaS ) > 0 ) {
				doScattering = true;
			}
		} else {
			sigmaA = ( AI_RGB_WHITE - attenuationColor ) * densityScale;
			// if we're doing a scattering medium, then make sure absorption is not zero
			if ( attenuationWeight > 0 ) {
				sigmaA = max( sigmaA, rgb( 0.0001f ) );
			}

			sigmaS = attenuationColor * attenuationWeight * densityScale;

			if ( maxh( sigmaA ) > 0 ) {
				doAttenuation = true;
			}

			if ( maxh( sigmaS ) > 0 ) {
				doScattering = true;
			}
		}

		sigmaT = sigmaS + sigmaA;
		sigmaSPrime = sigmaS * ( 1.0f - scatterDirection );
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
	if ( sg->Rr > 0 ) {
		AiStateGetMsgRGB( "HydroThroughput", &pathThroughput );
	}

	// if it's a shadow ray, handle shadow colouring through absorption
	if ( sg->Rt & AI_RAY_SHADOW ) {
		// if the object is transmissive
		AtRGB out_opacity = AI_RGB_WHITE;
		if ( maxh( refractionColor ) ) {
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
						out_opacity = 1.0f - ( ( 1.0f - out_opacity ) * kt * refractionColor );
					} else {
						// first intersection
						// tell the next shader invocation that we're now inside the surface and what our extinction coefficient is
						AiStateSetMsgRGB( "HydroPreviousSigmaT", sigmaTPrime );
						AiStateSetMsgBool( "HydroInside", true );
						out_opacity = 1.0f - kt * refractionColor;
					}
				} else { // no extinction, shadows are fresnel only.
					AiStateSetMsgRGB( "HydroPreviousSigmaT", AI_RGB_BLACK );
					out_opacity = 1.0f - kt * refractionColor;
				}
			}
		}

		// store intersection position
		AiStateSetMsgPnt( "HydroPreviousIntersection", sg->P );
		sg->out_opacity = out_opacity;
		return;
	}

	// Evaluate bump;
	// AtRGB bump = AiShaderEvalParamRGB( pBump );

	// set ray type to undefined
	int HydroRayType = HYDRO_RAY_UNDEFINED;
	AiStateSetMsgInt( "HydroRayType", HYDRO_RAY_UNDEFINED );

	// Grab the roughness from the previous surface and make sure we're slightly
	// rougher than it to avoid specular-specular fireflies
	float HydroPreviousRoughness = 0.0f;
	if ( sg->Rr > 0 ) {
		AiStateGetMsgFlt( "HydroPreviousRoughness", &HydroPreviousRoughness );
		roughness =	std::max( roughness, HydroPreviousRoughness * roughnessDepthScale );
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
			maxh( reflectionColor ) < IMPORTANCE_EPS || // disable specular if contribution is small
			roughness > 1.0f || // kill specular if roughness has been scaled up too far
			( HydroPreviousRoughness > 0.0f && roughness ==0.0f ) )
	{
		doSpecular = false;
	}

	if ( ( sg->Rr_diff > 0 ) || // disable transmitted caustics
		maxh( refractionColor ) < IMPORTANCE_EPS ) // disable transmission if contribution is small
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

	void* mis;
	mis = AiMicrofacetMISCreateData( sg, AI_MICROFACET_GGX, &U, 0, roughness, roughness );

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
	brdfw.N = sg->Nf;
	brdfw.kr = 0.0f;

	void* dmis = AiOrenNayarMISCreateData( sg, diffuseRoughness );
	sg->fhemi = true;

	AtRGBA shadowGroups[ NUMLIGHTGROUPS ];
	memset( shadowGroups, 0, sizeof( AtRGBA ) * NUMLIGHTGROUPS );

	AtVector Nforig = sg->Nf;
	AtVector Norig = sg->N;
	if ( doSpecular ) {
		AiLightsPrepare( sg );

		while ( AiLightsGetSample( sg ) ) {
			if ( AiLightGetAffectSpecular( sg->Lp ) ){
				// get the group assigned to this light from the hash table using
				// the light's pointer
				int lightGroup = data->lightGroups[ sg->Lp ];

				// per-light specular and diffuse Weight multipliers
				float specularWeight = AiLightGetSpecular( sg->Lp );

				brdfw.ibs = false;
				AtRGB LspecularDirect = AiEvaluateLightSample( sg, &brdfw, AiMicrofacetMISSampleWrap, AiMicrofacetMISBRDFWrap, AiMicrofacetMISPDFWrap ) * specularWeight * reflectionColor;
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

		if ( brdfw.ns > 0.0f ) {
			kti = 1.0f - ( maxh( brdfw.krInt ) / brdfw.ns * maxh( reflectionColor ) );
			assert( AiIsFinite( kti ) );
		}
	}

	if ( doDiffuse ) {
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
				assert( isFiniteColor( resultDiffuseDirect ) );
			}
		}
		sg->Nf = Nforig;
		sg->N = Norig;
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
	bool rrTransmission = ( doSpecular && doTransmission && sg->Rr >= data->rrDepth && !tir && roughness == 0.0f );
	bool rrSpecular = false;
	if ( rrTransmission ) {
		kr = fresnel( AiV3Dot( -sg->Rd, sg->Nf ), eta );

		// get a permuted, stratified random number
		float u = ( float( data->permTable[ sg->Rr * data->AASamples + sg->si ] ) +
					sampleTEAFloat( sg->Rr * data->AASamples + sg->si, TEA_STREAM_HYDRO_RR_JITTER ) ) *
					data->AASamplesInv;
		// offset based on pixel
		float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_HYDRO_RR_OFFSET );
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
		AtSamplerIterator* sampit = AiSamplerIterator( data->specularSampler, sg );
		// if we have perfect specular reflection, fall back to a single sample along the reflection direction
		if ( roughness == 0.0f ) {
			if ( !tir ) {
				AiStateSetMsgFlt( "HydroPreviousRoughness", std::max( roughness, roughness ) );
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

				AtRGB f = kr * reflectionColor * specularIndirectScale;
				bool cont = true;
				AtRGB throughput = pathThroughput * f;
				float rrP = 1.0f;

				rrP = std::min( 1.0f, sqrtf( maxh( throughput ) / maxh( pathThroughput ) ) );
				if ( sg->Rr > 0 ) {
					cont = false;
					// get a permuted, stratified random number
					int pb = data->AASamples * data->giSpecularSamplesSQR;
					int pi = data->giSpecularSamplesSQR * sg->si;
					float u = ( float( data->permTableSpecular[ sg->Rr * pb + pi ] ) +
								sampleTEAFloat( sg->Rr * data->AASamples + sg->si, TEA_STREAM_HYDRO_RR_SPEC_JITTER ) ) /
								float( pb );
					// offset based on pixel
					float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_HYDRO_RR_SPEC_OFFSET );
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
						resultSpecularIndirect = scrs.color * f;

						if ( doDeepGroups && hit ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsSpecular[ i ] += deepGroupPtr[ i ] * f;
							}
						}
					}
				}
				kti = 1.0f - kti * maxh( reflectionColor ) * specularIndirectScale;
			}
		} else {
			AiMakeRay( &wiRay, AI_RAY_GLOSSY, &sg->P, NULL, AI_BIG, sg );
			kti = 0.0f;
			AiStateSetMsgFlt( "HydroPreviousRoughness", std::max( roughness, roughness ) );
			AtRGB kr;
			int ssi = 0;
			while ( AiSamplerGetSample( sampit, samples ) ) {
				wi = AiMicrofacetMISSample( mis, float( samples[ 0 ] ), float( samples[ 1 ] ) );
				if ( AiV3Dot( wi, sg->Nf ) > 0.0f ) {
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
						AtRGB throughput = pathThroughput * f * reflectionColor * specularIndirectScale;
						bool cont = true;
						float rrP = 1.0f;

						rrP = std::min( 1.0f, sqrtf( maxh( throughput ) / maxh( pathThroughput ) ) );
						if ( sg->Rr > 0 ) {
							cont = false;
							// get a permuted, stratified random number
							int pb = data->AASamples * data->giSpecularSamplesSQR;
							int pi = data->giSpecularSamplesSQR * sg->si + ssi;
							int idx = sg->Rr * pb + pi;
							float u =	( float( data->permTableSpecular[ idx ] ) +	sampleTEAFloat( idx, TEA_STREAM_HYDRO_RR_SPEC_JITTER ) ) / float( pb );
							// offset based on pixel
							float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_HYDRO_RR_SPEC_OFFSET );
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
							f *= reflectionColor * specularIndirectScale;
							resultSpecularIndirect += scrs.color * f;

							// accumulate the lightgroup contributions calculated by
							// the child shader
							if ( doDeepGroups && hit ) {
								for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
									deepGroupsSpecular[ i ] += deepGroupPtr[ i ] * f;
								}
							}
						}
					}
				}
				ssi++;
			}  // END while( samples )

			resultSpecularIndirect *= AiSamplerGetSampleInvCount( sampit );
			kti *= AiSamplerGetSampleInvCount( sampit );
			kti = 1.0f - kti * maxh( reflectionColor );

			if ( doDeepGroups ) {
				for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
					deepGroupsSpecular[ i ] *= AiSamplerGetSampleInvCount( sampit );
				}
			}
		}
	}  // if ( doSpecular )

	// indirectDiffuse
	// ----------------
	if ( doDiffuse && kti * kti2 * maxh( diffuseColor ) * diffuseIndirectScale > IMPORTANCE_EPS ) {
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

			if ( sg->Rr > 0 ) {
				cont = false;
				// get a permuted, stratified random number
				int pb = data->AASamples * data->giDiffuseSamplesSQR;
				int pi = data->giDiffuseSamplesSQR * sg->si + ssi;
				int idx = sg->Rr * pb + pi;
				float u = ( float( data->permTableDiffuse[ idx ] ) + sampleTEAFloat( idx, TEA_STREAM_HYDRO_RR_DIFF_JITTER ) ) / float( pb );
				// offset based on pixel
				float offset = sampleTEAFloat( sg->y * data->xres + sg->x, TEA_STREAM_HYDRO_RR_DIFF_OFFSET );
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

				resultDiffuseIndirectRaw += scrs.color * f;

				// accumulate the lightgroup contributions calculated by the child shader
				if ( doDeepGroups && hit ) {
					for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
						deepGroupsDiffuse[ i ] += deepGroupPtr[ i ] * f;
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
	}  // if ( doDiffuse )

	// refraction
	// ----------
	AtRGB childAovs[ NUMAOVs ];
	memset( childAovs, 0, sizeof( AtRGB ) * NUMAOVs );

	if ( doTransmission ) {
		float samples[ 2 ];
		float kt;
		AtRay wiRay;

		AiMakeRay( &wiRay, AI_RAY_REFRACTED, &sg->P, NULL, AI_BIG, sg );
		AtVector wi, R;
		AtScrSample sample;

		AtRGB mfp = AI_RGB_WHITE / sigmaTPrime;

		float invNs = 1.0f;
		AtSamplerIterator* sampit = AiSamplerIterator( data->refractionSampler, sg );
		if ( roughness == 0.0f ) {
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

						resultTransmission += sample.color * f;
						assert( isFiniteColor( resultTransmission ) );

						// accumulate the lightgroup contributions calculated by the child shader
						if ( doDeepGroups ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsTransmission[ i ] += deepGroupPtr[ i ] * f;
							}
						}

						if ( transmitAovs ) {
							for ( int i = 0; i < NUMAOVs; ++i ) {
								childAovs[ i ] += transmittedAovPtr[ i ] * f;
							}
						}

						// single scattering
						if ( doAttenuation && maxh( sigmaSPrime ) > 0.0f && !inside && doScattering ) {
							resultSs += AiSSSTraceSingleScatter(  sg, bssrdfbrdf( sigmaSPrime / sigmaTPrime ), mfp, scatterDirection, ior );
						}

						if ( minh( sample.opacity ) < 1.0f ) {
							AiTraceBackground( &wiRay, &sample );
							resultTransmission +=	sample.color;
							assert( isFiniteColor( resultTransmission ) );
						}
					}
				} else { // trace the background if we've hit the refraction depth limit
					AiTraceBackground( &wiRay, &sample );
					resultTransmission += sample.color;
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
				resultTransmission += sample.color * transmittance;
				assert( isFiniteColor( resultTransmission ) );
				// accumulate the lightgroup contributions calculated by the child shader
				if ( doDeepGroups && hit ) {
					for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
						deepGroupsTransmission[ i ] += deepGroupPtr[ i ] * transmittance;
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
			MicrofacetTransmission* mft = MicrofacetTransmission::create( sg, roughness, roughness, t_eta, sg->Nf, U, V );

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
						AiStateSetMsgFlt( "HydroPreviousRoughness", roughness );
						AiTrace( &wiRay, &sample );
						AtRGB transmittance = AI_RGB_WHITE;
						if ( maxh( sigmaT ) > 0.0f && !inside ) {
							transmittance.r = fast_exp( float( -sample.z ) * sigmaT.r );
							transmittance.g = fast_exp( float( -sample.z ) * sigmaT.g );
							transmittance.b = fast_exp( float( -sample.z ) * sigmaT.b );
							f *= transmittance;
						}

						resultTransmission += sample.color * f;
						assert( isFiniteColor( resultTransmission ) );
						// accumulate the lightgroup contributions calculated by the
						// child shader
						if ( doDeepGroups ) {
							for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
								deepGroupsTransmission[ i ] += deepGroupPtr[ i ] * f;
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
							resultSs += AiSSSTraceSingleScatter(  sg, bssrdfbrdf( sigmaSPrime / sigmaTPrime ), mfp, scatterDirection, ior );
							sg->N = N;
						}

						if ( minh( sample.opacity ) < 1.0f ) {
							AiTraceBackground( &wiRay, &sample );
							resultTransmission += sample.color * f;
							assert( isFiniteColor( resultTransmission ) );
						}
					} else { // trace the background if we've hit nothing
						AiTraceBackground( &wiRay, &sample );
						resultTransmission += sample.color * f;
						assert( isFiniteColor( resultTransmission ) );
					}
				}
			}

			invNs = AiSamplerGetSampleInvCount( sampit );
		}

		if ( rrTransmission || tir ) {
			kti = kti2 = 1.0f;
		}

		resultTransmission *= invNs * refractionColor * kti * kti2;
		assert( isFiniteColor( resultTransmission ) );
		resultSs *= invNs * refractionColor * kti * kti2;

		if ( doDeepGroups ) {
			for ( int i = 0; i < NUMLIGHTGROUPS; ++i ) {
				deepGroupsTransmission[ i ] *= invNs * refractionColor * kti * kti2;
			}
		}

		if ( transmitAovs ) {
			for ( int i = 0; i < NUMAOVs; ++i ) {
				childAovs[ i ] *= invNs * refractionColor * kti * kti2;
			}
		}

		// reset the normal
		sg->N = Norig;
		sg->Nf = Nforig;

	}  // if ( doTransmission )

	assert( isFiniteColor( resultTransmission ) );

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
				AiAOVSetRGB( sg, data->aovs[ kInterior ].c_str(), resultSs );
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
				transmittedAovPtr[ kInterior ] = resultSs;
				transmittedAovPtr[ kRefraction ] = resultTransmission;
				transmittedAovPtr[ kEmission ] = resultEmission;
				transmittedAovPtr[ kUv ] = rgb( sg->u, sg->v, 0.0f );
				transmittedAovPtr[ kDepth ] = rgb( float( sg->Rl ), AiV3Dot( sg->Nf, wo ), 0.0f );
			}
		}
	}

	assert( isFiniteColor( resultDiffuseDirect ) );
	assert( isFiniteColor( resultSpecularDirect ) );
	assert( isFiniteColor( resultDiffuseIndirect ) );
	assert( isFiniteColor( resultSpecularIndirect ) );
	assert( isFiniteColor( resultSs ) );
	assert( isFiniteColor( resultTransmission ) );
	assert( isFiniteColor( resultEmission ) );

	// Sum final result from temporaries
	//
	sg->out.RGB =	resultDiffuseDirect + resultDiffuseIndirect + 
					resultSpecularDirect + resultSpecularIndirect +
					resultSs +
					resultTransmission + resultEmission;

	assert( isFiniteColor( sg->out.RGB ) );
}
