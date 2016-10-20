#pragma once

#include "translators/DagTranslator.h"

class CLightPortalTranslator : public CDagTranslator{
public:   
   bool RequiresMotionData();   
   static void* creator();   
   AtNode* CreateArnoldNodes();
protected:
   virtual void Export(AtNode* blocker);
};
