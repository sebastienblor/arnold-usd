#include "MaterialView.h"

#ifdef ENABLE_MATERIAL_VIEW

#include "platform/Platform.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"
#include "render/RenderSession.h"
#include "translators/DagTranslator.h"
#include "translators/options/OptionsTranslator.h"
#include "extension/ExtensionsManager.h"

#include <maya/MSwatchRenderBase.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MAngle.h>
#include <assert.h>
#ifdef _LINUX
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

extern AtNodeMethods* materialview_driver_mtd;
CMaterialView* CMaterialView::s_instance = NULL;

CMaterialView::CMaterialView()
: m_activeShader(NULL)
, m_dummyShader(NULL)
, m_environmentShader(NULL)
, m_environmentImage(NULL)
, m_renderThread(NULL)
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
   m_renderThread = AiThreadCreate(CMaterialView::RenderThread, this, AI_PRIORITY_LOW);

   ScheduleRefresh();

   return MStatus::kSuccess;
}

MStatus CMaterialView::stopAsync()
{
   CCritSec::CScopedLock sc(m_runningLock);

   m_terminationRequested = true;
   InterruptRender(true);

   // Wait for the thread to finish
   if (m_renderThread)
   {
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);
      m_renderThread = 0;
   }

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

   AiNodeSetBool(geometryNode, "opaque", false);

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

   if (!m_environmentShader)
   {
      // TODO: Use skydome light for more efficient sampling, and combine
      //       with sky shader for reflections and background.

      m_environmentShader = AiNode("sky");
      AiNodeSetStr(m_environmentShader, "name", "mtrlViewSky");
      AiNodeSetInt(m_environmentShader, "format", 2);
      AiNodeSetRGB(m_environmentShader, "color", 0.1f, 0.1f, 0.1f);

      // Invert in Z to account for the env sphere being viewed from inside
      AiNodeSetVec(m_environmentShader, "X", 1.0f, 0.0f, 0.0f);
      AiNodeSetVec(m_environmentShader, "Y", 0.0f, 1.0f, 0.0f);
      AiNodeSetVec(m_environmentShader, "Z", 0.0f, 0.0f, -1.0f);

      if (!m_environmentImage)
      {
         m_environmentImage = AiNode("image");
         AiNodeSetStr(m_environmentImage, "name", "mtrlViewSkyImage");
      }
   }

   AtNode* options = AiUniverseGetOptions();
   AiNodeSetPtr(options, "background", m_environmentShader);

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
      translator->ConvertMatrix(matrix, mayaMatrix, CMayaScene::GetArnoldSession());

      // Make sure the renderer is stopped
      InterruptRender(true);

      AtNode* arnoldNode = translator->GetArnoldRootNode();
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
   InterruptRender(true);

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

   if (m_environmentShader && m_environmentImage)
   {
      static const MString s_imageFileStr("imageFile");
      if (name == s_imageFileStr)
      {
         // Make sure the renderer is stopped
         InterruptRender(true);

         AiNodeSetStr(m_environmentImage, "filename", value.asChar());
         if (value.length())
         {
            AiNodeLink(m_environmentImage, "color", m_environmentShader);
         }
         else
         {
            AiNodeUnlink(m_environmentShader, "color");
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
   AtNode* geometryNode = geometryTranslator->GetArnoldRootNode();

   AtNode* shaderNode = NULL;

   it = m_translatorLookup.find(shaderId);
   if (it != m_translatorLookup.end())
   {
      // Shader found among our translatated shaders
      CNodeTranslator* shaderTranslator = it->second;
      shaderNode = shaderTranslator->GetArnoldRootNode();
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
   ScheduleRefresh();

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
      AiMsgDebug("[mtoa] Material View: Session already active!");
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

   m_dummyShader = AiNode("MayaSurfaceShader");
   AiNodeSetStr(m_dummyShader, "name", "mtrlViewDummyShader");
   AiNodeSetRGB(m_dummyShader, "outColor", 0.0f, 0.0f, 0.0f);

   m_active = true;

   return true;
}

void CMaterialView::EndSession()
{
   CCritSec::CScopedLock sc(m_sceneLock);

   if (!m_active)
   {
      AiMsgDebug("[mtoa] Material View: Session already ended!");
      return;
   }

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
   m_environmentShader = NULL;
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
      AtNode* camera = it->second->GetArnoldRootNode();
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
   AiNodeSetInt(options, "GI_sss_samples", 4);
   AiNodeSetInt(options, "GI_diffuse_depth", 1);
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

   if (AiRendering())
   {
      AiRenderInterrupt();
      if (waitFinished)
      {
         while(AiRendering())
         {
            SleepMS(1);
         }
      }
   }
}

bool CMaterialView::WaitForRefresh(unsigned int msTimeout)
{
   const bool result = m_refreshEvent.wait(msTimeout);

   CCritSec::CScopedLock sc(m_refreshLock);
   if (result && m_refreshAllowed)
   {
      // A refresh has been requested.
      // Clear interrupt state and refresh event,
      // then return true to trigger the refresh.
      m_interrupted = false;
      m_refreshEvent.unset();
      return true;
   }
   // No refresh requested yet
   return false;
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
         translator->Init(arnoldSession, node);
         arnoldNode = translator->DoExport(0);
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
      AiMsgDebug("[mtoa] %-30s | Exported as %s(%s)",  MFnDependencyNode(node).name().asChar(), AiNodeGetName(arnoldNode), AiNodeEntryGetTypeName(nodeEntry));
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
         translator->Init(arnoldSession, dagPath);
         arnoldNode = translator->DoExport(0);
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
      AtByte visibility = AI_RAY_ALL;
      AiNodeSetByte(arnoldNode, "visibility", visibility);

      const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(arnoldNode);
      AiMsgDebug("[mtoa] %-30s | Exported as %s(%s)",  MFnDagNode(node).fullPathName().asChar(), AiNodeGetName(arnoldNode), AiNodeEntryGetTypeName(nodeEntry));
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
      translator->DoCreateArnoldNodes();
      return translator->DoExport(0);
   }
   return translator->DoUpdate(0);
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

unsigned int CMaterialView::RenderThread(void* data)
{
   CMaterialView* view = static_cast<CMaterialView*>(data);
   AtNode* options = AiUniverseGetOptions();

   const int oldSampleRate = AiNodeGetInt(options, "AA_samples");
   const int numIterations = 6;
   const int sampleRate[numIterations] = {-3,-1, 1, 3, 6, 10};

   // Initialize options for rendering
   view->InitOptions();

   int status = AI_SUCCESS;
   while (view->m_terminationRequested == false)
   {
      if (view->WaitForRefresh(50))
      {
         // Notify rendering is in progress
         view->SendProgress(0);

         status = AI_SUCCESS;
         for (int i = 0; i < numIterations; ++i)
         {
            AiNodeSetInt(options, "AA_samples", sampleRate[i]);

            AiMsgInfo("[mtoa] Beginning progressive sampling at %d AA of %d AA", sampleRate[i], sampleRate[numIterations-1]);

            // Start the render if not interrupted already
            status = view->m_interrupted ? AI_INTERRUPT : AiRender(AI_RENDER_MODE_CAMERA);
            if (status != AI_SUCCESS)
            {
               break;
            }
         }
         if (status == AI_SUCCESS)
         {
            // Notify rendering completed and thread idle
            view->SendProgress(1);
         }
      }
   }

   // Restore sample rate
   AiNodeSetInt(options, "AA_samples", oldSampleRate);

   return status;
}

void* CMaterialView::Creator()
{
   assert(s_instance == NULL);
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

#endif // ENABLE_MATERIAL_VIEW
