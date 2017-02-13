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
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiUserDataString");
   AiMetaDataSetInt(nentry, NULL, "maya.id", ARNOLD_NODEID_USERDATASTRING);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "utility/user data");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);

   AiParameterStr("stringAttrName", "");
   AiParameterStr("defaultValue", "")
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
   AtString name = AtString(AiShaderEvalParamStr(p_stringAttrName));
   AtString value;

   if (AiUDataGetStr(name, value))
      sg->out.STR() = value;
   else
      sg->out.STR() = AtString(AiShaderEvalParamStr(p_defaultValue));

}
