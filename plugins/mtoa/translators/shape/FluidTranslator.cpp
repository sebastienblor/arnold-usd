#include "FluidTranslator.h"

#include <maya/MFnFluid.h>

void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   
}

AtNode* CFluidTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("standard");
}

void CFluidTranslator::Export(AtNode* fluid)
{
   MFnFluid mayaFluid(GetMayaObject());
   
   unsigned int xRes, yRes, zRes;
   double xDim, yDim, zDim;
   
   mayaFluid.getResolution(xRes, yRes, zRes);
   mayaFluid.getDimensions(xDim, yDim, zDim);
   
   std::cerr << xRes << " " << yRes << " " << zRes << std::endl;
   
   const unsigned int numVoxels = xRes * yRes * zRes;
   
   float* density = mayaFluid.density();
   if (density != 0)
   {
      AtArray* densityArray = AiArrayConvert(numVoxels, 1, AI_TYPE_FLOAT, density);
      AiNodeDeclare(fluid, "density", "constant ARRAY FLOAT");
      AiNodeSetArray(fluid, "density", densityArray);
   }
}
