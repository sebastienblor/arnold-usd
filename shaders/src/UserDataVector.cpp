#include <ai.h>

#define ARNOLD_NODEID_USERDATAVECTOR            0x00115D13

AI_SHADER_NODE_EXPORT_METHODS(UserDataVectorMtd);

namespace
{

enum UserDataVectorParams
{
   p_vectorAttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataVector");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATAVECTOR);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("vectorAttrName", "");
   AiParameterVEC("defaultValue", 0.f, 0.f, 0.f);
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

   AtVector v;
   AtRGB c;
   AtFloat f;

   name = AiShaderEvalParamStr(p_vectorAttrName);
   if (AiUDataGetVec(name, &v))
   {
      sg->out.RGB.r = v.x;
      sg->out.RGB.g = v.y;
      sg->out.RGB.b = v.z;

   }
   else if (AiUDataGetRGB(name, &c))
   {
      sg->out.RGB.r = c.r;
      sg->out.RGB.g = c.g;
      sg->out.RGB.b = c.b;
   }
   else if (AiUDataGetFlt(name, &f))
   {
      sg->out.RGB.r = f;
      sg->out.RGB.g = f;
      sg->out.RGB.b = f;
   }
   else
   {
      sg->out.VEC = AiShaderEvalParamVec(p_defaultValue);
   }
}
