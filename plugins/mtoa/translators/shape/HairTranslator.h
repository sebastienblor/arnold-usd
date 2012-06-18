#ifndef HAIRTRANSLATOR_H
#define HAIRTRANSLATOR_H

#include "GeometryTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

class CHairLine
{
public:

   void SetCurvePoints(MVectorArray &points) { m_curvePoints = points; }
   void SetCurveWidths(MDoubleArray &widths) { m_curveWidths = widths; }
   void SetCurveColors(MVectorArray &colors) { m_curveColors = colors; }
   void SetCurveRootUV(float2 &uv) { m_root_uv[0] = uv[0]; m_root_uv[1] = uv[1]; }
   void GetCurvePoints(MVectorArray &points) const { points = m_curvePoints; }
   void GetCurveWidths(MDoubleArray &widths) const { widths = m_curveWidths; }
   void GetCurveColors(MVectorArray &colors) { colors = m_curveColors; }
   void GetCurveRootUV(float2 &uv) const { uv[0] = m_root_uv[0]; uv[1] = m_root_uv[1]; }
   void clear()
   {
      m_curvePoints.clear();
      m_curveWidths.clear();
      m_curveColors.clear();
   }
  
private:
   MVectorArray m_curvePoints;
   MDoubleArray m_curveWidths;
   MVectorArray m_curveColors;
   float2       m_root_uv;
};

class CHairLines
{
   public:
      CHairLines() :
         m_numPoints(0),
         m_numPointsInterpolation(0)
      {
      }
      
      std::vector<CHairLine>& get()
      {
         return m_hairLines;
      }
      
      unsigned int m_numPoints;
      unsigned int m_numPointsInterpolation;   
      std::vector<CHairLine> m_hairLines;      
};

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
   virtual void GetMatrix(AtMatrix& matrix);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CHairTranslator();
   }

   
private:
   void ProcessHairLines(const unsigned int step,
                         AtArray* curvePoints,
                         AtArray* curveWidths,
                         AtArray* curveColors,
                         AtArray* curveUParamCoord = NULL,
                         AtArray* curveVParamCoord = NULL);
                         
   AtVector2 GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh);
   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes);
   unsigned int GetHairLines(MObject& hair, CHairLines& hairLines);
   void clear()
   {
      m_hairLines.get().clear();
   }

private:
   CHairLines       m_hairLines;
   MObject          m_hairInfo;
   bool             m_export_curve_uvs;
   bool             m_export_curve_id;
   MMeshIntersector m_meshInt;
   MFnMesh          m_mesh;
   bool             m_hasConnectedShapes;
   MDagPath         m_pfxHairPath;
   
};

#endif // HAIRTRANSLATOR_H
