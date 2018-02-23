#pragma once

#include <maya/MPxCommand.h>

class CArnoldViewportRendererOptionsCmd : public MPxCommand{
public:
   static void* creator();
   virtual MStatus doIt(const MArgList& args);
};
