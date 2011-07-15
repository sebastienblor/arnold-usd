#ifndef ARNOLDSESSION_H
#define ARNOLDSESSION_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
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

// TODO : translator per output attribute / arnold node for Maya node that can alternatively
// generate different Arnold nodes (multiple outputs), possibly in the same Arnold universe
// when the maya node is shared (Trac #351)
// depend nodes map from MObject to translator
typedef std::map<MObjectHandle, CNodeTranslator*, MObjectCompare> ObjectToTranslatorMap;
// dag nodes: have one translator per instance, so they map MObject to a sub-map, from dag instance number to translator
typedef std::map<MObjectHandle, std::map<int, CNodeTranslator*>, MObjectCompare> ObjectToDagTranslatorMap;

/// Opens an Arnold session, in which you can make changes to the Arnold universe: create or edit Arnold nodes.

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
   AtNode* ExportNode(MPlug& shaderOutputPlug, MStatus* stat=NULL);
   AtNode* ExportNode(MObject node, const MString &attrName="", MStatus* stat=NULL);

   // FIXME : shouldn't probably be public
   AtNode* ExportWithTranslator(MObject node, const MString &mayaNodeClass, const MString &translatorName);
   AtNode* ExportDriver(MObject node, const MString &translatorName);
   AtNode* ExportFilter(MObject node, const MString &translatorName);

   CNodeTranslator * GetActiveTranslator(const MObject node);
   static bool IsRenderablePath(MDagPath dagPath);

   inline const ArnoldSessionMode& GetSessionMode() const         { return m_sessionOptions.GetSessionMode(); }
   inline void SetSessionMode(ArnoldSessionMode mode)             { m_sessionOptions.SetSessionMode(mode); }

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
   inline void SetExportCamera(MDagPath camera)           { m_sessionOptions.SetExportCamera(camera); }

   inline unsigned int GetExportFilter() const { return m_sessionOptions.GetExportFilter(); }
   inline void SetExportFilter(unsigned int mask) { m_sessionOptions.SetExportFilter(mask); }

   inline const MObject& GetArnoldRenderOptions() const   { return m_sessionOptions.GetArnoldRenderOptions(); }
   
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_sessionOptions.IsMotionBlurEnabled(type); }
   inline unsigned int GetNumMotionSteps() const { return m_sessionOptions.GetNumMotionSteps(); }
   inline float GetShutterSize() const { return m_sessionOptions.GetShutterSize(); }
   inline unsigned int GetShutterType() const { return m_sessionOptions.GetShutterType(); }

   // Flag to avoid IPR loops
   inline bool IsExportingMotion() const {return m_isExportingMotion; }

   // Updates
   void QueueForUpdate(CNodeTranslator * translator);
   void RequestUpdate();

private:

   CArnoldSession()
      :  m_sessionOptions(CSessionOptions())
      ,  m_isExportingMotion(false)
      ,  m_requestUpdate(false)
   {
   }

   ~CArnoldSession() { End(); }

   // Export options
   inline const CSessionOptions& GetSessionOptions() const  { return m_sessionOptions; }
   inline CSessionOptions* SessionOptions()                 { return &m_sessionOptions; }
   inline void SetSessionOptions(CSessionOptions& options)  { m_sessionOptions = options; }

   /// Initialize with passed CSessionOptions, ready for translating to the Arnold universe
   MStatus Begin(CSessionOptions* options);
   /// Terminate an export session
   MStatus End();

   MStatus UpdateMotionBlurData();
   MStatus UpdateMotionFrames();
   
   MStatus Export(MSelectionList* selected = NULL);

   MStatus ExportScene();
   MStatus ExportCameras();
   MStatus ExportLights();
   MStatus ExportSelection(MSelectionList& selected);
   MStatus IterSelection(MSelectionList& selected);

   DagFiltered FilteredStatus(MDagPath dagPath);

   inline bool NeedsUpdate() { return m_requestUpdate; }

   void DoUpdate();
   void ClearUpdateCallbacks();
   
private:

   CSessionOptions m_sessionOptions;

   bool m_isExportingMotion;
   std::vector<double> m_motion_frames;

   bool m_requestUpdate;
   std::vector< CNodeTranslator * > m_translatorsToUpdate;

   // depend nodes, are a map with MObjectHandle as a key
   ObjectToTranslatorMap m_processedTranslators;
   // dag nodes, are a map in a map.
   // the first key is an MObjectHandle and the second the instance number
   ObjectToDagTranslatorMap m_processedDagTranslators;

};  // class CArnoldSession

#endif // ARNOLDSESSION_H
