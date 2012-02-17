#include "ShaderTranslator.h"
#include "extension/ExtensionsManager.h"
#include "nodes/MayaNodeIDs.h"

#include <maya/MPlugArray.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFnCompoundAttribute.h>

// Auto shader translator
//

/// Add all AOV outputs for this node
AtNode* CShaderTranslator::ProcessAOVOutput(AtNode* shader)
{
   AtNode* currNode = shader;
   MStatus stat;
   MObject node;
   int outType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(shader));
   MString nodeType = GetAOVNodeType(outType);
   if (!nodeType.numChars())
   {
      AiMsgWarning("[mtoa] Shader %s does not output a supported AOV data type",
                   GetMayaNodeName().asChar());
      return shader;
   }

   MPlugArray destPlugs;
   MPlugArray sourcePlugs;
   m_fnNode.getConnections(sourcePlugs);
   for (unsigned int i=0; i < sourcePlugs.length(); ++i)
   {
      // TODO: determine if the following two checks make performance faster or slower:
      MPlug resolvedPlug;
      if (!ResolveOutputPlug(sourcePlugs[i], resolvedPlug)) continue;
      // if (sourcePlugs[i].isDestination()) continue;

      sourcePlugs[i].connectedTo(destPlugs, false, true);
      for (unsigned int j=0; j < destPlugs.length(); ++j)
      {
         MPlug sgPlug = destPlugs[j];
         if (!sgPlug.node().hasFn(MFn::kShadingEngine))
            continue;
         // assert shading plug is destination
         MStatus status;
         MPlug parent =  sgPlug.parent(&status);
         if (status != MS::kSuccess)
            continue;
         if (parent.isElement() && parent.array().partialName(false, false, false, false, false, true) == "aiCustomAOVs")
         {
            // aiCustomAOVs is a compound with two children: name is child 0, input is child 1
            MString aovName = parent.child(0).asString();
            if (aovName == "")
            {
               AiMsgError("[mtoa.aov] %-30s | %s is set to empty string",
                          GetMayaNodeName().asChar(), parent.child(0).partialName(true, false, false, false, false, true).asChar());
               continue;
            }
            AiMsgDebug("[mtoa.aov] %-30s | writing AOV \"%s\"", GetMayaNodeName().asChar(), aovName.asChar());
            AtNode* writeNode = AddArnoldNode(nodeType.asChar(), aovName.asChar());

            AiNodeSetStr(writeNode, "aov_name", aovName.asChar());

            // link output of currNode to input of writeNode
            AiNodeLink(currNode, "input", writeNode);
            currNode = writeNode;
            /*
            MFnDependencyNode fnNode(sgPlug.node());
            MPlug plug = fnNode.findPlug("surfaceShader");
            MPlugArray connections;
            // get incoming connections to surfaceShader plug
            plug.connectedTo(connections, true, false);
            if (connections.length() > 0)
            {
               AtNode* node = m_session->ExportNode(connections[0]);
               cout << node << endl;
            }*/
         }
      }
   }
   return currNode;
}

/// Add a dynamic array attribute for custom AOVs
MStatus CShaderTranslator::MakeAOVAttributes(MObject& node)
{
   MFnDependencyNode fnNode(node);

   MFnCompoundAttribute cmpAttr;
   MFnMessageAttribute mAttr;
   MFnTypedAttribute tAttr;
   MFnNumericAttribute nAttr;

   MStatus status;

   MObject aName = tAttr.create("aovName", "aov_name", MFnData::kString, &status);
   CHECK_MSTATUS(status);
   tAttr.setStorable(true);
   tAttr.setReadable(true);
   tAttr.setWritable(true);

   MObject aNode = mAttr.create("aovInput", "aov_input", &status);
   CHECK_MSTATUS(status);
   mAttr.setReadable(true);
   mAttr.setWritable(true);

   MObject aAOV = cmpAttr.create("aiCustomAOVs", "aovs", &status);
   cmpAttr.setArray(true);
   cmpAttr.addChild(aName);
   cmpAttr.addChild(aNode);

   cmpAttr.setReadable(true);
   cmpAttr.setWritable(true);
   // cmpAttr.setIndexMatters(true);
   CHECK_MSTATUS(fnNode.addAttribute(aAOV));

   return status;
}

AtNode* CShaderTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode(m_abstract.arnold.asChar()));
}

void CShaderTranslator::Export(AtNode *shader)
{
   // the node passed in is the root node. we want the node tagged with ""
   shader = GetArnoldNode("");
   MStatus status;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(shader));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0) ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);

   MPlugArray connections;

   MPlug plug = FindMayaObjectPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      AtNode* bump = ExportNode(connections[0]);

      if (bump != NULL)
      {
         AiNodeLink(shader, "shader", bump);
         SetArnoldRootNode(bump);
      }
   }
}

void CShaderTranslator::ExportMotion(AtNode *shader, unsigned int step)
{
   MStatus status;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(shader));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      // FIXME: introduce "xform" metadata to explicitly mark a matrix parameter
      if (strcmp(paramName, "placementMatrix") == 0)
      {
         AtArray* matrices = AiNodeGetArray(GetArnoldNode(paramName), "values");
         ProcessConstantArrayElement(AI_TYPE_MATRIX, matrices, GetMotionStep(), FindMayaObjectPlug(paramName));
      }
   }
   AiParamIteratorDestroy(nodeParam);
}

bool CShaderTranslator::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   MStatus status;
   MFnAttribute fnAttr(outputPlug.attribute());
   MString attrName = outputPlug.partialName(false, false, false, false, false, true);
   if (fnAttr.type() == MFn::kMessageAttribute)
   {
      // for basic shaders with a single output, which this translator represents, message attributes are equivalent
      // to outColor/outValue
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
   // proper outputs are readable and not writable, but we should only check for those nodes created by mtoa
   // since maya nodes do not strictly adhere to this (surfaceShader.outColor is writable, for example)
   else if ((!fnAttr.isReadable() || fnAttr.isWritable()) &&
         CExtensionsManager::IsRegisteredMayaNode(MFnDependencyNode(outputPlug.node()).typeName()))
   {
      return false;
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

bool CShaderTranslator::RequiresMotionData()
{
   return IsMotionBlurEnabled(MTOA_MBLUR_SHADER);
}
