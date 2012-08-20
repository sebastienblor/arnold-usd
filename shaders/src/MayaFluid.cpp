#include <ai.h>

#include <memory.h>

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
   AiParameterArray("fuel", AiArrayCopy(emptyFloatArray));
   AiParameterArray("temperature", AiArrayCopy(emptyFloatArray));
   
   AtArray* emptyColorArray = AiArrayAllocate(0, 1, AI_TYPE_RGB);
   AiParameterArray("colors", emptyColorArray);
   
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
   
   p_density,
   p_fuel,
   p_temperature,   
   p_colors,
   
   p_matrix
};


struct MayaFluidData{
   int xres, yres, zres;
   float xdim, ydim, zdim;
   
   float* density; // at first only supporting grids
   float* fuel;
   float* temperature;
   AtRGB* colors;
   
   bool singleDensity;
   bool singleFuel;
   bool singleTemperature;
   bool singleColors;
   
   AtMatrix worldMatrix;
   AtMatrix inverseWorldMatrix;
};

node_initialize
{
   MayaFluidData* data = (MayaFluidData*)AiMalloc(sizeof(MayaFluidData));
   AiNodeSetLocalData(node, data);
   
   memset(data, sizeof(MayaFluidData), 0); // setting all of the values to zero
}

void ReadFloatArray(AtNode* node, const char* name, int numVoxels, float*& oParam, bool& oParamBool)
{
   AtArray* array = AiNodeGetArray(node, name);
   
   if (array->nelements == numVoxels)
   {
      oParamBool = false;
      oParam = (float*)AiMalloc(sizeof(float) * numVoxels);
      for (int i = 0; i < numVoxels; ++i)
         oParam[i] = AiArrayGetFlt(array, i);
   }
   else if (array->nelements == 1) // only one value
   {
      oParamBool = true;
      oParam = (float*)AiMalloc(sizeof(float));
      oParam[0] = AiArrayGetFlt(array, 0);
   }
   else
      oParam = 0;
}

void ReadRGBArray(AtNode* node, const char* name, int numVoxels, AtRGB*& oParam, bool oParamBool)
{
   AtArray* array = AiNodeGetArray(node, name);
   
   if (array->nelements == numVoxels)
   {
      oParamBool = false;
      oParam = (AtRGB*)AiMalloc(sizeof(AtRGB) * numVoxels);
      for (int i = 0; i < numVoxels; ++i)
         oParam[i] = AiArrayGetRGB(array, i);
   }
   else if (array->nelements == 1) // only one value
   {
      oParamBool = true;
      oParam = (AtRGB*)AiMalloc(sizeof(AtRGB));
      oParam[0] = AiArrayGetRGB(array, 0);
   }
   else
      oParam = 0;
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
   
   ReadFloatArray(node, "density", numVoxels, data->density, data->singleDensity);
   ReadFloatArray(node, "fuel", numVoxels, data->fuel, data->singleFuel);
   ReadFloatArray(node, "temperature", numVoxels, data->temperature, data->singleTemperature);
   ReadRGBArray(node, "colors", numVoxels, data->colors, data->singleColors);   
   
   AtArray* matrixArray = AiNodeGetArray(node, "matrix");
   
   if (matrixArray->nelements != 0)
   { // only using the first matrix atm
      AiArrayGetMtx(matrixArray, 0, data->worldMatrix);
      AiM4Invert(data->worldMatrix, data->inverseWorldMatrix);
   }
   else
   {
      AiM4Identity(data->worldMatrix);
      AiM4Identity(data->inverseWorldMatrix);
      AiMsgWarning("[aiMayaFluid] The matrix array is empty!");
   }
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   if (data->density)
      AiFree(data->density);
   
   if (data->fuel)
      AiFree(data->fuel);
   
   if (data->temperature)
      AiFree(data->temperature);
   
   if (data->colors)
      AiFree(data->colors);
   
   AiFree(data);
}

shader_evaluate
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   AtVector lRo;
   AiM4PointByMatrixMult(&lRo, data->inverseWorldMatrix, &sg->Ro);
   AtVector lRd;
   AiM4VectorByMatrixMult(&lRd, data->inverseWorldMatrix, &sg->Rd);
   float lRl = 1.0f / AiV3Length(lRd);
   lRd *= lRl;
   lRl = lRl * sg->Rl;
   float step_size = 0.1f;   
   
   float transparency = 1.f;
   
   for (float l = 0.f; l < lRl; l += step_size)
   {
      AtVector cPt = lRo + lRd * l;
      cPt.x += data->xdim / 2;
      cPt.y += data->ydim / 2;
      cPt.z += data->zdim / 2;
      cPt.x = CLAMP(cPt.x / data->xdim, 0.f, 1.f);
      cPt.y = CLAMP(cPt.y / data->ydim, 0.f, 1.f);
      cPt.z = CLAMP(cPt.z / data->zdim, 0.f, 1.f);
      
      int cx = CLAMP((int)(cPt.x * (float)data->xres), 0, data->xres - 1);
      int cy = CLAMP((int)(cPt.y * (float)data->yres), 0, data->yres - 1);
      int cz = CLAMP((int)(cPt.z * (float)data->zres), 0, data->zres - 1);
      
      int c = cx + cy * data->xres + cz * data->xres * data->yres;
      float tr = 1.f - data->density[c] * step_size;
      
      transparency *= tr;
      if (transparency < AI_EPSILON)
         break;
   }
   
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out.RGB = transparency * sg->Ci;
      return;
   }
   
   sg->out.RGB = AiShaderEvalParamRGB(p_color) * (1.f - transparency) + sg->Ci * transparency;
}
