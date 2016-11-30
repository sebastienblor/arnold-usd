#include <BifrostHydroTranslator.h>
#include <TranslatorUtils.h>
#include <vector>

#define MICROFACET_GGX 0
#define BECKMANN 1

AtNode* BifrostHydroTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("bifrostHydro");
}

void BifrostHydroTranslator::Export(AtNode* instance)
{
    // *** BIFROST HYDRO ATTRIBUTES ***

    // diffuse
    PROCESS_PARAM("diffuseWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("diffuseColor", AI_TYPE_RGB);
    PROCESS_PARAM("diffuseRoughness", AI_TYPE_FLOAT);
    PROCESS_PARAM("diffuseIndirectScale", AI_TYPE_FLOAT);
    PROCESS_PARAM("diffuseIndirectClamp", AI_TYPE_FLOAT);
    PROCESS_PARAM("diffuseNormal", AI_TYPE_VECTOR);

    // emission
    PROCESS_PARAM("emissionWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("emissionColor", AI_TYPE_RGB);

    // specular
    PROCESS_PARAM("specularWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularColor", AI_TYPE_RGB);
    PROCESS_PARAM("specularRoughness", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularAnisotropy", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularRotation", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularRoughnessDepthScale", AI_TYPE_FLOAT)
    PROCESS_PARAM("specularIndirectScale", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularIndirectClamp", AI_TYPE_FLOAT);
    PROCESS_PARAM("specularInternalDirect", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("specularNormal", AI_TYPE_VECTOR);

    // transmission
    PROCESS_PARAM("transmissionWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("transmissionColor", AI_TYPE_RGB);
    PROCESS_PARAM("transmissionRoughness", AI_TYPE_FLOAT);
    PROCESS_PARAM("transmissionIor", AI_TYPE_FLOAT);
    PROCESS_PARAM("transmissionRoughnessDepthScale", AI_TYPE_FLOAT);
    PROCESS_PARAM("transmissionEnableInternalReflections", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("transmissionClamp", AI_TYPE_FLOAT);
    PROCESS_PARAM("transmissionDoDirect", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("transmissionNormal", AI_TYPE_VECTOR);

    // scatterring
    PROCESS_PARAM("ssInteriorType", AI_TYPE_ENUM);
    PROCESS_PARAM("ssAttenuationColor", AI_TYPE_RGB);
    PROCESS_PARAM("ssScatteringWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("ssScattering", AI_TYPE_RGB);
    PROCESS_PARAM("ssAbsorption", AI_TYPE_RGB);
    PROCESS_PARAM("ssDensityScale", AI_TYPE_FLOAT);
    PROCESS_PARAM("ssDirection", AI_TYPE_FLOAT);


    // *** EXTENSION ATTRIBUTES ***

    PROCESS_PARAM("aiEnableMatte", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("aiMatteColor", AI_TYPE_RGB);
    PROCESS_PARAM("aiMatteColorA", AI_TYPE_FLOAT);

    PROCESS_PARAM("aiDiffuseExtraSamples", AI_TYPE_INT);
    PROCESS_PARAM("aiSpecularExtraSamples", AI_TYPE_INT);
    PROCESS_PARAM("aiSpecularDistribution", AI_TYPE_INT);
    PROCESS_PARAM("aiTransmissionExtraSamples", AI_TYPE_INT);

    // aovs
    PROCESS_PARAM("aiAovDiffuseColor", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectDiffuseRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectDiffuseRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectSpecular", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectSpecular", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovSingleScatter", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovRefraction", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovEmission", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovUV", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDepth", AI_TYPE_STRING);

    PROCESS_PARAM( "aiStandardCompatibleAOVs", AI_TYPE_BOOLEAN);
    PROCESS_PARAM( "aiTransmitAovs", AI_TYPE_BOOLEAN);

    // sets
    PROCESS_PARAM("aiTraceSetAll", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetShadows", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetSpecular", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetTransmission", AI_TYPE_STRING);

    PROCESS_PARAM("aiDebug", AI_TYPE_INT);
}

void BifrostHydroTranslator::NodeInitializer(CAbTranslator context)
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    ADD_BOOL("aiEnableMatte", "aiem", false);
    ADD_COLOR("aiMatteColor", "aima", 0.0f, 0.0f, 0.0f );
    ADD_FLOAT("aiMatteColorA", "aimca", 0.0f, 0.f, 1.f );

    ADD_INT_MIN("aiDiffuseExtraSamples", "aidxs", 0, 0, 10);
    ADD_INT_MIN("aiSpecularExtraSamples", "aisxs", 0, 0, 10);
    MStringArray distributions;
    distributions.append(MString("Microfacet GGX"));
    distributions.append(MString("Beckmann"));
    ADD_ENUM("aiSpecularDistribution", "aisdist", 0, distributions);
    ADD_INT_MIN("aiTransmissionExtraSamples", "aitxs", 0, 0, 10);

    ADD_BOOL("aiRussianRoulette", "airr", true);
    ADD_INT_MIN("aiRrTransmissionDepth", "airrtd", 1, 0, 10);

    // aovs
    ADD_STR("aiAovDiffuseColor",        "aiaovdc",  "");
    ADD_STR("aiAovDirectDiffuse",       "aiaovdd",  "");
    ADD_STR("aiAovDirectDiffuseRaw",    "aiaovddr", "");
    ADD_STR("aiAovIndirectDiffuse",     "aiaovid",  "");
    ADD_STR("aiAovIndirectDiffuseRaw",  "aiaovidr", "");
    ADD_STR("aiAovDirectSpecular",      "aiaovds",  "");
    ADD_STR("aiAovIndirectSpecular",    "aiaovis", "");
    ADD_STR("aiAovSingleScatter",       "aiaovss",  "");
    ADD_STR("aiAovRefraction",          "aiaovref",  "");
    ADD_STR("aiAovEmission",            "aiaovem",  "");
    ADD_STR("aiAovUV",                  "aiaovuv",  "");
    ADD_STR("aiAovDepth",               "aiaovdepth",  "");

    ADD_BOOL( "aiStandardCompatibleAOVs", "aiscaovs", false );
    ADD_BOOL( "aiTransmitAovs", "aitaovs", false );

    // sets
    ADD_STR("aiTraceSetAll",         "aitraceal", "" );
    ADD_STR("aiTraceSetShadows",     "aitracess", "" );
    ADD_STR("aiTraceSetDiffuse",     "aitracesd", "" );
    ADD_STR("aiTraceSetSpecular",    "aitracess", "" );
    ADD_STR("aiTraceSetTransmission","aitracest", "" );

    // debug
    ADD_INT_MIN("aiDebug", "aidbg", 0, 0, 5);
}

