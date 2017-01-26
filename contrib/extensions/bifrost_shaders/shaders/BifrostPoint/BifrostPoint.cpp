#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>

#include <ai.h>

#include "PointDefs.h"
#include "PointBRDF.h"
#include "PointUtils.h"

#ifdef MSVC
#define CRTSECURENOWARNINGS 1
#endif

AI_SHADER_NODE_EXPORT_METHODS(  BifrostPointMtd  )

namespace{

struct ShaderData
{
	ShaderData() {}

	bool densityNormalConnected;

	// AOV names
	std::vector<std::string> aovs;
	std::vector<std::string> aovs_rgba;

	int debug;
};
}

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

	pBacklightIntensity,
	pBacklightColor,

	// emission
	pEmissionIntensity,
	pEmissionColor,

	pShadowTransmission,
	pOpacity,

	pDensityNormal,

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

node_parameters
{
	AiParameterFLT( "diffuseIntensity", 1.0f );
	AiParameterRGB( "diffuseColor", 1.0f, 1.0f, 1.0f );
	AiParameterENUM( "scatterPhase", kHenyey, phaseFunctionNames );
	AiParameterBOOL( "scatterDual", false );
	AiParameterFLT( "scatterDirection1", 0.0f );
	AiParameterFLT( "scatterDirection2", 0.0f );
	AiParameterFlt( "scatterBlend", 0.5f );
	AiParameterFLT( "diffuseIndirectScale", 1.0f );

	AiParameterFLT( "backlightIntensity", 0.0f );
	AiParameterRGB( "backlightColor", 1.0f, 1.0f, 1.0f );

	AiParameterFLT( "emissionIntensity", 0.0f );
	AiParameterRGB( "emissionColor", 1.0f, 1.0f, 1.0f );

	AiParameterRGB( "shadowTransmission", 0.0f, 0.0f, 0.0f );
	AiParameterRGB( "opacity", 1.0f, 1.0f, 1.0f );

	AiParameterVec( "densityNormal", 0.0f, 0.0f, 0.0f );

	AiParameterInt( "debug", 0 );

	// AOVs
	AiParameterStr( "aovDensityNormal", "densityNormal" );
	AiParameterStr( "aovDiffuseColor", "diffuseColor" );
	AiParameterStr( "aovDirectDiffuse", "directDiffuse" );
	AiParameterStr( "aovDirectDiffuseRaw", "directDiffuseRaw" );
	AiParameterStr( "aovIndirectDiffuse", "indirectDiffuse" );
	AiParameterStr( "aovIndirectDiffuseRaw", "indirectDiffuseRaw" );
	AiParameterStr( "aovDirectBacklight", "directBacklight" );
	AiParameterStr( "aovDirectBacklightRaw", "directBacklightRaw" );
	AiParameterStr( "aovEmission", "emission" );
	AiParameterStr( "aovLightGroup1", "lightGroup1" );
	AiParameterStr( "aovLightGroup2", "lightGroup2" );
	AiParameterStr( "aovLightGroup3", "lightGroup3" );
	AiParameterStr( "aovLightGroup4", "lightGroup4" );
	AiParameterStr( "aovLightGroup5", "lightGroup5" );
	AiParameterStr( "aovLightGroup6", "lightGroup6" );
	AiParameterStr( "aovLightGroup7", "lightGroup7" );
	AiParameterStr( "aovLightGroup8", "lightGroup8" );

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

	AtRGB emissionColor = AiShaderEvalParamRGB( pEmissionColor ) * AiShaderEvalParamFlt( pEmissionIntensity );

	AtRGB shadowTransmission = AiShaderEvalParamRGB( pShadowTransmission );
	AtRGB opacity = AiShaderEvalParamRGB( pOpacity );

	// handle shadow ray
	if ( sg->Rt & AI_RAY_SHADOW ) {
		AtRGB out_opacity = AI_RGB_WHITE - shadowTransmission;
		sg->out_opacity = out_opacity * opacity;

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

	// if backlight is on we need to evaluate all directions
	if ( doBacklight ) {
		sg->fhemi = false;
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

	// do diffuse
	if ( doDiffuse ) {
		// set up brdf data
		volumeData = VolumeBRDFSetupData( sg, scatterDirection1, scatterDirection2, scatterDual, scatterBlend );

		// calc direct diffuse
		AtRGB resultDiffuseDirect = AI_RGB_BLACK;
		AtRGB resultDiffuseDirectRaw = AI_RGB_BLACK;
		AtRGB resultBacklightDirect = AI_RGB_BLACK;
		AtRGB resultBacklightDirectRaw = AI_RGB_BLACK;

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

				if ( AiV3Dot(sg->Nf, sg->Ld) > 0.0f ) {
					resultDiffuseDirectRaw += diffuseDirect;
					resultDiffuseDirect += diffuseDirect * diffuseColor;
				} else {
					resultBacklightDirectRaw += diffuseDirect;
					resultBacklightDirect += diffuseDirect * backlightColor;
				}
			}
		}

		AiLightsResetCache(sg);

		// Multiply by the colors
		AiAOVSetRGB(sg, data->aovs[kDirectDiffuseRaw].c_str(), resultDiffuseDirectRaw);
		AiAOVSetRGB(sg, data->aovs[kDirectDiffuse].c_str(), resultDiffuseDirect);
		AiAOVSetRGB(sg, data->aovs[kDirectBacklightRaw].c_str(), resultBacklightDirectRaw);
		AiAOVSetRGB(sg, data->aovs[kDirectBacklight].c_str(), resultBacklightDirect);

		AtRGB finalDiffuse = resultDiffuseDirect + resultBacklightDirect;
		assert( AiIsFinite( finalDiffuse ) );
		sg->out.RGB += finalDiffuse;

		// indirectDiffuse
		// ----------------
		if ( maxh( diffuseColor ) * diffuseIndirectScale > IMPORTANCE_EPS ) {
			AtRGB resultDiffuseIndirect = AI_RGB_BLACK;
			AtRGB resultDiffuseIndirectRaw = AI_RGB_BLACK;

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
	} // end diffuse check

	// Emission
	AtColor resultEmission = emissionColor;
	assert( AiIsFinite( resultEmission ) );
	AiAOVSetRGB(sg, data->aovs[kEmission].c_str(), resultEmission);
	sg->out.RGB += resultEmission;
	assert( AiIsFinite( sg->out.RGB ) );

	// set extra AOVs
	AtRGB tmp;
	tmp.r = densityNormal.x;
	tmp.g = densityNormal.y;
	tmp.b = densityNormal.z;
	AiAOVSetRGB(sg, data->aovs[kDensityNormal].c_str(), tmp);

	AiAOVSetRGB(sg, data->aovs[kDiffuseColor].c_str(), diffuseColor);

	//sg->out.RGB = AiColor( densityNormal.x, densityNormal.y, densityNormal.z );

	// revert back
	sg->Nf = NfOrig;
	sg->N = NOrig;
}
