#pragma once

#include "translators/NodeTranslator.h"



class DLLEXPORT CImagePlaneTranslator
   :   public CNodeTranslator
{
public:
   void Export(AtNode* imagePlane);
   void ExportMotion(AtNode* imagePlane);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CImagePlaneTranslator();
   }
   AtNode* CreateArnoldNodes();
   void SetCamera(MString cameraName);
   
   virtual bool RequiresMotionData();
   
   void ExportImagePlane();
protected:


   MString m_camera;
   MString m_colorSpace;
};
