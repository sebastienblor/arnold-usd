#include "DriverTranslator.h"

AtNode* CDriverTranslator::CreateArnoldNodes()
{
   MString mayaShader = GetMayaNodeTypeName();
   return AddArnoldNode(m_abstract.arnold.asChar(), m_abstract.arnold.asChar());
}

void CDriverTranslator::Export(AtNode *shader)
{
   MStatus status;
   MPlug plug;

   CBaseAttrHelper helper(shader->base_node);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(shader->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (!helper.IsHidden(paramName))
      {
         AtInt paramType = AiParamGetType(paramEntry);

         // attr name name remap
         MString attrName = helper.GetMayaAttrName(paramName);
         ProcessParameter(shader, paramName, paramType, FindMayaObjectPlug(attrName));
      }
   }
}

void CDriverTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   AiMsgDebug("[mtoa] Initializing CDriverTranslator %s, provided by %s",
              arnold.asChar(), provider.asChar());
   // FIXME: remove this hard-wire
   CExtensionAttrHelper helper("aiOptions", nodeEntry);

   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!helper.IsHidden(paramName))
         MObject attr = helper.MakeInput(paramName);
   }
   AiParamIteratorDestroy(nodeParam);

}
