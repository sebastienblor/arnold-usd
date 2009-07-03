
#include "RenderCmd.h"
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

__declspec(dllexport)
MStatus initializePlugin(MObject object)
{
   MFnPlugin plugin(object, "mtoa", "0.1", "Any");

   MGlobal::executeCommand( MString("renderer arnold -rendererUIName \"Arnold Renderer\" -renderProcedure \"ArnoldRender\""));

   return plugin.registerCommand("ArnoldRender", CRenderCmd::creator, CRenderCmd::newSyntax);
} // initializePlugin()


__declspec(dllexport)
MStatus uninitializePlugin(MObject object)
{
   MFnPlugin plugin(object);

   MGlobal::executeCommand( MString("renderer arnold -unregisterRenderer"));

   return plugin.deregisterCommand("ArnoldRender");
}  // uninitializePlugin()
