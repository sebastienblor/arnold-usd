#include <ai.h>


AI_SHADER_NODE_EXPORT_METHODS(UserDataFloatMtd);

namespace
{

enum UserDataFloatParams
{
   p_floatAttrName
};
}

node_parameters
{

   AiParameterSTR("floatAttrName", "");

   AiMetaDataSetBool(mds, NULL, "maya.hide", false);
   //AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D0C);  // setting this  fails on load?
   AiMetaDataSetStr(mds, NULL, "maya.name", "userDataFloat");
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

   AtFloat f;


   name = AiShaderEvalParamStr(p_floatAttrName);
   if (AiUDataGetFlt(name, &f))
   {
      sg->out.FLT = f;

   }
   else
   {
      sg->out.FLT = 0;
   }
}
