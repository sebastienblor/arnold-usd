
#include "platform/Platform.h"
#include "ArnoldAssTranslator.h"
#include "ArnoldExportAssCmd.h"
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "nodes/ArnoldNodeFactory.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldAOV.h"
#include "nodes/shaders/atmosphere/ArnoldFogShader.h"
#include "nodes/shaders/atmosphere/ArnoldVolumeScatteringShader.h"
#include "nodes/shaders/background/SphereLocator.h"
#include "nodes/shaders/background/ArnoldSkyShader.h"
#include "nodes/shaders/displacement/ArnoldDisplacementShader.h"
#include "nodes/shaders/light/ArnoldBarndoorShader.h"
#include "nodes/shaders/light/ArnoldGoboShader.h"
#include "nodes/shaders/light/ArnoldLightBlockerShader.h"
#include "nodes/shaders/light/ArnoldLightDecayShader.h"
#include "nodes/shaders/light/ArnoldSkyDomeLightShader.h"
#include "nodes/shaders/surface/ArnoldAmbientOcclusionShader.h"
#include "nodes/shaders/surface/ArnoldStandardShader.h"
#include "nodes/shaders/surface/ArnoldUtilityShader.h"
#include "nodes/shaders/surface/ArnoldMeshInfoShader.h"
#include "nodes/shaders/surface/ArnoldWireframeShader.h"
#include "nodes/shaders/surface/ArnoldHairShader.h"
#include "nodes/shaders/surface/ArnoldRaySwitchShader.h"
#include "scene/Shaders.h"
#include "scene/Lights.h"
#include "scene/Geometry.h"
#include "scene/Cameras.h"
#include "scene/Options.h"
#include "scene/Hair.h"
#include "scene/TranslatorRegistry.h"
#include "render/RenderSwatch.h"

#include <ai_render.h>
#include <ai_msg.h>

#include <maya/MSwatchRenderRegister.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#define MTOA_VENDOR "SolidAngle"
#define MTOA_VERSION "0.6.0.dev"
#define MAYA_VERSION "Any"

namespace // <anonymous>
{
   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);
      const MString ShaderClass("shader/surface");
      const MString DisplacementClass("shader/displacement");
      const MString LightClass("light");

      // Abstract Classes
      status = plugin.registerNode("SphereLocator", CSphereLocator::id, CSphereLocator::creator, CSphereLocator::initialize, MPxNode::kLocatorNode);

      // RENDER OPTIONS
      status = plugin.registerNode("ArnoldRenderOptions", CArnoldRenderOptionsNode::id, CArnoldRenderOptionsNode::creator, CArnoldRenderOptionsNode::initialize);

      // AOV
      status = plugin.registerNode("ArnoldAOV", CArnoldAOVNode::id, CArnoldAOVNode::creator, CArnoldAOVNode::initialize);

      // Swatch renderer.
      static MString swatchName("ArnoldRenderSwatch");
      MSwatchRenderRegister::registerSwatchRender(swatchName, CRenderSwatchGenerator::creator );

      // Ray switch shader [could be surface or environment...]
      status = plugin.registerNode("ArnoldRaySwitchShader", CArnoldRaySwitchShaderNode::id, CArnoldRaySwitchShaderNode::creator, CArnoldRaySwitchShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      // Surface Shaders
      status = plugin.registerNode("ArnoldAmbientOcclusionShader", CArnoldAmbientOcclusionShaderNode::id, CArnoldAmbientOcclusionShaderNode::creator, CArnoldAmbientOcclusionShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldStandardShader", CArnoldStandardShaderNode::id, CArnoldStandardShaderNode::creator, CArnoldStandardShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldUtilityShader", CArnoldUtilityShaderNode::id, CArnoldUtilityShaderNode::creator, CArnoldUtilityShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldMeshInfoShader", CArnoldMeshInfoShaderNode::id, CArnoldMeshInfoShaderNode::creator, CArnoldMeshInfoShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldWireframeShader", CArnoldWireframeShaderNode::id, CArnoldWireframeShaderNode::creator, CArnoldWireframeShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);
      status = plugin.registerNode("ArnoldHairShader", CArnoldHairShaderNode::id, CArnoldHairShaderNode::creator, CArnoldHairShaderNode::initialize, MPxNode::kDependNode, &ShaderClass);

      // Displacement Shaders
      status = plugin.registerNode("ArnoldDisplacementShader", CArnoldDisplacementShaderNode::id, CArnoldDisplacementShaderNode::creator, CArnoldDisplacementShaderNode::initialize, MPxNode::kDependNode, &DisplacementClass);

      // Light Shaders
      status = plugin.registerNode("ArnoldBarndoorShader", CArnoldBarndoorShaderNode::id, CArnoldBarndoorShaderNode::creator, CArnoldBarndoorShaderNode::initialize);
      status = plugin.registerNode("ArnoldGoboShader", CArnoldGoboShaderNode::id, CArnoldGoboShaderNode::creator, CArnoldGoboShaderNode::initialize);
      status = plugin.registerNode("ArnoldLightBlockerShader", CArnoldLightBlockerShaderNode::id, CArnoldLightBlockerShaderNode::creator, CArnoldLightBlockerShaderNode::initialize);
      status = plugin.registerNode("ArnoldLightDecayShader", CArnoldLightDecayShaderNode::id, CArnoldLightDecayShaderNode::creator, CArnoldLightDecayShaderNode::initialize);
      status = plugin.registerNode("ArnoldSkyDomeLightShader", CArnoldSkyDomeLightShaderNode::id, CArnoldSkyDomeLightShaderNode::creator, CArnoldSkyDomeLightShaderNode::initialize, MPxNode::kLocatorNode);

      // Special shaders (not visible from Maya shaders menu)
      status = plugin.registerNode("ArnoldFogShader", CArnoldFogShaderNode::id, CArnoldFogShaderNode::creator, CArnoldFogShaderNode::initialize);
      status = plugin.registerNode("ArnoldSkyShader", CArnoldSkyShaderNode::id, CArnoldSkyShaderNode::creator, CArnoldSkyShaderNode::initialize, MPxNode::kLocatorNode, &LightClass);
      status = plugin.registerNode("ArnoldVolumeScatteringShader", CArnoldVolumeScatteringShaderNode::id, CArnoldVolumeScatteringShaderNode::creator, CArnoldVolumeScatteringShaderNode::initialize);

      CArnoldNodeFactory* arnoldPluginFactory;
      arnoldPluginFactory = new CArnoldNodeFactory(object);

      arnoldPluginFactory->LoadPlugins();

      CTranslatorRegistry::CreateCallbacks();
      CTranslatorRegistry::RegisterDependTranslator("ArnoldRenderOptions", ARNOLD_NODEID_RENDER_OPTIONS, CRenderOptionsTranslator::creator);

      CTranslatorRegistry::RegisterDependTranslator("surfaceShader", MAYA_NODEID_SURFACE_SHADER, CSurfaceShaderTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("lambert", MAYA_NODEID_LAMBERT, CLambertTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("file", MAYA_NODEID_FILE, CFileTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("place2dTexture", MAYA_NODEID_PLACE2D_TEXTURE, CPlace2DTextureTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("bump2d", MAYA_NODEID_BUMP2D, CBump2DTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("bump3d", MAYA_NODEID_BUMP3D, CBump3DTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("samplerInfo", MAYA_NODEID_SAMPLER_INFO, CSamplerInfoTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("plusMinusAverage", MAYA_NODEID_PLUS_MINUS_AVERAGE, CPlusMinusAverageTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("remapValue", MAYA_NODEID_REMAP_VALUE, CRemapValueTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("remapColor", MAYA_NODEID_REMAP_COLOR, CRemapColorTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("projection", MAYA_NODEID_PROJECTION, CProjectionTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("ramp", MAYA_NODEID_RAMP, CRampTranslator::creator);
      CTranslatorRegistry::RegisterDependTranslator("layeredTexture", MAYA_NODEID_LAYERED_TEXTURE, CLayeredTextureTranslator::creator);

      // sky is technically a DAG node, but it behaves like a DG node (i.e. it is only exported when a connection is processed)
      // therefore, it is not registered as a DagTranslator
      CTranslatorRegistry::RegisterDependTranslator("ArnoldSkyShader", CArnoldSkyShaderNode::id.id(), CSkyShaderTranslator::creator);

      CTranslatorRegistry::RegisterDagTranslator("directionalLight", MAYA_NODEID_DIRECTIONAL_LIGHT, CDirectionalLightTranslator::creator, CLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("spotLight", MAYA_NODEID_SPOT_LIGHT, CSpotLightTranslator::creator, CSpotLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("areaLight", MAYA_NODEID_AREA_LIGHT, CAreaLightTranslator::creator, CAreaLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("pointLight", MAYA_NODEID_POINT_LIGHT, CPointLightTranslator::creator, CPointLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("ambientLight", MAYA_NODEID_AMBIENT_LIGHT, CAmbientLightTranslator::creator, CLightTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("ArnoldSkyDomeLightShader", CArnoldSkyDomeLightShaderNode::id.id(), CSkyDomeLightTranslator::creator);

      CTranslatorRegistry::RegisterDagTranslator("mesh", MAYA_NODEID_MESH, CMeshTranslator::creator, CMeshTranslator::NodeInitializer);
      CTranslatorRegistry::RegisterDagTranslator("nurbsSurface", MAYA_NODEID_NURBS_SURFACE, CNurbsSurfaceTranslator::creator, CNurbsSurfaceTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("camera", MAYA_NODEID_CAMERA, CCameraTranslator::creator, CCameraTranslator::NodeInitializer);

      CTranslatorRegistry::RegisterDagTranslator("hairSystem", MAYA_NODEID_HAIR, CHairTranslator::creator, CHairTranslator::NodeInitializer);

      arnoldPluginFactory->MapToMayaNode("ambient_occlusion", "ArnoldAmbientOcclusionShader", CArnoldAmbientOcclusionShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("standard", "ArnoldStandardShader", CArnoldStandardShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("utility", "ArnoldUtilityShader", CArnoldUtilityShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("wireframe", "ArnoldWireframeShader", CArnoldWireframeShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("hair", "ArnoldHairShader", CArnoldHairShaderNode::id.id());

      arnoldPluginFactory->MapToMayaNode("MeshInfo", "ArnoldMeshInfoShader", CArnoldMeshInfoShaderNode::id.id());

      arnoldPluginFactory->MapToMayaNode("barndoor", "ArnoldBarndoorShader", CArnoldBarndoorShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("gobo", "ArnoldGoboShader", CArnoldGoboShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("light_blocker", "ArnoldLightBlockerShader", CArnoldLightBlockerShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("light_decay", "ArnoldLightDecayShader", CArnoldLightDecayShaderNode::id.id());

      arnoldPluginFactory->MapToMayaNode("fog", "ArnoldFogShader", CArnoldFogShaderNode::id.id());
      arnoldPluginFactory->MapToMayaNode("volume_scattering", "ArnoldVolumeScatteringShader", CArnoldVolumeScatteringShaderNode::id.id());

      arnoldPluginFactory->MapToMayaNode("ray_switch", "ArnoldRaySwitchShader", CArnoldRaySwitchShaderNode::id.id());

      // Load extensions last so that they can override default translators
      arnoldPluginFactory->LoadExtensions();

      delete arnoldPluginFactory;

#if MAYA_API_VERSION < 201200
      MNodeClass::InitializeExistingNodes();
#endif
      return status;
   }

   MStatus UnregisterArnoldNodes(MObject object)
   {
      MStatus status;
      MFnPlugin plugin(object);

      // RENDER OPTIONS
      status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);
      // Remove creation callback
      MDGMessage::removeCallback( CArnoldRenderOptionsNode::sId );

      // AOV
      status = plugin.deregisterNode(CArnoldAOVNode::id);

      // Ray switch shader
      status = plugin.deregisterNode(CArnoldRaySwitchShaderNode::id);

      // Surface Shaders
      status = plugin.deregisterNode(CArnoldAmbientOcclusionShaderNode::id);
      status = plugin.deregisterNode(CArnoldStandardShaderNode::id);
      status = plugin.deregisterNode(CArnoldUtilityShaderNode::id);
      status = plugin.deregisterNode(CArnoldMeshInfoShaderNode::id);
      status = plugin.deregisterNode(CArnoldWireframeShaderNode::id);
      status = plugin.deregisterNode(CArnoldHairShaderNode::id);

      // Displacement Shaders
      status = plugin.deregisterNode(CArnoldDisplacementShaderNode::id);

      // Light Shaders
      status = plugin.deregisterNode(CArnoldBarndoorShaderNode::id);
      status = plugin.deregisterNode(CArnoldGoboShaderNode::id);
      status = plugin.deregisterNode(CArnoldLightBlockerShaderNode::id);
      status = plugin.deregisterNode(CArnoldLightDecayShaderNode::id);

      status = plugin.deregisterNode(CArnoldFogShaderNode::id);
      status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
      status = plugin.deregisterNode(CArnoldVolumeScatteringShaderNode::id);

      CArnoldNodeFactory* arnoldPluginFactory;
      arnoldPluginFactory = new CArnoldNodeFactory(object);
      arnoldPluginFactory->UnregisterAllNodes();
      arnoldPluginFactory->UnloadExtensions();
      delete arnoldPluginFactory;

      CTranslatorRegistry::RemoveCallbacks();

      return status;
   }
}


DLLEXPORT MStatus initializePlugin(MObject object)
{
   MStatus status;

	MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   AiBegin();

   // TODO: Add proper checking and handling of returned status
   status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator, CArnoldRenderCmd::newSyntax);
   status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator, CArnoldIprCmd::newSyntax);
   status = plugin.registerCommand("arnoldExportAss", CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax);
   status = plugin.registerFileTranslator(   CArnoldAssTranslator::fileType,
                                             CArnoldAssTranslator::fileIcon,
                                             CArnoldAssTranslator::creator,
                                             CArnoldAssTranslator::optionScript,
                                             CArnoldAssTranslator::optionDefault,
                                             false );

   RegisterArnoldNodes(object);

   MGlobal::executePythonCommand( MString("import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()") );

   AiEnd();

   return MS::kSuccess;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status;
   MFnPlugin plugin(object);

   MGlobal::executePythonCommand( MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"));

   UnregisterArnoldNodes(object);

   // TODO: Add proper checking and handling of returned status
   status = plugin.deregisterCommand("arnoldRender");
   status = plugin.deregisterCommand("arnoldIpr");
   status = plugin.deregisterCommand("arnoldExportAss");
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileType);

   return MS::kSuccess;
}
