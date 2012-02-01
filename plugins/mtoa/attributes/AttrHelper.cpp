#include "AttrHelper.h"
#include "nodes/ShaderUtils.h"
#include "attributes/Metadata.h"

#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <maya/MDGModifier.h>

#define AtBooleanToBool(x) ((x) ? true : false)

// convert from "arnold_style" to "mayaStyle"
// ignores the capitalization of input strings: letters are only capitalized
// if they follow an underscore
//
MString toMayaStyle(MString s)
{
   MString name;
   bool capitalize = false;
   MString c;
   for (unsigned int i=0; i < s.numChars(); i++)
   {
      c = s.substringW(i, i);
      if (c == "_")
      {
         capitalize = true;
      }
      else if (capitalize)
      {
         name += c.toUpperCase();
         capitalize = false;
      }
      else
      {
         // always go to lower case
         // this avoids ugly things like GI_diffuse_samples --> GIDiffuseSamples
         // and instead produces the slightly nicer giDiffuseSamples
         // TODO : but then ai_remapColor will yield aiRemapcolor
         // name += c.toLowerCase();
         name += c;
      }
   }
   return name;
}

/// Whether or not an Arnold parameter is hidden in Maya
bool CBaseAttrHelper::IsHidden(const char* paramName)
{
   // name is always hidden
   if (strcmp(paramName, "name") == 0) // 0 is match
      return true;

   AtBoolean hide;
   if (AiMetaDataGetBool(m_nodeEntry, paramName, "maya.hide", &hide) && hide)
      return true;
   else
      return false;
}

// uses "maya.name" parameter metadata if set, otherwise, converts from
// "arnold_style" to "mayaStyle"
// Add ai prefix to avoid clashes
MString CBaseAttrHelper::GetMayaAttrName(const char* paramName)
{
   const char* attrName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.name", &attrName))
      return MString(attrName);
   else
      return toMayaStyle(m_prefix + paramName);
}

// uses "maya.shortname" parameter metadata if set, otherwise, uses the arnold
// parameter name
MString CBaseAttrHelper::GetMayaAttrShortName(const char* paramName)
{
   const char* attrShortName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.shortname", &attrShortName))
      return MString(attrShortName);
   else
      return m_prefix + paramName;
}

bool CBaseAttrHelper::GetAttrData(const char* paramName, CAttrData& data)
{
   if (m_nodeEntry == NULL)
   {
      AiMsgError("[mtoa] Cannot retrieve parameter metadata from a null node entry.");
      return false;
   }

   const char* nodeName = AiNodeEntryGetName(m_nodeEntry);
   // AiMsgDebug("[mtoa] [node %s] [attr %s] Reading metadata", nodeName, paramName);

   const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(m_nodeEntry, paramName);
   if (paramEntry == NULL)
   {
      AiMsgError("[mtoa] [node %s] [attr %s] Attribute does not exist.", nodeName, paramName);
      return false;
   }

   data.defaultValue = MAiParamGetDefault(m_nodeEntry, paramEntry);
   data.name = GetMayaAttrName(paramName);
   data.shortName = GetMayaAttrShortName(paramName);
   data.type = AiParamGetType(paramEntry);
   const char* typeName = AiParamGetTypeName(data.type);

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
               AiMsgError("[mtoa] [node %s] [attr %s] Unknown parameter type %s", nodeName, paramName, typeName);
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
         int val;
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
         int val;
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
         float val;
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
         break;
      }
      case AI_TYPE_STRING:
      {
         data.stringDefault = data.defaultValue.STR;
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
      nAttr.setMin((int)data.min.INT);
   if (data.hasMax)
      nAttr.setMax((int)data.max.INT);
   if (data.hasSoftMin)
      nAttr.setSoftMin((int)data.softMin.INT);
   if (data.hasSoftMax)
      nAttr.setSoftMax((int)data.softMax.INT);
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
      nAttr.setMin((float)data.min.FLT);
   if (data.hasMax)
      nAttr.setMax((float)data.max.FLT);
   if (data.hasSoftMin)
      nAttr.setSoftMin((float)data.softMin.FLT);
   if (data.hasSoftMax)
      nAttr.setSoftMax((float)data.softMax.FLT);
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
   MObject defObj = strData.create(data.stringDefault);
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
   eAttr.setArray(AtBooleanToBool(AtBooleanToBool(data.isArray)));
   eAttr.setKeyable(AtBooleanToBool(data.keyable));
   eAttr.setStorable(true);
   eAttr.setReadable(true);
   eAttr.setWritable(true);
   addAttribute(attrib);
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
   msgAttr.setArray(AtBooleanToBool(AtBooleanToBool(data.isArray)));
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
         AiMsgWarning("[mtoa] Unable to create input attribute \"%s\": parameters of type %s are not supported", attrData.name.asChar(), typeName);
         return MObject::kNullObj;
      }
      default:
      {
         AiMsgError("[mtoa] Unable to create input attribute \"%s\": unknown parameter type", attrData.name.asChar());
         return MObject::kNullObj;
      }
   } // switch
   m_attributes[attrData.name.asChar()] = input;
   return input;
}

// Outputs

void CBaseAttrHelper::MakeOutputInt(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kInt);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputBoolean(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kBoolean);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputFloat(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kFloat);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGB(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, data.shortName);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGBA(MObject& attrib, MObject& attribA, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, data.shortName);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);

   attribA = nAttr.create(OUT_ALPHA_NAME, data.shortName + "a", MFnNumericData::kFloat);
   nAttr.setHidden(true);
   MAKE_OUTPUT(nAttr, attribA);
}

void CBaseAttrHelper::MakeOutputVector(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputPoint(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_INPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputPoint2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attribX = nAttr.create(data.name + "X", data.shortName + "x", MFnNumericData::kFloat);
   attribY = nAttr.create(data.name + "Y", data.shortName + "y", MFnNumericData::kFloat);
   attrib = nAttr.create(data.name, data.shortName, attribX, attribY);
   nAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputString(MObject& attrib, CAttrData& data)
{
   MFnTypedAttribute tAttr;

   attrib = tAttr.create(data.name, data.shortName, MFnData::kString);
   MFnStringData strData;
   MObject defObj = strData.create();
   tAttr.setDefault(defObj);
   tAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(tAttr, attrib);
}

void CBaseAttrHelper::MakeOutputMatrix(MObject& attrib, CAttrData& data)
{
   MFnMatrixAttribute mAttr;

   attrib = mAttr.create(data.name, data.shortName, MFnMatrixAttribute::kFloat);
   mAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(mAttr, attrib);
}

void CBaseAttrHelper::MakeOutputNode(MObject& attrib, CAttrData& data)
{
   MFnMessageAttribute msgAttr;

   attrib = msgAttr.create(data.name, data.shortName);
   msgAttr.setArray(AtBooleanToBool(data.isArray));
   MAKE_OUTPUT(msgAttr, attrib);
}

MObject CBaseAttrHelper::MakeOutput()
{
   MObject output;

   if (m_nodeEntry == NULL)
   {
      AiMsgError("[mtoa] Cannot retrieve output metadata from a null node entry.");
      return output;
   }
   // const char* nodeName = AiNodeEntryGetName(m_nodeEntry);
   // AiMsgDebug("[mtoa] [node %s] Reading output metadata", nodeName);

   CAttrData data;
   data.isArray = false;
   if (!AiMetaDataGetInt(m_nodeEntry, NULL, "maya.output", &data.type))
   {
      data.type = AiNodeEntryGetOutputType(m_nodeEntry);
   }
   const char* typeName = AiParamGetTypeName(data.type);
   const char* attrName;

   if (AiMetaDataGetStr(m_nodeEntry, NULL, "maya.output_name", &attrName))
   {
      data.name = MString(attrName);
   }
   else
   {
      if (AI_TYPE_RGB == data.type
            || AI_TYPE_RGBA == data.type)
      {
         data.name = OUT_COLOR_NAME;
      }
      else
      {
         data.name = OUT_NAME;
      }
   }
   const char* attrShortName;
   if (AiMetaDataGetStr(m_nodeEntry, NULL, "maya.output_shortname", &attrShortName))
   {
      data.shortName = MString(attrShortName);
   }
   else
   {
      data.shortName = OUT_SHORTNAME;
   }

   // cout << "making " << AiNodeEntryGetName(m_nodeEntry) << "." << attrData.name.asChar() << " (" << attrData.shortName.asChar() << "): " << attrData.type << endl;
   switch (data.type)
   {
      case AI_TYPE_INT:
      case AI_TYPE_UINT:
      {
         MakeOutputInt(output, data);
         break;
      }
      case AI_TYPE_BOOLEAN:
      {
         MakeOutputBoolean(output, data);
         break;
      }
      case AI_TYPE_FLOAT:
      {
         MakeOutputFloat(output, data);
         break;
      }
      case AI_TYPE_RGB:
      {
         MakeOutputRGB(output, data);
         break;
      }
      case AI_TYPE_RGBA:
      {
         MObject outputA;
         MakeOutputRGBA(output, outputA, data);
         m_attributes[OUT_ALPHA_NAME.asChar()] = outputA;
         break;
      }
      case AI_TYPE_VECTOR:
      {
         MakeOutputVector(output, data);
         break;
      }
      case AI_TYPE_POINT:
      {
         MakeOutputPoint(output, data);
         break;
      }
      case AI_TYPE_POINT2:
      {
         MObject outputX;
         MObject outputY;

         MakeOutputPoint2(output, outputX, outputY, data);
         m_attributes[std::string(data.name.asChar()) + "X"] = outputX;
         m_attributes[std::string(data.name.asChar()) + "Y"] = outputY;
         break;
      }
      case AI_TYPE_STRING:
      {
         MakeOutputString(output, data);
         break;
      }
      case AI_TYPE_NONE:
      case AI_TYPE_NODE:
      {
         MakeOutputNode(output, data);
         break;
      }
      case AI_TYPE_MATRIX:
      {
         MakeOutputMatrix(output, data);
         break;
      }
      case AI_TYPE_ENUM:
      case AI_TYPE_BYTE:
      case AI_TYPE_POINTER:
      {
         AiMsgWarning("[mtoa] Unable to create output attribute \"%s\": parameters of type %s are not supported", data.name.asChar(), typeName);
         return MObject::kNullObj;
      }
      default:
      {
         AiMsgError("[mtoa] Unable to create output attribute \"%s\": unknown parameter type", data.name.asChar());
         return MObject::kNullObj;
      }
   } // switch
   m_attributes[data.name.asChar()] = output;
   return output;
}

void CBaseAttrHelper::ReadPrefixMetadata()
{
   if (m_nodeEntry != NULL)
   {
      const char* tmp;
      if (AiMetaDataGetStr(m_nodeEntry, NULL, "maya.attr_prefix", &tmp))
         m_prefix = tmp;
   }
}

void CBaseAttrHelper::SetNode(const char* arnoldNodeName)
{
   m_nodeEntry = AiNodeEntryLookUp(arnoldNodeName);
   ReadPrefixMetadata();
};


// CStaticAttrHelper
//

MStatus CStaticAttrHelper::addAttribute(MObject& attrib)
{
   MStatus stat;
   stat = m_addFunc(attrib);
   // FIXME: not reliable to use MFnAttribute to get the name: the MObject could be invalid
   if (stat != MS::kSuccess)
   {
      AiMsgError("[mtoa] Unable to create static attribute corresponding to %s.%s", AiNodeEntryGetName(m_nodeEntry), MFnAttribute(attrib).name().asChar());
   }
   else
   {
      AiMsgDebug("[mtoa] Added static attribute %s.%s", AiNodeEntryGetName(m_nodeEntry), MFnAttribute(attrib).name().asChar());
   }
   CHECK_MSTATUS(stat);
   return stat;
}

// CDynamicAttrHelper
//

MStatus CDynamicAttrHelper::addAttribute(MObject& attrib)
{
   MStatus stat;
   MStatus statAttr;
   MFnDependencyNode fnNode;
   fnNode.setObject(m_instance);
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
   {
      AiMsgError("[mtoa] Unable to create dynamic attribute %s.%s", fnNode.name().asChar(), MFnAttribute(attrib).name().asChar());
   }
   else
   {
      AiMsgDebug("[mtoa] Added dynamic attribute %s.%s", fnNode.name().asChar(), MFnAttribute(attrib).name().asChar());
   }
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

   MString nodeType = m_class.typeName();
   MString attrName = MFnAttribute(attrib).name();

   MDGModifier dgMod;
   stat = dgMod.addExtensionAttribute(m_class, attrib);
   if (MStatus::kSuccess != stat)
   {
      AiMsgError("[mtoa] Unable to create extension attribute %s.%s", nodeType.asChar(), attrName.asChar());
   }
   else
   {
      AiMsgDebug("[mtoa] Added extension attribute %s.%s", nodeType.asChar(), attrName.asChar());
      stat = dgMod.doIt();
   }
   CHECK_MSTATUS(stat);
   return stat;
}
#endif
