#ifndef CURVETRANSLATOR_H
#define CURVETRANSLATOR_H

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CCurveLine
{
public:
   void SetCurvePoints(MVectorArray &points) { curvePoints = points; }
   void SetCurveWidths(MDoubleArray &widths) { curveWidths = widths; }
   void SetCurveColors(MVectorArray &colors) { curveColors = colors; }
   void GetCurvePoints(MVectorArray &points) const { points = curvePoints; }
   void GetCurveWidths(MDoubleArray &widths) const { widths = curveWidths; }
   void GetCurveColors(MVectorArray &colors) { colors = curveColors; }
   void clear()
   {
      curvePoints.clear();
      curveWidths.clear();
      curveColors.clear();
   }
   unsigned int curveNumPoints;
   unsigned int curveNumPointsInterp;

private:
   MVectorArray curvePoints;
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
                         AtArray* curveWidths,
                         AtArray* curveColors);
   MStatus GetCurveLines(MObject& curve);


private:

   CCurveLine mayaCurve;


};

#endif // CURVETRANSLATOR_H
