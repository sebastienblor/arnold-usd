#include "bifrost_parameters.h"
#include "utils.h"

std::vector<std::string> ArrayToStrings(const AtArray* array){
    std::vector<std::string> strings;
    for(unsigned int i = 0; i < AiArrayGetNumElements(array); ++i)
        strings.push_back(AiArrayGetStr(array,i).c_str());
    return strings;
}

AtArray* CreateStringArray(const std::vector<std::string>& strings){
    AtArray* array = AiArrayAllocate(strings.size(),1,AI_TYPE_STRING);
    for(unsigned int i = 0; i < strings.size(); ++i)
        AiArraySetStr(array, i, strings[i].c_str());
    return array;
}

BifrostParams::BifrostParams(const AtNode* node){
    GET_STR(cache_file);
    GET_STR(object);
    GET_STR_ARRAY(channels);
    GET_STR_ARRAY(velocity_channels);
    GET_STR(uv_channel);

    GET_FLT(velocity_scale);
    GET_FLT(space_scale);
    GET_UINT(fps);

    GET_BOOL(clip);
    GET_BBOX(clip);

    GET_INT(debug);
}

std::string BifrostParams::str() const{
    std::stringstream ss;
    ss << std::endl;
    DUMP_PARAM(cache_file);
    DUMP_PARAM(object);
    DUMP_ARRAY_PARAM(channels);
    DUMP_ARRAY_PARAM(velocity_channels);
    DUMP_PARAM(uv_channel);

    DUMP_PARAM(velocity_scale);
    DUMP_PARAM(space_scale);
    DUMP_PARAM(fps);

    DUMP_PARAM(clip);
    DUMP_PARAM(clip_bbox);

    DUMP_PARAM(debug);
    return ss.str();
}

void BifrostParams::declare(AtList* params, AtNodeEntry* nentry){
    PARAM_STR(cache_file, "");
    PARAM_STR(object, "");
    PARAM_ARRAY(channels, CreateStringArray({"vorticity"}));
    PARAM_ARRAY(velocity_channels, CreateStringArray({"velocity_u", "velocity_v", "velocity_w"}));
    PARAM_STR(uv_channel, "uv");

    PARAM_FLT(velocity_scale, 1);
    PARAM_FLT(space_scale, 1);
    PARAM_UINT(fps, 24);

    PARAM_BOOL(clip, 0);
    PARAM_BBOX(clip, -1,-1,-1, 1,1,1)

    PARAM_INT(debug, 0);
}
