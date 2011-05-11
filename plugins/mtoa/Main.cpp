
#include "platform/Platform.h"
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
#include "nodes/ShaderUtils.h"
#include "scene/Shaders.h"
#include "scene/Lights.h"
#include "scene/Geometry.h"
#include "scene/Cameras.h"
#include "scene/Options.h"
#include "scene/Hair.h"
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

AtVoid MtoaLogCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs)
{
   const char *header = "[mtoa] %s";
   char *buf;
   buf = new (std::nothrow) char[strlen(header)+strlen(msg_string)];

   if (NULL == buf)
   {
      // Fallback so normal logging still works in case of an allocation failure
      clog << msg_string << endl;
   }
   else
   {
      sprintf(buf, header, msg_string);
      // Standard log output
      clog << buf << endl;
      // Some feedback will go in Maya script window
      switch (severity)
      {
      case AI_SEVERITY_INFO:
         if (logmask & AI_LOG_INFO)
            MGlobal::displayInfo(buf);
         break;
      case AI_SEVERITY_WARNING:
         if (logmask & AI_LOG_WARNINGS)
            // if (clog != cerr) cerr << buf << endl;
            MGlobal::displayWarning(buf);
         break;
      case AI_SEVERITY_ERROR:
         if (logmask & AI_LOG_ERRORS)
            // if (clog != cerr) cerr << buf << endl;
            MGlobal::displayError(buf);
         break;
      default:
         break;
      }
      
      delete[] buf;
   }
}

namespace // <anonymous>
{
   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

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
      CExtension* builtin;
      builtin = CExtensionsManager::GetBuiltin();
      // Override for builtins for specific cases
      builtin->RegisterTranslator("options",
                                  "aiOptions",
                                  "",
                                  CRenderOptionsTranslator::creator);
      builtin->RegisterTranslator("flat",
                                  "surfaceShader",
                                  "",
                                  CSurfaceShaderTranslator::creator);
      builtin->RegisterTranslator("lambert",
                                  "lambert",
                                  "",
                                  CLambertTranslator::creator);
      // A Dag node in Maya but a depend node in Arnold
       builtin->RegisterTranslator("sky",
                                   "aiSky",
                                   "",
                                   CSkyShaderTranslator::creator);
       // Lights
       builtin->RegisterTranslator("distant_light",
                                   "directionalLight",
                                   "",
                                   CDirectionalLightTranslator::creator,
                                   CDirectionalLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("spot_light",
                                   "spotLight",
                                   "",
                                   CSpotLightTranslator::creator,
                                   CSpotLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("quad_light",
                                   "areaLight",
                                   "",
                                   CAreaLightTranslator::creator,
                                   CAreaLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("point_light",
                                   "pointLight",
                                   "",
                                   CPointLightTranslator::creator,
                                   CPointLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("ambient_light",
                                   "ambientLight",
                                   "",
                                   CAmbientLightTranslator::creator,
                                   CLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("skydome_light",
                                   "aiSkyDomeLight",
                                   "",
                                   CSkyDomeLightTranslator::creator);
       // Geometry
       builtin->RegisterTranslator("polymesh",
                                   "mesh",
                                   "",
                                   CMeshTranslator::creator,
                                   CMeshTranslator::NodeInitializer);
       builtin->RegisterTranslator("nurbs",
                                   "nurbsSurface",
                                   "",
                                   CNurbsSurfaceTranslator::creator,
                                   CNurbsSurfaceTranslator::NodeInitializer);
       // Multiple camera translators for single Maya camera node
       builtin->RegisterTranslator("persp_camera",
                                   "camera",
                                   "perspective",
                                   CPerspCameraTranslator::creator,
                                   CPerspCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("ortho_camera",
                                   "camera",
                                   "orthographic",
                                   COrthoCameraTranslator::creator,
                                   COrthoCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("fisheye_camera",
                                   "camera",
                                   "fisheye",
                                   CFishEyeCameraTranslator::creator,
                                   CFishEyeCameraTranslator::NodeInitializer);
       builtin->RegisterTranslator("cyl_camera",
                                   "camera",
                                   "cylindrical",
                                   CCylCameraTranslator::creator,
                                   CCylCameraTranslator::NodeInitializer);
       // Hair
       builtin->RegisterTranslator("curves",
                                   "hairSystem",
                                   "",
                                   CHairTranslator::creator,
                                   CHairTranslator::NodeInitializer);

      // Load all plugins path or only shaders?
      CExtension* shaders;
      shaders = CExtensionsManager::LoadArnoldPlugin("mtoa_shaders", "$ARNOLD_PLUGIN_PATH", &status);
      CHECK_MSTATUS(status);
      // Overrides for mtoa_shaders
      shaders->RegisterTranslator("MayaLayeredShader",
                                  "layeredShader",
                                  "",
                                  CLayeredShaderTranslator::creator);
      shaders->RegisterTranslator("MayaFile",
                                  "file",
                                  "",
                                  CFileTranslator::creator);
      shaders->RegisterTranslator("MayaPlace2DTexture",
                                  "place2dTexture",
                                  "",
                                  CPlace2DTextureTranslator::creator);
      shaders->RegisterTranslator("bump2d",
                                  "bump2d",
                                  "",
                                  CBump2DTranslator::creator);
      shaders->RegisterTranslator("bump3d",
                                  "bump3d",
                                  "",
                                  CBump3DTranslator::creator);
      shaders->RegisterTranslator("MayaFacingRatio",
                                  "samplerInfo",
                                  "facingRatio",
                                  CSamplerInfoTranslator::creator);
      shaders->RegisterTranslator("MayaFlippedNormal",
                                  "samplerInfo",
                                  "flippedNormal",
                                  CSamplerInfoTranslator::creator);
      // Explicit special case, no metadata can be read
      shaders->RegisterTranslator("",
                                  "plusMinusAverage",
                                  "",
                                  CPlusMinusAverageTranslator::creator);
      shaders->RegisterTranslator("MayaRemapValueToValue",
                                  "remapValue",
                                  "valueToValue",
                                  CRemapValueTranslator::creator);
      shaders->RegisterTranslator("MayaRemapValueToColor",
                                  "remapValue",
                                  "valueToColor",
                                  CRemapValueTranslator::creator);
      shaders->RegisterTranslator("MayaRemapColor",
                                  "remapColor",
                                  "",
                                  CRemapColorTranslator::creator);
      shaders->RegisterTranslator("MayaProjection",
                                  "projection",
                                  "",
                                  CProjectionTranslator::creator);
      shaders->RegisterTranslator("MayaRamp",
                                  "ramp",
                                  "",
                                  CRampTranslator::creator);
      shaders->RegisterTranslator("MayaLayeredTexture",
                                  "layeredTexture",
                                  "",
                                  CLayeredTextureTranslator::creator);


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

      //CExtension::RemoveCallbacks();
      CExtensionsManager::UnloadExtensions();

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

   // Setup a default logging level to use when not rendering.
   // Logging parameters are stored on the render options node and are only put in place when a render
   // is triggered.
   void SetupLogging()
   {
      // TODO: read this initial value from an environment variable or option variable
#ifdef _DEBUG
      AtInt defaultLogFlags = AI_LOG_ALL;
#else
      AtInt defaultLogFlags = AI_LOG_INFO | AI_LOG_WARNINGS | AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_BACKTRACE | AI_LOG_MEMORY | AI_LOG_COLOR;
#endif
      AiMsgSetConsoleFlags(defaultLogFlags);
      AiMsgSetCallback(MtoaLogCallback);
   }
} // namespace


DLLEXPORT MStatus initializePlugin(MObject object)
{
   MStatus status;

   MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   AiBegin();

   SetupLogging();

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

   AiEnd();

   return MS::kSuccess;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   MGlobal::executePythonCommand(MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"));

   UnregisterArnoldNodes(object);

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("arnoldRender");
   status = plugin.deregisterCommand("arnoldIpr");
   status = plugin.deregisterCommand("arnoldExportAss");
   status = plugin.deregisterCommand("arnoldPlugins");
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileType);

   // To avoid leaving dangling pointer in Msg callbacks
   AiMsgResetCallback();

   return MS::kSuccess;
}
