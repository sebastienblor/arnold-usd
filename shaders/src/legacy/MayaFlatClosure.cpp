#include <ai.h>

namespace
{

enum MayaFlatClosureShaderParams
{
   p_outColor
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaFlatClosureMtd);

node_parameters
{
   AiParameterRGB("color", 1.0f, 1.0f, 1.0f);

   //AiMetaDataSetStr(nentry, NULL, "maya.name", "MayaFlatClosure");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52535983);
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
   AtRGB color = AiShaderEvalParamRGB(p_outColor);
   AtClosureList closures;
   closures.add(AiClosureEmission(sg, color));
   sg->out.CLOSURE() = closures;
}
