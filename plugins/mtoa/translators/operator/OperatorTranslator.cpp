#include "OperatorTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include <maya/MPlugArray.h>

AtNode* COperatorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());   
}

void COperatorTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);

      if (paramNameStr != "name" && paramNameStr != "inputs")
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);

   // Deal with inputs now
   MPlug inputs = FindMayaPlug("inputs");
   unsigned nelems = inputs.numElements();
   MPlug elemPlug;
   AtArray* array = AiArrayAllocate(nelems, 1, AI_TYPE_STRING);
      
   for (unsigned int i = 0; i < nelems; ++i)
   {
      elemPlug = inputs[i];       
      
      MPlugArray connections;
      elemPlug.connectedTo(connections, true, false);
      AtNode* linkedNode = NULL;
      if (connections.length() > 0)
      {
         linkedNode = ExportConnectedNode(connections[0]);
      }
      if (linkedNode)
      {
         const char *elemName = AiNodeGetName(linkedNode);
         AiArraySetStr(array, i, elemName);
      }
   }
   AiNodeSetArray(shader, "inputs", array);
}

void COperatorTranslator::NodeInitializer(CAbTranslator context)
{   
}
