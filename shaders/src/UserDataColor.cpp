#include <ai.h>
#include <string.h>



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

   AiParameterSTR("colorAttrName", "");

   AiMetaDataSetBool(mds, NULL, "maya.hide", false);
   //AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D0D);  // setting this  fails on load?
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataColor");
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
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
   AtFloat f; // special case remapping  transparency as a color to / opacityPP as a float

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
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
   }
}
