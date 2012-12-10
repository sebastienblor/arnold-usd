#pragma once

#include <maya/MPxCommand.h>

class CArnoldTemperatureCmd : public MPxCommand{
public:
   static void* creator();
   virtual MStatus doIt(const MArgList& args);
};
