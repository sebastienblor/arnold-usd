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

   /*MString m_prefix = "blaahblahblah";
   if (strcmp(GetMayaNodeName().asChar(), "defaultArnoldRenderOptions") == 0)
   {
      AiMsgDebug("[mtoa] [driver] general driver");
      //ProcessParameter(shader, "filename", AI_STRING, srcNodeFn.findPlug("coverage"));
      m_prefix = "ririroro";
   }
   else
   {
      AiMsgDebug("[mtoa] [driver] AOV driver");
      m_prefix = FindMayaObjectPlug("prefix").asString().expandEnvironmentVariablesAndTilde();
      AiMsgError("[mtoa] [driver] %s",m_prefix.asChar());
   }
   AiNodeSetStr(shader, "filename", m_prefix.asChar());*/
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
