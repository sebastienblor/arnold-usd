#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include "maya_scene/MayaScene.h"
#include "render/RenderOptions.h"

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class CArnoldExportAssCmd
   :	public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldExportAssCmd();
   }

   static MSyntax newSyntax();

   MStatus doIt(const MArgList& argList);

private:

   MString VerifyFileName(MString fileName, bool compressed);

private:

   CMayaScene m_scene;

   CRenderOptions m_renderOptions;

}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
