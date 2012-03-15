#include "ObjectSetTranslator.h"


// This translator doesn't create Arnold nodes,
// it's only purpose is to create additionnal attributes on Sets
AtNode*  CObjectSetTranslator::CreateArnoldNodes()
{
   return NULL;
}

void CObjectSetTranslator::Export(AtNode *set)
{
}

void CObjectSetTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya);

   CAttrData data;
   data.name = "aiOverride";
   data.shortName = "ai_override";
   data.type = AI_TYPE_BOOLEAN;

   helper.MakeInput(data);
}


