#include "translators/shape/ShapeTranslator.h"
#include <maya/shaveAPI.h>
#include <maya/shaveItHair.h>

class CShaveTranslator
   :   public CShapeTranslator
{
public:

   virtual void Export(AtNode* camera);
   virtual void ExportMotion(AtNode* curve);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CShaveTranslator();
   }

private:

   AtNode* CreateShaveShader(AtNode* curve);
   void ProcessHairLines(const unsigned int step,
                         AtArray* curvePoints,
                         AtArray* curveNumPoints,
                         AtArray* curveWidths);
   MStatus UpdateHairInfo();
   shaveAPI::HairInfo m_hairInfo;
};
