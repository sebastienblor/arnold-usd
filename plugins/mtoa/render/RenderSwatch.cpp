#include "platform/Platform.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"
#include "render/RenderSession.h"
#include "RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "utils/time.h"

#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"

#include <maya/MImage.h>
#include <ai_msg.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <assert.h>
#include <string.h> // for memset.
#include <string>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MSwatchRenderBase* CRenderSwatchGenerator::creator(MObject dependNode,
                                                  MObject renderNode,
                                                  int imageResolution)
{
   return new CRenderSwatchGenerator(dependNode, renderNode, imageResolution);
}

CRenderSwatchGenerator::CRenderSwatchGenerator(MObject dependNode,
                                             MObject renderNode,
                                             int imageResolution)
                                                : MSwatchRenderBase(dependNode,
                                                                    renderNode,
                                                                    imageResolution)
{
   m_iteration = 0;
   ClearSwatch();
   SetSwatchClass(dependNode);

}

CRenderSwatchGenerator::~CRenderSwatchGenerator()
{
}

void CRenderSwatchGenerator::SetSwatchClass(const MObject & node)
{
   m_nodeClass = "";
   m_swatchClass = SWATCH_NONE;
   if (node.isNull())
   {
      ErrorSwatch("Invalid swatch node.");
      return;
   }
   // Find out what type of swatch we're rendering
   MFnDependencyNode depFn(node);
   MString classification = MFnDependencyNode::classification(depFn.typeName());

   // Classification string contains also the swatch render name, and the : separated parts
   // seem to be shuffled (according to alphabetic order?). So swatch render name is not
   // guaranteed to come in any specific position
   MStringArray classParts;
   MStringArray classes;
   classification.split(':', classParts);
   for (unsigned int i=0; i < classParts.length(); ++i)
   {
      classes.clear();
      classParts[i].split('/', classes);
      for (unsigned int j=0; j < classes.length(); ++j)
      {
         if (classes[j] == "swatch")
         {
            break;
         }
         else if (classes[j] == "surface"
                  || classes[j] == "texture"
                  || classes[j] == "utility"
                  || classes[j] == "volume")
         {
            m_swatchClass = SWATCH_SHADER;
         }
         else if (classes[j] == "environment")
         {
            m_swatchClass = SWATCH_ENVIRONMENT;
            break;
         }
         else if (classes[j] == "atmosphere")
         {
            m_swatchClass = SWATCH_ATMOSPHERE;
            break;
         }
         else if (classes[j] == "light")
         {
            m_swatchClass = SWATCH_LIGHT;
         }
         else if (classes[j] == "filter")
         {
            if (m_swatchClass == SWATCH_LIGHT)
            {
               m_swatchClass = SWATCH_LIGHTFILTER;
               break;
            }
         }
      }
      if (m_swatchClass != SWATCH_NONE) {
         m_nodeClass = classParts[i];
         break;
      }
   }

   if (m_swatchClass == SWATCH_NONE)
   {
      ErrorSwatch("No supported swatch for node \"" + depFn.name() +
                   "\" of classification \"" + classification + "\"");
   }
}

MStatus CRenderSwatchGenerator::BuildArnoldScene()
{
   MStatus status;

   MObject mayaNode = swatchNode();
   
   // What is this? Seem to always get the same node
   // MObject renderNode = node();

   MString mayaNodeName = "";
   MString mayaNodeType = MFnDependencyNode(mayaNode).typeName();
   if (mayaNode.hasFn(MFn::kDagNode))
      mayaNodeName = MFnDagNode(mayaNode).partialPathName();
   else
      mayaNodeName = MFnDependencyNode(mayaNode).name();

   // Load basic swatch scene (additions will be done depending on what is previewed)
   /*
   if (MStatus::kSuccess != LoadAssForNode()
         && MStatus::kSuccess != DefaultArnoldScene())
   {
      ErrorSwatch("Could not create Arnold swatch scene");
      return MStatus::kFailure;
   }
   */
   if (MStatus::kSuccess != DefaultArnoldScene())
   {
      ErrorSwatch("Could not create Arnold swatch scene");
      return MStatus::kFailure;
   }
   // Export the swatched shading node
   AtNode* arnoldNode = NULL;
   CNodeTranslator* translator = NULL;
   status = ExportNode(arnoldNode, translator);

   if (MStatus::kSuccess != status)
   {
      ErrorSwatch("Could not export \"" + mayaNodeName + "\" of type \"" + mayaNodeType + "\"");
   }
   else
   {
      MString arnoldNodeName(AiNodeGetName(arnoldNode));
      if (NULL != arnoldNode) {
         const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(arnoldNode);
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa.swatch] "+ mayaNodeName +" | Exported as "+ MString(AiNodeGetName(arnoldNode))+"("+MString(AiNodeEntryGetTypeName(nodeEntry))+")");
               
      }

      // Assign it in the scene, depending on what it is
      status = AssignNode(arnoldNode, translator);
      if (MStatus::kSuccess != status)
      {
         ErrorSwatch("Could not assign \"" + mayaNodeName + "\" exported as \"" + arnoldNodeName + "\"");
      }
      // Add a camera
      // TODO : would it be possible / interesting to allow preview of cameras?
      AtNode* camera = AiNode("persp_camera");
      AiNodeSetStr(camera, "name", "camera");
      AiNodeSetVec(camera, "position", 0.f, 0.f, 1.14f);
      // Apply any swatch options (overrides) that are present on mayaNode
      if (MStatus::kSuccess != ApplyOverrides(translator))
      {
         ErrorSwatch("Could not apply overrides present on \"" + mayaNodeName + "\"");
      }
   }

   if (translator != NULL) delete translator;
   return MStatus::kSuccess;

}

// This will try and find an ass file based on the classification
// of the shader. e.g. shader_surface.ass. It will try and find these
// ass files in a path pointed to by $MTOA_SWATCH_ASS_PATH.
MStatus CRenderSwatchGenerator::LoadAssForNode()
{
   const MString ass = MString("$MTOA_SWATCH_ASS_PATH/").expandFilePath();
   std::string c_ass(m_nodeClass.asChar());
   std::replace(c_ass.begin(), c_ass.end(), '/', '_');
   c_ass = ass.asChar() + c_ass + ".ass";

   if (AiASSLoad(c_ass.c_str()) == AI_SUCCESS)
   {
      if (NULL != AiNodeLookUpByName("geometry"))
         return MStatus::kSuccess;
      else
         ErrorSwatch("There should be at least one geometry named \"geometry\" in the swatch ass file.");
   }

   return MStatus::kFailure;
}

// Default scene when none was loaded
MStatus CRenderSwatchGenerator::DefaultArnoldScene()
{
   // FIXME : temp fix, add attributes on shading nodes swatch tab
   // to choose geometry / optional ground plane
   AtNode *geometry = NULL;
   MString typeName = MFnDependencyNode(swatchNode()).typeName();

   if (typeName == "aiHair")
   {
      geometry = AiNode("sphere");
      if (NULL != geometry)
      {
         AiNodeSetStr(geometry, "name", "geometry");
         AiNodeSetBool(geometry, "opaque", false);
         AtMatrix matrix = {{ { 1.0f, 0.0f, 0.0f, 0.0f },
                            { 0.0f, -1.0f, 0.0f, 0.0f },
                            { 0.0f, 0.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f, 1.0f } }};
         AiNodeSetMatrix(geometry, "matrix", matrix);
         return MStatus::kSuccess;
      }
   }
   else
   {
      geometry = PolySphere();
      if (NULL != geometry)
      {
         AiNodeSetStr(geometry, "name", "geometry");
         AiNodeSetByte(geometry, "subdiv_iterations", 1);
         AiNodeSetBool(geometry, "opaque", false);
         return MStatus::kSuccess;
      }
   }
   
   return MStatus::kFailure;
}

MStatus CRenderSwatchGenerator::ExportNode(AtNode* & arnoldNode,
                                           CNodeTranslator* & translator)
{
   MStatus status;
   MObject mayaNode = swatchNode();

   CArnoldSession* exportSession = CMayaScene::GetArnoldSession();

   if (mayaNode.hasFn(MFn::kDagNode))
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(mayaNode, dagPath);
      CDagTranslator* dagTranslator = CExtensionsManager::GetTranslator(dagPath);
      if (NULL != dagTranslator)
      {
         translator = (CNodeTranslator*) dagTranslator;
      }
      else
      {
         translator = CExtensionsManager::GetTranslator(mayaNode);
         dagTranslator = (CDagTranslator*) translator;
      }
      if (NULL != dagTranslator)
      {
         dagTranslator->m_impl->Init(exportSession, dagPath, "");
         arnoldNode = dagTranslator->m_impl->DoExport();
      }
   } 
   else {
      translator = CExtensionsManager::GetTranslator(mayaNode);
      if (NULL != translator)
      {
         translator->m_impl->Init(exportSession, mayaNode, "");
         arnoldNode = translator->m_impl->DoExport();
      }
   }
   if (NULL != arnoldNode)
   {
      status = MStatus::kSuccess;
   }
   else
   {
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CRenderSwatchGenerator::AssignNode(AtNode* arnoldNode, CNodeTranslator* translator)
{
   MStatus status;
   MFnDependencyNode depFn(swatchNode());

   // Assign what needs to be on geometry

   AtNode* geometry = AiNodeLookUpByName("geometry");

   // Assign exported geometry shader or use default one
   if (m_swatchClass == SWATCH_SHADER)
   {
      AiNodeSetPtr(geometry, "shader", arnoldNode);
   } else {
      AtNode* defaultShader = AiNode("standard");
      AiNodeSetStr(defaultShader, "name", "default");
      AiNodeSetFlt(defaultShader, "Ks", 0.35f);
      AiNodeSetBool(defaultShader, "reflection_exit_use_environment", true);
      AiNodeSetPtr(geometry, "shader", defaultShader);
   }

   // If we are swatching a light or light filter

   if (m_swatchClass == SWATCH_LIGHT) {
      // Build matrix for a light positioned at -1, 1, 1 looking at 0, 0, 0
      // NOTE: if it's considered too dark make it closer, do NOT overwrite
      // intensity or the actual swatched light intensity won't be used
      // which is what we want for swatching
      AtMatrix matrix;
      AtVector frame[4];
      frame[0] = AtVector(0.f, 0.f, 1.73f);
      frame[1] = AtVector(0.707107f, 0.408248f, -0.57735f);
      frame[2] = AtVector(0.0f, 0.816497f, 0.57735f);
      frame[3] = AtVector(0.707107f, -0.408248f, 0.57735f);
      matrix = AiM4Frame(frame[0], frame[1], frame[2], frame[3]);

      if (strcmp(AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)), "mesh_light") == 0)
      {
         // Special case for mesh lights.
         // The mesh can be of any size and shape and will not give a consistent 
         // result. So instead use a quad light stand-in with the same properties.
         AtNode *light = AiNode("quad_light");
         AiNodeSetStr(light, "name", "light");
         AiNodeSetMatrix(light, "matrix", matrix);
         AtRGB color = AiNodeGetRGB(arnoldNode, "color");
         AiNodeSetRGB(light,  "color", color.r, color.g, color.b);
         AiNodeSetFlt(light,  "intensity",       AiNodeGetFlt(arnoldNode, "intensity"));
         AiNodeSetFlt(light,  "exposure",        AiNodeGetFlt(arnoldNode, "exposure"));
         AiNodeSetInt(light,  "samples",         AiNodeGetInt(arnoldNode, "samples"));
         AiNodeSetBool(light, "normalize",       AiNodeGetBool(arnoldNode, "normalize"));
         AiNodeSetFlt(light,  "diffuse",         AiNodeGetFlt(arnoldNode, "diffuse"));
         AiNodeSetFlt(light,  "specular",        AiNodeGetFlt(arnoldNode, "specular"));

         // Hide original mesh light
         AtNode* meshNode = translator->GetArnoldNode("mesh");
         AiNodeSetDisabled(meshNode, true);
         AiNodeSetDisabled(arnoldNode, true);
      }
      else
      {
         AiNodeSetMatrix(arnoldNode, "matrix", matrix);
      }
   }
   else if (m_swatchClass == SWATCH_LIGHTFILTER || m_swatchClass == SWATCH_ATMOSPHERE)
   {
      // Use a spot light at -1, 1, 1 looking at 0, 0, 0
      // to preview light filters and atmosphere effects
      AtNode *light = AiNode("spot_light");
      AiNodeSetStr(light, "name", "light");
      AiNodeSetFlt(light, "intensity", 1.f);
      AiNodeSetVec(light, "position", -1.f, 1.f, 1.f);
      AiNodeSetVec(light, "look_at", 0.f, 0.f, 0.f);
      // AiNodeSetFlt(light, "cone_angle", 30.f);

      if (m_swatchClass == SWATCH_LIGHTFILTER)
      {
         AiNodeSetPtr(light, "filters", arnoldNode);
      }
      else if (m_swatchClass == SWATCH_ATMOSPHERE)
      {
         AiNodeSetBool(light, "cast_volumetric_shadows", true);
      }
   }
   else
   {
      // Default light for swatching shaders etc
      AtNode *light = AiNode("distant_light");
      AiNodeSetStr(light, "name", "light");
      AiNodeSetVec(light, "direction", 1.0f, -1.f, -1.0f);
   }

   // Set the global options for background and atmosphere
   
   AtNode * const options = AiUniverseGetOptions();
   
   MTime ct = MAnimControl::currentTime();
   AiNodeSetFlt(options, "frame", (float)ct.value());

   // If we are swatching an environment (background) shader
   if (m_swatchClass == SWATCH_ENVIRONMENT)
   {
      AiNodeSetPtr(options, "background", arnoldNode);
   }
   else
   {
      // Add a default sky shader to get solid alpha
      // TODO : options to use a custom environment for swatches or use render setting's?
      AtNode* background = AiNode("sky");
      AiNodeSetStr(background, "name", "background");
      AiNodeSetRGB (background, "color", 0.0f, 0.0f, 0.0f);
      AiNodeSetPtr(options, "background", background);
   }

   // If we are swatching an atmosphere shader
   if (m_swatchClass == SWATCH_ATMOSPHERE)
   {
      AiNodeSetPtr(options, "atmosphere", arnoldNode);
   }

   return MStatus::kSuccess;
}

MStatus CRenderSwatchGenerator::ApplyOverrides(CNodeTranslator* translator)
{
   // Temporary until it is exposed somewhere
   AtNode * const options = AiUniverseGetOptions();
   AiNodeSetBool(options, "skip_license_check", true);
   AiNodeSetBool(options, "texture_automip", false);

   // Commenting this as it was causing crashes (#2482)
   // when this value has changed since last render, but 
   // a texture is still in the texture cache
   //AiNodeSetInt(options, "texture_autotile", 0);

   // Read whatever "swatch" attribute we find on the node
   MStatus status;
   MObject mayaNode = swatchNode();
   MFnDependencyNode depFn(mayaNode);
   MPlug swatchPlug = depFn.findPlug("aiSwatch", true, &status);
   if (MStatus::kSuccess == status && swatchPlug.isCompound())
   {
      unsigned int nNodeOverrides = swatchPlug.numChildren();
      for (unsigned int c=0; c<nNodeOverrides; c++)
      {
         MPlug nodeOverPlug = swatchPlug.child(c);
         if (nodeOverPlug.isCompound())
         {
            MString nodeOverName = nodeOverPlug.partialName();
            AtNode* nodeOver = AiNodeLookUpByName(nodeOverName.asChar());
            const AtNodeEntry* nodeOverEntry = AiNodeGetNodeEntry(nodeOver);
            unsigned int nAttrOverrides = nodeOverPlug.numChildren();
            for (unsigned int a=0; a<nAttrOverrides; a++)
            {
               MPlug attrOverPlug = nodeOverPlug.child(a);
               MString attrOverName = attrOverPlug.partialName();
               const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(nodeOverEntry, attrOverName.asChar());
               if (NULL != paramEntry) translator->ProcessParameter(nodeOver, AiParamGetName(paramEntry), AiParamGetType(paramEntry), attrOverPlug);
            }
         }
         else
         {
            // Allow connection of a replacement node in that case ?
         }
      }
   }

   return MStatus::kSuccess;
}

// Output driver writes a float image, but we reset it as byte here
// as it is what Maya expects (it is converted if render succeeds)
void CRenderSwatchGenerator::ClearSwatch()
{
   const int res(resolution());
   const int num_pixels(res * res);
   image().create(res, res, 4, MImage::kByte);
   unsigned char * pixels = image().pixels();
   memset(pixels, 0, num_pixels * 4 * sizeof(char));
}

void CRenderSwatchGenerator::ErrorSwatch(const MString msg)
{
   AiMsgError("[mtoa.swatch] %s", msg.asChar());
   ClearSwatch();
}

// {
//    MStatus GetArnoldNode (MObject node, AtNode* arnoldNode)
//    {
//       CMayaScene::Begin(MTOA_SESSION_SWATCH);
//       CNodeTranslator* translator = NULL;
//       translator = CExtensionsManager::GetTranslator(mayaNode);
//       CArnoldSession* exportSession = CMayaScene::GetArnoldSession();
//       if (NULL != translator)
//       {
//          translator->m_impl->Init(exportSession, mayaNode, "");
//          arnoldNode = translator->m_impl->DoExport();
//       }
//       CMayaScene::End();
//       if ( arnoldNode == NULL )
//       {
//          return MStatus::kFailure;
//       }
//       return MStatus::kSuccess ;
//    }
// }

bool CRenderSwatchGenerator::DoSwatchRender()
{
   MStatus status ;
   status = CMayaScene::Begin(MTOA_SESSION_SWATCH);
   MObject ArnoldRenderOptionsNode = CMayaScene::GetSceneArnoldRenderOptionsNode();
   status = BuildArnoldScene();
   if (MStatus::kSuccess == status)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.swatch] " + MFnDependencyNode(swatchNode()).name() + " | Rendering");

      image().create(resolution(),
                     resolution(),
                  4,                               // RGBA
                  MImage::kFloat);                // Has to be for swatches it seems.

      // if use tx is enabled, call exportTx that will *not* try to convert the mipmaps
      // but will check for existing tx for sake of optimization
      if (ArnoldRenderOptionsNode.isNull() || MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("use_existing_tiled_textures", true).asBool())
         CMayaScene::GetArnoldSession()->ExportTxFiles();

      CMayaScene::GetRenderSession()->DoSwatchRender(image(), resolution());
      #ifndef NDEBUG
      // Catch this as it would lead to a Maya UI crash
      // with no proper stack info on what caused it
      unsigned int iWidth, iHeight;
      image().getSize(iWidth, iHeight);
      assert(resolution() == (int)iWidth);
      assert(resolution() == (int)iHeight);
      assert(MImage::kFloat == image().pixelType());
      #endif
      image().convertPixelFormat(MImage::kByte);
      // Stop being called/iterated.
      CMayaScene::End();
      return true;
   }
}

bool CRenderSwatchGenerator::DoNoGPUImage()
{
   MString iconsPath = MString(getenv("MTOA_PATH")) +MString ("icons//") ;
   MString noGpuPath = iconsPath + MString ("noGPU.png");
   MStatus stat =  image().readFromFile(noGpuPath);
   image().verticalFlip();
   image().resize(resolution(), resolution());
   return true;
}


bool CRenderSwatchGenerator::doIteration()
{
   MStatus status;
   MObject mayaNode = swatchNode();
   MObject ArnoldRenderOptionsNode = CMayaScene::GetSceneArnoldRenderOptionsNode();

   if (CMayaScene::GetSessionMode() == MTOA_SESSION_IPR)
   {
      return true;
   }

   CMayaScene::Begin(MTOA_SESSION_SWATCH);
   AtNode* arnoldNode = NULL;
   CNodeTranslator* translator = NULL;
   translator = CExtensionsManager::GetTranslator(mayaNode);
   CArnoldSession* exportSession = CMayaScene::GetArnoldSession();
   if (NULL != translator)
   {
      translator->m_impl->Init(exportSession, mayaNode, "");
      arnoldNode = translator->m_impl->DoExport();
   }

   bool gpuRenderCompatibility = true;
   bool doSwatch = true;
   bool isGPU = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("renderDevice", true).asBool();
   bool sceneSwatch = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("enable_swatch_render", true).asBool();
   
   AiMetaDataGetBool(AiNodeGetNodeEntry(arnoldNode), NULL, "gpu_support", &gpuRenderCompatibility);
   AiMetaDataGetBool(AiNodeGetNodeEntry(arnoldNode), NULL, "maya.swatch", &doSwatch);
   CMayaScene::End();

      // Start, must build the swatch scene
      // Arnold can only render one thing at a time. It may be an option to block/wait here,
      // but only if it's another swatch render taking place. We don't want to end the
      // current session there

      if (CMayaScene::IsActive())
      {
         // Other export or rendering process in progress.
         // Wait if it's another swatch, abort else.
         if (CMayaScene::GetSessionMode() == MTOA_SESSION_SWATCH)
         {
            return false;
         }
         else
         {
            return true;
         }
      }

      if (sceneSwatch && doSwatch )
      {
         if (isGPU )
         {
            if ( gpuRenderCompatibility)
               {
                  return DoSwatchRender();
               }
            else 
            {
               return DoNoGPUImage();
            }
         }
         else
         {
            return DoSwatchRender();
         }
      }
      else
      {
         if ( isGPU && !gpuRenderCompatibility )
         {
            return DoNoGPUImage();
         }
         else
         {
            return DoStaticImage();
         }
      }
   // we'll never get here
   return true;
}

// This will create a polygon sphere for swatching
AtNode* CRenderSwatchGenerator::PolySphere()
{
   AtNode *sph = AiNode("polymesh");

   AtArray *nsides = AiArray( 64, 1, AI_TYPE_UINT,
                              4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                              4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                              3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
                              );

   AtArray *vidxs = AiArray( 240, 1, AI_TYPE_UINT,
                             0, 1, 9, 8, 1, 2, 10, 9, 2, 3, 11, 10, 3, 4, 12, 11, 4, 5, 13, 12, 5, 6, 14, 13, 6, 7, 15, 14, 7, 0, 8, 15, 8, 9,
                             17, 16, 9, 10, 18, 17, 10, 11, 19, 18, 11, 12, 20, 19, 12, 13, 21, 20, 13, 14, 22, 21, 14, 15, 23, 22, 15,
                             8, 16, 23, 16, 17, 25, 24, 17, 18, 26, 25, 18, 19, 27, 26, 19, 20, 28, 27, 20, 21, 29, 28, 21, 22, 30, 29,
                             22, 23, 31, 30, 23, 16, 24, 31, 24, 25, 33, 32, 25, 26, 34, 33, 26, 27, 35, 34, 27, 28, 36, 35, 28, 29, 37,
                             36, 29, 30, 38, 37, 30, 31, 39, 38, 31, 24, 32, 39, 32, 33, 41, 40, 33, 34, 42, 41, 34, 35, 43, 42, 35, 36,
                             44, 43, 36, 37, 45, 44, 37, 38, 46, 45, 38, 39, 47, 46, 39, 32, 40, 47, 40, 41, 49, 48, 41, 42, 50, 49, 42,
                             43, 51, 50, 43, 44, 52, 51, 44, 45, 53, 52, 45, 46, 54, 53, 46, 47, 55, 54, 47, 40, 48, 55, 1, 0, 56, 2, 1,
                             56, 3, 2, 56, 4, 3, 56, 5, 4, 56, 6, 5, 56, 7, 6, 56, 0, 7, 56, 48, 49, 57, 49, 50, 57, 50, 51, 57, 51, 52, 57,
                             52, 53, 57, 53, 54, 57, 54, 55, 57, 55, 48, 57
                             );

   AtArray *uvidxs = AiArray( 240, 1, AI_TYPE_UINT,
                              0, 1, 10, 9, 1, 2, 11, 10, 2, 3, 12, 11, 3, 4, 13, 12, 4, 5, 14, 13, 5, 6, 15, 14, 6, 7, 16, 15, 7, 8, 17, 16, 9,
                              10, 19, 18, 10, 11, 20, 19, 11, 12, 21, 20, 12, 13, 22, 21, 13, 14, 23, 22, 14, 15, 24, 23, 15, 16, 25, 24,
                              16, 17, 26, 25, 18, 19, 28, 27, 19, 20, 29, 28, 20, 21, 30, 29, 21, 22, 31, 30, 22, 23, 32, 31, 23, 24, 33,
                              32, 24, 25, 34, 33, 25, 26, 35, 34, 27, 28, 37, 36, 28, 29, 38, 37, 29, 30, 39, 38, 30, 31, 40, 39, 31, 32,
                              41, 40, 32, 33, 42, 41, 33, 34, 43, 42, 34, 35, 44, 43, 36, 37, 46, 45, 37, 38, 47, 46, 38, 39, 48, 47, 39,
                              40, 49, 48, 40, 41, 50, 49, 41, 42, 51, 50, 42, 43, 52, 51, 43, 44, 53, 52, 45, 46, 55, 54, 46, 47, 56, 55,
                              47, 48, 57, 56, 48, 49, 58, 57, 49, 50, 59, 58, 50, 51, 60, 59, 51, 52, 61, 60, 52, 53, 62, 61, 1, 0, 63, 2,
                              1, 64, 3, 2, 65, 4, 3, 66, 5, 4, 67, 6, 5, 68, 7, 6, 69, 8, 7, 70, 54, 55, 71, 55, 56, 72, 56, 57, 73, 57, 58,
                              74, 58, 59, 75, 59, 60, 76, 60, 61, 77, 61, 62, 78
                             );

   float vertices[174] =    {
                              0.13529903f, -0.46193975f, -0.13529903f, 0.0f, -0.46193975f, -0.1913417f, -0.13529903f, -0.46193975f, -0.13529903f,
                              -0.1913417f, -0.46193975f, 0.0f, -0.13529903f, -0.46193975f, 0.13529903f, 0.0f, -0.46193975f, 0.19134171f,
                              0.13529903f, -0.46193975f, 0.13529903f, 0.19134173f, -0.46193975f, 0.0f, 0.24999997f, -0.35355338f, -0.24999997f,
                              0.0f, -0.35355338f, -0.35355335f, -0.24999997f, -0.35355338f, -0.24999997f, -0.35355335f, -0.35355338f, 0.0f,
                              -0.24999997f, -0.35355338f, 0.24999997f, 0.0f, -0.35355338f, 0.35355335f, 0.24999999f, -0.35355338f, 0.24999999f,
                              0.35355338f, -0.35355338f, 0.0f, 0.3266407f, -0.19134171f, -0.3266407f, 0.0f, -0.19134171f, -0.46193969f,
                              -0.3266407f, -0.19134171f, -0.3266407f, -0.46193969f, -0.19134171f, 0.0f, -0.3266407f, -0.19134171f, 0.3266407f,
                              0.0f, -0.19134171f, 0.46193972f, 0.32664073f, -0.19134171f, 0.32664073f, 0.46193975f, -0.19134171f, 0.0f,
                              0.35355335f, 0.0f, -0.35355335f, 0.0f, 0.0f, -0.49999994f, -0.35355335f, 0.0f, -0.35355335f, -0.49999994f, 0.0f, 0.0f,
                              -0.35355335f, 0.0f, 0.35355335f, 0.0f, 0.0f, 0.49999997f, 0.35355338f, 0.0f, 0.35355338f, 0.5f, 0.0f, 0.0f, 0.3266407f, 0.19134171f, -0.3266407f,
                              0.0f, 0.19134171f, -0.46193969f, -0.3266407f, 0.19134171f, -0.3266407f, -0.46193969f, 0.19134171f, 0.0f,
                              -0.3266407f, 0.19134171f, 0.3266407f, 0.0f, 0.19134171f, 0.46193972f, 0.32664073f, 0.19134171f, 0.32664073f,
                              0.46193975f, 0.19134171f, 0.0f, 0.24999997f, 0.35355338f, -0.24999997f, 0.0f, 0.35355338f, -0.35355335f,
                              -0.24999997f, 0.35355338f, -0.24999997f, -0.35355335f, 0.35355338f, 0.0f, -0.24999997f, 0.35355338f, 0.24999997f,
                              0.0f, 0.35355338f, 0.35355335f, 0.24999999f, 0.35355338f, 0.24999999f, 0.35355338f, 0.35355338f, 0.0f,
                              0.13529903f, 0.46193975f, -0.13529903f, 0.0f, 0.46193975f, -0.1913417f, -0.13529903f, 0.46193975f, -0.13529903f,
                              -0.1913417f, 0.46193975f, 0.0f, -0.13529903f, 0.46193975f, 0.13529903f, 0.0f, 0.46193975f, 0.19134171f,
                              0.13529903f, 0.46193975f, 0.13529903f, 0.19134173f, 0.46193975f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f
                            };

   AtArray* vlist = AiArrayConvert(58, 1, AI_TYPE_VECTOR, &(vertices[0]));

   float uvs[158] =         {
                              0.0f, 0.125f, 0.125f, 0.125f, 0.25f, 0.125f, 0.375f, 0.125f, 0.5f, 0.125f, 0.625f, 0.125f, 0.75f, 0.125f, 0.875f, 0.125f,
                              0.99999f, 0.125f, 0.0f, 0.25f, 0.125f, 0.25f, 0.25f, 0.25f, 0.375f, 0.25f, 0.5f, 0.25f, 0.625f, 0.25f, 0.75f, 0.25f, 0.875f, 0.25f, 
                              0.99999f, 0.25f, 0.0f, 0.375f, 0.125f, 0.375f, 0.25f, 0.375f, 0.375f, 0.375f, 0.5f, 0.375f, 0.625f, 0.375f, 0.75f, 0.375f, 
                              0.875f, 0.375f, 0.99999f, 0.375f, 0.0f, 0.5f, 0.125f, 0.5f, 0.25f, 0.5f, 0.375f, 0.5f, 0.5f, 0.5f, 0.625f, 0.5f, 0.75f, 0.5f, 
                              0.875f, 0.5f, 0.99999f, 0.5f, 0.0f, 0.625f, 0.125f, 0.625f, 0.25f, 0.625f, 0.375f, 0.625f, 0.5f, 0.625f, 0.625f, 0.625f, 
                              0.75f, 0.625f, 0.875f, 0.625f, 0.99999f, 0.625f, 0.0f, 0.75f, 0.125f, 0.75f, 0.25f, 0.75f, 0.375f, 0.75f, 0.5f, 0.75f, 0.625f, 0.75f, 
                              0.75f, 0.75f, 0.875f, 0.75f, 0.99999f, 0.75f, 0.0f, 0.875f, 0.125f, 0.875f, 0.25f, 0.875f, 0.375f, 0.875f, 0.5f, 0.875f, 
                              0.625f, 0.875f, 0.75f, 0.875f, 0.875f, 0.875f, 0.99999f, 0.875f, 0.0625f, 0.0f, 0.1875f, 0.0f, 0.3125f, 0.0f, 0.4375f, 0.0f, 0.5625f, 0.0f, 
                              0.6875f, 0.0f, 0.8125f, 0.0f, 0.9375f, 0.0f, 0.0625f, 0.99999f, 0.1875f, 0.99999f, 0.3125f, 0.99999f, 0.4375f, 0.99999f, 0.5625f, 0.99999f, 0.6875f, 0.99999f, 
                              0.8125f, 0.99999f, 0.9375f, 0.99999f 
                            };

   AtArray *uvlist = AiArrayConvert(79, 1, AI_TYPE_VECTOR2, &(uvs[0]));

   AiNodeSetArray(sph, "nsides", nsides);
   AiNodeSetArray(sph, "vidxs", vidxs);
   AiNodeSetArray(sph, "uvidxs", uvidxs);
   AiNodeSetArray(sph, "vlist", vlist);
   AiNodeSetArray(sph, "uvlist", uvlist);

   // Add tangents for better anisotropic preview
   AiNodeDeclare(sph, "tangent", "varying VECTOR");
   AiNodeDeclare(sph, "bitangent", "varying VECTOR");

   float tgts[174] =       {
                              -0.65328145f, -6.9356688e-08f, -0.65328145f, -0.92387944f, -1.7763568e-15f, -1.4901161e-08f,
                              -0.65328139f, 1.6896466e-08f, 0.65328139f, -7.4505806e-09f, 6.108297e-09f, 0.9238795f, 0.65328139f, -3.5501905e-08f, 0.65328145f,
                              0.9238795f, -1.6747313e-08f, 7.4505806e-09f, 0.65328151f, 5.8758971e-09f, -0.65328145f, 1.4901161e-08f, -2.9629858e-08f, -0.9238795f,
                              -0.65328151f, -4.1773145e-08f, -0.65328145f, -0.9238795f, -3.0965281e-09f, -1.4901161e-08f,
                              -0.65328139f, 1.5258161e-08f, 0.65328145f, 0.0f, 5.9914669e-09f, 0.9238795f, 0.65328145f, -3.7388681e-08f, 0.65328145f,
                              0.9238795f, -6.4997159e-09f, 2.2351742e-08f, 0.65328145f, -6.0602221e-09f, -0.65328139f, -7.4505806e-09f, -2.9228177e-08f, -0.92387944f,
                              -0.65328151f, 1.7873346e-09f, -0.65328145f, -0.92387944f, -4.0362891e-10f, 0.0f, -0.65328145f, 9.9857269e-09f, 0.65328145f,
                              7.4505806e-09f, 1.0319953e-09f, 0.92387944f, 0.65328145f, -2.4011302e-09f, 0.65328145f, 0.92387944f, 2.6300044e-09f, 5.9604645e-08f,
                              0.65328151f, -4.9289572e-10f, -0.65328145f, -6.7055225e-08f, -5.920664e-11f, -0.9238795f, -0.65328151f, 8.0725793e-10f, -0.65328139f,
                              -0.9238795f, -8.0725826e-10f, 0.0f, -0.65328145f, 3.3114511e-10f, 0.65328145f, 0.0f, 1.4496759e-10f, 0.9238795f,
                              0.65328145f, -2.1745161e-10f, 0.65328151f, 0.9238795f, -4.0362902e-10f, 1.0430813e-07f, 0.65328145f, 7.2483353e-11f, -0.65328133f,
                              -5.9604645e-08f, 7.2484574e-11f, -0.92387938f, -0.65328151f, 4.5547441e-09f, -0.65328145f, -0.92387944f, -1.7873343e-09f, 0.0f,
                              -0.65328139f, 2.2634461e-09f, 0.65328139f, -7.4505806e-09f, -1.311221e-09f, 0.92387938f, 0.65328139f, -2.3916296e-09f, 0.65328145f,
                              0.9238795f, 3.7737662e-09f, 7.4505806e-08f, 0.65328145f, 7.4839068e-09f, -0.65328139f, -4.4703484e-08f, 5.1311924e-09f, -0.9238795f,
                              -0.65328145f, 3.3575528e-08f, -0.65328145f, -0.92387944f, 4.9514273e-09f, 1.4901161e-08f, -0.65328139f, -5.3755318e-08f, 0.65328145f,
                              2.2351742e-08f, -1.416011e-08f, 0.9238795f, 0.65328139f, 1.4049734e-08f, 0.65328145f, 0.9238795f, 1.1992869e-08f, 5.2154064e-08f,
                              0.65328145f, 1.7575065e-08f, -0.65328139f, -2.9802322e-08f, 4.4397726e-08f, -0.9238795f, -0.65328145f, 5.7458521e-08f, -0.65328145f,
                              -0.92387944f, 7.7295281e-09f, 7.4505806e-09f, -0.65328133f, -6.4214369e-08f, 0.65328139f, 2.2351742e-08f, -1.1927268e-08f, 0.9238795f,
                              0.65328139f, 1.8769143e-08f, 0.65328145f, 0.9238795f, 1.2018861e-08f, 1.4901161e-08f, 0.65328145f, -4.9060702e-09f, -0.65328145f,
                              1.4901161e-08f, 4.0133635e-08f, -0.9238795f, 1.4901161e-08f, 0.0f, -2.9802322e-08f, 1.4901161e-08f, 0.0f, -2.9802322e-08f
                            };

   AtArray *tgtlist = AiArrayConvert(58, 1, AI_TYPE_VECTOR, &(tgts[0]));

   float bitgts[174] =      {
                              0.58399749f, 0.39825299f, -0.58399761f, 7.4505806e-09f, 0.39825299f, -0.82589734f, -0.58399755f, 0.39825302f, -0.58399749f,
                              -0.82589734f, 0.39825302f, -1.4901161e-08f, -0.58399761f, 0.39825302f, 0.58399755f, 7.4505806e-09f, 0.39825299f, 0.82589734f,
                              0.58399755f, 0.39825302f, 0.58399761f, 0.82589728f, 0.39825302f, 0.0f, 0.43625486f, 0.71847582f, -0.43625492f,
                              1.4901161e-08f, 0.71847582f, -0.61695755f, -0.43625492f, 0.71847582f, -0.43625486f, -0.61695766f, 0.71847582f, -1.4901161e-08f,
                              -0.43625492f, 0.71847582f, 0.43625492f, -7.4505806e-09f, 0.71847582f, 0.6169576f, 0.43625489f, 0.71847582f, 0.43625495f,
                              0.61695755f, 0.71847582f, 0.0f, 0.23061934f, 0.91722697f, -0.23061936f, 0.0f, 0.91722697f, -0.32614499f,
                              -0.23061934f, 0.91722697f, -0.23061934f, -0.32614496f, 0.91722697f, -1.3038516e-08f, -0.23061934f, 0.91722697f, 0.23061936f,
                              -7.4505806e-09f, 0.91722691f, 0.32614505f, 0.23061942f, 0.91722691f, 0.23061942f, 0.32614505f, 0.91722697f, 3.7252903e-09f,
                              0.0f, 0.9835301f, -1.8626451e-09f, 1.8626451e-09f, 0.9835301f, 3.7252903e-09f, -1.8626451e-09f, 0.98353016f, 1.8626451e-09f,
                              3.7252903e-09f, 0.98353016f, 1.8626451e-09f, 0.0f, 0.98353016f, 0.0f, 0.0f, 0.98353016f, 0.0f, 3.7252903e-09f, 0.9835301f, 1.8626451e-09f,
                              -3.7252903e-09f, 0.9835301f, 0.0f, -0.23061934f, 0.91722703f, 0.23061937f, -1.8626451e-09f, 0.91722697f, 0.32614499f,
                              0.23061934f, 0.91722697f, 0.23061934f, 0.32614499f, 0.91722703f, 9.3132257e-09f, 0.23061934f, 0.91722691f, -0.23061936f,
                              9.3132257e-09f, 0.91722691f, -0.32614505f, -0.2306194f, 0.91722691f, -0.23061942f, -0.32614508f, 0.91722691f, -7.4505806e-09f,
                              -0.43625486f, 0.71847582f, 0.43625489f, 7.4505806e-09f, 0.71847582f, 0.61695755f, 0.43625489f, 0.71847582f, 0.43625489f,
                              0.61695766f, 0.71847582f, -7.4505806e-09f, 0.43625492f, 0.71847582f, -0.43625489f, 1.4901161e-08f, 0.71847582f, -0.6169576f,
                              -0.43625489f, 0.71847582f, -0.43625495f, -0.61695755f, 0.71847582f, 2.6077032e-08f, -0.58399755f, 0.39825299f, 0.58399761f,
                              0.0f, 0.39825299f, 0.82589728f, 0.58399755f, 0.39825302f, 0.58399755f, 0.82589734f, 0.39825302f, -1.4901161e-08f,
                              0.58399755f, 0.39825302f, -0.58399749f, 0.0f, 0.39825299f, -0.82589734f, -0.58399755f, 0.39825302f, -0.58399761f,
                              -0.82589722f, 0.39825302f, 7.4505806e-09f, -1.4901161e-08f, 0.21047819f, 1.4901161e-08f, 1.4901161e-08f, 0.21047819f, -1.4901161e-08f
                            };

   AtArray *bitgtlist = AiArrayConvert(58, 1, AI_TYPE_VECTOR, &(bitgts[0]));

   AiNodeSetArray(sph, "tangent", tgtlist);
   AiNodeSetArray(sph, "bitangent", bitgtlist);

   // TODO : in some cases might be interesting to use more subdivision (displacement)
   // while in some cases it's not (wireframe shader)
   AiNodeSetBool(sph, "smoothing", true);
   AiNodeSetStr(sph, "subdiv_type", "catclark");

   return sph;
}