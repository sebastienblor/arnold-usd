#ifndef MAYA_SCENE_H
#define MAYA_SCENE_H

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

#include <vector>
#include <map>
#include <string>

enum ExportMode
{
   MTOA_EXPORT_ALL,
   MTOA_EXPORT_SELECTED
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

class CMayaScene
{

public:

   CMayaScene()
      :  m_fnCommonRenderOptions(NULL)
      ,  m_fnArnoldRenderOptions(NULL)
      ,  m_currentFrame(0)
   {
   }

   ~CMayaScene()
   {
      delete m_fnCommonRenderOptions;
      delete m_fnArnoldRenderOptions;
   }

   MStatus ExportToArnold(ExportMode exportMode = MTOA_EXPORT_ALL);
   AtNode* ExportShader(MObject mayaShader, const MString &attrName="");

   AtFloat GetCurrentFrame() { return m_currentFrame;};

private:
   
   void PrepareExport();
   MStatus IterSelection(MSelectionList selected);
   MStatus ExportScene(AtUInt step);
   MStatus ExportSelected();   

   void ExportCamera(const MDagPath& dagPath, AtUInt step);
   void ExportCameraData(AtNode* camera, const MDagPath& dagPath, bool mb);
   void ExportCameraMBData(const MDagPath& dagPath, AtUInt step);
   double GetDeviceAspect();
   void GetOrthoFilmback(AtNode* camera, MFnCamera& fnCamera);
   float GetPerspFilmback(AtNode* camera, MFnCamera& fnCamera);
   MVectorArray GetFilmTransform(MFnCamera& fnCamera, double width=0, bool persp=true);
   void ExportImagePlane(const MDagPath& dagPath, bool mb, AtUInt step);
   void ExportLight(const MDagPath& dagPath, AtUInt step);
   void ExportLightData(AtNode* light, const MDagPath& dagPath, bool mb, bool custom);
   void ExportLightMBData(const MDagPath& dagPath, AtUInt step);
   void ExportLightFilters(AtNode* light, const MObjectArray &filters);
   void ExportMesh(MObject mayaMesh, const MDagPath& dagPath, AtUInt step);
   void ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, const MDagPath& dagPath, AtUInt step);
   void ExportMeshInstance(const MDagPath& dagPath, const MDagPath& masterInstance, AtUInt step);
   void ExportHair(const MDagPath& dagPath, AtUInt step);
   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);
   void ExportCustomShape(const MDagPath &dagPath, AtUInt step, const MString &exportCmd, const MString &cleanupCmd);

   void ProcessShaderParameter(MFnDependencyNode shader, const char* param, AtNode* arnoldShader, const char* arnoldAttrib, int arnoldAttribType);
   AtNode* ExportArnoldShader(MObject mayaShader, MString arnoldShader);
   MObject GetNodeShader(MObject dagNode, int instanceNum);

   bool IsVisible(MFnDagNode node);
   bool IsVisibleDag(MDagPath dagPath);
   bool IsMasterInstance(const MDagPath &dagPath, MDagPath &masterDag);
   void GetMatrix(AtMatrix& matrix, const MDagPath& dagPath);
   void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayamatrix);

   void GetMotionBlurData();
   void GetCustomShapes();
   bool RegisterCustomShape(std::string &shapeType);
   void GetCustomShapeInstanceShader(const MDagPath &path, MFnDependencyNode &shadingEngineNode);

private:

   struct CShaderData
   {
      MObject mayaShader;
      AtNode* arnoldShader;
      MString attrName;
   };
   
   struct CCustomData
   {
      MString exportCmd;
      MString cleanupCmd;
   };

   std::vector<CShaderData> m_processedShaders;

   struct mobjcompare
   {
      bool operator()(MObjectHandle h1, MObjectHandle h2) const
      {
         return h1.hashCode() < h2.hashCode();
      }
   };

   typedef std::map<MObjectHandle, MDagPath, mobjcompare> ObjectHandleToDagMap;
   ObjectHandleToDagMap m_masterInstances;

   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;

   MDagPath m_camera;

   CMotionBlurData m_motionBlurData;

   AtFloat m_currentFrame;
   
   std::map<std::string, CCustomData> m_customShapes;

};  // class CMayaScene

#endif // MAYA_SCENE_H
