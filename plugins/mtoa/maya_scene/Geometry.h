#include "maya_scene/NodeTranslator.h"
#include <maya/MFnMesh.h>

#include <vector>
#include <map>
#include <string>

class CGeoTranslator
   :   public CDagTranslator
{
public:
   void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = m_scene->m_motionBlurData.enabled &&
                  m_scene->m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
                  m_fnNode.findPlug("motionBlur").asBool();
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }

protected:
   bool GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices);
   bool GetNormals(MFnMesh &fnMesh, std::vector<float> &normals);
   bool GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents);
   bool GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs);
   bool GetVertexColors(MFnMesh &fnMesh, std::map<std::string, std::vector<float> > &vcolors);
   void GetComponentIDs(MFnMesh &fnMesh,
         std::vector<AtUInt> &nsides,
         std::vector<AtLong> &vidxs,
         std::vector<AtLong> &nidxs,
         std::vector<AtLong> &uvidxs,
         bool exportNormals,
         bool exportUVs
         );

   MObject GetNodeShadingGroup(MObject dagNode, int instanceNum);
   MObject GetNodeShader(MObject dagNode, int instanceNum);

   void ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, AtUInt step);
   AtNode* ExportMesh(MObject mayaMesh);
   void ExportMeshMotion(AtNode* polymesh, MObject mayaMesh, AtUInt step);
   AtNode* ExportMeshInstance(const MDagPath& masterInstance);
   void ExportMeshInstanceMotion(AtNode* instance, AtUInt step);



protected:
   bool m_isMasterDag;
   bool m_motion;
};


class CMeshTranslator : public CGeoTranslator
{
public:
   AtNode* Export();
   void ExportMotion(AtNode* polymesh, AtUInt step);
   void Update(AtNode* polymesh);
   static void* creator()
   {
      return new CMeshTranslator();
   }
private:
   unsigned int GetNumMeshGroups();
};


class CNurbsSurfaceTranslator : public CGeoTranslator
{
public:
   AtNode* Export();
   void ExportMotion(AtNode* polymesh, AtUInt step);
   void Update(AtNode* polymesh);
   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
private:
   MObject m_tesselatedMesh;
};
