#pragma once

#define IMPORTANCE_EPS 1e-5f
#define M_RAN_INVM32 2.32830643653869628906e-010

#define HYDRO_RAY_UNDEFINED 0

#define VAR(x) #x << ": " << x
#define VARL(x) std::cerr << #x << ": " << x << "\n"

#define TEA_STREAM_HYDRO_RR_OFFSET 0
#define TEA_STREAM_HYDRO_RR_JITTER 1

#define TEA_STREAM_HYDRO_RR_DIFF_OFFSET 2
#define TEA_STREAM_HYDRO_RR_DIFF_JITTER 3

#define TEA_STREAM_HYDRO_RR_SPEC_OFFSET 4
#define TEA_STREAM_HYDRO_RR_SPEC_JITTER 5

#define TEA_STREAM_HYDRO_RR2_OFFSET 12

#define NUMLIGHTGROUPS 8
#define NUMAOVs 12

#define REGISTER_AOVS \
	data->aovs.clear(); \
	data->aovs.push_back(params[pAovDiffuseColor].STR); \
	data->aovs.push_back(params[pAovDirectDiffuse].STR); \
	data->aovs.push_back(params[pAovDirectDiffuseRaw].STR); \
	data->aovs.push_back(params[pAovIndirectDiffuse].STR); \
	data->aovs.push_back(params[pAovIndirectDiffuseRaw].STR); \
	data->aovs.push_back(params[pAovDirectSpecular].STR); \
	data->aovs.push_back(params[pAovIndirectSpecular].STR); \
	data->aovs.push_back(params[pAovInterior].STR); \
	data->aovs.push_back(params[pAovRefraction].STR); \
	data->aovs.push_back(params[pAovEmission].STR); \
	data->aovs.push_back(params[pAovUv].STR); \
	data->aovs.push_back(params[pAovDepth].STR); \
	assert(NUMAOVs == data->aovs.size() && "NUM_AOVs does not match size of aovs array!"); \
	for (size_t i=0; i < data->aovs.size(); ++i) \
	AiAOVRegister(data->aovs[i].c_str(), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY); \

enum AovIndices
{
	kDiffuseColor = 0,
	kDirectDiffuse,
	kDirectDiffuseRaw,
	kIndirectDiffuse,
	kIndirectDiffuseRaw,
	kDirectSpecular,
	kIndirectSpecular,
	kInterior,
	kRefraction,
	kEmission,
	kUv,
	kDepth
};
