#include <ai.h>

#define ARNOLD_NODEID_USER_DATA_INT            0x00115D1D

AI_SHADER_NODE_EXPORT_METHODS(UserDataIntMtd);

namespace
{

enum UserDataPnt2Params
{
   p_intAttrName,
   p_defaultValue
};
}

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiUserDataInt");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_USER_DATA_INT);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterSTR("intAttrName", "");
   AiParameterINT("defaultValue", 0);
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
   int i;
   if (AiUDataGetInt(AiShaderEvalParamStr(p_intAttrName), &i))
   {
      sg->out.INT = i;
   }
   else
   {
      sg->out.INT = AiShaderEvalParamInt(p_defaultValue);
   }
}


