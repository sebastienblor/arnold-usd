
#include "platform/Platform.h"
#include "utils/MtoaLogCallback.h"
#include "ArnoldAssTranslator.h"
#include "ArnoldExportAssCmd.h"
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "ArnoldPluginCmd.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldAOV.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/shaders/background/SphereLocator.h"
#include "nodes/shaders/background/ArnoldSkyShader.h"
#include "nodes/shaders/displacement/ArnoldDisplacementShader.h"
#include "nodes/shaders/light/ArnoldSkyDomeLightShader.h"
#include "nodes/ArnoldStandIns.h"
#include "nodes/ShaderUtils.h"
#include "scene/Shaders.h"
#include "scene/Lights.h"
#include "scene/Geometry.h"
#include "scene/Cameras.h"
#include "scene/Options.h"
#include "scene/Hair.h"
#include "scene/Standins.h"
#include "render/RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"

#include <ai_msg.h>
#include <ai_render.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSwatchRenderRegister.h>

#define MTOA_VENDOR "SolidAngle"
#define MTOA_VERSION "0.7.0.dev"
#define MAYA_VERSION "Any"


namespace // <anonymous>
{
   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // STANDINS
      status = plugin.registerShape("ArnoldStandIn",
                                    CArnoldStandInShape::id,
                                    CArnoldStandInShape::creator,
                                    CArnoldStandInShape::initialize,
                                    CArnoldStandInShapeUI::creator);
      CHECK_MSTATUS(status);

      // Swatch renderer.
      status = MSwatchRenderRegister::registerSwatchRender(ARNOLD_SWATCH,
                                                           CRenderSwatchGenerator::creator);
      CHECK_MSTATUS(status);

      // Abstract Classes
      status = plugin.registerNode("SphereLocator",
                                    CSphereLocator::id,
                                    CSphereLocator::creator,
                                    CSphereLocator::initialize,
                                    MPxNode::kLocatorNode);
      CHECK_MSTATUS(status);

      // Render Options
      status = plugin.registerNode("aiOptions",
                                    CArnoldRenderOptionsNode::id,
                                    CArnoldRenderOptionsNode::creator,
                                    CArnoldRenderOptionsNode::initialize);
      CHECK_MSTATUS(status);

      // AOV
      status = plugin.registerNode("ArnoldAOV",
                                   CArnoldAOVNode::id,
                                   CArnoldAOVNode::creator,
                                   CArnoldAOVNode::initialize);
      CHECK_MSTATUS(status);



      // Displacement Shaders
      MString displacementWithSwatch = CLASSIFY_SHADER_DISPLACEMENT
                                     + ":" + ARNOLD_CLASSIFY(CLASSIFY_SHADER_DISPLACEMENT)
                                     + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiDisplacement",
                                   CArnoldDisplacementShaderNode::id,
                                   CArnoldDisplacementShaderNode::creator,
                                   CArnoldDisplacementShaderNode::initialize,
                                   MPxNode::kDependNode,
                                   &displacementWithSwatch);
      CHECK_MSTATUS(status);

      // Light Shaders
      MString lightWithSwatch       = CLASSIFY_SHADER_LIGHT
                                    + ":" + ARNOLD_CLASSIFY(CLASSIFY_SHADER_LIGHT)
                                    + ":swatch/" + ARNOLD_SWATCH;
      MString lightNoSwatch         = CLASSIFY_SHADER_LIGHT
                                    + ":" + ARNOLD_CLASSIFY(CLASSIFY_SHADER_LIGHT);
      status = plugin.registerNode("aiSkyDomeLight",
                                   CArnoldSkyDomeLightShaderNode::id,
                                   CArnoldSkyDomeLightShaderNode::creator,
                                   CArnoldSkyDomeLightShaderNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &lightWithSwatch);
                                   // &lightNoSwatch);

      CHECK_MSTATUS(status);

      // Special shaders (not visible from Maya shaders menu)
      MString environmentWithSwatch = CLASSIFY_SHADER_ENVIRONMENT
                                    + ":" + ARNOLD_CLASSIFY(CLASSIFY_SHADER_ENVIRONMENT)
                                    + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiSky",
                                   CArnoldSkyShaderNode::id,
                                   CArnoldSkyShaderNode::creator,
                                   CArnoldSkyShaderNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &environmentWithSwatch);
      CHECK_MSTATUS(status);

      // Get a CExtension for the builtin nodes
      CExtensionsManager::SetMayaPlugin(object);
      CExtensionsManager::CreatePluginLoadedCallback();
      CExtension* builtin;
      builtin = CExtensionsManager::GetBuiltin();
      // Override for builtins for specific cases
      builtin->RegisterTranslator("aiOptions",
                                  "",
                                  CRenderOptionsTranslator::creator);
      builtin->RegisterTranslator("surfaceShader",
                                  "",
                                  CSurfaceShaderTranslator::creator);
      builtin->RegisterTranslator("lambert",
                                  "",
                                  CLambertTranslator::creator);
      // A Dag node in Maya but a depend node in Arnold
       builtin->RegisterTranslator("aiSky",
                                   "",
                                   CSkyShaderTranslator::creator);
       // Lights
       builtin->RegisterTranslator("directionalLight",
                                   "",
                                   CDirectionalLightTranslator::creator,
                                   CDirectionalLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("spotLight",
                                   "",
                                   CSpotLightTranslator::creator,
                                   CSpotLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("areaLight",
                                   "",
                                   CAreaLightTranslator::creator,
                                   CAreaLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("pointLight",
                                   "",
                                   CPointLightTranslator::creator,
                                   CPointLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("ambientLight",
                                   "",
                                   CAmbientLightTranslator::creator,
                                   CLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("aiSkyDomeLight",
                                   "",
                                   CSkyDomeLightTranslator::creator);
       // Geometry
       builtin->RegisterTranslator("mesh",
                                   "",
                                   CMeshTranslator::creator,
                                   CMeshTranslator::NodeInitializer);
       builtin->RegisterTranslator("nurbsSurface",
                                   "",
                                   CNurbsSurfaceTranslator::creator,
                                   CNurbsSurfaceTranslator::NodeInitializer);
       builtin->RegisterTranslator("ArnoldStandIn",
                                   "builtin",
                                   CArnoldStandInsTranslator::creator,
                                   CArnoldStandInsTranslator::NodeInitializer);
       // Multiple camera translators for single Maya camera node
       builtin->RegisterTranslator("camera",
                                   "perspective",
                                   CPerspCameraTranslator::creator,
                                   CPerspCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("camera",
                                   "orthographic",
                                   COrthoCameraTranslator::creator,
                                   COrthoCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("camera",
                                   "fisheye",
                                   CFishEyeCameraTranslator::creator,
                                   CFishEyeCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("camera",
                                   "cylindrical",
                                   CCylCameraTranslator::creator,
                                   CCylCameraTranslator::NodeInitializer);
       // Hair
       builtin->RegisterTranslator("hairSystem",
                                   "",
                                   CHairTranslator::creator,
                                   CHairTranslator::NodeInitializer);

      // Load all plugins path or only shaders?
      CExtension* shaders;
      shaders = CExtensionsManager::LoadArnoldPlugin("mtoa_shaders", "$ARNOLD_PLUGIN_PATH", &status);
      CHECK_MSTATUS(status);
      // Overrides for mtoa_shaders
      shaders->RegisterTranslator("layeredShader",
                                  "",
                                  CLayeredShaderTranslator::creator);
      shaders->RegisterTranslator("layeredTexture",
                                  "",
                                  CLayeredTextureTranslator::creator);
      shaders->RegisterTranslator("file",
                                  "",
                                  CFileTranslator::creator);
      shaders->RegisterTranslator("place2dTexture",
                                  "",
                                  CPlace2DTextureTranslator::creator);
      shaders->RegisterTranslator("bump2d",
                                  "",
                                  CBump2DTranslator::creator);
      shaders->RegisterTranslator("bump3d",
                                  "",
                                  CBump3DTranslator::creator);
      shaders->RegisterTranslator("samplerInfo",
                                  "facingRatio",
                                  CSamplerInfoTranslator::creator);
      shaders->RegisterTranslator("plusMinusAverage",
                                  "",
                                  CPlusMinusAverageTranslator::creator);
      shaders->RegisterTranslator("remapValue",
                                  "",
                                  CRemapValueTranslator::creator);
      shaders->RegisterTranslator("remapColor",
                                  "",
                                  CRemapColorTranslator::creator);
      shaders->RegisterTranslator("projection",
                                  "",
                                  CProjectionTranslator::creator);
      shaders->RegisterTranslator("ramp",
                                  "",
                                  CRampTranslator::creator);


      // Will load all found plugins and try to register nodes and translators
      // for the new Arnold node each create. A CExtension is initialized.
      status = CExtensionsManager::LoadExtensions();
      // status = CExtensionsManager::LoadArnoldPlugins();
      // Finally register all nodes from the loaded extensions with Maya in load order
      CExtensionsManager::RegisterExtensions();

      // CExtension::CreateCallbacks();

      // Or use MGlobal::apiVersion()
      // TODO : should be called by ExtensionsManager after new registrations?
#if MAYA_API_VERSION < 201200
      MNodeClass::InitializeExistingNodes();
#endif

      return status;
   }

   MStatus UnregisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      status = CExtensionsManager::RemovePluginLoadedCallback();
      CHECK_MSTATUS(status);
      status = CExtensionsManager::UnloadExtensions();
      CHECK_MSTATUS(status);

      // Render Options
      // Remove creation callback
      MDGMessage::removeCallback(CArnoldRenderOptionsNode::sId);
      // Deregister node
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);
      CHECK_MSTATUS(status);

      // AOV
      status = plugin.deregisterNode(CArnoldAOVNode::id);
      CHECK_MSTATUS(status);

      // Displacement Shaders
      status = plugin.deregisterNode(CArnoldDisplacementShaderNode::id);
      CHECK_MSTATUS(status);

      // Environment or Volume shaders
      status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
      CHECK_MSTATUS(status);

      return status;
   }
} // namespace


DLLEXPORT MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   AiBegin();
   SetupMtoaLogging();

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax);
   status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax);
   status = plugin.registerCommand("arnoldExportAss", CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax);
   status = plugin.registerCommand("arnoldPlugins", CArnoldPluginCmd::creator, CArnoldPluginCmd::newSyntax);
   status = plugin.registerFileTranslator(CArnoldAssTranslator::fileType,
         CArnoldAssTranslator::fileIcon,
         CArnoldAssTranslator::creator,
         CArnoldAssTranslator::optionScript,
         CArnoldAssTranslator::optionDefault,
         false);

   // Load metadata for builtin (mtoa.mtd)
   // const char* metafile = "/usr/solidangle/mtoadeploy/2011/plug-ins/mtoa.mtd";
   MString loadpath = plugin.loadPath();
   MString metafile = loadpath + "/" + "mtoa.mtd";
   AtBoolean readMetaSuccess = AiMetaDataLoadFile(metafile.asChar());
   if (!readMetaSuccess) AiMsgError("Could not read mtoa built-in metadata file mtoa.mtd");

   RegisterArnoldNodes(object);

   MGlobal::executePythonCommand(MString("import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()"));

   AiMsgResetCallback();
   AiEnd();

   return MS::kSuccess;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   AiBegin();
   SetupMtoaLogging();

   MGlobal::executePythonCommand(MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"));

   UnregisterArnoldNodes(object);

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("arnoldRender");
   status = plugin.deregisterCommand("arnoldIpr");
   status = plugin.deregisterCommand("arnoldExportAss");
   status = plugin.deregisterCommand("arnoldPlugins");
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileType);

   AiMsgResetCallback();
   AiEnd();

   return MS::kSuccess;
}
