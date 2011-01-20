#include "maya_scene/NodeTranslator.h"
#include <maya/MFnMesh.h>
#include <maya/MNodeMessage.h>

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
      m_motionDeform = m_scene->m_motionBlurData.enabled &&
                       m_scene->m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
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
   void Update(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);
   void UpdateMotion(AtNode* anode, AtUInt step);
   virtual void AddCallbacks();

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
   void ExportMeshShaders(AtNode* polymesh, MFnMesh &fnMesh);
   virtual void ExportShaders();

   void ExportMeshGeoData(AtNode* polymesh, AtUInt step);
   void ExportMeshParameters(AtNode* polymesh);
   AtNode* ExportMesh(AtNode* polymesh, bool update);
   void ExportMeshMotion(AtNode* polymesh, AtUInt step);
   AtNode* ExportInstance(AtNode* instance, const MDagPath& masterInstance);
   void ExportInstanceMotion(AtNode* instance, AtUInt step);

   static void ShaderAssignmentCallback( MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* );
   void AddShaderAssignmentCallbacks(MObject & dagNode );

protected:
   bool m_isMasterDag;
   bool m_motion;
   bool m_motionDeform;
   MFnMesh m_fnMesh;
   MDagPath m_masterDag;
};


class CMeshTranslator : public CGeoTranslator
{
public:
   AtNode* Export();
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
   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
private:
   MObject m_tesselatedMesh;
};
