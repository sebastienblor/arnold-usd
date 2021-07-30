#pragma once

#include "ImagerTranslator.h"


class DLLEXPORT CImagerColorCurvesTranslator
   :  public CImagerTranslator
{
public:
   static void* creator()
   {
      return new CImagerColorCurvesTranslator();
   }
   static void NodeInitializer(CAbTranslator context);
   void ExportRampCurve(AtNode *shader, const std::string attr_name);
   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* atNode);

};
