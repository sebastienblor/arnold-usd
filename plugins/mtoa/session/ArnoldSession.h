#ifndef ARNOLDSESSION_H
#define ARNOLDSESSION_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "render/AOV.h"
#include "SessionOptions.h"

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
class COptionsTranslator;

// TODO : translator per output attribute / arnold node for Maya node that can alternatively
// generate different Arnold nodes (multiple outputs), possibly in the same Arnold universe
// when the maya node is shared (Trac #351)
// depend nodes map from Node + Attr MObject to translator(s)
typedef std::multimap<CNodeAttrHandle, CNodeTranslator*> ObjectToTranslatorMap;
typedef std::pair<CNodeAttrHandle, CNodeTranslator*> ObjectToTranslatorPair;
// dag nodes: have one translator per instance, so they map MObject to a sub-map, from dag instance number to translator
typedef std::map<CNodeAttrHandle, std::map<int, CNodeTranslator*> > ObjectToDagTranslatorMap;
// Abstract base class for Dag node translators
//
typedef std::map<MObjectHandle, MDagPath, MObjectCompare> ObjectHandleToDagMap;
/// Opens an Arnold session, in which you can make changes to the Arnold universe: create or edit Arnold nodes.

typedef std::set<AtNode*> AtNodeSet;

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
   AtNode* ExportDagPath(MDagPath &dagPath, MStatus* stat=NULL);
   AtNode* ExportNode(const MPlug& shaderOutputPlug, AtNodeSet* nodes=NULL, AOVSet* aovs=NULL, MStatus* stat=NULL);

   // FIXME : shouldn't probably be public
   AtNode* ExportWithTranslator(MObject node, const MString &mayaNodeClass, const MString &translatorName);
   AtNode* ExportDriver(MObject node, const MString &translatorName);
   AtNode* ExportFilter(MObject node, const MString &translatorName);
   AtNode* ExportOptions();

   bool GetActiveTranslators(const MObject& object, std::vector<CNodeTranslator* >& result);
   CNodeTranslator * GetActiveTranslator(const MPlug& plug);
   static bool IsRenderablePath(MDagPath dagPath);

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
   void SetExportCamera(MDagPath camera);
   inline unsigned int GetExportFilter() const { return m_sessionOptions.GetExportFilter(); }
   inline void SetExportFilter(unsigned int mask) { m_sessionOptions.SetExportFilter(mask); }

   // Export options
   inline const CSessionOptions& GetSessionOptions() const  { return m_sessionOptions; }
   // Arnoldrender options
   inline const MObject& GetArnoldRenderOptions() const   { return m_sessionOptions.GetArnoldRenderOptions(); }
   
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_sessionOptions.IsMotionBlurEnabled(type); }
   inline unsigned int GetNumMotionSteps() const { return m_sessionOptions.GetNumMotionSteps(); }
   inline float GetShutterSize() const { return m_sessionOptions.GetShutterSize(); }
   inline unsigned int GetShutterType() const { return m_sessionOptions.GetShutterType(); }
   inline double GetMotionByFrame() const {return m_sessionOptions.GetMotionByFrame(); }

   // Light linker
   inline MLightLinks* MayaLightLinks() { return &m_lightLinks; }

   // Flag to avoid IPR loops
   inline bool IsExportingMotion() const {return m_isExportingMotion; }

   // Updates
   void QueueForUpdate(CNodeTranslator * translator);
   void RequestUpdate();

   // Instances
   int GetMasterInstanceNumber(MObject node);
   inline void AddMasterInstanceHandle(MObjectHandle handle, MDagPath dagPath){m_masterInstances[handle] = dagPath;};
   inline MDagPath GetMasterInstanceDagPath(MObjectHandle handle){return m_masterInstances[handle];};

   bool IsBatch() const { return (GetSessionMode() == MTOA_SESSION_BATCH || GetSessionMode() == MTOA_SESSION_ASS); }

   bool IsActiveAOV(CAOV &aov) const;
   AOVSet GetActiveAOVs() const;

   DagFiltered FilteredStatus(MDagPath dagPath);
   
/*
   bool IsActiveAOV(CAOV &aov) const
   {
      if (m_aovs.count(aov))
         return true;
      else
         return false;
   }

   AOVSet GetActiveAOVs() const
   {
      return m_aovs;
   }

   unsigned int NumAOVs() const
   {
      return static_cast<unsigned int>(m_aovs.size());
   }
*/
private:

   CArnoldSession()
      :  m_sessionOptions(CSessionOptions())
      ,  m_lightLinks(MLightLinks())
      ,  m_isExportingMotion(false)
      ,  m_requestUpdate(false)
      ,  m_optionsTranslator(NULL)
      ,  m_is_active(false)
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

   MStatus UpdateLightLinks();
   MStatus UpdateMotionFrames();
   
   MStatus Export(MSelectionList* selected = NULL);

   MStatus ExportScene();
   MStatus ExportCameras();
   MStatus ExportLights();

   MStatus ExportSelection(MSelectionList& selected);
   MStatus IterSelection(MSelectionList& selected);

   inline bool NeedsUpdate() const { return m_requestUpdate; }

   void DoUpdate();
   void ClearUpdateCallbacks();

   inline bool IsActive() const { return m_is_active; }
   //void ProcessAOVs();

private:

   CSessionOptions m_sessionOptions;
   MLightLinks m_lightLinks;

   bool m_isExportingMotion;
   std::vector<double> m_motion_frames;

   bool m_requestUpdate;
   std::vector< CNodeTranslator * > m_translatorsToUpdate;

   // depend nodes and dag nodes are a multimap with CNodeAttrHandle as a key
   ObjectToTranslatorMap m_processedTranslators;

protected:
   ObjectHandleToDagMap m_masterInstances;

   COptionsTranslator*  m_optionsTranslator;
   bool                 m_is_active;
   //AOVSet m_aovs;
};  // class CArnoldSession

#endif // ARNOLDSESSION_H
