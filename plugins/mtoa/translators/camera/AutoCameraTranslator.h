#pragma once

#include "CameraTranslator.h"

class DLLEXPORT CAutoCameraTranslator : public CCameraTranslator{
public:
   static void* creator()
   {
      return new CAutoCameraTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
};
