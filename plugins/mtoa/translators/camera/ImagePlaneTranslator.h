#pragma once

#include "translators/NodeTranslator.h"



class DLLEXPORT CImagePlaneTranslator
   :   public CNodeTranslator
{
public:
   void Export(AtNode* imagePlane);
   void ExportMotion(AtNode* imagePlane, unsigned int step);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CImagePlaneTranslator();
   }
   AtNode* CreateArnoldNodes();
   void SetCamera(MString cameraName);
   
   virtual bool RequiresMotionData()
   {
      return m_session->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA) && !m_isStatic;
   }
   void ExportImagePlane(unsigned int step);
protected:


   MString m_camera;
   MString m_colorSpace;
};
