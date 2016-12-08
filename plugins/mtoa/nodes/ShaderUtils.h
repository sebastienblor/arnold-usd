#pragma once

#include <ai_params.h>
#include <ai_node_entry.h>
#include <maya/MString.h>

// Arnold Swatch Renderer
const MString ARNOLD_SWATCH("ArnoldRenderSwatch");

// Shader classify constants
const MString CLASSIFY_SHADER("shader");
const MString CLASSIFY_SHADER_SURFACE("shader/surface");
const MString CLASSIFY_SHADER_VOLUME("shader/volume");
const MString CLASSIFY_SHADER_ATMOSPHERE("shader/volume/atmosphere");
const MString CLASSIFY_SHADER_DISPLACEMENT("shader/displacement");
const MString CLASSIFY_SHADER_LIGHT("light");
const MString CLASSIFY_SHADER_LIGHT_FILTER("light/filter");
const MString CLASSIFY_SHADER_TEXTURE("texture");
const MString CLASSIFY_SHADER_ENVIRONMENT("texture/environment");
const MString CLASSIFY_SHADER_UTILITY("utility");

#define ARNOLD_CLASSIFY( classification ) (MString("rendernode/arnold/") + classification)

const MString CLASSIFY_ARNOLD_DISPLACEMENT(ARNOLD_CLASSIFY(CLASSIFY_SHADER_DISPLACEMENT));
const MString CLASSIFY_ARNOLD_LIGHT(ARNOLD_CLASSIFY(CLASSIFY_SHADER_LIGHT));
const MString CLASSIFY_ARNOLD_LIGHT_FILTER(ARNOLD_CLASSIFY(CLASSIFY_SHADER_LIGHT_FILTER));
const MString CLASSIFY_ARNOLD_ENVIRONMENT(ARNOLD_CLASSIFY(CLASSIFY_SHADER_ENVIRONMENT));

const MString DISPLACEMENT_NO_SWATCH = CLASSIFY_ARNOLD_DISPLACEMENT
                               + ":" + CLASSIFY_SHADER_DISPLACEMENT;
const MString DISPLACEMENT_WITH_SWATCH = DISPLACEMENT_NO_SWATCH
                               + ":swatch/" + ARNOLD_SWATCH;

const MString LIGHT_NO_SWATCH = CLASSIFY_ARNOLD_LIGHT
                              + ":" + CLASSIFY_SHADER_LIGHT;
const MString LIGHT_WITH_SWATCH = LIGHT_NO_SWATCH
                              + ":swatch/" + ARNOLD_SWATCH;
                              
const MString LIGHT_FILTER_NO_SWATCH = CLASSIFY_ARNOLD_LIGHT_FILTER
                              + ":" + CLASSIFY_SHADER_LIGHT_FILTER;

const MString LIGHT_FILTER_WITH_SWATCH = CLASSIFY_ARNOLD_LIGHT_FILTER
                              + ":" + CLASSIFY_SHADER_LIGHT_FILTER
                              + ":swatch/" + ARNOLD_SWATCH;

const MString ENVIRONMENT_NO_SWATCH = CLASSIFY_ARNOLD_ENVIRONMENT
                              + ":" + CLASSIFY_SHADER_ENVIRONMENT;
const MString ENVIRONMENT_WITH_SWATCH = ENVIRONMENT_NO_SWATCH
                              + ":swatch/" + ARNOLD_SWATCH;

// attrFn.setCached(true);
// attrFn.setInternal(true);

#define MAKE_INPUT(attrFn, attr) \
   attrFn.setKeyable(true); \
   attrFn.setStorable(true);	\
   attrFn.setReadable(true);	\
   attrFn.setWritable(true); \
   addAttribute(attr)

#define MAKE_OUTPUT(attrFn, attr) \
   attrFn.setKeyable(false); \
   attrFn.setStorable(false); \
   attrFn.setReadable(true); \
   attrFn.setWritable(false); \
   addAttribute(attr)

#define MAKE_COLOR(attrib, attrName, attrShortName, defaultR, defaultG, defaultB) \
   attrib##R = nAttr.create(MString(attrName) + "R", MString(attrShortName) + "r", MFnNumericData::kFloat, defaultR);\
   attrib##G = nAttr.create(MString(attrName) + "G", MString(attrShortName) + "g", MFnNumericData::kFloat, defaultG);\
   attrib##B = nAttr.create(MString(attrName) + "B", MString(attrShortName) + "b", MFnNumericData::kFloat, defaultB);\
   attrib = nAttr.create(attrName, attrShortName, attrib##R, attrib##G, attrib##B);\
   nAttr.setUsedAsColor(true);\
   nAttr.setDefault(float(defaultR), float(defaultG), float(defaultB));\
   addAttribute(attrib##R);\
   addAttribute(attrib##G);\
   addAttribute(attrib##B);

#define MAKE_RGBA(attrib, attrName, attrShortName, defaultR, defaultG, defaultB, defaultA) \
   attrib##R = nAttr.create(MString(attrName) + "R", MString(attrShortName) + "r", MFnNumericData::kFloat, defaultR);\
   attrib##G = nAttr.create(MString(attrName) + "G", MString(attrShortName) + "g", MFnNumericData::kFloat, defaultG);\
   attrib##B = nAttr.create(MString(attrName) + "B", MString(attrShortName) + "b", MFnNumericData::kFloat, defaultB);\
   attrib = nAttr.create(attrName, attrShortName, attrib##R, attrib##G, attrib##B);\
   nAttr.setUsedAsColor(true);\
   nAttr.setDefault(float(defaultR), float(defaultG), float(defaultB));\
   addAttribute(attrib##R);\
   addAttribute(attrib##G);\
   addAttribute(attrib##B);\
   attrib##A = nAttr.create(MString(attrName) + "A", MString(attrShortName) + "a", MFnNumericData::kFloat, defaultA);\
   addAttribute(attrib##A);

#define MAKE_VECTOR2(attrib, attrName, attrShortName, defaultX, defaultY) \
   attrib##X = nAttr.create(MString(attrName) + "X", MString(attrShortName) + "x", MFnNumericData::kFloat, defaultX);\
   attrib##Y = nAttr.create(MString(attrName) + "Y", MString(attrShortName) + "y", MFnNumericData::kFloat, defaultY);\
   attrib = nAttr.create(attrName, attrShortName, attrib##X, attrib##Y);\
   nAttr.setDefault(float(defaultX), float(defaultY));\
   addAttribute(attrib);

#define MAKE_VECTOR2(attrib, attrName, attrShortName, defaultX, defaultY) \
   MAKE_VECTOR2(attrib, attrName, attrShortName, defaultX, defaultY)

#define MAKE_VECTOR(attrib, attrName, attrShortName, defaultX, defaultY, defaultZ) \
   attrib##X = nAttr.create(MString(attrName) + "X", MString(attrShortName) + "x", MFnNumericData::kFloat, defaultX);\
   attrib##Y = nAttr.create(MString(attrName) + "Y", MString(attrShortName) + "y", MFnNumericData::kFloat, defaultY);\
   attrib##Z = nAttr.create(MString(attrName) + "Z", MString(attrShortName) + "z", MFnNumericData::kFloat, defaultZ);\
   attrib = nAttr.create(attrName, attrShortName, attrib##X, attrib##Y, attrib##Z);\
   nAttr.setDefault(float(defaultX), float(defaultY), float(defaultZ));\
   addAttribute(attrib##X);\
   addAttribute(attrib##Y);\
   addAttribute(attrib##Z);

#define MAKE_ENUM(attrib, attrName, attrShortName, default_value, arnold_node, arnold_param) \
   attrib = eAttr.create(attrName, attrShortName, default_value); \
   { \
      const AtNodeEntry*  nentry = AiNodeEntryLookUp(arnold_node); \
      const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(nentry, arnold_param); \
      /* const AtParamValue* default_value = AiParamGetDefault(paramEntry); */ \
      for (int i=0;;i++) \
      { \
         const char* enum_string = AiEnumGetString(AiParamGetEnum(paramEntry), i); \
         if (!enum_string) \
            break; \
         eAttr.addField(enum_string, i); \
      } \
   } \
   addAttribute(attrib);

#define MAKE_ENUM_INPUT(attrib, attrName, attrShortName, default_value, arnold_node, arnold_param) \
   attrib = eAttr.create(attrName, attrShortName, default_value); \
   { \
      const AtNodeEntry*  nentry = AiNodeEntryLookUp(arnold_node); \
      const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(nentry, arnold_param); \
      /* const AtParamValue* default_value = AiParamGetDefault(paramEntry); */ \
      for (int i=0;;i++) \
      { \
         const char* enum_string = AiEnumGetString(AiParamGetEnum(paramEntry), i); \
         if (!enum_string) \
            break; \
         eAttr.addField(enum_string, i); \
      } \
   } \
   MAKE_INPUT(eAttr, attrib)

#define MAKE_INT_INPUT(attrib, nodeEntry, paramEntry, paramName, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   attrib = nAttr.create(attrName, attrShortName, MFnNumericData::kInt, default_value->INT);\
   int val;\
   if (AiMetaDataGetInt(nodeEntry, paramName, "min", &val))\
      nAttr.setMin(val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "max", &val))\
      nAttr.setMax(val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "softmin", &val))\
      nAttr.setSoftMin(val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "softmax", &val))\
      nAttr.setSoftMax(val);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_UINT_INPUT(attrib, nodeEntry, paramEntry, paramName, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   attrib = nAttr.create(attrName, attrShortName, MFnNumericData::kInt, default_value->INT);\
   int val;\
   if (AiMetaDataGetInt(nodeEntry, paramName, "min", &val))\
      nAttr.setMin(val < 0 ? 0 : val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "max", &val))\
      nAttr.setMax(val < 0 ? 0 : val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "softmin", &val))\
      nAttr.setSoftMin(val < 0 ? 0 : val);\
   if (AiMetaDataGetInt(nodeEntry, paramName, "softmax", &val))\
      nAttr.setSoftMax(val < 0 ? 0 : val);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_BOOLEAN_INPUT(attrib, paramEntry, attrName, attrShortName) \
   const AtParamValue* defaultValue = AiParamGetDefault(paramEntry);\
   attrib = nAttr.create(attrName, attrShortName, MFnNumericData::kBoolean, defaultValue->BOOL);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_FLOAT_INPUT(attrib, nodeEntry, paramEntry, paramName, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   attrib = nAttr.create(attrName, attrShortName, MFnNumericData::kFloat, default_value->FLT);\
   float val;\
   if (AiMetaDataGetFlt(nodeEntry, paramName, "min", &val))\
      nAttr.setMin(val);\
   if (AiMetaDataGetFlt(nodeEntry, paramName, "max", &val))\
      nAttr.setMax(val);\
   if (AiMetaDataGetFlt(nodeEntry, paramName, "softmin", &val))\
      nAttr.setSoftMin(val);\
   if (AiMetaDataGetFlt(nodeEntry, paramName, "softmax", &val))\
      nAttr.setSoftMax(val);\
   MAKE_INPUT(nAttr, attrib);\

#define MAKE_RGB_INPUT(attrib, paramEntry, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   MAKE_COLOR(attrib, attrName, attrShortName, default_value->RGB.r, default_value->RGB.g, default_value->RGB.b);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_RGBA_INPUT(attrib, attribA, paramEntry, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   MAKE_COLOR(attrib, attrName, attrShortName, default_value->RGB.r, default_value->RGB.g, default_value->RGB.b);\
   MAKE_INPUT(nAttr, attrib);\
   attribA = nAttr.create(MString(attrName) + "A", MString(attrShortName) + "a", MFnNumericData::kFloat, default_value->RGBA.a);\
   addAttribute(attribA);\
   MAKE_INPUT(nAttr, attribA);

#define MAKE_VECTOR_INPUT(attrib, paramEntry, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   MAKE_VECTOR(attrib, attrName, attrShortName, default_value->VEC.x, default_value->VEC.y, default_value->VEC.z);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_VECTOR2_INPUT(attrib, paramEntry, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   MAKE_VECTOR2(attrib, attrName, attrShortName, default_value->VEC2.x, default_value->VEC2.y);\
   MAKE_INPUT(nAttr, attrib);

#define MAKE_STRING_INPUT(attrib, paramEntry, attrName, attrShortName) \
   const AtParamValue* default_value = AiParamGetDefault(paramEntry);\
   attrib = tAttr.create(attrName, attrShortName, MFnData::kString);\
   MFnStringData strData;\
   MObject defObj = strData.create(default_value->STR);\
   tAttr.setDefault(defObj);\
   MAKE_INPUT(tAttr, attrib);

#define MAKE_MATRIX_INPUT(attrib, paramEntry, attrName, attrShortName) \
   attrib = mAttr.create(attrName, attrShortName, MFnMatrixAttribute::kFloat);\
   MAKE_INPUT(mAttr, attrib);

//   const AtParamValue* default_value = AiParamGetDefault(paramEntry);
//   MFloatMatrix mmat(default_value->pMTX);
//   mAttr.setDefault(mmat);



#define EXPORT_DYN_PARAM_FLOAT(arnold_node, param_name, node) \
   plug = node.findPlug(param_name);\
   if (!plug.isNull())\
      AiNodeSetFlt(arnold_node, param_name, plug.asFloat());

#define EXPORT_DYN_PARAM_BOOL(arnold_node, param_name, node) \
   plug = node.findPlug(param_name);\
   if (!plug.isNull())\
      AiNodeSetBool(arnold_node, param_name, plug.asBool());

#define EXPORT_DYN_PARAM_INT(arnold_node, param_name, node) \
   plug = node.findPlug(param_name);\
   if (!plug.isNull())\
      AiNodeSetInt(arnold_node, param_name, plug.asInt());
