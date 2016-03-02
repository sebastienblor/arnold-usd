#pragma once

#include <maya/MPxRenderer.h> 

#include "render/RenderSession.h"
#include "translators/NodeTranslator.h"
#include "platform/Platform.h"

/** Material View
 * 
 * Class controlling the rendering into Maya's material viewer.
 * 
 */
class DLLEXPORT CMaterialView
   :  public MPxRenderer
{
public:
   CMaterialView();
   virtual  ~CMaterialView();

   // Maya's MPxRenderer interface
   virtual MStatus startAsync(const JobParams& params);
   virtual MStatus stopAsync();
   virtual bool isRunningAsync();
   virtual MStatus beginSceneUpdate();
   virtual MStatus translateMesh(const MUuid& id, const MObject& node);
   virtual MStatus translateLightSource(const MUuid& id, const MObject& node);
   virtual MStatus translateCamera(const MUuid& id, const MObject& node);
   virtual MStatus translateEnvironment(const MUuid& id, EnvironmentType type);
   virtual MStatus translateTransform(const MUuid& id, const MUuid& childId, const MMatrix& matrix);
   virtual MStatus translateShader(const MUuid& id, const MObject& node);
   virtual MStatus setProperty(const MUuid& id, const MString& name, bool value);
   virtual MStatus setProperty(const MUuid& id, const MString& name, int value);
   virtual MStatus setProperty(const MUuid& id, const MString& name, float value);
   virtual MStatus setProperty(const MUuid& id, const MString& name, const MString& value);
   virtual MStatus setShader(const MUuid& id, const MUuid& shaderId);
   virtual MStatus setResolution(unsigned int width, unsigned int height);
   virtual MStatus endSceneUpdate();
   virtual MStatus destroyScene();
   virtual bool isSafeToUnload();

   void SendBucketToView(unsigned int left, unsigned int right, unsigned int bottom, unsigned int top, void* data);
   void SendProgress(float progress);

   static void* Creator();
   static const MString& Name();
   static void SuspendRenderer();
   static void ResumeRenderer();
   static void AbortSession();

   class ScopedSuspend {
   public:
      ScopedSuspend() { CMaterialView::SuspendRenderer(); }
      ~ScopedSuspend() { CMaterialView::ResumeRenderer(); }
   };

private:
   bool BeginSession();
   void EndSession();
   bool IsActive();
   void InterruptRender(bool waitFinished = false);
   bool WaitForRefresh(unsigned int msTimeout);
   void ScheduleRefresh();

   AtNode* TranslateNode(const MUuid& id, const MObject& node, int updateMode = AI_UPDATE_ONLY);
   AtNode* TranslateDagShape(const MUuid& id, const MObject& node, int updateMode = AI_UPDATE_ONLY);
   AtNode* UpdateNode(CNodeTranslator* translator, int updateMode = AI_UPDATE_ONLY);

   static unsigned int RenderThread(void* data);

   struct Uuid
   {
      uint8_t m_data[16];
      Uuid(const MUuid& id) { id.get(m_data); }
      bool operator<(const Uuid& rhs) const { return memcmp(m_data, rhs.m_data, 16) < 0; }
   };

   typedef std::map<Uuid, CNodeTranslator*> TranslatorLookup;
   typedef std::vector<CNodeTranslator*>    TranslatorVector;

   TranslatorVector m_deletables;
   TranslatorLookup m_translatorLookup;
   CNodeTranslator* m_shaderTranslator;
   AtNode*          m_environmentShader;
   AtNode*          m_environmentImage;

   CCritSec      m_sceneLock;
   CCritSec      m_runningLock;
   void*         m_renderThread;
   volatile bool m_isRunning;
   volatile bool m_terminationRequested;
   CEvent        m_refreshEvent;
   unsigned int  m_width;
   unsigned int  m_height;

   static CMaterialView* s_instance;
};
