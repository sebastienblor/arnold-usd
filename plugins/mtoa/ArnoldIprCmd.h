#ifndef ARNOLD_IPR_CMD_H
#define ARNOLD_IPR_CMD_H

#include "maya_scene/MayaScene.h"
#include "render/RenderOptions.h"

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

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

private:

   CMayaScene m_scene;

   CRenderOptions m_renderOptions;

}; // class CArnoldIprCmd

#endif // RENDER_CMD_H
