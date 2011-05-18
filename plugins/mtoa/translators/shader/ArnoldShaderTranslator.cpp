#include "ArnoldShaderTranslator.h"


// Auto shader translator
//
AtNode* CArnoldShaderTranslator::Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr)
{
   return CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
}

AtNode* CArnoldShaderTranslator::CreateArnoldNodes()
{
   MString mayaShader = GetFnNode().typeName();
   // return AddArnoldNode(CExtensionsManager::GetArnoldNodeFromMayaNode(mayaShader));
   return AddArnoldNode(m_abstract.arnold.asChar());
}

void CArnoldShaderTranslator::Export(AtNode *shader)
{
   MStatus status;
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(shader->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (!strncmp(paramName, "aov_", 4))
      {
         CRenderSession *renderSession = CRenderSession::GetInstance();
         const CRenderOptions *renderOptions = renderSession->RenderOptions();

         // do not check type for now
         std::string aovName(paramName);
         aovName = aovName.substr(4);
         if (renderOptions->FindAOV(aovName.c_str()) != size_t(-1))
         {
            AiNodeSetStr(shader, paramName, aovName.c_str());
         }
         else
         {
            AiNodeSetStr(shader, paramName, "");
         }
      }
      else if (strcmp(paramName, "name"))
      {
         AtInt paramType = AiParamGetType(paramEntry);

         // attr name name remap
         const char* attrName;
         if (!AiMetaDataGetStr(shader->base_node, paramName, "maya.name", &attrName))
            attrName = paramName;

         plug = GetFnNode().findPlug(attrName, &status);
         if (status == MS::kSuccess)
            ProcessParameter(shader, plug, paramName, paramType);
         else
            AiMsgWarning("Attribute %s.%s requested by translator does not exist", GetFnNode().name().asChar(), attrName);
      }
   }

   MPlugArray connections;

   plug = GetFnNode().findPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* bump = ExportShader(connections[0].node(), attrName);

      if (bump != NULL)
         AiNodeLink(bump, "@before", shader);
   }
}
