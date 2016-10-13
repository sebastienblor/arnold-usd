#include "FilterTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include <assert.h>

/*

   As a quick fix for 1.2.0.3 I'm adding the scaling of the
   distance like parameters here, instead of to the process parameter.
   This HAVE to be removed for 1.2.1.0.

*/

AtNode* CFilterTranslator::CreateArnoldNodes()
{
   assert(AiUniverseIsActive());

   const char *arnoldName = m_impl->m_abstract.arnold.asChar();

   AtNode* created = AddArnoldNode(arnoldName/*, arnoldName*/);

   // we used to set this as the filter's name (using tags)
   // so until we're sure there wasn't a good reason for it I'm keeping this behaviour
   std::string name = AiNodeGetName(created);
   name += "@";
   name += arnoldName;
   AiNodeSetStr(created, "name", name.c_str());

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
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0)
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);
}

void CFilterTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

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

// No callbacks currently, can this maya node be deleted ?
void CFilterTranslator::AddUpdateCallbacks()
{
}
