#pragma once

#include <vector>
#include <string>
#include <aminomath/vec.h>
#include <aminomath/bbox.h>
#include <ai_nodes.h>
#include <ai_msg.h>

#define GET_ENUM(param, CAST) param = (CAST) AiNodeGetInt(node, #param)
#define GET_BOOL(param) param = AiNodeGetBool(node, #param)
#define GET_FLT(param) param = AiNodeGetFlt(node, #param)
#define GET_INT(param) param = AiNodeGetInt(node, #param)
#define GET_UINT(param) param = AiNodeGetUInt(node, #param)
#define GET_VEC2(param) param = AtVector2ToAminoVec2f(AiNodeGetVec2(node, #param))
#define GET_VEC(param) param = AtVectorToAminoVec3f(AiNodeGetVec(node, #param))
#define GET_STR(param) param = AiNodeGetStr(node, #param)
#define GET_STR_ARRAY(param) param = ArrayToStrings(AiNodeGetArray(node, #param))
#define GET_BBOX(prefix) prefix ## _bbox = amino::Math::bboxf( AtVectorToAminoVec3f(AiNodeGetVec(node,#prefix "_min")), AtVectorToAminoVec3f(AiNodeGetVec(node,#prefix "_max")) );

#define PARAM_ENUM(param, v, strings) AiParameterEnum(#param, v, strings)
#define PARAM_BOOL(param, v) AiParameterBool(#param, v)
#define PARAM_FLT(param, v) AiParameterFlt(#param, v)
#define PARAM_INT(param, v) AiParameterInt(#param, v)
#define PARAM_UINT(param, v) AiParameterUInt(#param, v)
#define PARAM_VEC2(param, v1, v2) AiParameterVec2(#param, v1, v2)
#define PARAM_VEC(param, v1, v2, v3) AiParameterVec(#param, v1, v2, v3)
#define PARAM_STR(param, v) AiParameterStr(#param, v)
#define PARAM_ARRAY(param, v) AiParameterArray(#param, v)
#define PARAM_BBOX(prefix, a1, a2, a3, b1, b2, b3) AiParameterVec(#prefix "_min", a1, a2, a3); AiParameterVec(#prefix "_max",  b1, b2, b3)

#define DUMP_PARAM(param) ss << "    " << #param << " = " << (param) << std::endl;
#define DUMP_ARRAY_PARAM(param) \
    ss << "    " << #param << " = " << std::endl; \
    for(const std::string& s : param) \
        ss << "        " << s << std::endl;

std::vector<std::string> ArrayToStrings(const AtArray* array);
AtArray* CreateStringArray(const std::vector<std::string>& strings);

enum RenderComponent{
    VOXELS, PARTICLES
};
#define RENDER_COMPONENT_STRINGS { "voxels", "particles", NULL }

enum SmoothMode{
    MEAN, GAUSSIAN, MEDIAN, LAPLACIAN_FLOW, CURVATURE_FLOW
};
#define SMOOTH_MODE_STRINGS { "mean", "gaussian", "median", "laplacian_flow", "curvature_flow", NULL }

enum BlendType{
    LINEAR, SMOOTH, SMOOTHER
};
#define BLEND_TYPE_STRINGS { "linear", "smooth", "smoother", NULL }

struct BifrostParams{
    std::string cache_file;
    std::string tmp_folder;
    std::string object;
    std::vector<std::string> channels;
    std::vector<std::string> velocity_channels;
    std::string uv_channel;

    float velocity_scale;
    float space_scale;
    unsigned int fps;

    bool clip;
    amino::Math::bboxf clip_bbox;

    int debug;

    BifrostParams(const AtNode* node);
    virtual std::string str() const;
    static void declare(AtList* params, AtNodeEntry* nentry);
};
