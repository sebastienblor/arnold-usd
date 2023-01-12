#include "MaterialView.h"

#include "platform/Platform.h"
#include "utils/Universe.h"
#include "utils/MtoAAdpPayloads.h"
#include "utils/ConstantStrings.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/options/OptionsTranslator.h"
#include "session/SessionManager.h"
#include "extension/ExtensionsManager.h"
#include "attributes/AttrHelper.h"

#include <maya/MSwatchRenderBase.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MAngle.h>
#include <assert.h>
#ifndef _WIN64
#include <unistd.h>
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern const AtNodeMethods* materialview_driver_mtd;
CMaterialView* CMaterialView::s_instance = NULL;

CMaterialView::CMaterialView()
: m_arnoldSession(nullptr)
, m_renderSession(nullptr)
, m_activeShader(NULL)
, m_dummyShader(NULL)
, m_environmentLight(NULL)
, m_environmentImage(NULL)
, m_active(false)
, m_running(false)
, m_suspended(false)
, m_interrupted(false)
, m_terminationRequested(false)
, m_refreshAllowed(false)
, m_refreshEvent(true, false)
, m_width(1)
, m_height(1)
{
}

CMaterialView::~CMaterialView()
{
   // The material view owns its own arnold session, so we need to clear it here
   delete m_arnoldSession;
}

AtRenderStatus MaterialViewUpdateCallback(void *private_data, AtRenderUpdateType update_type, const AtRenderUpdateInfo *update_info)
{   
   AtRenderStatus status = AI_RENDER_STATUS_RENDERING;
   if (update_type == AI_RENDER_UPDATE_FINISHED)
      status = AI_RENDER_STATUS_FINISHED;
   else if (update_type == AI_RENDER_UPDATE_INTERRUPT)
      status = AI_RENDER_STATUS_PAUSED;
   else if (update_type == AI_RENDER_UPDATE_ERROR)
      status = AI_RENDER_STATUS_FAILED;
   
   return status;
}

MStatus CMaterialView::startAsync(const JobParams& params)
{
   std::lock_guard<AtMutex> guard(m_runningLock);
   if (!IsActive())
      return MStatus::kFailure;


   // Guard for being called twice
   if (m_running)
   {
      // Thread already started
      return MStatus::kSuccess;
   }

   // Start the render thread.
   m_job = params;
   m_terminationRequested = false;
   m_running = true;

   if (m_arnoldSession == nullptr || m_renderSession == nullptr)
      BeginSession();

   InitOptions();
    
   if (AiRenderGetStatus(m_renderSession) != AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      AiRenderEnd(m_renderSession);
   }


   // Ensure we're not generating any TX, but use it if it was already generated
   AtNode *options = AiUniverseGetOptions(m_arnoldSession->GetUniverse());
   AiNodeSetBool(options, str::texture_auto_generate_tx, false);
   AiNodeSetBool(options, str::texture_use_existing_tx, true);
   
   AiRenderSetHintBool(m_renderSession, AtString("progressive"), true);
   AiRenderSetHintBool(m_renderSession, AtString("progressive_show_all_outputs"), false);
   AiRenderSetHintInt(m_renderSession, AtString("progressive_min_AA_samples"), -3);
   AiRenderSetHintStr(m_renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_MATERIAL_SWATCH);
   
   AiRenderBegin(m_renderSession, AI_RENDER_MODE_CAMERA, MaterialViewUpdateCallback, (void*)this);

   ScheduleRefresh();

   return MStatus::kSuccess;
}

MStatus CMaterialView::stopAsync()
{

   std::lock_guard<AtMutex> guard(m_runningLock);

   m_terminationRequested = true;
   InterruptRender(true);

   AiRenderInterrupt(m_renderSession, AI_BLOCKING);
   AiRenderEnd(m_renderSession);

   m_running = false;

   return MStatus::kSuccess;
}

bool CMaterialView::isRunningAsync()
{
   std::lock_guard<AtMutex> guard(m_runningLock);
   return m_running;
}

MStatus CMaterialView::beginSceneUpdate()
{
   
   if (IsActive())
   {
      // Session is already active
      // Interupt rendering to prepare for the scene updates
      // Don't wait for finish here, just let the renderer finish
      // in parallel, and instead check for final completion before
      // applying any scene updates.
      InterruptRender();
      return MStatus::kSuccess;
   }
   // Begin a new session
   InterruptRender();
   return BeginSession() ? MStatus::kSuccess : MStatus::kFailure;
}


MStatus CMaterialView::translateMesh(const MUuid& id, const MObject& node)
{
   if (!IsActive())
      return MStatus::kFailure;
   
   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   // Make sure the renderer is stopped
   InterruptRender(true);

   AtNode* geometryNode = TranslateDagNode(id, node, MV_UPDATE_RECREATE);
   if (!geometryNode) 
   {
      return MStatus::kFailure;
   }

   if (m_activeShader)
   {
      AiNodeSetPtr(geometryNode, str::shader, m_activeShader);
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateLightSource(const MUuid& id, const MObject& node)
{
   if (!IsActive())
      return MStatus::kFailure;
   
   std::lock_guard<AtMutex> guard(m_sceneLock);
   // Make sure the renderer is stopped
   InterruptRender(true);

   if (!TranslateDagNode(id, node))
   {
      return MStatus::kFailure;
   }
   return MStatus::kSuccess;
}

MStatus CMaterialView::translateCamera(const MUuid& id, const MObject& node)
{
   if (!IsActive())
      return MStatus::kFailure;
   
   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   // Make sure the renderer is stopped
   InterruptRender(true);

   if (!TranslateDagNode(id, node))
   {
      return MStatus::kFailure;
   }
   return MStatus::kSuccess;
}

MStatus CMaterialView::translateEnvironment(const MUuid& id, EnvironmentType type)
{
   if (!IsActive())
      return MStatus::kFailure;

   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   // Make sure the renderer is stopped
   InterruptRender(true);
   AtUniverse *universe= m_arnoldSession->GetUniverse();
   
   if (!m_environmentLight)
   {
       const static AtString mtrlViewSkyDome("mtrlViewSkyDome");
       m_environmentLight = AiNode(universe, str::skydome_light, mtrlViewSkyDome);
       AiNodeSetInt(m_environmentLight, str::format, 2);
       AiNodeSetRGB(m_environmentLight, str::color, 0.1f, 0.1f, 0.1f);
 
       AtMatrix rotation = AiM4RotationY(180.0f);
       AiNodeSetMatrix(m_environmentLight, str::matrix, rotation);
   }
   if (!m_environmentImage)
   {
      const static AtString mtrlViewSkyDomeImage("mtrlViewSkyDomeImage");
      m_environmentImage = AiNode(universe, str::image, mtrlViewSkyDomeImage);
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateTransform(const MUuid& id, const MUuid& childId, const MMatrix& mayaMatrix)
{
   if (!IsActive())
      return MStatus::kFailure;

   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   CNodeTranslator* translator = NULL;
   
   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it != m_translatorLookup.end())
   {
      translator = it->second;
   }
   else
   {
      it = m_translatorLookup.find(childId);
      if (it != m_translatorLookup.end())
      {
         translator = it->second;
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));
      }
   }

   if (translator)
   {
      AtMatrix matrix;
      translator->ConvertMatrix(matrix, mayaMatrix);

      // Make sure the renderer is stopped
      InterruptRender(true);

      AtNode* arnoldNode = translator->GetArnoldNode();
      AiNodeSetMatrix(arnoldNode, str::matrix, matrix);
   }
   else
   {
      return MStatus::kFailure;
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateShader(const MUuid& id, const MObject& node)
{
   if (!IsActive())
      return MStatus::kFailure;
   
   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   // Make sure the renderer is stopped 
   if (AiRenderGetStatus(m_renderSession) == AI_RENDER_STATUS_RENDERING)
   {
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
   }
   InterruptRender(true);

   if (!TranslateNode(id, node, MV_UPDATE_DEFAULT, true))
   {
      return MStatus::kFailure;
   }
   return MStatus::kSuccess;
}

MStatus CMaterialView::setProperty(const MUuid& id, const MString& name, bool value)
{
   return MStatus::kSuccess;
}

MStatus CMaterialView::setProperty(const MUuid& id, const MString& name, int value)
{
   return MStatus::kSuccess;
}

MStatus CMaterialView::setProperty(const MUuid& id, const MString& name, float value)
{
   return MStatus::kSuccess;
}

MStatus CMaterialView::setProperty(const MUuid& id, const MString& name, const MString& value)
{
   if (!IsActive())
      return MStatus::kFailure;
   
   std::lock_guard<AtMutex> guard(m_sceneLock);

   
   if (m_environmentLight && m_environmentImage)
   {
      static const MString s_imageFileStr("imageFile");
      if (name == s_imageFileStr)
      {
         // Make sure the renderer is stopped
         InterruptRender(true);

         AiNodeSetStr(m_environmentImage, str::filename, AtString(value.asChar()));
         if (value.length())
         {
            AiNodeLink(m_environmentImage, str::color, m_environmentLight);
         }
         else
         {
            AiNodeUnlink(m_environmentLight, str::color);
         }
      }
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::setShader(const MUuid& id, const MUuid& shaderId)
{
   if (!IsActive())
      return MStatus::kFailure;

   std::lock_guard<AtMutex> guard(m_sceneLock);
   
   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it == m_translatorLookup.end())
      return MStatus::kFailure;
   CNodeTranslator* geometryTranslator = it->second;
   AtNode* geometryNode = geometryTranslator->GetArnoldNode();

   AtNode* shaderNode = NULL;

   it = m_translatorLookup.find(shaderId);
   if (it != m_translatorLookup.end())
   {
      // Shader found among our translatated shaders
      CNodeTranslator* shaderTranslator = it->second;
      if (shaderTranslator)
         shaderNode = shaderTranslator->GetArnoldNode();
   }
   else
   {
      // No such shader translated, it's an unsupported shader type
      // Assign the black dummy shader to indicate this to the user
      shaderNode = m_dummyShader;
   }

   // Make sure the renderer is stopped
   InterruptRender(true);

   AiNodeSetPtr(geometryNode, str::shader, shaderNode);
   m_activeShader = shaderNode;

   return MStatus::kSuccess;
}

MStatus CMaterialView::setResolution(unsigned int width, unsigned int height)
{
   std::lock_guard<AtMutex> guard(m_sceneLock);

   m_width = width;
   m_height = height;

   if (m_active)
   {
      // Resolution updates are not wrapped in begin/end calls
      // so we need to interrupt and schedule refresh explicitly here

      InterruptRender(true);

      AtNode* options = AiUniverseGetOptions(m_arnoldSession->GetUniverse());
      AiNodeSetInt(options, str::xres, m_width);
      AiNodeSetInt(options, str::yres, m_height);

      ScheduleRefresh();
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::endSceneUpdate()
{
   m_arnoldSession->ExportTxFiles();
   if (AiRenderGetStatus(m_renderSession) == AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderSetHintStr(m_renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_MATERIAL_SWATCH);
      AiRenderBegin(m_renderSession, AI_RENDER_MODE_CAMERA, MaterialViewUpdateCallback, (void*)this);
   }
   else{
      AiRenderRestart(m_renderSession);
   }
   return MStatus::kSuccess;
}

MStatus CMaterialView::destroyScene()
{
   EndSession();

   return MStatus::kSuccess;
}

bool CMaterialView::isSafeToUnload()
{
   return !IsActive();
}

bool CMaterialView::BeginSession()
{
   std::lock_guard<AtMutex> guard(m_sceneLock);
   if (m_active || m_arnoldSession)
   {
      m_active = true;
      // We are already active. We should never get here.
      return true;
   }
   // For Material View, we're not storing the session in the session manager, but we just store it locally in this class
   if (!m_arnoldSession) 
   {
      m_arnoldSession = new CArnoldSession();
      m_arnoldSession->SetCheckVisibility(false);
      m_renderSession = m_arnoldSession->GetRenderSession();
   }
   CSessionOptions &sessionOptions = m_arnoldSession->GetOptions();
   sessionOptions.SetUseExistingTx(true);
   sessionOptions.SetAutoTx(false);
   sessionOptions.DisableMotionBlur();

   // Install our driver
   if (AiNodeEntryLookUp(str::materialview_display) == nullptr)
      AiNodeEntryInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "materialview_display", "mtoa", (AtNodeMethods*) materialview_driver_mtd, AI_VERSION);

   m_dummyShader = AiNode(m_arnoldSession->GetUniverse(), str::utility, str::mtrlViewDummyShader);
   AiNodeSetRGB(m_dummyShader, str::color, 0.0f, 0.0f, 0.0f);

   m_active = true;
   return true;
}

void CMaterialView::EndSession()
{
   std::lock_guard<AtMutex> guard(m_sceneLock);

   if (!m_active)
      return;
   
   // Make sure we are not rendering
   InterruptRender(true);

   // Cleanup
   
   m_translatorLookup.clear();
   m_activeShader      = NULL;
   m_dummyShader       = NULL;
   m_environmentLight  = NULL;
   m_environmentImage  = NULL;

   // Note: We must set the active flag before calling CMayaScene::End() below
   // to prevent reqursive entry since End() checks for active mtrl view session
   m_active = false;

   // Uninstall our driver
//   AiNodeEntryUninstall("materialview_display");

   m_renderSession = nullptr;
   delete m_arnoldSession;
   m_arnoldSession = nullptr;
   // End our scene session
   
   // Notify scene is destroyed
   SendProgress(-1);
   // Re-enable swatches
   //MSwatchRenderBase::enableSwatchRender(true);

}

bool CMaterialView::IsActive()
{
   std::lock_guard<AtMutex> guard(m_sceneLock);
   return m_active;
}

void CMaterialView::InitOptions()
{
   std::lock_guard<AtMutex> guard(m_sceneLock);
   AtUniverse *universe = m_arnoldSession->GetUniverse();
   AtNode* options = AiUniverseGetOptions(universe);

   // Set render camera
   TranslatorLookup::iterator it = m_translatorLookup.find(m_job.cameraId);
   if (it != m_translatorLookup.end())
   {
      AtNode* camera = it->second->GetArnoldNode();
      AiNodeSetPtr(options, str::camera, camera);
   }
   else
   {
      AiMsgError("Render camera not found!");
      assert(false); // Should never happen
   }

   // Setup display driver
   AtNode* driver = AiNodeLookUpByName(universe, str::materialview_display1);
   if (!driver)
   {
      driver = AiNode(universe, str::materialview_display, str::materialview_display1);
   }
   AiNodeSetPtr(driver, str::view, this);

   // Setup filter
   AtNode* filter = AiNodeLookUpByName(universe, str::materialview_filter1);
   if (!filter)
   {
      filter = AiNode(universe, str::gaussian_filter, str::materialview_filter1);
   }
   AiNodeSetFlt(filter, str::width, 2.0f);

   // Create the single output line. No AOVs or anything.
   AtArray* outputs  = AiArrayAllocate(1, 1, AI_TYPE_STRING);
   char str[1024];
   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   AiArraySetStr(outputs, 0, str);
   AiNodeSetArray(options, str::outputs, outputs);

   AiNodeSetInt(options, str::xres, m_width);
   AiNodeSetInt(options, str::yres, m_height);
   AiNodeSetInt(options, str::bucket_size, 32);
   AiNodeSetInt(options, str::threads, m_job.maxThreads);
   AiNodeSetBool(options, str::texture_automip, false);
   

   // displacement not correctly supported in material viewer yet
   // just ignoring it for now
   AiNodeSetBool(options, str::ignore_displacement, true);

   // Setup ray depth and sampling options
   // Default setting will be used if the options node
   // has not been created yet
   MSelectionList list;
   list.add("defaultArnoldRenderOptions");
   if (list.length() > 0)
   {
      MObject renderOptions;
      list.getDependNode(0, renderOptions);
      MFnDependencyNode fnArnoldRenderOptions(renderOptions);
      AiNodeSetInt(options, str::GI_diffuse_samples,     fnArnoldRenderOptions.findPlug(toMayaStyle("GI_diffuse_samples"), true).asInt());
      AiNodeSetInt(options, str::GI_specular_samples,    fnArnoldRenderOptions.findPlug(toMayaStyle("GI_specular_samples"), true).asInt());
      AiNodeSetInt(options, str::GI_transmission_samples,fnArnoldRenderOptions.findPlug(toMayaStyle("GI_transmission_samples"), true).asInt());
      AiNodeSetInt(options, str::GI_sss_samples,         fnArnoldRenderOptions.findPlug(toMayaStyle("GI_sss_samples"), true).asInt());
      AiNodeSetInt(options, str::GI_total_depth,         fnArnoldRenderOptions.findPlug(toMayaStyle("GI_total_depth"), true).asInt());
      AiNodeSetInt(options, str::GI_diffuse_depth,       fnArnoldRenderOptions.findPlug(toMayaStyle("GI_diffuse_depth"), true).asInt());
      AiNodeSetInt(options, str::GI_specular_depth,      fnArnoldRenderOptions.findPlug(toMayaStyle("GI_specular_depth"), true).asInt());
      AiNodeSetInt(options, str::GI_transmission_depth,  fnArnoldRenderOptions.findPlug(toMayaStyle("GI_transmission_depth"), true).asInt());

      MString texture_searchpath = fnArnoldRenderOptions.findPlug("texture_searchpath", true).asString();
      if (texture_searchpath.length() > 0)
         AiNodeSetStr(options, str::texture_searchpath, AtString(texture_searchpath.asChar()));

      COptionsTranslator::AddProjectFoldersToSearchPaths(options);

      // Check if we're rendering in GPU or CPU
      bool gpuRender = false;
      /* 
      ** Forcing Material View to always only use the CPU becase of a current limitation that prevents multiple GPU
      ** render sessions.
      */

      /* FIXME this code is duplicated from OptionsTranslator, we should rather handle the whole export from there
      if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(options), "render_device") != NULL)
      {
         MPlug gpuPlug = fnArnoldRenderOptions.findPlug("renderDevice", true);
         if (!gpuPlug.isNull())
            gpuRender = gpuPlug.asBool();

         AiNodeSetStr(options, "render_device", (gpuRender) ? "GPU" : "CPU");
      }*/
      if (gpuRender)
      {

         MString gpu_default_names = fnArnoldRenderOptions.findPlug("gpu_default_names", true).asString();
         AiNodeSetStr(options, str::gpu_default_names, AtString(gpu_default_names.asChar()));
         AiNodeSetInt(options, str::gpu_default_min_memory_MB,fnArnoldRenderOptions.findPlug("gpu_default_min_memory_MB", true).asInt());
         
         bool autoSelect = true;
         MPlug manualDevices = fnArnoldRenderOptions.findPlug("manual_gpu_devices", true);
         if (manualDevices.asBool())
         {  // Manual Device selection
            std::vector<unsigned int> devices;
            MPlug gpuDevices = fnArnoldRenderOptions.findPlug("render_devices", true);
            if (!gpuDevices.isNull())
            {
               unsigned int numElements = gpuDevices.numElements();
               for (unsigned int i = 0; i < numElements; ++i)
               {
                  MPlug elemPlug = gpuDevices[i];
                  if (!elemPlug.isNull())
                  {
                     devices.push_back(elemPlug.asInt());
                  }
               }
            }

            //
            if (!devices.empty())
            {
               autoSelect = false;
               AtArray* selectDevices = AiArrayConvert(devices.size(), 1, AI_TYPE_UINT, &devices[0]);
               AiDeviceSelect(m_renderSession, AI_DEVICE_TYPE_GPU, selectDevices);
               AiArrayDestroy(selectDevices);
            } 
         }

         if (autoSelect) // automatically select the GPU devices
            AiDeviceAutoSelect(m_renderSession);
      } 
      AiNodeSetInt(options, str::AA_samples, 5);
      //AiNodeSetBool(options, "enable_adaptive_sampling", true);
      //AiNodeSetBool(options, "enable_progressive_render", true);
      //AiNodeSetInt(options, "AA_samples_max", 6);

   }
   
   m_arnoldSession->ExportColorManager();
   
}

void CMaterialView::InterruptRender(bool waitFinished)
{
   {
      // Remove any scheduled refresh and 
      // set interrupt state
      std::lock_guard<AtMutex> guard(m_refreshLock);
      m_refreshAllowed = false;
      m_refreshEvent.unset();
      m_interrupted = true;
   }

   if (AiRenderGetStatus(m_renderSession) != AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderInterrupt(m_renderSession, (waitFinished) ? AI_BLOCKING : AI_NON_BLOCKING);
   }
}

void CMaterialView::ScheduleRefresh()
{
   std::lock_guard<AtMutex> guard(m_refreshLock);
   m_refreshAllowed = true;
   m_refreshEvent.set();
}

void CMaterialView::DoSuspend()
{
   std::lock_guard<AtMutex> guard(m_runningLock);
   if (!m_suspended)
   {
      m_suspended = true;
      MGlobal::executeCommand("renderer -materialViewRendererSuspend true");
   }
}

void CMaterialView::DoResume()
{
   std::lock_guard<AtMutex> guard(m_runningLock);
   if (m_suspended)
   {
      m_suspended = false;
      MGlobal::executeCommand("renderer -materialViewRendererSuspend false");
   }
}

void CMaterialView::DoAbort()
{
   std::lock_guard<AtMutex> guard(m_runningLock);
   if (IsActive())
   {
      if (isRunningAsync()) {
         stopAsync();
      }
      destroyScene();
   }
}

AtNode* CMaterialView::TranslateNode(const MUuid& id, const MObject& node, int updateMode, bool updateConnections)
{
   AtNode* arnoldNode = NULL;

   if (m_arnoldSession == nullptr)
      return nullptr;

   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it == m_translatorLookup.end())
   {
      CNodeTranslator* translator = m_arnoldSession->ExportNode(node);
      if (translator)
      {
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));         
         arnoldNode = translator->GetArnoldNode();
      }
      
      /*
      CNodeTranslator* translator = CExtensionsManager::GetTranslator(node);
      if (translator)
      {
         translator->m_impl->Init(arnoldSession, node);
         arnoldNode = translator->m_impl->DoExport();
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));
         m_deletables.push_back(translator);
      }*/
   }
   else
   {
      CNodeTranslator* translator = it->second;
      arnoldNode = UpdateNode(translator, updateMode, updateConnections);
   }

   if (arnoldNode)
   {
      const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(arnoldNode);
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] "+MFnDependencyNode(node).name()+" | Exported as "+MString(AiNodeGetName(arnoldNode))+"("+MString(AiNodeEntryGetTypeName(nodeEntry))+")");
   }
   else
   {
      AiMsgError("Failed to export node %s", MFnDependencyNode(node).name().asChar());
   }

   return arnoldNode;
}

AtNode* CMaterialView::TranslateDagNode(const MUuid& id, const MObject& node, int updateMode)
{
   AtNode* arnoldNode = NULL;
   if (m_arnoldSession == nullptr)
      return nullptr;
   
   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it == m_translatorLookup.end())
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(node, dagPath);
      dagPath.extendToShape();

      CDagTranslator *translator = m_arnoldSession->ExportDagPath(dagPath);
      if (translator)
      {
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));
         arnoldNode = translator->GetArnoldNode();
      }
   }
   else
   {
      CNodeTranslator* translator = it->second;
      arnoldNode = UpdateNode(translator, updateMode);
   }

   if (arnoldNode)
   {
      if (node.hasFn(MFn::kMesh))
      {
          AiNodeSetByte(arnoldNode, str::visibility, AI_RAY_ALL);
      }

      const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(arnoldNode);
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] "+MFnDagNode(node).fullPathName()+" | Exported as "+MString(AiNodeGetName(arnoldNode))+"("+MString(AiNodeEntryGetTypeName(nodeEntry))+")");
   }
   else
   {
      AiMsgError("Failed to export DAG node %s", MFnDagNode(node).fullPathName().asChar());
   }
   return arnoldNode;
}

AtNode* CMaterialView::UpdateNode(CNodeTranslator* translator, int updateMode, bool updateConnections)
{
   AtNode *node = nullptr;
   if (updateMode == MV_UPDATE_RECREATE)
   {
      translator->Delete();
      translator->m_impl->DoCreateArnoldNodes();
      node = translator->m_impl->DoExport();
   } else
      node = translator->m_impl->DoUpdate();

   if (updateConnections)
   {
      const std::vector<CNodeTranslator *> &references = translator->m_impl->m_references;
      for (auto childTr : references)
      {
         UpdateNode(childTr, updateMode, true);
      }
   }
   return node;
}

void CMaterialView::SendBucketToView(unsigned int left, unsigned int right, unsigned int bottom, unsigned int top, void* data)
{
   MPxRenderer::RefreshParams rp;
   rp.width = m_width;
   rp.height = m_height;
   rp.left = left;
   rp.right = right;
   rp.bottom = m_height - top - 1; // flip vertically
   rp.top = m_height - bottom - 1; // 
   rp.channels = 4;
   rp.bytesPerChannel = 4;
   rp.data = data;

   refresh(rp);
}

void CMaterialView::SendProgress(float progress)
{
   MPxRenderer::ProgressParams params;
   params.progress = progress;
   this->progress(params);
}

void* CMaterialView::Creator()
{
   s_instance = new CMaterialView();
   return (void*)s_instance;
}

const MString& CMaterialView::Name()
{
   static const MString s_name("Arnold");
   return s_name;
}

void CMaterialView::Suspend()
{   
   if (s_instance)
   {
      s_instance->DoSuspend();
   }
}

void CMaterialView::Resume()
{
   if (s_instance)
   {
      s_instance->DoResume();
   }
}

void CMaterialView::Abort()
{
   if (s_instance)
   {
      s_instance->DoAbort();
   }
}

void CMaterialView::End()
{
   if (s_instance)
   {
      s_instance->EndSession();
   }
}
