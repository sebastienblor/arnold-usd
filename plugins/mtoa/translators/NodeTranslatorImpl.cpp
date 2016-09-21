#include "NodeTranslatorImpl.h"

#include "render/RenderOptions.h"
#include "extension/ExtensionsManager.h"
#include "attributes/Components.h"
#include "common/UtilityFunctions.h"
#include "scene/MayaScene.h"

#include <ai_ray.h>
#include <ai_metadata.h>
#include <ai_metadata.h>

#include <maya/MPoint.h>
#include <maya/MMatrix.h>
#include <maya/MPlugArray.h>
#include <maya/MFnSet.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnStringData.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MNodeMessage.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MEventMessage.h>

#include <string>
#include <algorithm>
#include <iterator>

#include <ai_universe.h>
#include <assert.h>

#include "utils/time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// internal use only
AtNode* CNodeTranslatorImpl::DoExport()
{
   AtNode* node = m_tr.GetArnoldNode("");
   MString outputAttr = m_tr.GetMayaOutputAttributeName();
   int step = m_tr.GetMotionStep();

   // FIXME : for now we're setting isExported to false when we ask for a full re-export
   // but as refactoring continues we'll stop doing it. 
   // And we'll restore it to false only when Delete() is called
   m_isExported = false; 
   if (node == NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Export requested but no Arnold node was created by this translator (%s)",
                   m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar());
      return NULL;
   }

   // FIXME couldn't we just call the same functions for whatever step, 
   // and do an early out on the other methods when GetMotionStep() > 0 ?
   if (step == 0)
   {
      if (outputAttr != "")
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting on plug %s (%s)",
                    m_tr.GetMayaNodeName().asChar(), outputAttr.asChar(), m_tr.GetTranslatorName().asChar());
      else
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting (%s)",
                    m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar());

      ComputeAOVs();
      m_tr.Export(node);
      ExportUserAttribute(node);
      WriteAOVUserAttributes(node);
   }
   else if (m_tr.RequiresMotionData())
   {
      if (outputAttr != "")
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting Motion on plug %s (%s)",
                    m_tr.GetMayaNodeName().asChar(), outputAttr.asChar(), m_tr.GetTranslatorName().asChar());
      else
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting Motion (%s)",
                    m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar());

      m_tr.ExportMotion(node);
   }

   return m_tr.GetArnoldNode();
}

// internal use only
AtNode* CNodeTranslatorImpl::DoUpdate()
{
   // if this translator has never been exported, we should rather call DoExport()
   if (!m_isExported) return DoExport();

   assert(AiUniverseIsActive());
   AtNode* node = m_tr.GetArnoldNode("");
   int step = m_tr.GetMotionStep();

   if (node == NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Update requested but no Arnold node was created by this translator (%s)",
                   m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar());
      return NULL;
   }
   
   AiMsgDebug("[mtoa.translator]  %-30s | %s: Updating Arnold %s(%s): %p",
              m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar(),
              AiNodeGetName(node), AiNodeEntryGetName(AiNodeGetNodeEntry(node)),
              node);

   if (step == 0)
   {
#ifdef NODE_TRANSLATOR_REFERENCES 
      // before exporting, clear all the references
      // Note that this shouldn't happen here
      std::vector<CNodeTranslator *> previousRefs = m_references;
      m_references.clear();
#endif
      m_sourceTranslator = NULL; // this will be set during Export

      m_tr.Export(node);
      ExportUserAttribute(node);

#ifdef NODE_TRANSLATOR_REFERENCES 
      // now for all previous references, check if one of them has disappeared
      if (!previousRefs.empty())
      {
         // main goal is to make it fast on the most common cases,
         // which is when the list of references doesn't change during updates
         unsigned int minSize = MIN(previousRefs.size(), m_references.size());
         for (unsigned int i = 0; i < minSize; ++i)
         {
            // as it happens quite often, we first check if the reference is in the same index as before
            if (previousRefs[i] == m_references[i]) continue;
            
            // otherwise need to find the previous element in the reference list, can be much longer
            if (std::find(m_references.begin(), m_references.end(), previousRefs[i]) == m_references.end())
            {
               // previous reference has disappeared
               // make sure this translator still exists !
               previousRefs[i]->m_impl->RemoveBackReference(&m_tr);
            }
         }
         // in case previousRefs was larger than current references
         for (unsigned int i = minSize; i < previousRefs.size(); ++i)
         {
            if (std::find(m_references.begin(), m_references.end(), previousRefs[i]) == m_references.end())
            {
               // previous reference has disappeared
               // make sure this translator still exists !
               previousRefs[i]->m_impl->RemoveBackReference(&m_tr);
            }
         }
      }
#endif


   }
   else if (m_tr.RequiresMotionData())
   {
      m_tr.ExportMotion(node);
   }

   return m_tr.GetArnoldNode();
}

void CNodeTranslatorImpl::DoCreateArnoldNodes()
{   
   m_atNode = m_tr.CreateArnoldNodes();
   
   if (m_atNode == NULL)
      AiMsgDebug("[mtoa.translator]  %s (%s): Translator %s returned an empty Arnold root node.",
            m_tr.GetMayaNodeName().asChar(), m_tr.GetMayaNodeTypeName().asChar(), m_tr.GetTranslatorName().asChar());
   else if (AiNodeIs(m_atNode, "procedural"))
   {
      // FIXME : make sure we get rid of this once a DG is implemented in arnold
      
      // this is a procedural node, so it can "contain"
      // other arnold nodes, we're flagging it this way
      m_isProcedural = true;
      // need to register to arnold session, that will keep track of it
      m_session->RegisterProcedural(m_atNode, &m_tr);
   }
}



/// Calls ExportConnectedNode and AiNodeLink if there are incoming connections to 'plug'
/// returns the connected arnold AtNode or NULL
AtNode* CNodeTranslatorImpl::ProcessParameterInputs(AtNode* arnoldNode, const MPlug &plug,
                                                const char* arnoldParamName,
                                                int arnoldParamType)
{
   MPlugArray connections;
   plug.connectedTo(connections, true, false);

   if (connections.length() > 0)
   {
      // process connections
      MPlug srcMayaPlug = connections[0];
      
#if MAYA_API_VERSION >= 201650
      MPlug directSrcMayaPlug = srcMayaPlug;
      while(srcMayaPlug.isDestination())
      {
         // Entering this loop will happen if the source plug (readable)
         // is also writable (i.e. it's value is not computed by the
         // node, but it's given as an input to it) => no need to translate
         // that node, just need to forward evaluation to its source if any)
         srcMayaPlug = srcMayaPlug.source();
         if(srcMayaPlug == directSrcMayaPlug)
            break;
      }
#endif
      
      CNodeTranslator* srcNodeTranslator = NULL;
      AtNode* srcArnoldNode = ExportConnectedNode(srcMayaPlug, true, &srcNodeTranslator);

      if (srcArnoldNode == NULL)
         return NULL;

      // For material view sessions we need to propagate any shader update upstream
      // because it's only the root shader that gets sent for update, even if the
      // update was caused by some upstream shader node
      if (srcNodeTranslator && m_session->GetSessionMode() == MTOA_SESSION_MATERIALVIEW)
      {
         // Check if the given node is a shader
         const AtNodeEntry* nodeEntry = AiNodeGetNodeEntry(arnoldNode);
         const int type = AiNodeEntryGetType(nodeEntry);
         if (type == AI_NODE_SHADER)
         {
            // Propagate the update upstream
            srcNodeTranslator->m_impl->DoUpdate();
         }
      }

      if (arnoldParamType == AI_TYPE_NODE)
      {
         // An AI_TYPE_NODE param is controlled via a Maya message attribute. Unlike numeric attributes, in Maya
         // there is no way of assigning the value of a message attribute other than via a connection.
         // In the case of a NODE/message connection we should not use AiNodeLink, which is used to delay evaluation
         // of a parameter until render, we should just set the value.
         AiNodeSetPtr(arnoldNode, arnoldParamName, srcArnoldNode);
      }
      else
      {
         // Check for success
         // get component name: "r", "g", "b", "x", etc
         int outputType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(srcArnoldNode));
         MString component = GetComponentName(outputType,srcMayaPlug);
         if (!AiNodeLinkOutput(srcArnoldNode, component.asChar(), arnoldNode, arnoldParamName))
         {
            AiMsgWarning("[mtoa] Could not link %s to %s.%s.",
               AiNodeGetName(srcArnoldNode),
               AiNodeGetName(arnoldNode),
               arnoldParamName);
            return NULL;
         }
      }
      return srcArnoldNode;
   }
   return NULL;
}

bool CNodeTranslatorImpl::ProcessParameterComponentInputs(AtNode* arnoldNode, const MPlug &parentPlug,
                                                      const char* arnoldParamName,
                                                      int arnoldParamType)
{
   const MStringArray componentNames = GetComponentNames(arnoldParamType);
   unsigned int compConnected = 0;
   unsigned int numComponents = componentNames.length();
   MPlugArray conn;
   for (unsigned int i=0; i < numComponents; i++)
   {
      MString compAttrName(arnoldParamName);
      compAttrName += "." + componentNames[i];
      MPlug childPlug = parentPlug.child(i);
      // components are always float
      if (ProcessParameterInputs(arnoldNode, childPlug, compAttrName.asChar(), AI_TYPE_FLOAT) != NULL)
         ++compConnected;
   }
   // RGBA is a special case because the alpha is not a child plug
   if (arnoldParamType == AI_TYPE_RGBA)
   {
      numComponents = 4;
      MStatus stat;
      MString name = parentPlug.partialName(false, false, false, false, false, true) + "A";
      MPlug childPlug = MFnDependencyNode(parentPlug.node()).findPlug(name, false, &stat);
      if (stat == MS::kSuccess)
      {
         MString compAttrName(arnoldParamName);
         compAttrName += "." + componentNames[3];
         if (ProcessParameterInputs(arnoldNode, childPlug, compAttrName.asChar(), AI_TYPE_FLOAT) != NULL)
            ++compConnected;
      }
   }
   return compConnected == numComponents;
}

// FIXME: store translators list instead of MObject list for m_overrideSets
// ExportNode and ExportDagPath should return a pointer to translator, much easier
// than a pointer to Arnold Node
/// Get a plug for that attribute name on the maya override sets, if any
MPlug CNodeTranslatorImpl::FindMayaOverridePlug(const MString &attrName, MStatus* ReturnStatus) const
{
   MPlug plug;
   MStatus status(MStatus::kSuccess);   
   // Check if a set override this plug's value
   std::vector<CNodeTranslator*>::iterator it;
   std::vector<CNodeTranslator*> translators;
   unsigned int novr = m_overrideSets.size();
   for (unsigned int i=0; i<novr; i++)
   {
      CNodeTranslator* translator = m_overrideSets[i];
      
      if (translator == 0)
          continue;
      // MString setName = translator->GetMayaObjectName();
      // Search only on active translators
      if (translator->m_impl->FindMayaObjectPlug("aiOverride", &status).asBool())
      {
         plug = translator->m_impl->FindMayaObjectPlug(attrName, &status);
      }
      if (plug.isNull())
      {
         // But chain on all
         // It's a depth first search on sets of sets
         plug = translator->m_impl->FindMayaOverridePlug(attrName, &status);
      }

      // More than one (non nested) set contains object, stop on first one
      if (!plug.isNull()) break;
   }

   if (ReturnStatus != NULL) *ReturnStatus = status;
   return plug;
}

/// Get a plug for that attribute name on the maya translated object
MPlug CNodeTranslatorImpl::FindMayaObjectPlug(const MString &attrName, MStatus* ReturnStatus) const
{
   MFnDependencyNode fnNode(m_handle.object());
   return fnNode.findPlug(attrName, ReturnStatus);
}

void CNodeTranslatorImpl::RemoveUpdateCallbacks()
{
   if (m_mayaCallbackIDs.length() == 0) return;
   
   const MStatus status = MNodeMessage::removeCallbacks(m_mayaCallbackIDs);
   if (status == MS::kSuccess) m_mayaCallbackIDs.clear();
}

void CNodeTranslatorImpl::Init(CArnoldSession* session, const CNodeAttrHandle& object)
{
   m_session = session;
   m_handle = object;
   ExportOverrideSets();
   
   // first eventually initialize the translator
   m_tr.Init();

   // then create the arnoldNodes
   DoCreateArnoldNodes();

   // If the "arnold" name of this translator hasn't been set yet 
   // we set it with the type of the created arnold node
   if (m_abstract.arnold.length() == 0 && m_atNode)
      m_abstract.arnold = AiNodeEntryGetName(AiNodeGetNodeEntry(m_atNode));
   
}


/// get override sets containing the passed Maya dag path
/// and add them to the passed MObjectArray
/// and we also need to check the parent nodes, so groups are handled properly
static MStatus GetOverrideSets(MDagPath path, MObjectArray &overrideSets)
{
   MStatus status;
   MDagPath pathRec = path;
   for(;pathRec.length();pathRec.pop(1))
   {
      MFnDagNode fnDag(pathRec);
      unsigned int instNum = pathRec.instanceNumber();
      MPlug instObjGroups = fnDag.findPlug("instObjGroups", true, &status).elementByLogicalIndex(instNum);
      CHECK_MSTATUS(status)
      MPlugArray connections;
      MFnSet fnSet;
      // MString plugName = instObjGroups.name();
      if (instObjGroups.connectedTo(connections, false, true, &status))
      {
         unsigned int nc = connections.length();
         for (unsigned int i=0; i<nc; i++)
         {
            MObject set = connections[i].node();
            MFnDependencyNode setDNode(set);
            if (setDNode.typeName() == MString("objectSet"))
            {
               if (!fnSet.setObject(set))
                  continue;
               // MString setName = fnSet.name();
               // Also add sets with override turned off to allow chaining
               // on these as well
               MPlug p = fnSet.findPlug("aiOverride", true, &status);
               if ((MStatus::kSuccess == status) && !p.isNull())
               {
                  overrideSets.append(set);
               }
            }
         }
      }
   }

   return status;
}

/// find override sets containing the passed Maya node
/// and add them to the passed MObjectArray
static MStatus GetOverrideSets(MObject object, MObjectArray &overrideSets)
{
   MStatus status;

   MFnDependencyNode fnNode(object);
   MPlug message = fnNode.findPlug("message", true, &status);
   MPlugArray connections;
   MFnSet fnSet;
   // MString plugName = message.name();
   if (message.connectedTo(connections, false, true, &status))
   {
      unsigned int nc = connections.length();
      for (unsigned int i=0; i<nc; i++)
      {
         MObject set = connections[i].node();
         MFnDependencyNode setDNode(set);
         if (setDNode.typeName() == MString("objectSet"))
         {
            if (!fnSet.setObject(set))
               continue;
            // MString setName = fnSet.name();
            // Also add sets with override turned off to allow chaining
            // on these as well
            MPlug p = fnSet.findPlug("aiOverride", true, &status);
            if ((MStatus::kSuccess == status) && !p.isNull())
            {
               overrideSets.append(set);
            }
         }
      }
   }

   return status;
}


bool CNodeTranslatorImpl::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   resolvedOutputPlug=outputPlug;
   return true;
}
/// gather up the active AOVs for the current node and add them to m_AOVs
void CNodeTranslatorImpl::ComputeAOVs()
{
   // FIXME: add early bail out if AOVs are not enabled

   MStringArray aovAttrs;

   MString typeName = m_tr.GetMayaNodeTypeName();
   CExtensionsManager::GetNodeAOVs(typeName, aovAttrs);
   // FIXME: use more efficient insertion method
   MStatus stat;
   MPlug plug;
   for (unsigned int i=1; i < aovAttrs.length(); i+=3)
   {
      plug = m_tr.FindMayaPlug(aovAttrs[i], &stat);
      if (stat == MS::kSuccess)
      {
         CAOV aov;
         MString value = plug.asString();
         aov.SetName(value);
         if (m_session->IsActiveAOV(aov))
         {
            m_localAOVs.insert(aov);
         }
      }
   }
}



/// gather the active override sets containing this node
MStatus CNodeTranslatorImpl::ExportOverrideSets()
{
   MStatus status;
   m_overrideSets.clear();

   MObjectArray overrideSetObjs;

   if (IsMayaTypeDag())
   {
      MDagPath path;
      MDagPath::getAPathTo(m_tr.GetMayaObject(), path);
      MDagPath nodePath = path;

      status = GetOverrideSets(path, overrideSetObjs);
      // If passed path is a shape, check for its transform as well
      // FIXME: do we want to consider full hierarchy ?
      // Also consider the sets the transform of that shape might be in
      const MObject transformObj = path.transform(&status);
      while ((MStatus::kSuccess == status) && (transformObj != path.node(&status)))
      {
         status = path.pop();
      }
      if (!(path == nodePath))
      {
         status = GetOverrideSets(path, overrideSetObjs);
      }

   } else
   {
      status = GetOverrideSets(m_tr.GetMayaObject(), overrideSetObjs);
   }


   // Exporting a set creates no Arnold object but allows IPR to track it
   MFnSet fnSet;
   unsigned int ns = overrideSetObjs.length();
   for (unsigned int i=0; i<ns; i++)
   {
      fnSet.setObject(overrideSetObjs[i]);
      m_overrideSets.push_back(m_session->ExportNode(fnSet.findPlug("message")));
   }
   if (ns > 0)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | %s: Exported %i override sets.",
              m_tr.GetMayaNodeName().asChar(), m_tr.GetTranslatorName().asChar(), ns);
   }
   return status;
}
void CNodeTranslatorImpl::SetShadersList(AtNodeSet* nodes)
{
   // by default m_impl->m_shaders is null, and thus we just copy the AtNodeSet pointer.
   // However, ShadingEngineTranslator allocates m_shaders in its constructor, and it must NOT
   // copy the pointer in that case. 
   //So we copy the pointer only if m_shaders hasn't been allocated yet
   if (m_shaders == NULL) m_shaders = nodes;

}

void CNodeTranslatorImpl::TrackAOVs(AOVSet* aovs)
{
   // create union
   AOVSet tempSet;
   std::set_union(m_localAOVs.begin(), m_localAOVs.end(),
                  m_upstreamAOVs.begin(), m_upstreamAOVs.end(),
                  std::inserter(tempSet, tempSet.begin()));
   std::set_union(tempSet.begin(), tempSet.end(),
                  aovs->begin(), aovs->end(),
                  std::inserter(tempSet, tempSet.begin()));
   aovs->swap(tempSet);
}



/// Adds new AOV write nodes to aovShaders for any AOVs with defaults not present in this shading network.
/// Defaults are specified by connecting a shader to the "defaultValue" attribute of an aiAOV node.
/// Can be used by ShadingEngineTranslator or by ShapeTranslator for nodes like shave which act like
/// Shape + ShadingGroup + Shader in one
void CNodeTranslatorImpl::AddAOVDefaults(AtNode* shadingEngine, std::vector<AtNode*> &aovShaders)
{
   // FIXME: add early bail out if AOVs are not enabled

   AOVSet active = m_session->GetActiveAOVs();
   AOVSet total;
   AOVSet unused;

   // get the active AOVs not in the exported list
   std::set_union(m_localAOVs.begin(), m_localAOVs.end(),
                  m_upstreamAOVs.begin(), m_upstreamAOVs.end(),
                  std::inserter(total, total.begin()));

   std::set_difference(active.begin(), active.end(),
                       total.begin(), total.end(),
                       std::inserter(unused, unused.begin()));

   MFnDependencyNode fnNode;
   for (AOVSet::iterator it=unused.begin(); it!=unused.end(); ++it)
   {
      CAOV aov = *it;
      MObject oAOV = aov.GetNode();
      if (oAOV != MObject::kNullObj)
      {
         fnNode.setObject(oAOV);
         MPlug plug = fnNode.findPlug("defaultValue");
         MPlugArray connections;
         plug.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            int outType = fnNode.findPlug("type").asInt();
            MString nodeType = GetAOVNodeType(outType);

            // process connections
            // use false to avoid processing aovs for this node
            AtNode* linkedNode = ExportConnectedNode(connections[0], false);
            if (linkedNode != NULL)
            {
               const char* aovName = aov.GetName().asChar();
               AtNode* writeNode = m_tr.AddArnoldNode(nodeType.asChar(), aovName);
               AiNodeSetStr(writeNode, "aov_name", aovName);
               AiNodeLink(linkedNode, "input", writeNode);
               aovShaders.push_back(writeNode);
            }
            else
               AiMsgWarning("[mtoa] [aov] invalid input on default value for \"%s\"", aov.GetName().asChar());
         }
         //ProcessParameter(shader, plug, "input", AI_TYPE_RGB);
      }
   }
   if (aovShaders.size() > 0)
      AiNodeSetArray(shadingEngine, "aov_inputs", AiArrayConvert(aovShaders.size(), 1, AI_TYPE_NODE, &aovShaders[0]));
}



/// Get the override plug for the passed maya plug
/// if there is one, otherwise, returns the passed maya plug.
MPlug CNodeTranslatorImpl::GetOverridePlug(const MPlug &plug, MStatus* ReturnStatus) const
{
   MStatus status(MStatus::kSuccess);
   MPlug resultPlug(plug);

   // MPlug::partialName signature is
   // (bool includeNodeName=false, bool includeNonMandatoryIndices=false,
   // bool includeInstancedIndices=false, bool useAlias=false, bool useFullAttributePath=false,
   // bool useLongNames=false, MStatus *ReturnStatus=NULL)
   MString attrName = plug.partialName(false, true, true, false, true, true, &status);
   MPlug overridePlug = FindMayaOverridePlug(attrName, &status);
   CHECK_MSTATUS(status)
   if ((MStatus::kSuccess == status) && !overridePlug.isNull())
      resultPlug = overridePlug;

   if (ReturnStatus != NULL) *ReturnStatus = status;
   return resultPlug;
}

void CNodeTranslatorImpl::WriteAOVUserAttributes(AtNode* atNode)
{
   if (m_upstreamAOVs.size() && AiNodeDeclare(atNode, "mtoa_aovs", "constant ARRAY STRING"))
   {
      AiMsgDebug("[mtoa] [aovs] %s writing accumulated AOVs", m_tr.GetMayaNodeName().asChar());
      AtArray *ary = AiArrayAllocate(m_upstreamAOVs.size(), 1, AI_TYPE_STRING);
      unsigned int i=0;
      for (AOVSet::iterator it=m_upstreamAOVs.begin(); it!=m_upstreamAOVs.end(); ++it)
      {
         AiMsgDebug("[mtoa] [aovs]     %s", it->GetName().asChar());
         AiArraySetStr(ary, i, it->GetName().asChar());
         ++i;
      }
      AiNodeSetArray(atNode, "mtoa_aovs", ary);
      /*
      const CRenderOptions* renderOptions = CRenderSession::GetInstance()->RenderOptions();
      std::vector<std::string> activeAOVs;
      for (AOVSet::iterator it=m_localAOVs.begin(); it!=m_localAOVs.end(); ++it)
      {
         CAOV aov = *it;
         if (renderOptions->IsActiveAOV(aov))
         {
            activeAOVs.push_back(aov.GetName().asChar());
         }
      }
      unsigned int size = activeAOVs.size();
      if (size)
      {
         cout << GetFnNode().name() << ": " << size << " active AOVs" << endl;
         AtArray *ary = AiArrayAllocate(size, 1, AI_TYPE_STRING);
         for (unsigned int i=0; i < size; ++i)
         {
            cout << "   " << activeAOVs[i] << endl;
            AiArraySetStr(ary, i, activeAOVs[i].c_str());
         }
         AiNodeSetArray(atNode, "mtoa_aovs", ary);
      }*/
   }
}

/// Export value for a plug with no direct connections (may have child or element connections).
/// For arrays, calls ProcessArrayParameter.
/// For simple numeric types, calls AiNodeSet*.
/// For compound types, first calls ProcessParameterComponentInputs. If ProcessParameterComponentInputs returns
/// false (not all components linked) then AiNodeSet* is called for all components.
AtNode* CNodeTranslatorImpl::ProcessConstantParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                                  int arnoldParamType, const MPlug& plug)
{
   MStatus status;

   if (plug.isArray())
   {
      if (arnoldParamType != AI_TYPE_ARRAY)
      {
         MGlobal::displayError("[mtoa] Maya attribute is of type array, but Arnold parameter is not");
         return NULL;
      }
      m_tr.ProcessArrayParameter(arnoldNode, arnoldParamName, plug);
      return NULL;
   }
   if (plug.isCompound())
   {
      // Process the childs for compound plugs with at least one connected child
      if (ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
      {
         return NULL;
      }
   }

   switch(arnoldParamType)
   {
   case AI_TYPE_RGB:
      {
         unsigned int numChildren = plug.numChildren();
         if (numChildren== 3)
         {
            AiNodeSetRGB(arnoldNode, arnoldParamName,
                         plug.child(0).asFloat(),
                         plug.child(1).asFloat(),
                         plug.child(2).asFloat());
         }
         else
         {
            AiMsgError("[mtoa] Improper RGB attribute %s",
                       plug.partialName(true, false, false, false, false, true).asChar());
         }
      }
      break;
   case AI_TYPE_RGBA:
      {
         unsigned int numChildren = plug.numChildren();

         if (numChildren== 4)
         {
            // this type of plug is not created by MtoA, custom nodes may implement RGBA this way
            AiNodeSetRGBA(arnoldNode, arnoldParamName,
                          plug.child(0).asFloat(),
                          plug.child(1).asFloat(),
                          plug.child(2).asFloat(),
                          plug.child(3).asFloat());
         }
         else if (numChildren== 3)
         {
            MStatus stat;
            MString alphaName = plug.partialName(false, false, false, false, false, true) + "A";
            MPlug alphaPlug = MFnDependencyNode(plug.node()).findPlug(alphaName, false, &stat);
            if (stat == MS::kSuccess)
            {
               AiNodeSetRGBA(arnoldNode, arnoldParamName,
                             plug.child(0).asFloat(),
                             plug.child(1).asFloat(),
                             plug.child(2).asFloat(),
                             alphaPlug.asFloat());
            }
            else
            {
               AiMsgWarning("[mtoa] RGBA attribute %s has no alpha component: exporting as RGBA",
                            plug.partialName(true, false, false, false, false, true).asChar());
               AiNodeSetRGBA(arnoldNode, arnoldParamName,
                            plug.child(0).asFloat(),
                            plug.child(1).asFloat(),
                            plug.child(2).asFloat(),
                            1.0f);
            }
         }
         else
         {
            AiMsgError("[mtoa] Improper RGBA attribute %s",
                       plug.partialName(true, false, false, false, false, true).asChar());
         }
      }
      break;
   case AI_TYPE_FLOAT:
      {
         float val = plug.asFloat();
         // check for scaling
         const AtNodeEntry* nentry = AiNodeGetNodeEntry(arnoldNode);
         AtMetaDataIterator* miter = AiNodeEntryGetMetaDataIterator(nentry, arnoldParamName);
         while(!AiMetaDataIteratorFinished(miter))
         {
            const AtMetaDataEntry* mentry = AiMetaDataIteratorGetNext(miter);
            if ((strcmp(mentry->name, "scale") == 0) && (mentry->type == AI_TYPE_INT))
            {
               if (mentry->value.INT == 1) // scale distance
                  m_session->ScaleDistance(val);
            }
            continue;
         }
         AiMetaDataIteratorDestroy(miter);

         AiNodeSetFlt(arnoldNode, arnoldParamName, val);
      }
      break;
   case AI_TYPE_POINT2:
      {
         float x, y;
         MObject numObj = plug.asMObject();
         MFnNumericData numData(numObj);
         numData.getData2Float(x, y);
         AiNodeSetPnt2(arnoldNode, arnoldParamName, x, y);
      }
      break;
   case AI_TYPE_MATRIX:
      {
         // special case for shaders with matrix values that represent transformations
         // FIXME: introduce "xform" metadata to explicitly mark a matrix parameter
         if (m_tr.RequiresMotionData() && strcmp(arnoldParamName, "placementMatrix") == 0)
         {
            // create an interpolation node for matrices
            AtNode* animNode = m_tr.AddArnoldNode("anim_matrix", arnoldParamName);
            AtArray* matrices = AiArrayAllocate(1, m_tr.GetNumMotionSteps(), AI_TYPE_MATRIX);

            ProcessConstantArrayElement(AI_TYPE_MATRIX, matrices, 0, plug);

            // Set the parameter for the interpolation node
            AiNodeSetArray(animNode, "values", matrices);
            // link to our node
            AiNodeLink(animNode, arnoldParamName, arnoldNode);
         }
         else
         {
            AtMatrix am;
            MObject matObj = plug.asMObject();
            MFnMatrixData matData(matObj);
            MMatrix mm = matData.matrix();
            CNodeTranslator::ConvertMatrix(am, mm);
            AiNodeSetMatrix(arnoldNode, arnoldParamName, am);
         }
      }
      break;
   case AI_TYPE_BOOLEAN:
      {
         AiNodeSetBool(arnoldNode, arnoldParamName, plug.asBool());
      }
      break;
   case AI_TYPE_ENUM:
      {
         AiNodeSetInt(arnoldNode, arnoldParamName, plug.asInt());
      }
      break;
   case AI_TYPE_INT:
      {
         AiNodeSetInt(arnoldNode, arnoldParamName, plug.asInt());
      }
      break;
   case AI_TYPE_UINT:
      {
         // no uint in maya MPlug
         AiNodeSetUInt(arnoldNode, arnoldParamName, plug.asInt());
      }
      break;
   case AI_TYPE_STRING:
      {
         AiNodeSetStr(arnoldNode, arnoldParamName, plug.asString().asChar());
      }
      break;
   case AI_TYPE_VECTOR:
      {
         AiNodeSetVec(arnoldNode, arnoldParamName,
                      plug.child(0).asFloat(),
                      plug.child(1).asFloat(),
                      plug.child(2).asFloat());
      }
      break;
   case AI_TYPE_POINT:
      {
         AiNodeSetPnt(arnoldNode, arnoldParamName,
                      plug.child(0).asFloat(),
                      plug.child(1).asFloat(),
                      plug.child(2).asFloat());
      }
      break;
   case AI_TYPE_NODE:
      // handled above by ProcessParameterInputs
      break;
   case AI_TYPE_ARRAY:
      {
         if (!plug.isArray())
         {
            MGlobal::displayError("[mtoa] Arnold parameter is of type array, but corresponding Maya attribute is not");
            return NULL;
         }
         m_tr.ProcessArrayParameter(arnoldNode, arnoldParamName, plug);
      }
      break;
   case AI_TYPE_BYTE:
      AiNodeSetByte(arnoldNode, arnoldParamName, (unsigned char)plug.asChar());
      break;
   }
   return NULL;
}

void CNodeTranslatorImpl::ProcessArrayParameterElement(AtNode* arnoldNode, AtArray* array, const char* arnoldParamName, const MPlug& elemPlug, unsigned int arnoldParamType, unsigned int pos)
{
   // connections:
   // An AI_TYPE_NODE param is controlled via a Maya message attribute. Unlike numeric attributes, in Maya
   // there is no way of assigning the value of a message attribute other than via a connection.
   // Therefore, we handle node/message connections in ProcessConstantArrayElement
   if (arnoldParamType != AI_TYPE_NODE)
   {
      MString elemName = MString(arnoldParamName) + "[" + pos + "]";
      AtNode* connected = ProcessParameterInputs(arnoldNode, elemPlug, elemName.asChar(), arnoldParamType);
      // FIXME: should we always evaluate components even if something was connected at the parent level?
      if (connected == NULL)
      {
         // component connections
         switch(arnoldParamType)
         {
         case AI_TYPE_RGB:
         case AI_TYPE_RGBA:
         case AI_TYPE_POINT2:
         case AI_TYPE_VECTOR:
         case AI_TYPE_POINT:
            {
               if(ProcessParameterComponentInputs(arnoldNode, elemPlug, elemName.asChar(), arnoldParamType) == false)
               {
                  // constant value
                  ProcessConstantArrayElement(arnoldParamType, array, pos, elemPlug);
               }
            }
            break;
         default:
            // constant value
            ProcessConstantArrayElement(arnoldParamType, array, pos, elemPlug);
         }
      }
   }
   else
   {
      // constant value
      ProcessConstantArrayElement(arnoldParamType, array, pos, elemPlug);
   }
}

void CNodeTranslatorImpl::ProcessConstantArrayElement(int type, AtArray* array, unsigned int i, const MPlug& elem)
{
   switch(type)
   {
   case AI_TYPE_RGB:
      {
         // FIXME: exporting all zeros!!!
         AtRGB color;
         color.r = elem.child(0).asFloat();
         color.g = elem.child(1).asFloat();
         color.b = elem.child(2).asFloat();
         AiArraySetRGB(array, i, color);
      }
      break;
   case AI_TYPE_RGBA:
      {
         AtRGBA color;
         color.r = elem.child(0).asFloat();
         color.g = elem.child(1).asFloat();
         color.b = elem.child(2).asFloat();
         // Is the source parameter RGB or RGBA?
         if (elem.numChildren() == 4)
         {
            color.a = elem.child(3).asFloat();
            AiArraySetRGBA(array, i, color);
         }
         else
         {
            color.a = 1.0f;
            // FIXME: handle alphas!
            AiArraySetRGBA(array, i, color);
         }
      }
      break;
   case AI_TYPE_FLOAT:
      {
         AiArraySetFlt(array, i, elem.asFloat());
      }
      break;
   case AI_TYPE_POINT2:
      {
         float x, y;
         MObject numObj = elem.asMObject();
         MFnNumericData numData(numObj);
         numData.getData2Float(x, y);
         AtPoint2 vec2;
         AiV2Create(vec2, x, y);
         AiArraySetPnt2(array, i, vec2);
      }
      break;
   case AI_TYPE_MATRIX:
      {
         AtMatrix am;
         MObject matObj = elem.asMObject();
         MFnMatrixData matData(matObj);
         MMatrix mm = matData.matrix();
         CNodeTranslator::ConvertMatrix(am, mm);
         AiArraySetMtx(array, i, am);
      }
      break;
   case AI_TYPE_BOOLEAN:
      {
         AiArraySetBool(array, i, elem.asBool());
      }
      break;
   case AI_TYPE_ENUM:
      {
         AiArraySetInt(array, i, elem.asInt());
      }
      break;
   case AI_TYPE_INT:
      {
         AiArraySetInt(array, i, elem.asInt());
      }
      break;
   case AI_TYPE_UINT:
      {
         AiArraySetUInt(array, i, elem.asInt());
      }
      break;
   case AI_TYPE_STRING:
      {
         AiArraySetStr(array, i, elem.asString().asChar());
      }
      break;
   case AI_TYPE_VECTOR:
      {
         AtVector vec3;
         AiV3Create(vec3, elem.child(0).asFloat(), elem.child(1).asFloat(), elem.child(2).asFloat());
         AiArraySetVec(array, i, vec3);
      }
      break;
   case AI_TYPE_POINT:
      {
         AtVector vec3;
         AiV3Create(vec3, elem.child(0).asFloat(), elem.child(1).asFloat(), elem.child(2).asFloat());
         AiArraySetPnt(array, i, vec3);
      }
      break;
   case AI_TYPE_NODE:
      {
         MPlugArray connections;
         elem.connectedTo(connections, true, false);
         AtNode* linkedNode = NULL;
         if (connections.length() > 0)
         {
            linkedNode = ExportConnectedNode(connections[0]);
         }
         AiArraySetPtr(array, i, linkedNode);
      }
      break;
   } // switch
}

// Export (and eventually create) the AtNode based on the connection to this outputPlug
AtNode* CNodeTranslatorImpl::ExportConnectedNode(const MPlug& outputPlug, bool track, CNodeTranslator** outTranslator)
{
   CNodeTranslator* translator = NULL;
   
   if (track)
      translator = m_session->ExportNode(outputPlug, m_shaders, &m_upstreamAOVs);
   else
      translator = m_session->ExportNode(outputPlug);
   if (translator != NULL)
   {
      if (outTranslator != NULL)
         *outTranslator = translator;

      // this is used when the DG order is different between Maya and Arnold.
      // For now this only happens with bump mapping. In that case, from the outside the sourceTranslator
      // will have to be referenced instead of this one.
      if (translator->m_impl->m_sourceTranslator) 
         translator = translator->m_impl->m_sourceTranslator;

#ifdef NODE_TRANSLATOR_REFERENCES 
      AddReference(translator);
#endif

      return translator->GetArnoldNode();
   }
   return NULL;
}

void CNodeTranslatorImpl::ExportUserAttribute(AtNode *anode)
{
   // TODO: allow overrides here too ?
   CNodeTranslator::ExportUserAttributes(anode, m_tr.GetMayaObject(), &m_tr);
   
   // Exporting the UnexposedOptions parameter
   MPlug plug = m_tr.FindMayaPlug("aiUserOptions");
   if (!plug.isNull())
      AiNodeSetAttributes(anode, plug.asString().asChar());
}
void CNodeTranslatorImpl::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = CNodeTranslator::GetArnoldNaming(m_tr.GetMayaObject());

   char nodeName[MAX_NAME_SIZE];
   if (m_tr.DependsOnOutputPlug())
   {
      MString outputAttr = m_handle.attribute();
      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (tag != NULL && strlen(tag))
      name = name + AI_TAG_SEP + tag;

   // If name is alredy used, create a new one
   if(AiNodeLookUpByName(name.asChar()))
   {
      AiNodeSetStr(arnoldNode, "name", NodeUniqueName(arnoldNode, nodeName));
   }
   else
   {
      AiNodeSetStr(arnoldNode, "name", name.asChar());
   }
}


// check if an AtArray is animated,  i.e. has different values on multiple keys
// it would be nice if this could be done in arnold core
static inline bool IsArrayAnimated(const AtArray* array)
{
   AtByte type = array->type;
   switch (array->type)
   {
      case AI_TYPE_BOOLEAN:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            bool valInit = AiArrayGetBool(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetBool(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_BYTE:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtByte valInit = AiArrayGetByte(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetByte(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_INT:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            int valInit = AiArrayGetInt(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetInt(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_UINT:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtUInt32 valInit = AiArrayGetUInt(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetUInt(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_FLOAT:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            float valInit = AiArrayGetFlt(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetFlt(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_RGB:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtRGB valInit = AiArrayGetRGB(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetRGB(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_RGBA:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtRGBA valInit = AiArrayGetRGBA(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetRGBA(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_VECTOR:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtVector valInit = AiArrayGetVec(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetVec(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_POINT:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtPoint valInit = AiArrayGetPnt(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetPnt(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_POINT2:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtPoint2 valInit = AiArrayGetPnt2(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if (valInit != AiArrayGetPnt2(array, i + (k * array->nelements))) return true;
            }
         }
      return false;
      case AI_TYPE_MATRIX:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            AtMatrix mtxInit, mtx;
            AiArrayGetMtx(array, i, mtxInit);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               AiArrayGetMtx(array, i + (k * array->nelements), mtx);
               for (int x = 0; x < 4; ++x)
               {
                  for (int y = 0; y < 4; ++y)
                  {
                     if (mtx[x][y] != mtxInit[x][y]) return true;
                  }
               }
            }
         }
      return false;
      case AI_TYPE_STRING:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            const char *valInit = AiArrayGetStr(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if(strcmp(valInit, AiArrayGetStr(array, i + (k * array->nelements))) != 0) return true; 
            }
         }
      return false;
      case AI_TYPE_POINTER:
         for (AtUInt32 i = 0; i < array->nelements; ++i)
         {
            void *valInit = AiArrayGetPtr(array, i);
            for (AtByte k = 1; k < array->nkeys; ++k)
            {
               if(valInit != AiArrayGetPtr(array, i + (k * array->nelements))) return true; 
            }
         }
      return false;

      break;
      default:
      return true;     

   }
}

// Function used to check which AtNodes* in the scene actually require motion evaluation
// since it's hard to get the info from Maya (#2316)

bool CNodeTranslatorImpl::HasAnimatedArrays() const
{
   // for all nodes related to this translator
   if (m_atNode == NULL) return false;

   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(m_atNode));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      if (AiParamGetType(paramEntry) != AI_TYPE_ARRAY)
      {
         continue;
      }
      AtArray *array = AiNodeGetArray(m_atNode, AiParamGetName(paramEntry));
      if (array != NULL && array->nkeys > (AtByte)1)
      {
         // we need to compare the array's keys to check if it's really animated or not
         if (IsArrayAnimated(array))
         {            
            AiParamIteratorDestroy(nodeParam);
            return true;
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);

   if (m_additionalAtNodes != NULL)
   {
      std::map<std::string, AtNode*>::const_iterator it = m_additionalAtNodes->begin();
      std::map<std::string, AtNode*>::const_iterator itEnd = m_additionalAtNodes->end();

      for ( ; it != itEnd; ++it)
      {
         AtNode *node = it->second;
         if (node == NULL) continue;

         nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
         while (!AiParamIteratorFinished(nodeParam))
         {
            const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
            if (AiParamGetType(paramEntry) != AI_TYPE_ARRAY)
            {
               continue;
            }
            AtArray *array = AiNodeGetArray(node, AiParamGetName(paramEntry));
            if (array != NULL && array->nkeys > (AtByte)1)
            {
               // we need to compare the array's keys to check if it's really animated or not
               if (IsArrayAnimated(array))
               {                  
                  AiParamIteratorDestroy(nodeParam);
                  return true;
               }
            }
         }
         AiParamIteratorDestroy(nodeParam);
      }
   }

   // no animated array has been found,
   return false;
}




