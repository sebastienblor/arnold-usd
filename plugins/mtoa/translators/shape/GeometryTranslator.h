#ifndef GEOMETRYTRANSLATOR_H
#define GEOMETRYTRANSLATOR_H

#include "ShapeTranslator.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MMeshSmoothOptions.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MColorArray.h>
#include <maya/MNodeMessage.h>

class CGeometryTranslator
   :   public CShapeTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_displaced = false;
      m_isRefSmooth = false;
      return CShapeTranslator::Init(session, dagPath, outputAttr);
   }
   virtual void Update(AtNode* anode);
   virtual void ExportMotion(AtNode* anode, AtUInt step);
   virtual void UpdateMotion(AtNode* anode, AtUInt step);
   static void NodeInitializer(CAbTranslator context);
   virtual void AddUpdateCallbacks();

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
   virtual bool IsGeoDeforming();

protected:
   bool m_isMasterDag;
   bool m_displaced;
   bool m_isRefSmooth;
   MObject m_data_mobj;
   MFnMesh m_fnMesh;
   MFnMesh m_fnMeshRef;
   MDagPath m_masterDag;
};


#endif // GEOMETRYTRANSLATOR_H
