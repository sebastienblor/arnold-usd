#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>

#include <ai_universe.h>

class CArnoldImportAssCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldImportAssCmd();
   }

   static MSyntax newSyntax();
   MStatus doIt(const MArgList& argList);
   bool writeAssFromAxf(const MString axfFileName, AtUniverse* universe, MString& assFileName);

}; // class CArnoldImportAssCmd
