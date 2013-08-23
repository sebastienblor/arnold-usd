#include <ai.h>
#include <vector>

AI_SHADER_NODE_EXPORT_METHODS(MayaTripleShadingSwitchMtd);

node_parameters
{
	AiParameterRGB("default", 0.8f, 0.8f, 0.8f);
   AiParameterARRAY("inputs", AiArrayAllocate(0, 1, AI_TYPE_NODE));
   AiParameterArray("shapes", AiArrayAllocate(0, 1, AI_TYPE_NODE));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

struct MayaTripleShadingSwitchData{
   std::vector<AtNode*> inputs;
   std::vector<AtNode*> shapes;
};

enum MayaTripleShadingSwitchParams{
   p_default = 0,
   p_inputs,
   p_shapes
};

node_initialize
{
   AiNodeSetLocalData(node, new MayaTripleShadingSwitchData);
}

node_update
{
   MayaTripleShadingSwitchData* data = (MayaTripleShadingSwitchData*)AiNodeGetLocalData(node);
   data->inputs.clear();
   data->shapes.clear();
   AtArray* inputs = AiNodeGetArray(node, "inputs");
   AtArray* shapes = AiNodeGetArray(node, "shapes");
   if (inputs->nelements == 0)
      return;
   data->inputs.reserve(inputs->nelements);
   data->shapes.reserve(shapes->nelements);
   for (unsigned int i = 0; i < inputs->nelements; ++i)
   {
      data->inputs.push_back((AtNode*)AiArrayGetPtr(inputs, i));
      data->shapes.push_back((AtNode*)AiArrayGetPtr(shapes, i));
   }
}

node_finish
{
   MayaTripleShadingSwitchData* data = (MayaTripleShadingSwitchData*)AiNodeGetLocalData(node);
   delete data;
}

shader_evaluate
{
   MayaTripleShadingSwitchData* data = (MayaTripleShadingSwitchData*)AiNodeGetLocalData(node);
   for (size_t i = 0; i < data->shapes.size(); ++i)
   {
      if (sg->Op == data->shapes[i])
      {         
         AiShaderEvaluate(data->inputs[i], sg);
         return;
      }
   }
   sg->out.RGB = AiShaderEvalParamRGB(p_default);
}
