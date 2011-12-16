#include <ai.h>
#include <string.h>

#define ARNOLD_NODEID_USERDATACOLOR             0x00115D14

AI_SHADER_NODE_EXPORT_METHODS(UserDataColorMtd);

namespace
{

enum UserDataColorParams
{
   p_colorAttrName
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataColor");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATACOLOR);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", FALSE);

   AiParameterSTR("colorAttrName", "");
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

   const AtChar *name = 0;
   AtChar transp[] = "opacityPP";

   AtRGB c;
   AtVector v;
   AtFloat f;

   name = AiShaderEvalParamStr(p_colorAttrName);

   if (strcmp (name, transp) == 0)
   {
      if (AiUDataGetFlt(name, &f))
      {
         sg->out.RGB.r = f;
         sg->out.RGB.g = f;
         sg->out.RGB.b = f;
      }
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
   }
   else
   {
      if (AiUDataGetRGB(name, &c))
      {
         sg->out.RGB.r = c.r;
         sg->out.RGB.g = c.g;
         sg->out.RGB.b = c.b;

      }
      else if (AiUDataGetVec(name, &v))
      {
         sg->out.RGB.r = v.x;
         sg->out.RGB.g = v.y;
         sg->out.RGB.b = v.z;
      }
      else if (AiUDataGetFlt(name, &f))
      {
         sg->out.RGB.r = f;
         sg->out.RGB.g = f;
         sg->out.RGB.b = f;
      }
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
   }
}
