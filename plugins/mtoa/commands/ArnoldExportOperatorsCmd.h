#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>

class CArnoldExportOperatorsCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldExportOperatorsCmd();
   }

   static MSyntax newSyntax();
   MStatus doIt(const MArgList& argList);

}; // class CArnoldUpdateTxCmd