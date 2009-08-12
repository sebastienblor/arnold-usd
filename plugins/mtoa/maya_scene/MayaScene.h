#ifndef MAYA_SCENE_H
#define MAYA_SCENE_H

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>

#include <vector>

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

   MStatus ExportToArnold();
   AtNode* ExportShader(MObject mayaShader);

private:
   
   void PrepareExport();
   MStatus ExportScene(AtUInt step);

   void ExportCamera(const MDagPath& dagPath, AtUInt step);
   void ExportLight(const MDagPath& dagPath, AtUInt step);
   void ExportLightData(AtNode* light, const MDagPath& dagPath, bool mb, bool custom);
   void ExportLightMBData(const MDagPath& dagPath, AtUInt step);
   void ExportMesh(MObject mayaMesh, const MDagPath& dagPath, AtUInt step);
   void ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, const MDagPath& dagPath, AtUInt step);

   MObject GetNodeShader(MObject dagNode);

   bool IsVisible(MFnDagNode node);
   void GetMatrix(AtMatrix& matrix, const MDagPath& dagPath);

   void GetMotionBlurData();

private:

   struct CShaderData
   {
      MObject mayaShader;
      AtNode* arnoldShader;
   };

   std::vector<CShaderData> m_processedShaders;

   MFnDependencyNode* m_fnCommonRenderOptions;
   MFnDependencyNode* m_fnArnoldRenderOptions;

   MDagPath m_camera;

   CMotionBlurData m_motionBlurData;

   AtInt m_currentFrame;

};  // class CMayaScene

#endif // MAYA_SCENE_H
