#include <aminomath/vec.h>
#include <aminomath/bbox.h>
#include "defs.h"
#include <ai_vector.h>
#include <ai_msg.h>
#include <ai_array.h>
#include <vector>
#include <ai_params.h>
#include <ai_nodes.h>

#define GET_ENUM(param, CAST) param = (CAST) AiNodeGetInt(node, #param)
#define GET_BOOL(param) param = AiNodeGetBool(node, #param)
#define GET_FLT(param) param = AiNodeGetFlt(node, #param)
#define GET_INT(param) param = AiNodeGetInt(node, #param)
#define GET_UINT(param) param = AiNodeGetUInt(node, #param)
#define GET_VEC2(param) param = Convert(AiNodeGetVec2(node, #param))
#define GET_VEC(param) param = Convert(AiNodeGetVec(node, #param))
#define GET_STR(param) param = AiNodeGetStr(node, #param)
#define GET_STR_ARRAY(param) param = ArrayToStrings(AiNodeGetArray(node, #param))
#define GET_BBOX(prefix) prefix ## _bbox = amino::Math::bboxf( Convert(AiNodeGetVec(node,#prefix "_min")), Convert(AiNodeGetVec(node,#prefix "_max")) );

#define PARAM_ENUM(param, strings) AiParameterEnum(#param, (int) param, strings)
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

#define PARAM_SHAPE(channels_) \
    PARAM_STR(activeGraph);\
    PARAM_STR(resourceId);\
    PARAM_STR(cache_folder);\
    PARAM_STR(object);\
    PARAM_STR(point_component);\
    PARAM_STR(voxel_component);\
    PARAM_INT(frame);\
    PARAM_ARRAY(channels, CreateStringArray channels_);\
    PARAM_STR(velocity_channel);\
    PARAM_STR(uv_channel);\
    PARAM_FLT(velocity_scale);\
    PARAM_FLT(space_scale);\
    PARAM_UINT(fps);\
    PARAM_BOOL(clip);\
    PARAM_BBOX(clip, -1,-1,-1, 1,1,1)

#define GET_SHAPE() \
    GET_STR(activeGraph);\
    GET_STR(resourceId);\
    GET_STR(cache_folder);\
    GET_STR(object);\
    GET_STR(point_component);\
    GET_STR(voxel_component);\
    GET_STR(object);\
    GET_INT(frame);\
    GET_STR_ARRAY(channels);\
    GET_STR(velocity_channel);\
    GET_STR(uv_channel);\
    GET_FLT(velocity_scale);\
    GET_FLT(space_scale);\
    GET_UINT(fps);\
    GET_BOOL(clip);\
    GET_BBOX(clip)
