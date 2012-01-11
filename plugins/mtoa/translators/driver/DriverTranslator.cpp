#include "DriverTranslator.h"
#include "utils/Universe.h"

#include <assert.h>
#include <ai_universe.h>

AtNode* CDriverTranslator::CreateArnoldNodes()
{
   assert(AiUniverseIsActive());

   MString mayaShader = GetMayaNodeTypeName();
   AtNode* created = AddArnoldNode(m_abstract.arnold.asChar(), m_abstract.arnold.asChar());
   const AtNodeEntry* nodeEntry = created->base_node;

   AiMsgDebug("CDriverTranslator %s: CreateArnoldNodes on Maya node %s(%s) created arnold node %p: %s(%s).",
         GetTranslatorName().asChar(),
         GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
         created, AiNodeGetName(created), AiNodeEntryGetName(nodeEntry));

   m_atNode = created;

   return created;
}

void CDriverTranslator::Export(AtNode *shader)
{
   assert(AiUniverseIsActive());

   MStatus status;
   MPlug plug;

   const AtNodeEntry* nodeEntry = shader->base_node;

   AiMsgDebug("CDriverTranslator %s: Export on Maya node %s(%s), Arnold node %s(%s).",
         GetTranslatorName().asChar(),
         GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
         AiNodeGetName(shader), AiNodeEntryGetName(nodeEntry));

   CBaseAttrHelper helper(nodeEntry);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      int paramType = AiParamGetType(paramEntry);
      AiMsgDebug("CDriverTranslator %s: Export on parameter %s", GetTranslatorName().asChar(), paramName);
      ProcessParameter(shader, paramName, paramType);
   }

}

void CDriverTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());
   if (NULL == nodeEntry)
   {
      AiMsgError("[mtoa] [maya %s] No Arnold driver of name %s is provided by %s",
              maya.asChar(), arnold.asChar(), provider.asChar());
   }
   else
   {
      AiMsgDebug("[mtoa] [maya %s] NodeInitializer for CDriverTranslator %s, provided by %s",
              maya.asChar(), arnold.asChar(), provider.asChar());
   }
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

