#pragma once

inline amino::Math::vec2f AtVector2ToAminoVec2f(const AtVector2& v){ return amino::Math::vec2f(v.x, v.y); }
inline amino::Math::vec3f AtVectorToAminoVec3f(const AtVector& v){ return amino::Math::vec3f(v.x, v.y, v.z); }
inline AtVector2 AminoVec2fToAtVector2(const amino::Math::vec2f& v){ return AtVector2(v[0],v[1]); }
inline AtVector AminoVec3fToAtVector(const amino::Math::vec3f& v){ return AtVector(v[0],v[1],v[2]); }
inline AtVector2 AminoVec2iToAtVector2(const amino::Math::vec2i& v){ return AtVector2(v[0],v[1]); }
inline AtVector AminoVec3iToAtVector(const amino::Math::vec3i& v){ return AtVector(v[0],v[1],v[2]); }

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

#define PARAM_ENUM(param, strings) AiParameterEnum(#param, param, strings)
#define PARAM_BOOL(param) AiParameterBool(#param, param)
#define PARAM_FLT(param) AiParameterFlt(#param, param)
#define PARAM_INT(param) AiParameterInt(#param, param)
#define PARAM_UINT(param) AiParameterUInt(#param, param)
#define PARAM_VEC2(param) AiParameterVec2(#param, param[0], param[1])
#define PARAM_VEC(param) AiParameterVec(#param, param[0], param[1], param[2])
#define PARAM_STR(param) AiParameterStr(#param, param.c_str())
#define PARAM_ARRAY(param, v) AiParameterArray(#param, v)
#define PARAM_BBOX(prefix, a1, a2, a3, b1, b2, b3) AiParameterVec(#prefix "_min", a1, a2, a3); AiParameterVec(#prefix "_max",  b1, b2, b3)

#define DUMP_PARAM(param) ss << "    " << #param << " = " << (param) << std::endl;
#define DUMP_ARRAY_PARAM(param) \
    ss << "    " << #param << " = " << std::endl; \
    for(int i = 0; i < param.count(); ++i) \
        ss << "        " << param[i].c_str() << std::endl;

#define RENDER_COMPONENT_STRINGS { "voxels", "particles", NULL }
#define SMOOTH_MODE_STRINGS { "mean", "gaussian", "median", "laplacian_flow", "curvature_flow", NULL }
#define BLEND_TYPE_STRINGS { "linear", "smooth", "smoother", NULL }
