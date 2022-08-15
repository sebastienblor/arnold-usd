#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <string.h>

class CArnoldPluginCmd
   :	public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldPluginCmd();
   }

   static MSyntax newSyntax();

   MStatus doIt(const MArgList& argList);

}; // class CArnoldPluginCmd
