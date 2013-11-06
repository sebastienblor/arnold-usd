#pragma once

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CCurveLine
{
public:
   void SetCurvePoints(const MVectorArray &points) { curvePoints = points; }
   void SetReferenceCurvePoints(const MVectorArray &points) { referenceCurvePoints = points; }
   void SetCurveWidths(const MDoubleArray &widths) { curveWidths = widths; }
   void SetCurveColors(const MVectorArray &colors) { curveColors = colors; }
   const MVectorArray& GetCurvePoints() const { return curvePoints; }
   const MVectorArray& GetReferenceCurvePoints() const { return referenceCurvePoints; }
   const MDoubleArray& GetCurveWidths() const { return curveWidths; }
   const MVectorArray& GetCurveColors() const { return curveColors; }
   unsigned int GetCurvePointsCount() const { return curvePoints.length(); }
   void clear()
   {
      curvePoints.clear();
      referenceCurvePoints.clear();
      curveWidths.clear();
      curveColors.clear();
   }
   unsigned int curveNumPoints;
   unsigned int curveNumPointsInterp;

private:
   MVectorArray curvePoints;
   MVectorArray referenceCurvePoints;
   MDoubleArray curveWidths;
   MVectorArray curveColors;
};

class CCurveTranslator
   :   public CGeometryTranslator
{
public:
   CCurveTranslator() :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "curves";
      exportReferenceObject = false;
   }

   virtual void Export(AtNode* curve);
   virtual void Update(AtNode* curve);
   virtual void ExportMotion(AtNode* curve, unsigned int step);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CCurveTranslator();
   }


private:
   void ProcessCurveLines(unsigned int step,
                         AtArray* curvePoints,
                         AtArray* referenceCurvePoints,
                         AtArray* curveWidths,
                         AtArray* curveColors);
   MStatus GetCurveLines(MObject& curve);


private:

   CCurveLine mayaCurve;
   bool exportReferenceObject;


};
