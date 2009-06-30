
#include "RenderCmd.h"
#include <maya/MFnPlugin.h>

__declspec(dllexport)
MStatus initializePlugin(MObject object)
{
   MFnPlugin plugin(object, "mtoa", "0.1", "Any");

   return plugin.registerCommand("ArnoldRender", CRenderCmd::creator, CRenderCmd::newSyntax);
} // initializePlugin()


__declspec(dllexport)
MStatus uninitializePlugin(MObject object)
{
   MFnPlugin plugin(object);

   return plugin.deregisterCommand("ArnoldRender");
}  // uninitializePlugin()
