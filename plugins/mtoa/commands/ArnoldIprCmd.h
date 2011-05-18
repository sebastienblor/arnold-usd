#ifndef ARNOLDIPRCMD_H
#define ARNOLDIPRCMD_H


#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MMessage.h> // for MCallbackId

#include <vector>

class CArnoldIprCmd
   :	public MPxCommand
{

public:
   static void* creator()
   {
      return new CArnoldIprCmd();
   }

   static MSyntax newSyntax();

   MStatus doIt(const MArgList& argList);

}; // class CArnoldIprCmd

#endif // ARNOLDIPRCMD_H
