#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>

#include <string.h>

#include <ai.h>

#include <iostream>
#define DL std::cerr << __FILE__ << ":" << __LINE__ << " (" << __PRETTY_FUNCTION__ << ")" << std::endl

#include "PointDefs.h"
#include "PointBRDF.h"
#include "PointUtils.h"

#ifdef MSVC
#define CRTSECURENOWARNINGS 1
#endif

AI_SHADER_NODE_EXPORT_METHODS(  BifrostPointMethod  )

namespace
{

struct ShaderData
{
    ShaderData() {}

	bool densityNormalConnected;

	std::string traceSetAll;
	bool traceSetAllEnabled;
	bool traceSetAllInclusive;

	std::string traceSetShadows;
	bool traceSetShadowsEnabled;
	bool traceSetShadowsInclusive;

	std::string traceSetDiffuse;
	bool traceSetDiffuseEnabled;
	bool traceSetDiffuseInclusive;

	std::string traceSetBacklight;
	bool traceSetBacklightEnabled;
	bool traceSetBacklightInclusive;

	// AOV names
	std::vector<std::string> aovs;
	std::vector<std::string> aovs_rgba;

	int debug;
};

enum BifrostPointAIParams
{
	pDiffuseIntensity,
	pDiffuseColor,
	pScatterPhase,
	pScatterDual,
	pScatterDirection1,
	pScatterDirection2,
	pScatterBlend,
	pDiffuseIndirectScale,
	pDensityNormal,

	pBacklightIntensity,
	pBacklightColor,
	pBacklightIndirectScale,

	// emission
	pEmissionWeight,
	pEmissionColor,

	pAiEnableMatte,
	pAiMatteColor,
	pAiMatteColorA,
	
	pShadowTransmission,
	pOpacity,

	pTraceSetAll,
	pTraceSetShadows,
	pTraceSetDiffuse,
	pTraceSetBacklight,

	pDebug,

	// AOVs
	pAovDensityNormal,
	pAovDiffuseColor,
	pAovDirectDiffuse,
	pAovDirectDiffuseRaw,
	pAovIndirectDiffuse,
	pAovIndirectDiffuseRaw,
	pAovDirectBacklight,
	pAovDirectBacklightRaw,
	pAovIndirectBacklight,
	pAovIndirectBacklightRaw,
	pAovEmission,
	pAovLightGroup1,
	pAovLightGroup2,
	pAovLightGroup3,
	pAovLightGroup4,
	pAovLightGroup5,
	pAovLightGroup6,
	pAovLightGroup7,
	pAovLightGroup8
};

}

node_parameters
{
	AiParameterFLT( "diffuseIntensity", 1.0f );
	AiParameterRGB( "diffuseColor", 1.0f, 1.0f, 1.0f );
	AiParameterENUM( "scatterPhase", kSchlick, phaseFunctionNames );
	AiParameterBOOL( "scatterDual", false );
	AiParameterFLT( "scatterDirection1", 0.0f );
	AiParameterFLT( "scatterDirection2", 0.0f );
	AiParameterFlt( "scatterBlend", 0.5f );
	AiParameterFLT( "diffuseIndirectScale", 1.0f );
	AiParameterVec( "densityNormal", 0.0f, 0.0f, 0.0f );

	AiParameterFLT( "backlightIntensity", 0.0f );
	AiParameterRGB( "backlightColor", 1.0f, 1.0f, 1.0f );
	AiParameterFLT( "backlightIndirectScale", 1.0f );

	AiParameterFLT( "emissionWeight", 0.0f );
	AiParameterRGB( "emissionColor", 1.0f, 1.0f, 1.0f );

	AiParameterBOOL( "aiEnableMatte", false );
	AiParameterRGB( "aiMatteColor", 0.0f, 0.0f, 0.0f );
	AiParameterFlt( "aiMatteColorA", 0.0f );

	AiParameterRGB( "shadowTransmission", 0.0f, 0.0f, 0.0f );
	AiParameterRGB( "opacity", 1.0f, 1.0f, 1.0f );

    AiParameterSTR( "aiTraceSetAll", "" );
    AiParameterSTR( "aiTraceSetShadows", "" );
    AiParameterSTR( "aiTraceSetDiffuse", "" );
    AiParameterSTR( "aiTraceSetBacklight", "" );

    AiParameterInt( "aiDebug", 0 );

	// AOVs
    AiParameterStr( "aiAovDensityNormal", "densityNormal" );
    AiParameterStr( "aiAovDiffuseColor", "diffuseColor" );
    AiParameterStr( "aiAovDirectDiffuse", "directDiffuse" );
    AiParameterStr( "aiAovDirectDiffuseRaw", "directDiffuseRaw" );
    AiParameterStr( "aiAovIndirectDiffuse", "indirectDiffuse" );
    AiParameterStr( "aiAovIndirectDiffuseRaw", "indirectDiffuseRaw" );
    AiParameterStr( "aiAovDirectBacklight", "directBacklight" );
    AiParameterStr( "aiAovDirectBacklightRaw", "directBacklightRaw" );
    AiParameterStr( "aiAovIndirectBacklight", "indirectBacklight" );
    AiParameterStr( "aiAovIndirectBacklightRaw", "indirectBacklightRaw" );
    AiParameterStr( "aiAovEmission", "emission" );
    AiParameterStr( "aiAovLightGroup1", "lightGroup1" );
    AiParameterStr( "aiAovLightGroup2", "lightGroup2" );
    AiParameterStr( "aiAovLightGroup3", "lightGroup3" );
    AiParameterStr( "aiAovLightGroup4", "lightGroup4" );
    AiParameterStr( "aiAovLightGroup5", "lightGroup5" );
    AiParameterStr( "aiAovLightGroup6", "lightGroup6" );
    AiParameterStr( "aiAovLightGroup7", "lightGroup7" );
    AiParameterStr( "aiAovLightGroup8", "lightGroup8" );

    AiMetaDataSetStr(mds, NULL, "maya.name", "bifrostPoint");
}

node_initialize
{
    ShaderData* data = new ShaderData();
	AiNodeSetLocalData( node, data );
}

node_finish
{
	if ( AiNodeGetLocalData( node ) ) {
		ShaderData* data = ( ShaderData* )AiNodeGetLocalData( node );
		AiNodeSetLocalData( node, NULL );
		delete data;
	}
}

node_update
{
	ShaderData* data = (  ShaderData* )AiNodeGetLocalData(  node );

	// set up AOVs
	REGISTER_AOVS

	// check whether the normal parameters are connected or not
	data->densityNormalConnected = AiNodeIsLinked( node, "densityNormal" );

	// Trace sets setup
	data->traceSetAllEnabled = false;
	data->traceSetShadowsEnabled = false;
	data->traceSetDiffuseEnabled = false;
	data->traceSetBacklightEnabled = false;

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

	if ( strlen( params[ pTraceSetBacklight ].STR ) ) {
		std::string tmp( params[ pTraceSetBacklight ].STR );
		data->traceSetBacklightEnabled = true;

		if ( tmp[ 0 ] == '-' ) {
			data->traceSetBacklightInclusive = false;
			data->traceSetBacklight = tmp.substr( 1 );
		} else {
			data->traceSetBacklightInclusive = true;
			data->traceSetBacklight = tmp;
		}
	}

	data->debug = params[ pDebug ].INT;
}

shader_evaluate
{
    ShaderData* data = ( ShaderData* )AiNodeGetLocalData( node );

	// get params
	AtRGB diffuseColor = AiShaderEvalParamRGB( pDiffuseColor ) * AiShaderEvalParamFlt( pDiffuseIntensity );
	float diffuseIndirectScale = AiShaderEvalParamFlt( pDiffuseIndirectScale );
	PhaseFunctions scatterPhase = (PhaseFunctions) AiShaderEvalParamInt( pScatterPhase );
	bool scatterDual = AiShaderEvalParamBool( pScatterDual );
	float scatterDirection1 = AiShaderEvalParamFlt( pScatterDirection1 );
	float scatterDirection2 = AiShaderEvalParamFlt( pScatterDirection2 );

	// clamp blend to 0->1
	float scatterBlend = std::max( 0.0f, std::min( AiShaderEvalParamFlt( pScatterBlend ), 1.0f ) );

	// adjust direction depending on Phase Function
	if ( scatterPhase == kSchlick ) {
		scatterDirection1 = 1.45352f * scatterDirection1 - 0.45352f * scatterDirection1 * scatterDirection1 * scatterDirection1;
		scatterDirection2 = 1.45352f * scatterDirection2 - 0.45352f * scatterDirection2 * scatterDirection2 * scatterDirection2;
	}

	AtRGB backlightColor = AiShaderEvalParamRGB( pBacklightColor ) * AiShaderEvalParamFlt( pBacklightIntensity );
	float backlightIndirectScale = AiShaderEvalParamFlt( pBacklightIndirectScale );

	// balance diffuse and backlight?
	//float t_sum = maxh( diffuseColor ) + maxh( backlightColor );
	//if (t_sum > 1.0f) {
	//	float t_sum_inv = 1.0f / t_sum;
	//	diffuseColor *= t_sum_inv;
	//	backlightColor *= t_sum_inv;
	//}

	AtRGB emissionColor = AiShaderEvalParamRGB( pEmissionColor ) * AiShaderEvalParamFlt( pEmissionWeight );

	AtRGB shadowTransmission = AiShaderEvalParamRGB( pShadowTransmission );
	AtRGB opacity = AiShaderEvalParamRGB( pOpacity );

	// handle shadow ray
	if ( sg->Rt & AI_RAY_SHADOW ) {
		// set the shadows trace set if it's defined
		if ( data->traceSetShadowsEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetShadows.c_str(), data->traceSetShadowsInclusive );
		}

		AtRGB out_opacity = AI_RGB_WHITE - shadowTransmission;
		sg->out_opacity = out_opacity * opacity;

		// unset the shadows trace set
		if ( data->traceSetShadowsEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}

		return;
	}

	// not a shadow ray
	sg->out_opacity = opacity;
	if ( maxh( opacity ) < IMPORTANCE_EPS ) {
		sg->out.RGB = AI_RGB_BLACK;
		sg->out_opacity = AI_RGB_BLACK;
	}

	// early out if we're fully transparent or the object is matte
	if ( AiColorIsZero( sg->out_opacity ) || ( AiShaderGlobalsIsObjectMatte( sg ) && ( sg->Rt & AI_RAY_CAMERA ) ) ) {
		return;
	}

	//
	//
	// ILLUMINATION CALCULATION
	//
	//

	// init output RGB
	sg->out.RGB = AI_RGB_BLACK;

	// set shading normals
	AtVector NOrig = sg->N;
	AtVector NfOrig = sg->Nf;
	AtVector densityNormal = sg->Nf;
	if ( data->densityNormalConnected ) {
		densityNormal = AiV3Normalize( AiShaderEvalParamVec( pDensityNormal ) );
	}

	sg->N = sg->Nf = densityNormal;

	// Set up flags
	bool doDiffuse = true;
	if ( maxh( diffuseColor ) < IMPORTANCE_EPS ) {
		doDiffuse = false;
	}

	bool doBacklight = true;
	if ( maxh( backlightColor ) < IMPORTANCE_EPS  ) {
		doBacklight = false;
	}

	// set phase function methods
	AtBRDFEvalSampleFunc BRDFEvalSample;
	AtBRDFEvalBrdfFunc BRDFEvalBRDF;
	AtBRDFEvalPdfFunc BRDFEvalPDF;
	if ( scatterPhase == kHenyey ) {
		BRDFEvalSample = (AtBRDFEvalSampleFunc) HGVolumeBRDFEvalSample;
		BRDFEvalBRDF = (AtBRDFEvalBrdfFunc) HGVolumeBRDFEvalBRDF;
		BRDFEvalPDF = (AtBRDFEvalPdfFunc) HGVolumeBRDFEvalPDF;
	} else {
		BRDFEvalSample = (AtBRDFEvalSampleFunc) SchlickVolumeBRDFEvalSample;
		BRDFEvalBRDF = (AtBRDFEvalBrdfFunc) SchlickVolumeBRDFEvalBRDF;
		BRDFEvalPDF = (AtBRDFEvalPdfFunc) SchlickVolumeBRDFEvalPDF;
	}

	void* volumeData = NULL;

	// do backlighting
	if ( doBacklight ) {
		sg->fhemi = false;
		flipNormals(sg);

		volumeData = VolumeBRDFSetupData( sg, scatterDirection1, scatterDirection2, scatterDual, scatterBlend );

		AtRGB resultBacklightDirect = AI_RGB_BLACK;
		AtRGB resultBacklightDirectRaw = AI_RGB_BLACK;
		AtRGB backlightDirect;

		// set the backlight trace set if it's defined
		if ( data->traceSetBacklightEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetBacklight.c_str(), data->traceSetBacklightInclusive );
		}

		AiLightsPrepare(sg);
		while ( AiLightsGetSample( sg ) ) {
			if (AiLightGetAffectDiffuse(sg->Lp)) {
				float lightDiffuseWeight = AiLightGetDiffuse( sg->Lp );

				backlightDirect = AiEvaluateLightSample(	sg,
															volumeData,
															BRDFEvalSample,
															BRDFEvalBRDF,
															BRDFEvalPDF);

				backlightDirect *= lightDiffuseWeight;
			}

			resultBacklightDirect += backlightDirect;
		}

		flipNormals(sg);
		AiLightsResetCache(sg);

		// Multiply by the colors
		resultBacklightDirectRaw = resultBacklightDirect;
		AiAOVSetRGB(sg, data->aovs[kDirectBacklightRaw].c_str(), resultBacklightDirectRaw);
		resultBacklightDirect *= backlightColor;
		AiAOVSetRGB(sg, data->aovs[kDirectBacklight].c_str(), resultBacklightDirect);

		assert( AiIsFinite( resultBacklightDirect ) );
		sg->out.RGB += resultBacklightDirect;

		// indirectBacklight
		// ----------------
		if ( maxh( backlightColor ) * backlightIndirectScale > IMPORTANCE_EPS ) {
			AtRGB resultBacklightIndirect = AI_RGB_BLACK;
			AtRGB resultBacklightIndirectRaw = AI_RGB_BLACK;

			resultBacklightIndirectRaw = AiBRDFIntegrate(	sg,
															volumeData,
															BRDFEvalSample,
															BRDFEvalBRDF,
															BRDFEvalPDF,
															AI_RAY_DIFFUSE );

			AiAOVSetRGB(sg, data->aovs[kIndirectBacklightRaw].c_str(), resultBacklightIndirectRaw);
			resultBacklightIndirect = resultBacklightIndirectRaw * backlightColor * backlightIndirectScale;
			AiAOVSetRGB(sg, data->aovs[kIndirectBacklight].c_str(), resultBacklightIndirect);

			assert( AiIsFinite( resultBacklightIndirect ) );
			sg->out.RGB += resultBacklightIndirect;
		}  // end indirect backlight

		// unset the backlight trace set
		if ( data->traceSetBacklightEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}
	}

	// do diffuse
	if ( doDiffuse ) {
		// we need to sample all spherical directions
		sg->fhemi = false;

		// set up brdf data
		if ( volumeData == NULL ) {
			volumeData = VolumeBRDFSetupData( sg, scatterDirection1, scatterDirection2, scatterDual, scatterBlend );
		}

		// set the diffuse trace set if it's defined
		if ( data->traceSetDiffuseEnabled ) {
			AiShaderGlobalsSetTraceSet( sg, data->traceSetDiffuse.c_str(), data->traceSetDiffuseInclusive );
		} else {
			// diffuse set is not defined! Is global defined?
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			}
		}

		// calc direct diffuse
		AtRGB resultDiffuseDirect = AI_RGB_BLACK;
		AtRGB resultDiffuseDirectRaw = AI_RGB_BLACK;

		AiLightsPrepare( sg );
		AtRGB diffuseDirect = AI_RGB_BLACK;

		while ( AiLightsGetSample( sg ) ) {
			if ( AiLightGetAffectDiffuse( sg->Lp ) ) {
				// get diffuse Weight multiplier
				float lightDiffuseWeight = AiLightGetDiffuse( sg->Lp );

				diffuseDirect = AiEvaluateLightSample(	sg,
														volumeData,
														BRDFEvalSample,
														BRDFEvalBRDF,
														BRDFEvalPDF );
				diffuseDirect *= lightDiffuseWeight;

				resultDiffuseDirectRaw += diffuseDirect;
			}
		}

		AiLightsResetCache(sg);

		// Multiply by the colors
		AiAOVSetRGB(sg, data->aovs[kDirectDiffuseRaw].c_str(), resultDiffuseDirectRaw);
		resultDiffuseDirect = resultDiffuseDirectRaw * diffuseColor;
		AiAOVSetRGB(sg, data->aovs[kDirectDiffuse].c_str(), resultDiffuseDirect);

		assert( AiIsFinite( resultDiffuseDirect ) );
		sg->out.RGB += resultDiffuseDirect;

		// indirectDiffuse
		// ----------------
		sg->fhemi = true;
		AtRGB resultDiffuseIndirect = AI_RGB_BLACK;
		AtRGB resultDiffuseIndirectRaw = AI_RGB_BLACK;
		if ( maxh( diffuseColor ) * diffuseIndirectScale > IMPORTANCE_EPS ) {
			resultDiffuseIndirectRaw = AiBRDFIntegrate( sg,
														volumeData,
														BRDFEvalSample,
														BRDFEvalBRDF,
														BRDFEvalPDF,
														AI_RAY_DIFFUSE,
														AI_RGB_WHITE );

			AiAOVSetRGB(sg, data->aovs[kIndirectDiffuseRaw].c_str(), resultDiffuseIndirectRaw);
			resultDiffuseIndirect = resultDiffuseIndirectRaw * diffuseColor * diffuseIndirectScale;
			AiAOVSetRGB(sg, data->aovs[kIndirectDiffuse].c_str(), resultDiffuseIndirect);

			assert( AiIsFinite( resultDiffuseIndirect ) );
			sg->out.RGB += resultDiffuseIndirect;
		}  // end indirect diffuse

		// reset trace set
		if ( data->traceSetDiffuseEnabled ) {
			// if we defined a global trace set, re-set this, otherwise, unset
			if ( data->traceSetAllEnabled ) {
				AiShaderGlobalsSetTraceSet( sg, data->traceSetAll.c_str(), data->traceSetAllInclusive );
			} else {
				AiShaderGlobalsUnsetTraceSet( sg );
			}
		}
	} // end diffuse check

	// Emission
	AtColor resultEmission = emissionColor;
	assert( AiIsFinite( resultEmission ) );
	AiAOVSetRGB(sg, data->aovs[kEmission].c_str(), resultEmission);

	// set extra AOVs
	AtRGB tmp;
	tmp.r = densityNormal.x;
	tmp.g = densityNormal.y;
	tmp.b = densityNormal.z;
	AiAOVSetRGB(sg, data->aovs[kDensityNormal].c_str(), tmp);

	AiAOVSetRGB(sg, data->aovs[kDiffuseColor].c_str(), diffuseColor);

	sg->out.RGB += resultEmission;
	assert( AiIsFinite( sg->out.RGB ) );

	//sg->out.RGB = AiColor( densityNormal.x, densityNormal.y, densityNormal.z );

	// revert back
	sg->Nf = NfOrig;
	sg->N = NOrig;
}
