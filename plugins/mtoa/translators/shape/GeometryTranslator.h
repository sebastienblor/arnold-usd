#pragma once

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

class  CPolygonGeometryTranslator : public CShapeTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_displaced = false;
      m_isRefSmooth = false;
      m_useMotionVectors = false;
      m_geometry = dagPath.node();
      return CShapeTranslator::Init(session, dagPath, outputAttr);
   }
   virtual void Export(AtNode* anode);
   static void NodeInitializer(CAbTranslator context);
   
protected:
   virtual bool Tessellate(const MDagPath &dagPath) {return false;}

   bool GetVertices(const MObject& geometry,
         const float*& vertices);
   bool GetPerVertexNormals(const MObject &geometry,
         AtArray*& normals,
         MSpace::Space space=MSpace::kObject,
         bool force=false);
   bool GetNormals(const MObject& geometry,
           const float*& normals);
   bool GetTangents(const MObject &geometry,
         AtArray*& tangents,
         AtArray*& bitangents,
         MSpace::Space space=MSpace::kObject,
         bool force=false);
   bool GetUVs(const MObject &geometry,
               std::vector<AtArray*>& uvs,
               std::vector<MString>& uvNames);
   MDagPath GetMeshRefObj();
   bool GetRefObj(const float*& refVertices,
         AtArray*& refNormals,
         AtArray*& rnidxs,
         AtArray*& refTangents,
         AtArray*& refBitangents);
   bool GetVertexColors(const MObject &geometry,
         std::map<std::string, std::vector<float> > &vcolors);
   bool GetComponentIDs(const MObject &geometry,
         AtArray*& nsides,
         AtArray*& vidxs,
         AtArray*& nidxs,
         std::vector<AtArray*>& uvidxs,
         const std::vector<MString>& uvNames,
         bool exportNormals,
         bool exportUVs);
   
   void GetDisplacement(MObject& obj, 
         float& dispPadding, 
         bool& enableAutoBump);

   MObject GetNodeShader(MObject dagNode, int instanceNum);
   void ExportMeshShaders(AtNode* polymesh, const MDagPath& path);
   virtual void ExportShaders();

   void ExportBBox(AtNode* polymesh);
   void ExportMeshGeoData(AtNode* polymesh);
   void ExportMeshParameters(AtNode* polymesh);
   AtNode* ExportMesh(AtNode* polymesh, bool update);
   void ExportMeshMotion(AtNode* polymesh, unsigned int step);
   AtNode* ExportInstance(AtNode* instance, const MDagPath& masterInstance);
   void ExportInstanceMotion(AtNode* instance, unsigned int step);

   virtual bool IsGeoDeforming();

protected:
   bool m_displaced;
   bool m_isRefSmooth;
   bool m_useMotionVectors;
   MString m_motionVectorSource;
   MObject m_geometry;
   MDagPath m_dagPathRef;
};
