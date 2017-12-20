#include "AttrHelper.h"
#include "nodes/ShaderUtils.h"
#include "attributes/Metadata.h"
#include "extension/ExtensionsManager.h"
#include "utils/MtoaLog.h"

#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <maya/MDGModifier.h>
#include <maya/MObjectArray.h>

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
         c = c.toUpperCase();
         name += c;
         capitalize = false;
      }
      else if (c.length() > 0)
      {
         // always go to lower case
         // this avoids ugly things like GI_diffuse_samples --> GIDiffuseSamples
         // and instead produces the slightly nicer giDiffuseSamples
         // TODO : but then ai_remapColor will yield aiRemapcolor
         // name += c.toLowerCase();
         name = name + c;
      }
   }
   return name;
}

/// Whether or not an Arnold parameter is hidden in Maya
bool CBaseAttrHelper::IsHidden(const char* paramName) const
{
   // name is always hidden
   if (strcmp(paramName, "name") == 0) // 0 is match
      return true;

   bool hide;
   if (AiMetaDataGetBool(m_nodeEntry, paramName, "maya.hide", &hide) && hide)
      return true;
   else
      return false;
}

// uses "maya.name" parameter metadata if set, otherwise, converts from
// "arnold_style" to "mayaStyle"
// Add ai prefix to avoid clashes
MString CBaseAttrHelper::GetMayaAttrName(const char* paramName) const
{
   AtString attrName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.name", &attrName))
   {
      MString attrNameStr(attrName.c_str());
      return attrNameStr;
   }
   else
   {
      MString paramNameStr(paramName);
      paramNameStr = m_prefix + paramNameStr;
      return toMayaStyle(paramNameStr);
   }
}

// uses "maya.shortname" parameter metadata if set, otherwise, uses the arnold
// parameter name
MString CBaseAttrHelper::GetMayaAttrShortName(const char* paramName) const
{
   AtString attrShortName;
   if (AiMetaDataGetStr(m_nodeEntry, paramName, "maya.shortname", &attrShortName))
      return MString(attrShortName);
   else
      return m_prefix + paramName;
}

bool CBaseAttrHelper::GetAttrData(const char* paramName, CAttrData& data)
{
   if (m_nodeEntry == NULL)
   {
      AiMsgError("[mtoa.attr] A valid node entry must be passed to query data for parameter \"%s\"", paramName);
      return false;
   }

   const char* nodeName = AiNodeEntryGetName(m_nodeEntry);
   
   const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(m_nodeEntry, paramName);
   if (paramEntry == NULL)
   {
      AiMsgError("[mtoa.attr] Arnold parameter \"%s.%s\" does not exist.", nodeName, paramName);
      return false;
   }

   data.defaultValue = MAiParamGetDefault(m_nodeEntry, paramEntry);
   data.name = GetMayaAttrName(paramName);
   data.shortName = GetMayaAttrShortName(paramName);
   data.type = AiParamGetType(paramEntry);
   const char* typeName = AiParamGetTypeName(data.type);

   bool keyable;
   if (AiMetaDataGetBool(m_nodeEntry, paramName, "maya.keyable", &keyable) && keyable)
      data.keyable = true;
   else
      data.keyable = false;

   bool connectable;
   if (!AiMetaDataGetBool(m_nodeEntry, paramName, "maya.connectable", &connectable))
      connectable = true;
   data.linkable = connectable;

   bool channelBox;
   if (AiMetaDataGetBool(m_nodeEntry, paramName, "maya.channelbox", &channelBox) && channelBox)
      channelBox = true;
   else
      channelBox = false; 
   data.channelBox = channelBox;

   if (data.type == AI_TYPE_ARRAY)
   {
      const AtParamValue* real = AiParamGetDefault(paramEntry);
      data.type = AiArrayGetType(real->ARRAY());
      bool animatable;
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
      }
      else
      {
         data.isArray = true;
      }
         
      switch (data.type)
      {
         case AI_TYPE_BYTE:
         case AI_TYPE_INT:
         case AI_TYPE_UINT:
         case AI_TYPE_BOOLEAN:
         case AI_TYPE_FLOAT:
         case AI_TYPE_RGB:
         case AI_TYPE_CLOSURE:
         case AI_TYPE_RGBA:
         case AI_TYPE_VECTOR:
         case AI_TYPE_VECTOR2:
         case AI_TYPE_STRING:
         case AI_TYPE_POINTER:
         {
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
         case AI_TYPE_ARRAY:
         {
            // not supported by arrays
            break;
         }
         default:
         {
            AiMsgError("[mtoa.attr] [node %s] [attr %s] Unknown parameter type %s", nodeName, paramName, typeName);
            break;
         }
      }
   }

   switch (data.type)
   {
      case AI_TYPE_INT:
      {
         int val;
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.INT() = val;
            data.hasMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.INT() = val;
            data.hasMax = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.INT() = val;
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.INT() = val;
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_BYTE:
      {
         int val;
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.BYTE() = (int)val;
            data.hasMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.BYTE() = (int)val;
            data.hasMax = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.BYTE() = (int)val;
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.BYTE() = (int)val;
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_UINT:
      {
         int val;
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.INT() = (val < 0 ? 0 : val);
            data.hasMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.INT() = (val < 0 ? 0 : val);
            data.hasMax = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.INT() = (val < 0 ? 0 : val);
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetInt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.INT() = (val < 0 ? 0 : val);
            data.hasSoftMax = true;
         }
         break;
      }
      case AI_TYPE_FLOAT:
      {
         float val;
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "min", &val))
         {
            data.min.FLT() = val;
            data.hasMin = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "max", &val))
         {
            data.max.FLT() = val;
            data.hasMax = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "softmin", &val))
         {
            data.softMin.FLT() = val;
            data.hasSoftMin = true;
         }
         if (AiMetaDataGetFlt(m_nodeEntry, paramName, "softmax", &val))
         {
            data.softMax.FLT() = val;
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

            MString enumMString(enumStr);
            data.enums.append(enumMString);
         }
         break;
      }
      case AI_TYPE_STRING:
      {
         data.stringDefault = data.defaultValue.STR();
         bool usedAsFilename = false;
         if (AiMetaDataGetBool(m_nodeEntry, paramName, "maya.usedAsFilename", &usedAsFilename))
            data.usedAsFilename = usedAsFilename;       

      }
   }
   return true;
}

void CBaseAttrHelper::MakeInputInt(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputInt(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputInt(CAttrData& data)
{
   MObject attrib;
   MakeInputInt(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputInt(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   MStatus status;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kInt, data.defaultValue.INT(), &status);
   CHECK_MSTATUS(status);
   if (data.hasMin)
      nAttr.setMin((int)data.min.INT());
   if (data.hasMax)
      nAttr.setMax((int)data.max.INT());
   if (data.hasSoftMin)
      nAttr.setSoftMin((int)data.softMin.INT());
   if (data.hasSoftMax)
      nAttr.setSoftMax((int)data.softMax.INT());
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputByte(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputByte(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputByte(CAttrData& data)
{
   MObject attrib;
   MakeInputByte(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputByte(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   MStatus status;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kByte, data.defaultValue.BYTE(), &status);
   CHECK_MSTATUS(status);
   if (data.hasMin)
      nAttr.setMin(data.min.BYTE());
   if (data.hasMax)
      nAttr.setMax(data.max.BYTE());
   if (data.hasSoftMin)
      nAttr.setSoftMin(data.softMin.BYTE());
   if (data.hasSoftMax)
      nAttr.setSoftMax(data.softMax.BYTE());
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

/*
void CBaseAttrHelper::MakeInputCurveRamp(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputCurveRamp(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputCurveRamp(CAttrData& data)
{
   MObject attrib;
   MakeInputCurveRamp(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputCurveRamp(MObject& attrib, CAttrData& data)
{
   MRampAttribute rAttr;
   MStatus status;
   attrib = rAttr.createCurveRamp(data.name, data.shortName, &status);
   CHECK_MSTATUS(status);
}

void CBaseAttrHelper::MakeInputColorRamp(MObject& attrib, const char* paramName)
{
   CAttrData data;
   GetAttrData(paramName, data);
   MakeInputColorRamp(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputColorRamp(CAttrData& data)
{
   MObject attrib;
   MakeInputColorRamp(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputColorRamp(MObject& attrib, CAttrData& data)
{
   MRampAttribute rAttr;
   MStatus status;
   attrib = rAttr.createColorRamp(data.name, data.shortName, &status);
   CHECK_MSTATUS(status);
}
*/

void CBaseAttrHelper::MakeInputBoolean(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputBoolean(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputBoolean(CAttrData& data)
{
   MObject attrib;
   MakeInputBoolean(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputBoolean(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kBoolean, data.defaultValue.BOOL());
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputFloat(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputFloat(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputFloat(CAttrData& data)
{
   MObject attrib;
   MakeInputFloat(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputFloat(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kFloat, data.defaultValue.FLT());
   if (data.hasMin)
      nAttr.setMin((float)data.min.FLT());
   if (data.hasMax)
      nAttr.setMax((float)data.max.FLT());
   if (data.hasSoftMin)
      nAttr.setSoftMin((float)data.softMin.FLT());
   if (data.hasSoftMax)
      nAttr.setSoftMax((float)data.softMax.FLT());
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputRGB(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputRGB(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputRGB(CAttrData& data)
{
   MObject attrib;
   MakeInputRGB(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputRGB(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.RGB().r, data.defaultValue.RGB().g, data.defaultValue.RGB().b);
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputRGBA(MObject& attrib, MObject& attribA, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputRGBA(attrib, attribA, data);
      addAttribute(attrib);
      addAttribute(attribA);
   }
}

void CBaseAttrHelper::MakeInputRGBA(CAttrData& data)
{
   MObject attrib;
   MObject attribA;
   MakeInputRGBA(attrib, attribA, data);
   addAttribute(attrib);
   addAttribute(attribA);
}

void CBaseAttrHelper::MakeInputRGBA(MObject& attrib, MObject& attribA, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.RGBA().r, data.defaultValue.RGBA().g, data.defaultValue.RGBA().b);
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);

   attribA = nAttr.create(data.name + "A", data.shortName + "a", MFnNumericData::kFloat, data.defaultValue.RGBA().a);
   nAttr.setHidden(true);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setMin(0.);
   nAttr.setMax(1.);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputVector(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputVector(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputVector(CAttrData& data)
{
   MObject attrib;
   MakeInputVector(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputVector(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setDefault(data.defaultValue.VEC().x, data.defaultValue.VEC().y, data.defaultValue.VEC().z);
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputVector2(MObject& attrib, MObject& attribX, MObject& attribY, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputVector2(attrib, attribX, attribY, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputVector2(CAttrData& data)
{
   MObject attrib;
   MObject attribX;
   MObject attribY;
   MakeInputVector2(attrib, attribX, attribY, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputVector2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attribX = nAttr.create(data.name + "X", data.shortName + "x", MFnNumericData::kFloat, data.defaultValue.VEC2().x);
   attribY = nAttr.create(data.name + "Y", data.shortName + "y", MFnNumericData::kFloat, data.defaultValue.VEC2().y);
   attrib = nAttr.create(data.name, data.shortName, attribX, attribY);
   nAttr.setDefault(float(data.defaultValue.VEC2().x), float(data.defaultValue.VEC2().y));
   nAttr.setArray(data.isArray);
   nAttr.setKeyable(data.keyable);
   nAttr.setConnectable(data.linkable);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputString(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputString(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputString(CAttrData& data)
{
   MObject attrib;
   MakeInputString(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputString(MObject& attrib, CAttrData& data)
{
   MFnTypedAttribute tAttr;

   attrib = tAttr.create(data.name, data.shortName, MFnData::kString);
   MFnStringData strData;
   MString stringDefault(data.stringDefault);
   MObject defObj = strData.create(stringDefault);
   tAttr.setDefault(defObj);
   tAttr.setArray(data.isArray);
   tAttr.setKeyable(data.keyable);
   tAttr.setConnectable(data.linkable);
   tAttr.setStorable(true);
   tAttr.setReadable(true);
   tAttr.setWritable(true);
   tAttr.setUsedAsFilename(data.usedAsFilename);
   tAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputMatrix(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputMatrix(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputMatrix(CAttrData& data)
{
   MObject attrib;
   MakeInputMatrix(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputMatrix(MObject& attrib, CAttrData& data)
{
   MFnMatrixAttribute mAttr;

   attrib = mAttr.create(data.name, data.shortName, MFnMatrixAttribute::kFloat);
   mAttr.setArray(data.isArray);
   mAttr.setKeyable(data.keyable);
   mAttr.setStorable(true);
   mAttr.setReadable(true);
   mAttr.setWritable(true);

//   const AtParamValue* data.defaultValue = AiParamGetDefault(paramEntry);
//   MFloatMatrix mmat(data.defaultValue.pMTX);
//   mAttr.setDefault(mmat);
}

void CBaseAttrHelper::MakeInputEnum(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputEnum(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputEnum(CAttrData& data)
{
   MObject attrib;
   MakeInputEnum(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputEnum(MObject& attrib, CAttrData& data)
{
   MFnEnumAttribute eAttr;
   attrib = eAttr.create(data.name, data.shortName, data.defaultValue.INT());
   for (unsigned int ei = 0; ei < data.enums.length(); ++ei)
      eAttr.addField(data.enums[ei], ei);
   eAttr.setArray(data.isArray);
   eAttr.setKeyable(data.keyable);
   eAttr.setConnectable(data.linkable);
   eAttr.setStorable(true);
   eAttr.setReadable(true);
   eAttr.setWritable(true);
   eAttr.setChannelBox(data.channelBox);
}

void CBaseAttrHelper::MakeInputNode(MObject& attrib, const char* paramName)
{
   CAttrData data;
   if (GetAttrData(paramName, data))
   {
      MakeInputNode(attrib, data);
      addAttribute(attrib);
   }
}

void CBaseAttrHelper::MakeInputNode(CAttrData& data)
{
   MObject attrib;
   MakeInputNode(attrib, data);
   addAttribute(attrib);
}

void CBaseAttrHelper::MakeInputNode(MObject& attrib, CAttrData& data)
{
   MFnMessageAttribute msgAttr;

   attrib = msgAttr.create(data.name, data.shortName);
   msgAttr.setArray(data.isArray);
   if (data.isArray)
      // no need to keep disconnected plugs around for message attributes
      msgAttr.setDisconnectBehavior(MFnAttribute::kDelete);
   msgAttr.setKeyable(true);
   msgAttr.setStorable(true);
   msgAttr.setReadable(true);
   msgAttr.setWritable(true);
}

void CBaseAttrHelper::MakeInputCompound(CAttrData& data, std::vector<CAttrData>& children)
{
   MObject attrib;
   return MakeInputCompound(attrib, data, children);
}

void CBaseAttrHelper::MakeInputCompound(MObject& attrib, CAttrData& data, std::vector<CAttrData>& children)
{
   MFnCompoundAttribute cmpAttr;

   MObjectArray childObjs;
   for (unsigned int i=0; i < children.size(); i++)
   {
      MObject obj;
      // the following does not call addAttribute, which is essential
      MakeInput(obj, children[i]);
      childObjs.append(obj);
   }
   attrib = cmpAttr.create(data.name, data.shortName);
   cmpAttr.setArray(true);
   for (unsigned int i=0; i < childObjs.length(); i++)
      cmpAttr.addChild(childObjs[i]);

   cmpAttr.setReadable(true);
   cmpAttr.setWritable(true);
   // cmpAttr.setIndexMatters(true);
   CHECK_MSTATUS(addAttribute(attrib));
}

MObject CBaseAttrHelper::MakeInput(const char* paramName)
{
   CAttrData attrData;
   if (GetAttrData(paramName, attrData))
      return MakeInput(attrData);
   else
      return MObject::kNullObj;
}

MObject CBaseAttrHelper::MakeInput(CAttrData& attrData)
{
   MObject input;
   MakeInput(input, attrData);
   if (input != MObject::kNullObj)
      addAttribute(input);
   return input;
}

void CBaseAttrHelper::MakeInput(MObject& input, CAttrData& attrData)
{
   switch (attrData.type)
   {
      case AI_TYPE_INT:
      {
         MakeInputInt(input, attrData);
         break;
      }

      case AI_TYPE_BYTE:
      {
         MakeInputByte(input, attrData);
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
      case AI_TYPE_CLOSURE: // doing the same thing for closures ?
      case AI_TYPE_RGBA:
      {
         MObject inputA;
         MakeInputRGBA(input, inputA, attrData);
         // FIXME: !!!
         addAttribute(inputA);
         m_attributes[std::string(attrData.name.asChar()) + "A"] = inputA;
         break;
      }
      case AI_TYPE_VECTOR:
      {
         MakeInputVector(input, attrData);
         break;
      }
      case AI_TYPE_VECTOR2:
      {
         MObject inputX;
         MObject inputY;

         MakeInputVector2(input, inputX, inputY, attrData);
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
      case AI_TYPE_POINTER:
      {
         const char* typeName = AiParamGetTypeName(attrData.type);
         AiMsgWarning("[mtoa.attr] Unable to create input attribute \"%s\": parameters of type %s are not supported", attrData.name.asChar(), typeName);
         return;
      }
      case AI_TYPE_UNDEFINED: // same as AI_TYPE_NONE
      {
         if (m_nodeEntry == NULL)
            AiMsgError("[mtoa.attr] Unable to create attribute \"%s\": parameters type is undefined", attrData.name.asChar());
         else
            AiMsgError("[mtoa.attr] Unable to create attribute \"%s\" from arnold node \"%s\": parameters type is undefined",
                       attrData.name.asChar(), AiNodeEntryGetName(m_nodeEntry));
         input = MObject::kNullObj;
         return;
      }
      default:
      {
         AiMsgError("[mtoa.attr] Unable to create input attribute \"%s\": unknown parameter type", attrData.name.asChar());
         input = MObject::kNullObj;
         return;
      }
   } // switch
   m_attributes[attrData.name.asChar()] = input;
}

// Outputs

void CBaseAttrHelper::MakeOutputInt(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kInt);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputBoolean(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;
   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kBoolean);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputFloat(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.create(data.name, data.shortName, MFnNumericData::kFloat);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGB(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, data.shortName);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputRGBA(MObject& attrib, MObject& attribA, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createColor(OUT_COLOR_NAME, data.shortName);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);

   attribA = nAttr.create(OUT_ALPHA_NAME, data.shortName + "a", MFnNumericData::kFloat);
   nAttr.setHidden(true);
   MAKE_OUTPUT(nAttr, attribA);
}

void CBaseAttrHelper::MakeOutputVector(MObject& attrib, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attrib = nAttr.createPoint(data.name, data.shortName);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}


void CBaseAttrHelper::MakeOutputVector2(MObject& attrib, MObject& attribX, MObject& attribY, CAttrData& data)
{
   MFnNumericAttribute nAttr;

   attribX = nAttr.create(data.name + "X", data.shortName + "x", MFnNumericData::kFloat);
   attribY = nAttr.create(data.name + "Y", data.shortName + "y", MFnNumericData::kFloat);
   attrib = nAttr.create(data.name, data.shortName, attribX, attribY);
   nAttr.setArray(data.isArray);
   MAKE_OUTPUT(nAttr, attrib);
}

void CBaseAttrHelper::MakeOutputString(MObject& attrib, CAttrData& data)
{
   MFnTypedAttribute tAttr;

   attrib = tAttr.create(data.name, data.shortName, MFnData::kString);
   MFnStringData strData;
   MObject defObj = strData.create();
   tAttr.setDefault(defObj);
   tAttr.setArray(data.isArray);
   MAKE_OUTPUT(tAttr, attrib);
}

void CBaseAttrHelper::MakeOutputMatrix(MObject& attrib, CAttrData& data)
{
   MFnMatrixAttribute mAttr;

   attrib = mAttr.create(data.name, data.shortName, MFnMatrixAttribute::kFloat);
   mAttr.setArray(data.isArray);
   MAKE_OUTPUT(mAttr, attrib);
}

void CBaseAttrHelper::MakeOutputNode(MObject& attrib, CAttrData& data)
{
   MFnMessageAttribute msgAttr;

   attrib = msgAttr.create(data.name, data.shortName);
   msgAttr.setArray(data.isArray);
   MAKE_OUTPUT(msgAttr, attrib);
}

MObject CBaseAttrHelper::MakeOutput()
{
   MObject output;

   if (m_nodeEntry == NULL)
   {
      AiMsgError("[mtoa.attr] Cannot retrieve output metadata from a null node entry.");
      return output;
   }
   
   CAttrData data;
   data.isArray = false;
   if (!AiMetaDataGetInt(m_nodeEntry, NULL, "maya.output", &data.type))
   {
      data.type = AiNodeEntryGetOutputType(m_nodeEntry);
   }
   const char* typeName = AiParamGetTypeName(data.type);
   AtString attrName;

   if (AiMetaDataGetStr(m_nodeEntry, NULL, "maya.output_name", &attrName))
   {
      data.name = MString(attrName);
   }
   else
   {
      if (AI_TYPE_RGB == data.type
            || AI_TYPE_RGBA == data.type || AI_TYPE_CLOSURE == data.type)
      {
         data.name = OUT_COLOR_NAME;
      }
      else
      {
         data.name = OUT_NAME;
      }
   }
   AtString attrShortName;
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
      case AI_TYPE_CLOSURE: // do we want to do something different for closures ?
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
      case AI_TYPE_VECTOR2:
      {
         MObject outputX;
         MObject outputY;

         MakeOutputVector2(output, outputX, outputY, data);
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
         AiMsgWarning("[mtoa.attr] Unable to create output attribute \"%s\": parameters of type %s are not supported", data.name.asChar(), typeName);
         return MObject::kNullObj;
      }
      default:
      {
         AiMsgError("[mtoa.attr] Unable to create output attribute \"%s\": unknown parameter type", data.name.asChar());
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
      AtString tmp;
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
      AiMsgError("[mtoa.attr] Unable to create static attribute corresponding to %s.%s", AiNodeEntryGetName(m_nodeEntry), MFnAttribute(attrib).name().asChar());
   }
   else
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.attr] Added static attribute "+MString(AiNodeEntryGetName(m_nodeEntry))+"."+ MFnAttribute(attrib).name());
   }
   CHECK_MSTATUS(stat);
   return stat;
}


// LEGACY
// We're keeping this to avoid breaking binary compatibility
MStatus CDynamicAttrHelper::addAttribute(MObject& attrib)
{
   return MS::kFailure;
}

// CExtensionAttrHelper
//

void CExtensionAttrHelper::AddCommonAttributes()
{
   CAttrData data;

   data.name = "aiUserOptions";
   data.shortName = "ai_user_options";
   data.stringDefault = "";
   MakeInputString(data);
}

MStatus CExtensionAttrHelper::addAttribute(MObject& attrib)
{
   return CExtensionsManager::RegisterExtensionAttribute(m_class, attrib);
}
