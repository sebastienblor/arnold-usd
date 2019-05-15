#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MMessage.h> // for MCallbackId

#include <vector>

class CArnoldRenderViewCmd
   :	public MPxCommand
{

public:
   static void* creator()
   {
      return new CArnoldRenderViewCmd();
   }

   static MSyntax newSyntax();

   MStatus doIt(const MArgList& argList);
   void startRenderView(const MDagPath &camera, int width, int height, bool ui=true);

}; // class CArnoldRenderViewCmd
