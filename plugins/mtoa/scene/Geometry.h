#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "scene/NodeTranslator.h"
#include <maya/MFnMesh.h>
#include <maya/MNodeMessage.h>

#include <vector>
#include <map>
#include <string>

class DLLEXPORT CGeoTranslator
   :   public CShapeTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_displaced = false;
      m_isRefSmooth = false;
      return CShapeTranslator::Init(dagPath, scene, outputAttr);
   }
   virtual void Update(AtNode* anode);
   virtual void ExportMotion(AtNode* anode, AtUInt step);
   virtual void UpdateMotion(AtNode* anode, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   virtual void AddIPRCallbacks();

protected:

   bool GetVerticesWorld(MFnMesh &fnMesh, std::vector<float> &vertices);
   bool GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices);
   bool GetNormals(MFnMesh &fnMesh, std::vector<float> &normals);
   bool GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents);
   bool GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs);
   bool GetMeshRefObj(MFnMesh &fnMesh);
   bool GetRefObj(MFnMesh &fnMesh, std::vector<float> &refVertices, std::vector<float> &refNormals);
   bool GetVertexColors(MFnMesh &fnMesh, std::map<std::string, std::vector<float> > &vcolors);
   void GetComponentIDs(MFnMesh &fnMesh,
         std::vector<AtUInt> &nsides,
         std::vector<AtLong> &vidxs,
         std::vector<AtLong> &nidxs,
         std::vector<AtLong> &uvidxs,
         bool exportNormals,
         bool exportUVs);

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

   static void ShaderAssignmentCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*);
   void AddShaderAssignmentCallbacks(MObject & dagNode);
   virtual void IsGeoDeforming();

protected:
   bool m_isMasterDag;
   bool m_displaced;
   bool m_isRefSmooth;
   MObject m_data_mobj;
   MFnMesh m_fnMesh;
   MFnMesh m_fnMeshRef;
   MDagPath m_masterDag;
};


class DLLEXPORT CMeshTranslator : public CGeoTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_fnMesh.setObject(dagPath);
      return CGeoTranslator::Init(dagPath, scene, outputAttr);
   }

   virtual void Export(AtNode* anode);
   static void* creator()
   {
      return new CMeshTranslator();
   }
   AtNode* CreateArnoldNodes();
private:
   unsigned int GetNumMeshGroups();
};

class MFnMeshData;
class MTesselationParams;
class MFnNurbsSurface;

class DLLEXPORT CNurbsSurfaceTranslator : public CGeoTranslator
{
public:
   virtual void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode, AtUInt step);
   virtual void IsGeoDeforming();

   static void* creator()
   {
      return new CNurbsSurfaceTranslator();
   }
   AtNode* CreateArnoldNodes();
private:
   MObject m_data_mobj;
   bool Tessellate(MDagPath & dagPath);
   void GetTessellationOptions(MTesselationParams & params,
                        MFnNurbsSurface & surface);

};

#endif // GEOMETRY_H
