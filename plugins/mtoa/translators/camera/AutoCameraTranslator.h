#pragma once

#include "translators/NodeTranslator.h"

class DLLEXPORT CAutoCameraTranslator : public CDagTranslator{
public:
   static void* creator()
   {
      return new CAutoCameraTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual bool RequiresMotionData();
};
