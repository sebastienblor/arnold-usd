#include <ai.h>

#define ARNOLD_NODEID_USERDATA_BOOL             0x00115D18

AI_SHADER_NODE_EXPORT_METHODS(UserDataBoolMtd);

namespace
{
   enum UserDataBoolParams
   {
      p_boolAttrName,
      p_defaultValue
   };
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataBool");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATA_BOOL);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("boolAttrName", "");
   AiParameterBOOL("defaultValue", false);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   const char* name = 0;
   name = AiShaderEvalParamStr(p_boolAttrName);
   AtParamValue val;
   bool valid = false;
   
   // As of today, if we want to handle the data conversion properly,
   // it requires 2 calls to userData functions using the attribute name (which is costly)
   // 1 to get the UserData type, 2 to get the value.
   // So, compared to previous versions, supporting this would make it longer
   // for those using the appropriate type (single call).
   // So here's the compromise we chose :
   // 1) we try to get the value of the CORRECT type
   // 2) if it doesn't succeed, then get the type and the value

   if (AiUDataGetBool(name, &val.BOOL))
   {
      valid = true;
      sg->out.BOOL = val.BOOL;
   } else
   {   
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(name, sg);
      int valueType = AiUserParamGetType(pentry);

      switch (valueType)
      {
         case AI_TYPE_FLOAT:
         if (AiUDataGetFlt(name, &val.FLT))
         {
            valid = true;
            sg->out.BOOL = (fabs(val.FLT) < AI_EPSILON);
         }
         break;
         case AI_TYPE_RGB:
         if (AiUDataGetRGB(name, &val.RGB))
         {
            valid = true;
            sg->out.BOOL =
               ((fabs(val.RGB.r) > AI_EPSILON) || 
               (fabs(val.RGB.g) > AI_EPSILON) ||
               (fabs(val.RGB.b) > AI_EPSILON));
         }
         break;
         case AI_TYPE_RGBA:
         if (AiUDataGetRGBA(name, &val.RGBA))
         {
            valid = true;
            sg->out.BOOL =
               ((fabs(val.RGBA.r) > AI_EPSILON) ||
               (fabs(val.RGBA.g) > AI_EPSILON) ||
               (fabs(val.RGBA.b) > AI_EPSILON));
         }
         break;
         case AI_TYPE_BYTE:
         if (AiUDataGetByte(name, &val.BYTE))
         {
            valid = true;
            sg->out.BOOL = (val.BYTE != 0);			
         }
         break;
         case AI_TYPE_INT:
         if (AiUDataGetInt(name, &val.INT))
         {
            valid = true;
            sg->out.BOOL = (val.INT != 0);			
         }		 
         break;
         case AI_TYPE_UINT:
         if (AiUDataGetUInt(name, &val.UINT))
         {
            valid = true;
            sg->out.BOOL = (val.UINT != 0);
         }
         break;
         case AI_TYPE_BOOLEAN:
         if (AiUDataGetBool(name, &val.BOOL))
         {
            valid = true;
            sg->out.BOOL = val.BOOL;			
         }
         break;
         case AI_TYPE_POINT:
         if (AiUDataGetPnt(name, &val.PNT))
         {
            valid = true;
            sg->out.BOOL = 
               ((fabs(val.PNT.x) > AI_EPSILON) || 
               (fabs(val.PNT.y) > AI_EPSILON) ||
               (fabs(val.PNT.z) > AI_EPSILON));
         }
         break;
         case AI_TYPE_POINT2:
         if (AiUDataGetPnt2(name, &val.PNT2))
         {
            valid = true;
            sg->out.BOOL = 
               ((fabs(val.PNT2.x) > AI_EPSILON) || 
               (fabs(val.PNT2.y) > AI_EPSILON));
         }
         break;
         default:
         break;
      }
   }
   if (!valid)
   {
      sg->out.BOOL = AiShaderEvalParamBool(p_defaultValue);
   }
}

