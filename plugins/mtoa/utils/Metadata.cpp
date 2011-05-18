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

   AtInt type = AiParamGetType(paramEntry);

   bool isArray = false;
   if (type == AI_TYPE_ARRAY)
   {
      type = value.ARRAY->type;
      isArray = true;
   }
   switch (type)
   {
      case AI_TYPE_INT:
      {
         AtInt result;
         if (AiMetaDataGetInt(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetInt(value.ARRAY, 0, result);
            else
               value.INT = result;
         }
         break;
      }
      case AI_TYPE_UINT:
      {
         AtInt result;
         if (AiMetaDataGetInt(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetUInt(value.ARRAY, 0, result);
            else
               value.UINT = result;
         }
         break;
      }
      case AI_TYPE_BOOLEAN:
      {
         AtBoolean result;
         if (AiMetaDataGetBool(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetBool(value.ARRAY, 0, result);
            else
               value.BOOL = result;
         }
         break;
      }
      case AI_TYPE_FLOAT:
      {
         AtFloat result;
         if (AiMetaDataGetFlt(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetFlt(value.ARRAY, 0, result);
            else
               value.FLT = result;
         }
         break;
      }
      case AI_TYPE_POINT:
      {
         AtPoint result;
         if (AiMetaDataGetPnt(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetPnt(value.ARRAY, 0, result);
            else
               value.PNT = result;
         }
         break;
      }
      case AI_TYPE_VECTOR:
      {
         AtVector result;
         if (AiMetaDataGetVec(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetVec(value.ARRAY, 0, result);
            else
               value.VEC = result;
         }
         break;
      }
      case AI_TYPE_POINT2:
      {
         AtPoint2 result;
         if (AiMetaDataGetPnt2(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetPnt2(value.ARRAY, 0, result);
            else
               value.PNT2 = result;
         }
         break;
      }
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
      {
         AtRGB result;
         if (AiMetaDataGetRGB(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetRGB(value.ARRAY, 0, result);
            else
               value.RGB = result;
         }
         break;
      }
      case AI_TYPE_STRING:
      {
         const char* result;
         if (AiMetaDataGetStr(entry, param, "default", &result))
         {
            if (isArray)
               AiArraySetStr(value.ARRAY, 0, result);
            else
               value.STR = result;
         }
         break;
      }
      case AI_TYPE_ENUM:
      case AI_TYPE_NODE:
      case AI_TYPE_MATRIX:
         break;
      default:
      {
         AiMsgError("[mtoa] Cannot override default value of type %s", AiParamGetTypeName(type));
      }
   }

   return value;
}
