#include <ai.h>

#define ARNOLD_NODEID_USER_DATA_PNT2            0x00115D1C

AI_SHADER_NODE_EXPORT_METHODS(UserDataPnt2Mtd);

namespace
{

enum UserDataPnt2Params
{
   p_pnt2AttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataPnt2");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USER_DATA_PNT2);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("pnt2AttrName", "");
   AiParameterPNT2("defaultValue", 0.f, 0.f);
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
   name = AiShaderEvalParamStr(p_pnt2AttrName);
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

   if (AiUDataGetPnt2(name, &val.PNT2))
   {
      valid = true;
      sg->out.PNT2.x = val.PNT2.x;
      sg->out.PNT2.y = val.PNT2.y;
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
            sg->out.PNT2.x = val.FLT;
            sg->out.PNT2.y = val.FLT;
         }
         break;
         case AI_TYPE_RGB:
         if (AiUDataGetRGB(name, &val.RGB))
         {
            valid = true;
            sg->out.PNT2.x = val.RGB.r;
            sg->out.PNT2.y = val.RGB.g;
         }
         break;
         case AI_TYPE_RGBA:
         if (AiUDataGetRGBA(name, &val.RGBA))
         {
            valid = true;
            sg->out.PNT2.x = val.RGBA.r;
            sg->out.PNT2.y = val.RGBA.g;
         }
         break;
         case AI_TYPE_BYTE:
         if (AiUDataGetByte(name, &val.BYTE))
         {
            valid = true;
            sg->out.PNT2.x = sg->out.PNT2.y = (float)val.BYTE;			
         }
         break;
         case AI_TYPE_INT:
         if (AiUDataGetInt(name, &val.INT))
         {
            valid = true;
            sg->out.PNT2.x = sg->out.PNT2.y = (float)val.INT;			
         }		 
         break;
         case AI_TYPE_UINT:
         if (AiUDataGetUInt(name, &val.UINT))
         {
            valid = true;
            sg->out.PNT2.x = sg->out.PNT2.y = (float)val.UINT;			
         }
         break;
         case AI_TYPE_BOOLEAN:
         if (AiUDataGetBool(name, &val.BOOL))
         {
            valid = true;
            sg->out.PNT2.x = sg->out.PNT2.y = (val.BOOL ? 1.f : 0.f);			
         }
         break;
         case AI_TYPE_POINT:
         if (AiUDataGetPnt(name, &val.PNT))
         {
            valid = true;
            sg->out.PNT2.x = val.PNT.x;
            sg->out.PNT2.y = val.PNT.y;
         }
         break;
		 case AI_TYPE_VECTOR:
         if (AiUDataGetVec(name, &val.VEC))
         {
            valid = true;
            sg->out.PNT2.x = val.VEC.x;
            sg->out.PNT2.y = val.VEC.y;
         }
         break;
         case AI_TYPE_POINT2:
         if (AiUDataGetPnt2(name, &val.PNT2))
         {
            valid = true;
            sg->out.PNT2.x = val.PNT2.x;
            sg->out.PNT2.y = val.PNT2.y;
         }
         break;
      }
   }
   if (!valid)
   {
      sg->out.PNT2 = AiShaderEvalParamPnt2(p_defaultValue);
   }
}

