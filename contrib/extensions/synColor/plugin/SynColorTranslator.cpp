#include "SynColorTranslator.h"

#include "utils/time.h"
#include <iostream>
void CSynColorTranslator::Export(AtNode* node)
{
   printf("PH ==> CSynColorTranslator::Export()\n");
   
   // Can there be multiple color management nodes in the scene ?
   AtNode *options = AiUniverseGetOptions();
   AiNodeSetPtr(options, "color_manager", (void*)node);

}

void CSynColorTranslator::NodeInitializer(CAbTranslator context)
{
   printf("PH ==> CSynColorTranslator::NodeInitializer()\n");
}

AtNode* CSynColorTranslator::CreateArnoldNodes()
{
   printf("PH ==> CSynColorTranslator::CreateArnoldNodes()\n");
   return AddArnoldNode("synColor_color_manager");
}


