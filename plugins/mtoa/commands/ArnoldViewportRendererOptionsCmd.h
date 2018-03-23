#pragma once

#include <maya/MPxCommand.h>

class CArnoldViewportRendererOptionsCmd : public MPxCommand{
public:
   static void* creator()
   {
      return new CArnoldViewportRendererOptionsCmd();
   }
   virtual MStatus doIt(const MArgList& args);
   static MSyntax newSyntax();
};
