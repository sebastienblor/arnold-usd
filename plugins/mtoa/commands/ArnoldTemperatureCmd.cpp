#include "ArnoldTemperatureCmd.h"

#include "../translators/light/LightTranslator.h"

#include <maya/MArgList.h>

void* CArnoldTemperatureCmd::creator()
{
   return new CArnoldTemperatureCmd;
}

MStatus CArnoldTemperatureCmd::doIt(const MArgList& args)
{
   if (args.length() < 1)
      return MS::kInvalidParameter;
   float kelvin = (float)args.asDouble(0);
   const AtRGB rgb = CLightTranslator::ConvertKelvinToRGB(kelvin);
   appendToResult(rgb.r);
   appendToResult(rgb.g);
   appendToResult(rgb.b);
   return MS::kSuccess;
}

