#include "ShaderTranslator.h"


// Auto shader translator
//
AtNode* CShaderTranslator::Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr)
{
   return CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
}

AtNode* CShaderTranslator::CreateArnoldNodes()
{
   MString mayaShader = GetFnNode().typeName();
   if (m_outputAttr == "outAlpha")
   {
      AtNode* node = AddArnoldNode(m_abstract.arnold.asChar(), "base");
      AtNode* alpha = AddArnoldNode("colorToFloat", "alpha");
      AiNodeLink(node, "color", alpha);
      // set colorToFloat output to alpha mode:
      AiNodeSetInt(alpha, "channel", 3);
      return alpha;
   }
   return AddArnoldNode(m_abstract.arnold.asChar());
}

void CShaderTranslator::Export(AtNode *shader)
{
   // the arnold node passed in shader is the root: the node whose output is passed to the next node.
   // if we're in outAlpha mode the root is the alpha conversion node.
   // we don't want to set our parameters on this node, so get the base shader
   if (m_outputAttr == "outAlpha")
      shader = GetArnoldNode("base");

   MStatus status;
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(shader->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name"))
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
            AiMsgWarning("[mtoa] [translator %s] Attribute %s.%s requested by translator does not exist",
                  GetName().asChar(), GetFnNode().name().asChar(), attrName);
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
