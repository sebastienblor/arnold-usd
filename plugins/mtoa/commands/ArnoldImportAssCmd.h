#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MStringArray.h>

#include <ai_universe.h>

namespace // <anonymous>
{
const char* rgbComp[3] = {"R", "G", "B"};
const MStringArray RGB_COMPONENTS(rgbComp, 3);
const char* vectorComp[3] = {"X", "Y", "Z"};
const MStringArray VECTOR_COMPONENTS(vectorComp, 3);
}

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
   MStatus convertAxfToArnold(const MString axfFileName, AtUniverse* universe);

}; // class CArnoldImportAssCmd
