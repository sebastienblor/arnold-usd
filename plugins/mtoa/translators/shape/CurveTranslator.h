#pragma once

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

struct CCurveLine
{
   void clear()
   {
      points.clear();
      referencePoints.clear();
      widths.clear();
      colors.clear();
   }
   unsigned int curveNumPoints;
   unsigned int curveNumPointsInterp;

   std::vector<AtVector> points;
   std::vector<AtVector> referencePoints;
   std::vector<float> widths;
   std::vector<AtRGB> colors;
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
