#include <ai.h>


AI_SHADER_NODE_EXPORT_METHODS(UserDataVectorMtd);

namespace
{

enum UserDataVectorParams
{
   p_vectorAttrName
};
}

node_parameters
{

   AiParameterSTR("vectorAttrName", "");

   AiMetaDataSetBool(mds, NULL, "maya.hide", false);
   //AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D0E);  // setting this  fails on load?
   AiMetaDataSetStr(mds, NULL, "maya.name", "userDataVector");
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

   AtVector v;


   name = AiShaderEvalParamStr(p_vectorAttrName);
   if (AiUDataGetVec(name, &v))
   {
      sg->out.RGB.r = v.x;
      sg->out.RGB.g = v.y;
      sg->out.RGB.b = v.z;

   }
   else
   {
      sg->out.RGB = AI_RGB_BLACK;
   }
}
