#include <ai.h>

#define ARNOLD_NODEID_USERDATAFLOAT             0x00115D12

AI_SHADER_NODE_EXPORT_METHODS(UserDataFloatMtd);

namespace
{

enum UserDataFloatParams
{
   p_floatAttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiUserDataFloat");
   AiMetaDataSetInt(nentry, NULL, "maya.id", ARNOLD_NODEID_USERDATAFLOAT);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);

   AiParameterStr("floatAttrName", "");
   AiParameterFlt("defaultValue", 0.f);
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
   AtString name = AtString(AiShaderEvalParamStr(p_floatAttrName));
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

   if (AiUDataGetFlt(name, val.FLT()))
   {
      valid = true;
      sg->out.FLT() = val.FLT();
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
            sg->out.FLT() = val.FLT();
         }
         break;
         case AI_TYPE_RGB:
         if (AiUDataGetRGB(name, val.RGB()))
         {
            valid = true;
            sg->out.FLT() = (val.RGB().r + val.RGB().g + val.RGB().b) / 3.f;
         }
         break;
         case AI_TYPE_RGBA:
         if (AiUDataGetRGBA(name, val.RGBA()))
         {
            valid = true;
            sg->out.FLT() = (val.RGBA().r + val.RGBA().g + val.RGBA().b) / 3.f;
         }
         break;
         case AI_TYPE_BYTE:
         if (AiUDataGetByte(name, val.BYTE()))
         {
            valid = true;
            sg->out.FLT() = (float)val.BYTE();			
         }
         break;
         case AI_TYPE_INT:
         if (AiUDataGetInt(name, val.INT()))
         {
            valid = true;
            sg->out.FLT() = (float)val.INT();			
         }		 
         break;
         case AI_TYPE_UINT:
         if (AiUDataGetUInt(name, val.UINT()))
         {
            valid = true;
            sg->out.FLT() = (float)val.UINT();			
         }
         break;
         case AI_TYPE_BOOLEAN:
         if (AiUDataGetBool(name, val.BOOL()))
         {
            valid = true;
            sg->out.FLT() = (val.BOOL() ? 1.f : 0.f);			
         }
         break;
         case AI_TYPE_VECTOR2:
         if (AiUDataGetVec2(name, val.VEC2()))
         {
            valid = true;
            sg->out.FLT() = (val.VEC2().x + val.VEC2().y) / 2.f;
         }
         break;
         case AI_TYPE_VECTOR:
         if (AiUDataGetVec(name, val.VEC()))
         {
            valid = true;
            sg->out.FLT() = (val.VEC().x + val.VEC().y + val.VEC().z) / 3.f;
         }
         break;

         default:
         break;
      }
   }
   if (!valid)
   {
      sg->out.FLT() = AiShaderEvalParamFlt(p_defaultValue);
   }
}
