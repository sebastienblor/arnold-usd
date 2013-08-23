#include <ai.h>


AI_SHADER_NODE_EXPORT_METHODS(MayaTripleShadingSwitchMtd);

node_parameters
{
	AiParameterRGB("default", 0.8f, 0.8f, 0.8f);
   AiParameterARRAY("inputTriples", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterArray("inputNodes", AiArrayAllocate(0, 1, AI_TYPE_NODE));
}

enum MayaTripleShadingSwitchParams{
   p_default = 0,
   p_input_triples,
   p_input_nodes
};

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
   AtArray* triples = AiShaderEvalParamArray(p_input_triples); // TODO : optimize this!!!
   AtArray* nodes = AiShaderEvalParamArray(p_input_nodes);
   for (unsigned int i = 0; i < nodes->nelements; ++i)
   {
      if (sg->Op == (AtNode*)AiArrayGetPtr(nodes, i))
      {
         sg->out.RGB = AiArrayGetRGB(triples, i);
         return;
      }
   }
   sg->out.RGB = AiShaderEvalParamRGB(p_default);
}
