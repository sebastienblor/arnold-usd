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
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataFloat");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATAFLOAT);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("floatAttrName", "");
   AiParameterFLT("defaultValue", 0.f);
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

   float f;

   name = AiShaderEvalParamStr(p_floatAttrName);
   if (AiUDataGetFlt(name, &f))
   {
      sg->out.FLT = f;
   }
   else
   {
      sg->out.FLT = AiShaderEvalParamFlt(p_defaultValue);
   }
}
