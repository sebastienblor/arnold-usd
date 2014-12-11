#include <ai.h>
#include <string.h>

#define ARNOLD_NODEID_USERDATACOLOR             0x00115D14

AI_SHADER_NODE_EXPORT_METHODS(UserDataColorMtd);

namespace
{

enum UserDataColorParams
{
   p_colorAttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataColor");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATACOLOR);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("colorAttrName", "");
   AiParameterRGB("defaultValue", 0.f, 0.f, 0.f);
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
   const char *name = 0;
   char transp[] = "opacityPP";
  
   name = AiShaderEvalParamStr(p_colorAttrName);
   AtParamValue val;
   bool valid = false;	
   
   if (strcmp (name, transp) == 0)
   {
      if (AiUDataGetFlt(name, &val.FLT))
      {
	     valid = true;
         sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = val.FLT;
      }
   }
   else 
   {
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(name, sg);
      int valueType = AiUserParamGetType(pentry);

      switch (valueType)
      {
         case AI_TYPE_FLOAT:
         if (AiUDataGetFlt(name, &val.FLT))
         {
            valid = true;
            sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = val.FLT;
         }
         break;
         case AI_TYPE_RGB:
         if (AiUDataGetRGB(name, &val.RGB))
         {
            valid = true;
            sg->out.RGB.r = val.RGB.r;
            sg->out.RGB.g = val.RGB.g;
            sg->out.RGB.b = val.RGB.b;
         }
         break;
         case AI_TYPE_RGBA:
         if (AiUDataGetRGBA(name, &val.RGBA))
         {
            valid = true;
            sg->out.RGB.r = val.RGBA.r;
            sg->out.RGB.g = val.RGBA.g;
            sg->out.RGB.b = val.RGBA.b;
         }
         break;
         case AI_TYPE_BYTE:
         if (AiUDataGetByte(name, &val.BYTE))
   	     {
            valid = true;
            sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = (float)val.BYTE;			
         }
         break;
         case AI_TYPE_INT:
         if (AiUDataGetInt(name, &val.INT))
   	     {
            valid = true;
            sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = (float)val.INT;			
         }		 
         break;
         case AI_TYPE_UINT:
         if (AiUDataGetUInt(name, &val.UINT))
         {
            valid = true;
            sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = (float)val.UINT;			
         }
         break;
         case AI_TYPE_BOOLEAN:
         if (AiUDataGetBool(name, &val.BOOL))
   	     {
            valid = true;
            sg->out.RGB.r = sg->out.RGB.g = sg->out.RGB.b = (val.BOOL ? 1.f : 0.f);			
         }
         break;
         case AI_TYPE_POINT:
         if (AiUDataGetVec(name, &val.PNT))
   	     {
            valid = true;
            sg->out.RGB.r = val.PNT.x;
            sg->out.RGB.g = val.PNT.y;
            sg->out.RGB.b = val.PNT.z;
         }
         break;
         case AI_TYPE_POINT2:
         if (AiUDataGetPnt2(name, &val.PNT2))
   	     {
            valid = true;
            sg->out.RGB.r = val.PNT2.x;
            sg->out.RGB.g = val.PNT2.y;
            sg->out.RGB.b = 0.f;
         }
         break;
         case AI_TYPE_VECTOR:
         if (AiUDataGetVec(name, &val.VEC))
         {
            valid = true;
            sg->out.RGB.r = val.VEC.x;
            sg->out.RGB.g = val.VEC.y;
            sg->out.RGB.b = val.VEC.z;
         }
         break;

         default:
         break;
      }
   }
   if (!valid)
   {
      sg->out.RGB = AiShaderEvalParamRGB(p_defaultValue);
   }
}
