#include "platform/Platform.h"
#include "utils/MtoaLog.h"

#include "commands/ArnoldAssTranslator.h"
#include "commands/ArnoldExportAssCmd.h"
#include "commands/ArnoldRenderCmd.h"
#include "commands/ArnoldIprCmd.h"
#include "commands/ArnoldPluginCmd.h"

#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldAOVNode.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/SphereLocator.h"
#include "nodes/options/ArnoldOptionsNode.h"
#include "nodes/shader/ArnoldSkyNode.h"
#include "nodes/shader/ArnoldDisplacementNode.h"
#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"

#include "translators/options/OptionsTranslator.h"
#include "translators/camera/CameraTranslators.h"
#include "translators/light/LightTranslators.h"
#include "translators/shader/ShaderTranslators.h"
#include "translators/shape/MeshTranslator.h"
#include "translators/shape/NurbsSurfaceTranslator.h"
#include "translators/shape/HairTranslator.h"
#include "translators/shape/StandinsTranslator.h"

#include "render/RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"

#include <ai_msg.h>
#include <ai_render.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSwatchRenderRegister.h>

#define VAL(str) #str
#define TOSTRING(str) VAL(str)

#define MTOA_VENDOR "SolidAngle"
#define MTOA_MAJOR_VERSION_NUM 0
#define MTOA_MINOR_VERSION_NUM 8
#define MTOA_FIX_VERSION "0.dev"

#define MTOA_MAJOR_VERSION         TOSTRING(MTOA_MAJOR_VERSION_NUM)
#define MTOA_MINOR_VERSION         TOSTRING(MTOA_MINOR_VERSION_NUM)

#define MTOA_VERSION MTOA_MAJOR_VERSION "." MTOA_MINOR_VERSION "." MTOA_FIX_VERSION

#define MAYA_VERSION "Any"
   
namespace // <anonymous>
{
   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // STANDINS
      status = plugin.registerShape("aiStandIn",
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
                                    CArnoldOptionsNode::id,
                                    CArnoldOptionsNode::creator,
                                    CArnoldOptionsNode::initialize);
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
                                   CArnoldDisplacementNode::id,
                                   CArnoldDisplacementNode::creator,
                                   CArnoldDisplacementNode::initialize,
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
                                   CArnoldSkyDomeLightNode::id,
                                   CArnoldSkyDomeLightNode::creator,
                                   CArnoldSkyDomeLightNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &lightWithSwatch);
                                   // &lightNoSwatch);

      CHECK_MSTATUS(status);

      // Special shaders (not visible from Maya shaders menu)
      MString environmentWithSwatch = CLASSIFY_SHADER_ENVIRONMENT
                                    + ":" + ARNOLD_CLASSIFY(CLASSIFY_SHADER_ENVIRONMENT)
                                    + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiSky",
                                   CArnoldSkyNode::id,
                                   CArnoldSkyNode::creator,
                                   CArnoldSkyNode::initialize,
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
                                  COptionsTranslator::creator);
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
                                   CSkyDomeLightTranslator::creator,
                                   CSkyDomeLightTranslator::NodeInitializer);
       // Geometry
       builtin->RegisterTranslator("mesh",
                                   "",
                                   CMeshTranslator::creator,
                                   CMeshTranslator::NodeInitializer);
       builtin->RegisterTranslator("nurbsSurface",
                                   "",
                                   CNurbsSurfaceTranslator::creator,
                                   CNurbsSurfaceTranslator::NodeInitializer);
       builtin->RegisterTranslator("aiStandIn",
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
      status = CExtensionsManager::LoadArnoldPlugins();
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
      MDGMessage::removeCallback(CArnoldOptionsNode::sId);
      // Deregister node
      status = plugin.deregisterNode(CArnoldOptionsNode::id);
      CHECK_MSTATUS(status);

      // AOV
      status = plugin.deregisterNode(CArnoldAOVNode::id);
      CHECK_MSTATUS(status);

      // Displacement Shaders
      status = plugin.deregisterNode(CArnoldDisplacementNode::id);
      CHECK_MSTATUS(status);

      // Sky dome light
      status = plugin.deregisterNode(CArnoldSkyDomeLightNode::id);
      CHECK_MSTATUS(status);

      // Environment or Volume shaders
      status = plugin.deregisterNode(CArnoldSkyNode::id);
      CHECK_MSTATUS(status);

      return status;
   }
} // namespace


DLLEXPORT MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   AiBegin();
   MtoaSetupLogging();

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
   if (!readMetaSuccess) AiMsgError("[mtoa] Could not read mtoa built-in metadata file mtoa.mtd");

   RegisterArnoldNodes(object);

   MGlobal::executePythonCommand(MString("import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()"));

   AiEnd();

   return MS::kSuccess;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   AiBegin();
   MtoaSetupLogging();

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
