
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
   char *header = "[mtoa] %s";
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
      CExtension* builtin = CExtensionsManager::GetBuiltin();
      // Load all plugins path or only shaders?
      // TODO: what should be in builtin or get it's own extension ?
      builtin->LoadArnoldPlugin("mtoa_shaders");

      // Add Translators for new nodes
      builtin->RegisterTranslator("options",
                                  "aiOptions",
                                  "<built-in>",
                                  CRenderOptionsTranslator::creator);
      builtin->RegisterTranslator("surfaceShader",
                                  "surfaceShader",
                                  "<built-in>",
                                  CSurfaceShaderTranslator::creator);
      builtin->RegisterTranslator("lambert",
                                  "lambert", "<built-in>",
                                  CLambertTranslator::creator);
      builtin->RegisterTranslator("layeredShader",
                                  "layeredShader",
                                  "<built-in>",
                                  CLayeredShaderTranslator::creator);
      builtin->RegisterTranslator("file",
                                  "file",
                                  "<built-in>",
                                  CFileTranslator::creator);
      builtin->RegisterTranslator("place2dTexture",
                                                 MAYA_NODEID_PLACE2D_TEXTURE,
                                                 "builtin",
                                                 CPlace2DTextureTranslator::creator);
      builtin->RegisterTranslator("bump2d",
                                                 MAYA_NODEID_BUMP2D,
                                                 "builtin",
                                                 CBump2DTranslator::creator);
      builtin->RegisterTranslator("bump3d",
                                                 MAYA_NODEID_BUMP3D,
                                                 "builtin",
                                                 CBump3DTranslator::creator);
      builtin->RegisterTranslator("samplerInfo",
                                                 MAYA_NODEID_SAMPLER_INFO,
                                                 "builtin",
                                                 CSamplerInfoTranslator::creator);
      builtin->RegisterTranslator("plusMinusAverage",
                                                 MAYA_NODEID_PLUS_MINUS_AVERAGE,
                                                 "builtin",
                                                 CPlusMinusAverageTranslator::creator);
      builtin->RegisterTranslator("remapValue",
                                                 MAYA_NODEID_REMAP_VALUE,
                                                 "builtin",
                                                 CRemapValueTranslator::creator);
      builtin->RegisterTranslator("remapColor",
                                                 MAYA_NODEID_REMAP_COLOR,
                                                 "builtin",
                                                 CRemapColorTranslator::creator);
      builtin->RegisterTranslator("projection",
                                                 MAYA_NODEID_PROJECTION,
                                                 "builtin",
                                                 CProjectionTranslator::creator);
      builtin->RegisterTranslator("ramp",
                                                 MAYA_NODEID_RAMP,
                                                 "builtin",
                                                 CRampTranslator::creator);
      builtin->RegisterTranslator("layeredTexture",
                                                 MAYA_NODEID_LAYERED_TEXTURE,
                                                 "builtin",
                                                 CLayeredTextureTranslator::creator);
      // A Dag node in Maya but a depend node in Arnold
      builtin->RegisterTranslator("aiSky",
                                                 CArnoldSkyShaderNode::id.id(),
                                                 "builtin",
                                                 CSkyShaderTranslator::creator);
      // Lights
      builtin->RegisterTranslator("directionalLight",
                                              MAYA_NODEID_DIRECTIONAL_LIGHT,
                                              "builtin",
                                              CDirectionalLightTranslator::creator,
                                              CDirectionalLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("spotLight",
                                              MAYA_NODEID_SPOT_LIGHT,
                                              "builtin",
                                              CSpotLightTranslator::creator,
                                              CSpotLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("areaLight",
                                              MAYA_NODEID_AREA_LIGHT,
                                              "builtin",
                                              CAreaLightTranslator::creator,
                                              CAreaLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("pointLight",
                                              MAYA_NODEID_POINT_LIGHT,
                                              "builtin",
                                              CPointLightTranslator::creator,
                                              CPointLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("ambientLight",
                                              MAYA_NODEID_AMBIENT_LIGHT,
                                              "builtin",
                                              CAmbientLightTranslator::creator,
                                              CLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiSkyDomeLight",
                                              CArnoldSkyDomeLightShaderNode::id.id(),
                                              "builtin",
                                              CSkyDomeLightTranslator::creator);
      // Geometry
      builtin->RegisterTranslator("mesh",
                                              MAYA_NODEID_MESH,
                                              "builtin",
                                              CMeshTranslator::creator,
                                              CMeshTranslator::NodeInitializer);
      builtin->RegisterTranslator("nurbsSurface",
                                              MAYA_NODEID_NURBS_SURFACE,
                                              "builtin",
                                              CNurbsSurfaceTranslator::creator,
                                              CNurbsSurfaceTranslator::NodeInitializer);
      // Multiple camera translators for single Maya camera node
      builtin->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "perspective",
                                              CPerspCameraTranslator::creator,
                                              CPerspCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "orthographic",
                                              COrthoCameraTranslator::creator,
                                              COrthoCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "fisheye",
                                              CFishEyeCameraTranslator::creator,
                                              CFishEyeCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "cylindrical",
                                              CCylCameraTranslator::creator,
                                              CCylCameraTranslator::NodeInitializer);
      // Hair
      builtin->RegisterTranslator("hairSystem",
                                              MAYA_NODEID_HAIR,
                                              "builtin",
                                              CHairTranslator::creator,
                                              CHairTranslator::NodeInitializer);

      CExtensionsManager::LoadArnoldPlugins();

      // CExtension::CreateCallbacks();

      // Or use MGlobal::apiVersion()
      // TODO : should be called by ExtensionsManager
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
      //CExtensionsManager::UnloadExtensions();

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
