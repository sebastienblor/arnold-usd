#include "ShaderTranslator.h"
#include "ShaderTranslatorImpl.h"
#include "extension/ExtensionsManager.h"
#include "nodes/MayaNodeIDs.h"
#include "session/ArnoldSession.h"
#include <maya/MItDependencyGraph.h>
#include <maya/MFnCompoundAttribute.h>

void CShaderTranslator::CreateImplementation()
{
   m_impl = new CShaderTranslatorImpl(*this);
}

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
      if (!m_impl->ResolveOutputPlug(sourcePlugs[i], resolvedPlug)) continue;
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
            if (!m_impl->m_session->IsActiveAOV(aov))
               continue;
            m_impl->m_localAOVs.insert(aov);
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
   return ProcessAOVOutput(AddArnoldNode(m_impl->m_abstract.arnold.asChar()));
}

void CShaderTranslator::Export(AtNode *shader)
{
   CNodeTranslator::Export(shader);
   ExportBump(shader);
}

   // For motion blur we just search for the parameter placementMatrix
void CShaderTranslator::ExportMotion(AtNode *shader)
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
         m_impl->ProcessConstantArrayElement(AI_TYPE_MATRIX, matrices, GetMotionStep(), FindMayaPlug(paramName));
      }
   }
   AiParamIteratorDestroy(nodeParam);
}

bool CShaderTranslator::RequiresMotionData()
{
   return IsMotionBlurEnabled(MTOA_MBLUR_SHADER);
}

void CShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   CNodeTranslator::NodeChanged(node, plug);

   if (m_impl->m_sourceTranslator == NULL) return;

   // if my connection to "normalCamera" changes, for example if I disconnect a bump node,
   // I not only need to re-export myself, but I also need to advert the node which is connected to the bump
   // (for example the ShadingEngine). Otherwise it will keep its connection to the bump
   
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);

   if (plugName == ".normalCamera")
   {
      // simply delete the sourceTranslator, it will be re-generated at next export
      // this way if it's disconnected we delete it from the scene
      // This might be a bit heavy but it will only happen when user tweaks the bump node
      m_impl->m_sourceTranslator->SetUpdateMode(AI_DELETE_NODE);
      m_impl->m_sourceTranslator->RequestUpdate();
   }
}

   // Maya:
   //  ----------       -----------
   //  | Bump2d | --->  | Shader1 |
   //  ---------- \     -----------
   //              \    -----------
   //               \-> | Shader2 |
   //                   -----------
   //             __
   //             ||
   //            _||_
   //            \  /
   //             \/
   //
   // Arnold:
   //  -----------     ---------
   //  | Shader1 | --> | Bump1 |
   //  -----------     ---------
   //  -----------     ---------
   //  | Shader2 | --> | Bump2 |
   //  -----------     ---------
   //

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
         // ugly way to get a unique instance number integer from this translator's pointer.
         // we should have a better system, like a map that increases an index whenever a new entry is added, or something....
         size_t instNum64 = (size_t)this;
         int instanceNumber = (int)(instNum64/8);

         CNodeTranslator *bumpTranslator = m_impl->m_session->ExportNode(connections[0], m_impl->m_shaders, &m_impl->m_upstreamAOVs, false, instanceNumber);
         
         if (bumpTranslator != NULL)
         {
            m_impl->m_sourceTranslator = bumpTranslator;

#ifdef NODE_TRANSLATOR_REFERENCES 
            m_impl->AddBackReference(bumpTranslator, true); // "true" in order to add the reverse connection too 
#endif
            AtNode* bump = bumpTranslator->GetArnoldNode();
      
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

bool CShaderTranslatorImpl::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   // If this is a multi-output shader, just copy the MPlug
   if (m_tr.DependsOnOutputPlug()) 
   {
      resolvedOutputPlug=outputPlug;
      return true;
   }

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