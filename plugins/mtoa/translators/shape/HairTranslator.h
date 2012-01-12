#ifndef HAIRTRANSLATOR_H
#define HAIRTRANSLATOR_H

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CHairLine
{
public:
   void SetCurvePoints(MVectorArray &points) { curvePoints = points; }
   void SetCurveWidths(MDoubleArray &widths) { curveWidths = widths; }
   void SetCurveColors(MVectorArray &colors) { curveColors = colors; }
   void SetCurveRootUV(float2 &uv) { root_uv[0] = uv[0]; root_uv[1] = uv[1]; }
   void GetCurvePoints(MVectorArray &points) const { points = curvePoints; }
   void GetCurveWidths(MDoubleArray &widths) const { widths = curveWidths; }
   void GetCurveColors(MVectorArray &colors) { colors = curveColors; }
   void GetCurveRootUV(float2 &uv) const { uv[0] = root_uv[0]; uv[1] = root_uv[1]; }
   void clear()
   {
      curvePoints.clear();
      curveWidths.clear();
      curveColors.clear();
   }

private:
   MVectorArray curvePoints;
   MDoubleArray curveWidths;
   MVectorArray curveColors;
   float2       root_uv;
};

class CHairTranslator
   :   public CGeometryTranslator
{
public:
   CHairTranslator() :
      CGeometryTranslator(),
      m_numMainLines(0)
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
   void ProcessHairLines(unsigned int step,
                         AtArray* curvePoints,
                         AtArray* curveNextLineStartsInterp,
                         AtArray* curveNextLineStarts,
                         AtArray* curveWidths,
                         AtArray* curveColors);
   AtVector2 GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   unsigned int GetHairLines(MObject& hair, std::vector<CHairLine>& hairLines);
   void clear()
   {
      m_numMainLines = 0;
      m_hairLines.clear();
   }

private:
   std::vector<CHairLine> m_hairLines;
   unsigned int m_numMainLines;

   
};

#endif // HAIRTRANSLATOR_H
