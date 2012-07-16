#include "utils/Version.h"
#include "platform/Platform.h"
#include "utils/Universe.h"

#include "commands/ArnoldAssTranslator.h"
#include "commands/ArnoldExportAssCmd.h"
#include "commands/ArnoldRenderCmd.h"
#include "commands/ArnoldIprCmd.h"
#include "commands/ArnoldPluginCmd.h"

#include "nodes/TxTextureFile.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldAOVNode.h"
#include "nodes/ArnoldDriverNode.h"
#include "nodes/ArnoldFilterNode.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/SphereLocator.h"
#include "nodes/options/ArnoldOptionsNode.h"
#include "nodes/shader/ArnoldSkyNode.h"
#include "nodes/shader/ArnoldDisplacementNode.h"
#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"
#include "nodes/light/ArnoldAreaLightNode.h"
#include "nodes/light/ArnoldLightBlockerNode.h"
#include "nodes/shader/ArnoldStandardNode.h"

#include "translators/options/OptionsTranslator.h"
#include "translators/camera/CameraTranslators.h"
#include "translators/light/LightTranslators.h"
#include "translators/light/LightLinkerTranslator.h"
#include "translators/light/LightBlockerTranslator.h"
#include "translators/shader/ShaderTranslators.h"
#include "translators/shape/MeshTranslator.h"
#include "translators/shape/NurbsSurfaceTranslator.h"
#include "translators/shape/HairTranslator.h"
#include "translators/shape/StandinsTranslator.h"
#include "translators/shape/ParticleTranslator.h"
#include "translators/shape/NParticleTranslator.h"
#include "translators/shape/InstancerTranslator.h"
#include "translators/shader/ShadingEngineTranslator.h"
#include "translators/ObjectSetTranslator.h"

#include "render/RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"

#include "scene/MayaScene.h"

#include <ai_msg.h>
#include <ai_render.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSwatchRenderRegister.h>

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
      status = plugin.registerNode("aiAOV",
                                   CArnoldAOVNode::id,
                                   CArnoldAOVNode::creator,
                                   CArnoldAOVNode::initialize);
      CHECK_MSTATUS(status);

      status = plugin.registerNode("aiAOVDriver",
                                   CArnoldDriverNode::id,
                                   CArnoldDriverNode::creator,
                                   CArnoldDriverNode::initialize);
      CHECK_MSTATUS(status);

      status = plugin.registerNode("aiAOVFilter",
                                   CArnoldFilterNode::id,
                                   CArnoldFilterNode::creator,
                                   CArnoldFilterNode::initialize);
      CHECK_MSTATUS(status);


      // Displacement Shaders
      // TODO: remove this next release (this remains only to make it easier to upgrade to new displacement method)
      status = plugin.registerNode("aiDisplacement",
                                   CArnoldDisplacementNode::id,
                                   CArnoldDisplacementNode::creator,
                                   CArnoldDisplacementNode::initialize,
                                   MPxNode::kDependNode,
                                   &DISPLACEMENT_WITH_SWATCH);
      CHECK_MSTATUS(status);

      // Light Shaders
      status = plugin.registerNode("aiSkyDomeLight",
                                   CArnoldSkyDomeLightNode::id,
                                   CArnoldSkyDomeLightNode::creator,
                                   CArnoldSkyDomeLightNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &LIGHT_WITH_SWATCH);
                                   // &lightNoSwatch);
      CHECK_MSTATUS(status);

      status = plugin.registerNode("aiAreaLight",
                                   CArnoldAreaLightNode::id,
                                   CArnoldAreaLightNode::creator,
                                   CArnoldAreaLightNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &LIGHT_WITH_SWATCH);
      CHECK_MSTATUS(status);
      
      status = plugin.registerNode("aiLightBlocker",
                                   CArnoldLightBlockerNode::id,
                                   CArnoldLightBlockerNode::creator,
                                   CArnoldLightBlockerNode::initialize,
                                   MPxNode::kLocatorNode);
      
      CHECK_MSTATUS(status);

      // Special shaders (not visible from Maya shaders menu)
      status = plugin.registerNode("aiSky",
                                   CArnoldSkyNode::id,
                                   CArnoldSkyNode::creator,
                                   CArnoldSkyNode::initialize,
                                   MPxNode::kLocatorNode,
                                   &ENVIRONMENT_WITH_SWATCH);
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
                                   CQuadLightTranslator::creator,
                                   CQuadLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("pointLight",
                                   "",
                                   CPointLightTranslator::creator,
                                   CPointLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("ambientLight",
                                   "",
                                   CAmbientLightTranslator::creator,
                                   CAmbientLightTranslator::NodeInitializer);
       // Multiple light translators for single Arnold areaLight node
       builtin->RegisterTranslator("aiAreaLight",
                                   "quad",
                                   CQuadLightTranslator::creator,
                                   CQuadLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("aiAreaLight",
                                   "cylinder",
                                   CCylinderLightTranslator::creator,
                                   CCylinderLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("aiAreaLight",
                                   "disk",
                                   CDiskLightTranslator::creator,
                                   CDiskLightTranslator::NodeInitializer);       
       builtin->RegisterTranslator("aiAreaLight",
                                   "mesh",
                                   CMeshLightTranslator::creator,
                                   CMeshLightTranslator::NodeInitializer);
       builtin->RegisterTranslator("aiLightBlocker",
                                   "",
                                   CLightBlockerTranslator::creator);
       // Arnold skyDomeLight node
       builtin->RegisterTranslator("aiSkyDomeLight",
                                   "",
                                   CSkyDomeLightTranslator::creator,
                                   CSkyDomeLightTranslator::NodeInitializer);

       builtin->RegisterTranslator("lightLinker",
                                   "",
                                   CLightLinkerTranslator::creator);

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
                                   "",
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
       builtin->RegisterTranslator("camera",
                                   "spherical",
                                   CSphericalCameraTranslator::creator,
                                   CSphericalCameraTranslator::NodeInitializer);

      // stereoCameraRig is a sub-type of the maya camera, and is also renderable
      builtin->RegisterTranslator("stereoRigCamera",
                                  "perspective",
                                  CPerspCameraTranslator::creator,
                                  CPerspCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("stereoRigCamera",
                                  "orthographic",
                                  COrthoCameraTranslator::creator,
                                  COrthoCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("stereoRigCamera",
                                  "fisheye",
                                  CFishEyeCameraTranslator::creator,
                                  CFishEyeCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("stereoRigCamera",
                                  "cylindrical",
                                  CCylCameraTranslator::creator,
                                  CCylCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("stereoRigCamera",
                                  "spherical",
                                  CSphericalCameraTranslator::creator,
                                  CSphericalCameraTranslator::NodeInitializer);
                                  
       // Hair
       builtin->RegisterTranslator("pfxHair",
                                   "",
                                   CHairTranslator::creator,
                                   CHairTranslator::NodeInitializer);

       // Particles
       builtin->RegisterTranslator("particle",
                                   "",
                                   CParticleTranslator::creator,
                                   CParticleTranslator::NodeInitializer);

       builtin->RegisterTranslator("nParticle",
                                   "",
                                   CNParticleTranslator::creator,
                                   CNParticleTranslator::NodeInitializer);

       builtin->RegisterTranslator("instancer",
                                   "",
                                   CInstancerTranslator::creator,
                                   CInstancerTranslator::NodeInitializer);

       builtin->RegisterTranslator("objectSet",
                                   "",
                                   CObjectSetTranslator::creator,
                                   CObjectSetTranslator::NodeInitializer);

      // Load all plugins path or only shaders?
      CExtension* shaders;
      shaders = CExtensionsManager::LoadArnoldPlugin("mtoa_shaders", "$ARNOLD_PLUGIN_PATH", &status);
      CHECK_MSTATUS(status);
      // Overrides for mtoa_shaders if load was successful
      if (MStatus::kSuccess == status)
      {
         shaders->RegisterTranslator("lambert",
                                     "",
                                     CLambertTranslator::creator);
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
         shaders->RegisterTranslator("particleSamplerInfo",
                                     "",
                                     CParticleSamplerInfoTranslator::creator);
         shaders->RegisterTranslator("remapValue",
                                     "",
                                     CRemapValueTranslator::creator);
         shaders->RegisterTranslator("remapColor",
                                     "",
                                     CRemapColorTranslator::creator);
         shaders->RegisterTranslator("remapHsv",
                                     "",
                                     CRemapHsvTranslator::creator);
         shaders->RegisterTranslator("projection",
                                     "",
                                     CProjectionTranslator::creator);
         shaders->RegisterTranslator("ramp",
                                     "",
                                     CRampTranslator::creator);

         shaders->RegisterTranslator("animCurveTA",
                                     "",
                                     CAnimCurveTranslator::creator);
         shaders->RegisterTranslator("animCurveTL",
                                     "",
                                     CAnimCurveTranslator::creator);
         shaders->RegisterTranslator("animCurveTU",
                                     "",
                                     CAnimCurveTranslator::creator);

         shaders->RegisterTranslator("shadingEngine",
                                     "",
                                     CShadingEngineTranslator::creator,
                                     CShadingEngineTranslator::NodeInitializer);
         shaders->RegisterTranslator("displacementShader",
                                     "",
                                     CDisplacementTranslator::creator,
                                     DisplacementTranslatorNodeInitializer);         
         shaders->RegisterTranslator("blinn",
                                     "",
                                     CMayaBlinnTranslator::creator);
      }

      // Will load all found plugins and try to register nodes and translators


      // for the new Arnold node each create. A CExtension is initialized.
      status = CExtensionsManager::LoadExtensions();
      status = CExtensionsManager::LoadArnoldPlugins();
      // Finally register all nodes from the loaded extensions with Maya in load order
      status = CExtensionsManager::RegisterExtensions();

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
      if (CArnoldOptionsNode::sId != 0)
      {
         MDGMessage::removeCallback(CArnoldOptionsNode::sId);
         CArnoldOptionsNode::sId = 0;
      }
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
   // This will dump memory leak info in debugger output window on program exit.
#ifdef _DEBUG
   _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

   MStatus status, returnStatus;
   returnStatus = MStatus::kSuccess;

   MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   // Load metadata for builtin (mtoa.mtd)
   MString loadpath = plugin.loadPath();
   MString metafile = loadpath + "/" + "mtoa.mtd";
   SetMetafile(metafile);

   ArnoldUniverseBegin();

   // ASS file translator
   status = plugin.registerFileTranslator(CArnoldAssTranslator::fileTypeExport,
         CArnoldAssTranslator::fileIcon,
         CArnoldAssTranslator::exportCreator,
         CArnoldAssTranslator::optionScriptExport,
         CArnoldAssTranslator::optionDefault,
         false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered Arnold ass file exporter");
   }
   else
   {
      AiMsgError("Failed to register Arnold ass file exporter");
      MGlobal::displayError("Failed to register Arnold ass file exporter");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   
   status = plugin.registerFileTranslator(CArnoldAssTranslator::fileTypeImport,
         CArnoldAssTranslator::fileIcon,
         CArnoldAssTranslator::importCreator,
         CArnoldAssTranslator::optionScriptImport,
         CArnoldAssTranslator::optionDefault,
         false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered Arnold ass file importer");
   }
   else
   {
      AiMsgError("Failed to register Arnold ass file importer");
      MGlobal::displayError("Failed to register Arnold ass file importer");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   
   // Register image formats
   MStringArray extensions;
   extensions.append("tex");
   extensions.append("tx");
   plugin.registerImageFile(CTxTextureFile::fileName,
                           CTxTextureFile::creator, 
                           extensions);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered tx texture file");
   }
   else
   {
      AiMsgError("Failed to register tx texture file");
      MGlobal::displayError("Failed to register tx texture file");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   
   
   // Swatch renderer
   status = MSwatchRenderRegister::registerSwatchRender(ARNOLD_SWATCH, CRenderSwatchGenerator::creator);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered Arnold swatch renderer");
   }
   else
   {
      AiMsgError("Failed to register Arnold swatch renderer");
      MGlobal::displayError("Failed to register Arnold swatch renderer");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   // Commands
   status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered 'arnoldRender' command");
   }
   else
   {
      AiMsgError("Failed to register 'arnoldRender' command");
      MGlobal::displayError("Failed to register 'arnoldRender' command");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered 'arnoldIpr' command");
   }
   else
   {
      AiMsgError("Failed to register 'arnoldIpr' command");
      MGlobal::displayError("Failed to register 'arnoldIpr' command");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   status = plugin.registerCommand("arnoldExportAss", CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered 'arnoldExportAss' command");
   }
   else
   {
      AiMsgError("Failed to register 'arnoldExportAss' command");
      MGlobal::displayError("Failed to register 'arnoldExportAss' command");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   status = plugin.registerCommand("arnoldPlugins", CArnoldPluginCmd::creator, CArnoldPluginCmd::newSyntax);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered 'arnoldPlugins' command");
   }
   else
   {
      AiMsgError("Failed to register 'arnoldPlugins' command");
      MGlobal::displayError("Failed to register 'arnoldPlugins' command");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }

   status = RegisterArnoldNodes(object);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered Arnold nodes");
   }
   else
   {
      AiMsgError("Failed to register one or more Arnold nodes");
      MGlobal::displayError("Failed to register one or more Arnold nodes");
      // NOTE: allow to keep going
      // ArnoldUniverseEnd();
      // return MStatus::kFailure;
   }

   // Since executePythonCommand eats error output, trying to see if we can access every required module
   status = MGlobal::executePythonCommand(MString("import arnold"), true, false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully imported python module 'arnold'");
      MGlobal::displayInfo("Successfully imported python module 'arnold'");
   }
   else
   {
      AiMsgError("Failed to import python module 'arnold'");
      MGlobal::displayError("Failed to import python module 'arnold'");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }
   status = MGlobal::executePythonCommand(MString("import mtoa"), true, false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully imported python module 'mtoa'");
      MGlobal::displayInfo("Successfully imported python module 'mtoa'");
   }
   else
   {
      AiMsgError("Failed to import python module 'mtoa'");
      MGlobal::displayError("Failed to import python module 'mtoa'");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }

   // Register the Arnold renderer
   status = MGlobal::executePythonCommand(MString("import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()"), true, false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully registered renderer 'arnold'");
      MGlobal::displayInfo("Successfully registered renderer 'arnold'");
   }
   else
   {
      AiMsgError("Failed to register renderer 'arnold'");
      MGlobal::displayError("Failed to register renderer 'arnold'");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }

   ArnoldUniverseEnd();

   return returnStatus;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status, returnStatus;
   returnStatus = MStatus::kSuccess;

   MFnPlugin plugin(object);

#if MAYA_API_VERSION < 201200
   // Remove custom MNodeClass class callbacks
   MNodeClass::RemoveCallbacks();
#endif

   // Should be done when render finishes
   CMayaScene::End();

   ArnoldUniverseBegin();

   status = MGlobal::executePythonCommand(MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"), true, false);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered renderer 'arnold'");
      MGlobal::displayInfo("Successfully deregistered renderer 'arnold'");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister renderer 'arnold'");
      MGlobal::displayError("Failed to deregister renderer 'arnold'");
   }

   status = UnregisterArnoldNodes(object);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered Arnold nodes");
      MGlobal::displayInfo("Successfully deregistered Arnold nodes");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister Arnold nodes");
      MGlobal::displayError("Failed to deregister Arnold nodes");
   }

   // Deregister in inverse order of registration
   // Commands
   status = plugin.deregisterCommand("arnoldPlugins");
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered 'arnoldPlugins' command");
      MGlobal::displayInfo("Successfully deregistered 'arnoldPlugins' command");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister 'arnoldPlugins' command");
      MGlobal::displayError("Failed to deregister 'arnoldPlugins' command");
   }
   status = plugin.deregisterCommand("arnoldExportAss");
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered 'arnoldExportAss' command");
      MGlobal::displayInfo("Successfully deregistered 'arnoldExportAss' command");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister 'arnoldExportAss' command");
      MGlobal::displayError("Failed to deregister 'arnoldExportAss' command");
   }
   status = plugin.deregisterCommand("arnoldIpr");
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered 'arnoldIpr' command");
      MGlobal::displayInfo("Successfully deregistered 'arnoldIpr' command");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister 'arnoldIpr' command");
      MGlobal::displayError("Failed to deregister 'arnoldIpr' command");
   }
   status = plugin.deregisterCommand("arnoldRender");
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered 'arnoldRender' command");
      MGlobal::displayInfo("Successfully deregistered 'arnoldRender' command");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister 'arnoldRender' command");
      MGlobal::displayError("Failed to deregister 'arnoldRender' command");
   }
   // Swatch renderer
   status = MSwatchRenderRegister::unregisterSwatchRender(ARNOLD_SWATCH);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered Arnold swatch renderer");
      MGlobal::displayInfo("Successfully deregistered Arnold swatch renderer");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister Arnold swatch renderer");
      MGlobal::displayError("Failed to deregister Arnold swatch renderer");
   }
   
   // Deregister image formats
   status = plugin.deregisterImageFile(CTxTextureFile::fileName);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered tx texture file");
      MGlobal::displayInfo("Successfully deregistered tx texture file");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister tx texture file");
      MGlobal::displayError("Failed to deregister tx texture file");
   }
   
   // ASS file translator
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeExport);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered Arnold ass file exporter");
      MGlobal::displayInfo("Successfully deregistered Arnold ass file exporter");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister Arnold ass file exporter");
      MGlobal::displayError("Failed to deregister Arnold ass file exporter");
   }
   
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeImport);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered Arnold ass file importer");
      MGlobal::displayInfo("Successfully deregistered Arnold ass file importer");
   }
   else
   {
      returnStatus = MStatus::kFailure;
      AiMsgError("Failed to deregister Arnold ass file importer");
      MGlobal::displayError("Failed to deregister Arnold ass file importer");
   }

   ArnoldUniverseEnd();

   return returnStatus;
}
