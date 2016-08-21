#pragma once

#include "translators/DagTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   // do we really need to re-define this function ?
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      CDagTranslator::Init(session, dagPath, outputAttr);
      return GetArnoldRootNode();
   }
   virtual bool RequiresMotionData();
   static AtRGB ConvertKelvinToRGB(float kelvin);
protected:

   virtual void Export(AtNode* light);
   virtual void ExportMotion(AtNode* light);
   virtual void Delete();
   virtual bool IsFinite() const { return true; } // to decide if scaling is required or not
   static void MakeCommonAttributes(CBaseAttrHelper& helper);   
};
