#include "AttrHelper.h"
#include "nodes/ShaderUtils.h"
#include "utils/Metadata.h"
#include "Utils.h"

#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <maya/MDGModifier.h>

// convert from "arnold_style" to "mayaStyle"
// ignores the capitalization of input strings: letters are only capitalized
// if they follow an underscore
//
MString toMayaStyle(MString s)
{
   MString name;
   bool capitalize = false;
   for (unsigned int i=0; i < s.numChars(); i++)
   {
      MString c = s.substringW(i, i);
      if (c == "_")
         capitalize = true;
      else if (capitalize)
      {
         name += c.toUpperCase();
         capitalize = false;
      }
      else
         // always go to lower case
         // this avoids ugly things like GI_diffuse_samples --> GIDiffuseSamples
         // and instead produces the slightly nicer giDiffuseSamples
         name += c.toLowerCase();
   }
   return name;
}

// uses "maya.name" parameter metadata if set, otherwise, converts from
// "arnold_style" to "mayaStyle"
MString CBaseAttrHelper::GetMayaAttrName(const char* paramName)
{
   const char* attrName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.name", &attrName))
	   return MString(attrName);
   return toMayaStyle(paramName);
}

// uses "maya.shortname" parameter metadata if set, otherwise, uses the arnold
// parameter name
MString CBaseAttrHelper::GetMayaAttrShortName(const char* paramName)
{
	const char* attrShortName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.shortname", &attrShortName))
      return MString(attrShortName);
   return MString(paramName);
}


bool CBaseAttrHelper::GetAttrData(const char* paramName, CAttrData& data)
{
   if (m_nodeEntry == NULL)
   {
      AiMsgError("Cannot retrieve parameter metadata from a null node entry");
      return false;
   }

   const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(m_nodeEntry, paramName);
   if (paramEntry == NULL)
   {
      AiMsgError("Parameter does not exist: %s", paramName);
      return false;
   }

   AiMsgDebug("Getting attribute metadata for %s.%s", AiNodeEntryGetName(m_nodeEntry), paramName);

   data.defaultValue = MAiParamGetDefault(m_nodeEntry, paramEntry);
   data.name = GetMayaAttrName(paramName);
   data.shortName = GetMayaAttrShortName(paramName);
   data.type = AiParamGetType(paramEntry);

   AiMetaDataGetBool(m_nodeEntry, paramName, "maya.keyable", &data.keyable);

   if (data.type == AI_TYPE_ARRAY)
   {
      data.type = data.defaultValue.ARRAY->type;
      AtBoolean animatable;
      // if a parameter is marked as animatable, this means that the value can
      // change over the course of a single render. if this is the case, and the
      // attribute is an array, then it should be exposed as a non-array within maya;
      // the array will be populated during translation of motion steps
      //
      // Also, by convention, matrix arrays with name "matrix" are animatable
      // attributes.
      if ((AiMetaDataGetBool(m_nodeEntry, paramName, "animatable", &animatable) && animatable) ||
           (data.type == AI_TYPE_MATRIX && strcmp(paramName, "matrix") == 0))
      {
         data.isArray = false;
         // since this parameter is not to be treated as an array within maya,
         // change the default value
         switch (data.type)
         {
            case AI_TYPE_BYTE:
            {
               data.defaultValue.BYTE = AiArrayGetByte(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_INT:
            {
               data.defaultValue.INT = AiArrayGetInt(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_UINT:
            {
               data.defaultValue.UINT = AiArrayGetUInt(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_BOOLEAN:
            {
               data.defaultValue.BOOL = AiArrayGetBool(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_FLOAT:
            {
               data.defaultValue.FLT = AiArrayGetFlt(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_RGB:
            {
               data.defaultValue.RGB = AiArrayGetRGB(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_RGBA:
            {
               data.defaultValue.RGBA = AiArrayGetRGBA(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_VECTOR:
            {
               data.defaultValue.VEC = AiArrayGetVec(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_POINT:
            {
               data.defaultValue.PNT = AiArrayGetPnt(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_POINT2:
            {
               data.defaultValue.PNT2 = AiArrayGetPnt2(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_STRING:
            {
               data.defaultValue.STR = AiArrayGetStr(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_POINTER:
            {
               data.defaultValue.PTR = AiArrayGetPtr(data.defaultValue.ARRAY, 0);
               break;
            }
            case AI_TYPE_MATRIX:
            {
               // FIXME: doesn't work
               // AiArrayGetMtx(data.defaultValue.ARRAY, 0, data.defaultValue.pMTX);
               break;
            }
            case AI_TYPE_ENUM:
            case AI_TYPE_NONE:
            case AI_TYPE_NODE:
            {
               // not supported by arrays
               break;
            }
            default:
            {
               AiMsgError("Unable to get data for parameter %s: unknown parameter type", paramName);
               break;
            }
         }
      }
      else
         data.isArray = true;
   }

   switch (data.type)
   {
       case AI_TYPE_INT:
      {
         AtInt val;
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.INT = val;
            data.hasMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.INT = val;
            data.hasMax = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.INT = val;
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.INT = val;
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_UINT:
      {
         AtInt val;
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.INT = (val < 0 ? 0 : val);
            data.hasMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.INT = (val < 0 ? 0 : val);
            data.hasMax = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.INT = (val < 0 ? 0 : val);
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.INT = (val < 0 ? 0 : val);
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_FLOAT:
      {
         AtFloat val;
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.FLT = val;
            data.hasMin = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.FLT = val;
            data.hasMax = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.FLT = val;
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.FLT = val;
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_ENUM:
      {
         const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(m_nodeEntry, paramName);
         for (int ei=0;;ei++)
         {
            const char* enumStr = AiEnumGetString(AiParamGetEnum(paramEntry), ei);
            if (!enumStr)
               break;
            data.enums.append(enumStr);
         }
      }
   }
   return true;
}

void CBaseAttrHelper::MakeInputInt(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputInt(attrib, data);
}

void CBaseAttrHelper::MakeInputInt(CAttrData& data)
{
   MObject attrib;
   return MakeInputInt(attrib, data);
}

void CBaseAttrHelper::MakeInputInt(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   MStatus status;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kInt, data.defaultValue.INT, &status);
   CHECK_MSTATUS(status);
   if (data.hasMin)
      nAttr.setMin((AtInt)data.min.INT);
   if (data.hasMax)
      nAttr.setMax((AtInt)data.max.INT);
   if (data.hasSoftMin)
      nAttr.setSoftMin((AtInt)data.softMin.INT);
   if (data.hasSoftMax)
      nAttr.setSoftMax((AtInt)data.softMax.INT);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputBoolean(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputBoolean(attrib, data);
}

void CBaseAttrHelper::MakeInputBoolean(CAttrData& data)
{
   MObject attrib;
   return MakeInputBoolean(attrib, data);
}

void CBaseAttrHelper::MakeInputBoolean(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kBoolean, data.defaultValue.BOOL);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputFloat(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputFloat(attrib, data);
}

void CBaseAttrHelper::MakeInputFloat(CAttrData& data)
{
   MObject attrib;
   return MakeInputFloat(attrib, data);
}

void CBaseAttrHelper::MakeInputFloat(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kFloat, data.defaultValue.FLT);
   if (data.hasMin)
      nAttr.setMin((AtFloat)data.min.FLT);
   if (data.hasMax)
      nAttr.setMax((AtFloat)data.max.FLT);
   if (data.hasSoftMin)
      nAttr.setSoftMin((AtFloat)data.softMin.FLT);
   if (data.hasSoftMax)
      nAttr.setSoftMax((AtFloat)data.softMax.FLT);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputRGB(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputRGB(attrib, data);
}

void CBaseAttrHelper::MakeInputRGB(CAttrData& data)
{
   MObject attrib;
   return MakeInputRGB(attrib, data);
}

void CBaseAttrHelper::MakeInputRGB(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.RGB.r, data.defaultValue.RGB.g, data.defaultValue.RGB.b);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputRGBA(MObject& attrib, MObject& attribA, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputRGBA(attrib, attribA, data);
}

void CBaseAttrHelper::MakeInputRGBA(CAttrData& data)
{
   MObject attrib;
   MObject attribA;
   return MakeInputRGBA(attrib, attribA, data);
}

void CBaseAttrHelper::MakeInputRGBA(MObject& attrib, MObject& attribA, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.RGBA.r, data.defaultValue.RGBA.g, data.defaultValue.RGBA.b);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);

   attribA = nAttr.create(data.name + "A", data.shortName + "a", MFnNumericData::kFloat, data.defaultValue.RGBA.a);
   nAttr.setHidden(true);
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attribA);
}

void CBaseAttrHelper::MakeInputVector(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputVector(attrib, data);
}

void CBaseAttrHelper::MakeInputVector(CAttrData& data)
{
   MObject attrib;
   return MakeInputVector(attrib, data);
}

void CBaseAttrHelper::MakeInputVector(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.VEC.x, data.defaultValue.VEC.y, data.defaultValue.VEC.z);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputPoint(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputPoint(attrib, data);
}

void CBaseAttrHelper::MakeInputPoint(CAttrData& data)
{
   MObject attrib;
   return MakeInputPoint(attrib, data);
}

void CBaseAttrHelper::MakeInputPoint(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.PNT.x, data.defaultValue.PNT.y, data.defaultValue.PNT.z);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}
void CBaseAttrHelper::MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputPoint2(attrib, attribX, attribY, data);
}

void CBaseAttrHelper::MakeInputPoint2(CAttrData& data)
{
   MObject attrib;
   MObject attribX;
   MObject attribY;
   return MakeInputPoint2(attrib, attribX, attribY, data);
}

void CBaseAttrHelper::MakeInputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attribX = nAttr.create(data.name + "X", data.shortName + "x", MFnNumericData::kFloat, data.defaultValue.PNT2.x);
   attribY = nAttr.create(data.name + "Y", data.shortName + "y", MFnNumericData::kFloat, data.defaultValue.PNT2.y);
   attrib = nAttr.create(data.name, data.shortName, attribX, attribY);
   nAttr.setDefault(float(data.defaultValue.PNT2.x), float(data.defaultValue.PNT2.y));
   nAttr.setArray(AtBooleanToBool(data.isArray));
   nAttr.setKeyable(AtBooleanToBool(data.keyable));
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputString(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputString(attrib, data);
}

void CBaseAttrHelper::MakeInputString(CAttrData& data)
{
   MObject attrib;
   return MakeInputString(attrib, data);
}

void CBaseAttrHelper::MakeInputString(MObject& attrib, CAttrData& data)
{
   MFnTypedAttribute tAttr;

   attrib = tAttr.create(data.name, data.shortName, MFnData::kString);
   MFnStringData strData;
   MObject defObj = strData.create(data.defaultValue.STR);
   tAttr.setDefault(defObj);
   tAttr.setArray(AtBooleanToBool(data.isArray));
   tAttr.setKeyable(AtBooleanToBool(data.keyable));
   tAttr.setStorable(true);
   tAttr.setReadable(true);
   tAttr.setWritable(true);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputMatrix(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputMatrix(attrib, data);
}

void CBaseAttrHelper::MakeInputMatrix(CAttrData& data)
{
   MObject attrib;
   return MakeInputMatrix(attrib, data);
}

void CBaseAttrHelper::MakeInputMatrix(MObject& attrib, CAttrData& data)
{
   MFnMatrixAttribute mAttr;

   attrib = mAttr.create(data.name, data.shortName, MFnMatrixAttribute::kFloat);
   mAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_INPUT(mAttr, attrib);

//   const AtParamValue* data.defaultValue = AiParamGetDefault(paramEntry);
//   MFloatMatrix mmat(data.defaultValue.pMTX);
//   mAttr.setDefault(mmat);
}

void CBaseAttrHelper::MakeInputEnum(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputEnum(attrib, data);
}

void CBaseAttrHelper::MakeInputEnum(CAttrData& data)
{
   MObject attrib;
   return MakeInputEnum(attrib, data);
}

void CBaseAttrHelper::MakeInputEnum(MObject& attrib, CAttrData& data)
{
   MFnEnumAttribute eAttr;
   attrib = eAttr.create(data.name, data.shortName, data.defaultValue.INT);
   for (unsigned int ei = 0; ei < data.enums.length(); ++ei)
      eAttr.addField(data.enums[ei], ei);

   addAttribute(attrib);
   eAttr.setArray(AtBooleanToBool(data.isArray));
}

void CBaseAttrHelper::MakeInputNode(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputNode(attrib, data);
}

void CBaseAttrHelper::MakeInputNode(CAttrData& data)
{
   MObject attrib;
   return MakeInputNode(attrib, data);
}

void CBaseAttrHelper::MakeInputNode(MObject& attrib, CAttrData& data)
{
   MFnMessageAttribute msgAttr;

   attrib = msgAttr.create(data.name, data.shortName);
   msgAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_INPUT(msgAttr, attrib);
}

MObject CBaseAttrHelper::MakeInput(const char* paramName)
{
   CAttrData attrData;
   GetAttrData(paramName, attrData);
   return MakeInput(attrData);
}

MObject CBaseAttrHelper::MakeInput(CAttrData& attrData)
{
   MObject input;
   switch (attrData.type)
   {
      case AI_TYPE_INT:
      {
         MakeInputInt(input, attrData);
         break;
      }

      case AI_TYPE_UINT:
      {
         MakeInputInt(input, attrData);
         break;
      }
      case AI_TYPE_BOOLEAN:
      {
         MakeInputBoolean(input, attrData);
         break;
      }
      case AI_TYPE_FLOAT:
      {
         MakeInputFloat(input, attrData);
         break;
      }
      case AI_TYPE_RGB:
      {
         MakeInputRGB(input, attrData);
         break;
      }
      case AI_TYPE_RGBA:
      {
         MObject inputA;
         MakeInputRGBA(input, inputA, attrData);
         m_attributes[std::string(attrData.name.asChar()) + "A"] = inputA;
         break;
      }
      case AI_TYPE_VECTOR:
      {
         MakeInputVector(input, attrData);
         break;
      }
      case AI_TYPE_POINT:
      {
         MakeInputPoint(input, attrData);
         break;
      }
      case AI_TYPE_POINT2:
      {
         MObject inputX;
         MObject inputY;

         MakeInputPoint2(input, inputX, inputY, attrData);
         m_attributes[std::string(attrData.name.asChar()) + "X"] = inputX;
         m_attributes[std::string(attrData.name.asChar()) + "Y"] = inputY;
         break;
      }
      case AI_TYPE_STRING:
      {
         MakeInputString(input, attrData);
         break;
      }
      case AI_TYPE_NODE:
      {
         MakeInputNode(input, attrData);
         break;
      }
      case AI_TYPE_MATRIX:
      {
         MakeInputMatrix(input, attrData);
         break;
      }
      case AI_TYPE_ENUM:
      {
         MakeInputEnum(input, attrData);
         break;
      }
      case AI_TYPE_NONE:
      case AI_TYPE_BYTE:
      case AI_TYPE_POINTER:
      {
         const char* typeName = AiParamGetTypeName(attrData.type);
         MGlobal::displayWarning(MString("[mtoa] Unable to create attribute \"") + attrData.name + "\": parameters of type " + typeName + " are not supported");
         return MObject::kNullObj;
      }
      default:
      {
         MGlobal::displayError(MString("[mtoa] Unable to create attribute \"") + attrData.name + "\": unknown parameter type");
         return MObject::kNullObj;
      }
   } // switch
   m_attributes[attrData.name.asChar()] = input;
   return input;
}



void CBaseAttrHelper::MakeOutputInt(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(OUT_NAME, OUT_SHORTNAME, MFnNumericData::kInt);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputBoolean(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(OUT_NAME, OUT_SHORTNAME, MFnNumericData::kBoolean);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputFloat(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(OUT_NAME, OUT_SHORTNAME, MFnNumericData::kFloat);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGB(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, OUT_SHORTNAME);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGBA(MObject& attrib, MObject& attribA, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, OUT_SHORTNAME);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);

   attribA = nAttr.create(OUT_COLOR_NAME + "A", OUT_SHORTNAME + "a", MFnNumericData::kFloat);
   nAttr.setHidden(true);
   MAKE_OUTPUT(nAttr, attribA);
}

void CBaseAttrHelper::MakeOutputVector(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(OUT_NAME, OUT_SHORTNAME);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputPoint(MObject& attrib, bool isArray)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(OUT_NAME, OUT_SHORTNAME);
   nAttr.setArray(isArray);
   MAKE_INPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, bool isArray)
{
   MFnNumericAttribute nAttr;

   attribX = nAttr.create(OUT_NAME + "X", OUT_SHORTNAME + "x", MFnNumericData::kFloat);
   attribY = nAttr.create(OUT_NAME + "Y", OUT_SHORTNAME + "y", MFnNumericData::kFloat);
   attrib = nAttr.create(OUT_NAME, OUT_SHORTNAME, attribX, attribY);
   nAttr.setArray(isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputString(MObject& attrib, bool isArray)
{
   MFnTypedAttribute tAttr;

   attrib = tAttr.create(OUT_NAME, OUT_SHORTNAME, MFnData::kString);
   MFnStringData strData;
   MObject defObj = strData.create();
   tAttr.setDefault(defObj);
   tAttr.setArray(isArray);
   MAKE_OUTPUT(tAttr, attrib);
}

void CBaseAttrHelper::MakeOutputMatrix(MObject& attrib, bool isArray)
{
   MFnMatrixAttribute mAttr;

   attrib = mAttr.create(OUT_NAME, OUT_SHORTNAME, MFnMatrixAttribute::kFloat);
   // attrib = msgAttr.create(OUT_MATRIX_NAME, OUT_SHORTNAME);
   mAttr.setArray(isArray);
   MAKE_OUTPUT(mAttr, attrib);

//   const AtParamValue* data.defaultValue = AiParamGetDefault(paramEntry);
//   MFloatMatrix mmat(data.defaultValue.pMTX);
//   mAttr.setDefault(mmat);
}

void CBaseAttrHelper::MakeOutputNode(MObject& attrib, bool isArray)
{
   MFnMessageAttribute msgAttr;

   attrib = msgAttr.create(OUT_NAME, OUT_SHORTNAME);
   // attrib = msgAttr.create(OUT_NODE_NAME, OUT_SHORTNAME);
   msgAttr.setArray(isArray);
   MAKE_OUTPUT(msgAttr, attrib);
}

MObject CBaseAttrHelper::MakeOutput()
{

   AtInt outputType;
   if (!AiMetaDataGetInt(m_nodeEntry, NULL, "maya.output", &outputType))
   {
      outputType = AiNodeEntryGetOutputType(m_nodeEntry);
   }
   /*
   if (data.type == AI_TYPE_ARRAY)
   {
      data.type = data.defaultValue.ARRAY->type;
      // FIXME: a boolean "animatable" metatada should be added to these
      // paramaters to explicitly control this behavior.
      //
      // by convention, matrix arrays with name "matrix" are animatable
      // attributes, meaning they are intended to hold values for multiple
      // motion steps. because they are filled during translation from multiple
      // time samples, on the maya node they should be created as non-array
      // attributes.
      if (!(data.type == AI_TYPE_MATRIX && strcmp(paramName, "matrix") == 0))
         data.isArray = true;
   }
   */
   // cout << "making " << AiNodeEntryGetName(m_nodeEntry) << "." << attrData.name.asChar() << " (" << attrData.shortName.asChar() << "): " << attrData.type << endl;
   MObject output;
   switch (outputType)
   {
      case AI_TYPE_INT:
      {
         MakeOutputInt(output);
         break;
      }

      case AI_TYPE_UINT:
      {
         MakeOutputInt(output);
         break;
      }
      case AI_TYPE_BOOLEAN:
      {
         MakeOutputBoolean(output);
         break;
      }
      case AI_TYPE_FLOAT:
      {
         MakeOutputFloat(output);
         break;
      }
      case AI_TYPE_RGB:
      {
         MakeOutputRGB(output);
         break;
      }
      case AI_TYPE_RGBA:
      {
         MObject outputA;
         MakeOutputRGBA(output, outputA);
         m_attributes["outValueA"] = outputA;
         break;
      }
      case AI_TYPE_VECTOR:
      {
         MakeOutputVector(output);
         break;
      }
      case AI_TYPE_POINT:
      {
         MakeOutputPoint(output);
         break;
      }
      case AI_TYPE_POINT2:
      {
         MObject outputX;
         MObject outputY;

         MakeOutputPoint2(output, outputX, outputY);
         m_attributes["outValueX"] = outputX;
         m_attributes["outValueY"] = outputY;
         break;
      }
      case AI_TYPE_STRING:
      {
         MakeOutputString(output);
         break;
      }
      case AI_TYPE_NONE:
      case AI_TYPE_NODE:
      {
         MakeOutputNode(output);
         break;
      }
      case AI_TYPE_MATRIX:
      {
         MakeOutputMatrix(output);
         break;
      }
      case AI_TYPE_ENUM:
      case AI_TYPE_BYTE:
      case AI_TYPE_POINTER:
      {
         const char* typeName = AiParamGetTypeName(outputType);
         MGlobal::displayWarning(MString("[mtoa]  Unable to create attribute \"") + OUT_NAME + "\": parameters of type " + typeName + " are not supported");
         return MObject::kNullObj;
      }
      default:
      {
         MGlobal::displayError(MString("[mtoa] Unable to create attribute \"") + OUT_NAME + "\": unknown parameter type");
         return MObject::kNullObj;
      }
   } // switch
   m_attributes[OUT_NAME.asChar()] = output;
   return output;
}

void CBaseAttrHelper::SetNode(const char* arnoldNodeName)
{
   m_nodeEntry = AiNodeEntryLookUp(arnoldNodeName);
};


// CStaticAttrHelper
//
MStatus CStaticAttrHelper::addAttribute(MObject& attrib)
{
   MStatus stat;
   stat = m_addFunc(attrib);
   // FIXME: not reliable to use MFnAttribute to get the name: the MObject could be invalid
   if (stat != MS::kSuccess)
      MGlobal::displayError(MString("[mtoa] Unable to create attribute ") + AiNodeEntryGetName(m_nodeEntry) + "." + MFnAttribute(attrib).name());
   CHECK_MSTATUS(stat);
   return stat;
}

// CDynamicAttrHelper
//
MStatus CDynamicAttrHelper::addAttribute(MObject& attrib)
{
   MStatus stat;
   MStatus statAttr;
   MFnDependencyNode fnNode = MFnDependencyNode(m_instance);
   MObject mAttr = fnNode.attribute(MFnAttribute(attrib).name(),&statAttr);
   if (statAttr == MS::kSuccess)
   {
      if (MFnAttribute(attrib).type() == MFnAttribute(mAttr).type())
      {
         return stat;
      }
   }
   stat = fnNode.addAttribute(attrib);
   // FIXME: not reliable to use MFnAttribute to get the name: the MObject could be invalid
   if (stat != MS::kSuccess)
      MGlobal::displayError(MString("[mtoa] Unable to create attribute ") + fnNode.name() + "." + MFnAttribute(attrib).name());
   CHECK_MSTATUS(stat);
   return stat;
}

// CExtensionAttrHelper
//

#if MAYA_API_VERSION < 201200

void CExtensionAttrHelper::MakeInputInt(CAttrData& data)
{
   data.type = AI_TYPE_INT;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputBoolean(CAttrData& data)
{
   data.type = AI_TYPE_BOOLEAN;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputFloat(CAttrData& data)
{
   data.type = AI_TYPE_FLOAT;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputRGB(CAttrData& data)
{
   data.type = AI_TYPE_RGB;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputRGBA(CAttrData& data)
{
   data.type = AI_TYPE_RGBA;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputVector(CAttrData& data)
{
   data.type = AI_TYPE_VECTOR;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputPoint(CAttrData& data)
{
   data.type = AI_TYPE_POINT;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputPoint2(CAttrData& data)
{
   data.type = AI_TYPE_POINT2;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputString(CAttrData& data)
{
   data.type = AI_TYPE_STRING;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputMatrix(CAttrData& data)
{
   data.type = AI_TYPE_MATRIX;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputEnum(CAttrData& data)
{
   data.type = AI_TYPE_ENUM;
   MakeInput(data);
}

void CExtensionAttrHelper::MakeInputNode(CAttrData& data)
{
   data.type = AI_TYPE_NODE;
   MakeInput(data);
}

MObject CExtensionAttrHelper::MakeInput(const char* paramName)
{
   CAttrData attrData;
   GetAttrData(paramName, attrData);
   return MakeInput(attrData);
}

MObject CExtensionAttrHelper::MakeInput(CAttrData& attrData)
{
   MStatus stat;
   stat = m_class.addExtensionAttribute(attrData);
   CHECK_MSTATUS(stat);
   // this is bad form, but we don't have an MObject to return yet
   return MObject::kNullObj;
}
#else
MStatus CExtensionAttrHelper::addAttribute(MObject& attrib)
{
   MStatus stat;
   MDGModifier dgMod;
   stat = dgMod.addExtensionAttribute(m_class, attrib);
   CHECK_MSTATUS(stat);
   return stat;
}
#endif
