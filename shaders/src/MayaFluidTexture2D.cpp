#include <ai.h>

#include "MayaFluidData.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidTexture2DMtd);

struct MayaFluidTexture2D {
   CMayaFluidData<false>* fluidData;

   static void* operator new(size_t size)
   {
      return AiMalloc(size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }

   MayaFluidTexture2D() : fluidData(0)
   {

   }

   ~MayaFluidTexture2D()
   {
      if (fluidData)
         delete fluidData;
   }
};

node_parameters
{
   InitializeFluidShaderParameters(params, false);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
   AiNodeSetLocalData(node, new MayaFluidTexture2D());
}

node_update
{
   MayaFluidTexture2D* data = reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
   if (data->fluidData)
      delete data->fluidData;
   data->fluidData = new CMayaFluidData<false>(node);
}

node_finish
{
   delete reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
}

shader_evaluate
{
   MayaFluidTexture2D* data = reinterpret_cast<MayaFluidTexture2D*>(AiNodeGetLocalData(node));
   AtVector lpt = {sg->u, sg->v};
   sg->out.RGB = data->fluidData->readDensity(lpt, FT_LINEAR);
}
