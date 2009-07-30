#ifndef RENDER_CMD_H
#define RENDER_CMD_H

#include "maya_scene/MayaScene.h"

#include <ai_nodes.h>
#include <ai_types.h>

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class CRenderCmd
   :	public MPxCommand
{

public:

   static void* creator()
   {
      return new CRenderCmd();
   }

   static MSyntax newSyntax()
   {
      MSyntax syntax;

      return syntax;
   }

   CRenderCmd();

   ~CRenderCmd()
   {
   }

   MStatus doIt(const MArgList& argList);

private:

   void InitOutputDriver();
   void Render();
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

}; // class CRenderCmd

#endif // RENDER_CMD_H
