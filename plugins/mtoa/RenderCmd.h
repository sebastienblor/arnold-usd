#ifndef RENDER_CMD_H
#define RENDER_CMD_H

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

   static AtVoid LogMessage(AtInt logMask, AtInt severity, const char* msg, AtInt tabs);

   CRenderCmd()
   {
   }

   ~CRenderCmd()
   {
   }

   MStatus doIt(const MArgList& argList);

protected:

   void InitOutputDriver();
   void Render();
   void GetOutputResolution(AtUInt32& width, AtUInt32& height);
   void ProcessArnoldRenderOptions();

}; // class CRenderCmd

#endif // RENDER_CMD_H
