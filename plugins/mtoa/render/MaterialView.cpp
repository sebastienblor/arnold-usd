#include "MaterialView.h"

#include "platform/Platform.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"
#include "render/RenderSession.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/options/OptionsTranslator.h"
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

namespace 
{

void SleepMS(unsigned int ms)
{
#ifdef _WIN64
   Sleep(ms);
#else
   usleep(ms*1000);
#endif
}

}

extern const AtNodeMethods* materialview_driver_mtd;
CMaterialView* CMaterialView::s_instance = NULL;

CMaterialView::CMaterialView()
: m_activeShader(NULL)
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
}

AtRenderStatus MaterialViewUpdateCallback(void *private_data, AtRenderUpdateType update_type, const AtRenderUpdateInfo *update_info)
{
   CMaterialView *material_view = (CMaterialView *)private_data;

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
   if (!IsActive())
      return MStatus::kFailure;

   CCritSec::CScopedLock sc(m_runningLock);

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
   
   InitOptions();
    
   if (AiRenderGetStatus() != AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderInterrupt(AI_BLOCKING);
      AiRenderEnd();
   }

   AiRenderSetHintBool(AtString("progressive"), true);
   AiRenderSetHintBool(AtString("progressive_show_all_outputs"), false);
   AiRenderSetHintInt(AtString("progressive_min_AA_samples"), -3);

   AiRenderBegin(AI_RENDER_MODE_CAMERA, MaterialViewUpdateCallback, (void*)this);

   ScheduleRefresh();

   return MStatus::kSuccess;
}

MStatus CMaterialView::stopAsync()
{
   CCritSec::CScopedLock sc(m_runningLock);

   m_terminationRequested = true;
   InterruptRender(true);

   AiRenderInterrupt(AI_BLOCKING);
   AiRenderEnd();

   m_running = false;

   return MStatus::kSuccess;
}

bool CMaterialView::isRunningAsync()
{
   CCritSec::CScopedLock sc(m_runningLock);
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
   return BeginSession() ? MStatus::kSuccess : MStatus::kFailure;
}


MStatus CMaterialView::translateMesh(const MUuid& id, const MObject& node)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

   // Make sure the renderer is stopped
   InterruptRender(true);

   AtNode* geometryNode = TranslateDagNode(id, node, MV_UPDATE_RECREATE);
   if (!geometryNode) 
   {
      return MStatus::kFailure;
   }

   if (m_activeShader)
   {
      AiNodeSetPtr(geometryNode, "shader", m_activeShader);
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateLightSource(const MUuid& id, const MObject& node)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

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
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

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
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

   // Make sure the renderer is stopped
   InterruptRender(true);

   if (!m_environmentLight)
   {
       m_environmentLight = AiNode("skydome_light");
       AiNodeSetStr(m_environmentLight, "name", "mtrlViewSkyDome");
       AiNodeSetInt(m_environmentLight, "format", 2);
       AiNodeSetRGB(m_environmentLight, "color", 0.1f, 0.1f, 0.1f);
 
       AtMatrix rotation = AiM4RotationY(180.0f);
       AiNodeSetMatrix(m_environmentLight, "matrix", rotation);
   }
   if (!m_environmentImage)
   {
       m_environmentImage = AiNode("image");
       AiNodeSetStr(m_environmentImage, "name", "mtrlViewSkyDomeImage");
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateTransform(const MUuid& id, const MUuid& childId, const MMatrix& mayaMatrix)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

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
      AiNodeSetMatrix(arnoldNode, "matrix", matrix);
   }
   else
   {
      return MStatus::kFailure;
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::translateShader(const MUuid& id, const MObject& node)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

   // Make sure the renderer is stopped 
   if (AiRenderGetStatus() == AI_RENDER_STATUS_RENDERING)
   {
      AiRenderInterrupt(AI_BLOCKING);
   }
//   InterruptRender(true);

   if (!TranslateNode(id, node))
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
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;

   if (m_environmentLight && m_environmentImage)
   {
      static const MString s_imageFileStr("imageFile");
      if (name == s_imageFileStr)
      {
         // Make sure the renderer is stopped
         InterruptRender(true);

         AiNodeSetStr(m_environmentImage, "filename", value.asChar());
         if (value.length())
         {
            AiNodeLink(m_environmentImage, "color", m_environmentLight);
         }
         else
         {
            AiNodeUnlink(m_environmentLight, "color");
         }
      }
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::setShader(const MUuid& id, const MUuid& shaderId)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!IsActive())
      return MStatus::kFailure;
   
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

   AiNodeSetPtr(geometryNode, "shader", shaderNode);
   m_activeShader = shaderNode;

   return MStatus::kSuccess;
}

MStatus CMaterialView::setResolution(unsigned int width, unsigned int height)
{
   CCritSec::CScopedLock sc(m_sceneLock);

   m_width = width;
   m_height = height;

   if (IsActive())
   {
      // Resolution updates are not wrapped in begin/end calls
      // so we need to interrupt and schedule refresh explicitly here

      InterruptRender();

      AtNode* options = AiUniverseGetOptions();
      AiNodeSetInt(options, "xres", m_width);
      AiNodeSetInt(options, "yres", m_height);

      ScheduleRefresh();
   }

   return MStatus::kSuccess;
}

MStatus CMaterialView::endSceneUpdate()
{
   if (AiRenderGetStatus() == AI_RENDER_STATUS_NOT_STARTED)
      AiRenderBegin(AI_RENDER_MODE_CAMERA, MaterialViewUpdateCallback, (void*)this);
   else
      AiRenderRestart();
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
   CCritSec::CScopedLock sc(m_sceneLock);

   if (m_active)
   {
      // We are already active. We should never get here.
      return true;
   }

   // Make sure no other session is active. In that case we're not allowed to start.
   // Except for swatch rendering which has a lower priority and will be disabled below.
   if(CMayaScene::IsActive(MTOA_SESSION_ANY) && !CMayaScene::IsActive(MTOA_SESSION_SWATCH))
   {
      return false;
   }

   // We have higher priority than swatch rendering, so disable it.
   // This will also cancell any active swatch rendering.
   MSwatchRenderBase::enableSwatchRender(false);

   // Begin a scene session in material view mode
   CMayaScene::Begin(MTOA_SESSION_MATERIALVIEW);

   // Install our driver
   AiNodeEntryInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "materialview_display", "mtoa", (AtNodeMethods*) materialview_driver_mtd, AI_VERSION);

   m_dummyShader = AiNode("utility");
   AiNodeSetStr(m_dummyShader, "name", "mtrlViewDummyShader");
   AiNodeSetRGB(m_dummyShader, "color", 0.0f, 0.0f, 0.0f);

   m_active = true;

   return true;
}

void CMaterialView::EndSession()
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!m_active)
      return;
   
   // Make sure we are not rendering
   InterruptRender(true);

   // Cleanup
   for (TranslatorVector::iterator it = m_deletables.begin(); it != m_deletables.end(); ++it)
   {
      delete *it;
   }
   m_deletables.clear();
   m_translatorLookup.clear();
   m_activeShader      = NULL;
   m_dummyShader       = NULL;
   m_environmentLight  = NULL;
   m_environmentImage  = NULL;

   // Note: We must set the active flag before calling CMayaScene::End() below
   // to prevent reqursive entry since End() checks for active mtrl view session
   m_active = false;

   // Uninstall our driver
   AiNodeEntryUninstall("materialview_display");

   // End our scene session
   CMayaScene::End();

   // Notify scene is destroyed
   SendProgress(-1);

   // Re-enable swatches
   MSwatchRenderBase::enableSwatchRender(true);
}

bool CMaterialView::IsActive()
{
   CCritSec::CScopedLock sc(m_sceneLock);
   return m_active;
}

void CMaterialView::InitOptions()
{
   CCritSec::CScopedLock sc(m_sceneLock);

   AtNode* options = AiUniverseGetOptions();

   // Set render camera
   TranslatorLookup::iterator it = m_translatorLookup.find(m_job.cameraId);
   if (it != m_translatorLookup.end())
   {
      AtNode* camera = it->second->GetArnoldNode();
      AiNodeSetPtr(options, "camera", camera);
   }
   else
   {
      AiMsgError("Render camera not found!");
      assert(false); // Should never happen
   }

   // Setup display driver
   AtNode* driver = AiNodeLookUpByName("materialview_display1");
   if (!driver)
   {
      driver = AiNode("materialview_display");
      AiNodeSetStr(driver, "name", "materialview_display1");
   }
   AiNodeSetPtr(driver, "view", this);

   // Setup filter
   AtNode* filter = AiNodeLookUpByName("materialview_filter1");
   if (!filter)
   {
      filter = AiNode("gaussian_filter");
      AiNodeSetStr(filter, "name", "materialview_filter1");
   }
   AiNodeSetFlt(filter, "width", 2.0f);

   // Create the single output line. No AOVs or anything.
   AtArray* outputs  = AiArrayAllocate(1, 1, AI_TYPE_STRING);
   char str[1024];
   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   AiArraySetStr(outputs, 0, str);
   AiNodeSetArray(options, "outputs", outputs);

   AiNodeSetInt(options, "xres", m_width);
   AiNodeSetInt(options, "yres", m_height);
   AiNodeSetInt(options, "bucket_size", 32);
   AiNodeSetStr(options, "pin_threads", "off");
   AiNodeSetInt(options, "threads", m_job.maxThreads);

   // displacement not correctly supported in material viewer yet
   // just ignoring it for now
   AiNodeSetBool(options, "ignore_displacement", true);

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
      AiNodeSetInt(options, "GI_diffuse_samples",     fnArnoldRenderOptions.findPlug(toMayaStyle("GI_diffuse_samples"), true).asInt());
      AiNodeSetInt(options, "GI_specular_samples",    fnArnoldRenderOptions.findPlug(toMayaStyle("GI_specular_samples"), true).asInt());
      AiNodeSetInt(options, "GI_transmission_samples",fnArnoldRenderOptions.findPlug(toMayaStyle("GI_transmission_samples"), true).asInt());
      AiNodeSetInt(options, "GI_sss_samples",         fnArnoldRenderOptions.findPlug(toMayaStyle("GI_sss_samples"), true).asInt());
      AiNodeSetInt(options, "GI_total_depth",         fnArnoldRenderOptions.findPlug(toMayaStyle("GI_total_depth"), true).asInt());
      AiNodeSetInt(options, "GI_diffuse_depth",       fnArnoldRenderOptions.findPlug(toMayaStyle("GI_diffuse_depth"), true).asInt());
      AiNodeSetInt(options, "GI_specular_depth",      fnArnoldRenderOptions.findPlug(toMayaStyle("GI_specular_depth"), true).asInt());
      AiNodeSetInt(options, "GI_transmission_depth",  fnArnoldRenderOptions.findPlug(toMayaStyle("GI_transmission_depth"), true).asInt());


      // Check if we're rendering in GPU or CPU
      bool gpuRender = false;
      // FIXME this code is duplicated from OptionsTranslator, we should rather handle the whole export from there
      if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(options), "render_device") != NULL)
      {
         MPlug gpuPlug = fnArnoldRenderOptions.findPlug("renderDevice");
         if (!gpuPlug.isNull())
            gpuRender = gpuPlug.asBool();

         AiNodeSetStr(options, "render_device", (gpuRender) ? "GPU" : "CPU");
      }
      if (gpuRender)
      {

         MString gpu_default_names = fnArnoldRenderOptions.findPlug("gpu_default_names").asString();
         AiNodeSetStr(options, "gpu_default_names", AtString(gpu_default_names.asChar()));
         AiNodeSetInt(options, "gpu_default_min_memory_MB",fnArnoldRenderOptions.findPlug("gpu_default_min_memory_MB").asInt());
         
         bool autoSelect = true;
         MPlug manualDevices = fnArnoldRenderOptions.findPlug("manual_gpu_devices");
         if (manualDevices.asBool())
         {  // Manual Device selection
            std::vector<unsigned int> devices;
            MPlug gpuDevices = fnArnoldRenderOptions.findPlug("render_devices");
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
               AiDeviceSelect(AI_DEVICE_TYPE_GPU, selectDevices);
               AiArrayDestroy(selectDevices);
            } 
         }

         if (autoSelect) // automatically select the GPU devices
            AiDeviceAutoSelect();
      } 
      AiNodeSetInt(options, "AA_samples", 5);
      //AiNodeSetBool(options, "enable_adaptive_sampling", true);
      //AiNodeSetBool(options, "enable_progressive_render", true);
      //AiNodeSetInt(options, "AA_samples_max", 6);

   }
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   if (arnoldSession)
      arnoldSession->ExportColorManager();

   
}

void CMaterialView::InterruptRender(bool waitFinished)
{
   {
      // Remove any scheduled refresh and 
      // set interrupt state
      CCritSec::CScopedLock sc(m_refreshLock);
      m_refreshAllowed = false;
      m_refreshEvent.unset();
      m_interrupted = true;
   }

   if (AiRenderGetStatus() != AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderInterrupt((waitFinished) ? AI_BLOCKING : AI_NON_BLOCKING);
   }
}

void CMaterialView::ScheduleRefresh()
{
   CCritSec::CScopedLock sc(m_refreshLock);
   m_refreshAllowed = true;
   m_refreshEvent.set();
}

void CMaterialView::DoSuspend()
{
   CCritSec::CScopedLock sc(m_runningLock);
   if (!m_suspended)
   {
      m_suspended = true;
      CMayaScene::ExecuteScript("renderer -materialViewRendererSuspend true");
   }
}

void CMaterialView::DoResume()
{
   CCritSec::CScopedLock sc(m_runningLock);
   if (m_suspended)
   {
      m_suspended = false;
      CMayaScene::ExecuteScript("renderer -materialViewRendererSuspend false");
   }
}

void CMaterialView::DoAbort()
{
   CCritSec::CScopedLock sc(m_runningLock);
   if (IsActive())
   {
      if (isRunningAsync()) {
         stopAsync();
      }
      destroyScene();
   }
}

AtNode* CMaterialView::TranslateNode(const MUuid& id, const MObject& node, int updateMode)
{
   AtNode* arnoldNode = NULL;

   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();

   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it == m_translatorLookup.end())
   {
      CNodeTranslator* translator = CExtensionsManager::GetTranslator(node);
      if (translator)
      {
         translator->m_impl->Init(arnoldSession, node);
         arnoldNode = translator->m_impl->DoExport();
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));
         m_deletables.push_back(translator);
      }
   }
   else
   {
      CNodeTranslator* translator = it->second;
      arnoldNode = UpdateNode(translator, updateMode);
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

   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();

   TranslatorLookup::iterator it = m_translatorLookup.find(id);
   if (it == m_translatorLookup.end())
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(node, dagPath);
      dagPath.extendToShape();

      CDagTranslator* translator = CExtensionsManager::GetTranslator(dagPath);
      if (translator)
      {
         translator->m_impl->Init(arnoldSession, dagPath);
         arnoldNode = translator->m_impl->DoExport();
         m_translatorLookup.insert(TranslatorLookup::value_type(id,translator));
         m_deletables.push_back(translator);
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
          AiNodeSetByte(arnoldNode, "visibility", AI_RAY_ALL);
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

AtNode* CMaterialView::UpdateNode(CNodeTranslator* translator, int updateMode)
{
   if (updateMode == MV_UPDATE_RECREATE)
   {
      translator->Delete();
      translator->m_impl->DoCreateArnoldNodes();
      return translator->m_impl->DoExport();
   }
   return translator->m_impl->DoUpdate();
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