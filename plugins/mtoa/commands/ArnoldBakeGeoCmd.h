#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MMessage.h> // for MCallbackId

class CArnoldBakeGeoCmd : public MPxCommand{
public:
   CArnoldBakeGeoCmd();
   ~CArnoldBakeGeoCmd();
   
   static void* creator();
   virtual MStatus doIt(const MArgList& args);
   static MSyntax newSyntax();
};