#include "utils/MtoaLog.h"
#include "scene/MayaScene.h"
#include "render/RenderSession.h"
#include "RenderSwatch.h"

#include "extension/ExtensionsManager.h"


#include <maya/MImage.h>
#include <ai_msg.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <assert.h>
#include <string.h> // for memset.
#include <string>
#include <algorithm>

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
   m_renderSession = NULL;
   m_iteration = 0;
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

   AiMsgDebug("[mtoa] [swatch] [maya %s] of classification %s", depFn.name().asChar(), classification.asChar());

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
         else if (classes[j] == "displacement")
         {
            m_swatchClass = SWATCH_DISPLACEMENT;
            break;
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

   m_renderSession = CRenderSession::GetInstance();
   m_renderSession->Init();
   m_renderSession->SetBatch(false);
   m_renderSession->SetProgressive(false);
   m_renderSession->GetMayaScene()->SetExportMode(MTOA_EXPORT_SWATCH);
   // Problem. Will cause a creash when exporting a dag node (moblur checking)
   // if left to NULL. But we don't want the scene options and don't want to create
   // a dummy Maya node just to have default options
   // m_renderSession->GetMayaScene()->PrepareExport();

   AiBegin();
   // TODO: Should we be using render options for logging, or is it better not to clutter
   // the log with swatch output?
   // If we use global render options, m_renderSession->Init() already did
   // m_renderSession->m_renderOptions.SetupLog();
   MtoaSetupLogging();
   // TODO: should use the list of loaded plugins from CExtensionsManager instead
   m_renderSession->LoadPlugins();

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
   if (MStatus::kSuccess != LoadAssForNode()
         && MStatus::kSuccess != DefaultArnoldScene())
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
      return MStatus::kFailure;
   }
   MString arnoldNodeName(AiNodeGetName(arnoldNode));
   if (NULL != arnoldNode) {
      const AtNodeEntry *nodeEntry = arnoldNode->base_node;
      AiMsgDebug("[mtoa] [swatch] Exported %s(%s) as %s(%s)",
            mayaNodeName.asChar(), mayaNodeType.asChar(),
            AiNodeGetName(arnoldNode), AiNodeEntryGetTypeName(nodeEntry));
   }

   // Assign it in the scene, depending on what it is
   status = AssignNode(arnoldNode);
   if (MStatus::kSuccess != status)
   {
      ErrorSwatch("Could not assign \"" + mayaNodeName + "\" exported as \"" + arnoldNodeName + "\"");
      return MStatus::kFailure;
   }
   // Add a camera
   // TODO : would it be possible / interesting to allow preview of cameras?
   AtNode* camera = AiNode("persp_camera");
   AiNodeSetStr(camera, "name", "camera");
   AiNodeSetPnt(camera, "position", 0.f, 0.f, 1.14f);
   // Apply any swatch options (overrides) that are present on mayaNode
   if (MStatus::kSuccess != ApplyOverrides(translator))
   {
      ErrorSwatch("Could not apply overrides present on \"" + mayaNodeName + "\"");
      return MStatus::kFailure;
   }

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

   if (m_renderSession->LoadAss(c_ass.c_str()) == AI_SUCCESS)
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

   geometry = AiNode("sphere");
   if (NULL != geometry)
   {
      AiNodeSetStr(geometry, "name", "geometry");
      if (typeName == "aiDisplacement")
      {
         AiNodeSetBool(geometry, "opaque", true);
      }
      else
      {
         AiNodeSetBool(geometry, "opaque", false);
      }
      return MStatus::kSuccess;
   }
   
   return MStatus::kFailure;
}

MStatus CRenderSwatchGenerator::ExportNode(AtNode* & arnoldNode,
                                           CNodeTranslator* & translator)
{
   MStatus status;
   MObject mayaNode = swatchNode();
   CMayaScene * m_mayaScene = m_renderSession->GetMayaScene();

   // FIXME: Special case for displacement
   if (m_swatchClass == SWATCH_DISPLACEMENT)
   {
      arnoldNode = NULL;
      // Get file translator in that case
      // Commented as later translator redesigns will break it
      translator = CExtensionsManager::GetTranslator("aiStandard");
      if (NULL != translator)
      {
         status = MStatus::kSuccess;
      }
      else
      {
         status = MStatus::kFailure;
      }
   }
   else
   {
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
            dagTranslator->Init(dagPath, m_mayaScene, "");
            arnoldNode = dagTranslator->DoExport(0);
         }
      } else {
         translator = CExtensionsManager::GetTranslator(mayaNode);
         if (NULL != translator)
         {
            translator->Init(mayaNode, m_mayaScene, "");
            arnoldNode = translator->DoExport(0);
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
   }

   return status;
}

MStatus CRenderSwatchGenerator::AssignNode(AtNode* arnoldNode)
{
   MStatus status;
   MFnDependencyNode depFn(swatchNode());
   CMayaScene * m_mayaScene = m_renderSession->GetMayaScene();

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

   // Export displacement map if required
   if (m_swatchClass == SWATCH_DISPLACEMENT)
   {
      AiNodeSetFlt(geometry, "disp_height", depFn.findPlug("disp_height").asFloat());
      AiNodeSetFlt(geometry, "disp_zero_value", depFn.findPlug("disp_zero_value").asFloat());
      AiNodeSetBool(geometry, "autobump", depFn.findPlug("autobump").asBool());

      MPlugArray connections;
      connections.clear();
      depFn.findPlug("disp_map").connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         MString attrName = connections[0].partialName(false, false, false, false, false, true);
         AtNode* dispImage(m_mayaScene->ExportShader(connections[0].node(), attrName));
         if (dispImage != NULL)
         {
            MPlug pVectorDisp = depFn.findPlug("vector_displacement", false);
            if (!pVectorDisp.isNull() && pVectorDisp.asBool())
            {
               AtNode* tangentToObject = AiNode("TangentToObjectSpace");
               m_mayaScene->ProcessShaderParameter(depFn, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
               AiNodeLink(dispImage, "map", tangentToObject);
               AiNodeSetPtr(geometry, "disp_map", tangentToObject);
            }
            else
            {
               AiNodeSetPtr(geometry, "disp_map", dispImage);
            }
         } else {
            ErrorSwatch("Could not export displacement map on \"" + depFn.name() + "\".");
            return MStatus::kFailure;
         }
      }
   }

   // If we are swatching a light or light filter

   if (m_swatchClass == SWATCH_LIGHT) {
      AiNodeSetPnt(arnoldNode, "position", -5.f, 5.f, 5.f);
   }
   else if (m_swatchClass == SWATCH_LIGHTFILTER)
   {
      // Use a spot light to preview light filters
      AtNode *light = AiNode("spot_light");
      AiNodeSetStr(light, "name", "light");
      AiNodeSetPnt(light, "position", -5.f, 5.f, 5.f);
      AiNodeSetPnt(light, "look_at", 0.f, 0.f, 0.f);

      AiNodeSetPtr(light, "filters", arnoldNode);
   }
   else
   {
      // Default swatch light
      AtNode *light = AiNode("point_light");
      AiNodeSetStr(light, "name", "light");
      AiNodeSetPnt(light, "position", -5.f, 5.f, 5.f);
   }

   // Set the global options for background and atmosphere
   
   AtNode * const options = AiUniverseGetOptions();

   // If we are swatching an environment (background) shader
   if (m_swatchClass == SWATCH_ENVIRONMENT)
   {
      AiNodeSetPtr(options, "background", arnoldNode);
   }
   else
   {
      // Add a default sky shader to get solid alpha
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
            const AtNodeEntry* nodeOverEntry = nodeOver->base_node;
            unsigned int nAttrOverrides = nodeOverPlug.numChildren();
            for (unsigned int a=0; a<nAttrOverrides; a++)
            {
               MPlug attrOverPlug = nodeOverPlug.child(a);
               MString attrOverName = attrOverPlug.partialName();
               const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(nodeOverEntry, attrOverName.asChar());
               translator->ProcessParameter(nodeOver, attrOverPlug, paramEntry, -1);
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

void CRenderSwatchGenerator::ClearSwatch()
{
   const int num_pixels(resolution() * resolution());
   image().create(resolution(), resolution(), 4, MImage::kByte);
   unsigned char * pixels = image().pixels();
   memset(pixels, 0, num_pixels * 4 * sizeof(char));
}

void CRenderSwatchGenerator::ErrorSwatch(const MString msg)
{
   const MString error_message("[mtoa] [swatch] "+msg);
   AiMsgError(error_message.asChar());
   ClearSwatch();
}

bool CRenderSwatchGenerator::doIteration()
{
   
   // Arnold is rendering, so bail out.
   // Return false to be called again.
   // This is how we manage to render many
   // swatches "at the same time".
   MStatus status;

   if (AiRendering()) {
      return false;
   }

   if (m_iteration == 0)
   {
      // Arnold can only render one thing at a time.
      // It may be an option to block/wait here, but only
      // if it's another swatch render taking place.
      if (CRenderSession::GetInstance()->IsActive())
      {
         return false;
      }
      // Build the swatch scene
      status = BuildArnoldScene();
      if (MStatus::kSuccess != status)
      {
         ErrorSwatch("Render failed: could not complete swatch scene.");
         m_renderSession->Finish();
         return true;
      }
   }
   // Scene/ass is built, so start the render.
   else if (m_iteration == 1)
   {
      if (!AiUniverseIsActive())
      {
         ErrorSwatch("Render failed: Arnold universe not active.");
         return true; // Stop iterating/rendering.
      }
      m_renderSession->DoSwatchRender(resolution());
   }
   // We must be done rendering.
   else
   {
      if (m_renderSession->GetSwatchImage(image()))
      {
         image().convertPixelFormat(MImage::kByte, 1.0f/255);
         m_renderSession->Finish();
         // Stop being called/iterated.
         return true;
      }
      else
      {
         // Start again as we were interupted.
         m_iteration = 0;
         return false;
      }
   }

   // Up the iteration count and return false so we're
   // called again.
   m_iteration++;
   return false;
}
