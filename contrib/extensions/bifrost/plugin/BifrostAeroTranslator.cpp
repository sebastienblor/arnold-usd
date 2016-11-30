#include <BifrostAeroTranslator.h>
#include <TranslatorUtils.h>

#define MICROFACET_GGX 0
#define BECKMANN 1

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
    PROCESS_PARAM("anisotropy", AI_TYPE_FLOAT);
    PROCESS_PARAM("attenuationSource", AI_TYPE_INT);
    PROCESS_PARAM("attenuation", AI_TYPE_RGB);
    PROCESS_PARAM("attenuationChannel", AI_TYPE_STRING);
    PROCESS_PARAM("attenuationColor", AI_TYPE_RGB);
    PROCESS_PARAM("attenuationIntensity", AI_TYPE_FLOAT);
    PROCESS_PARAM("attenuationMode", AI_TYPE_INT);
    PROCESS_PARAM("emissionSource", AI_TYPE_INT);
    PROCESS_PARAM("emission", AI_TYPE_RGB);
    PROCESS_PARAM("emissionChannel", AI_TYPE_STRING);
    PROCESS_PARAM("emissionColor", AI_TYPE_RGB);
    PROCESS_PARAM("emissionIntensity", AI_TYPE_FLOAT);
    PROCESS_PARAM("blackBodyOn", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("blackBodyIntensity", AI_TYPE_FLOAT);
    PROCESS_PARAM("blackBodyExposure", AI_TYPE_FLOAT);
    PROCESS_PARAM("blackBodyPhysicalMix", AI_TYPE_FLOAT);
    PROCESS_PARAM("blackBodyOutputStand", AI_TYPE_ENUM);
    PROCESS_PARAM("positionOffset", AI_TYPE_VECTOR);
    PROCESS_PARAM("interpolation", AI_TYPE_INT);
}

void BifrostAeroTranslator::NodeInitializer(CAbTranslator context){}

