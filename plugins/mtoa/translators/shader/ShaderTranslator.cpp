#include "ShaderTranslator.h"

#include <maya/MPlugArray.h>

// Auto shader translator
//
AtNode* CShaderTranslator::Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr)
{
   m_motion = session->IsMotionBlurEnabled(MTOA_MBLUR_SHADER);

   return CNodeTranslator::Init(session, dagPath.node(), outputAttr);
}

AtNode* CShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}

void CShaderTranslator::Export(AtNode *shader)
{
   MStatus status;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(shader->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0) ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }

   MPlugArray connections;

   MPlug plug = FindMayaObjectPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      AtNode* bump = ExportNode(connections[0]);

      if (bump != NULL)
         AiNodeLink(bump, "@before", shader);
   }
}

bool CShaderTranslator::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   MStatus status;
   MFnAttribute fnAttr(outputPlug.attribute());
   MString attrName = outputPlug.partialName(false, false, false, false, false, true);
   if (fnAttr.type() == MFn::kMessageAttribute)
   {
      // for basic shaders with a single output, which this translator represents, message attributes are equivalent to outColor/outValue
      MFnDependencyNode fnNode(outputPlug.node());
      resolvedOutputPlug = fnNode.findPlug("outColor", &status);
      if (status != MS::kSuccess)
      {
         resolvedOutputPlug = fnNode.findPlug("outValue", &status);
         if (status != MS::kSuccess)
         {
            AiMsgError("[mtoa] Cannot resolve message attribute \"%s\" to a valid shader output (e.g. outColor/outValue)",
                       outputPlug.partialName(true, false, false, false, false, true).asChar());
            return false;
         }
      }
   }
   /*
   // FIXME: can't use this because of built-in maya shaders which may use output3D, and other craziness
   else if (attrName != "outColor" && attrName != "outValue")
   {
      AiMsgError("[mtoa] Cannot export \"%s\" because it is not a valid output attribute",
                 outputPlug.partialName(true, false, false, false, false, true).asChar());
      return false;
   }*/
   else
      resolvedOutputPlug=outputPlug;
   return true;
}
