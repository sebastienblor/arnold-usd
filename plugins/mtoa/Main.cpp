
#include "ArnoldExportAssCmd.h"
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/shaders/atmosphere/ArnoldFogShader.h"
#include "nodes/shaders/atmosphere/ArnoldVolumeScatteringShader.h"
#include "nodes/shaders/background/ArnoldSkyShader.h"
#include "nodes/shaders/light/ArnoldBarndoorShader.h"
#include "nodes/shaders/light/ArnoldGoboShader.h"
#include "nodes/shaders/light/ArnoldLightBlockerShader.h"
#include "nodes/shaders/light/ArnoldLightDecayShader.h"
#include "nodes/shaders/surface/ArnoldAmbientOcclusionShader.h"
#include "nodes/shaders/surface/ArnoldStandardShader.h"
#include "nodes/shaders/surface/ArnoldUtilityShader.h"
#include "nodes/shaders/surface/ArnoldWireframeShader.h"

#include <ai_render.h>

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

      // Surface Shaders
      status = plugin.registerNode("ArnoldAmbientOcclusionShader", CArnoldAmbientOcclusionShaderNode::id, CArnoldAmbientOcclusionShaderNode::creator, CArnoldAmbientOcclusionShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldStandardShader", CArnoldStandardShaderNode::id, CArnoldStandardShaderNode::creator, CArnoldStandardShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldUtilityShader", CArnoldUtilityShaderNode::id, CArnoldUtilityShaderNode::creator, CArnoldUtilityShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldWireframeShader", CArnoldWireframeShaderNode::id, CArnoldWireframeShaderNode::creator, CArnoldWireframeShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);

      // Light Shaders
      status = plugin.registerNode("ArnoldBarndoorShader", CArnoldBarndoorShaderNode::id, CArnoldBarndoorShaderNode::creator, CArnoldBarndoorShaderNode::initialize);
      status = plugin.registerNode("ArnoldGoboShader", CArnoldGoboShaderNode::id, CArnoldGoboShaderNode::creator, CArnoldGoboShaderNode::initialize);
      status = plugin.registerNode("ArnoldLightBlockerShader", CArnoldLightBlockerShaderNode::id, CArnoldLightBlockerShaderNode::creator, CArnoldLightBlockerShaderNode::initialize);
      status = plugin.registerNode("ArnoldLightDecayShader", CArnoldLightDecayShaderNode::id, CArnoldLightDecayShaderNode::creator, CArnoldLightDecayShaderNode::initialize);

      // Special shaders (not visible from Maya shaders menu)
      status = plugin.registerNode("ArnoldFogShader", CArnoldFogShaderNode::id, CArnoldFogShaderNode::creator, CArnoldFogShaderNode::initialize);
      status = plugin.registerNode("ArnoldSkyShader", CArnoldSkyShaderNode::id, CArnoldSkyShaderNode::creator, CArnoldSkyShaderNode::initialize);
      status = plugin.registerNode("ArnoldVolumeScatteringShader", CArnoldVolumeScatteringShaderNode::id, CArnoldVolumeScatteringShaderNode::creator, CArnoldVolumeScatteringShaderNode::initialize);
   }

   void UnregisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // RENDER OPTIONS
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);

      // Surface Shaders
      status = plugin.deregisterNode(CArnoldAmbientOcclusionShaderNode::id);
      status = plugin.deregisterNode(CArnoldStandardShaderNode::id);
      status = plugin.deregisterNode(CArnoldUtilityShaderNode::id);
      status = plugin.deregisterNode(CArnoldWireframeShaderNode::id);

      // Light Shaders
      status = plugin.deregisterNode(CArnoldBarndoorShaderNode::id);
      status = plugin.deregisterNode(CArnoldGoboShaderNode::id);
      status = plugin.deregisterNode(CArnoldLightBlockerShaderNode::id);
      status = plugin.deregisterNode(CArnoldLightDecayShaderNode::id);

      status = plugin.deregisterNode(CArnoldFogShaderNode::id);
      status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
      status = plugin.deregisterNode(CArnoldVolumeScatteringShaderNode::id);
   }

}  // namespace <anonymous>

#ifdef _WIN32
__declspec(dllexport)
#endif
MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, "SolidAngle", "0.3", "Any");

   plugin.setName("Arnold Render");

   AiBegin();

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax);
   status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax);
   status = plugin.registerCommand("arnoldExportAss", CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax);

   RegisterArnoldNodes(object);

   MGlobal::executeCommand( MString("RegisterArnoldRenderer;") );

   AiEnd();

   return MS::kSuccess;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   MGlobal::executeCommand( MString("UnregisterArnoldRenderer;"));

   UnregisterArnoldNodes(object);

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("arnoldRender");
   status = plugin.deregisterCommand("arnoldIpr");
   status = plugin.deregisterCommand("arnoldExportAss");

   return MS::kSuccess;
}
