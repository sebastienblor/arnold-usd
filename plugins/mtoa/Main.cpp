
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
#include "scene/Extension.h"
#include "render/RenderSwatch.h"

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

      // .ext is included so that the file path parsing logic works properly
      CExtension* builtinExtension = CExtension::Init("mtoa.ext");
      // sky is technically a DAG node, but it behaves like a DG node (i.e. it is only exported when a connection is processed)
      builtinExtension->RegisterTranslator("aiOptions",
                                                 ARNOLD_NODEID_RENDER_OPTIONS,
                                                 "builtin",
                                                 CRenderOptionsTranslator::creator);
      builtinExtension->RegisterTranslator("surfaceShader",
                                                 MAYA_NODEID_SURFACE_SHADER,
                                                 "builtin",
                                                 CSurfaceShaderTranslator::creator);
      builtinExtension->RegisterTranslator("lambert",
                                                 MAYA_NODEID_LAMBERT,
                                                 "builtin",
                                                 CLambertTranslator::creator);
      builtinExtension->RegisterTranslator("layeredShader",
                                                 MAYA_NODEID_LAYERED_SHADER,
                                                 "builtin",
                                                 CLayeredShaderTranslator::creator);
      builtinExtension->RegisterTranslator("file",
                                                 MAYA_NODEID_FILE,
                                                 "builtin",
                                                 CFileTranslator::creator);
      builtinExtension->RegisterTranslator("place2dTexture",
                                                 MAYA_NODEID_PLACE2D_TEXTURE,
                                                 "builtin",
                                                 CPlace2DTextureTranslator::creator);
      builtinExtension->RegisterTranslator("bump2d",
                                                 MAYA_NODEID_BUMP2D,
                                                 "builtin",
                                                 CBump2DTranslator::creator);
      builtinExtension->RegisterTranslator("bump3d",
                                                 MAYA_NODEID_BUMP3D,
                                                 "builtin",
                                                 CBump3DTranslator::creator);
      builtinExtension->RegisterTranslator("samplerInfo",
                                                 MAYA_NODEID_SAMPLER_INFO,
                                                 "builtin",
                                                 CSamplerInfoTranslator::creator);
      builtinExtension->RegisterTranslator("plusMinusAverage",
                                                 MAYA_NODEID_PLUS_MINUS_AVERAGE,
                                                 "builtin",
                                                 CPlusMinusAverageTranslator::creator);
      builtinExtension->RegisterTranslator("remapValue",
                                                 MAYA_NODEID_REMAP_VALUE,
                                                 "builtin",
                                                 CRemapValueTranslator::creator);
      builtinExtension->RegisterTranslator("remapColor",
                                                 MAYA_NODEID_REMAP_COLOR,
                                                 "builtin",
                                                 CRemapColorTranslator::creator);
      builtinExtension->RegisterTranslator("projection",
                                                 MAYA_NODEID_PROJECTION,
                                                 "builtin",
                                                 CProjectionTranslator::creator);
      builtinExtension->RegisterTranslator("ramp",
                                                 MAYA_NODEID_RAMP,
                                                 "builtin",
                                                 CRampTranslator::creator);
      builtinExtension->RegisterTranslator("layeredTexture",
                                                 MAYA_NODEID_LAYERED_TEXTURE,
                                                 "builtin",
                                                 CLayeredTextureTranslator::creator);
      // therefore, it is not registered as a DagTranslator


      builtinExtension->RegisterTranslator("aiSky",
                                                 CArnoldSkyShaderNode::id.id(),
                                                 "builtin",
                                                 CSkyShaderTranslator::creator);

      builtinExtension->RegisterTranslator("directionalLight",
                                              MAYA_NODEID_DIRECTIONAL_LIGHT,
                                              "builtin",
                                              CDirectionalLightTranslator::creator,
                                              CDirectionalLightTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("spotLight",
                                              MAYA_NODEID_SPOT_LIGHT,
                                              "builtin",
                                              CSpotLightTranslator::creator,
                                              CSpotLightTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("areaLight",
                                              MAYA_NODEID_AREA_LIGHT,
                                              "builtin",
                                              CAreaLightTranslator::creator,
                                              CAreaLightTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("pointLight",
                                              MAYA_NODEID_POINT_LIGHT,
                                              "builtin",
                                              CPointLightTranslator::creator,
                                              CPointLightTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("ambientLight",
                                              MAYA_NODEID_AMBIENT_LIGHT,
                                              "builtin",
                                              CAmbientLightTranslator::creator,
                                              CLightTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("aiSkyDomeLight",
                                              CArnoldSkyDomeLightShaderNode::id.id(),
                                              "builtin",
                                              CSkyDomeLightTranslator::creator);

      builtinExtension->RegisterTranslator("mesh",
                                              MAYA_NODEID_MESH,
                                              "builtin",
                                              CMeshTranslator::creator,
                                              CMeshTranslator::NodeInitializer);
      builtinExtension->RegisterTranslator("nurbsSurface",
                                              MAYA_NODEID_NURBS_SURFACE,
                                              "builtin",
                                              CNurbsSurfaceTranslator::creator,
                                              CNurbsSurfaceTranslator::NodeInitializer);

      builtinExtension->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "perspective",
                                              CPerspCameraTranslator::creator,
                                              CPerspCameraTranslator::NodeInitializer);

      builtinExtension->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "orthographic",
                                              COrthoCameraTranslator::creator,
                                              COrthoCameraTranslator::NodeInitializer);

      builtinExtension->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "fisheye",
                                              CFishEyeCameraTranslator::creator,
                                              CFishEyeCameraTranslator::NodeInitializer);

      builtinExtension->RegisterTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "cylindrical",
                                              CCylCameraTranslator::creator,
                                              CCylCameraTranslator::NodeInitializer);

      builtinExtension->RegisterTranslator("hairSystem",
                                              MAYA_NODEID_HAIR,
                                              "builtin",
                                              CHairTranslator::creator,
                                              CHairTranslator::NodeInitializer);

      CLoader::LoadExtensions();

      // Start up the plugin factory.
      CArnoldNodeFactory::SetMayaPlugin(object);

      // Create nodes for the builtin shaders: empty string is how AiNodeEntryGetFilename identifies builtin shaders
      // FIXME: node factory should accept pointer
      CArnoldNodeFactory* builtinShaders = CArnoldNodeFactory::Init(builtinExtension, "");
      builtinShaders->CreateNodes();

      CLoader::LoadPlugins();
      CExtension::CreateCallbacks();

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

      CArnoldNodeFactory::UnloadPlugins();
      //arnoldPluginFactory.UnregisterAllNodes();
      //arnoldPluginFactory.UnloadExtensions();

      //CExtension::RemoveCallbacks();

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
