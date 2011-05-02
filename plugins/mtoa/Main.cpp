
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
      builtinExtension->RegisterDependTranslator("aiOptions",
                                                 ARNOLD_NODEID_RENDER_OPTIONS,
                                                 "builtin",
                                                 CRenderOptionsTranslator::creator);
      builtinExtension->RegisterDependTranslator("surfaceShader",
                                                 MAYA_NODEID_SURFACE_SHADER,
                                                 "builtin",
                                                 CSurfaceShaderTranslator::creator);
      builtinExtension->RegisterDependTranslator("lambert",
                                                 MAYA_NODEID_LAMBERT,
                                                 "builtin",
                                                 CLambertTranslator::creator);
      builtinExtension->RegisterDependTranslator("layeredShader",
                                                 MAYA_NODEID_LAYERED_SHADER,
                                                 "builtin",
                                                 CLayeredShaderTranslator::creator);
      builtinExtension->RegisterDependTranslator("file",
                                                 MAYA_NODEID_FILE,
                                                 "builtin",
                                                 CFileTranslator::creator);
      builtinExtension->RegisterDependTranslator("place2dTexture",
                                                 MAYA_NODEID_PLACE2D_TEXTURE,
                                                 "builtin",
                                                 CPlace2DTextureTranslator::creator);
      builtinExtension->RegisterDependTranslator("bump2d",
                                                 MAYA_NODEID_BUMP2D,
                                                 "builtin",
                                                 CBump2DTranslator::creator);
      builtinExtension->RegisterDependTranslator("bump3d",
                                                 MAYA_NODEID_BUMP3D,
                                                 "builtin",
                                                 CBump3DTranslator::creator);
      builtinExtension->RegisterDependTranslator("samplerInfo",
                                                 MAYA_NODEID_SAMPLER_INFO,
                                                 "builtin",
                                                 CSamplerInfoTranslator::creator);
      builtinExtension->RegisterDependTranslator("plusMinusAverage",
                                                 MAYA_NODEID_PLUS_MINUS_AVERAGE,
                                                 "builtin",
                                                 CPlusMinusAverageTranslator::creator);
      builtinExtension->RegisterDependTranslator("remapValue",
                                                 MAYA_NODEID_REMAP_VALUE,
                                                 "builtin",
                                                 CRemapValueTranslator::creator);
      builtinExtension->RegisterDependTranslator("remapColor",
                                                 MAYA_NODEID_REMAP_COLOR,
                                                 "builtin",
                                                 CRemapColorTranslator::creator);
      builtinExtension->RegisterDependTranslator("projection",
                                                 MAYA_NODEID_PROJECTION,
                                                 "builtin",
                                                 CProjectionTranslator::creator);
      builtinExtension->RegisterDependTranslator("ramp",
                                                 MAYA_NODEID_RAMP,
                                                 "builtin",
                                                 CRampTranslator::creator);
      builtinExtension->RegisterDependTranslator("layeredTexture",
                                                 MAYA_NODEID_LAYERED_TEXTURE,
                                                 "builtin",
                                                 CLayeredTextureTranslator::creator);
      // therefore, it is not registered as a DagTranslator


      builtinExtension->RegisterDependTranslator("aiSky",
                                                 CArnoldSkyShaderNode::id.id(),
                                                 "builtin",
                                                 CSkyShaderTranslator::creator);

      builtinExtension->RegisterDagTranslator("directionalLight",
                                              MAYA_NODEID_DIRECTIONAL_LIGHT,
                                              "builtin",
                                              CDirectionalLightTranslator::creator,
                                              CLightTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("spotLight",
                                              MAYA_NODEID_SPOT_LIGHT,
                                              "builtin",
                                              CSpotLightTranslator::creator,
                                              CSpotLightTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("areaLight",
                                              MAYA_NODEID_AREA_LIGHT,
                                              "builtin",
                                              CAreaLightTranslator::creator,
                                              CAreaLightTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("pointLight",
                                              MAYA_NODEID_POINT_LIGHT,
                                              "builtin",
                                              CPointLightTranslator::creator,
                                              CPointLightTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("ambientLight",
                                              MAYA_NODEID_AMBIENT_LIGHT,
                                              "builtin",
                                              CAmbientLightTranslator::creator,
                                              CLightTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("aiSkyDomeLight",
                                              CArnoldSkyDomeLightShaderNode::id.id(),
                                              "builtin",
                                              CSkyDomeLightTranslator::creator);

      builtinExtension->RegisterDagTranslator("mesh",
                                              MAYA_NODEID_MESH,
                                              "builtin",
                                              CMeshTranslator::creator,
                                              CMeshTranslator::NodeInitializer);
      builtinExtension->RegisterDagTranslator("nurbsSurface",
                                              MAYA_NODEID_NURBS_SURFACE,
                                              "builtin",
                                              CNurbsSurfaceTranslator::creator,
                                              CNurbsSurfaceTranslator::NodeInitializer);

      builtinExtension->RegisterDagTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "perspective",
                                              CPerspCameraTranslator::creator,
                                              CPerspCameraTranslator::NodeInitializer);

      builtinExtension->RegisterDagTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "orthographic",
                                              COrthoCameraTranslator::creator,
                                              COrthoCameraTranslator::NodeInitializer);

      builtinExtension->RegisterDagTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "fisheye",
                                              CFishEyeCameraTranslator::creator,
                                              CFishEyeCameraTranslator::NodeInitializer);

      builtinExtension->RegisterDagTranslator("camera",
                                              MAYA_NODEID_CAMERA,
                                              "cylindrical",
                                              CCylCameraTranslator::creator,
                                              CCylCameraTranslator::NodeInitializer);

      builtinExtension->RegisterDagTranslator("hairSystem",
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
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);
      CHECK_MSTATUS(status);
      // Remove creation callback
      MDGMessage::removeCallback(CArnoldRenderOptionsNode::sId);

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
