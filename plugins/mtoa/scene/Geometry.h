#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "scene/NodeTranslator.h"
#include <maya/MFnMesh.h>
#include <maya/MNodeMessage.h>

#include <vector>
#include <map>
#include <string>

class CGeoTranslator
   :   public CShapeTranslator
{
public:
   void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CShapeTranslator::Init(dagPath, scene, outputAttr);
      m_motion = m_scene->IsObjectMotionBlurEnabled() && m_fnNode.findPlug("motionBlur").asBool();
      m_motionDeform = m_motion && m_scene->IsObjectDeformMotionBlurEnabled();
      m_displaced = false;
   }
   virtual bool RequiresMotionData()
   {
      return m_motion;
   }
   void Update(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);
   void UpdateMotion(AtNode* anode, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   virtual void AddIPRCallbacks();

protected:

   bool GetVerticesWorld(MObject &dagNode, MFnMesh &fnMesh, std::vector<float> &vertices);
   bool GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices);
   bool GetNormals(MFnMesh &fnMesh, std::vector<float> &normals);
   bool GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents);
   bool GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs);
   bool GetRefObj(MFnMesh &fnMesh, std::vector<float> &refVertices, std::vector<float> &refNormals);
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
   virtual void IsGeoDeforming();

protected:
   bool m_isMasterDag;
   bool m_motion;
   bool m_motionDeform;
   bool m_displaced;
   MFnMesh m_fnMesh;
   MDagPath m_masterDag;
};


class CMeshTranslator : public CGeoTranslator
{
public:
   void Export(AtNode* anode);
   static void* creator()
   {
      return new CMeshTranslator();
   }
   const char* GetArnoldNodeType();
private:
   unsigned int GetNumMeshGroups();
};

class MFnMeshData;
class MTesselationParams;
class MFnNurbsSurface;

class CNurbsSurfaceTranslator : public CGeoTranslator
{
public:
   void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode, AtUInt step);
   virtual void IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   const char* GetArnoldNodeType();
private:
   MObject m_data_mobj;
   bool Tessellate(MDagPath & dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface );

};

#endif // GEOMETRY_H
