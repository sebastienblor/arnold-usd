#ifndef MAYA_SCENE_H
#define MAYA_SCENE_H

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

typedef std::map<MObjectHandle, CNodeTranslator*, mobjcompare> ObjectToTranslatorMap;

typedef void *   (*CreatorFunction)();
typedef void     (*NodeInitFunction)(MObject&);

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

   AtFloat GetCurrentFrame() { return m_currentFrame;}

   ExportMode GetExportMode() { return m_exportMode; }
   void SetExportMode( ExportMode mode ) { m_exportMode = mode; }
   
   
//private:
   
   void PrepareExport();
   MStatus IterSelection(MSelectionList selected);
   MStatus ExportScene(AtUInt step);
   MStatus ExportForIPR(AtUInt step);
   MStatus ExportSelected();
   bool ExportDagPath(MDagPath &dagPath, AtUInt step);
   static void RegisterDagTranslator(int typeId, CreatorFunction creator);
   static void RegisterTranslator(int typeId, CreatorFunction creator);
   CNodeTranslator * GetActiveTranslator( const MObject node );

   void ExportHair(const MDagPath& dagPath, AtUInt step);
   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);

   void ProcessShaderParameter(MFnDependencyNode shader, const char* param, AtNode* arnoldShader, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   AtNode* ExportArnoldShader(MObject mayaShader, MString arnoldShader);

   static bool IsVisible(MFnDagNode node);
   static bool IsTemplated(MFnDagNode node);
   static bool IsVisibleDag(MDagPath dagPath);
   bool IsMasterInstance(const MDagPath &dagPath, MDagPath &masterDag);
   void GetMatrix(AtMatrix& matrix, const MDagPath& dagPath);
   void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayamatrix);

   void GetMotionBlurData();

   void ClearIPRCallbacks();
   static void IPRNewNodeCallback(MObject & node, void *);
   static void IPRIdleCallback(void *);
   static void UpdateIPR(CNodeTranslator * translator=0x0);

   static std::vector< CNodeTranslator * > s_translatorsToIPRUpdate;
   static MCallbackId s_IPRIdleCallbackId;
   static MCallbackId s_NewNodeCallbackId;
   
//private:
   ExportMode m_exportMode;
   
   struct CShaderData
   {
      MObject mayaShader;
      AtNode* arnoldShader;
      MString attrName;
   };

   std::vector<CShaderData> m_processedShaders;

   ObjectToTranslatorMap m_processedTranslators;

   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;

   MDagPath m_camera;

   CMotionBlurData m_motionBlurData;

   AtFloat m_currentFrame;

   static std::map<int, CreatorFunction>  s_dagTranslators;
   static std::map<int, CreatorFunction>  s_dependTranslators;
};  // class CMayaScene

#endif // MAYA_SCENE_H
