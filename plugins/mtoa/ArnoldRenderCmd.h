#ifndef ARNOLD_RENDER_CMD_H
#define ARNOLD_RENDER_CMD_H

#include "maya_scene/MayaScene.h"
#include "render/RenderOptions.h"

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class CArnoldRenderCmd
   :	public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldRenderCmd();
   }

   static MSyntax newSyntax();

   MStatus doIt(const MArgList& argList);

private:

private:

   CMayaScene m_scene;

   CRenderOptions m_renderOptions;

}; // class CArnoldRenderCmd

#endif // ARNOLD_RENDER_CMD_H
