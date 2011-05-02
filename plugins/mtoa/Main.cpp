
#include "platform/Platform.h"
#include "ArnoldAssTranslator.h"
#include "ArnoldExportAssCmd.h"
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "ArnoldPluginCmd.h"
#include "nodes/ArnoldNodeFactory.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldAOV.h"
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
#include "scene/TranslatorRegistry.h"
#include "render/RenderSwatch.h"

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

      // Swatch renderer.
      status = MSwatchRenderRegister::registerSwatchRender(ARNOLD_SWATCH,
                                                           CRenderSwatchGenerator::creator);
      CHECK_MSTATUS(status);

      // Displacement Shaders
      MString displacementWithSwatch = ARNOLD_SHADER_DISPLACEMENT + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiDisplacement",
                                   CArnoldDisplacementShaderNode::id,
                                   CArnoldDisplacementShaderNode::creator,
                                   CArnoldDisplacementShaderNode::initialize,
                                   MPxNode::kDependNode,
                                   &displacementWithSwatch);
      CHECK_MSTATUS(status);

      // Light Shaders
      MString lightWithSwatch = ARNOLD_SHADER_LIGHT + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiSkyDomeLight",
                                   CArnoldSkyDomeLightShaderNode::id,
                                   CArnoldSkyDomeLightShaderNode::creator,
                                   CArnoldSkyDomeLightShaderNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &lightWithSwatch);
                                   // &ARNOLD_SHADER_LIGHT);
      CHECK_MSTATUS(status);

      // Special shaders (not visible from Maya shaders menu)
      MString environmentWithSwatch = ARNOLD_SHADER_ENVIRONMENT + ":swatch/" + ARNOLD_SWATCH;
      status = plugin.registerNode("aiSky",
                                   CArnoldSkyShaderNode::id,
                                   CArnoldSkyShaderNode::creator,
                                   CArnoldSkyShaderNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &environmentWithSwatch);
      CHECK_MSTATUS(status);

      // Start up the plugin factory.
      CArnoldNodeFactory arnoldPluginFactory(object);

      // Light Filters
      arnoldPluginFactory.RegisterMayaNode("barndoor",
                                           "aiBarndoor",
                                           ARNOLD_NODEID_BARNDOOR,
                                           ARNOLD_SHADER_LIGHT_FILTER);
      arnoldPluginFactory.RegisterMayaNode("gobo",
                                           "aiGobo",
                                           ARNOLD_NODEID_GOBO,
                                           ARNOLD_SHADER_LIGHT_FILTER);
      arnoldPluginFactory.RegisterMayaNode("light_blocker",
                                           "aiLightBlocker",
                                           ARNOLD_NODEID_LIGHT_BLOCKER,
                                           ARNOLD_SHADER_LIGHT_FILTER);
      arnoldPluginFactory.RegisterMayaNode("light_decay",
                                           "aiLightDecay",
                                           ARNOLD_NODEID_LIGHT_DECAY,
                                           ARNOLD_SHADER_LIGHT_FILTER);

      // Surface Shaders
      arnoldPluginFactory.RegisterMayaNode("ray_switch",
                                           "aiRaySwitch",
                                           ARNOLD_NODEID_RAY_SWITCH,
                                           ARNOLD_SHADER_UTILITY);
      arnoldPluginFactory.RegisterMayaNode("ambient_occlusion",
                                           "aiAmbientOcclusion",
                                           ARNOLD_NODEID_AMBIENT_OCCLUSION,
                                           ARNOLD_SHADER_SURFACE);
      arnoldPluginFactory.RegisterMayaNode("standard",
                                           "aiStandard",
                                           ARNOLD_NODEID_STANDARD,
                                           ARNOLD_SHADER_SURFACE);
      arnoldPluginFactory.RegisterMayaNode("utility",
                                           "aiUtility",
                                           ARNOLD_NODEID_UTILITY,
                                           ARNOLD_SHADER_SURFACE);
      arnoldPluginFactory.RegisterMayaNode("wireframe",
                                           "aiWireframe",
                                           ARNOLD_NODEID_WIREFRAME,
                                           ARNOLD_SHADER_SURFACE);
      arnoldPluginFactory.RegisterMayaNode("hair",
                                           "aiHair",
                                           ARNOLD_NODEID_HAIR,
                                           ARNOLD_SHADER_SURFACE);

      // Environment/Volume shaders
      arnoldPluginFactory.RegisterMayaNode("fog",
                                           "aiFog",
                                           ARNOLD_NODEID_FOG,
                                           ARNOLD_SHADER_ATMOSPHERE);
      arnoldPluginFactory.RegisterMayaNode("volume_scattering",
                                           "aiVolumeScattering",
                                           ARNOLD_NODEID_VOLUME_SCATTERING,
                                           ARNOLD_SHADER_ATMOSPHERE);

      arnoldPluginFactory.LoadPlugins();

      CTranslatorRegistry::CreateCallbacks();
      CTranslatorRegistry::RegisterDependTranslator("aiOptions",
                                                    ARNOLD_NODEID_RENDER_OPTIONS,
                                                    "builtin",
                                                    CRenderOptionsTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("surfaceShader",
                                                    MAYA_NODEID_SURFACE_SHADER,
                                                    "builtin",
                                                    CSurfaceShaderTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("lambert",
                                                    MAYA_NODEID_LAMBERT,
                                                    "builtin",
                                                    CLambertTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("file",
                                                    MAYA_NODEID_FILE,
                                                    "builtin",
                                                    CFileTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("place2dTexture",
                                                    MAYA_NODEID_PLACE2D_TEXTURE,
                                                    "builtin",
                                                    CPlace2DTextureTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("bump2d",
                                                    MAYA_NODEID_BUMP2D,
                                                    "builtin",
                                                    CBump2DTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("bump3d",
                                                    MAYA_NODEID_BUMP3D,
                                                    "builtin",
                                                    CBump3DTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("samplerInfo",
                                                    MAYA_NODEID_SAMPLER_INFO,
                                                    "builtin",
                                                    CSamplerInfoTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("plusMinusAverage",
                                                    MAYA_NODEID_PLUS_MINUS_AVERAGE,
                                                    "builtin",
                                                    CPlusMinusAverageTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("remapValue",
                                                    MAYA_NODEID_REMAP_VALUE,
                                                    "builtin",
                                                    CRemapValueTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("remapColor",
                                                    MAYA_NODEID_REMAP_COLOR,
                                                    "builtin",
                                                    CRemapColorTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("projection",
                                                    MAYA_NODEID_PROJECTION,
                                                    "builtin",
                                                    CProjectionTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("ramp",
                                                    MAYA_NODEID_RAMP,
                                                    "builtin",
                                                    CRampTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("layeredTexture",
                                                    MAYA_NODEID_LAYERED_TEXTURE,
                                                    "builtin",
                                                    CLayeredTextureTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("layeredShader",
                                                    MAYA_NODEID_LAYERED_SHADER,
                                                    "builtin",
                                                    CLayeredShaderTranslator::creator);

      // sky is technically a DAG node, but it behaves like a DG node (i.e. it is only exported when a connection is processed)
      // therefore, it is not registered as a DagTranslator
      CTranslatorRegistry::RegisterDependTranslator("aiSky",
                                                    CArnoldSkyShaderNode::id.id(),
                                                    "builtin",
                                                    CSkyShaderTranslator::creator);

      CTranslatorRegistry::RegisterDagTranslator("directionalLight",
                                                 MAYA_NODEID_DIRECTIONAL_LIGHT,
                                                 "builtin",
                                                 CDirectionalLightTranslator::creator,
                                                 CLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("spotLight",
                                                 MAYA_NODEID_SPOT_LIGHT,
                                                 "builtin",
                                                 CSpotLightTranslator::creator,
                                                 CSpotLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("areaLight",
                                                 MAYA_NODEID_AREA_LIGHT,
                                                 "builtin",
                                                 CAreaLightTranslator::creator,
                                                 CAreaLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("pointLight",
                                                 MAYA_NODEID_POINT_LIGHT,
                                                 "builtin",
                                                 CPointLightTranslator::creator,
                                                 CPointLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("ambientLight",
                                                 MAYA_NODEID_AMBIENT_LIGHT,
                                                 "builtin",
                                                 CAmbientLightTranslator::creator,
                                                 CLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("aiSkyDomeLight",
                                                 CArnoldSkyDomeLightShaderNode::id.id(),
                                                 "builtin",
                                                 CSkyDomeLightTranslator::creator);

      CTranslatorRegistry::RegisterDagTranslator("mesh",
                                                 MAYA_NODEID_MESH,
                                                 "builtin",
                                                 CMeshTranslator::creator,
                                                 CMeshTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("nurbsSurface",
                                                 MAYA_NODEID_NURBS_SURFACE,
                                                 "builtin",
                                                 CNurbsSurfaceTranslator::creator,
                                                 CNurbsSurfaceTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("camera",
                                                 MAYA_NODEID_CAMERA,
                                                 "perspective",
                                                 CPerspCameraTranslator::creator,
                                                 CPerspCameraTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("camera",
                                                 MAYA_NODEID_CAMERA,
                                                 "orthographic",
                                                 COrthoCameraTranslator::creator,
                                                 COrthoCameraTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("camera",
                                                 MAYA_NODEID_CAMERA,
                                                 "fisheye",
                                                 CFishEyeCameraTranslator::creator,
                                                 CFishEyeCameraTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("camera",
                                                 MAYA_NODEID_CAMERA,
                                                 "cylindrical",
                                                 CCylCameraTranslator::creator,
                                                 CCylCameraTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("hairSystem",
                                                 MAYA_NODEID_HAIR,
                                                 "builtin",
                                                 CHairTranslator::creator,
                                                 CHairTranslator::NodeInitializer);

      // Load extensions last so that they can override default translators
      arnoldPluginFactory.LoadExtensions();

      // Or use MGlobal::apiVersion()
#if MAYA_API_VERSION < 201200
      MNodeClass::InitializeExistingNodes();
#endif

      return status;
   }

   MStatus UnregisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // Render Options
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);
      CHECK_MSTATUS(status);
      // Remove creation callback
      MDGMessage::removeCallback(CArnoldRenderOptionsNode::sId);

      // AOV
      status = plugin.deregisterNode(CArnoldAOVNode::id);
      CHECK_MSTATUS(status);

      // Ray switch shader
      status = plugin.deregisterNode(ARNOLD_NODEID_RAY_SWITCH);
      CHECK_MSTATUS(status);

      // Surface Shaders
      status = plugin.deregisterNode(ARNOLD_NODEID_AMBIENT_OCCLUSION);
      CHECK_MSTATUS(status);
      status = plugin.deregisterNode(ARNOLD_NODEID_STANDARD);
      CHECK_MSTATUS(status);
      status = plugin.deregisterNode(ARNOLD_NODEID_UTILITY);
      CHECK_MSTATUS(status);
      status = plugin.deregisterNode(ARNOLD_NODEID_MESH_INFO);
      CHECK_MSTATUS(status);
      status = plugin.deregisterNode(ARNOLD_NODEID_WIREFRAME);
      CHECK_MSTATUS(status);
      status = plugin.deregisterNode(ARNOLD_NODEID_HAIR);
      CHECK_MSTATUS(status);

      // Displacement Shaders
      status = plugin.deregisterNode(CArnoldDisplacementShaderNode::id);
      CHECK_MSTATUS(status);


      // Environment or Volume shaders
      status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
      CHECK_MSTATUS(status);

      CArnoldNodeFactory arnoldPluginFactory(object);
      arnoldPluginFactory.UnregisterAllNodes();
      arnoldPluginFactory.UnloadExtensions();

      CTranslatorRegistry::RemoveCallbacks();

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
   }
}

AtVoid MtoaLoggingCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs)
{
   char *buf;
   buf= new char[strlen(msg_string)+7]; // reserve and allocate memory
   sprintf(buf, "[mtoa] %s", msg_string);
   switch (severity)
   {
   case AI_SEVERITY_INFO:
      if ((logmask & AI_LOG_INFO) || (logmask & AI_LOG_DEBUG))
         MGlobal::displayInfo(buf);
      break;
   case AI_SEVERITY_WARNING:
      if (logmask & AI_LOG_WARNINGS)
         MGlobal::displayWarning(buf);
      break;
   case AI_SEVERITY_ERROR:
      if (logmask & AI_LOG_ERRORS)
         MGlobal::displayError(buf);
      break;
   default:
      // call default logging callback
      break;
   }
   if (buf!=NULL) delete[] buf; // clear memory
}


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
   if (!readMetaSuccess) {
      AiMsgError("[mtoa] Could not read mtoa built-in metadata file mtoa.mtd");
      MGlobal::displayError("[mtoa] Could not read mtoa built-in metadata file mtoa.mtd");
   }
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

   return MS::kSuccess;
}
