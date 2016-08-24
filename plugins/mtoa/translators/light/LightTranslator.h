#pragma once

#include "translators/DagTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   virtual bool RequiresMotionData();
   static AtRGB ConvertKelvinToRGB(float kelvin);
protected:

   virtual void Export(AtNode* light);
   virtual void ExportMotion(AtNode* light);
   virtual bool IsFinite() const { return true; } // to decide if scaling is required or not
   static void MakeCommonAttributes(CBaseAttrHelper& helper);   
};
