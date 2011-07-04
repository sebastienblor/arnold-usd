#ifndef MAYASCENE_H
#define MAYASCENE_H

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
/// Once CMayaScene::ExportToArnold() is called, the DAG hierarchy is traversed and CDagTranslators
/// are found and exported for all relevant Maya nodes.  Those translators in turn call
/// and CMayaScene::ExportShader() as they require, which triggers dependency graph evaluation and the
/// generation of CNodeTranslators.

class DLLEXPORT CMayaScene
{

public:

   CMayaScene()
      :  m_exportOptions(CExportOptions())
      ,  m_fnCommonRenderOptions(NULL)
      ,  m_fnArnoldRenderOptions(NULL)
      ,  m_currentFrame(0)
   {
   }

   ~CMayaScene();

   MStatus ExportToArnold();
   AtNode* ExportShader(MObject mayaShader, const MString &attrName="");
   AtNode* ExportShader(MPlug& shaderOutputPlug);
   AtNode* ExportDagPath(MDagPath &dagPath);

   inline double GetCurrentFrame()                        { return m_currentFrame;}

   inline CExportOptions* GetExportOptions()              { return &m_exportOptions; }
   inline void SetExportOptions(CExportOptions& options)  { m_exportOptions = options; }

   inline ExportMode GetExportMode()                      { return m_exportOptions.GetExportMode(); }
   inline void SetExportMode(ExportMode mode)             { m_exportOptions.SetExportMode(mode); }

   void ProcessShaderParameter(MFnDependencyNode shader,
                               const char* param,
                               AtNode* arnoldShader,
                               const char* arnoldAttrib,
                               int arnoldAttribType,
                               int element=-1);
   
   CNodeTranslator * GetActiveTranslator(const MObject node);
   
   // Called by translators to get the master instance
   static bool IsExportedPath(MDagPath dagPath);
   static bool IsRenderablePath(MDagPath dagPath);
   static void UpdateIPR(CNodeTranslator * translator=NULL);

   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_exportOptions.IsMotionBlurEnabled(type); }
   inline unsigned int GetNumMotionSteps() const { return m_exportOptions.GetNumMotionSteps(); }
   inline float GetShutterSize() const { return m_exportOptions.GetShutterSize(); }
   inline unsigned int GetShutterType() const { return m_exportOptions.GetShutterType(); }

private:
   
   void PrepareExport();
   MStatus ExportScene();
   MStatus ExportCameras();
   MStatus ExportLights();
   MStatus ExportSelected();
   MStatus IterSelection(MSelectionList& selected);

   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);

   static DagFiltered FilteredStatus(CExportFilter filter, MDagPath dagPath);
   bool IsExportedPath(CExportFilter filter, MDagPath path);
   static bool IsInRenderLayer(MDagPath dagPath);
   static bool IsVisiblePath(MDagPath dagPath);
   static bool IsTemplatedPath(MDagPath dagPath);
   static bool IsVisible(MFnDagNode node);
   static bool IsTemplated(MFnDagNode node);
   bool IsMasterInstance(const MDagPath &dagPath, MDagPath &masterDag);
   void GetMatrix(AtMatrix& matrix, const MDagPath& dagPath);
   void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayamatrix);

   void GetMotionBlurData();

   void ClearIPRCallbacks();
   static void IPRNewNodeCallback(MObject & node, void *);
   static void IPRIdleCallback(void *);

   static std::vector< CNodeTranslator * > s_translatorsToIPRUpdate;
   static MCallbackId s_IPRIdleCallbackId;
   static MCallbackId s_NewNodeCallbackId;
   static bool s_isExportingMotion;
   
private:

   CExportOptions m_exportOptions;
   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;
   MDagPath m_camera;

   double m_currentFrame;
   std::vector<double> m_motion_frames;

   // depend nodes, are a map with MObjectHandle as a key
   ObjectToTranslatorMap m_processedTranslators;
   // dag nodes, are a map in a map.
   // the first key is an MObjectHandle and the second the instance number
   ObjectToDagTranslatorMap m_processedDagTranslators;


};  // class CMayaScene

#endif // MAYASCENE_H
