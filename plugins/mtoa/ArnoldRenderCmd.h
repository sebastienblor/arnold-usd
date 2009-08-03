#ifndef ARNOLD_RENDER_CMD_H
#define ARNOLD_RENDER_CMD_H

#include "maya_scene/MayaScene.h"

#include <ai_types.h>

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

   static MSyntax newSyntax()
   {
      MSyntax syntax;

      return syntax;
   }

   CArnoldRenderCmd();

   ~CArnoldRenderCmd()
   {
   }

   MStatus doIt(const MArgList& argList);

private:

   void ProcessCommonRenderOptions();
   void ProcessArnoldRenderOptions();

private:

   CMayaScene m_scene;

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   float    m_pixelAspectRatio;
   bool     m_useRenderRegion;
   bool     m_clearBeforeRender; 
   float    m_gamma;

}; // class CArnoldRenderCmd

#endif // ARNOLD_RENDER_CMD_H
