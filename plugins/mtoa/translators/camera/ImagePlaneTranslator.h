#pragma once

#include "translators/NodeTranslator.h"



class DLLEXPORT CImagePlaneTranslator
   :   public CNodeTranslator
{
public:
   void Export(AtNode* imagePlane);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CImagePlaneTranslator();
   }
   AtNode* CreateArnoldNodes();
   
   virtual bool RequiresMotionData() {return false;}
   
protected:


   MString m_camera;
   bool RequiresColorCorrect();

};
