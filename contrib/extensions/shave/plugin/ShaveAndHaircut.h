#include "scene/NodeTranslator.h"
#include <maya/shaveAPI.h>
#include <maya/shaveItHair.h>

class CShaveTranslator
   :   public CDagTranslator
{
public:

   void Export(AtNode* camera);
   void Update(AtNode* curve);
   void UpdateMotion(AtNode* curve, AtUInt step);
   static void* creator()
   {
      return new CShaveTranslator();
   }
   static void NodeInitializer(MString nodeClassName);
   const char* GetArnoldNodeType();

private:
   void ProcessHairLines(AtUInt step, AtArray* curvePoints, AtArray* curveNextLineStartsInterp, AtArray* curveNextLineStarts, AtArray* curveWidths);
   MStatus SetHairInfo();
   shaveAPI::HairInfo m_hairInfo;
};

