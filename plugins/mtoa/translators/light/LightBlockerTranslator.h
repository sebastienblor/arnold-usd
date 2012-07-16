#ifndef LIGHTBLOCKERTRANSLATOR_H
#define LIGHTBLOCKERTRANSLATOR_H

#include "translators/NodeTranslator.h"

class CLightBlockerTranslator : public CDagTranslator{
public:   
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="");   
   bool RequiresMotionData();   
   static void* creator();   
   AtNode* CreateArnoldNodes();
protected:
   virtual void Export(AtNode* blocker);
};

#endif // LIGHTBLOCKERTRANSLATOR_H
