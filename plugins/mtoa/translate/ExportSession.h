#ifndef EXPORTSESSION_H
#define EXPORTSESSION_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "ExportOptions.h"

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

/// Translates the current state of all or part of an open Maya scene into the active Arnold universe.

/// In IPR mode, the resulting instance allows the scene to be quickly and incrementally retranslated
/// as changes occur to previously translated Maya objects.
///
/// Once CExportSession::ExportToArnold() is called, the DAG hierarchy is traversed and CDagTranslators
/// are found and exported for all relevant Maya nodes.  Those translators in turn call
/// and CExportSession::ExportShader() as they require, which triggers dependency graph evaluation and the
/// generation of CNodeTranslators.

class DLLEXPORT CExportSession
{
   friend class CMayaScene;

public:

   // Called by translators
   AtNode* ExportDagPath(MDagPath &dagPath);
   AtNode* ExportNode(MPlug& shaderOutputPlug);
   AtNode* ExportNode(MObject node, const MString &attrName="");

   CNodeTranslator * GetActiveTranslator(const MObject node);
   static bool IsRenderablePath(MDagPath dagPath);

   // Export options
   inline const CExportOptions& GetExportOptions() const  { return m_exportOptions; }
   inline CExportOptions* ExportOptions()                 { return &m_exportOptions; }
   inline void SetExportOptions(CExportOptions& options)  { m_exportOptions = options; }

   inline const ExportMode& GetExportMode() const         { return m_exportOptions.GetExportMode(); }
   inline void SetExportMode(ExportMode mode)             { m_exportOptions.SetExportMode(mode); }

   inline double GetExportFrame() const                   { return m_exportOptions.GetExportFrame(); }
   void SetExportFrame(double frame)
   {
      if (frame != m_exportOptions.GetExportFrame())
      {
         m_exportOptions.SetExportFrame(frame);
         UpdateMotionFrames();
      }
   }

   inline const MDagPath& GetExportCamera() const         { return m_exportOptions.GetExportCamera(); }
   inline void SetExportCamera(MDagPath camera)           { m_exportOptions.SetExportCamera(camera); }

   inline const MObject& GetArnoldRenderOptions() const   { return m_exportOptions.GetArnoldRenderOptions(); }
   
   inline const CExportFilter& GetExportFilter() const    { return m_exportOptions.GetExportFilter(); }
   inline CExportFilter* ExportFilter()                   { return m_exportOptions.ExportFilter(); }
   inline void SetExportFilter(CExportFilter& filter)     { m_exportOptions.SetExportFilter(filter); }
   
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_exportOptions.IsMotionBlurEnabled(type); }
   inline unsigned int GetNumMotionSteps() const { return m_exportOptions.GetNumMotionSteps(); }
   inline float GetShutterSize() const { return m_exportOptions.GetShutterSize(); }
   inline unsigned int GetShutterType() const { return m_exportOptions.GetShutterType(); }

   // Flag to avoid IPR loops
   inline bool IsExportingMotion() const {return m_isExportingMotion; }

   // Updates
   void QueueForUpdate(CNodeTranslator * translator);
   void RequestUpdate();

private:

   CExportSession()
      :  m_exportOptions(CExportOptions())
      ,  m_isExportingMotion(false)
      ,  m_requestUpdate(false)
   {
   }

   ~CExportSession() { End(); }

   /// Initialize with passed CExportOptions, ready for translating to the Arnold universe
   MStatus Begin(CExportOptions* options);
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

   CExportOptions m_exportOptions;

   bool m_isExportingMotion;
   std::vector<double> m_motion_frames;

   bool m_requestUpdate;
   std::vector< CNodeTranslator * > m_translatorsToUpdate;

   // depend nodes, are a map with MObjectHandle as a key
   ObjectToTranslatorMap m_processedTranslators;
   // dag nodes, are a map in a map.
   // the first key is an MObjectHandle and the second the instance number
   ObjectToDagTranslatorMap m_processedDagTranslators;

};  // class CExportSession

#endif // EXPORTSESSION_H
