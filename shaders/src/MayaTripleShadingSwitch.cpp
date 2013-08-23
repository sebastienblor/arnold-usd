#include <ai.h>
#include <vector>

AI_SHADER_NODE_EXPORT_METHODS(MayaTripleShadingSwitchMtd);

node_parameters
{
	AiParameterRGB("default", 0.8f, 0.8f, 0.8f);
   AiParameterARRAY("inputTriples", AiArrayAllocate(0, 1, AI_TYPE_NODE));
   AiParameterArray("inputShapes", AiArrayAllocate(0, 1, AI_TYPE_NODE));
}

struct MayaTripleShadingSwitchData{
   std::vector<AtNode*> triples;
   std::vector<AtNode*> shapes;
};

enum MayaTripleShadingSwitchParams{
   p_default = 0,
   p_input_triples,
   p_input_nodes
};

node_initialize
{
   AiNodeSetLocalData(node, new MayaTripleShadingSwitchData);
}

node_update
{
   MayaTripleShadingSwitchData* data = (MayaTripleShadingSwitchData*)AiNodeGetLocalData(node);
   data->triples.clear();
   data->shapes.clear();
   AtArray* triples = AiNodeGetArray(node, "inputTriples");
   AtArray* shapes = AiNodeGetArray(node, "inputShapes");
   if (triples->nelements == 0)
      return;
   data->triples.reserve(triples->nelements);
   data->shapes.reserve(shapes->nelements);
   for (unsigned int i = 0; i < triples->nelements; ++i)
   {
      data->triples.push_back((AtNode*)AiArrayGetPtr(triples, i));
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
         AiShaderEvaluate(data->triples[i], sg);
         return;
      }
   }
   sg->out.RGB = AiShaderEvalParamRGB(p_default);
}
