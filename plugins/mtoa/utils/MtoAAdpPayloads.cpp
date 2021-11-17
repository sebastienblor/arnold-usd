#include "MtoAAdpPayloads.h"
#include "Version.h"
#include <ai_adp.h>
#include <maya/MGlobal.h>

#include <unordered_map>
#include <string>

namespace MtoAADPPayloads
{
    bool product_metadata_posted = false;
}

std::unordered_map<std::string, std::string> shaders_map = {
{"lambert","AS_ARNOLD_SHADER"},
{"layeredShader","AS_ARNOLD_SHADER"},
{"layeredTexture","AS_ARNOLD_SHADER"},
{"file","AS_ARNOLD_SHADER"},
{"place2dTexture","AS_ARNOLD_SHADER"},
{"bump2d","AS_ARNOLD_SHADER"},
{"bump3d","AS_ARNOLD_SHADER"},
{"samplerInfo","AS_ARNOLD_SHADER"},
{"plusMinusAverage","AS_ARNOLD_SHADER"},
{"particleSamplerInfo","AS_ARNOLD_SHADER"},
{"remapValue","AS_ARNOLD_SHADER"},
{"remapColor","AS_ARNOLD_SHADER"},
{"remapHsv","AS_ARNOLD_SHADER"},
{"projection","AS_ARNOLD_SHADER"},
{"ramp","AS_ARNOLD_SHADER"},
{"animCurveTA","AS_ARNOLD_SHADER"},
{"animCurveTL","AS_ARNOLD_SHADER"},
{"animCurveTU","AS_ARNOLD_SHADER"},
{"shadingEngine","AS_ARNOLD_SHADER"},
{"displacementShader","AS_ARNOLD_SHADER"},
{"blinn","AS_ARNOLD_SHADER"},
{"phong","AS_ARNOLD_SHADER"},
{"phongE","AS_ARNOLD_SHADER"},
{"anisotropic","AS_ARNOLD_SHADER"},
{"rampShader","AS_ARNOLD_SHADER"},
{"singleShadingSwitch","AS_ARNOLD_SHADER"},
{"doubleShadingSwitch","AS_ARNOLD_SHADER"},
{"tripleShadingSwitch","AS_ARNOLD_SHADER"},
{"quadShadingSwitch","AS_ARNOLD_SHADER"},
{"fluidTexture2D","AS_ARNOLD_SHADER"},
{"blendColors","AS_ARNOLD_SHADER"},
{"multiplyDivide","AS_ARNOLD_SHADER"},
{"checker","AS_ARNOLD_SHADER"},
{"clamp","AS_ARNOLD_SHADER"},
{"condition","AS_ARNOLD_SHADER"},
{"luminance","AS_ARNOLD_SHADER"},
{"reverse","AS_ARNOLD_SHADER"},
{"surfaceLuminance","AS_ARNOLD_SHADER"},
{"gammaCorrect","AS_ARNOLD_SHADER"},
{"surfaceShader","AS_ARNOLD_SHADER"},
{"rgbToHsv","AS_ARNOLD_SHADER"},
{"hsvToRgb","AS_ARNOLD_SHADER"},
{"contrast","AS_ARNOLD_SHADER"},
{"setRange","AS_ARNOLD_SHADER"},
{"brownian","AS_MTOA_SHADER"},
{"bulge","AS_MTOA_SHADER"},
{"cloth","AS_MTOA_SHADER"},
{"cloudAlpha","AS_MTOA_SHADER"},
{"cloud","AS_MTOA_SHADER"},
{"crater","AS_MTOA_SHADER"},
{"envSphere","AS_MTOA_SHADER"},
{"fluid","AS_MTOA_SHADER"},
{"fluidData","AS_MTOA_SHADER"},
{"fluidTexture2D","AS_MTOA_SHADER"},
{"fractal","AS_MTOA_SHADER"},
{"granite","AS_MTOA_SHADER"},
{"grid","AS_MTOA_SHADER"},
{"leather","AS_MTOA_SHADER"},
{"marble","AS_MTOA_SHADER"},
{"mountain","AS_MTOA_SHADER"},
{"noise","AS_MTOA_SHADER"},
{"rock","AS_MTOA_SHADER"},
{"snow","AS_MTOA_SHADER"},
{"solidFractal","AS_MTOA_SHADER"},
{"stucco","AS_MTOA_SHADER"},
{"volumeNoise","AS_MTOA_SHADER"},
{"water","AS_MTOA_SHADER"}
};

void MtoAADPPayloads::ADPPostShaderUsed(const std::string shader_name, AtRenderSession* render_session)
{
    if (shader_name.size() == 0)
            return;

    if (shaders_map.find(shader_name) == shaders_map.end()) 
        return ; 
    
    AtParamValueMap* attributes = AiParamValueMap();
    AtParamValueMap* hints = AiParamValueMap();
    AiParamValueMapSetStr(attributes, AtString("SHADER_NAME"), AtString(shader_name.c_str()));
    AiParamValueMapSetStr(attributes, AtString("EXPORT_TYPE"), AtString(shaders_map[shader_name].c_str()));
    AiParamValueMapSetPtr(hints,AI_ADP_PAYLOAD_HINT_RENDER_SESSION, static_cast<void*>(render_session));
    AiADPSendPayload("MTOA.MAYA.SHADER.EXPORT", attributes, hints);
    AiParamValueMapDestroy(attributes);
    AiParamValueMapDestroy(hints);
}

void MtoAADPPayloads::ADPPostProductMetadata()
{
    if (!MtoAADPPayloads::product_metadata_posted)
    {
        AiADPAddProductMetadata(AI_ADP_PLUGINNAME, AtString("mtoa"));
        AiADPAddProductMetadata(AI_ADP_PLUGINVERSION, AtString(MTOA_VERSION));
        AiADPAddProductMetadata(AI_ADP_HOSTNAME, AtString("Maya"));
        AiADPAddProductMetadata(AI_ADP_HOSTVERSION, AtString(std::to_string(MAYA_API_VERSION).c_str()));
        MtoAADPPayloads::product_metadata_posted = true;
        return;
    }
}