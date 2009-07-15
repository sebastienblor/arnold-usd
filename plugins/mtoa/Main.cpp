
#include "RenderCmd.h"
#include "nodes/ArnoldRenderOptions.h"
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

__declspec(dllexport)
MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, "mtoa", "0.1", "Any");

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("ArnoldRender", CRenderCmd::creator, CRenderCmd::newSyntax);
   status = plugin.registerNode("ArnoldRenderOptions", CArnoldRenderOptionsNode::id, CArnoldRenderOptionsNode::creator, CArnoldRenderOptionsNode::initialize);

   MGlobal::executeCommand( MString("renderer arnold -rendererUIName \"Arnold Renderer\" -renderProcedure \"ArnoldRender\"") );
   MGlobal::executeCommand( MString("SetupArnoldGlobalsTabs;") );

   return MS::kSuccess;
} // initializePlugin()


__declspec(dllexport)
MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   MGlobal::executeCommand( MString("renderer arnold -unregisterRenderer"));

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("ArnoldRender");
   status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);

   return MS::kSuccess;

}  // uninitializePlugin()
