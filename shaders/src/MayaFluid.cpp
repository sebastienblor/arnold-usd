#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidMtd);

node_parameters
{
   AiParameterRGB("color", 1.f, 1.f, 1.f);
   
   AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);
   AiParameterInt("zres", 0);
   
   AiParameterFlt("xdim", 0.f);
   AiParameterFlt("ydim", 0.f);
   AiParameterFlt("zdim", 0.f);
   
   AtArray* emptyFloatArray = AiArrayAllocate(0, 1, AI_TYPE_FLOAT);
   AiParameterArray("density", emptyFloatArray);
   
   AtArray* emptyMatrixArray = AiArrayAllocate(0, 1, AI_TYPE_MATRIX);
   AiParameterArray("matrix", emptyMatrixArray);
   
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMayaFluid");
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1C);
}

enum MayaFluidParams{
   p_color=0,
   
   p_xres,
   p_yres,
   p_zres,

   p_xdim,
   p_ydim,
   p_zdim,
   
   p_density
};


struct MayaFluidData{
   int xres, yres, zres;
   float xdim, ydim, zdim;
   
   float* density; // at first only supporting grids
   
   AtMatrix worldMatrix;
   AtMatrix inverseWorldMatrix;
};

node_initialize
{
   MayaFluidData* data = (MayaFluidData*)AiMalloc(sizeof(MayaFluidData));
   AiNodeSetLocalData(node, data);
   
   memset(data, sizeof(MayaFluidData), 0); // setting all of the values to zero
}

node_update
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   data->xres = AiNodeGetInt(node, "xres");
   data->yres = AiNodeGetInt(node, "yres");
   data->zres = AiNodeGetInt(node, "zres");
   
   const int numVoxels = data->xres * data->yres * data->zres;
   
   if (numVoxels == 0)
   {
      AiMsgWarning("[aiMayaFluid] The resolution of the fluid grid is zero!");
      return;
   }
   
   data->xdim = AiNodeGetFlt(node, "xdim");
   data->ydim = AiNodeGetFlt(node, "ydim");
   data->zdim = AiNodeGetFlt(node, "zdim");
   
   AtArray* densityArray = AiNodeGetArray(node, "density");
   
   if (densityArray->nelements == numVoxels)
   {
      data->density = (float*)AiMalloc(sizeof(float) * numVoxels);
      for (int i = 0; i < numVoxels; ++i)
         data->density[i] = AiArrayGetFlt(densityArray, i);
   }
   else
      AiMsgWarning("[aiMayaFluid] The density arrays length is not the same as the voxel count! %i : %i", densityArray->nelements, numVoxels);
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   if (data->density)
      AiFree(data->density);
   
   AiFree(data);
}

shader_evaluate
{
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out_opacity = AI_RGB_WHITE;
      return;
   }
   sg->out.RGB = AiShaderEvalParamRGB(p_color);
}
