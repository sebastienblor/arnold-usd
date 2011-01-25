#include "scene/NodeTranslator.h"
#include <maya/shaveAPI.h>
#include <maya/shaveItHair.h>

class CShaveTranslator
   :   public CDagTranslator
{
public:

   AtNode* Export();
   void Update(AtNode* curve);
   void UpdateMotion(AtNode* curve, AtUInt step);
   static void* creator()
   {
      return new CShaveTranslator();
   }
   static void NodeInitializer(MObject& node);
private:
   void ProcessHairLines(AtUInt step, AtArray* curvePoints, AtArray* curveNextLineStartsInterp, AtArray* curveNextLineStarts, AtArray* curveWidths);
   MStatus SetHairInfo();
   shaveAPI::HairInfo m_hairInfo;
};

