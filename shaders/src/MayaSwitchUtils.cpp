#include "MayaSwitchUtils.h"

void CreateShadingSwitch(AtNode* node)
{
   AiNodeSetLocalData(node, new MayaShadingSwitchData);
}

void UpdateShadingSwitch(AtNode* node)
{
   MayaShadingSwitchData* data = (MayaShadingSwitchData*)AiNodeGetLocalData(node);
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

void FinishShadingSwitch(AtNode* node)
{
   delete (MayaShadingSwitchData*)AiNodeGetLocalData(node);
}

bool EvaluateShadingSwitch(AtShaderGlobals* sg, AtNode* node)
{
   MayaShadingSwitchData* data = (MayaShadingSwitchData*)AiNodeGetLocalData(node);
   for (size_t i = 0; i < data->shapes.size(); ++i)
   {
      if (sg->Op == data->shapes[i])
      {         
         AiShaderEvaluate(data->inputs[i], sg);
         return false;
      }
   }
   return true;
}
