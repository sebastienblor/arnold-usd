#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class CArnoldCopyAsAdminCmd
   :  public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldCopyAsAdminCmd();
   }
   
   static MSyntax newSyntax();
   
   MStatus doIt(const MArgList& argList);
};

