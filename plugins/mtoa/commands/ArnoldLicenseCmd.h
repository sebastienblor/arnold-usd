#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>

class CArnoldLicenseCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldLicenseCmd();
   }

   static MSyntax newSyntax();
   MStatus doIt(const MArgList& argList);

}; // class CArnoldLicenseCmd
