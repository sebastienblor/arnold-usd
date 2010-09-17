#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <ai_params.h>
#include <ai_node_entry.h>

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
   attrib##R = nAttr.create(MString(name) + "R", MString(shortname) + "r", MFnNumericData::kFloat, defaultR);\
   attrib##G = nAttr.create(MString(name) + "G", MString(shortname) + "g", MFnNumericData::kFloat, defaultG);\
   attrib##B = nAttr.create(MString(name) + "B", MString(shortname) + "b", MFnNumericData::kFloat, defaultB);\
   attrib = nAttr.create(name, shortname, attrib##R, attrib##G, attrib##B);\
   nAttr.setUsedAsColor(true);\
   nAttr.setDefault(float(defaultR), float(defaultG), float(defaultB));\
   addAttribute(attrib##R);\
   addAttribute(attrib##G);\
   addAttribute(attrib##B);

#define MAKE_VECTOR2(attrib, name, shortname, defaultX, defaultY) \
   attrib##X = nAttr.create(MString(name) + "X", MString(shortname) + "x", MFnNumericData::kFloat, defaultX);\
   attrib##Y = nAttr.create(MString(name) + "Y", MString(shortname) + "y", MFnNumericData::kFloat, defaultY);\
   attrib = nAttr.create(name, shortname, attrib##X, attrib##Y);\
   nAttr.setDefault(float(defaultX), float(defaultY));\
   addAttribute(attrib##X);\
   addAttribute(attrib##Y);

#define MAKE_POINT2(attrib, name, shortname, defaultX, defaultY) \
   MAKE_VECTOR2(attrib, name, shortname, defaultX, defaultY)

#define MAKE_VECTOR(attrib, name, shortname, defaultX, defaultY, defaultZ) \
   attrib##X = nAttr.create(MString(name) + "X", MString(shortname) + "x", MFnNumericData::kFloat, defaultX);\
   attrib##Y = nAttr.create(MString(name) + "Y", MString(shortname) + "y", MFnNumericData::kFloat, defaultY);\
   attrib##Z = nAttr.create(MString(name) + "Z", MString(shortname) + "z", MFnNumericData::kFloat, defaultZ);\
   attrib = nAttr.create(name, shortname, attrib##X, attrib##Y, attrib##Z);\
   nAttr.setDefault(float(defaultX), float(defaultY), float(defaultZ));\
   addAttribute(attrib##X);\
   addAttribute(attrib##Y);\
   addAttribute(attrib##Z);

#define MAKE_POINT(attrib, name, shortname, defaultX, defaultY, defaultZ) \
   MAKE_VECTOR(attrib, name, shortname, defaultX, defaultY, defaultZ)

#define MAKE_ENUM(attrib, name, shortname, default_value, arnold_node, arnold_param) \
   attrib = eAttr.create(name, shortname, default_value); \
   { \
      const AtNodeEntry*  nentry = AiNodeEntryLookUp(arnold_node); \
      const AtParamEntry* pentry = AiNodeEntryLookUpParameter(nentry, arnold_param); \
      for (int i=0;;i++) \
      { \
         const char* enum_string = AiEnumGetString(AiParamGetEnum(pentry), i); \
         if (!enum_string) \
            break; \
         eAttr.addField(enum_string, i); \
      } \
   } \
   addAttribute(attrib);

#endif // SHADER_UTILS_H
