#include <ai.h>

#define ARNOLD_NODEID_USERDATA_BOOL             0x00115D18

AI_SHADER_NODE_EXPORT_METHODS(UserDataBoolMtd);

namespace
{
   enum UserDataBoolParams
   {
      p_boolAttrName
   };
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataBool");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USERDATA_BOOL);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("boolAttrName", "");
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
   const AtChar *name = AiShaderEvalParamStr(p_boolAttrName);

   AtBoolean value = FALSE;
   if (AiUDataGetBool(name, &value))
      sg->out.BOOL = value;
   else
      sg->out.BOOL = FALSE;
}

