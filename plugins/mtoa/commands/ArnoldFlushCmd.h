#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class CArnoldFlushCmd
   :  public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldFlushCmd();
   }
   
   static MSyntax newSyntax();
   
   MStatus doIt(const MArgList& argList);
};
