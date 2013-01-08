#pragma once

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CHairTranslator
   :   public CShapeTranslator
{
public:
   CHairTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "curves";
   }

   virtual void Export(AtNode* curve);
   virtual void Update(AtNode* curve);
   virtual void ExportMotion(AtNode* curve, unsigned int step);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CHairTranslator();
   }
   
private:                         
   AtVector2 GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   void GetHairLines(MObject& hair, MRenderLineArray& mainLines, bool firstStep = true);

   MObject          m_hairInfo;
   bool             m_export_curve_uvs;
   bool             m_export_curve_id;
   MMeshIntersector m_meshInt;
   MFnMesh          m_mesh;
   bool             m_hasConnectedShapes;
   MDagPath         m_pfxHairPath;
   unsigned int     m_numPointsInterpolation;
};
