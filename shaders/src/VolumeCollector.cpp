#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(VolumeCollectorMtd);

#define ARNOLD_NODEID_VOLUME_COLLECTOR 0x00115D1F

enum VolumeCollectorParams{
   p_scattering,
   p_attenuation,
   p_attenuation_mode,
   p_emission,
   p_phase
};

enum AttenuationMode{
   AM_ABSORPTION = 0,
   AM_EXTINCTION
};

const char* AttenuationModes[] = {"absorption", "extinction", 0};

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiVolumeCollector");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_VOLUME_COLLECTOR);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterRGB("scattering", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("attenuation", 1.0f, 1.0f, 1.0f);
   AiParameterEnum("attenuation_mode", 0, AttenuationModes);
   AiParameterRGB("emission", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("phase", 0.0f);

   AiMetaDataSetRGB(mds, "scattering", "min", AI_RGB_BLACK);
   AiMetaDataSetRGB(mds, "attenuation", "min", AI_RGB_BLACK);
   AiMetaDataSetRGB(mds, "emission", "min", AI_RGB_BLACK);
   AiMetaDataSetFlt(mds, "phase", "min", -1.0f);
   AiMetaDataSetFlt(mds, "phase", "max", 1.0f);
}

node_initialize
{

}

node_update
{

}

node_finish
{

}

shader_evaluate
{
   AiShaderGlobalsSetVolumeScattering(sg, AiShaderEvalParamRGB(p_scattering), AiShaderEvalParamFlt(p_phase));
   if (AiShaderEvalParamInt(p_attenuation_mode) == AM_ABSORPTION)
      AiShaderGlobalsSetVolumeAbsorption(sg, AiShaderEvalParamRGB(p_attenuation));
   else
      AiShaderGlobalsSetVolumeAttenuation(sg, AiShaderEvalParamRGB(p_attenuation));
   AiShaderGlobalsSetVolumeEmission(sg, AiShaderEvalParamRGB(p_emission));
}
