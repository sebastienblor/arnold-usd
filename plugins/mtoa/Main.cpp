
#include "RenderCmd.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldStandardShader.h"
#include "nodes/ArnoldUtilityShader.h"
#include "nodes/ArnoldAmbientOcclusionShader.h"
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

namespace // <anonymous>
{

   void RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);
      const MString ShaderClass("shader/surface");

      // RENDER OPTIONS
      status = plugin.registerNode("ArnoldRenderOptions", CArnoldRenderOptionsNode::id, CArnoldRenderOptionsNode::creator, CArnoldRenderOptionsNode::initialize);

      // SHADERS
      status = plugin.registerNode("ArnoldStandardShader", CArnoldStandardShaderNode::id, CArnoldStandardShaderNode::creator, CArnoldStandardShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldUtilityShader", CArnoldUtilityShaderNode::id, CArnoldUtilityShaderNode::creator, CArnoldUtilityShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldAmbientOcclusionShader", CArnoldAmbientOcclusionShaderNode::id, CArnoldAmbientOcclusionShaderNode::creator, CArnoldAmbientOcclusionShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
   }

   void UnregisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // RENDER OPTIONS
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);

      // SHADERS
      status = plugin.deregisterNode(CArnoldStandardShaderNode::id);
      status = plugin.deregisterNode(CArnoldUtilityShaderNode::id);
      status = plugin.deregisterNode(CArnoldAmbientOcclusionShaderNode::id);
   }

}  // namespace <anonymous>

__declspec(dllexport)
MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, "SolidAngle", "0.2", "Any");

   plugin.setName("Arnold Render");

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("arnoldRenderer", CRenderCmd::creator, CRenderCmd::newSyntax);

   RegisterArnoldNodes(object);

   MGlobal::executeCommand( MString("RegisterArnoldRenderer;") );

   return MS::kSuccess;
} // initializePlugin()


__declspec(dllexport)
MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   MGlobal::executeCommand( MString("UnregisterArnoldRenderer;"));

   UnregisterArnoldNodes(object);

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("arnoldRenderer");

   return MS::kSuccess;
}  // uninitializePlugin()
