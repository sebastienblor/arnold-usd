#ifndef MAYA_SCENE_H
#define MAYA_SCENE_H

#include <ai_nodes.h>

#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>

#include <vector>

class CMayaScene
{

public:

   MStatus ExportToArnold();
   AtNode* ExportShader(MObject mayaShader);

private:

   void ExportCamera(const MDagPath& dagPath);
   void ExportLight(const MDagPath& dagPath);
   void ExportMesh(MObject mayaMesh, MObject dagNode, MMatrix tm);
   MObject GetNodeShader(MObject dagNode);

   bool IsVisible(MFnDagNode node);

private:

   struct CShaderData
   {
      MObject mayaShader;
      AtNode* arnoldShader;
   };  // struct CShaderData

   std::vector<CShaderData> m_processedShaders;

};  // class CMayaScene

#endif // MAYA_SCENE_H
