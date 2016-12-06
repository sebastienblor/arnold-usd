#include "Metadata.h"

#include <ai_msg.h>

#include <maya/MGlobal.h>

#include <cstring>

AtParamValue MAiParamGetDefault(const AtNodeEntry *entry, const AtParamEntry* paramEntry)
{
   const char* param = AiParamGetName(paramEntry);
   const AtParamValue* real = AiParamGetDefault(paramEntry);
   AtParamValue value;
   memcpy(&value, real, sizeof(AtParamValue));

   int type = AiParamGetType(paramEntry);

   bool isArray = false;
   if (type == AI_TYPE_ARRAY)
   {
      type = AiArrayGetType(value.ARRAY());
      isArray = true;
   }
   switch (type)
   {
      case AI_TYPE_ENUM:
      case AI_TYPE_INT:
      {
         int result;
         if (AiMetaDataGetInt(entry, param, "default", &result))
         {
               value.INT() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.INT() = AiArrayGetInt(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_UINT:
      {
         int result;
         if (AiMetaDataGetInt(entry, param, "default", &result))
         {
               value.UINT() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.UINT() = AiArrayGetUInt(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_BOOLEAN:
      {
         bool result;
         if (AiMetaDataGetBool(entry, param, "default", &result))
         {
               value.BOOL() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.BOOL() = AiArrayGetBool(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_FLOAT:
      {
         float result;
         if (AiMetaDataGetFlt(entry, param, "default", &result))
         {
               value.FLT() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.FLT() = AiArrayGetFlt(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_VECTOR:
      {
         AtVector result;
         if (AiMetaDataGetVec(entry, param, "default", &result))
         {
               value.VEC() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.VEC() = AiArrayGetVec(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_VECTOR2:
      {
         AtVector2 result;
         if (AiMetaDataGetVec2(entry, param, "default", &result))
         {
               value.VEC2() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.VEC2() = AiArrayGetVec2(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
      {
         AtRGB result;
         if (AiMetaDataGetRGB(entry, param, "default", &result))
         {
               value.RGB() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.RGB() = AiArrayGetRGB(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_STRING:
      {
         AtString result;
         if (AiMetaDataGetStr(entry, param, "default", &result))
         {
               value.STR() = result;
         }
         else if (isArray && AiArrayGetNumElements(value.ARRAY()) > 0)
         {
               value.STR() = AiArrayGetStr(value.ARRAY(), 0);
         }
         break;
      }
      case AI_TYPE_BYTE:
      case AI_TYPE_POINTER:
      case AI_TYPE_ARRAY:
      case AI_TYPE_NODE:
      case AI_TYPE_MATRIX:
         break;
      default:
      {
         AiMsgError("[mtoa] Cannot override default value of type %s for param %s on node %s", AiParamGetTypeName(type), param, AiNodeEntryGetName(entry));
      }
   }

   return value;
}
