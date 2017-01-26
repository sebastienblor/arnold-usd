#include <vector>

#include <maya/MTypes.h> 

#include "BifrostMayaShadersTranslator.h"
#include "TranslatorUtils.h"

//
//
// BIFROST HYDRO TRANSLATOR

AtNode* BifrostHydroTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("bifrostHydro");
}

void BifrostHydroTranslator::Export(AtNode* instance)
{
	// *** BIFROST HYDRO ATTRIBUTES ***

	// diffuse
	PROCESS_PARAM("diffuseIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("diffuseColor", AI_TYPE_RGB);
	PROCESS_PARAM("diffuseRoughness", AI_TYPE_FLOAT);
	PROCESS_PARAM("diffuseIndirectScale", AI_TYPE_FLOAT);

	// emission
	PROCESS_PARAM("emissionIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("emissionColor", AI_TYPE_RGB);

	// specular
	PROCESS_PARAM("ior", AI_TYPE_FLOAT);
	PROCESS_PARAM("specularRoughness", AI_TYPE_FLOAT);
	PROCESS_PARAM("reflectionIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("reflectionColor", AI_TYPE_RGB);
	PROCESS_PARAM("refractionIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("refractionColor", AI_TYPE_RGB);
	PROCESS_PARAM("specularIndirectScale", AI_TYPE_FLOAT);

	// scatterring
	PROCESS_PARAM("interiorType", AI_TYPE_ENUM);
	PROCESS_PARAM("densityScale", AI_TYPE_FLOAT);
	PROCESS_PARAM("scatterDirection", AI_TYPE_FLOAT);
	PROCESS_PARAM("attenuationColor", AI_TYPE_RGB);
	PROCESS_PARAM("attenuationWeight", AI_TYPE_FLOAT);
	PROCESS_PARAM("transmissionColor", AI_TYPE_RGB);
	PROCESS_PARAM("transmissionRadius", AI_TYPE_RGB);

	PROCESS_PARAM("shadowTransmission", AI_TYPE_RGB);

	// *** EXTENSION ATTRIBUTES ***

    PROCESS_PARAM("aiRRDepth", AI_TYPE_INT);
    PROCESS_PARAM("aiRoughnessDepthScale", AI_TYPE_FLOAT);
}

void BifrostHydroTranslator::NodeInitializer(CAbTranslator context)
{
	CExtensionAttrHelper helper(context.maya, "standard");
	CAttrData data;

	ADD_INT_MIN("aiRRDepth", "airrtd", 1, 0, 10);
	ADD_FLOAT("aiRoughnessDepthScale", "airghdscl", 1, 0, 2);
}

//
//
// BIFROST POINT TRANSLATOR

AtNode* BifrostPointTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("bifrostPoint");
}

void BifrostPointTranslator::Export(AtNode* instance)
{
	// *** BIFROST POINT ATTRIBUTES ***

	PROCESS_PARAM("diffuseIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("diffuseColor", AI_TYPE_RGB);
	PROCESS_PARAM("scatterPhase", AI_TYPE_INT);
	PROCESS_PARAM("scatterDual", AI_TYPE_BOOLEAN);
	PROCESS_PARAM("scatterDirection1", AI_TYPE_FLOAT);
	PROCESS_PARAM("scatterDirection2", AI_TYPE_FLOAT);
	PROCESS_PARAM("scatterBlend", AI_TYPE_FLOAT);
	PROCESS_PARAM("diffuseIndirectScale", AI_TYPE_FLOAT);

	PROCESS_PARAM("backlightIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("backlightColor", AI_TYPE_RGB);

	PROCESS_PARAM("emissionIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("emissionColor", AI_TYPE_RGB);

	PROCESS_PARAM("shadowTransmission", AI_TYPE_RGB);
	PROCESS_PARAM("opacity", AI_TYPE_RGB);

	PROCESS_PARAM("densityNormal", AI_TYPE_VECTOR);
}

void BifrostPointTranslator::NodeInitializer(CAbTranslator context)
{
    // nothing here
}

//
//
// BIFROST AERO TRANSLATOR

AtNode* BifrostAeroTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("bifrostAero");
}

void BifrostAeroTranslator::Export(AtNode* instance)
{
	// *** BIFROST AERO ATTRIBUTES ***

	PROCESS_PARAM("scatteringSource", AI_TYPE_INT);
	PROCESS_PARAM("scattering", AI_TYPE_RGB);
	PROCESS_PARAM("scatteringChannel", AI_TYPE_STRING);
	PROCESS_PARAM("scatteringColor", AI_TYPE_RGB);
	PROCESS_PARAM("scatteringIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("scatteringThreshold", AI_TYPE_FLOAT);
	PROCESS_PARAM("scatteringDirection", AI_TYPE_FLOAT);
	PROCESS_PARAM("attenuationSource", AI_TYPE_INT);
	PROCESS_PARAM("attenuation", AI_TYPE_RGB);
	PROCESS_PARAM("attenuationChannel", AI_TYPE_STRING);
	PROCESS_PARAM("attenuationColor", AI_TYPE_RGB);
	PROCESS_PARAM("attenuationIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("attenuationThreshold", AI_TYPE_FLOAT);
	PROCESS_PARAM("attenuationMode", AI_TYPE_INT);
	PROCESS_PARAM("emissionSource", AI_TYPE_INT);
	PROCESS_PARAM("emission", AI_TYPE_RGB);
	PROCESS_PARAM("emissionChannel", AI_TYPE_STRING);
	PROCESS_PARAM("emissionColor", AI_TYPE_RGB);
	PROCESS_PARAM("emissionIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("emissionThreshold", AI_TYPE_FLOAT);
	PROCESS_PARAM("blackBodyOn", AI_TYPE_BOOLEAN);
	PROCESS_PARAM("blackBodyIntensity", AI_TYPE_FLOAT);
	PROCESS_PARAM("blackBodyExposure", AI_TYPE_FLOAT);
	PROCESS_PARAM("blackBodyPhysicalMix", AI_TYPE_FLOAT);
	PROCESS_PARAM("blackBodyOutputStandard", AI_TYPE_ENUM);
	PROCESS_PARAM("positionOffset", AI_TYPE_VECTOR);
}

void BifrostAeroTranslator::NodeInitializer(CAbTranslator context)
{
	// nothing here
}
