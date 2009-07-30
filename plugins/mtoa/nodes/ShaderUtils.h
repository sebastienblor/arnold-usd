#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#define MAKE_INPUT(attr, name) \
   attr.setKeyable(true); \
   attr.setStorable(true);	\
   attr.setReadable(true);	\
   attr.setWritable(true); \
   addAttribute(name)

#define MAKE_OUTPUT(attr, name) \
   attr.setKeyable(false); \
   attr.setStorable(false); \
   attr.setReadable(true); \
   attr.setWritable(false); \
   addAttribute(name)

#define MAKE_COLOR(attrib, name, shortname, defaultR, defaultG, defaultB) \
   attrib##R = nAttr.create(name##"R", shortname##"r", MFnNumericData::kFloat, defaultR);\
   attrib##G = nAttr.create(name##"G", shortname##"g", MFnNumericData::kFloat, defaultG);\
   attrib##B = nAttr.create(name##"B", shortname##"b", MFnNumericData::kFloat, defaultB);\
   attrib = nAttr.create(name, shortname, attrib##R, attrib##G, attrib##B);\
   nAttr.setUsedAsColor(true);\
   addAttribute(attrib##R);\
   addAttribute(attrib##G);\
   addAttribute(attrib##B);

#define MAKE_VECTOR(attrib, name, shortname, defaultX, defaultY, defaultZ) \
   attrib##X = nAttr.create(name##"X", shortname##"x", MFnNumericData::kFloat, defaultX);\
   attrib##Y = nAttr.create(name##"Y", shortname##"y", MFnNumericData::kFloat, defaultY);\
   attrib##Z = nAttr.create(name##"Z", shortname##"z", MFnNumericData::kFloat, defaultZ);\
   attrib = nAttr.create(name, shortname, attrib##X, attrib##Y, attrib##Z);\
   addAttribute(attrib##X);\
   addAttribute(attrib##Y);\
   addAttribute(attrib##Z);

#endif // SHADER_UTILS_H
