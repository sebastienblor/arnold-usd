#include "DriverTranslator.h"

AtNode* CDriverTranslator::CreateArnoldNodes()
{
   MString mayaShader = GetFnNode().typeName();
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
         plug = GetFnNode().findPlug(attrName, &status);
         if (status == MS::kSuccess)
            ProcessParameter(shader, plug, paramName, paramType);
         else
            AiMsgWarning("[mtoa] [translator %s] Attribute %s.%s requested by translator does not exist",
                  GetName().asChar(), GetFnNode().name().asChar(), attrName.asChar());
      }
   }
}

void CDriverTranslator::NodeInitializer(MString nodeClassName, CNodeInitContext context)
{
   MString maya = context.mayaNodeName;
   MString arnold = context.arnoldNodeName;
   //MString classification = context.classification;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   AiMsgDebug("[mtoa] Initializing CDriverTranslator %s, provided by %s",
              arnold.asChar(), provider.asChar());
   // FIXME: remove this hard-wire
   CExtensionAttrHelper helper("aiOptions", nodeEntry);
   CExtensionAttrHelper helper2("aixAOV", nodeEntry);
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
