#pragma once

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

struct CCurveLine
{
   CCurveLine() : widthConnected(false)
   {

   }
   void clear()
   {
      points.clear();
      referencePoints.clear();
      widths.clear();
      widthConnected = false;
   }
   std::vector<AtVector> points;
   std::vector<AtVector> referencePoints;
   std::vector<float> widths;
   AtRGB color;
   bool widthConnected;
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
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
private:
   void ProcessCurveLines(unsigned int step,
                         AtArray* curvePoints,
                         AtArray* referenceCurvePoints,
                         AtArray* curveWidths,
                         AtArray* curveColors);
   MStatus GetCurveLines(MObject& curve, unsigned int step);


private:

   CCurveLine mayaCurve;
   bool exportReferenceObject;


};
