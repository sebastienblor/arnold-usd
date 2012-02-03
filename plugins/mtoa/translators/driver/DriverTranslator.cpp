#include "DriverTranslator.h"
#include "utils/Universe.h"

#include <assert.h>
#include <ai_universe.h>

AtNode* CDriverTranslator::CreateArnoldNodes()
{
   assert(AiUniverseIsActive());

   MString mayaShader = GetMayaNodeTypeName();
   AtNode* created = AddArnoldNode(m_abstract.arnold.asChar(), m_abstract.arnold.asChar());
   return created;
}

void CDriverTranslator::Export(AtNode *shader)
{
   assert(AiUniverseIsActive());

   MStatus status;
   MPlug plug;

   const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(shader);

   CBaseAttrHelper helper(nodeEntry);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      int paramType = AiParamGetType(paramEntry);
      ProcessParameter(shader, paramName, paramType);
   }
   AiParamIteratorDestroy(nodeParam);
}

void CDriverTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   // FIXME: remove this hard-wire
   CExtensionAttrHelper helper("aiOptions", nodeEntry);
   CExtensionAttrHelper helper2("aiAOV", nodeEntry);
   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!helper.IsHidden(paramName))
      {
         helper.MakeInput(paramName);
         helper2.MakeInput(paramName);
      }
   }
   AiParamIteratorDestroy(nodeParam);

}

// No callbacks currently
void CDriverTranslator::AddUpdateCallbacks()
{
}

void CDriverTranslator::RemoveUpdateCallbacks()
{
}

