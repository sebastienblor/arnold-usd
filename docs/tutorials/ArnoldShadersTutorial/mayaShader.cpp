#include <stdio.h>
#include <cstring>
#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaSimpleMethods);

enum MayaSimpleParams { p_color };

node_parameters
{
   // Node metadata
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiSimple");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00070000);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetStr(mds, NULL, "maya.output_name", "outColor");
   AiMetaDataSetStr(mds, NULL, "maya.output_shortname", "out");

   AiParameterRGB("constantColor", 0.7f, 0.7f, 0.7f);
   AiMetaDataSetStr(mds, "constantColor", "maya.name", "color");
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
   AtColor color = AiShaderEvalParamRGB(p_color);
   sg->out.RGB = color;
}

node_loader
{
   if (i > 0)
      return false;

   node->methods      = MayaSimpleMethods;
   node->output_type  = AI_TYPE_RGB;
   node->name         = "simple";
   node->node_type    = AI_NODE_SHADER;
   strcpy(node->version, AI_VERSION);
   return true;
}
