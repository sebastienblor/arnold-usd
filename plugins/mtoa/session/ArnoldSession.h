#pragma once

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "render/AOV.h"
#include "SessionOptions.h"
#include "ArnoldLightLinks.h"
#include "common/AiStream.h"
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
#include <maya/MLightLinks.h>
#include <maya/MCallbackIdArray.h>

#include <vector>
#include <set>
#include <map>
#include <string>

// To allow to specify that a dag node gets filtered out,
// or that it's whole hierarchy does
enum DagFiltered
{
   MTOA_EXPORT_ACCEPTED,
   MTOA_EXPORT_REJECTED_NODE,
   MTOA_EXPORT_REJECTED_BRANCH
};

class CNodeTranslator;
class CDagTranslator;
class COptionsTranslator;

// Map translated Maya objects (node, dag node with instance number, or plug) to
// their processed translator.
// It's a multimap as there can be one different Arnold node and translator per output attribute
// for Maya node with multiple outputs (until Arnold nodes support multiple outputs)
// (Trac #351)
// FIXME: careful that multiple occurrence of same translator will cause a crash when clearing map and deleting them!
typedef unordered_map<std::string, CNodeTranslator*> ObjectToTranslatorMap;
typedef std::pair<CNodeAttrHandle, CNodeTranslator*> ObjectToTranslatorPair;
typedef std::pair<CNodeAttrHandle, MCallbackId> HiddenObjectCallbackPair;

// Map dag object handles to master instance
typedef unordered_map<MObjectHandle, MDagPath> ObjectHandleToDagMap;

/// Opens an Arnold session, in which you can make changes to the Arnold universe: create or edit Arnold nodes.

typedef unordered_set<AtNode*> AtNodeSet;

/// This class handles exporting all or part of the Maya scene to Arnold, for rendering, exporting as an ass
/// file or any other use. It has methods to export individual objects or whole scenes, and tracks the list
/// of translators created for each Maya node export.
///
/// Once CArnoldSession::Export() is called, the DAG hierarchy is traversed and CDagTranslators
/// are found and exported for all relevant Maya nodes.  Those translators in turn call
/// and CArnoldSession::ExportShader() as they require, which triggers dependency graph evaluation and the
/// generation of CNodeTranslators.
///
/// In IPR mode, the racked translators allows the scene to be quickly and incrementally retranslated
/// as changes occur to previously translated Maya objects.
class DLLEXPORT CArnoldSession
{
   friend class CMayaScene;

public:

   // Called by translators
   CDagTranslator* ExportDagPath(const MDagPath &dagPath, bool initOnly=false, MStatus* stat=NULL);
   CNodeTranslator* ExportNode(const MPlug& shaderOutputPlug,
                      bool initOnly=false, int instanceNumber = -1, MStatus* stat=NULL);
   AtNode* ExportOptions();
   AtNode* ExportColorManager();

   CNodeTranslator *GetActiveTranslator(const CNodeAttrHandle &handle);
   bool IsRenderablePath(MDagPath dagPath);

   inline const ArnoldSessionMode& GetSessionMode() const         { return m_sessionOptions.GetSessionMode(); }
   inline void SetSessionMode(ArnoldSessionMode mode)             { m_sessionOptions.SetSessionMode(mode); }

   inline const ArnoldLightLinkMode& GetLightLinkMode() const {return m_sessionOptions.m_lightlink;}
   inline void SetLightLinkMode(ArnoldLightLinkMode mode) { m_sessionOptions.m_lightlink = mode; }

   inline const ArnoldShadowLinkMode& GetShadowLinkMode() const {return m_sessionOptions.m_shadowlink;}
   inline void SetShadowLinkMode(ArnoldShadowLinkMode mode) { m_sessionOptions.m_shadowlink = mode; }

   inline double GetExportFrame() const                   { return m_sessionOptions.GetExportFrame(); }
   void SetExportFrame(double frame)
   {
      if (frame != m_sessionOptions.GetExportFrame())
      {
         m_sessionOptions.SetExportFrame(frame);
         UpdateMotionFrames();
      }
   }

   inline const MDagPath& GetExportCamera() const         { return m_sessionOptions.GetExportCamera(); }
   void SetExportCamera(MDagPath camera, bool updateRender = true);
   inline const CMayaExportFilter& GetExportFilter() const { return m_sessionOptions.GetExportFilter(); }
   inline unsigned int GetExportFilterMask() const { return m_sessionOptions.GetExportFilterMask(); }
   inline void SetExportFilterMask(unsigned int mask) { m_sessionOptions.SetExportFilterMask(mask); }

   inline bool GetExportFullPath() const {return m_sessionOptions.GetExportFullPath();}
   inline const MString &GetExportPrefix() const {return m_sessionOptions.GetExportPrefix();}

   inline unsigned int GetExportNamespace() const {return m_sessionOptions.GetExportNamespace();}
   inline unsigned int GetExportSeparator() const {return m_sessionOptions.GetExportSeparator();}
   
   inline void SetExportFullPath(bool b) {m_sessionOptions.SetExportFullPath(b);}
   inline void SetExportPrefix(const MString &prefix) {m_sessionOptions.SetExportPrefix(prefix);}

   inline bool GetExportAllShadingGroups() const {return m_sessionOptions.GetExportAllShadingGroups();}
   inline void SetExportAllShadingGroups(bool b) {m_sessionOptions.SetExportAllShadingGroups(b);}
   

   // Export options
   inline const CSessionOptions& GetSessionOptions() const { return m_sessionOptions; }
   // Arnoldrender options
   inline const MObject& GetArnoldRenderOptions() const { return m_sessionOptions.GetArnoldRenderOptions(); }
   
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ANY) const { return m_sessionOptions.IsMotionBlurEnabled(type); }
   inline unsigned int GetNumMotionSteps() const { return m_sessionOptions.GetNumMotionSteps(); }
   inline const std::vector<double> &GetMotionFrames() const { return m_motion_frames; }
   inline unsigned int GetMotionStep() const {return m_motionStep;}
   inline double GetMotionByFrame() const {return m_sessionOptions.GetMotionByFrame(); }
   inline void GetMotionRange(double &motion_start, double &motion_end) const {m_sessionOptions.GetMotionRange(motion_start, motion_end); }

   inline bool IsProgressive() const { return m_sessionOptions.IsProgressive(); }

   // Light linker
   inline void FlagLightLinksDirty(bool flag=true) { m_lightLinksDirty = flag; }
   inline bool IsLightLinksDirty() const { return m_lightLinksDirty; }
   MStatus UpdateLightLinks();   
   inline MLightLinks* MayaLightLinks() { return &m_lightLinks; }
   inline unsigned int MayaLightCount() const { return m_numLights; }
   void ExportLightLinking(AtNode* shape, const MDagPath& path);
   
   // Flag to avoid IPR loops
   inline bool IsExportingMotion() const {return m_isExportingMotion; }

   // Updates
   void QueueForUpdate(CNodeTranslator * translator);
   void QueueForUpdate(const CNodeAttrHandle & handle);
   void RequestUpdate();
   void EraseActiveTranslator(CNodeTranslator *translator);

   // Instances
   inline void AddMasterInstanceHandle(MObjectHandle handle, MDagPath dagPath){m_masterInstances[handle] = dagPath;};
   inline MDagPath GetMasterInstanceDagPath(MObjectHandle handle){return m_masterInstances[handle];};

   bool IsBatch() const { return m_sessionOptions.IsBatch(); }
   bool IsInteractiveRender() const {return m_sessionOptions.IsInteractiveRender();}

   bool IsActiveAOV(CAOV &aov) const;
   AOVSet GetActiveAOVs() const;

   MStringArray GetActiveImageFilenames() const;
   
   DagFiltered FilteredStatus(const MDagPath &dagPath, const CMayaExportFilter *filter=NULL) const; 

   void FormatTexturePath(MString& texturePath);
   void FormatProceduralPath(MString& proceduralPath);

   MMatrix& ScaleMatrix(MMatrix& matrix) const;
   AtMatrix& ScaleMatrix(AtMatrix& matrix) const;
   float& ScaleDistance(float& distance) const;
   double& ScaleDistance(double& distance) const;
   float& ScaleArea(float& area) const;
   float& ScaleLightExposure(float& exposure) const;
   MVector GetOrigin() const;
   bool HasObjectsToUpdate() const {return !m_objectsToUpdate.empty();}
   
   MString GetMayaObjectName(const AtNode *node) const;
   
   // from a Maya name, get corresponding name in Arnold scene
   const char *GetArnoldObjectName(const MString &mayaName) const;

   const MStringArray &GetTextureSearchPaths() const;
   const MStringArray &GetProceduralSearchPaths() const;

   void RequestUpdateTx() {m_updateTx = true;}   
   
   // this is going to recreate all nodes that require motion data (when MB parameters are edited)
   void RequestUpdateMotion();

   void RequestUpdateOptions();

   void ExportTxFiles();
   void ExportImagePlane();

   const ObjectToTranslatorMap &GetProcessedTranslators() const {return m_processedTranslators;}

private:

   CArnoldSession()
      :  m_sessionOptions(CSessionOptions())
      ,  m_lightLinksDirty(false)
      ,  m_numLights(0)
      ,  m_lightLinks(MLightLinks())
      ,  m_isExportingMotion(false)
      ,  m_motionStep(0)
      ,  m_requestUpdate(false)
      ,  m_optionsTranslator(NULL)
      ,  m_is_active(false)
      ,  m_updateTx(false)
      ,  m_updateMotionData(false)
      ,  m_updateOptions(false)
   {
   }

   ~CArnoldSession() { End(); }

   // Edit Export options
   inline CSessionOptions* SessionOptions()                 { return &m_sessionOptions; }
   inline void SetSessionOptions(CSessionOptions& options)  { m_sessionOptions = options; }

   /// Initialize with passed CSessionOptions, ready for translating to the Arnold universe
   MStatus Begin(const CSessionOptions &options);
   /// Terminate an export session
   MStatus End();

   MStatus UpdateMotionFrames();
   
   MStatus Export(MSelectionList* selected = NULL);

   MStatus FlattenSelection(MSelectionList* selected, bool skipRoot = false);

   MStatus ExportCameras(MSelectionList* selected = NULL);
   MStatus ExportLights(MSelectionList* selected = NULL);
   MStatus ExportDag(MSelectionList* selected = NULL);

   inline bool NeedsUpdate() const { return m_requestUpdate; }


   void DoUpdate();
   void ClearUpdateCallbacks();

   inline bool IsActive() const { return m_is_active; }
   //void ProcessAOVs();

   static void HiddenNodeCallback(MObject& node, MPlug& plug, void* clientData);
   static void DoHiddenCallback(void* clientData);
   void SetDagVisible(MDagPath &path);

   static bool IsVisible(MFnDagNode &node);
   static bool IsVisiblePath(MDagPath dagPath);
   
   void RecursiveUpdateDagChildren(MDagPath &parent);
   
private:

   CSessionOptions m_sessionOptions;

   bool m_lightLinksDirty;
   unsigned int m_numLights;
   MLightLinks m_lightLinks;
   CArnoldLightLinks m_arnoldLightLinks;

   bool m_isExportingMotion;
   std::vector<double> m_motion_frames;
   int m_motionStep;

   bool m_requestUpdate;
   std::vector<ObjectToTranslatorPair> m_objectsToUpdate;
   
   ObjectToTranslatorMap m_processedTranslators;
   
   double m_scaleFactor;
   MMatrix m_scaleFactorMMatrix;
   AtMatrix m_scaleFactorAtMatrix;
   MVector m_origin;
   std::vector<HiddenObjectCallbackPair> m_hiddenObjectsCallbacks;
   
protected:
   ObjectHandleToDagMap m_masterInstances;

   COptionsTranslator*  m_optionsTranslator;
   bool                 m_is_active;
   bool                 m_updateTx;
   bool                 m_updateMotionData;
   bool                 m_updateOptions;
};  // class CArnoldSession