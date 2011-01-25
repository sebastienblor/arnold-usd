#ifndef HAIR_H
#define HAIR_H

#include "scene/NodeTranslator.h"

class CHairLine
{
public:
   CHairLine() {}
   ~CHairLine() {}
   void SetCurvePoints( MVectorArray points ) { curvePoints = points; };
   void SetCurveWidths( MDoubleArray widths ) { curveWidths = widths; };
   MVectorArray GetCurvePoints() const { return curvePoints; };
   MDoubleArray GetCurveWidths() const { return curveWidths; };
private:
   MVectorArray curvePoints;
   MDoubleArray curveWidths;
};

class CHairTranslator
   :   public CDagTranslator
{
public:
   void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;

      m_numMainLines = 0;
   }
   bool RequiresMotionData()
   {
      return true;
   }
   AtNode* Export();
   void Update(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MObject& node) {}
   static void* creator()
   {
      return new CHairTranslator();
   }
private:
   std::vector<CHairLine> m_hairLines;
   AtUInt m_numMainLines;

   AtUInt GetHairLines(MObject& hair, std::vector<CHairLine>& hairLines);
};

#endif // HAIR_H
