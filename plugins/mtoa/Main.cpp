#include "viewport2/ArnoldStandardShaderOverride.h"
#include "viewport2/ArnoldSkinShaderOverride.h"
#include "viewport2/ArnoldStandardSurfaceShaderOverride.h"
#include "viewport2/ArnoldStandardHairShaderOverride.h"
#include "viewport2/ArnoldGenericShaderOverride.h"
#include "viewport2/ArnoldAiImageShaderOverride.h"
#include "viewport2/ArnoldUtilityShaderOverrides.h"
#include "viewport2/ViewportUtils.h"
#include "viewport2/ArnoldVolumeDrawOverride.h"
#include "viewport2/ArnoldAreaLightDrawOverride.h"
#include "viewport2/ArnoldLightPortalDrawOverride.h"
#include "viewport2/ArnoldPhotometricLightDrawOverride.h"
#include "viewport2/ArnoldMeshLightDrawOverride.h"
#include "viewport2/ArnoldSkyDomeLightGeometryOverride.h"
#include "viewport2/ArnoldLightBlockerGeometryOverride.h"
#include "viewport2/ArnoldVolumeGeometryOverride.h"
#include "viewport2/ArnoldStandInSubSceneOverride.h"
#include <maya/MSelectionMask.h>
#include <maya/MViewport2Renderer.h>
#include "viewport2/ArnoldViewOverride.h"
#include "viewport2/ArnoldViewRegionManip.h"
#include <maya/MDrawRegistry.h>

// Must be included to export MapiVersion properly which
// sets the API version to a valid version versus "unknown"
#include <maya/MApiVersion.h>

#include "utils/Version.h"
#include "utils/MayaUtils.h"
#include "platform/Platform.h"
#include "utils/Universe.h"

#include "commands/ArnoldAssTranslator.h"
#include "commands/ArnoldUsdTranslator.h"
#include "commands/ArnoldExportAssCmd.h"
#include "commands/ArnoldImportAssCmd.h"
#include "commands/ArnoldUpdateTxCmd.h"
#include "commands/ArnoldSceneCmd.h"
#include "commands/ArnoldLicenseCmd.h"
#include "commands/ArnoldRenderCmd.h"
#include "commands/ArnoldIprCmd.h"
#include "commands/ArnoldBakeGeoCmd.h"
#include "commands/ArnoldRenderToTextureCmd.h"
#include "commands/ArnoldExportToMaterialXCmd.h"
#include "commands/ArnoldExportOperatorsCmd.h"
#include "commands/ArnoldPluginCmd.h"
#include "commands/ArnoldListAttributesCmd.h"
#include "commands/ArnoldTemperatureCmd.h"
#include "commands/ArnoldFlushCmd.h"
#include "commands/ArnoldRenderViewCmd.h"
#include "commands/ArnoldViewportRendererOptionsCmd.h"
#include "commands/ArnoldViewportRegionTool.h"
#ifdef ENABLE_ALEMBIC
#include "commands/AbcExport/ArnoldAbcExportCmd.h"
#endif
#include "nodes/TxTextureFile.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldAOVNode.h"
#include "nodes/ArnoldDriverNode.h"
#include "nodes/ArnoldFilterNode.h"
#include "nodes/ArnoldLookSwitchNode.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/SphereLocator.h"
#include "nodes/options/ArnoldOptionsNode.h"
#include "nodes/shader/ArnoldSkyNode.h"
#include "nodes/shader/ArnoldUserDataVec2Node.h"
#include "nodes/shader/ArnoldUserDataVectorNode.h"
#include "nodes/shader/ArnoldUserDataBoolNode.h"
#include "nodes/shader/ArnoldAxfShaderNode.h"
#include "nodes/shader/ArnoldMaterialXShaderNode.h"
#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/shape/ArnoldCurveCollector.h"
#include "nodes/shape/ArnoldVolume.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"
#include "nodes/light/ArnoldAreaLightNode.h"
#include "nodes/light/ArnoldLightBlockerNode.h"
#include "nodes/light/ArnoldPhotometricLightNode.h"
#include "nodes/light/ArnoldMeshLightNode.h"
#include "nodes/light/ArnoldLightPortal.h"

#include "translators/options/OptionsTranslator.h"
#include "translators/camera/CameraTranslators.h"
#include "translators/camera/ImagePlaneTranslator.h"
#include "translators/light/LightTranslators.h"
#include "translators/light/LightLinkerTranslator.h"
#include "translators/light/LightBlockerTranslator.h"
#include "translators/light/LightPortalTranslator.h"
#include "translators/shader/ShaderTranslators.h"
#include "translators/shape/MeshTranslator.h"
#include "translators/shape/NurbsSurfaceTranslator.h"
#include "translators/shape/HairTranslator.h"
#include "translators/shape/CurveTranslator.h"
#include "translators/shape/CurveCollectorTranslator.h"
#include "translators/shape/StandinsTranslator.h"
#include "translators/shape/MeshProceduralTranslator.h"
#include "translators/shape/ParticleTranslator.h"
#include "translators/shape/NParticleTranslator.h"
#include "translators/shape/InstancerTranslator.h"
#include "translators/shape/FluidTranslator.h"
#include "translators/shape/VolumeTranslator.h"
#include "translators/shader/ShadingEngineTranslator.h"
#include "translators/shader/FluidTexture2DTranslator.h"
#include "translators/operator/LookSwitchTranslator.h"
#include "translators/ObjectSetTranslator.h"
#include "translators/shader/AxfTranslator.h"
#include "translators/shader/MaterialXShaderTranslator.h"

#include "render/MaterialView.h"
#include "render/RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"

#include <ai_msg.h>
#include <ai_render.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSwatchRenderRegister.h>
#include <maya/MTemplateCommand.h>
#include <maya/MSceneMessage.h>

#include <ai.h>

static MString s_arnold_plugin_path_orig = MString("");
static MString s_mtoa_extensions_path_orig = MString("");
namespace // <anonymous>
{
   MCallbackId connectionCallback;
   MCallbackId sceneOpenCallback;

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
      {"arnoldImportAss", CArnoldImportAssCmd::creator, CArnoldImportAssCmd::newSyntax},
      {"arnoldPlugins", CArnoldPluginCmd::creator, CArnoldPluginCmd::newSyntax},
      {"arnoldListAttributes", CArnoldListAttributesCmd::creator, 0},
      {"arnoldTemperatureToColor", CArnoldTemperatureCmd::creator, 0},
      {"arnoldFlushCache", CArnoldFlushCmd::creator, CArnoldFlushCmd::newSyntax},
      {"arnoldRenderView", CArnoldRenderViewCmd::creator, CArnoldRenderViewCmd::newSyntax},
      {"arnoldUpdateTx", CArnoldUpdateTxCmd::creator, CArnoldUpdateTxCmd::newSyntax},
      {"arnoldScene", CArnoldSceneCmd::creator, CArnoldSceneCmd::newSyntax},
      {"arnoldLicense", CArnoldLicenseCmd::creator, CArnoldLicenseCmd::newSyntax},
      {"arnoldViewport", CArnoldViewportRendererOptionsCmd::creator, CArnoldViewportRendererOptionsCmd::newSyntax},
      {"arnoldExportToMaterialX", CArnoldExportToMaterialXCmd::creator, CArnoldExportToMaterialXCmd::newSyntax},
      {"arnoldExportOperators", CArnoldExportOperatorsCmd::creator, CArnoldExportOperatorsCmd::newSyntax}
#ifdef ENABLE_ALEMBIC
      ,{"arnoldExportAlembic", CArnoldExportAbcCmd::creator, CArnoldExportAbcCmd::createSyntax}
#endif
   };

   // Note that we use drawdb/geometry/light to classify it as UI for light.
   // This will allow it to be automatically filtered out by viewport display filters, and 
   // allow it to not participate in shadow map bounding box computations.
   const MString AI_AREA_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/areaLight";
   const MString AI_LIGHT_PORTAL_CLASSIFICATION = "drawdb/geometry/light/arnold/lightPortal";
   const MString AI_AREA_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_AREA_LIGHT_CLASSIFICATION + ":drawdb/light/areaLight:lightShader/aiRectangleAreaLight";
   const MString AI_SKYDOME_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/skydome";
   const MString AI_SKYDOME_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_SKYDOME_LIGHT_CLASSIFICATION + ":drawdb/light/image";
   const MString AI_SKYNODE_CLASSIFICATION = "drawdb/geometry/light/arnold/skynode";
   const MString AI_STANDIN_CLASSIFICATION = "drawdb/subscene/arnold/standin";
   const MString AI_PROCEDURAL_CLASSIFICATION = "drawdb/geometry/arnold/procedural"; // should we also be using "subscene" for versions >= 2017 as the standin do ?
   const MString AI_VOLUME_CLASSIFICATION = "drawdb/geometry/arnold/volume";
   const MString AI_PHOTOMETRIC_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/photometricLight";
   const MString AI_LIGHT_FILTER_CLASSIFICATION = "drawdb/geometry/arnold/lightFilter";
   const MString AI_PHOTOMETRIC_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_PHOTOMETRIC_LIGHT_CLASSIFICATION + ":drawdb/light/spotLight";
#if MAYA_API_VERSION >= 201800
   const MString AI_SKYNODE_WITH_ENVIRONMENT_WITH_SWATCH = ENVIRONMENT_WITH_SWATCH + ":" + AI_SKYNODE_CLASSIFICATION + ":drawdb/light/image/environment"; 
#else
   const MString AI_SKYNODE_WITH_ENVIRONMENT_WITH_SWATCH = ENVIRONMENT_WITH_SWATCH + ":" + AI_SKYNODE_CLASSIFICATION; 
#endif
   const MString AI_MESH_LIGHT_CLASSIFICATION = "drawdb/geometry/light/arnold/meshLight";
   const MString AI_MESH_LIGHT_WITH_SWATCH = LIGHT_WITH_SWATCH + ":" + AI_MESH_LIGHT_CLASSIFICATION + ":drawdb/light/pointLight";
   const MString AI_LIGHT_FILTER_WITH_SWATCH = LIGHT_FILTER_WITH_SWATCH + ":" + AI_LIGHT_FILTER_CLASSIFICATION;
   const MString AI_USER_DATA_NODE_CLASSIFICATION = ":rendernode/arnold/utility/user data";
   const MString AI_ANOLD_AXF_SHADER = "shader/surface:rendernode/arnold/shader/surface:swatch/ArnoldRenderSwatch:drawdb/shader/surface/arnold/axf_shader";
   const MString AI_ANOLD_MTLX_SHADER = "shader/surface:rendernode/arnold/shader/surface:swatch/ArnoldRenderSwatch:drawdb/shader/surface/arnold/mtlx_shader";

   

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
         "aiCurveCollector", CArnoldCurveCollector::id,
         CArnoldCurveCollector::creator, CArnoldCurveCollector::initialize,
         MPxNode::kLocatorNode, 0
      } , {
         "aiAreaLight", CArnoldAreaLightNode::id,
         CArnoldAreaLightNode::creator, CArnoldAreaLightNode::initialize,
         MPxNode::kLocatorNode, &AI_AREA_LIGHT_WITH_SWATCH
      } , {
         "aiLightPortal", CArnoldLightPortalNode::id,
         CArnoldLightPortalNode::creator, CArnoldLightPortalNode::initialize,
         MPxNode::kLocatorNode, &AI_LIGHT_PORTAL_CLASSIFICATION
      } , {
         "aiPhotometricLight", CArnoldPhotometricLightNode::id,
         CArnoldPhotometricLightNode::creator, CArnoldPhotometricLightNode::initialize,
         MPxNode::kLocatorNode, &AI_PHOTOMETRIC_LIGHT_WITH_SWATCH
      } , {
         "aiMeshLight", CArnoldMeshLightNode::id,
         CArnoldMeshLightNode::creator, CArnoldMeshLightNode::initialize,
         MPxNode::kLocatorNode, &AI_MESH_LIGHT_WITH_SWATCH
      } , {
         "aiLightBlocker", CArnoldLightBlockerNode::id,
         CArnoldLightBlockerNode::creator, CArnoldLightBlockerNode::initialize,
         MPxNode::kLocatorNode, &AI_LIGHT_FILTER_WITH_SWATCH
      } , {
         "aiUserDataVec2", CArnoldUserDataVec2Node::id,
         CArnoldUserDataVec2Node::creator, CArnoldUserDataVec2Node::initialize,
         MPxNode::kDependNode, &AI_USER_DATA_NODE_CLASSIFICATION
      } , {
         "aiUserDataVector", CArnoldUserDataVectorNode::id,
         CArnoldUserDataVectorNode::creator, CArnoldUserDataVectorNode::initialize,
         MPxNode::kDependNode, &AI_USER_DATA_NODE_CLASSIFICATION
      } , {
         "aiUserDataBool", CArnoldUserDataBoolNode::id,
         CArnoldUserDataBoolNode::creator, CArnoldUserDataBoolNode::initialize,
         MPxNode::kDependNode, &AI_USER_DATA_NODE_CLASSIFICATION
      }, {
         "aiAxfShader", CArnoldAxfShaderNode::id,
         CArnoldAxfShaderNode::creator, CArnoldAxfShaderNode::initialize,
         MPxNode::kDependNode, &AI_ANOLD_AXF_SHADER
      } , {
         "aiLookSwitch", CArnoldLookSwitchNode::id,
         CArnoldLookSwitchNode::creator, CArnoldLookSwitchNode::initialize,
         MPxNode::kDependNode, 0
      }, {
         "aiMaterialXShader", CArnoldMaterialXShaderNode::id,
         CArnoldMaterialXShaderNode::creator, CArnoldMaterialXShaderNode::initialize,
         MPxNode::kDependNode, &AI_ANOLD_AXF_SHADER
      }

   };

   struct shadingNodeOverride{
      MString classification;
      MString registrant;
      MHWRender::MPxSurfaceShadingNodeOverride* (*creator)(const MObject&);
   } shadingNodeOverrideList [] = {
      {
         "drawdb/shader/surface/arnold/standard",
         "arnoldStandardShaderOverride",
         ArnoldStandardShaderOverride::creator
       } ,
       {
           "drawdb/shader/surface/arnold/standard_surface",
           "arnoldStandardSurfaceShaderOverride",
           ArnoldStandardSurfaceShaderOverride::creator
       } ,
       {
        "drawdb/shader/surface/arnold/standard_hair",
        "arnoldStandardHairShaderOverride",
        ArnoldStandardHairShaderOverride::creator
        } ,
        {
         "drawdb/shader/surface/arnold/skin",
         "arnoldSkinShaderOverride",
         ArnoldSkinShaderOverride::creator
      } , {
         "drawdb/shader/surface/arnold/genericShader",
         "arnoldGenericShaderOverride",
         ArnoldGenericShaderOverride::creator
      } , {
          "drawdb/shader/surface/arnold/image",
          "arnoldAiImageShaderOverride",
          ArnoldAiImageShaderOverride::creator
       } ,{
           "drawdb/shader/utility/math/arnold/abs",
           "arnoldAiAbsShaderOverride",
           ArnoldUtilityShaderOverride::creator_Abs
       } ,{
           "drawdb/shader/utility/math/arnold/add",
           "arnoldAiAddShaderOverride",
           ArnoldUtilityShaderOverride::creator_Add
       } ,{
           "drawdb/shader/utility/math/arnold/atan",
           "arnoldAiAtanShaderOverride",
           ArnoldUtilityShaderOverride::creator_Atan
       } ,{
           "drawdb/shader/utility/math/arnold/max",
           "arnoldAiMaxShaderOverride",
           ArnoldUtilityShaderOverride::creator_Max
       } ,{
           "drawdb/shader/utility/math/arnold/min",
           "arnoldAiMinShaderOverride",
           ArnoldUtilityShaderOverride::creator_Min
       },{
           "drawdb/shader/utility/math/arnold/negate",
           "arnoldAiNegateShaderOverride",
           ArnoldUtilityShaderOverride::creator_Negate
       },{
           "drawdb/shader/utility/math/arnold/multiply",
           "arnoldAiMultiplyShaderOverride",
           ArnoldUtilityShaderOverride::creator_Multiply
       },{
           "drawdb/shader/utility/math/arnold/divide",
           "arnoldAiDivideShaderOverride",
           ArnoldUtilityShaderOverride::creator_Divide
       },{
           "drawdb/shader/utility/math/arnold/subtract",
           "arnoldAiSubtractShaderOverride",
           ArnoldUtilityShaderOverride::creator_Subtract
       },{
           "drawdb/shader/utility/math/arnold/pow",
           "arnoldAiPowShaderOverride",
           ArnoldUtilityShaderOverride::creator_Pow
       } ,{
           "drawdb/shader/utility/math/arnold/sqrt",
           "arnoldAiSqrtShaderOverride",
           ArnoldUtilityShaderOverride::creator_Sqrt
       },{
           "drawdb/shader/utility/math/arnold/dot",
           "arnoldAiDotShaderOverride",
           ArnoldUtilityShaderOverride::creator_Dot
       },{
           "drawdb/shader/utility/math/arnold/log",
           "arnoldAiLogShaderOverride",
           ArnoldUtilityShaderOverride::creator_Log
       },{
           "drawdb/shader/utility/math/arnold/exp",
           "arnoldAiExpShaderOverride",
           ArnoldUtilityShaderOverride::creator_Exp
       },{
           "drawdb/shader/utility/math/arnold/cross",
           "arnoldAiCrossShaderOverride",
           ArnoldUtilityShaderOverride::creator_Cross
       },{
           "drawdb/shader/utility/math/arnold/normalize",
           "arnoldAiNormalizeShaderOverride",
           ArnoldUtilityShaderOverride::creator_Normalize
       },{
           "drawdb/shader/utility/math/arnold/length",
           "arnoldAiLengthShaderOverride",
           ArnoldUtilityShaderOverride::creator_Length
       },{
           "drawdb/shader/utility/math/arnold/modulo",
           "arnoldAiModuloShaderOverride",
           ArnoldUtilityShaderOverride::creator_Modulo
       },{
           "drawdb/shader/utility/math/arnold/fraction",
           "arnoldAiFractionShaderOverride",
           ArnoldUtilityShaderOverride::creator_Fraction
       }, {
           "drawdb/shader/utility/math/arnold/clamp",
           "arnoldAiClampShaderOverride",
           ArnoldUtilityShaderOverride::creator_Clamp
       }, 
    //    {
    //        "drawdb/shader/surface/flat",
    //        "arnoldAiFlatShaderOverride",
    //        ArnoldUtilityShaderOverride::creator_Flat
    //    }, {
    //        "drawdb/shader/surface/mix",
    //        "arnoldAiMixShaderOverride",
    //        ArnoldUtilityShaderOverride::creator_Mix
    //    },
        {
           "drawdb/shader/utility/round_corners",
           "arnoldAiRoundCornersOverride",
           ArnoldUtilityShaderOverride::creator_Round_Corners
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
         "arnoldLightPortalNodeOverride",
         AI_LIGHT_PORTAL_CLASSIFICATION,
         CArnoldLightPortalDrawOverride::creator
      } , 
      {
         "arnoldPhotometricLightNodeOverride",
         AI_PHOTOMETRIC_LIGHT_CLASSIFICATION,
         CArnoldPhotometricLightDrawOverride::creator
      } ,
      {
         "arnoldMeshLightNodeOverride",
         AI_MESH_LIGHT_CLASSIFICATION,
         CArnoldMeshLightDrawOverride::creator
      }  
   };

   //
   // Template command that creates and deletes the manipulator
   //
   class aiViewRegionCmd;
   char cmdName[] = "aiViewRegionCmd";
   char nodeName[] = "aiViewRegion";
   class aiViewRegionCmd : public MTemplateCreateNodeCommand<aiViewRegionCmd, cmdName, nodeName>
   {
   public:
       aiViewRegionCmd() {}
   };
   static aiViewRegionCmd _aiViewRegionCmd;


   template < typename T, size_t N >
   size_t sizeOfArray(T const (&array)[ N ])
   {
      return N;
   }

   MStatus RegisterArnoldNodes(MObject object)
   {
      MStatus status;
      bool isBatch = IsBatch();

      MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

      // STANDINS
      status = plugin.registerShape(
                           "aiStandIn",
                           CArnoldStandInShape::id,
                           CArnoldStandInShape::creator,
                           CArnoldStandInShape::initialize,
                           nullptr,
                           &AI_STANDIN_CLASSIFICATION);
      CHECK_MSTATUS(status);
      
      // VOLUME
      status = plugin.registerShape(
                           "aiVolume",
                           CArnoldVolumeShape::id,
                           CArnoldVolumeShape::creator,
                           CArnoldVolumeShape::initialize,
                           nullptr,
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

      builtin->RegisterTranslator("aiStandardHair",
                                    "",
                                    CAiStandardHairTranslator::creator,
                                    CAiStandardHairTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiImage",
                                    "",
                                    CAiImageTranslator::creator,
                                    CAiImageTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiRaySwitch",
                                    "",
                                    CAiRaySwitchTranslator::creator,
                                    CAiRaySwitchTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiMixShader",
                                    "",
                                    CAiMixShaderTranslator::creator,
                                    CAiMixShaderTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiSwitch",
                                    "",
                                    CAiSwitchShaderTranslator::creator,
                                    CAiSwitchShaderTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiPassthrough",
                                    "",
                                    CAiPassthroughTranslator::creator,
                                    CAiPassthroughTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiWriteFloat",
                                    "",
                                    CAiAovWriteFloatTranslator::creator,
                                    CAiAovWriteFloatTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiWriteColor",
                                    "",
                                    CAiAovWriteColorTranslator::creator,
                                    CAiAovWriteColorTranslator::NodeInitializer);
      
      builtin->RegisterTranslator("aiOslShader",
                                    "",
                                    CAiOslShaderTranslator::creator,
                                    CAiOslShaderTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiRampRgb",
                                    "",
                                    CRampRgbTranslator::creator,
                                    CRampRgbTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiRampFloat",
                                    "",
                                    CRampFloatTranslator::creator,
                                    CRampFloatTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiAxfShader",
                                    "",
                                    CArnoldAxfShaderTranslator::creator,
                                    CArnoldAxfShaderTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiMaterialXShader",
                                    "",
                                    CArnoldMaterialXShaderTranslator::creator,
                                    CArnoldMaterialXShaderTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiToon",
                                       "",
                                       CToonTranslator::creator,
                                       CToonTranslator::NodeInitializer);         

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

      builtin->RegisterTranslator("aiLightPortal",
                                  "",
                                  CLightPortalTranslator::creator,
                                  CLightPortalTranslator::NodeInitializer);
                                   
      builtin->RegisterTranslator("aiPhotometricLight",
                                    "",
                                    CPhotometricLightTranslator::creator,
                                    CPhotometricLightTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiMeshLight",
                                    "",
                                    CMeshLightNewTranslator::creator,
                                    CMeshLightNewTranslator::NodeInitializer);

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
                                    CMeshProceduralTranslator::creator,
                                    CMeshProceduralTranslator::NodeInitializer);
      builtin->RegisterTranslator("nurbsSurface",
                                    "",
                                    CNurbsSurfaceTranslator::creator,
                                    CNurbsSurfaceTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiStandIn",
                                    "",
                                    CArnoldStandInsTranslator::creator,
                                    CArnoldStandInsTranslator::NodeInitializer);
      if (MayaNodeTypeExists("fluidShape"))
      {
         builtin->RegisterTranslator("fluidShape",
                                       "",
                                       CFluidTranslator::creator,
                                       CFluidTranslator::NodeInitializer);
      }
   
      builtin->RegisterTranslator("aiVolume",
                                    "",
                                    CArnoldVolumeTranslator::creator,
                                    CArnoldVolumeTranslator::NodeInitializer);
      builtin->RegisterTranslator("aiRoundCorners",
                                    "",
                                    CAiRoundCornersTranslator::creator,
                                    CAiRoundCornersTranslator::NodeInitializer);

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
      // we don't call the NodeInitializer function for UFE cameras, 
      // because we don't want to add "ai" attributes
      builtin->RegisterTranslator("ufeProxyCameraShape",
                                    "",
                                    CStandardCameraTranslator::creator);

      builtin->RegisterTranslator("imagePlane",
                                    "",
                                    CImagePlaneTranslator::creator,
                                    CImagePlaneTranslator::NodeInitializer);                          
       // Hair
      if (MayaNodeTypeExists("pfxHair"))
      {
         builtin->RegisterTranslator("pfxHair",
                                       "",
                                       CHairTranslator::creator,
                                       CHairTranslator::NodeInitializer);
      }
      // Curves
      builtin->RegisterTranslator("nurbsCurve",
                                    "",
                                    CCurveTranslator::creator,
                                    CCurveTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiCurveCollector",
                                    "",
                                    CCurveCollectorTranslator::creator,
                                    CCurveCollectorTranslator::NodeInitializer);
      // Particles
      if (MayaNodeTypeExists("particle"))
      {
         builtin->RegisterTranslator("particle",
                                       "",
                                       CParticleTranslator::creator,
                                       CParticleTranslator::NodeInitializer);
      }

      if (MayaNodeTypeExists("nParticle"))
      {
         builtin->RegisterTranslator("nParticle",
                                       "",
                                       CNParticleTranslator::creator,
                                       CNParticleTranslator::NodeInitializer);
      }

      builtin->RegisterTranslator("instancer",
                                    "",
                                    CInstancerTranslator::creator,
                                    CInstancerTranslator::NodeInitializer);

      builtin->RegisterTranslator("objectSet",
                                    "",
                                    CObjectSetTranslator::creator,
                                    CObjectSetTranslator::NodeInitializer);

      builtin->RegisterTranslator("aiLookSwitch",
                                    "",
                                    CLookSwitchTranslator::creator,
                                    CLookSwitchTranslator::NodeInitializer);

      
      // Load all plugins path or only shaders?
      CExtension* shaders;
      MString pluginPath = plugin.loadPath();
      unsigned int pluginPathLength = pluginPath.length();
      if (pluginPath.substring(pluginPathLength - 8, pluginPathLength-1) == MString("plug-ins"))
      {
         pluginPath = pluginPath.substring(0, pluginPathLength - 9);
         SetEnv("MTOA_PATH", pluginPath);
         if (!isBatch)
            SetFragmentSearchPath(pluginPath + MString("vp2"));
         MString modulePluginPath = pluginPath + MString("shaders");
         MString proceduralsPath = pluginPath + MString("procedurals");
         MString moduleExtensionPath = pluginPath + MString("extensions");
         const char* envVar = getenv("ARNOLD_PLUGIN_PATH");
         MString envVarStr = (envVar) ? MString(envVar) : MString("");
         MString arnoldPluginPath;

         if (envVarStr.length() > 0 && envVarStr != modulePluginPath && envVarStr != proceduralsPath)
         {
            // store current variable to restore at unload
            s_arnold_plugin_path_orig = envVarStr;
            arnoldPluginPath = envVarStr + MString(PATH_SEPARATOR) + modulePluginPath;
         }
         else
            arnoldPluginPath =  modulePluginPath;
         
         bool appendProceduralsPath = true;

#if MAYA_API_VERSION >= 20210000
         int ltAbout = 0;
#ifdef MOD_SUFFIX
         MString aboutCmd = MString("about -"+MString(MOD_SUFFIX)+"Version");
#else
         MString aboutCmd = MString("about -ltVersion");
#endif
         if (MGlobal::executeCommand(aboutCmd, ltAbout) == MStatus::kSuccess && ltAbout > 0)
            appendProceduralsPath = false;
#endif

         if (appendProceduralsPath)
            arnoldPluginPath += MString(PATH_SEPARATOR) + proceduralsPath;

         SetEnv("ARNOLD_PLUGIN_PATH", arnoldPluginPath);

         envVar = getenv("MTOA_EXTENSIONS_PATH");
         envVarStr = (envVar) ? MString(envVar) : MString("");
         if (envVarStr.length() > 0 && envVarStr != moduleExtensionPath)
         {
            // store current variable to restore at unload
            s_mtoa_extensions_path_orig = envVarStr;
            SetEnv("MTOA_EXTENSIONS_PATH", envVarStr + MString(PATH_SEPARATOR) + moduleExtensionPath);
         }
         else
            SetEnv("MTOA_EXTENSIONS_PATH", moduleExtensionPath);
      }

      status = CExtensionsManager::LoadExtensions(EXTENSION_SEARCH);
      //CHECK_MSTATUS(status); no longer return error if an extension didn't load properly

      // Will load all found plugins and try to register nodes and translators
      status = CExtensionsManager::LoadArnoldPlugins(PLUGIN_SEARCH);
      //CHECK_MSTATUS(status);

      // I need to retrieve the mtoa_shaders extension, so that I can register its translators
      shaders = CExtensionsManager::GetExtensionByName("mtoa_shaders"); 
      if (shaders)
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
                                       CProjectionTranslator::NodeInitializer);
         shaders->RegisterTranslator("ramp",
                                       "",
                                       CRampTranslator::creator,
                                       CRampTranslator::NodeInitializer);
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
         shaders->RegisterTranslator("phongE",
                                       "",
                                       CMayaPhongETranslator::creator);
         shaders->RegisterTranslator("anisotropic",
                                       "",
                                       CMayaAnisotropicTranslator::creator);
         shaders->RegisterTranslator("rampShader",
                                       "",
                                      CMayaRampShaderTranslator::creator);
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
         shaders->RegisterTranslator("blendColors",
                                       "",
                                       CMayaBlendColorsTranslator::creator);
         shaders->RegisterTranslator("multiplyDivide",
                                       "",
                                       CMultiplyDivideTranslator::creator);
         shaders->RegisterTranslator("checker",
                                       "",
                                       CCheckerTranslator::creator);
         shaders->RegisterTranslator("clamp",
                                       "",
                                       CMayaClampTranslator::creator);

         shaders->RegisterTranslator("condition",
                                       "",
                                       CConditionTranslator::creator);
         shaders->RegisterTranslator("luminance",
                                       "",
                                       CLuminanceTranslator::creator);
         shaders->RegisterTranslator("reverse",
                                       "",
                                       CReverseTranslator::creator);
         shaders->RegisterTranslator("surfaceLuminance",
                                       "",
                                       CSurfaceLuminanceTranslator::creator);
         shaders->RegisterTranslator("gammaCorrect",
                                       "",
                                       CGammaCorrectTranslator::creator);
         shaders->RegisterTranslator("surfaceShader",
                                       "",
                                       CSurfaceShaderTranslator::creator);
         shaders->RegisterTranslator("rgbToHsv",
                                       "",
                                       CRgbToHsvTranslator::creator);
         shaders->RegisterTranslator("hsvToRgb",
                                       "",
                                       CHsvToRgbTranslator::creator);
         shaders->RegisterTranslator("aiUserDataVec2",
                                       "",
                                       CUserDataVec2Translator::creator);
         shaders->RegisterTranslator("aiUserDataVector",
                                       "",
                                       CUserDataVectorTranslator::creator);
         shaders->RegisterTranslator("aiUserDataBool",
                                       "",
                                       CUserDataBoolTranslator::creator);
         shaders->RegisterTranslator("contrast",
                                       "",
                                       CContrastTranslator::creator);
         shaders->RegisterTranslator("setRange",
                                       "",
                                       CSetRangeTranslator::creator);
#if MAYA_API_VERSION >= 20200000
         shaders->RegisterTranslator("standardSurface",
                                       "",
                                       CStandardSurfaceTranslator::creator,
                                       CStandardSurfaceTranslator::NodeInitializer);         
#endif
         if(MGlobal::apiVersion() >= 20180400)
            LoadShadeFragment("aiRectangleAreaLight");

      }

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

      // Reversing the order in which nodes are de-registered
      for (int i = sizeOfArray(mayaNodeList) -1 ; i >= 0; i--)
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

enum MtoAInitializedData
{
   MTOA_INIT_FILE_EXPORT = 0,
   MTOA_INIT_FILE_IMPORT = 1,
   MTOA_INIT_USD_EXPORT,
   MTOA_INIT_TX_FILE,
   MTOA_INIT_MATERIAL_VIEW,
   MTOA_INIT_REGISTER_SWATCH,
   MTOA_INIT_COMMANDS,
   MTOA_INIT_ARNOLD_NODES,
   MTOA_INIT_RENDERER,
   MTOA_INIT_COUNT
};
void MtoAInitFailed(MObject object, MFnPlugin &plugin, const std::vector<bool> &initData)
{
   if (initData.empty())
      return;

   if (initData[MTOA_INIT_RENDERER])
      MGlobal::executePythonCommand(MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"), true, false);

   if (initData[MTOA_INIT_ARNOLD_NODES])
      UnregisterArnoldNodes(object);

   // This will only unregister what was previously registered,
   // so we don't need to test initData
   CExtensionsManager::UnregisterExtensionAttributes(object);

   if (initData[MTOA_INIT_COMMANDS])
      for (size_t i = 0; i < sizeOfArray(mayaCmdList); ++i)
         plugin.deregisterCommand(mayaCmdList[i].name);

   if (initData[MTOA_INIT_MATERIAL_VIEW])
      plugin.deregisterRenderer(CMaterialView::Name());
   
   if (initData[MTOA_INIT_REGISTER_SWATCH])
      MSwatchRenderRegister::unregisterSwatchRender(ARNOLD_SWATCH);

   if (initData[MTOA_INIT_TX_FILE])
      plugin.deregisterImageFile(CTxTextureFile::fileName);

   if (initData[MTOA_INIT_FILE_IMPORT])
   {
      plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeImport);
   }

   if (initData[MTOA_INIT_FILE_EXPORT])
   {
      plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeExport);
   }
   if (initData[MTOA_INIT_USD_EXPORT])
   {
      plugin.deregisterFileTranslator(CArnoldUsdTranslator::fileTypeExport);
   }

   if (connectionCallback)
   {
      MMessage::removeCallback(connectionCallback);
      connectionCallback = 0;
   }

   if (sceneOpenCallback)
   {
      MMessage::removeCallback(sceneOpenCallback);
      sceneOpenCallback = 0;
   }

   if (AiArnoldIsActive())
      ArnoldEnd();

}

void CreateDefaultArnoldRenderOptions(void *data)
{
   MObject options;
   MSelectionList list;
   list.add("defaultArnoldRenderOptions");
   if (list.length() == 0)
   {
       // defaultArnoldRenderOptions doesn't exist, we need to initialize it
      MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()");
   }
}

DLLEXPORT MStatus initializePlugin(MObject object)
{
   // This will dump memory leak info in debugger output window on program exit.
#ifdef _DEBUG
   _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

   MStatus status, returnStatus;
   returnStatus = MStatus::kSuccess;
   connectionCallback = 0;
   sceneOpenCallback = 0;

   bool isBatch = IsBatch();

   MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

   // Load metadata for builtin (mtoa.mtd)
   // FIXME to be moved to bin/arnold.mtd so that it's loaded automatically by arnold
   MString loadpath = plugin.loadPath();
   MString metafile = loadpath + "/" + "mtoa.mtd";
   SetMetafile(metafile);

   ArnoldBegin(GetStartupLogLevel());

   std::vector<bool> initializedData(MTOA_INIT_COUNT, false);

   auto VerifyInititalization = [](MStatus status, MtoAInitializedData step, MObject &object, 
                                 MFnPlugin &plugin, std::string stepName, std::vector<bool> &initializedData) 
   {
      CHECK_MSTATUS(status);
      if (MStatus::kSuccess == status)
      {
         std::string msg = "Successfully loaded ";
         msg += stepName;
         AiMsgDebug(msg.c_str());
         if (step < initializedData.size())
            initializedData[step] = true;
      }
      else
      {
         std::string msg = "Failed to load ";
         msg += stepName;
         AiMsgError(msg.c_str());
         MGlobal::displayError(msg.c_str());
         
         MtoAInitFailed(object, plugin, initializedData);
         return false;
      }
      return true;
      
   };

   // ASS file translator
   status = plugin.registerFileTranslator(CArnoldAssTranslator::fileTypeExport,
                                          CArnoldAssTranslator::fileIcon,
                                          CArnoldAssTranslator::exportCreator,
                                          CArnoldAssTranslator::optionScriptExport,
                                          CArnoldAssTranslator::optionDefault,
                                          false);
   if (!VerifyInititalization(status, MTOA_INIT_FILE_EXPORT, object, plugin, "Arnold ass file exporter", initializedData))
      return MStatus::kFailure;
   
   
   status = plugin.registerFileTranslator(CArnoldAssTranslator::fileTypeImport,
                                          CArnoldAssTranslator::fileIcon,
                                          CArnoldAssTranslator::importCreator,
                                          CArnoldAssTranslator::optionScriptImport,
                                          CArnoldAssTranslator::optionDefault,
                                          false);
   if (!VerifyInititalization(status, MTOA_INIT_FILE_IMPORT, object, plugin, "Arnold ass file importer", initializedData))
      return MStatus::kFailure;
   
   // Arnold-USD file translator
   status = plugin.registerFileTranslator(CArnoldUsdTranslator::fileTypeExport,
                                          CArnoldUsdTranslator::fileIcon,
                                          CArnoldUsdTranslator::exportCreator,
                                          CArnoldUsdTranslator::optionScriptExport,
                                          CArnoldUsdTranslator::optionDefault,
                                          false);
   if (!VerifyInititalization(status, MTOA_INIT_USD_EXPORT, object, plugin, "Arnold-USD file exporter", initializedData))
      return MStatus::kFailure;

   // Register image formats (should this be only for batch ??)
   MStringArray extensions;
   extensions.append("tex");
   extensions.append("tx");
   plugin.registerImageFile(CTxTextureFile::fileName,
                              CTxTextureFile::creator, 
                              extensions);

   if (!VerifyInititalization(status, MTOA_INIT_TX_FILE, object, plugin, "tx texture file", initializedData))
      return MStatus::kFailure;

   if (!isBatch)
   {
      // Material view renderer
      status = plugin.registerRenderer(CMaterialView::Name(), CMaterialView::Creator);
      if (!VerifyInititalization(status, MTOA_INIT_MATERIAL_VIEW, object, plugin, "Arnold material view renderer", initializedData))
         return MStatus::kFailure;
   
      // Swatch renderer
      status = MSwatchRenderRegister::registerSwatchRender(ARNOLD_SWATCH, CRenderSwatchGenerator::creator);
      if (!VerifyInititalization(status, MTOA_INIT_REGISTER_SWATCH, object, plugin, "Arnold swatch renderer", initializedData))
         return MStatus::kFailure;
   }

   // Commands
   for (size_t i = 0; i < sizeOfArray(mayaCmdList); ++i)
   {
      const mayaCmd& cmd = mayaCmdList[i];
      std::string cmdStr = cmd.name;
      cmdStr += " command";
      status = plugin.registerCommand(cmd.name, cmd.creator, cmd.syntax);
      if (!VerifyInititalization(status, MTOA_INIT_COUNT, object, plugin, cmdStr, initializedData))
      {
         // unregister the previous commands
         for (size_t j = 0; j < i; ++j)
            plugin.deregisterCommand(mayaCmdList[j].name);
         
         return MStatus::kFailure;
      }
   }
   initializedData[MTOA_INIT_COMMANDS] = true;

   // Arnold nodes: we allow for some nodes to fail
   status = RegisterArnoldNodes(object);
   if (MStatus::kSuccess == status)
   {
      AiMsgDebug("Successfully registered Arnold nodes");
      initializedData[MTOA_INIT_ARNOLD_NODES] = true;      
   }
   else
   {
      AiMsgError("Failed to register one or more Arnold nodes");
      MGlobal::displayError("Failed to register one or more Arnold nodes");
   }

   // Since executePythonCommand eats error output, trying to see if we can access every required module
   status = MGlobal::executePythonCommand(MString("import arnold"), true, false);
   if (!VerifyInititalization(status, MTOA_INIT_COUNT, object, plugin, "python module 'arnold'", initializedData))
         return MStatus::kFailure;

   status = MGlobal::executePythonCommand(MString("import mtoa"), true, false);
   if (!VerifyInititalization(status, MTOA_INIT_COUNT, object, plugin, "python module 'mtoa'", initializedData))
         return MStatus::kFailure;

   // Register the Arnold renderer
   status = MGlobal::executePythonCommand(MString("import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()"), true, false);
   if (!VerifyInititalization(status, MTOA_INIT_RENDERER, object, plugin, "renderer 'arnold'", initializedData))
         return MStatus::kFailure;
   if (!isBatch)
   {

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
    
      status = MHWRender::MDrawRegistry::registerSubSceneOverrideCreator(
          AI_STANDIN_CLASSIFICATION,
          "arnoldStandInNodeOverride",
          CArnoldStandInSubSceneOverride::Creator);
      CHECK_MSTATUS(status);

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

      status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
         AI_VOLUME_CLASSIFICATION,
         "arnoldVolumeNodeOverride",
        CArnoldVolumeGeometryOverride::Creator);
      CHECK_MSTATUS(status); 

      MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
      if (renderer)
      {
          // We register with a given name
          ArnoldViewOverride *overridePtr = new ArnoldViewOverride("arnoldViewOverride");
          if (overridePtr)
          {
              renderer->registerOverride(overridePtr);
          }
      }
      char nodeName[] = "aiViewRegion";
      status = plugin.registerNode(
          nodeName,
          ArnoldViewRegionManipulator::id,
          ArnoldViewRegionManipulator::creator,
          ArnoldViewRegionManipulator::initialize,
          MPxNode::kManipulatorNode);
      if (!status) {
          status.perror("registerNode");
      }

      status = _aiViewRegionCmd.registerCommand(object);
      if (!status) {
          status.perror("registerCommand");
      }

      status = plugin.registerContextCommand( "arnoldViewportRegionToolContext",
                                  ArnoldViewportRegionContextCmd::creator );

      // create the viewport toolbar
      status = MGlobal::executePythonCommand(MString("import mtoa.viewport;mtoa.viewport.add_controls()"), true, false);
   }
   connectionCallback = MDGMessage::addConnectionCallback(updateEnvironment);

   if (isBatch)
   {
      // if we are in batch mode we should check if a valid options node  exists after opening the secne file,
      // otherwise we get errors when running in batch see MTOA-848
      sceneOpenCallback = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, CreateDefaultArnoldRenderOptions);
   }

   return returnStatus;
}

DLLEXPORT MStatus uninitializePlugin(MObject object)
{
   MStatus status, returnStatus;
   returnStatus = MStatus::kSuccess;

   MFnPlugin plugin(object);
   bool isBatch = IsBatch();

   // TODO: Ensure we clear all sessions

   auto VerifyUninititalization = [](MStatus status, std::string stepName) 
   {
      CHECK_MSTATUS(status);
      if (MStatus::kSuccess == status)
      {
         std::string cmdStr = "Successfully unloaded ";
         cmdStr += stepName;
         AiMsgInfo(cmdStr.c_str());
         MGlobal::displayInfo(cmdStr.c_str());
         return true;
      }
      else
      {
         std::string cmdStr = "Failed to unload ";
         cmdStr += stepName;
         AiMsgError(cmdStr.c_str());
         MGlobal::displayError(cmdStr.c_str());
         return false;
      }
   };

   status = MGlobal::executePythonCommand(MString("import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"), true, false);
   if (!VerifyUninititalization(status, "renderer 'arnold'"))
      returnStatus = MStatus::kFailure;

   // Unregister all attribute extensions from Maya atributes
   status = CExtensionsManager::UnregisterExtensionAttributes(object);
   if (!VerifyUninititalization(status, "Arnold attribute extensions"))
      returnStatus = MStatus::kFailure;

   // Deregister in inverse order of registration
   // Commands
   for (size_t i = 0; i < sizeOfArray(mayaCmdList); ++i)
   {
      const mayaCmd& cmd = mayaCmdList[i];
      std::string cmdStr = cmd.name;
      cmdStr += " command";
      status = plugin.deregisterCommand(cmd.name);

      if (!VerifyUninititalization(status, cmdStr))
         returnStatus = MStatus::kFailure;
   }

   if (!isBatch)
   {
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

      status = MHWRender::MDrawRegistry::deregisterSubSceneOverrideCreator(
          AI_STANDIN_CLASSIFICATION,
          "arnoldStandInNodeOverride");
      CHECK_MSTATUS(status);

      status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
         AI_SKYDOME_LIGHT_CLASSIFICATION,
         "arnoldSkyDomeLightNodeOverride");
      CHECK_MSTATUS(status);

      status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
         AI_SKYNODE_CLASSIFICATION,
         "arnoldSkyNodeOverride");
      CHECK_MSTATUS(status);

      status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
         AI_LIGHT_FILTER_CLASSIFICATION,
         "arnoldLightBlockerNodeOverride");
      CHECK_MSTATUS(status);

      status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
         AI_VOLUME_CLASSIFICATION,
         "arnoldVolumeNodeOverride");
      CHECK_MSTATUS(status);

      // Register a custom selection mask
      MSelectionMask::deregisterSelectionType("arnoldLightSelection");

      MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
      if (renderer)
      {
          // Find override with the given name and deregister
          const MHWRender::MRenderOverride* overridePtr = renderer->findRenderOverride("arnoldViewOverride");
          if (overridePtr)
          {
              renderer->deregisterOverride(overridePtr);
              delete overridePtr;
          }
      }

      status = plugin.deregisterNode(ArnoldViewRegionManipulator::id);
      if (!status) {
         status.perror("deregisterNode");
      }

      status = _aiViewRegionCmd.deregisterCommand(object);
      if (!status) {
         status.perror("deregisterCommand");
      }

      status = plugin.deregisterContextCommand( "ArnoldViewportRegionToolContext" );
      if (!status) {
         status.perror("deregisterContextCommand");
      }
   }

   if (!isBatch)
   {
      // Material view renderer
      status = plugin.deregisterRenderer(CMaterialView::Name());
      if (!VerifyUninititalization(status, "Arnold material view renderer"))
         returnStatus = MStatus::kFailure;
   
      // Swatch renderer
      status = MSwatchRenderRegister::unregisterSwatchRender(ARNOLD_SWATCH);
      if (!VerifyUninititalization(status, "Arnold swatch renderer"))
         returnStatus = MStatus::kFailure;
   } 
   // Deregister image formats
   status = plugin.deregisterImageFile(CTxTextureFile::fileName);
   if (!VerifyUninititalization(status, "tx texture file"))
      returnStatus = MStatus::kFailure;

   // ASS file translator
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeExport);
   if (!VerifyUninititalization(status, "Arnold ass file exporter"))
      returnStatus = MStatus::kFailure;
   
   status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileTypeImport);
   if (!VerifyUninititalization(status, "Arnold ass file importer"))
      returnStatus = MStatus::kFailure;

   status = plugin.deregisterFileTranslator(CArnoldUsdTranslator::fileTypeExport);
   if (!VerifyUninititalization(status, "Arnold-USD file exporter"))
      returnStatus = MStatus::kFailure;

   // Restore the original env variables, so that they don't accumulate if we re-load MtoA
   SetEnv("MTOA_EXTENSIONS_PATH", s_mtoa_extensions_path_orig);
   SetEnv("ARNOLD_PLUGIN_PATH", s_arnold_plugin_path_orig);

   if (connectionCallback)
   {
      MMessage::removeCallback(connectionCallback);
      connectionCallback = 0;
   }

   if (sceneOpenCallback)
   {
      MMessage::removeCallback(sceneOpenCallback);
      sceneOpenCallback = 0;
   }
   
   // Removing all nodes as the last step
   status = UnregisterArnoldNodes(object);
   if (!VerifyUninititalization(status, "Arnold nodes"))
      returnStatus = MStatus::kFailure;

   ArnoldEnd();
   return returnStatus;
}
