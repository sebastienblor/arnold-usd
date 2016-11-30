#include <ai.h>

#include <string>
#include <cassert>
#include <cstring>

#include "BlackBody.h"

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(BifrostAeroMethod);

namespace {

	enum BifrostAeroAIParams
	{
		pScatteringSource,
		pScattering,
		pScatteringChannel,
		pScatteringColor,
		pScatteringIntensity,
		pAnisotropy,
		pAttenuationSource,
		pAttenuation,
		pAttenuationChannel,
		pAttenuationColor,
		pAttenuationIntensity,
		pAttenuationMode,
		pEmissionSource,
		pEmission,
		pEmissionChannel,
		pEmissionColor,
		pEmissionIntensity,
		pBlackBodyOn,
		pBlackBodyIntensity,
		pBlackBodyExposure,
		pBlackBodyPhysicalMix,
		pBlackBodyOutputStandard,
		pPositionOffset,
		pInterpolation,
	};

	static const char* scatteringSourceLabels[] = { "From Input", "From Channel", NULL };
	static const char* attenuationSourceLabels[] = { "From Input", "From Channel", "From Scattering", NULL };
	static const char* emissionSourceLabels[] = { "From Input", "From Channel", NULL };
	static const char* attenuationModeLabels[] = { "absorption", "extinction", NULL };
	static const char* interpolationLabels[] = { "linear", "bspline", NULL };
	static const char* outputStandardLabels[] = { "ACES CG", "Rec.709 D65", "sRGB D65", NULL };

	enum InputSource
	{
		SOURCE_INPUT  = 0,
		SOURCE_CHANNEL    = 1,
		SOURCE_SCATTERING = 2,
	};

	enum AttenuationMode
	{
		ATTENUATION_MODE_ABSORPTION = 0,
		ATTENUATION_MODE_EXTINCTION = 1,
	};

	enum InputFrom
	{
		INPUT_FROM_NONE,
		INPUT_FROM_EVALUATE,
		INPUT_FROM_CACHE,
		INPUT_FROM_CHANNEL,
		INPUT_FROM_SCATTERING,
	};

	struct ShaderData
	{
		bool scatteringIsLinked;
		bool scatteringColorIsLinked;
		bool scatteringIntensityIsLinked;
		bool anisotropyIsLinked;
		bool attenuationIsLinked;
		bool attenuationColorIsLinked;
		bool attenuationIntensityIsLinked;
		bool emissionIsLinked;
		bool emissionColorIsLinked;
		bool emissionIntensityIsLinked;
		bool positionOffsetIsLinked;

		AtRGB scattering;
		std::string scatteringChannel;
		AtRGB scatteringColor;
		float scatteringIntensity;
		float anisotropy;
		int attenuationMode;
		AtRGB attenuation;
		AtRGB attenuationColor;
		std::string attenuationChannel;
		float attenuationIntensity;
		AtRGB emission;
		std::string emissionChannel;
		AtRGB emissionColor;
		float emissionIntensity;
		bool blackBodyOn;
		float blackBodyIntensity;
		float blackBodyExposure;
		float blackBodyPhysicalMix;
		OutputStandard blackBodyOutputStandard;
		AtPoint positionOffset;
		int interpolation;

		int scatteringSource;
		int attenuationSource;
		int emissionSource;

		InputFrom scatteringFrom;
		InputFrom attenuationFrom;
		InputFrom emissionFrom;
		InputFrom positionOffsetFrom;
	};

} // namespace

node_parameters
{
	AiParameterEnum("scatteringSource", SOURCE_CHANNEL, scatteringSourceLabels);
	AiParameterRGB("scattering", 1.0f, 1.0f, 1.0f);
	AiParameterStr("scatteringChannel", "smoke");
	AiParameterRGB("scatteringColor", 1.0f, 1.0f, 1.0f);
	AiParameterFlt("scatteringIntensity", 1.0f);
	AiParameterFlt("anisotropy", 0);
	AiParameterEnum("attenuationSource", SOURCE_SCATTERING, attenuationSourceLabels);
	AiParameterRGB("attenuation", 1.0f, 1.0f, 1.0f);
	AiParameterStr("attenuationChannel", "");
	AiParameterRGB("attenuationColor", 1.0f, 1.0f, 1.0f);
	AiParameterFlt("attenuationIntensity", 1.0f);
	AiParameterEnum("attenuationMode", ATTENUATION_MODE_ABSORPTION, attenuationModeLabels);
	AiParameterEnum("emissionSource", SOURCE_CHANNEL, emissionSourceLabels);
	AiParameterRGB("emission", 0.0f, 0.0f, 0.0f);
	AiParameterStr("emissionChannel", "temperature");
	AiParameterRGB("emissionColor", 1.0f, 1.0f, 1.0f);
	AiParameterFlt("emissionIntensity", 0.0f);
	AiParameterBool("blackBodyOn", false);
	AiParameterFLT("blackBodyIntensity", 1.0f);
	AiParameterFLT("blackBodyExposure", -15.0f);
	AiParameterFLT("blackBodyPhysicalMix", 1.0f);
	AiParameterEnum("blackBodyOutputStandard", OUT_ACESCG, outputStandardLabels);
	AiParameterVec("positionOffset", 0, 0, 0);
	AiParameterEnum("interpolation", AI_VOLUME_INTERP_TRILINEAR, interpolationLabels);

	AiMetaDataSetBool(mds, "scatteringSource"         , "linkable", false);
	AiMetaDataSetBool(mds, "attenuationSource"        , "linkable", false);
	AiMetaDataSetBool(mds, "emissionSource"           , "linkable", false);
	AiMetaDataSetBool(mds, "interpolation"             , "linkable", false);
	AiMetaDataSetBool(mds, "scatteringChannel"        , "linkable", false);
	AiMetaDataSetBool(mds, "attenuationChannel"       , "linkable", false);
	AiMetaDataSetBool(mds, "emissionChannel"          , "linkable", false);
	AiMetaDataSetBool(mds, "attenuationMode"          , "linkable", false);

	AiMetaDataSetBool(mds, "scattering"       , "always_linear", true);
	AiMetaDataSetBool(mds, "scatteringColor" , "always_linear", true);
	AiMetaDataSetBool(mds, "attenuation"      , "always_linear", true);
	AiMetaDataSetBool(mds, "attenuationColor", "always_linear", true);
	AiMetaDataSetBool(mds, "emission"         , "always_linear", true);
	AiMetaDataSetBool(mds, "emissionColor"   , "always_linear", true);

    AiMetaDataSetStr(mds, NULL, "maya.name", "bifrostAero");
}

node_initialize
{
	AiNodeSetLocalData(node, new ShaderData);
}

node_update
{
	ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

	// position offset
	data->positionOffset = AiNodeGetVec(node, "positionOffset");

	if (AiNodeIsLinked(node, "positionOffset"))
		data->positionOffsetFrom = INPUT_FROM_EVALUATE;
	else if (data->positionOffset != AI_V3_ZERO)
		data->positionOffsetFrom = INPUT_FROM_CACHE;
	else
		data->positionOffsetFrom = INPUT_FROM_NONE;

	// get linked status
	data->scatteringIsLinked            = AiNodeIsLinked(node, "scattering");
	data->scatteringColorIsLinked      = AiNodeIsLinked(node, "scatteringColor");
	data->scatteringIntensityIsLinked  = AiNodeIsLinked(node, "scatteringIntensity");
	data->anisotropyIsLinked            = AiNodeIsLinked(node, "anisotropy");

	data->attenuationIsLinked           = AiNodeIsLinked(node, "attenuation");
	data->attenuationColorIsLinked     = AiNodeIsLinked(node, "attenuationColor");
	data->attenuationIntensityIsLinked = AiNodeIsLinked(node, "attenuationIntensity");

	data->emissionIsLinked              = AiNodeIsLinked(node, "emission");
	data->emissionColorIsLinked        = AiNodeIsLinked(node, "emissionColor");
	data->emissionIntensityIsLinked    = AiNodeIsLinked(node, "emissionIntensity");

	// cache parameter values
	data->interpolation = AiNodeGetInt(node, "interpolation");

	data->scatteringSource			= AiNodeGetInt(node, "scatteringSource");
	data->scattering				= AiNodeGetRGB(node, "scattering");
	data->scatteringChannel			= AiNodeGetStr(node, "scatteringChannel");
	data->scatteringColor			= AiNodeGetRGB(node, "scatteringColor");
	data->scatteringIntensity		= AiNodeGetFlt(node, "scatteringIntensity");
	data->anisotropy				= AiNodeGetFlt(node, "anisotropy");

	data->attenuationSource			= AiNodeGetInt(node, "attenuationSource");
	data->attenuation				= AiNodeGetRGB(node, "attenuation");
	data->attenuationChannel		= AiNodeGetStr(node, "attenuationChannel");
	data->attenuationColor			= AiNodeGetRGB(node, "attenuationColor");
	data->attenuationIntensity		= AiNodeGetFlt(node, "attenuationIntensity");
	data->attenuationMode			= AiNodeGetInt(node, "attenuationMode");

	data->emissionSource			= AiNodeGetInt(node, "emissionSource");
	data->emission					= AiNodeGetRGB(node, "emission");
	data->emissionChannel			= AiNodeGetStr(node, "emissionChannel");
	data->emissionColor				= AiNodeGetRGB(node, "emissionColor");
	data->emissionIntensity			= AiNodeGetFlt(node, "emissionIntensity");
	data->blackBodyOn				= AiNodeGetBool( node, "blackBodyOn" );
	data->blackBodyIntensity		= AiNodeGetFlt( node, "blackBodyIntensity" );
	data->blackBodyExposure			= AiNodeGetFlt( node, "blackBodyExposure" );
	data->blackBodyPhysicalMix		= AiNodeGetFlt( node, "blackBodyPhysicalMix" );
	data->blackBodyOutputStandard	= (OutputStandard) AiNodeGetInt( node, "blackBodyOutputStandard" );

	switch (data->scatteringSource) {
	case SOURCE_INPUT:
		data->scatteringFrom = data->scatteringIsLinked ? INPUT_FROM_EVALUATE : INPUT_FROM_CACHE;
		break;
	case SOURCE_CHANNEL:
		data->scatteringFrom = INPUT_FROM_CHANNEL;
		break;
	default:
		assert("invalid value for data->scatteringSource"); break;
		break;
	}

	switch (data->attenuationSource) {
	case SOURCE_INPUT:
		if (data->attenuationIsLinked) {
			if (AiNodeGetLink(node, "attenuation") == AiNodeGetLink(node, "scattering")) {
				data->attenuationFrom = INPUT_FROM_SCATTERING;
			} else {
				data->attenuationFrom = INPUT_FROM_EVALUATE;
			}
		} else {
			data->attenuationFrom = INPUT_FROM_CACHE;
		}
		break;
	case SOURCE_CHANNEL:
		if (data->scatteringChannel == data->attenuationChannel) {
			data->attenuationFrom = INPUT_FROM_SCATTERING;
		} else {
			data->attenuationFrom = INPUT_FROM_CHANNEL;
		}
		break;
	case SOURCE_SCATTERING:
		data->attenuationFrom = INPUT_FROM_SCATTERING;
		break;
	default:
		assert("invalid value for data->attenuationSource"); break;
		break;
	}

	switch (data->emissionSource) {
	case SOURCE_INPUT:
		data->emissionFrom = data->emissionIsLinked ? INPUT_FROM_EVALUATE : INPUT_FROM_CACHE;
		break;
	case SOURCE_CHANNEL:
		data->emissionFrom = INPUT_FROM_CHANNEL;
		break;
	default:
		assert("invalid value for data->emissionSource"); break;
		break;
	}

	// detect constant zero values for color and intensity
	if ((!data->scatteringIntensityIsLinked && data->scatteringIntensity == 0.0f) ||
		(!data->scatteringColorIsLinked && AiColorEqual(data->scatteringColor, AI_RGB_BLACK)))
	{
		data->scatteringFrom = INPUT_FROM_CACHE;
		data->scattering = AI_RGB_BLACK;
	}

	if ((!data->attenuationIntensityIsLinked && data->attenuationIntensity == 0.0f) ||
		(!data->attenuationColorIsLinked && AiColorEqual(data->attenuationColor, AI_RGB_BLACK)))
	{
		data->attenuationFrom = INPUT_FROM_CACHE;
		data->attenuation = AI_RGB_BLACK;
	}

	if ((!data->emissionIntensityIsLinked && data->emissionIntensity == 0.0f) ||
		(!data->emissionColorIsLinked && AiColorEqual(data->emissionColor, AI_RGB_BLACK)))
	{
		data->emissionFrom = INPUT_FROM_NONE;
		data->emission = AI_RGB_BLACK;
	}
}

node_finish
{
	ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
	delete data;
}

shader_evaluate
{
    ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

	// sampling position offset
	AtPoint Po_orig;

	switch (data->positionOffsetFrom) {
	case INPUT_FROM_EVALUATE:
		Po_orig = sg->Po;
		sg->Po += AiShaderEvalParamVec(pPositionOffset);
		break;
	case INPUT_FROM_CACHE:
		Po_orig = sg->Po;
		sg->Po += data->positionOffset;
		break;
	default:
		Po_orig = AI_V3_ZERO;
		break;
	}

	// the values storing the result of AiVolumeSampleRGB() need to be zeroed
	// or NaNs will occur in optimized builds (htoa#374)
	AtColor scattering  = AI_RGB_BLACK;
	AtColor attenuation = AI_RGB_BLACK;

	// scattering
	if (!(sg->Rt & AI_RAY_SHADOW) || (data->attenuationFrom == INPUT_FROM_SCATTERING) || (data->attenuationMode == ATTENUATION_MODE_ABSORPTION)) {
		switch (data->scatteringFrom) {
		case INPUT_FROM_CHANNEL:
			AiVolumeSampleRGB(data->scatteringChannel.c_str(), data->interpolation, &scattering);
			break;
		case INPUT_FROM_EVALUATE:
			scattering = AiShaderEvalParamRGB(pScattering);
			break;
		case INPUT_FROM_CACHE:
			scattering = data->scattering;
			break;
		default:
			assert("invalid value for data->scatteringFrom");
			break;
		}

		if (!(sg->Rt & AI_RAY_SHADOW) || (data->attenuationMode == ATTENUATION_MODE_ABSORPTION)) {
			// color, intensity, anisotropy and clipping
			const AtRGB scatteringColor		= data->scatteringColorIsLinked		? AiShaderEvalParamRGB(pScatteringColor)		: data->scatteringColor;
			const float scatteringIntensity	= data->scatteringIntensityIsLinked	? AiShaderEvalParamFlt(pScatteringIntensity)	: data->scatteringIntensity;
			const float anisotropy			= data->anisotropyIsLinked			? AiShaderEvalParamFlt(pAnisotropy)				: data->anisotropy;

			AtRGB scattering_result = scattering * scatteringColor * scatteringIntensity;
			AiColorClipToZero(scattering_result);

			// update volume shader globals
			AiShaderGlobalsSetVolumeScattering(sg, scattering_result, anisotropy);
		}
	}

	// attenuation
	switch (data->attenuationFrom) {
	case INPUT_FROM_CHANNEL:
		AiVolumeSampleRGB(data->attenuationChannel.c_str(), data->interpolation, &attenuation);
		break;
	case INPUT_FROM_EVALUATE:
		attenuation = AiShaderEvalParamRGB(pAttenuation);
		break;
	case INPUT_FROM_CACHE:
		attenuation = data->attenuation;
		break;
	case INPUT_FROM_SCATTERING:
		attenuation = scattering;
		break;
	default:
		assert("invalid value for data->attenuationFrom");
		break;
	}

	// color, intensity and clipping
	const AtRGB attenuationColor     = data->attenuationColorIsLinked     ? AiShaderEvalParamRGB(pAttenuationColor)     : data->attenuationColor;
	const float attenuationIntensity = data->attenuationIntensityIsLinked ? AiShaderEvalParamFlt(pAttenuationIntensity) : data->attenuationIntensity;
	attenuation *= attenuationColor * attenuationIntensity;
	AiColorClipToZero(attenuation);

	// update volume shader globals
	switch (data->attenuationMode) {
	case ATTENUATION_MODE_ABSORPTION:
		AiShaderGlobalsSetVolumeAbsorption(sg, attenuation);
		break;
	case ATTENUATION_MODE_EXTINCTION:
		AiShaderGlobalsSetVolumeAttenuation(sg, attenuation);
		break;
	}

	// emission - most probably from temperature
	if (!(sg->Rt & AI_RAY_SHADOW) && (data->emissionFrom != INPUT_FROM_NONE)) {
		AtColor emission = AI_RGB_BLACK;
		switch (data->emissionFrom) {
		case INPUT_FROM_CHANNEL:
			AiVolumeSampleRGB(data->emissionChannel.c_str(), data->interpolation, &emission);
			break;
		case INPUT_FROM_EVALUATE:
			emission = AiShaderEvalParamRGB(pEmission);
			break;
		case INPUT_FROM_CACHE:
			emission = data->emission;
			break;
		default:
			assert("invalid value for data->emissionFrom");
			break;
		}

		// color, intensity and clipping
		const AtRGB emissionColor		= data->emissionColorIsLinked     ? AiShaderEvalParamRGB(pEmissionColor)     : data->emissionColor;
		const float emissionIntensity	= data->emissionIntensityIsLinked ? AiShaderEvalParamFlt(pEmissionIntensity) : data->emissionIntensity;
		emission *= emissionColor * emissionIntensity;
		AiColorClipToZero(emission);
		
		if ( AiShaderEvalParamBool( pBlackBodyOn ) ) {
			float blackBodyExposure = AiShaderEvalParamFlt( pBlackBodyExposure );
			float blackBodyPhysicalMix = AiShaderEvalParamFlt( pBlackBodyPhysicalMix );
			float blackBodyIntensity = AiShaderEvalParamFlt( pBlackBodyIntensity );
			OutputStandard blackBodyOutputStandard = (OutputStandard)  AiShaderEvalParamInt( pBlackBodyOutputStandard );
			// if blackbody is on we assume the emission values correspond to tempreature values
			// and then depending on the tempreature values we output a color
			float temperature = emission[ 0 ];

            emission = calcBlackBodyColor( temperature, blackBodyPhysicalMix, blackBodyExposure, blackBodyOutputStandard ) * blackBodyIntensity;
		}

		// update volume shader globals
		if (!AiColorIsZero(emission)) {
			AiShaderGlobalsSetVolumeEmission(sg, emission);
		}
	}

	// restore sampling position
	if (data->positionOffsetFrom != INPUT_FROM_NONE) {
		sg->Po = Po_orig;
	}
}
