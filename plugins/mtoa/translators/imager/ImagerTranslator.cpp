#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>

AtNode* CImagerTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());   
}

void CImagerTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);

      if (paramNameStr != "name" && paramNameStr != "input")
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);
}
void CImagerTranslator::NodeInitializer(CAbTranslator context)
{   
}

void CImagerTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   CNodeTranslator::NodeChanged(node, plug);
}
