#include <ai.h>

#define ARNOLD_NODEID_USERDATASTRING            0x00115D15

AI_SHADER_NODE_EXPORT_METHODS(UserDataStringMtd);

namespace
{

enum UserDataStringParams
{
   p_stringAttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataString");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATASTRING);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("stringAttrName", "");
   AiParameterSTR("defaultValue", "")
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
   const AtChar *value = 0;

   name = AiShaderEvalParamStr(p_stringAttrName);
   if (AiUDataGetStr(name, &value))
      sg->out.STR = value;
   else
      sg->out.STR = AiShaderEvalParamStr(p_defaultValue);

}
