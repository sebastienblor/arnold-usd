#ifndef TRANSLATORUTILS_H
#define TRANSLATORUTILS_H

#define DUMP(v) std::cerr << __FILE__ << ":" << __LINE__ << ": " << #v << " = "  << (v) << std::endl
#define DL std::cerr << __FILE__ << ":" << __LINE__ << " (" << __PRETTY_FUNCTION__ << ")" << std::endl

#define PROCESS_PARAM(name, type) ProcessParameter(instance, name, type, name);

#define ADD_BOOL(_longName, _shortName, _default) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.defaultValue.BOOL = _default;\
    helper.MakeInputBoolean(data)

#define ADD_STR(_longName, _shortName, _default) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.defaultValue.STR = _default;\
    helper.MakeInputString(data)

#define ADD_COLOR(_longName, _shortName, _r, _g, _b) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.defaultValue.RGB = AiColor(_r, _g, _b);\
    helper.MakeInputRGB(data)

#define ADD_ENUM(_longName, _shortName, _default, _enums) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.defaultValue.INT = _default;\
    data.enums = _enums; \
    helper.MakeInputEnum(data)

#define ADD_FLOAT(_longName, _shortName, _default, _min, _max) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.hasSoftMin = data.hasSoftMax = true;\
    data.hasMin = data.hasMax = true;\
    data.softMin.FLT = data.min.FLT = _min;\
    data.softMax.FLT = data.max.FLT = _max;\
    data.defaultValue.FLT = _default;\
    helper.MakeInputFloat(data)

#define ADD_FLOAT_NOMINMAX(_longName, _shortName, _default) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.hasSoftMin = false;\
    data.hasSoftMax = false;\
    data.hasMin = false;\
    data.hasMax = false;\
    data.defaultValue.FLT = _default;\
    helper.MakeInputFloat(data)

#define ADD_FLOAT_SOFT(_longName, _shortName, _default, _softMin, _softMax) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.hasSoftMin = true;\
    data.softMin.FLT = _softMin;\
    data.hasSoftMax = true;\
    data.softMax.FLT = _softMax;\
    data.hasMin = true;\
    data.min.FLT = 0.f;\
    data.hasMax = false;\
    data.defaultValue.FLT = _default;\
    helper.MakeInputFloat(data)

#define ADD_INT_MIN(_longName, _shortName, _default, _min, _softMax) \
    data.name = _longName; \
    data.shortName = _shortName;\
    data.hasSoftMin =  data.hasSoftMax = true;\
    data.hasMin = true;\
    data.hasMax = false;\
    data.min.INT = data.softMin.INT = _min;\
    data.softMax.INT = _softMax;\
    data.defaultValue.INT = _default;\
    helper.MakeInputInt(data)

#endif // TRANSLATORUTILS_H
