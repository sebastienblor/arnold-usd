#include "platform/Platform.h"
#include "utils/Universe.h"
#include "RenderSwatch.h"

#include "extension/ExtensionsManager.h"
#include "session/SessionManager.h"
#include "utils/time.h"
#include "utils/AiAdpPayload.h"
#include "utils/MayaUtils.h"
#include "utils/ConstantStrings.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"


#include <maya/MImage.h>
#include <ai_msg.h>
#include <ai_node_entry.h>
#include <ai_nodes.h>
#include <assert.h>
#include <string.h> // for memset.
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

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
   const void * address = static_cast<const void*>(this);
   std::stringstream ss;
   ss << address;  
   m_sessionId = "swatch_";
   m_sessionId += ss.str(); 
   ClearSwatch();
   SetSwatchClass(dependNode);
   m_initialized = false;
   

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

bool returnResult(MStatus status)
{
   if (status == MStatus::kSuccess) { return true;}
   else {return false ;}
}


MStatus CRenderSwatchGenerator::BuildArnoldScene(CArnoldSession *session)
{
   MStatus status;
   
   MObject mayaNode = swatchNode();
   
   // What is this? Seem to always get the same node
   // MObject renderNode = node();
   AtUniverse *universe = session->GetUniverse();

   MString mayaNodeName = "";
   MString mayaNodeType = MFnDependencyNode(mayaNode).typeName();
   if (mayaNode.hasFn(MFn::kDagNode))
      mayaNodeName = MFnDagNode(mayaNode).partialPathName();
   else
      mayaNodeName = MFnDependencyNode(mayaNode).name();

   if (!m_initialized)
   {
      m_initialized = true;

      AtNode *geometry = NULL;
      MString typeName = MFnDependencyNode(swatchNode()).typeName();

      if (typeName == "aiHair" || typeName == "aiStandardHair")
      {

         geometry = AiNode(universe, str::sphere, str::geometry);
         if (NULL != geometry)
         {
            AiNodeSetBool(geometry, str::opaque, false);
            AtMatrix matrix = {{ { 1.0f, 0.0f, 0.0f, 0.0f },
                               { 0.0f, -1.0f, 0.0f, 0.0f },
                               { 0.0f, 0.0f, 1.0f, 0.0f },
                               { 0.0f, 0.0f, 0.0f, 1.0f } }};
            AiNodeSetMatrix(geometry, str::matrix, matrix);
         }
      }
      else
      {
         geometry = PolySphere(universe);
         if (NULL != geometry)
         {
            AiNodeSetStr(geometry, str::name, str::geometry);
            AiNodeSetByte(geometry, str::subdiv_iterations, 1);
            AiNodeSetBool(geometry, str::opaque, false);
         }
      }
      AtNode* camera = AiNode(universe, str::persp_camera, str::camera);
      AiNodeSetVec(camera, str::position, 0.f, 0.f, 1.14f);
   }

   // Export the swatched shading node
   AtNode* arnoldNode = NULL;
   CNodeTranslator* translator = NULL;
   if (mayaNode.hasFn(MFn::kDagNode))
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(mayaNode, dagPath);
      translator = session->ExportDagPath(dagPath);
   } 
   else 
      translator = session->ExportNode(mayaNode);
   
   if (translator)
      arnoldNode = translator->GetArnoldNode();

   if (arnoldNode == nullptr)
      return MS::kFailure;

   MString arnoldNodeName(AiNodeGetName(arnoldNode));
   
   // Assign it in the scene, depending on what it is
   status = AssignNode(arnoldNode, translator);
   
   
   // Apply any swatch options (overrides) that are present on mayaNode
   /*
   if (MStatus::kSuccess != ApplyOverrides(translator))
   {
      ErrorSwatch("Could not apply overrides present on \"" + mayaNodeName + "\"");
   }*/

   return MStatus::kSuccess;

}

MStatus CRenderSwatchGenerator::AssignNode(AtNode* arnoldNode, CNodeTranslator* translator)
{
   MStatus status;
   MFnDependencyNode depFn(swatchNode());
   AtUniverse *universe = AiNodeGetUniverse(arnoldNode);
   // Assign what needs to be on geometry

   AtNode* geometry = AiNodeLookUpByName(universe, str::geometry);

   // Assign exported geometry shader or use default one
   if (m_swatchClass == SWATCH_SHADER)
   {
      AiNodeSetPtr(geometry, str::shader, arnoldNode);
   } else {
      AtNode* defaultShader = AiNode(universe, str::standard_surface, str::_default);
      AiNodeSetPtr(geometry, str::shader, defaultShader);
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

      if (strcmp(AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)), str::mesh_light) == 0)
      {
         // Special case for mesh lights.
         // The mesh can be of any size and shape and will not give a consistent 
         // result. So instead use a quad light stand-in with the same properties.
         AtNode *light = AiNode(universe, str::quad_light, str::light);
         AiNodeSetMatrix(light, str::matrix, matrix);
         AtRGB color = AiNodeGetRGB(arnoldNode, str::color);
         AiNodeSetRGB(light,  str::color, color.r, color.g, color.b);
         AiNodeSetFlt(light,  str::intensity,       AiNodeGetFlt(arnoldNode, str::intensity));
         AiNodeSetFlt(light,  str::exposure,        AiNodeGetFlt(arnoldNode, str::exposure));
         AiNodeSetInt(light,  str::samples,         AiNodeGetInt(arnoldNode, str::samples));
         AiNodeSetBool(light, str::normalize,       AiNodeGetBool(arnoldNode, str::normalize));
         AiNodeSetFlt(light,  str::diffuse,         AiNodeGetFlt(arnoldNode, str::diffuse));
         AiNodeSetFlt(light,  str::specular,        AiNodeGetFlt(arnoldNode, str::specular));

         // Hide original mesh light
         AtNode* meshNode = translator->GetArnoldNode("mesh");
         if (meshNode)
            AiNodeSetDisabled(meshNode, true);

         AiNodeSetDisabled(arnoldNode, true);
      }
      else
      {
         AiNodeSetMatrix(arnoldNode, str::matrix, matrix);
      }
   }
   else if (m_swatchClass == SWATCH_LIGHTFILTER || m_swatchClass == SWATCH_ATMOSPHERE)
   {
      // Use a spot light at -1, 1, 1 looking at 0, 0, 0
      // to preview light filters and atmosphere effects
      AtNode *light = AiNode(universe, str::spot_light, str::light);
      AiNodeSetFlt(light, str::intensity, 1.f);
      AiNodeSetVec(light, str::position, -1.f, 1.f, 1.f);
      AiNodeSetVec(light, str::look_at, 0.f, 0.f, 0.f);
      // AiNodeSetFlt(light, "cone_angle", 30.f);

      if (m_swatchClass == SWATCH_LIGHTFILTER)
      {
         AiNodeSetPtr(light, str::filters, arnoldNode);
      }
      else if (m_swatchClass == SWATCH_ATMOSPHERE)
      {
         AiNodeSetBool(light, str::cast_volumetric_shadows, true);
      }
   }
   else
   {
      // Default light for swatching shaders etc
      AtNode *light = AiNode(universe, str::distant_light, str::light);
      AiNodeSetVec(light, str::direction, 1.0f, -1.f, -1.0f);
   }

   // Set the global options for background and atmosphere
   
   AtNode * const options = AiUniverseGetOptions(universe);
   
   MTime ct = MAnimControl::currentTime();
   AiNodeSetFlt(options, str::frame, (float)ct.value());

   // If we are swatching an environment (background) shader
   if (m_swatchClass == SWATCH_ENVIRONMENT)
   {
      AiNodeSetPtr(options, str::background, arnoldNode);
   }
   else
   {
      // Add a default sky shader to get solid alpha
      // TODO : options to use a custom environment for swatches or use render setting's?
      AtNode* background = AiNode(universe, str::sky, str::background);
      AiNodeSetRGB (background, str::color, 0.0f, 0.0f, 0.0f);
      AiNodeSetPtr(options, str::background, background);
   }

   // If we are swatching an atmosphere shader
   if (m_swatchClass == SWATCH_ATMOSPHERE)
   {
      AiNodeSetPtr(options, str::atmosphere, arnoldNode);
   }
   AiNodeSetBool(options, str::skip_license_check, true);
   AiNodeSetBool(options, str::texture_automip, false);

   return MStatus::kSuccess;
}

/* FIXME should we restore this ?
MStatus CRenderSwatchGenerator::ApplyOverrides(CNodeTranslator* translator)
{
   // Temporary until it is exposed somewhere
   AtNode * const options = AiUniverseGetOptions();
   
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
*/

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

bool CRenderSwatchGenerator::DoSwatchRender()
{
   CArnoldSession *session = nullptr;
   session = new CArnoldSession(false); // don't init the sessionOptions here
   if (!CSessionManager::AddActiveSession(m_sessionId, session))
   {
      delete session;
      return false;
   }
   
   MObject ArnoldRenderOptionsNode = CArnoldSession::GetDefaultArnoldRenderOptions();
   AtUniverse *universe = session->GetUniverse();
   AtRenderSession *renderSession = session->GetRenderSession();
   CSessionOptions &sessionOptions = session->GetOptions();
   sessionOptions.SetSupportGpu(false);
   BuildArnoldScene(session);
   int res = resolution();
 
   image().create(res, res,
                  4,                              // RGBA
                  MImage::kFloat);                // Has to be for swatches it seems.

   // if use tx is enabled, call exportTx that will *not* try to convert the mipmaps
   // but will check for existing tx for sake of optimization
      
   // Here the session options haven't been updated (for optimization reasons), so we need
   // to look for the render options node parameters and fill the options before calling ExportTxFiles
   MStatus stat;
   MFnDependencyNode fnOptions(sessionOptions.GetArnoldRenderOptions(), &stat);
   if (stat != MS::kSuccess || fnOptions.findPlug("use_existing_tiled_textures", true).asBool())
   {
      sessionOptions.SetAutoTx(false);
      sessionOptions.SetUseExistingTx(true);
      session->ExportTxFiles();
   }

   // Use the render view output driver. It will *not* be displayed
   // in the render view, we're just using the Arnold Node.
   // See DisplayUpdateQueueToMImage() for how we get the image.
   AtNode* render_view = AiNode(universe, str::renderview_display, str::swatch_renderview_display);
   
   AiNodeSetPtr(render_view, str::swatch, image().floatPixels());


   AtNode* filter = AiNode(universe, str::gaussian_filter, str::swatch_renderview_filter);
   AiNodeSetFlt(filter, str::width, 2.0f);

   AtNode* options = AiUniverseGetOptions(universe);

   COptionsTranslator::AddProjectFoldersToSearchPaths(options);
   AiNodeDeclare(options, str::is_swatch, str::constant_BOOL);
   AiNodeSetBool(options, str::is_swatch, true);
   AiNodeSetStr(options, str::pin_threads, str::off);
   AiNodeSetInt(options, str::threads, 4);

   MString texture_searchpath = fnOptions.findPlug("texture_searchpath", true).asString();
   if (texture_searchpath.length() > 0)
      AiNodeSetStr(options, str::texture_searchpath, AtString(texture_searchpath.asChar()));

   COptionsTranslator::AddProjectFoldersToSearchPaths(options);

   // Create the single output line. No AOVs or anything.
   AtArray* outputs  = AiArrayAllocate(1, 1, AI_TYPE_STRING);
   std::string outputsStr ("RGBA RGBA ");
   outputsStr += AiNodeGetName(filter);
   outputsStr += " ";
   outputsStr += AiNodeGetName(render_view);
   
   AiArraySetStr(outputs, 0, outputsStr.c_str());
   AiNodeSetArray(options, str::outputs, outputs);

   // Most options should be read from an ass so just need to set the res and
   // guess a reasonable bucket size.
   AiNodeSetInt(options, str::xres, res);
   AiNodeSetInt(options, str::yres, res);
   AiNodeSetInt(options, str::bucket_size, res/4);
   AiNodeSetInt(options, str::GI_sss_samples, 4);


   // Start the render on the current thread.
   AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_MATERIAL_SWATCH);
   AiRenderSetHintBool(renderSession, str::progressive, false);
   AiRenderBegin(renderSession);
   
   while (true)
   {
      AtRenderStatus status = AiRenderGetStatus(renderSession);
      if (status == AI_RENDER_STATUS_FINISHED || status == AI_RENDER_STATUS_FAILED)
         break;
   }

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

   AiRenderInterrupt(renderSession, AI_BLOCKING);   
   AiRenderEnd(renderSession);
  
   CSessionManager::DeleteActiveSession(m_sessionId);
 
   return true;
}

bool CRenderSwatchGenerator::DoNoGPUImage()
{
   MString iconsPath = MString(getenv("MTOA_PATH")) +MString ("icons//") ;
   MString noGpuPath = iconsPath + MString ("noGPU.png");
   MStatus stat =  image().readFromFile(noGpuPath);
   image().verticalFlip();
   image().resize(resolution(), resolution());
   
   return returnResult(stat);
}

bool CRenderSwatchGenerator::DoStaticImage()
{
   MString iconsPath = MString(getenv("MTOA_PATH")) +MString ("icons//") ;
   MString noGpuPath = iconsPath + MString ("arnold.png");
   MStatus stat =  image().readFromFile(noGpuPath);
   image().verticalFlip();
   image().resize(resolution(), resolution());
   
   return returnResult(stat);
}


bool CRenderSwatchGenerator::doIteration()
{
   if (IsBatch())
      return false;

   MObject mayaNode = swatchNode();
   MObject arnoldRenderOptionsNode = CArnoldSession::GetDefaultArnoldRenderOptions();

   if (arnoldRenderOptionsNode.isNull())
   {
      MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()"); 
      arnoldRenderOptionsNode = CArnoldSession::GetDefaultArnoldRenderOptions();
   }
   MFnDependencyNode depNode(mayaNode);
   MString nodeType = depNode.typeName();
   
   const ArnoldNodeMetadataStore *metadataStore = CExtensionsManager::FindNodeMetadatas(nodeType, true);
   bool sceneSwatch = MFnDependencyNode(arnoldRenderOptionsNode).findPlug("enable_swatch_render", true).asBool();
   bool gpuRenderCompatibility = true;
   bool doSwatch = true;

   if (metadataStore)
   {
      for (size_t i = 0; i < metadataStore->size(); ++i)
      {
         const AtString &metadataName = metadataStore->at(i).name;
         static const AtString gpu_supportStr("gpu_support");
         static const AtString maya_swatchStr("maya.swatch");
         if (metadataName == gpu_supportStr) {
            gpuRenderCompatibility = metadataStore->at(i).value.BOOL();
         }
         else if (metadataName == maya_swatchStr)
            doSwatch = metadataStore->at(i).value.BOOL();
      }
   }

   if (!gpuRenderCompatibility)
   {
      // If GPU isn't supported for this node, we always show the no-gpu icon
      DoNoGPUImage();
   } else if (doSwatch && sceneSwatch)
   {
      // if swatch is enabled, we can start a swatch rendering
      DoSwatchRender();
   } else
   {
      // fallback behaviour, show a static image
      DoStaticImage();
   }


   return true;
}

// This will create a polygon sphere for swatching
AtNode* CRenderSwatchGenerator::PolySphere(AtUniverse *universe)
{
   AtNode *sph = AiNode(universe, str::polymesh);

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

   AiNodeSetArray(sph, str::nsides, nsides);
   AiNodeSetArray(sph, str::vidxs, vidxs);
   AiNodeSetArray(sph, str::uvidxs, uvidxs);
   AiNodeSetArray(sph, str::vlist, vlist);
   AiNodeSetArray(sph, str::uvlist, uvlist);

   // Add tangents for better anisotropic preview
   AiNodeDeclare(sph, str::tangent, str::varying_VECTOR);
   AiNodeDeclare(sph, str::bitangent, str::varying_VECTOR);

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

   AiNodeSetArray(sph, str::tangent, tgtlist);
   AiNodeSetArray(sph, str::bitangent, bitgtlist);

   // TODO : in some cases might be interesting to use more subdivision (displacement)
   // while in some cases it's not (wireframe shader)
   AiNodeSetBool(sph, str::smoothing, true);
   AiNodeSetStr(sph, str::subdiv_type, str::catclark);

   return sph;
}