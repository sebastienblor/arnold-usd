#include "scene/Geometry.h"
#include <maya/shaveAPI.h>
#include <maya/shaveItHair.h>

class CShaveTranslator
   :   public CGeoTranslator
{
public:

   virtual void Export(AtNode* camera);
   virtual void Update(AtNode* curve);
   virtual void ExportMotion(AtNode* curve, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   const char* GetArnoldNodeType();
   static void* creator()
   {
      return new CShaveTranslator();
   }

private:
   void ProcessHairLines(AtUInt step,
                         AtArray* curvePoints,
                         AtArray* curveNextLineStartsInterp,
                         AtArray* curveNextLineStarts,
                         AtArray* curveWidths);
   MStatus UpdateHairInfo();
   shaveAPI::HairInfo m_hairInfo;
};
