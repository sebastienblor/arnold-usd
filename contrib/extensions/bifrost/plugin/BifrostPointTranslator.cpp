#include <BifrostPointTranslator.h>
#include <TranslatorUtils.h>

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
    PROCESS_PARAM("densityNormal", AI_TYPE_VECTOR);

    PROCESS_PARAM("backlightIntensity", AI_TYPE_FLOAT);
    PROCESS_PARAM("backlightColor", AI_TYPE_RGB);
    PROCESS_PARAM("backlightIndirectScale", AI_TYPE_FLOAT);

    PROCESS_PARAM("emissionWeight", AI_TYPE_FLOAT);
    PROCESS_PARAM("emissionColor", AI_TYPE_RGB);

    PROCESS_PARAM("shadowTransmission", AI_TYPE_RGB);
    PROCESS_PARAM("opacity", AI_TYPE_RGB);

    // *** EXTENSION ATTRIBUTES ***

    PROCESS_PARAM("aiEnableMatte", AI_TYPE_BOOLEAN);
    PROCESS_PARAM("aiMatteColor", AI_TYPE_RGB);
    PROCESS_PARAM("aiMatteColorA", AI_TYPE_FLOAT);

    PROCESS_PARAM("aiTraceSetAll", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetShadows", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiTraceSetBacklight", AI_TYPE_STRING);

    // aovs
    PROCESS_PARAM("aiAovDensityNormal", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDiffuseColor", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectDiffuseRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectDiffuse", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectDiffuseRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectBacklight", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovDirectBacklightRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectBacklight", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovIndirectBacklightRaw", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovEmission", AI_TYPE_STRING);

    PROCESS_PARAM("aiAovLightGroup1", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup2", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup3", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup4", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup5", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup6", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup7", AI_TYPE_STRING);
    PROCESS_PARAM("aiAovLightGroup8", AI_TYPE_STRING);

    PROCESS_PARAM("aiDebug", AI_TYPE_INT);
}

void BifrostPointTranslator::NodeInitializer(CAbTranslator context)
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    ADD_BOOL("aiEnableMatte", "aiem", false);
    ADD_COLOR("aiMatteColor", "aima", 0.0f, 0.0f, 0.0f );
    ADD_FLOAT("aiMatteColorA", "aimca", 0.0f, 0.f, 1.f );

    ADD_STR("aiTraceSetAll",       "aitraceal", "" );
    ADD_STR("aiTraceSetShadows",   "aitracess", "" );
    ADD_STR("aiTraceSetDiffuse",   "aitracesd", "" );
    ADD_STR("aiTraceSetBacklight", "aitracesb", "" );

    // aovs
    ADD_STR("aiAovDensityNormal",       "aiaovdn",  "");
    ADD_STR("aiAovDiffuseColor",        "aiaovdc",  "");
    ADD_STR("aiAovDirectDiffuse",       "aiaovdd",  "");
    ADD_STR("aiAovDirectDiffuseRaw",    "aiaovddr", "");
    ADD_STR("aiAovIndirectDiffuse",     "aiaovid",  "");
    ADD_STR("aiAovIndirectDiffuseRaw",  "aiaovidr", "");
    ADD_STR("aiAovDirectBacklight",     "aiaovdb",  "");
    ADD_STR("aiAovDirectBacklightRaw",  "aiaovdbr", "");
    ADD_STR("aiAovIndirectBacklight",   "aiaovib",  "");
    ADD_STR("aiAovIndirectBacklightRaw","aiaovibr", "");
    ADD_STR("aiAovEmission",            "aiaovem",  "");

    ADD_STR("aiAovLightGroup1",         "aiaovlg1", "");
    ADD_STR("aiAovLightGroup2",         "aiaovlg2", "");
    ADD_STR("aiAovLightGroup3",         "aiaovlg3", "");
    ADD_STR("aiAovLightGroup4",         "aiaovlg4", "");
    ADD_STR("aiAovLightGroup5",         "aiaovlg5", "");
    ADD_STR("aiAovLightGroup6",         "aiaovlg6", "");
    ADD_STR("aiAovLightGroup7",         "aiaovlg7", "");
    ADD_STR("aiAovLightGroup8",         "aiaovlg8", "");

    // debug
    ADD_INT_MIN("aiDebug", "aidbg", 0, 0, 5);
}

