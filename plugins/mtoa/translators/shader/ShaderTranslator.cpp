#include "ShaderTranslator.h"
#include "ShaderTranslatorImpl.h"
#include "extension/ExtensionsManager.h"
#include "nodes/MayaNodeIDs.h"
#include "session/ArnoldSession.h"
#include <maya/MItDependencyGraph.h>
#include <maya/MFnCompoundAttribute.h>

#include "common/UnorderedContainer.h"
#include "utils/MtoaLog.h"


CShaderTranslator::~CShaderTranslator()
{
}

void CShaderTranslator::CreateImplementation()
{
   m_impl = new CShaderTranslatorImpl(*this);
}

// Auto shader translator
//

// This function should be renamed once we can break binary compatibility
// We no longer need it to process AOVs, but instead we need it to eventually 
// insert a matte shader at the root of the shading tree.
// Maybe we should totally remove this function, and instead derive ShaderTranslatorImpl::DoCreateArnoldNodes()
// so that the necessary things are always done, otherwise all shaders need to call 
// ProcessAOVOutput during CreateArnoldNodes()
AtNode* CShaderTranslator::ProcessAOVOutput(AtNode* shader)
{
   MStatus status;
   MPlug mattePlug = FindMayaPlug("aiEnableMatte", &status);
   if (status != MS::kSuccess || (!mattePlug.asBool()))
      return shader; // matte is disabled (or parameter doesn't exist)


   MPlug matteColorPlug = FindMayaPlug("aiMatteColor", &status); 
   if (status != MS::kSuccess ) return shader;

   MPlug matteColorAPlug = FindMayaPlug("aiMatteColorA", &status); 
   if (status != MS::kSuccess ) return shader;

   // Matte is enabled, I need to insert a matte shader at the root of my shading tree
   AtNode *matteShader = GetArnoldNode("_matte");
   if (matteShader == NULL)
      matteShader = AddArnoldNode("matte", "_matte");

   AiNodeSetRGBA(matteShader, "color",
                matteColorPlug.child(0).asFloat(),
                matteColorPlug.child(1).asFloat(),
                matteColorPlug.child(2).asFloat(),
                matteColorAPlug.asFloat());

   AiNodeLink(shader, "passthrough", matteShader); 

   return matteShader;
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
   // if my connection to "normalCamera" changes, for example if I disconnect a bump node,
   // I not only need to re-export myself, but I also need to advert the node which is connected to the bump
   // (for example the ShadingEngine). Otherwise it will keep its connection to the bump
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (plugName == "aiEnableMatte" || plugName == "aiMatteColor" || plugName == "aiMatteColorA" )
      SetUpdateMode(AI_RECREATE_NODE); // I need to re-generate the shaders, so that they include the matte at the root of the shading tree

   CNodeTranslator::NodeChanged(node, plug);

}


void CShaderTranslator::ExportBump(AtNode* shader)
{
   static AtString normalParameter("normal");

   MStatus status;
   MPlug plug = FindMayaPlug("normalCamera", &status);
   if (status != MS::kSuccess || plug.isNull())
      return; // no "normalCamera" attribute in this shader

   if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(shader), normalParameter) == NULL)
      return; // this arnold shader doesn't have a "normal" attribute so we're skipping this

   if (AiNodeIsLinked(shader, "normal"))
      AiNodeUnlink(shader, "normal");

   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      AtNode *bump = ExportConnectedNode(connections[0]);
      if (bump)
         AiNodeLink(bump, "normal", shader);

   }
}

bool CShaderTranslatorImpl::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   // If this is a multi-output shader, just copy the MPlug
   if (DependsOnOutputPlug()) 
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