#ifndef MAYASCENE_H
#define MAYASCENE_H

#include "platform/Platform.h"
#include "render/RenderOptions.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>
#include <maya/MObjectArray.h>
#include <maya/MSelectionList.h>
#include <maya/MObjectHandle.h>
#include <maya/MFnCamera.h>
#include <maya/MVectorArray.h>
#include <maya/MMessage.h>
#include <maya/MPlug.h>

#include <vector>
#include <set>
#include <map>
#include <string>

// The different ExportMode were not really mutually exclusive
// (you can have IPR render on selected only)
// So redone as ExportMode and ExportFilter
enum ExportMode
{
   MTOA_EXPORT_UNDEFINED,
   MTOA_EXPORT_ALL,
   MTOA_EXPORT_IPR,
   MTOA_EXPORT_SWATCH,
   MTOA_EXPORT_FILE
};

typedef std::set<MFn::Type> ExcludeSet;
// Any custom filter we might want on exports
// true means filtered OUT, ie NOT exported
struct ExportFilter
{
   bool unselected;
   bool templated;
   bool hidden;
   bool notinlayer;
   ExcludeSet excluded;

   ExportFilter() :  unselected(false),
                     templated(true),
                     hidden(true),
                     notinlayer(true)   {}
};

struct ExportOptions
{
   ExportMode mode;
   ExportFilter filter;
   MDagPath camera;

   ExportOptions() : mode(MTOA_EXPORT_UNDEFINED),
                     filter(ExportFilter()),
                     camera() {}
};

// To allow to specify that a dag node gets filtered out,
// or that it's whole hierarchy does
enum DagFiltered
{
   MTOA_EXPORT_ACCEPTED,
   MTOA_EXPORT_REJECTED_NODE,
   MTOA_EXPORT_REJECTED_BRANCH
};

class CNodeTranslator;

struct CMotionBlurData
{
   bool enabled;
   AtFloat shutter_size;
   AtFloat shutter_offset;
   AtUInt shutter_type;
   AtUInt motion_steps;
   AtFloat motion_frames;

   std::vector<float> frames;
};

struct mobjcompare
{
   bool operator()(MObjectHandle h1, MObjectHandle h2) const
   {
      return h1.hashCode() < h2.hashCode();
   }
};

// depend nodes map from MObject to translator
typedef std::map<MObjectHandle, CNodeTranslator*, mobjcompare> ObjectToTranslatorMap;
// dag nodes: have one translator per instance, so they map MObject to a sub-map, from dag instance number to translator
typedef std::map<MObjectHandle, std::map<int, CNodeTranslator*>, mobjcompare> ObjectToDagTranslatorMap;

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
      :  m_exportOptions(ExportOptions())
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

   inline AtFloat GetCurrentFrame()                      { return m_currentFrame;}

   inline ExportOptions GetExportOptions()               { return m_exportOptions; }
   inline void SetExportOptions(ExportOptions& options)  { m_exportOptions = options; }
   inline ExportMode GetExportMode()                     { return m_exportOptions.mode; }
   inline void SetExportMode(ExportMode mode)            { m_exportOptions.mode = mode; }
   inline ExportFilter GetExportFilter()                 { return m_exportOptions.filter; }
   inline void SetExportFilter(ExportFilter& filter)     { m_exportOptions.filter = filter; }
   inline MDagPath GetExportCamera()                     { return m_exportOptions.camera; }
   inline void SetExportCamera(MDagPath camera)   { camera.extendToShape();m_exportOptions.camera = camera; }

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

   bool IsMotionBlurEnabled() const
   {
      return (NULL != m_fnArnoldRenderOptions) && m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool();
   }
   
   bool IsCameraMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool();
   }

   bool IsObjectMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool();
   }

   bool IsDeformMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool();
   }
   
   bool IsLightMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool();
   }
   
   AtUInt GetNumMotionSteps() const
   {
      return m_motionBlurData.motion_steps;
   }
   
   AtFloat GetShutterSize() const
   {
      return m_motionBlurData.shutter_size;
   }
   
   AtUInt GetShutterType()
   {
      return m_motionBlurData.shutter_type;
   }

   MString GetAssName(const MString& customName,
                      const MCommonRenderSettingsData& renderGlobals,
                      double frameNumber,
                      const MString &sceneName,
                      const MString &cameraName,
                      const MString &fileFormat,
                      const MObject layer,
                      const bool createDirectory=true,
                      const bool isSequence=false,
                      const bool subFrames=false,
                      const bool isBatch=false,
                      MStatus *ReturnStatus=NULL) const;

   MStatus WriteAsstoc(const MString& filename, const AtBBox& bBox);

private:
   
   void PrepareExport();
   MStatus ExportScene();
   MStatus ExportCameras();
   MStatus ExportLights();
   MStatus ExportSelected();
   MStatus IterSelection(MSelectionList& selected);

   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);

   static DagFiltered FilteredStatus(ExportFilter filter, MDagPath dagPath);
   bool IsExportedPath(ExportFilter filter, MDagPath path);
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

   ExportOptions m_exportOptions;
   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;
   CMotionBlurData m_motionBlurData;

   // depend nodes, are a map with MObjectHandle as a key
   ObjectToTranslatorMap m_processedTranslators;
   // dag nodes, are a map in a map.
   // the first key is an MObjectHandle and the second the instance number
   ObjectToDagTranslatorMap m_processedDagTranslators;
   
   MDagPath m_camera;

   AtFloat m_currentFrame;

};  // class CMayaScene

#endif // MAYASCENE_H
