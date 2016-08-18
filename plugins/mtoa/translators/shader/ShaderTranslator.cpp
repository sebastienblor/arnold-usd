#include "ShaderTranslator.h"
#include "extension/ExtensionsManager.h"
#include "nodes/MayaNodeIDs.h"

#include <maya/MItDependencyGraph.h>
#include <maya/MFnCompoundAttribute.h>

// Auto shader translator
//

/// Add all AOV outputs for this node
/// This is accomplished by detecting connections from the current node to the aiCustomAOV
/// attribute of a shadingEngine node.  We then create an AOV writing node for each connection.
AtNode* CShaderTranslator::ProcessAOVOutput(AtNode* shader)
{
   // FIXME: add early bail out if AOVs are not enabled
   AtNode* currNode = shader;
   MStatus stat;
   MObject node;
   int outType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(shader));
   MString nodeType = GetAOVNodeType(outType);
   if (!nodeType.numChars())
   {
      AiMsgDebug("[mtoa] Shader %s does not output a supported AOV data type",
                   GetMayaNodeName().asChar());
      return shader;
   }

   MFnDependencyNode fnNode(GetMayaObject());
   MPlugArray destPlugs;
   MPlugArray sourcePlugs;
   fnNode.getConnections(sourcePlugs);
   // loop through the source plugs of all connections on this node
   for (unsigned int i=0; i < sourcePlugs.length(); ++i)
   {
      // TODO: determine if the following two checks make performance faster or slower:
      MPlug resolvedPlug;
      if (!ResolveOutputPlug(sourcePlugs[i], resolvedPlug)) continue;
      // if (sourcePlugs[i].isDestination()) continue;

      sourcePlugs[i].connectedTo(destPlugs, false, true);
      // loop through the destinations, looking for connections to shadingEngines
      for (unsigned int j=0; j < destPlugs.length(); ++j)
      {
         MPlug sgPlug = destPlugs[j];
         if (!sgPlug.node().hasFn(MFn::kShadingEngine))
            continue;
         // ensure that our special aiCustomAOV shading plug is destination
         MStatus status;
         MPlug parent =  sgPlug.parent(&status);
         if (status != MS::kSuccess)
            continue;
         if (parent.isElement() && parent.array().partialName(false, false, false, false, false, true) == "aiCustomAOVs")
         {
            // aiCustomAOVs is a compound with two children: "name" is child 0, "input" is child 1
            MString aovName = parent.child(0).asString();
            if (aovName == "")
            {
               AiMsgError("[mtoa.aov] %-30s | %s is set to empty string",
                          GetMayaNodeName().asChar(), parent.child(0).partialName(true, false, false, false, false, true).asChar());
               continue;
            }
            CAOV aov;
            aov.SetName(aovName);
            if (!m_session->IsActiveAOV(aov))
               continue;
            m_localAOVs.insert(aov);
            m_aovShadingGroups[aovName.asChar()].append(sgPlug);
         }
      }
   }
   // at this stage, we only care about the aov names, which are the key in the map
   std::map<std::string, MPlugArray>::const_iterator it;
   for (it=m_aovShadingGroups.begin(); it!=m_aovShadingGroups.end(); it++)
   {
      const char* aovName = it->first.c_str();
      AiMsgDebug("[mtoa.translator.aov] %-30s | adding AOV write node for \"%s\"",
                 GetMayaNodeName().asChar(), aovName);
      AtNode* writeNode = AddArnoldNode(nodeType.asChar(), aovName);

      AiNodeSetStr(writeNode, "aov_name", aovName);

      MPlugArray plugs = it->second;
      /*
       * this is the more "pure" way of doing things, because we set a node pointer
       * instead of a node name as a string (as below).  however, in order to get the
       * node pointer we must export all shading engines here (otherwise we have no
       * guarantee that a node pointer even exists yet), which causes problems.
      AtArray* sets = AiArrayAllocate(plugs.length(), 1, AI_TYPE_NODE);
      for (unsigned int i = 0; i < plugs.length(); i++)
      {
         MFnDependencyNode fnNode(plugs[i].node());
         MPlug plug = fnNode.findPlug("dagSetMembers");
         AtNode* node = m_session->ExportNode(plug);
         AiArraySetPtr(sets, i, node);
      }
      */
      AtArray* sets = AiArrayAllocate(plugs.length(), 1, AI_TYPE_STRING);
      for (unsigned int i = 0; i < plugs.length(); i++)
      {
         MFnDependencyNode fnNode(plugs[i].node());
         MString nodeName = fnNode.name();
         AiArraySetStr(sets, i, nodeName.asChar());
      }
      AiNodeSetArray(writeNode, "sets", sets);

      // link output of currNode to input of writeNode
      AiNodeLink(currNode, "input", writeNode);
      currNode = writeNode;
   }
   return currNode;
}

AtNode* CShaderTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode(m_abstract.arnold.asChar()));
}

/// Associate each AOV writing node with its shadingGroup.
/// This requires that the shadingGroup itself be exported so that we can refer to it
/// by its AtNode pointer.  As a result, entire shading networks may be triggered to
/// export along with this one, before the network's shape is encountered in the DAG walk.
/// That is why this function must be called at the very end of the Export() routine.
void CShaderTranslator::AssociateAOVsWithShadingGroups()
{
   std::map<std::string, MPlugArray>::const_iterator it;

   for (it=m_aovShadingGroups.begin(); it!=m_aovShadingGroups.end(); it++)
   {
      AtArray *sgs = AiArrayAllocate(it->second.length(), 1, AI_TYPE_NODE);
      for (unsigned int i=0; i < it->second.length(); i++)
      {
         // shadingEngines are exported on their dagMembers plug
         MFnDependencyNode fnNode(it->second[i].node());
         MStatus stat;
         MPlug sgPlug = fnNode.findPlug("dagSetMembers", stat);
         CHECK_MSTATUS(stat);
         if (!sgPlug.isNull())
         {
            AtNode* shadingEngine = ExportNode(sgPlug);
            if (shadingEngine != NULL)
            {
               AiArraySetPtr(sgs, i, shadingEngine);
            }
         }
      }
      AtNode* writeNode = GetArnoldNode(it->first.c_str());
      AiNodeSetArray(writeNode, "sets", sgs);
   }
}

void CShaderTranslator::Export(AtNode *shader)
{
   // the node passed in is the root node. we want the node tagged with ""
   shader = GetArnoldNode("");

   CNodeTranslator::Export(shader);

   ExportBump(shader);
   // This routine is not currently used. It could eventually be used in order for
   // MayaShadingEngine.sets to be a node array instead of a string array
   //AssociateAOVsWithShadingGroups();
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
         ProcessConstantArrayElement(AI_TYPE_MATRIX, matrices, GetMotionStep(), FindMayaPlug(paramName));
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

void CShaderTranslator::ExportBump(AtNode* shader)
{
   MStatus status;
   MPlugArray connections;
   MPlug plug = FindMayaPlug("normalCamera", &status);
   if (status && !plug.isNull())
   {
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* bump = ExportNode(connections[0]);

         if (bump != NULL)
         {
            m_atNode = bump;
            while (true)
            {
               AtNode* connectedBump = AiNodeGetLink(bump, "shader");
               if (connectedBump != 0 && AiNodeIs(connectedBump, "mayaBump2D"))
                  bump = connectedBump;
               else
                  break;
            }
            AiNodeLink(shader, "shader", bump);            
         }
      }
   }
}
