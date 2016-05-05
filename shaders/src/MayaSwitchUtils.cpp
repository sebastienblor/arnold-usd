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
   if (AiArrayGetNumElements(inputs) == 0)
      return;
   data->inputs.reserve(AiArrayGetNumElements(inputs));
   data->shapes.reserve(AiArrayGetNumElements(shapes));
   for (unsigned int i = 0; i < AiArrayGetNumElements(inputs); ++i)
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
