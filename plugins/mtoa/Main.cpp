
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "nodes/ArnoldBackgroundImageShader.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldSkyShader.h"
#include "nodes/ArnoldSky_HDRIShader.h"
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

      // Special shaders (not visible from Maya shaders menu)
      status = plugin.registerNode("ArnoldBackgroundImageShader", CArnoldBackgroundImageShaderNode::id, CArnoldBackgroundImageShaderNode::creator, CArnoldBackgroundImageShaderNode::initialize);
      status = plugin.registerNode("ArnoldSkyShader", CArnoldSkyShaderNode::id, CArnoldSkyShaderNode::creator, CArnoldSkyShaderNode::initialize);
      status = plugin.registerNode("ArnoldSky_HDRIShader", CArnoldSky_HDRIShaderNode::id, CArnoldSky_HDRIShaderNode::creator, CArnoldSky_HDRIShaderNode::initialize);
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

      status = plugin.deregisterNode(CArnoldBackgroundImageShaderNode::id);
      status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
      status = plugin.deregisterNode(CArnoldSky_HDRIShaderNode::id);
   }

}  // namespace <anonymous>

__declspec(dllexport)
MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, "SolidAngle", "0.3", "Any");

   plugin.setName("Arnold Render");

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax);
   status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax);

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
   status = plugin.deregisterCommand("arnoldRender");
   status = plugin.deregisterCommand("arnoldIpr");

   return MS::kSuccess;
}  // uninitializePlugin()
