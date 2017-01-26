#pragma once

#define IMPORTANCE_EPS 1e-5f
#define M_RAN_INVM32 2.32830643653869628906e-010

#define VAR(x) #x << ": " << x
#define VARL(x) std::cerr << #x << ": " << x << "\n"

#define NUMLIGHTGROUPS 8

#define REGISTER_AOVS \
	data->aovs.clear(); \
	data->aovs.push_back(params[pAovDensityNormal].STR); \
	data->aovs.push_back(params[pAovDiffuseColor].STR); \
	data->aovs.push_back(params[pAovDirectDiffuse].STR); \
	data->aovs.push_back(params[pAovDirectDiffuseRaw].STR); \
	data->aovs.push_back(params[pAovIndirectDiffuse].STR); \
	data->aovs.push_back(params[pAovIndirectDiffuseRaw].STR); \
	data->aovs.push_back(params[pAovDirectBacklight].STR); \
	data->aovs.push_back(params[pAovDirectBacklightRaw].STR); \
	data->aovs.push_back(params[pAovEmission].STR); \
	for (size_t i=0; i < data->aovs.size(); ++i) \
	AiAOVRegister(data->aovs[i].c_str(), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY); \

enum AovIndices
{
	kDensityNormal = 0,
	kDiffuseColor,
	kDirectDiffuse,
	kDirectDiffuseRaw,
	kIndirectDiffuse,
	kIndirectDiffuseRaw,
	kDirectBacklight,
	kDirectBacklightRaw,
	kEmission
};

enum PhaseFunctions
{
	kSchlick = 0,
	kHenyey
};

static const char* phaseFunctionNames[] = {"Henyey-Greenstein", NULL};
