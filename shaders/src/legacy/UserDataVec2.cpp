#include <ai.h>

#define ARNOLD_NODEID_USER_DATA_VEC2            0x00115D1C

AI_SHADER_NODE_EXPORT_METHODS(UserDataVec2Mtd);

namespace
{

enum UserDataVec2Params
{
   p_vec2AttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(nentry, NULL, "_synonym", "userDataVec2");
   // AiMetaDataSetStr(nentry, NULL, "maya.name", "aiUserDataVec2");
   // AiMetaDataSetInt(nentry, NULL, "maya.id", ARNOLD_NODEID_USER_DATA_VEC2);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "utility/user data");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);

   AiParameterStr("vec2AttrName", "");
   AiParameterVec2("defaultValue", 0.f, 0.f);

   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
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
   AtString name = AtString(AiShaderEvalParamStr(p_vec2AttrName));
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

   if (AiUDataGetVec2(name, val.VEC2()))
   {
      valid = true;
      sg->out.VEC2().x = val.VEC2().x;
      sg->out.VEC2().y = val.VEC2().y;
   } else
   {   
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(name, sg);
      int valueType = AiUserParamGetType(pentry);

      switch (valueType)
      {
         case AI_TYPE_FLOAT:
         if (AiUDataGetFlt(name, val.FLT()))
         {
            valid = true;
            sg->out.VEC2().x = val.FLT();
            sg->out.VEC2().y = val.FLT();
         }
         break;
         case AI_TYPE_RGB:
         if (AiUDataGetRGB(name, val.RGB()))
         {
            valid = true;
            sg->out.VEC2().x = val.RGB().r;
            sg->out.VEC2().y = val.RGB().g;
         }
         break;
         case AI_TYPE_RGBA:
         if (AiUDataGetRGBA(name, val.RGBA()))
         {
            valid = true;
            sg->out.VEC2().x = val.RGBA().r;
            sg->out.VEC2().y = val.RGBA().g;
         }
         break;
         case AI_TYPE_BYTE:
         if (AiUDataGetByte(name, val.BYTE()))
         {
            valid = true;
            sg->out.VEC2().x = sg->out.VEC2().y = (float)val.BYTE();			
         }
         break;
         case AI_TYPE_INT:
         if (AiUDataGetInt(name, val.INT()))
         {
            valid = true;
            sg->out.VEC2().x = sg->out.VEC2().y = (float)val.INT();			
         }		 
         break;
         case AI_TYPE_UINT:
         if (AiUDataGetUInt(name, val.UINT()))
         {
            valid = true;
            sg->out.VEC2().x = sg->out.VEC2().y = (float)val.UINT();			
         }
         break;
         case AI_TYPE_BOOLEAN:
         if (AiUDataGetBool(name, val.BOOL()))
         {
            valid = true;
            sg->out.VEC2().x = sg->out.VEC2().y = (val.BOOL() ? 1.f : 0.f);			
         }
         break;
         case AI_TYPE_VECTOR:
         if (AiUDataGetVec(name, val.VEC()))
         {
            valid = true;
            sg->out.VEC2().x = val.VEC().x;
            sg->out.VEC2().y = val.VEC().y;
         }
         break;
         case AI_TYPE_VECTOR2:
         if (AiUDataGetVec2(name, val.VEC2()))
         {
            valid = true;
            sg->out.VEC2().x = val.VEC2().x;
            sg->out.VEC2().y = val.VEC2().y;
         }
         break;
      }
   }
   if (!valid)
   {
      sg->out.VEC2() = AiShaderEvalParamVec2(p_defaultValue);
   }
}
