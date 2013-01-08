#pragma once

#include <maya/MPxCommand.h>

class CArnoldListAttributesCmd : public MPxCommand{
public:
   CArnoldListAttributesCmd();
   ~CArnoldListAttributesCmd();
   
   static void* creator();
   virtual MStatus doIt(const MArgList& args);
};
