#ifndef MAYASCENE_H
#define MAYASCENE_H

#include "platform/Platform.h"

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
#include <map>
#include <string>

class CNodeTranslator;

enum ExportMode
{
   MTOA_EXPORT_UNDEFINED,
   MTOA_EXPORT_ALL,
   MTOA_EXPORT_SELECTED,
   MTOA_EXPORT_IPR,
   MTOA_EXPORT_SWATCH
};

struct CMotionBlurData
{
   bool enabled;
   float shutter_size;
   float shutter_offset;
   int shutter_type;
   int motion_steps;
   float motion_frames;

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

class DLLEXPORT CMayaScene
{

public:

   CMayaScene()
      :  m_exportMode( MTOA_EXPORT_UNDEFINED )
      ,  m_fnCommonRenderOptions(NULL)
      ,  m_fnArnoldRenderOptions(NULL)
      ,  m_currentFrame(0)
   {
   }

   ~CMayaScene();

   MStatus ExportToArnold(ExportMode exportMode = MTOA_EXPORT_ALL);
   AtNode* ExportShader(MObject mayaShader, const MString &attrName="");
   AtNode* ExportShader(MPlug& shaderOutputPlug);

   AtFloat GetCurrentFrame()              { return m_currentFrame;}
   ExportMode GetExportMode()             { return m_exportMode; }
   void SetExportMode( ExportMode mode )  { m_exportMode = mode; }

   void ProcessShaderParameter(MFnDependencyNode shader,
                               const char* param,
                               AtNode* arnoldShader,
                               const char* arnoldAttrib,
                               int arnoldAttribType,
                               int element=-1);
   
   CNodeTranslator * GetActiveTranslator( const MObject node );
   
   static void UpdateIPR(CNodeTranslator * translator=NULL);
   static bool IsVisibleDag(MDagPath dagPath);

   bool IsMotionBlurEnabled() const
   {
      return m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool();
   }
   
   bool IsCameraMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool();
   }

   bool IsObjectMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool();
   }

   bool IsObjectDeformMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool();
   }
   
   bool IsLightMotionBlurEnabled() const
   {
      return IsMotionBlurEnabled() && m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool();
   }
   
   int GetNumMotionSteps() const
   {
      return m_motionBlurData.motion_steps;
   }
   
   float GetShutterSize() const
   {
      return m_motionBlurData.shutter_size;
   }
   
   int GetShutterType()
   {
      return m_motionBlurData.shutter_type;
   }

private:
   
   void PrepareExport();
   MStatus ExportScene(AtUInt step);
   MStatus ExportSelected(AtUInt step);
   MStatus IterSelection(MSelectionList selected, AtUInt step);
   bool ExportDagPath(MDagPath &dagPath, AtUInt step);

   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);

   static bool IsInRenderLayer(MDagPath dagPath);
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
   
private:
   struct CShaderData
   {
      MObject mayaShader;
      AtNode* arnoldShader;
      MString attrName;
   };

   ExportMode m_exportMode;
   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;
   CMotionBlurData m_motionBlurData;
   
   // this variable will go away when we get multiple output support
   std::vector<CShaderData> m_processedShaders;
   // depend nodes, are a map with MObjectHandle as a key
   ObjectToTranslatorMap m_processedTranslators;
   // dag nodes, are a map in a map.
   // the first key is an MObjectHandle and the second the instance number
   ObjectToDagTranslatorMap m_processedDagTranslators;
   
   MDagPath m_camera;

   AtFloat m_currentFrame;
};  // class CMayaScene

#endif // MAYASCENE_H
