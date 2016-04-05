#ifdef ENABLE_VP2
#include "viewport2/ArnoldStandardShaderOverride.h"
#include "viewport2/ArnoldSkinShaderOverride.h"
#include "viewport2/ArnoldGenericShaderOverride.h"
#include "viewport2/ArnoldStandInDrawOverride.h"
#include "viewport2/ViewportUtils.h"
#include "viewport2/ArnoldVolumeDrawOverride.h"
#include "viewport2/ArnoldAreaLightDrawOverride.h"
#include "viewport2/ArnoldPhotometricLightDrawOverride.h"
#if MAYA_API_VERSION >= 201650
#include "viewport2/ArnoldSkyDomeLightGeometryOverride.h"
#include "viewport2/ArnoldLightBlockerGeometryOverride.h"
#include <maya/MSelectionMask.h>
#else
#include "viewport2/ArnoldSkyDomeLightDrawOverride.h"
#include "viewport2/ArnoldLightBlockerDrawOverride.h"
#endif
#include <maya/MDrawRegistry.h>
#endif

#include "utils/Version.h"
#include "platform/Platform.h"
#include "utils/Universe.h"

#include "commands/ArnoldAssTranslator.h"
#include "commands/ArnoldExportAssCmd.h"
#include "commands/ArnoldRenderCmd.h"
#include "commands/ArnoldIprCmd.h"
#include "commands/ArnoldBakeGeoCmd.h"
#include "commands/ArnoldRenderToTextureCmd.h"
#include "commands/ArnoldPluginCmd.h"
#include "commands/ArnoldListAttributesCmd.h"
#include "commands/ArnoldTemperatureCmd.h"
#include "commands/ArnoldFlushCmd.h"
#include "commands/ArnoldCopyAsAdminCmd.h"
#include "commands/ArnoldAIRCmd.h"
#include "commands/ArnoldRenderViewCmd.h"
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
#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/shape/ArnoldVolume.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"
#include "nodes/light/ArnoldAreaLightNode.h"
#include "nodes/light/ArnoldLightBlockerNode.h"
#include "nodes/light/ArnoldPhotometricLightNode.h"

#include "translators/options/OptionsTranslator.h"
#include "translators/camera/CameraTranslators.h"
#include "translators/light/LightTranslators.h"
#include "translators/light/LightLinkerTranslator.h"
#include "translators/light/LightBlockerTranslator.h"
#include "translators/shader/ShaderTranslators.h"
#include "translators/shape/MeshTranslator.h"
#include "translators/shape/NurbsSurfaceTranslator.h"
#include "translators/shape/HairTranslator.h"
#include "translators/shape/CurveTranslator.h"
#include "translators/shape/StandinsTranslator.h"
#include "translators/shape/ProceduralTranslator.h"
#include "translators/shape/ParticleTranslator.h"
#include "translators/shape/NParticleTranslator.h"
#include "translators/shape/InstancerTranslator.h"
#include "translators/shape/FluidTranslator.h"
#include "translators/shape/VolumeTranslator.h"
#include "translators/shader/ShadingEngineTranslator.h"
#include "translators/shader/FluidTexture2DTranslator.h"
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

#include <ai.h>

namespace // <anonymous>
{
   MCallbackId connectionCallback;

   static void SetEnv(const MString& env, const MString& val)
   {
#ifdef WIN32
   #if (_MSC_VER >= 1700) || _DEBUG // checking for vs 2012
      MGlobal::executePythonCommand(MString("import os;os.environ['")+env+MString("']=r'")+val+MString("'"));
   #else
      _putenv_s(env.asChar(), val.asChar());
   #endif
#else
      setenv(env.asChar(), val.asChar(), true);
#endif      
   }

   struct mayaCmd {
      const char* name;
      void* (*creator)();
      MSyntax (*syntax)();
   } mayaCmdList [] = {
      {"arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax},
      {"arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax},
      {"arnoldBakeGeo", CArnoldBakeGeoCmd::creator, CArnoldBakeGeoCmd::newSyntax},
      {"arnoldRenderToTexture", CArnoldRenderToTextureCmd::creator, CArnoldRenderToTextureCmd::newSyntax},
      {"arnoldExportAss", CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax},
      {"arnoldPlugins", CArnoldPluginCmd::creator, CArnoldPluginCmd::newSyntax},
      {"arnoldListAttributes", CArnoldListAttributesCmd::creator, 0},
      {"arnoldTemperatureToColor", CArnoldTemperatureCmd::creator, 0},
      {"arnoldFlushCache", CArnoldFlushCmd::creator, CArnoldFlushCmd::newSyntax},
      {"arnoldCopyAsAdmin", CArnoldCopyAsAdminCmd::creator, CArnoldCopyAsAdminCmd::newSyntax},
      {"arnoldAIR", CArnoldAIRCmd::creator, CArnoldAIRCmd::newSyntax},
      {"arnoldRenderView", CArnoldRenderViewCmd::creator, CArnoldRenderViewCmd::newSyntax}
   };

   // Note that we use drawdb/geometry/light to classify it as UI for light.
   // This will allow it to be automatically filtered out by viewport display filters.
   const MString AI_AREA_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/areaLight";
   const MString AI_AREA_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_AREA_LIGHT_CLASSIFICATION;
   const MString AI_SKYDOME_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/skydome";
   const MString AI_SKYDOME_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_SKYDOME_LIGHT_CLASSIFICATION;
   const MString AI_SKYNODE_CLASSIFICATION = "drawdb/geometry/arnold/skynode";
   const MString AI_STANDIN_CLASSIFICATION = "drawdb/geometry/arnold/standin";
   const MString AI_VOLUME_CLASSIFICATION = "drawdb/geometry/arnold/volume";
   const MString AI_PHOTOMETRIC_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/photometricLight";
   const MString AI_LIGHT_FILTER_CLASSIFICATION = "drawdb/geometry/arnold/lightFilter";
   const MString AI_PHOTOMETRIC_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_PHOTOMETRIC_LIGHT_CLASSIFICATION;
   const MString AI_SKYNODE_WITH_ENVIRONMENT_WITH_SWATCH = ENVIRONMENT_WITH_SWATCH + ":" + AI_SKYNODE_CLASSIFICATION;
   const MString AI_LIGHT_FILTER_WITH_SWATCH = LIGHT_FILTER_WITH_SWATCH + ":" + AI_LIGHT_FILTER_CLASSIFICATION;

   struct mayaNode {
      const char* name;
      const MTypeId& id;
      void* (*creator)();
      MStatus (*initialize)();
      MPxNode::Type type;
      const MString* classification;
   } mayaNodeList [] = {
      {
         "SphereLocator", CSphereLocator::id, 
         CSphereLocator::creator, CSphereLocator::initialize,
         MPxNode::kLocatorNode, 0
      } , {
         "aiOptions", CArnoldOptionsNode::id,
         CArnoldOptionsNode::creator, CArnoldOptionsNode::initialize,
         MPxNode::kDependNode, 0
      } , {
         "aiAOV", CArnoldAOVNode::id,
         CArnoldAOVNode::creator, CArnoldAOVNode::initialize,
         MPxNode::kDependNode, 0
      } , {
         "aiAOVDriver", CArnoldDriverNode::id,
         CArnoldDriverNode::creator, CArnoldDriverNode::initialize,
         MPxNode::kDependNode, 0
      } , {
         "aiAOVFilter", CArnoldFilterNode::id,
         CArnoldFilterNode::creator, CArnoldFilterNode::initialize,
         MPxNode::kDependNode, 0
      } , {
         "aiSkyDomeLight", CArnoldSkyDomeLightNode::id,
         CArnoldSkyDomeLightNode::creator, CArnoldSkyDomeLightNode::initialize,
         MPxNode::kLocatorNode, &AI_SKYDOME_LIGHT_WITH_SWATCH
      } , {
         "aiAreaLight", CArnoldAreaLightNode::id,
         CArnoldAreaLightNode::creator, CArnoldAreaLightNode::initialize,
         MPxNode::kLocatorNode, &AI_AREA_LIGHT_WITH_SWATCH
      } , {
         "aiPhotometricLight", CArnoldPhotometricLightNode::id,
         CArnoldPhotometricLightNode::creator, CArnoldPhotometricLightNode::initialize,
         MPxNode::kLocatorNode, &AI_PHOTOMETRIC_LIGHT_WITH_SWATCH
      } , {
         "aiLightBlocker", CArnoldLightBlockerNode::id,
         CArnoldLightBlockerNode::creator, CArnoldLightBlockerNode::initialize,
         MPxNode::kLocatorNode, &AI_LIGHT_FILTER_WITH_SWATCH
      } , {
         "aiSky", CArnoldSkyNode::id,
         CArnoldSkyNode::creator, CArnoldSkyNode::initialize,
         MPxNode::kLocatorNode, &AI_SKYNODE_WITH_ENVIRONMENT_WITH_SWATCH
      }
   };

#ifdef ENABLE_VP2

   struct shadingNodeOverride{
      MString classification;
      MString registrant;
      MHWRender::MPxSurfaceShadingNodeOverride* (*creator)(const MObject&);
   } shadingNodeOverrideList [] = {
      {
         "drawdb/shader/surface/arnold/standard",
         "arnoldStandardShaderOverride",
         ArnoldStandardShaderOverride::creator
      } , {
         "drawdb/shader/surface/arnold/skin",
         "arnoldSkinShaderOverride",
         ArnoldSkinShaderOverride::creator
      } , {
         "drawdb/shader/surface/arnold/genericShader",
         "arnoldGenericShaderOverride",
         ArnoldGenericShaderOverride::creator
      }
   };

   struct drawOverride{
      MString registrant;
      MString classification;
      MHWRender::MPxDrawOverride* (*creator)(const MObject&);
   } drawOverrideList [] = {
      {
         "arnoldAreaLightNodeOverride",
         AI_AREA_LIGHT_CLASSIFICATION,
         CArnoldAreaLightDrawOverride::creator
      } , 
      {
         "arnoldStandInNodeOverride",
         AI_STANDIN_CLASSIFICATION,
         CArnoldStandInDrawOverride::creator
      } , 
#if MAYA_API_VERSION < 201650
      {
         "arnoldSkyDomeLightNodeOverride",
         AI_SKYDOME_LIGHT_CLASSIFICATION,
         CArnoldSkyDomeLightDrawOverride::creator
      } , 
#endif
      {
         "arnoldPhotometricLightNodeOverride",
         AI_PHOTOMETRIC_LIGHT_CLASSIFICATION,
         CArnoldPhotometricLightDrawOverride::creator
      } ,
      {
         "arnoldVolumeNodeOverride",
         AI_VOLUME_CLASSIFICATION,
         CArnoldVolumeDrawOverride::creator
      } 
   };
#endif

   template < typename T, size_t N >
   size_t sizeOfArray(T const (&array)[ N ])
   {
      return N;
   }

   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

      // STANDINS
      status = plugin.registerShape(
                           "aiStandIn",
                           CArnoldStandInShape::id,
                           CArnoldStandInShape::creator,
                           CArnoldStandInShape::initialize,
                           CArnoldStandInShapeUI::creator,
                           &AI_STANDIN_CLASSIFICATION);
      CHECK_MSTATUS(status);
      
      // VOLUME
      status = plugin.registerShape(
                           "aiVolume",
                           CArnoldVolumeShape::id,
                           CArnoldVolumeShape::creator,
                           CArnoldVolumeShape::initialize,
                           CArnoldVolumeShapeUI::creator,
                           &AI_VOLUME_CLASSIFICATION);
      CHECK_MSTATUS(status);

      for (size_t i = 0; i < sizeOfArray(mayaNodeList); ++i)
      {
         const mayaNode& node = mayaNodeList[i];
         status = plugin.registerNode(node.name, node.id, node.creator,
                     node.initialize, node.type, node.classification);
         CHECK_MSTATUS(status);
      }

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
      builtin->RegisterTranslator("aiPhysicalSky",
                                 "",
                                 CPhysicalSkyTranslator::creator);
      builtin->RegisterTranslator("aiHair",
                                    "",
                                    CAiHairTranslator::creator,
                                    CAiHairTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiImage",
                                    "",
                                    CAiImageTranslator::creator);
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
      builtin->RegisterTranslator("aiLightBlocker",
                                    "",
                                    CLightBlockerTranslator::creator);
      // Arnold skyDomeLight node
      builtin->RegisterTranslator("aiSkyDomeLight",
                                  "",
                                  CSkyDomeLightTranslator::creator,
                                  CSkyDomeLightTranslator::NodeInitializer);
                                   
      builtin->RegisterTranslator("aiPhotometricLight",
                                    "",
                                    CPhotometricLightTranslator::creator,
                                    CPhotometricLightTranslator::NodeInitializer);

      builtin->RegisterTranslator("lightLinker",
                                    "",
                                    CLightLinkerTranslator::creator);
      // Geometry
      builtin->RegisterTranslator("mesh",
                                    "polymesh",
                                    CMeshTranslator::creator,
                                    CMeshTranslator::NodeInitializer);
      builtin->RegisterTranslator("mesh",
                                    "mesh_light",
                                    CMeshLightTranslator::creator,
                                    CMeshLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("mesh",
                                    "procedural",
                                    CArnoldProceduralTranslator::creator,
                                    CArnoldProceduralTranslator::NodeInitializer);
      builtin->RegisterTranslator("nurbsSurface",
                                    "",
                                    CNurbsSurfaceTranslator::creator,
                                    CNurbsSurfaceTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiStandIn",
                                    "",
                                    CArnoldStandInsTranslator::creator,
                                    CArnoldStandInsTranslator::NodeInitializer);
      builtin->RegisterTranslator("fluidShape",
                                    "",
                                    CFluidTranslator::creator,
                                    CFluidTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiVolume",
                                    "",
                                    CArnoldVolumeTranslator::creator,
                                    CArnoldVolumeTranslator::NodeInitializer);
      // Multiple camera translators for single Maya camera node
      builtin->RegisterTranslator("camera",
                                    "perspective",
                                    CStandardCameraTranslator::creator,
                                    CStandardCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("camera",
                                    "orthographic",
                                    CStandardCameraTranslator::creator,
                                    CStandardCameraTranslator::NodeInitializer);
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
                                    CStandardCameraTranslator::creator,
                                    CStandardCameraTranslator::NodeInitializer);
      builtin->RegisterTranslator("stereoRigCamera",
                                    "orthographic",
                                    CStandardCameraTranslator::creator,
                                    CStandardCameraTranslator::NodeInitializer);
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
      // Curves
      builtin->RegisterTranslator("nurbsCurve",
                                    "",
                                    CCurveTranslator::creator,
                                    CCurveTranslator::NodeInitializer);

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
      MString pluginPath = plugin.loadPath();
      unsigned int pluginPathLength = pluginPath.length();
      if (pluginPath.substring(pluginPathLength - 8, pluginPathLength-1) == MString("plug-ins"))
      {
         pluginPath = pluginPath.substring(0, pluginPathLength - 9);
         SetEnv("MTOA_PATH", pluginPath);
#ifdef ENABLE_VP2
         SetFragmentSearchPath(pluginPath + MString("vp2"));
#endif
         MString modulePluginPath = pluginPath + MString("shaders");
         MString moduleExtensionPath = pluginPath + MString("extensions");         
         const char* envVar = getenv("ARNOLD_PLUGIN_PATH");
         if (envVar != 0)
            SetEnv("ARNOLD_PLUGIN_PATH", (MString(envVar) + MString(PATH_SEPARATOR) + modulePluginPath));
         else
            SetEnv("ARNOLD_PLUGIN_PATH", modulePluginPath);
         envVar = getenv("MTOA_EXTENSIONS_PATH");
         if (envVar != 0)
            SetEnv("MTOA_EXTENSIONS_PATH", (MString(envVar) + MString(PATH_SEPARATOR) + moduleExtensionPath));
         else
            SetEnv("MTOA_EXTENSIONS_PATH", moduleExtensionPath);
      }
      
      shaders = CExtensionsManager::LoadArnoldPlugin("mtoa_shaders", PLUGIN_SEARCH, &status);
      CHECK_MSTATUS(status);
      // Overrides for mtoa_shaders if load was successful
      if (MStatus::kSuccess == status)
      {
         // Register nodes built into mtoa (display driver)
         InstallNodes();
         shaders->RegisterPluginNodesAndTranslators("mtoa");

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
                                       CFileTranslator::creator,
                                       CFileTranslator::NodeInitializer);
         shaders->RegisterTranslator("place2dTexture",
                                       "",
                                       CPlace2DTextureTranslator::creator);
         shaders->RegisterTranslator("bump2d",
                                       "",
                                       CBump2DTranslator::creator,
                                       CBump2DTranslator::NodeInitializer);
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
                                       CProjectionTranslator::creator,
                                       ProjectionTranslatorNodeInitializer);
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
         shaders->RegisterTranslator("phong",
                                       "",
                                       CMayaPhongTranslator::creator);
         shaders->RegisterTranslator("singleShadingSwitch",
                                       "",
                                       CreateSingleShadingSwitchTranslator);
         shaders->RegisterTranslator("doubleShadingSwitch",
                                       "",
                                       CreateDoubleShadingSwitchTranslator);
         shaders->RegisterTranslator("tripleShadingSwitch",
                                       "",
                                       CreateTripleShadingSwitchTranslator);
         shaders->RegisterTranslator("quadShadingSwitch",
                                       "",
                                       CreateQuadShadingSwitchTranslator);
         shaders->RegisterTranslator("fluidTexture2D",
                                       "",
                                       CFluidTexture2DTranslator::creator);
      }
      
      // Will load all found plugins and try to register nodes and translators


      // for the new Arnold node each create. A CExtension is initialized.
      status = CExtensionsManager::LoadExtensions(EXTENSION_SEARCH);
      status = CExtensionsManager::LoadArnoldPlugins(PLUGIN_SEARCH);      
      // Finally register all nodes from the loaded extensions with Maya in load order
      status = CExtensionsManager::RegisterExtensions();

      // CExtension::CreateCallbacks();

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

      // Nodes
      for (size_t i = 0; i < sizeOfArray(mayaNodeList); ++i)
      {
         const mayaNode& node = mayaNodeList[i];
         status = plugin.deregisterNode(node.id);
         CHECK_MSTATUS(status);
      }

      status = plugin.deregisterNode(CArnoldStandInShape::id);
      CHECK_MSTATUS(status);

      // Remove creation callback
      if (CArnoldOptionsNode::sId != 0)
      {
         MDGMessage::removeCallback(CArnoldOptionsNode::sId);
         CArnoldOptionsNode::sId = 0;
      }
      CRenderSession::ClearIdleRenderViewCallback();      
      return status;
   }

   int GetStartupLogLevel()
   {
      const char* env = getenv("MTOA_STARTUP_LOG_VERBOSITY");
      int baseFlags = AI_LOG_BACKTRACE | AI_LOG_MEMORY | AI_LOG_TIMESTAMP | AI_LOG_COLOR;
      if (env == 0)
         return AI_LOG_ERRORS | AI_LOG_WARNINGS | baseFlags;
      else
      {
         int envRes = atoi(env);
         if (envRes == 1)
            return AI_LOG_ERRORS | AI_LOG_WARNINGS | baseFlags;
         else if (envRes == 2)
            return AI_LOG_ERRORS | AI_LOG_WARNINGS | AI_LOG_INFO | baseFlags;
         else if (envRes == 3)
            return AI_LOG_ALL;
         else
            return 0;
      } 
   }

   void updateEnvironment(MPlug &srcPlug, MPlug &destPlug, bool made, void *clientData)
   {
      MString srcName = srcPlug.partialName(false, false, false, false, false, true);
      MString destName = destPlug.name();
      
      if(srcName == "message")
      {  
         if(destName == "defaultArnoldRenderOptions.background")
            MGlobal::executeCommandOnIdle("updateBackgroundSettings()");
         else if(destName == "defaultArnoldRenderOptions.atmosphere")
            MGlobal::executeCommandOnIdle("updateAtmosphereSettings()");
      }
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

   ArnoldUniverseBegin(GetStartupLogLevel());

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
      AiMsgDebug("Successfully registered Arnold ass file exporter");
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
      AiMsgDebug("Successfully registered Arnold ass file importer");
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
      AiMsgDebug("Successfully registered tx texture file");
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
      AiMsgDebug("Successfully registered Arnold swatch renderer");
   }
   else
   {
      AiMsgError("Failed to register Arnold swatch renderer");
      MGlobal::displayError("Failed to register Arnold swatch renderer");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }

   // Commands
   for (size_t i = 0; i < sizeOfArray(mayaCmdList); ++i)
   {
      const mayaCmd& cmd = mayaCmdList[i];
      status = plugin.registerCommand(cmd.name, cmd.creator, cmd.syntax);
      CHECK_MSTATUS(status);
      if (status == MS::kSuccess)
         AiMsgDebug("[mtoa] Successfully registered '%s' command.", cmd.name);
      else
      {
         AiMsgError("[mtoa] Failed to register '%s' command.\n[mtoa] Status : %s", 
                     cmd.name, status.errorString().asChar());
         MGlobal::displayError(MString("[mtoa] Failed to register '") +
                     MString(cmd.name) + MString("'' command."));
         return MStatus::kFailure;
      }
   }

   status = RegisterArnoldNodes(object);
   // Nodes
   if (MStatus::kSuccess == status)
   {
      AiMsgDebug("Successfully registered Arnold nodes");
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
      AiMsgDebug("Successfully registered renderer 'arnold'");
      MGlobal::displayInfo("Successfully registered renderer 'arnold'");
   }
   else
   {
      AiMsgError("Failed to register renderer 'arnold'");
      MGlobal::displayError("Failed to register renderer 'arnold'");
      ArnoldUniverseEnd();
      return MStatus::kFailure;
   }

#ifdef ENABLE_VP2
   for (size_t i = 0; i < sizeOfArray(shadingNodeOverrideList); ++i)
   {
      const shadingNodeOverride& override = shadingNodeOverrideList[i];
      status = MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(
               override.classification,
               override.registrant,
               override.creator);
      CHECK_MSTATUS(status);
   }

   for (size_t i = 0; i < sizeOfArray(drawOverrideList); ++i)
   {
      const drawOverride& override = drawOverrideList[i];
      status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
               override.classification,
               override.registrant,
               override.creator);
      CHECK_MSTATUS(status);
   }
	
#if MAYA_API_VERSION >= 201650
   // Skydome light and sky shader share the same override as
   // they are drawn the same way.
   status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
      AI_SKYDOME_LIGHT_CLASSIFICATION,
      "arnoldSkyDomeLightNodeOverride",
		CArnoldSkyDomeLightGeometryOverride::Creator);
   CHECK_MSTATUS(status);

   status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
      AI_SKYNODE_CLASSIFICATION,
      "arnoldSkyNodeOverride",
		CArnoldSkyDomeLightGeometryOverride::Creator);
   CHECK_MSTATUS(status);
   // Register a custom selection mask
   MSelectionMask::registerSelectionType("arnoldLightSelection", 0);
   status = MGlobal::executeCommand("selectType -byName \"arnoldLightSelection\" 1");
   CHECK_MSTATUS(status);

   status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
      AI_LIGHT_FILTER_CLASSIFICATION,
      "arnoldLightBlockerNodeOverride",
		CArnoldLightBlockerGeometryOverride::Creator);
   CHECK_MSTATUS(status);
#endif
#endif
   
   connectionCallback = MDGMessage::addConnectionCallback(updateEnvironment);

   ArnoldUniverseEnd();

   CMayaScene::Init();
   return returnStatus;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status, returnStatus;
   returnStatus = MStatus::kSuccess;

   MFnPlugin plugin(object);

   // Should be done when render finishes
   CMayaScene::End();

   ArnoldUniverseBegin(GetStartupLogLevel());

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
   for (size_t i = 0; i < sizeOfArray(mayaCmdList); ++i)
   {
      const mayaCmd& cmd = mayaCmdList[i];
      status = plugin.deregisterCommand(cmd.name);
      CHECK_MSTATUS(status);
      if (status == MStatus::kSuccess)
      {
         AiMsgDebug("[mtoa] Successfully deregistered '%s' command.", cmd.name);
         MGlobal::displayInfo(MString("[mtoa] Successfully deregisterdd '") +
                     MString(cmd.name) + MString("' command."));
      }
      else
      {
         returnStatus = MStatus::kFailure;
         AiMsgError("[mtoa] Failed to deregister '%s' command.\n[mtoa] Status : %s",
                     cmd.name, status.errorString().asChar());
         MGlobal::displayError(MString("[mtoa] Failed to deregister '") +
                     MString(cmd.name) + MString("'' command."));
      }
   }
#ifdef ENABLE_VP2
   for (size_t i = 0; i < sizeOfArray(shadingNodeOverrideList); ++i)
   {
      const shadingNodeOverride& override = shadingNodeOverrideList[i];
      status = MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(
               override.classification,
               override.registrant);
      CHECK_MSTATUS(status);
   }
   
   for (size_t i = 0; i < sizeOfArray(drawOverrideList); ++i)
   {
      const drawOverride& override = drawOverrideList[i];
      status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
               override.classification,
               override.registrant);
      CHECK_MSTATUS(status);
   }
   
   status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
      AI_LIGHT_FILTER_CLASSIFICATION,
      "arnoldLightBlockerNodeOverride");
   CHECK_MSTATUS(status);

   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
#if MAYA_API_VERSION < 201650
      CArnoldPhotometricLightDrawOverride::clearGPUResources();
      CArnoldAreaLightDrawOverride::clearGPUResources();
#endif
      CArnoldStandInDrawOverride::clearGPUResources();
      CArnoldVolumeDrawOverride::clearGPUResources();
   }
#if MAYA_API_VERSION >= 201650
   // Register a custom selection mask
   MSelectionMask::deregisterSelectionType("arnoldLightSelection");
#endif
#endif
   
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

   MMessage::removeCallback(connectionCallback);
   
   ArnoldUniverseEnd();

   CMayaScene::DeInit();
   return returnStatus;
}
