#pragma once

#include "translators/shape/ProceduralTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CBifShapeTranslator
   :   public CProceduralTranslator
{
public:
   CBifShapeTranslator() :
      CProceduralTranslator(),
      m_vpRenderSelect(-1)
   {}

   void Export(AtNode* curve) override;
   void ExportMotion(AtNode* curve) override;
   bool RequiresMotionData() override;
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes() override;
   static void* creator()
   {
      return new CBifShapeTranslator();
   }
   void ExportShaders(AtNode* shape);
   void NodeChanged(MObject& node, MPlug& plug) override; 

protected:
   void PostExport(AtNode *node) override;

   int m_vpRenderSelect;
};
