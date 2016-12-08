#include <ai.h>

namespace
{

enum ShadingGroupParams
{
   p_beauty,
   p_volume,
   p_aov_inputs,
   p_aov_names,
   p_enable_matte,
   p_matte_color
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaShadingEngineMtd);

#define MAiArrayGetNode(arr, i) (AtNode*)AiArrayGetPtr(arr, i)

node_parameters
{
   // Node metadata
   //AiMetaDataSetStr(nentry, NULL, "maya.name", "shadingEngine");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);

   AiParameterClosure("beauty");
   AiParameterRGB("volume", 0.f, 0.f, 0.f);
   AiParameterArray("aov_inputs", AiArray(0, 0, AI_TYPE_NODE));
   AiParameterArray("aov_names", AiArray(0, 0, AI_TYPE_STRING));
   AiParameterBool("enable_matte", false);
   AiParameterRGBA("matte_color", 0.f, 0.f, 0.f, 0.f);
}

shader_evaluate
{
   if (sg->sc & AI_CONTEXT_VOLUME)
   {
      // FIXME Arnold5 what about here, how to use closure ?
      AtRGB volume_rgb = AiShaderEvalParamRGB(p_volume);
      AtClosureList closures;
      closures.add(AiClosureEmission(sg, volume_rgb));
      sg->out.CLOSURE() = closures;
      return;
   }
   if (sg->Rt & AI_RAY_CAMERA && AiShaderEvalParamBool(p_enable_matte))
   {
      AtRGBA matte_color = AiShaderEvalParamRGBA(p_matte_color);
      AtClosureList closures;
      closures.add(AiClosureEmission(sg, matte_color.rgb()));
      closures.add(AiClosureMatte(sg, AtRGB(1 - matte_color.a)));
      sg->out.CLOSURE() = closures;
      return;
   }
   AtArray *inputs = AiShaderEvalParamArray(p_aov_inputs);
//   AtArray *names = AiShaderEvalParamArray(p_aov_names);
//   if (AiArrayGetNumElements(inputs) != names->getNumElements())
//   {
//      AiMsgWarning("inputs and names are not the same length");
//      return;
//   }       
   if (inputs == NULL)
   {
      AiMsgWarning("%s.aov_inputs is NULL", AiNodeGetName(node));
   }
   else if (sg->Rt & AI_RAY_CAMERA)
   {
      for (unsigned int i = 0; (i < AiArrayGetNumElements(inputs)); ++i)
      {
         AtNode *input = MAiArrayGetNode(inputs, i);
         if (input != NULL)
         {
            AiShaderEvaluate(input, sg);
         }
         else
            AiMsgWarning("%s.aov_inputs[%d] is NULL", AiNodeGetName(node), i);
         // currently, this shader is only required to evaluate the aov inputs to trigger their
         // computation (they should already have an aov write node in their network), but
         // eventually, when caching is fixed in arnold core, this shader will also be responsible
         // for writing the results to aovs.
         /*
         const char* name = AiArrayGetStr(names, i);
         if (strlen(name))
         {
            switch (type)
            {
               case AI_TYPE_BOOLEAN:
                  AiAOVSetBool(sg, name, sg->out.BOOL());
                  break;
               case AI_TYPE_INT:
                  AiAOVSetInt(sg, name, sg->out.UINT());
                  break;
               case AI_TYPE_UINT:
                  AiAOVSetInt(sg, name, sg->out.INT());
                  break;
               case AI_TYPE_FLOAT:
                  AiAOVSetFlt(sg, name, sg->out.FLT());
                  break;
               case AI_TYPE_RGB:
                  AiAOVSetRGB(sg, name, sg->out.RGB());
                  break;
               case AI_TYPE_RGBA:
                  AiAOVSetRGBA(sg, name, sg->out.RGBA());
                  break;
               case AI_TYPE_VECTOR:
                  AiAOVSetVec(sg, name, sg->out.VEC());
                  break;
               case AI_TYPE_VECTOR:
                  AiAOVSetVec(sg, name, sg->out.VEC());
                  break;
               case AI_TYPE_VECTOR2:
                  AiAOVSetVec2(sg, name, sg->out.VEC2());
                  break;
               case AI_TYPE_NODE:
               case AI_TYPE_VECTORER:
                  AiAOVSetPtr(sg, name, sg->out.PTR);
                  break;
            }
         }*/
      }
   }
   // This must occur last because AiShaderEvaluate fills sg->out
   sg->out.CLOSURE() = AiShaderEvalParamClosure(p_beauty);
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
