#include "AutoProceduralTranslator.h"
#include "translators/NodeTranslatorImpl.h"

// Auto Shape (procedural) translator
//

AtNode* CAutoProceduralTranslator::CreateArnoldNodes()
{
   if (IsMasterInstance())
      return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
   else
      return AddArnoldNode("ginstance");
}


void CAutoProceduralTranslator::Export(AtNode* node)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(node));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(node, GetMasterInstance());
   }
   else
   {
      // First export generic procedural parameters from ProceduralTranslator
      ExportProcedural(node);

      // Now export the arnold-specific attributes
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         std::string paramNameStr(paramName);

         if (paramNameStr == "name" || paramNameStr == "visibility" || paramNameStr == "matrix" || paramNameStr == "trace_sets")
            continue;
         
         ProcessParameter(node, paramName, AiParamGetType(paramEntry));
      }
      AiParamIteratorDestroy(nodeParam);
   }

}
