#pragma once

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "render/AOV.h"
#include "SessionOptions.h"
#include "ArnoldLightLinks.h"
#include "common/AiStream.h"
#include "translators/NodeTranslator.h"
#include "translators/options/OptionsTranslator.h"
#include "translators/DagTranslator.h"
#include <ai_nodes.h>

#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>
#include <maya/MObjectArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnCamera.h>
#include <maya/MVectorArray.h>
#include <maya/MMessage.h>
#include <maya/MPlug.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MDagPathArray.h>

#include <vector>
#include <set>
#include <map>
#include <string>

class DLLEXPORT CArnoldSession
{
   friend class CSessionManager;

public:
   typedef std::pair<CNodeAttrHandle, CNodeTranslator*> ObjectToTranslatorPair;
   typedef unordered_map<std::string, CNodeTranslator*> ObjectToTranslatorMap;
   typedef std::pair<CNodeAttrHandle, MCallbackId> HiddenObjectCallbackPair;

   // Map dag object handles to primary instance
   typedef unordered_map<MObjectHandle, MDagPath> ObjectHandleToDagMap;

   CArnoldSession(bool initOptions = true, bool defaultUniverse = false);
   virtual ~CArnoldSession();

   // return the list of renderable cameras in the scene
   static MDagPathArray GetRenderCameras(bool activeView = false);

   const MString &GetSessionName() const {return m_sessionName;}

   virtual void Export(MSelectionList* selected = NULL);
   virtual void Update();

   void InitSessionOptions();
   virtual void UpdateSessionOptions();

   virtual void ChangeCurrentFrame(MTime time, bool forceViewport);

   AtUniverse *GetUniverse() {return m_universe;}
   CSessionOptions &GetOptions() {return m_sessionOptions;}

   virtual bool IsFileExport() const {return false;}
   bool IsBatchSession() const {return m_batch;}
   void SetBatch(bool b) {m_batch = b;}
   bool IsInteractiveSession() const {return m_updateCallbacks;}

   virtual void Clear();

   CDagTranslator* ExportDagPath(const MDagPath &dagPath, bool initOnly=false, MStatus* stat=NULL);
   CNodeTranslator* ExportNode(const MPlug& shaderOutputPlug,
                      bool initOnly=false, int instanceNumber = -1, MStatus* stat=NULL);
   CNodeTranslator* ExportNode(const MObject& node,
                      bool initOnly=false, int instanceNumber = -1, MStatus* stat=NULL);

   void InterruptRender();
   bool IsRendering();

   enum DagFiltered
   {
      MTOA_EXPORT_ACCEPTED,
      MTOA_EXPORT_REJECTED_NODE,
      MTOA_EXPORT_REJECTED_BRANCH
   };
   
   DagFiltered IsExportable(const MDagPath &dagPath) const; 
   
   void QueueForUpdate(CNodeTranslator * translator);
   void QueueForUpdate(const CNodeAttrHandle & handle);
   virtual void RequestUpdate();
   void RequestUpdateTx(const std::string &filename, const std::string &colorSpace);
   void RequestUpdateOptions() {m_updateOptions = true; RequestUpdate();}
   void RequestUpdateMotion() {m_updateMotion = true;}
   void RequestUpdateLightLinks() {m_updateLightLinks = true;}
   virtual void RequestUpdateImagers(bool listChanged);

   bool IsExportingMotion() const {return m_isExportingMotion;}
   unsigned int GetMotionStep() const {return m_motionStep;}

   CNodeTranslator *GetActiveTranslator(const CNodeAttrHandle &handle);
   void EraseActiveTranslator(CNodeTranslator *translator);
   
   void AddUpdateCallbacks();
   void ClearUpdateCallbacks();
   
   MString GetMayaObjectName(const AtNode *node) const;
   void SetExportCamera(MDagPath camera, bool updateRender = true);

   virtual AtNode* ExportOptions();
   virtual MStatus ExportCameras(MSelectionList* selected = NULL);
   virtual MStatus ExportLights(MSelectionList* selected = NULL);
   virtual MStatus ExportDag(MSelectionList* selected = NULL);
   virtual void ExportTxFiles();
   virtual void SetStatus(MString status);

   inline void AddMasterInstanceHandle(MObjectHandle handle, MDagPath dagPath){m_masterInstances[handle] = dagPath;};
   inline MDagPath GetMasterInstanceDagPath(MObjectHandle handle){return m_masterInstances[handle];};

   virtual void ObjectNameChanged(MObject &node, const MString &str) {};

   // FIXME ideally the translator would handle the export of its own light linking,
   // it wouldn't ask the session to do it for him
   void ExportLightLinking(AtNode *shape, const MDagPath &path);
   
   static MObject GetDefaultArnoldRenderOptions();
   void SetCheckVisibility(bool b) {m_checkVisibility = b;}
   // Get a render session for this arnold session. Note that for the renderview it can be owned by ARV, so it shouldn't be cleared
   virtual AtRenderSession *GetRenderSession();
   COptionsTranslator* GetOptionsTranslator() {return m_optionsTranslator;}
   AtNode *ExportColorManager();


protected:
      
   void SetSessionName(const MString &name) {m_sessionName = name;}
   static void NewNodeCallback(MObject & node, void *);
   static void ParentingChangedCallback(MDagPath &child, MDagPath &parent, void *);
   void RecursiveUpdateDagChildren(MDagPath &parent);
   
   static void HiddenNodeCallback(MObject& node, MPlug& plug, void* clientData);
   static void DoHiddenCallback(void* clientData);
   void SetDagVisible(MDagPath &path);

   void UpdateLightLinks();
   virtual void NewNode(MObject &node);
   
   AtUniverse *m_universe;
   AtRenderSession *m_renderSession;
   ObjectToTranslatorMap m_translators;
   CSessionOptions m_sessionOptions;
   COptionsTranslator*  m_optionsTranslator;
   CArnoldLightLinks m_lightLinks;
   MString m_sessionName;
   int m_motionStep;
   
   bool m_isExportingMotion;
   bool m_updateOptions;
   bool m_updateMotion;
   bool m_updateTx;
   bool m_updateLightLinks;
   bool m_checkVisibility; // FIXME for material viewer, turn it off
   bool m_checkRenderLayer;
   
   bool m_requestUpdate;
   bool m_updateCallbacks;
   bool m_batch;
   std::vector<ObjectToTranslatorPair> m_objectsToUpdate;
   std::unordered_map<std::string, std::string> m_updateTxFiles;

   MCallbackId m_newNodeCallbackId;
   MCallbackId m_addParentCallbackId;
   MCallbackId m_removeParentCallbackId;
   MCallbackId m_hiddenNodeCb;
   MDagPathArray m_hiddenNodesArray;
   std::vector<HiddenObjectCallbackPair> m_hiddenObjectsCallbacks;

   ObjectHandleToDagMap m_masterInstances;


};  // class CArnoldSession
