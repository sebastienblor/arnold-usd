#include "FilterTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "utils/ConstantStrings.h"
#include <assert.h>

AtNode* CFilterTranslator::CreateArnoldNodes()
{
   const char *arnoldName = m_impl->m_abstract.arnold.asChar();

   AtNode* created = AddArnoldNode(arnoldName/*, arnoldName*/);

   // we used to set this as the filter's name (using tags)
   // so until we're sure there wasn't a good reason for it I'm keeping this behaviour
   std::string name = AiNodeGetName(created);
   name += "/";
   name += arnoldName;
   AiNodeSetStr(created, str::name, AtString(name.c_str()));

   return created;
}

void CFilterTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const AtString paramName = AiParamGetName(paramEntry);

      if (paramName != str::name)
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);
}

void CFilterTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(AtString(arnold.asChar()));

   CExtensionAttrHelper helper(maya, nodeEntry);
   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!helper.IsHidden(paramName))
      {
         helper.MakeInput(paramName);
      }
   }
   AiParamIteratorDestroy(nodeParam);

}
