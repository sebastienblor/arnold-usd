#ifndef HAIRTRANSLATOR_H
#define HAIRTRANSLATOR_H

#include "GeometryTranslators.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CHairLine
{
public:
   void SetCurvePoints(MVectorArray &points) { curvePoints = points; }
   void SetCurveWidths(MDoubleArray &widths) { curveWidths = widths; }
   void GetCurvePoints(MVectorArray &points) const { points = curvePoints; }
   void GetCurveWidths(MDoubleArray &widths) const { widths = curveWidths; }
   void clear()
   {
      curvePoints.clear();
      curveWidths.clear();
   }

private:
   MVectorArray curvePoints;
   MDoubleArray curveWidths;
};

class CHairTranslator
   :   public CGeoTranslator
{
public:
   CHairTranslator()
      :m_numMainLines(0)
   {}
   
   virtual void Export(AtNode* curve);
   virtual void Update(AtNode* curve);
   virtual void ExportMotion(AtNode* curve, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CHairTranslator();
   }

   
private:
   void ProcessHairLines(AtUInt step,
                         AtArray* curvePoints,
                         AtArray* curveNextLineStartsInterp,
                         AtArray* curveNextLineStarts,
                         AtArray* curveWidths);
   AtVector2 GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   AtUInt GetHairLines(MObject& hair, std::vector<CHairLine>& hairLines);
   void clear()
   {
      m_numMainLines = 0;
      m_hairLines.clear();
   }

private:
   std::vector<CHairLine> m_hairLines;
   AtUInt m_numMainLines;

   
};

#endif // HAIRTRANSLATOR_H
