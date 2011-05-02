#ifndef HAIR_H
#define HAIR_H

#include "scene/Geometry.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CHairLine
{
public:
   void SetCurvePoints(MVectorArray &points) { curvePoints = points; }
   void SetCurveWidths(MDoubleArray &widths) { curveWidths = widths; }
   void GetCurvePoints(MVectorArray &points) const { points = curvePoints; }
   void GetCurveWidths(MDoubleArray &widths) const { widths = curveWidths; }
private:
   MVectorArray curvePoints;
   MDoubleArray curveWidths;
};

class DLLEXPORT CHairTranslator
   :   public CGeoTranslator
{
public:
   CHairTranslator()
      :m_numMainLines(0)
   {}
   virtual void Export(AtNode* camera);
   virtual void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CHairTranslator();
   }
   AtNode* CreateArnoldNodes();
private:
   std::vector<CHairLine> m_hairLines;
   AtUInt m_numMainLines;

   AtVector2 GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   AtUInt GetHairLines(MObject& hair, std::vector<CHairLine>& hairLines);
};

#endif // HAIR_H
