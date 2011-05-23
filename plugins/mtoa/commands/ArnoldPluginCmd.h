#ifndef ARNOLDPLUGINCMD_H
#define ARNOLDPLUGINCMD_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

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

#endif // ARNOLDPLUGINCMD_H
