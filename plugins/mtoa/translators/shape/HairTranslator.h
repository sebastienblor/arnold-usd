#pragma once

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CHairTranslator
   :   public CShapeTranslator
{
public:
   CHairTranslator() : 
      CShapeTranslator() 
   {}

   virtual void Export(AtNode* curve);
   
   virtual void ExportMotion(AtNode* curve);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CHairTranslator();
   }
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
private:                         
   AtVector2 GetHairRootUVs(const MVector& lineStart, MFnMesh& mesh, MMatrix shapeTransform);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   void GetHairLines(MObject& hair, MRenderLineArray& mainLines, bool firstStep = true);

   MObject          m_hairInfo;
   bool             m_export_curve_uvs;
   MMeshIntersector m_meshInt;
   MFnMesh          m_mesh;
   bool             m_hasConnectedShapes;
   MDagPath         m_pfxHairPath;
   unsigned int     m_numPointsInterpolation;
};