#include "platform/Platform.h"
#include "NodeTranslator.h"
#include "DagTranslator.h"
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

MString GetAOVNodeType(int type)
{
   MString nodeType = "";
   switch (type)
   {
   case AI_TYPE_FLOAT:
      nodeType = "aovWriteFloat";
      break;
   case AI_TYPE_RGB:
   case AI_TYPE_RGBA:
      nodeType = "aovWriteColor";
      break;
   case AI_TYPE_VECTOR:
      nodeType = "aovWriteVector";
      break;
   case AI_TYPE_POINT:
      nodeType = "aovWritePoint";
      break;
   case AI_TYPE_POINT2:
      nodeType = "aovWritePoint2";
      break;
   default:
      {
         break;
      }
   }
   return nodeType;
}

// Utility function to check plug for any incoming connection on it or its childs or elements
bool HasIncomingConnection(const MPlug &plug)
{
   MStatus status;

   if (!plug.isNull())
   {
      // Is directly connected
      MPlugArray inConnections;
      if (plug.connectedTo(inConnections, true, false, &status)
            && (inConnections.length() > 0))
      {
         return true;
      }
      // Is compound and has connected childs ?
      if (plug.isCompound())
      {
         unsigned int nc = plug.numChildren();
         for (unsigned int i=0; i<nc; i++) {
            if (HasIncomingConnection(plug.child(i)))
            {
               return true;
            }
         }
      }
      // Is array and has connected elements ?
      if (plug.isArray() && (plug.numConnectedElements() > 0))
      {
         unsigned int ne = plug.numElements();
         for (unsigned int i=0; i<ne; i++) {
            if (HasIncomingConnection(plug.elementByPhysicalIndex(i)))
            {
               return true;
            }
         }
      }
   }

   return false;
}

//------------ CNodeTranslator ------------//

AtNode* CNodeTranslator::ExportNode(const MPlug& outputPlug, bool track, CNodeTranslator** outTranslator)
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
      return translator->GetArnoldRootNode();
   }
   return NULL;
}

AtNode* CNodeTranslator::ExportDagPath(MDagPath &dagPath)
{
   CDagTranslator* translator = NULL;
   translator = m_session->ExportDagPath(dagPath);
   if (translator != NULL)
      return translator->GetArnoldRootNode();
   return NULL;
}

/// Get a plug for that attribute name on the maya translated object
MPlug CNodeTranslator::FindMayaObjectPlug(const MString &attrName, MStatus* ReturnStatus) const
{
   MFnDependencyNode fnNode(m_handle.object());
   return fnNode.findPlug(attrName, ReturnStatus);
}

// FIXME: store translators list instead of MObject list for m_overrideSets
// ExportNode and ExportDagPath should return a pointer to translator, much easier
// than a pointer to Arnold Node
/// Get a plug for that attribute name on the maya override sets, if any
MPlug CNodeTranslator::FindMayaOverridePlug(const MString &attrName, MStatus* ReturnStatus) const
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
      if (translator->FindMayaObjectPlug("aiOverride", &status).asBool())
      {
         plug = translator->FindMayaObjectPlug(attrName, &status);
      }
      if (plug.isNull())
      {
         // But chain on all
         // It's a depth first search on sets of sets
         plug = translator->FindMayaOverridePlug(attrName, &status);
      }

      // More than one (non nested) set contains object, stop on first one
      if (!plug.isNull()) break;
   }

   if (ReturnStatus != NULL) *ReturnStatus = status;
   return plug;
}

/// Get actual plug to be used for that attribute name, either the one on the translated maya object,
/// or the one on the override set to be used, if any.
MPlug CNodeTranslator::FindMayaPlug(const MString &attrName, MStatus* ReturnStatus) const
{
   MStatus status(MStatus::kSuccess);

   MPlug plug = FindMayaObjectPlug(attrName, &status);
   if ((MStatus::kSuccess == status) && !plug.isNull())
   {
      MStatus overstat;
      MString attrLongName = plug.partialName(false, true, true, false, true, true, &overstat);
      MPlug overridePlug = FindMayaOverridePlug(attrLongName, &overstat);
      if ((MStatus::kSuccess == overstat) && !overridePlug.isNull())
      {
         plug = overridePlug;
         status = overstat;
      }
   }

   if (ReturnStatus != NULL) *ReturnStatus = status;
   return plug;
}

/// find override sets containing the passed Maya node
/// and add them to the passed MObjectArray
MStatus CNodeTranslator::GetOverrideSets(MObject object, MObjectArray &overrideSets)
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

/// gather the active override sets containing this node
MStatus CNodeTranslator::ExportOverrideSets()
{
   MStatus status;

   MString nodeName = GetMayaNodeName();
   m_overrideSets.clear();
   MObjectArray overrideSetObjs;
   status = GetOverrideSets(m_handle.object(), overrideSetObjs);
   // Exporting a set creates no Arnold object but allows IPR to track it
   MFnSet fnSet;
   unsigned int ns = overrideSetObjs.length();
   for (unsigned int i=0; i<ns; i++)
   {
      fnSet.setObject(overrideSetObjs[i]);
      m_overrideSets.push_back(m_session->ExportNode(fnSet.findPlug("message")));
   }
   AiMsgDebug("[mtoa.translator]  %-30s | %s: Exported %i override sets.",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(), ns);
   return status;
}

/// Get the override plug for the passed maya plug
/// if there is one, otherwise, returns the passed maya plug.
MPlug CNodeTranslator::GetOverridePlug(const MPlug &plug, MStatus* ReturnStatus) const
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

/// gather up the active AOVs for the current node and add them to m_AOVs
void CNodeTranslator::ComputeAOVs()
{
   // FIXME: add early bail out if AOVs are not enabled

   MStringArray aovAttrs;

   MString typeName = GetMayaNodeTypeName();
   CExtensionsManager::GetNodeAOVs(typeName, aovAttrs);
   // FIXME: use more efficient insertion method
   MStatus stat;
   MPlug plug;
   for (unsigned int i=1; i < aovAttrs.length(); i+=3)
   {
      plug = FindMayaPlug(aovAttrs[i], &stat);
      if (stat == MS::kSuccess)
      {
         CAOV aov;
         MString value = plug.asString();
         aov.SetName(value);
         if (m_session->IsActiveAOV(aov))
         {
            m_localAOVs.insert(aov);
            AiMsgDebug("[mtoa.translator.aov] %-30s | \"%s\" is active on attr %s",
                       GetMayaNodeName().asChar(), value.asChar(), aovAttrs[i].asChar());
         }
      }
   }
}


void CNodeTranslator::TrackAOVs(AOVSet* aovs)
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
void CNodeTranslator::AddAOVDefaults(AtNode* shadingEngine, std::vector<AtNode*> &aovShaders)
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
            AtNode* linkedNode = ExportNode(connections[0], false);
            if (linkedNode != NULL)
            {
               const char* aovName = aov.GetName().asChar();
               AtNode* writeNode = AddArnoldNode(nodeType.asChar(), aovName);
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

void CNodeTranslator::WriteAOVUserAttributes(AtNode* atNode)
{
   if (m_upstreamAOVs.size() && AiNodeDeclare(atNode, "mtoa_aovs", "constant ARRAY STRING"))
   {
      AiMsgDebug("[mtoa] [aovs] %s writing accumulated AOVs", GetMayaNodeName().asChar());
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

// internal use only
AtNode* CNodeTranslator::DoExport(unsigned int step)
{
   AtNode* node = GetArnoldNode("");
   MString outputAttr = GetMayaAttributeName();
   m_step = step;

   if (node == NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Export requested but no Arnold node was created by this translator (%s)",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar());
      return NULL;
   }
   
   AiMsgDebug("[mtoa.translator]  %-30s | %s: Exporting Arnold %s(%s): %p",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              AiNodeGetName(node), AiNodeEntryGetName(AiNodeGetNodeEntry(node)),
              node);

   if (step == 0)
   {
      if (outputAttr != "")
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting on plug %s (%s)",
                    GetMayaNodeName().asChar(), outputAttr.asChar(), GetTranslatorName().asChar());
      else
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting (%s)",
                    GetMayaNodeName().asChar(), GetTranslatorName().asChar());
      ComputeAOVs();
      Export(node);
      ExportUserAttribute(node);
      WriteAOVUserAttributes(node);
   }
   else if (RequiresMotionData())
   {
      if (outputAttr != "")
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting Motion on plug %s (%s)",
                    GetMayaNodeName().asChar(), outputAttr.asChar(), GetTranslatorName().asChar());
      else
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting Motion (%s)",
                    GetMayaNodeName().asChar(), GetTranslatorName().asChar());

      ExportMotion(node, step);
   }

   return GetArnoldRootNode();
}

// internal use only
AtNode* CNodeTranslator::DoUpdate(unsigned int step)
{
   assert(AiUniverseIsActive());
   AtNode* node = GetArnoldNode("");
   m_step = step;

   if (node == NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Update requested but no Arnold node was created by this translator (%s)",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar());
      return NULL;
   }
   
   AiMsgDebug("[mtoa.translator]  %-30s | %s: Updating Arnold %s(%s): %p",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              AiNodeGetName(node), AiNodeEntryGetName(AiNodeGetNodeEntry(node)),
              node);

   if (step == 0)
   {
      Update(node);
      ExportUserAttribute(node);
   }
   else if (RequiresMotionData())
   {
      UpdateMotion(node, step);
   }

   return GetArnoldRootNode();
}

void CNodeTranslator::Export(AtNode* node)
{
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0) ProcessParameter(node, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);
}

AtNode* CNodeTranslator::DoCreateArnoldNodes()
{
   m_atNode = CreateArnoldNodes();
   if (m_atNode == NULL)
      AiMsgDebug("[mtoa.translator]  %s (%s): Translator %s returned an empty Arnold root node.",
            GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(), GetTranslatorName().asChar());
   if (m_atNodes.count("") == 0)
      m_atNodes[""] = m_atNode;
   return GetArnoldRootNode();
}

AtNode* CNodeTranslator::GetArnoldRootNode()
{
   return m_atNode;
}

/// Set the root node of the exported shading network
///
/// Usually this function does not need to be called as the return result of CreateArnoldNodes()
/// serves the same purpose. This is included to allow dynamic networks to be created where the root node
/// is still unknown during CreateArnoldNodes()
void CNodeTranslator::SetArnoldRootNode(AtNode* node)
{
   m_atNode = node;
}

/// convert from maya matrix to AtMatrix
void CNodeTranslator::ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix, const CArnoldSession* arnoldSession)
{
   if (arnoldSession)
   {
      MTransformationMatrix trMat = mayaMatrix;
      trMat.addTranslation((-1.0) * arnoldSession->GetOrigin(), MSpace::kWorld);
      MMatrix copyMayaMatrix = trMat.asMatrix();

      arnoldSession->ScaleMatrix(copyMayaMatrix);
      for (int J = 0; (J < 4); ++J)
      {
         for (int I = 0; (I < 4); ++I)
         {
            matrix[I][J] = (float) copyMayaMatrix[I][J];
         }
      }
   }
   else
   {
      for (int J = 0; (J < 4); ++J)
      {
         for (int I = 0; (I < 4); ++I)
         {
            matrix[I][J] = (float) mayaMatrix[I][J];
         }
      }      
   }
   
}

/// Retrieve a node previously created using AddArnoldNode()
AtNode* CNodeTranslator::GetArnoldNode(const char* tag)
{
   if (m_atNodes.count(tag))
   {
      return m_atNodes[tag];
   }
   else
   {
      AiMsgError("[mtoa.translator]  %s: Translation has not created an Arnold node with tag \"%s\".", GetTranslatorName().asChar(), tag);
      return NULL;
   }
}

/// Create an arnold node of the specified type, and save with the given tag (defaults to "")
AtNode* CNodeTranslator::AddArnoldNode(const char* type, const char* tag)
{
   const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(type);
   // Make sure that the given type of node exists
   if (nodeEntry != NULL)
   {
      AtNode* node = AiNode(type);
      SetArnoldNodeName(node, tag);
      if (m_atNodes.count(tag))
      {
         AiMsgWarning("[mtoa] Translator has already added Arnold node with tag \"%s\"", tag);
         return node;
      }
      else
         m_atNodes[tag] = node;
      return node;
   }
   else
   {
      AiMsgError("[mtoa.translator]   %s: Arnold node type %s does not exist.", GetTranslatorName().asChar(), type);
      return NULL;
   }
}

void CNodeTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = GetMayaNodeName();
   char nodeName[MAX_NAME_SIZE];
   if (DependsOnOutputPlug())
   {
      MString outputAttr = GetMayaAttributeName();
      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (strlen(tag))
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

const char* CNodeTranslator::GetArnoldNodeName(const char* tag)
{
   return AiNodeGetName(GetArnoldNode(tag));
}

const char* CNodeTranslator::GetArnoldTypeName(const char* tag)
{
   AtNode* node = GetArnoldNode(tag);
   if (NULL == node)
   {
      return NULL;
   }
   else
   {
      return AiNodeEntryGetName(AiNodeGetNodeEntry(node));
   }
}

bool CNodeTranslator::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   resolvedOutputPlug=outputPlug;
   return true;
}

// Add callbacks to the node passed in. It's a few simple
// callbacks by default. Since this method is virtual - you can
// add whatever callbacks you need to trigger a fresh.
void CNodeTranslator::AddUpdateCallbacks()
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Add update callbacks for translator %p",
      GetMayaNodeName().asChar(), GetTranslatorName().asChar(), this);
   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(object,
                                             NameChangedCallback,
                                             this,
                                             &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   id = MNodeMessage::addNodeDestroyedCallback(object,
                                               NodeDestroyedCallback,
                                               this,
                                               &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);
}

void CNodeTranslator::ManageUpdateCallback(const MCallbackId id)
{
   m_mayaCallbackIDs.append(id);
}

void CNodeTranslator::RemoveUpdateCallbacks()
{
   const MStatus status = MNodeMessage::removeCallbacks(m_mayaCallbackIDs);
   if (status == MS::kSuccess) m_mayaCallbackIDs.clear();
}

void CNodeTranslator::IdleCallback(void *data)
{
   if (data == NULL) return;
   CNodeTranslator* translator = static_cast< CNodeTranslator* >(data);

   if(translator->m_idleCallback)
   {
      MMessage::removeCallback(translator->m_idleCallback);
      translator->m_idleCallback = 0;
   }
   translator->m_updateMode = AI_RECREATE_NODE;
   translator->RequestUpdate(data);
}


// This is a simple callback triggered when a node is marked as dirty.
void CNodeTranslator::NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData)
{
   MFnDependencyNode dnode(node);
   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: plug that fired: %s, client data: %p.",
         dnode.name().asChar(), plug.name().asChar(), clientData);

   CNodeTranslator* translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      if (translator->m_holdUpdates) // only happens for Arnold RenderView
      {
         // I'm still waiting to update this translator, 
         // I can leave this place
         return;
      }
      
      // procedurals need to clear and re-export at next update (ticket #2314)
      // only for renderView to avoid new bugs.... 
      if (translator->m_session->GetSessionMode() == MTOA_SESSION_RENDERVIEW)
      {
         // I first wanted to do that in CProceduralTranslator
         // but CStandinTranslator doesn't inherit from CProceduralTranslator,
         // and nothing guarantees that people won't create procedural nodes in whatever 
         //  translator. So instead I'm testing the Translator Arnold NodeType
         if(strcmp(translator->GetArnoldNodeType().asChar(), "procedural") == 0) 
         {
            AtNode *rootNode = translator->GetArnoldRootNode();
            if (rootNode != NULL)
            {
               bool b = false;

               const AtParamEntry *pe = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(rootNode), "allow_updates");
               bool allowUpdates = (pe != NULL && AiNodeGetBool(rootNode, "allow_updates") == true);
               allowUpdates |= (AiNodeLookUpUserParameter(rootNode, "allow_updates") ? AiNodeGetBool(rootNode, "allow_updates") : false); 
         
               if (allowUpdates)
               {
                  // check if user data exists
                  if(translator->m_idleCallback == 0)
                  {
                     MStatus status;
                     translator->m_idleCallback = MEventMessage::addEventCallback("idle",
                        CNodeTranslator::IdleCallback, (void*)translator, &status);
                  }
               }
               return;
            }
         }
      }


      if (translator->m_session->IsExportingMotion() && translator->m_session->IsInteractiveRender()) return;

      AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: client data is translator %s, providing Arnold %s(%s): %p",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      MString plugName = plug.name().substring(plug.name().rindex('.'),plug.name().length());
      
      if (plugName == ".aiTranslator")
      {
         // The Arnold translator has changed :
         // This means the current one won't be able to export as it should.
         // By setting its update mode to AI_RECREATE_TRANSLATOR this translator 
         // will be cleared and a new one will be generated
         translator->m_updateMode = AI_RECREATE_TRANSLATOR;
         translator->RequestUpdate(clientData);
         return;
      }

      if(node.apiType() == MFn::kShadingEngine && plugName == ".displacementShader")
      {
         std::vector< CDagTranslator * > translatorsToUpdate;
         bool reexport = true;
         MPlug dagSetMembersPlug = dnode.findPlug("dagSetMembers");
         const unsigned int numElements = dagSetMembersPlug.numElements();
         for(unsigned int i = 0; i < numElements; i++)
         {
            MPlug a = dagSetMembersPlug[i];
            MPlugArray connectedPlugs;
            a.connectedTo(connectedPlugs,true,false);

            const unsigned int connectedPlugsLength = connectedPlugs.length();
            for(unsigned int j = 0; j < connectedPlugsLength; j++)
            {
               MPlug connection = connectedPlugs[j];
               MObject parent = connection.node();
               MFnDependencyNode parentDag(parent);
               MString nameParent = parentDag.name();

               MDagPath dagPath;
               MStatus status = MDagPath::getAPathTo(parent, dagPath);
               if (!status)
                  continue;

               CDagTranslator* translator2 = translator->m_session->ExportDagPath(dagPath, true);

               if (translator2 == 0)
                  continue;

               // TODO: By now we have to check the connected nodes and if something that is not a mesh
               //  is connected, we do not reexport, as some crashes may happen.
               if(translator2->GetMayaNodeTypeName() != "mesh")
               {
                  reexport = false;
                  break;
               }

               translatorsToUpdate.push_back(translator2);
            }

            if(reexport == false)
               break;
         }

         // We only reexport if all nodes connected to the displacement are mesh nodes
         if (reexport)
         {
            for (std::vector<CDagTranslator*>::iterator iter = translatorsToUpdate.begin();
               iter != translatorsToUpdate.end(); ++iter)
            {
               CDagTranslator* translator3 = (*iter);
               if (translator3 != NULL)
               {
                  translator3->m_updateMode = AI_RECREATE_NODE;
                  translator3->RequestUpdate(static_cast<void*>(translator3));
               }
            }
         }
      }

      if(node.apiType() == MFn::kDisplacementShader)
      {
         MPlug disp = MFnDependencyNode(node).findPlug("displacement");
         MPlugArray connectedPlugs;
         disp.connectedTo(connectedPlugs,false,true);;

         // For each shading engine connected to the displacement node
         for(unsigned int j = 0; j < connectedPlugs.length(); j++)
         {
            MPlug connection = connectedPlugs[j];
            MObject shadingEngine = connection.node();

            std::vector< CDagTranslator * > translatorsToUpdate;
            bool reexport = true;

            MFnDependencyNode shadingEngineDNode(shadingEngine);
            MPlug dagSetMembersPlug = shadingEngineDNode.findPlug("dagSetMembers");
            const unsigned int numElements = dagSetMembersPlug.numElements();
            // For each geometry connected to the shading engine
            for(unsigned int i = 0; i < numElements; i++)
            {
               MPlug a = dagSetMembersPlug[i];
               MPlugArray connectedPlugs;
               a.connectedTo(connectedPlugs,true,false);;

               // This should be only one connection; connectedPlugs.length() should be 0 or 1
               const unsigned int connectedPlugsLength = connectedPlugs.length();
               for(unsigned int j = 0; j < connectedPlugsLength; j++)
               {
                  MPlug connection = connectedPlugs[j];
                  MObject parent = connection.node();
                  MFnDependencyNode parentDag(parent);
                  MString nameParent = parentDag.name();

                  MDagPath dagPath;
                  MStatus status = MDagPath::getAPathTo(parent, dagPath);
                  if (!status)
                     continue;

                  CDagTranslator* translator2 = translator->m_session->ExportDagPath(dagPath, true);
                  if (translator2 == 0)
                     continue;

                  translator2->m_updateMode = AI_RECREATE_NODE;
                  translator2->RequestUpdate(static_cast<void*>(translator2));

                  // TODO: By now we have to check the connected nodes and if something that is not a mesh
                  //  is connected, we do not reexport, as some crashes may happen.
                  if(translator2->GetMayaNodeTypeName() != "mesh")
                  {
                     reexport = false;
                     break;
                  }

                  translatorsToUpdate.push_back(translator2);

               }

               if(reexport == false)
                  break;
            }

            // We only reexport if all nodes connected to the displacement are mesh nodes
            if (reexport)
            {
               for (std::vector<CDagTranslator*>::iterator iter = translatorsToUpdate.begin();
                  iter != translatorsToUpdate.end(); ++iter)
               {
                  CDagTranslator* translator3 = (*iter);
                  if (translator3 != NULL)
                  {
                     translator3->m_updateMode = AI_RECREATE_NODE;
                     translator3->RequestUpdate(static_cast<void*>(translator3));
                  }
               }
            }

         }
      }

      if(node.apiType() == MFn::kMesh && (plugName == ".pnts" || plugName == ".inMesh" || plugName == ".dispResolution" ||
         (plugName.length() > 9 && plugName.substring(0,8) == ".aiSubdiv"))/*|| node.apiType() == MFn::kPluginShape*/)
         translator->m_updateMode = AI_RECREATE_NODE;
      else if ((node.apiType() == MFn::kNurbsCurve) && (plugName == ".create"))
         translator->m_updateMode = AI_RECREATE_NODE;
      else
         translator->m_updateMode = AI_UPDATE_ONLY;
      
      const char* arnoldType = translator->GetArnoldTypeName();
      if(arnoldType && strcmp(arnoldType, "skydome_light") == 0)
      {
         CMayaScene::GetRenderSession()->InterruptRender();
         AiUniverseCacheFlush(AI_CACHE_BACKGROUND);
      }
         
      translator->RequestUpdate(clientData);
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: no translator in client data: %p.", translator->GetMayaNodeName().asChar(), clientData);
   }
}

void CNodeTranslator::NameChangedCallback(MObject& node, const MString& str, void* clientData)
// This is a simple callback triggered when the name changes.
{
   CNodeTranslator* translator = static_cast<CNodeTranslator*>(clientData);
   if (translator != NULL)
   {
      translator->SetArnoldNodeName(translator->GetArnoldRootNode());
      AiMsgDebug("[mtoa.translator.ipr]  %-30s | %s: NameChangedCallback: providing Arnold %s(%s): %p",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NameChangedCallback: no translator in client data: %p.", translator->GetMayaNodeName().asChar(), clientData);
   }

   CMayaScene::GetRenderSession()->ObjectNameChanged(node, str);
}

// Arnold doesn't really support deleting nodes. But we can make things invisible,
// disconnect them, turn them off, etc.
void CNodeTranslator::NodeDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData)
{
   CNodeTranslator* translator = static_cast<CNodeTranslator*>(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Node deleted, deleting processed translator instance, client data: %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), clientData);
      if(node.apiType() == MFn::kMesh || node.apiType() == MFn::kLight)
         translator->m_updateMode = AI_DELETE_NODE;
      translator->RequestUpdate(clientData);
   }
   else
   {
      // TODO: Shouldn't we avoid call translator->GetMayaNodeName().asChar() if translator is NULL?
      AiMsgWarning("[mtoa.translator.ipr] %-30s | Translator callback for node deleted, no translator in client data: %p.",
                   translator->GetMayaNodeName().asChar(), clientData);
   }
}

void CNodeTranslator::NodeDestroyedCallback(void* clientData)
{
   CNodeTranslator* translator = static_cast<CNodeTranslator*>(clientData);
   if (translator != NULL)
   {
      translator->RequestUpdate();
      translator->RemoveUpdateCallbacks();
      translator->Delete();  
   }
}

/// add this node's AOVs into the passed AOVSet
void CNodeTranslator::RequestUpdate(void *clientData)
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   CArnoldSession *session = CMayaScene::GetArnoldSession();

   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate: Arnold node %s(%s): %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
 
      if (session->IsInteractiveRender() && session->IsExportingMotion()) return;
      if (session->GetSessionMode() == MTOA_SESSION_RENDERVIEW)
      {
         
         if (!m_holdUpdates)
         {
            m_holdUpdates = true;
            // Add translator to the list of translators to update
            m_session->QueueForUpdate(translator);
         }
      } else
      {
         translator->RemoveUpdateCallbacks();
         // Add translator to the list of translators to update
         m_session->QueueForUpdate(translator);
      }
   }
   else
   {
      // Deletion doesn't pass a translator
      AiMsgDebug("[mtoa.translator.ipr] RequestUpdate: no translator in client data: %p.", clientData);
   }

   // Pass the update request to the export session
   m_session->RequestUpdate();
}

enum EAttributeDeclarationType{
   DECLARATION_CONSTANT = 1,
   DECLARATION_UNIFORM = 2,
   DECLARATION_VARYING = 3
};

template <signed ATTR>
void TExportArrayAttribute(AtArray* arr, MPlug& plug, unsigned int element) { }

template <>
void TExportArrayAttribute<AI_TYPE_BYTE>(AtArray* arr, MPlug& plug, unsigned int element)
{
   AiArraySetByte(arr, element, plug[element].asChar());
}

template <>
void TExportArrayAttribute<AI_TYPE_INT>(AtArray* arr, MPlug& plug, unsigned int element)
{
   AiArraySetInt(arr, element, plug[element].asInt());
}

template <>
void TExportArrayAttribute<AI_TYPE_BOOLEAN>(AtArray* arr, MPlug& plug, unsigned int element)
{
   AiArraySetBool(arr, element, plug[element].asBool());
}

template <>
void TExportArrayAttribute<AI_TYPE_FLOAT>(AtArray* arr, MPlug& plug, unsigned int element)
{
   AiArraySetFlt(arr, element, plug[element].asFloat());
}

template <>
void TExportArrayAttribute<AI_TYPE_RGB>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtRGB rgb = {p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat()};
   AiArraySetRGB(arr, element, rgb);
}

template <>
void TExportArrayAttribute<AI_TYPE_RGBA>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtRGBA rgba = {p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat(), p.child(3).asFloat()};
   AiArraySetRGBA(arr, element, rgba);
}

template <>
void TExportArrayAttribute<AI_TYPE_VECTOR>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtVector vec = {p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat()};
   AiArraySetVec(arr, element, vec);
}

template <>
void TExportArrayAttribute<AI_TYPE_POINT>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtPoint pnt = {p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat()};
   AiArraySetPnt(arr, element, pnt);
}

template <>
void TExportArrayAttribute<AI_TYPE_POINT2>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtPoint2 pnt2 = {p.child(0).asFloat(), p.child(1).asFloat()};
   AiArraySetPnt2(arr, element, pnt2);
}

template <>
void TExportArrayAttribute<AI_TYPE_STRING>(AtArray* arr, MPlug& plug, unsigned int element)
{
   AiArraySetStr(arr, element, plug[element].asString().asChar());
}

template <signed ATTR>
void TExportAttribute(AtNode* node, MPlug& plug, const char* attrName) { }

template <>
void TExportAttribute<AI_TYPE_BYTE>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetByte(node, attrName, plug.asChar());
}

template <>
void TExportAttribute<AI_TYPE_INT>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetInt(node, attrName, plug.asInt());
}

template <>
void TExportAttribute<AI_TYPE_BOOLEAN>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetBool(node, attrName, plug.asBool());
}

template <>
void TExportAttribute<AI_TYPE_FLOAT>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetFlt(node, attrName, plug.asFloat());
}

template <>
void TExportAttribute<AI_TYPE_RGB>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetRGB(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
}

template <>
void TExportAttribute<AI_TYPE_RGBA>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetRGBA(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), plug.child(3).asFloat());
}

template <>
void TExportAttribute<AI_TYPE_VECTOR>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetVec(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
}

template <>
void TExportAttribute<AI_TYPE_POINT>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetPnt(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
}

template <>
void TExportAttribute<AI_TYPE_POINT2>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetPnt2(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat());
}

template <>
void TExportAttribute<AI_TYPE_STRING>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetStr(node, attrName, plug.asString().asChar());
}

typedef bool (*declarationPointer)(AtNode*, const char*, unsigned int);

static declarationPointer declarationPointers[] = {
   0,
   AiNodeDeclareConstantArray,
   AiNodeDeclareUniform,
   AiNodeDeclareVarying
};

template <signed ATTR>
void TExportUserAttributeArray(AtNode* node, MPlug& plug, const char* attrName, EAttributeDeclarationType declType)
{
   if (declarationPointers[declType](node, attrName, ATTR))
   {
      const unsigned int numElements = plug.numElements();
      AtArray* arr = AiArrayAllocate(numElements, 1, ATTR);
      for (unsigned int i = 0; i < numElements; ++i)
         TExportArrayAttribute<ATTR>(arr, plug, i);
      AiNodeSetArray(node, attrName, arr);
   }
}

template <signed ATTR>
void TExportUserAttribute(AtNode* node, MPlug& plug, const char* attrName, EAttributeDeclarationType declType)
{
   if (plug.isArray())
      TExportUserAttributeArray<ATTR>(node, plug, attrName, declType);
   else
   {
      if (AiNodeDeclareConstant(node, attrName, ATTR))
         TExportAttribute<ATTR>(node, plug, attrName);
   }
}

template <signed ATTR, typename T>
void TExportUserAttributeData(AtArray* array, T& data, unsigned int element)
{
   
}

template <>
void TExportUserAttributeData<AI_TYPE_STRING, MFnStringArrayData>(AtArray* array, MFnStringArrayData& data, unsigned int element)
{
   AiArraySetStr(array, element, data[element].asChar());
}

template <>
void TExportUserAttributeData<AI_TYPE_FLOAT, MFnDoubleArrayData>(AtArray* array, MFnDoubleArrayData& data, unsigned int element)
{
   AiArraySetFlt(array, element, (float)data[element]);
}

template <>
void TExportUserAttributeData<AI_TYPE_INT, MFnIntArrayData>(AtArray* array, MFnIntArrayData& data, unsigned int element)
{
   AiArraySetInt(array, element, data[element]);
}

template <>
void TExportUserAttributeData<AI_TYPE_VECTOR, MFnVectorArrayData>(AtArray* array, MFnVectorArrayData& data, unsigned int element)
{
   AtVector vec = {(float)data[element].x, (float)data[element].y, (float)data[element].z};
   AiArraySetVec(array, element, vec);
}

template <>
void TExportUserAttributeData<AI_TYPE_RGB, MFnVectorArrayData>(AtArray* array, MFnVectorArrayData& data, unsigned int element)
{
   AtRGB rgb = {(float)data[element].x, (float)data[element].y, (float)data[element].z};
   AiArraySetRGB(array, element, rgb);
}

template <>
void TExportUserAttributeData<AI_TYPE_VECTOR, MFnPointArrayData>(AtArray* array, MFnPointArrayData& data, unsigned int element)
{
   AtVector vec = {(float)data[element].x, (float)data[element].y, (float)data[element].z};
   AiArraySetVec(array, element, vec);
}

template <>
void TExportUserAttributeData<AI_TYPE_RGB, MFnPointArrayData>(AtArray* array, MFnPointArrayData& data, unsigned int element)
{
   AtRGB rgb = {(float)data[element].x, (float)data[element].y, (float)data[element].z};
   AiArraySetRGB(array, element, rgb);
}

template <signed ATTR, typename T>
void TExportUserAttributeData(AtNode* node, MPlug& plug, const char* attrName, EAttributeDeclarationType declType)
{
   if (!plug.isArray())
   {
      if (declarationPointers[declType](node, attrName, ATTR))
      {
         T data(plug.asMObject());
         const unsigned int length = data.length();
         AtArray* arr = AiArrayAllocate(length, 1, ATTR);
         for (unsigned int i = 0; i < length; ++i)
            TExportUserAttributeData<ATTR, T>(arr, data, i);
         AiNodeSetArray(node, attrName, arr);
      }
   }
}

void CNodeTranslator::ExportUserAttributes(AtNode* anode, MObject object, CNodeTranslator* translator)
{
   MFnDependencyNode fnDepNode(object);

   for (unsigned int i=0; i<fnDepNode.attributeCount(); ++i)
   {
      MObject oAttr = fnDepNode.attribute(i);
      if (fnDepNode.attributeClass(oAttr) == MFnDependencyNode::kNormalAttr)
         continue; // we don`t need to check normal attributes,
      // they should be exported by the translator
      
      MPlug pAttr(object, oAttr);
      if (!pAttr.parent().isNull())
         continue;      
      // we only need to export the compound attribute, not the compounds itself
      // after the string comparisons, or before them? probably this one is faster

      MFnAttribute fnAttr(oAttr);      
      // The indexW in the MString is very slow!
      // so hard coding the check is a better option
      MString name = fnAttr.name();
      if (name.length() < 6)
         continue;
      const char *aname = name.asChar();
      EAttributeDeclarationType attributeDeclaration = DECLARATION_CONSTANT;
      if ((aname[0] != 'm') || (aname[1] != 't') ||
          (aname[2] != 'o') || (aname[3] != 'a') || (aname[4] != '_'))
         continue;
      aname += 5;
      if (strncmp(aname, "constant_", 9) == 0)
         aname += 9;
      else if (strncmp(aname, "uniform_", 8) == 0)
      {
         attributeDeclaration = DECLARATION_UNIFORM;
         aname += 8;
      }
      else if (strncmp(aname, "varying_", 8) == 0)
      {
         attributeDeclaration = DECLARATION_VARYING;
         aname += 8;
      }
      else
         AiMsgWarning("[mtoa] The mtoa_ prefix for constant attributes is deprecated, please use mtoa_constant_ for attribute: %s!", name.asChar());
      if (AiNodeLookUpUserParameter(anode, aname) != NULL)
         continue;

      if (translator)
         pAttr = translator->GetOverridePlug(pAttr);

      if (oAttr.hasFn(MFn::kNumericAttribute))
      {
         MFnNumericAttribute nAttr(oAttr);
         MFnNumericData::Type unitType = nAttr.unitType();
         switch (unitType)
         {
         case MFnNumericData::kBoolean:
            TExportUserAttribute<AI_TYPE_BOOLEAN>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::kByte:
         case MFnNumericData::kChar:
            TExportUserAttribute<AI_TYPE_BYTE>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::kShort:
         case MFnNumericData::kLong:
            TExportUserAttribute<AI_TYPE_INT>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::kFloat:
         case MFnNumericData::kDouble:
            TExportUserAttribute<AI_TYPE_FLOAT>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::k2Float:
         case MFnNumericData::k2Double:
            TExportUserAttribute<AI_TYPE_POINT2>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::k3Float:
         case MFnNumericData::k3Double:
            if (nAttr.isUsedAsColor())
               TExportUserAttribute<AI_TYPE_RGB>(anode, pAttr, aname, attributeDeclaration);
            else
               TExportUserAttribute<AI_TYPE_VECTOR>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnNumericData::k4Double:
            TExportUserAttribute<AI_TYPE_RGBA>(anode, pAttr, aname, attributeDeclaration);
            break;
         default:
            // not supported: k2Short, k2Long, k3Short, k3Long, kAddr
            //AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
            //      GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
            break;
         }
      }
      else if (oAttr.hasFn(MFn::kTypedAttribute))
      {
         MFnTypedAttribute tAttr(oAttr);
         const bool usedAsColor = tAttr.isUsedAsColor();
         switch (tAttr.attrType())
         {
         case MFnData::kString:
            TExportUserAttribute<AI_TYPE_STRING>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnData::kStringArray:
            TExportUserAttributeData<AI_TYPE_STRING, MFnStringArrayData>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnData::kDoubleArray:
            TExportUserAttributeData<AI_TYPE_FLOAT, MFnDoubleArrayData>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnData::kIntArray:
            TExportUserAttributeData<AI_TYPE_INT, MFnIntArrayData>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnData::kPointArray:
            if (usedAsColor)
               TExportUserAttributeData<AI_TYPE_RGB, MFnPointArrayData>(anode, pAttr, aname, attributeDeclaration);
            else
               TExportUserAttributeData<AI_TYPE_VECTOR, MFnPointArrayData>(anode, pAttr, aname, attributeDeclaration);
            break;
         case MFnData::kVectorArray:
            if (usedAsColor)
               TExportUserAttributeData<AI_TYPE_RGB, MFnVectorArrayData>(anode, pAttr, aname, attributeDeclaration);
            else
               TExportUserAttributeData<AI_TYPE_VECTOR, MFnVectorArrayData>(anode, pAttr, aname, attributeDeclaration);
            break;
         default:
            // kMatrix, kNumeric (this one should have be caught be hasFn(MFn::kNumericAttribute))
            //AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
            //   GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
            break;
         }
      }
      else if (oAttr.hasFn(MFn::kMessageAttribute) && (attributeDeclaration == DECLARATION_CONSTANT))
      {
         MPlugArray pArr;
         pAttr.connectedTo(pArr, true, false);
         if (pArr.length() > 0)
         {
            MPlug connectedPlug = pArr[0];
            AtNode* connectedNode = translator->ExportNode(connectedPlug);
            if (connectedNode != 0)
            {
               AiNodeDeclareConstant(anode, aname, AI_TYPE_NODE);
               AiNodeSetPtr(anode, aname, connectedNode);
            }
         }
      }      
      //else
      //   AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
      //         GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
   }
}

void CNodeTranslator::ExportUserAttribute(AtNode *anode)
{
   // TODO: allow overrides here too ?
   ExportUserAttributes(anode, GetMayaObject(), this);
   
   // Exporting the UnexposedOptions parameter
   MPlug plug = FindMayaPlug("aiUserOptions");
   if (!plug.isNull())
      AiNodeSetAttributes(anode, plug.asString().asChar());
}

/// Calls ExportNode and AiNodeLink if there are incoming connections to 'plug'
/// returns the connected arnold AtNode or NULL
AtNode* CNodeTranslator::ProcessParameterInputs(AtNode* arnoldNode, const MPlug &plug,
                                                const char* arnoldParamName,
                                                int arnoldParamType)
{
   MPlugArray connections;
   plug.connectedTo(connections, true, false);

   if (connections.length() > 0)
   {
      // process connections
      MPlug srcMayaPlug = connections[0];
      AtNode* srcArnoldNode = ExportNode(srcMayaPlug);

      if (srcArnoldNode == NULL)
         return NULL;

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

/// Called for compound plugs with float children. calls ProcessParameterInputs for each component
/// returns true if all components have incoming nodes.
bool CNodeTranslator::ProcessParameterComponentInputs(AtNode* arnoldNode, const MPlug &parentPlug,
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


/// Using the translator's m_handle Maya Object and corresponding attrbuteName (default behavior)
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType)
{
   MStatus status;

   // attr name name remap
   const AtNodeEntry* arnoldNodeEntry = AiNodeGetNodeEntry(arnoldNode);
   CBaseAttrHelper helper(arnoldNodeEntry);
   MFnDependencyNode fnNode(GetMayaObject());
   if (!helper.IsHidden(arnoldParamName))
   {
      // check paramName
      MString mayaAttribName = helper.GetMayaAttrName(arnoldParamName);
      MPlug plug = FindMayaPlug(mayaAttribName, &status);
      if ((MStatus::kSuccess != status) || plug.isNull())
      {
         // check aiParamName
         helper.SetPrefix("ai_");
         MString mayaExtAttribName = helper.GetMayaAttrName(arnoldParamName);
         plug = FindMayaPlug(mayaExtAttribName, &status);
         if ((MStatus::kSuccess != status) || plug.isNull())
         {
            AiMsgWarning("[mtoa.translator]  %s: Maya node %s(%s) does not have attribute %s or %s to match parameter %s on Arnold node %s(%s).",
                  GetTranslatorName().asChar(),
                  GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
                  mayaAttribName.asChar(), mayaExtAttribName.asChar(), arnoldParamName,
                  AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
            return NULL;
         }
         mayaAttribName = mayaExtAttribName;
      }
      return ProcessParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
   }
   else
   {
      AiMsgDebug("[mtoa.translator]  %s: Parameter %s is hidden on Arnold node %s(%s).",
            GetTranslatorName().asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(arnoldNodeEntry));
   }

   return NULL;
}

/// Using the translator's m_handle Maya Object and specific attrName
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType, const MString& mayaAttrName)
{
   MStatus status;

   // Get plug on the Maya object or override set if there is one
   MPlug plug = FindMayaPlug(mayaAttrName, &status);
   if ((MStatus::kSuccess == status) && !plug.isNull())
   {
      return ProcessParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
   }
   else
   {
      AiMsgWarning("[mtoa.translator]  %s: Maya node %s(%s) does not have attribute %s to match parameter %s on Arnold node %s(%s).",
            GetTranslatorName().asChar(),
            GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
            mayaAttrName.asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
   }

   return NULL;
}

/// Main entry point to export values to an arnold parameter from a maya plug, recursively following
/// connections in the dependency graph.
/// Calls ProcessParameterInputs for parameters that allow linking or ProcessConstantParameter
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType, const MPlug& plug)
{
   if (arnoldNode == NULL)
   {
      AiMsgError("[mtoa.translator]  %s: Cannot process parameter %s on null node.",
            GetTranslatorName().asChar(), arnoldParamName);
      return NULL;
   }
   if (plug.isNull())
   {
      AiMsgError("[mtoa.translator]  %s: Invalid Maya plug was passed as source for parameter %s on Arnold node %s(%s)",
            GetTranslatorName().asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
      return NULL;
   }

   // It doesn't make sense to call this method when step is greater than 0
   if (GetMotionStep() > 0)
   {
      AiMsgWarning("[mtoa] [translator %s] %s.%s: ProcessParameter should not be used on motion steps greater than 0",
            GetTranslatorName().asChar(), AiNodeGetName(arnoldNode), arnoldParamName);
      return NULL;
   }
   // Uncomment only when necessary, will be very verbose
   // AiMsgDebug("[mtoa.translator]  %s: Processing Maya %s(%s) for Arnold %s.%s(%s).",
   //      GetTranslatorName().asChar(),
   //      plug.name().asChar(), MFnDependencyNode(plug.node()).typeName().asChar(),
   //      AiNodeGetName(arnoldNode), arnoldParamName, AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));

   bool acceptLinks = false;
   // if the linkable metadata is not set, then only link if the node is a shader or param type is Node pointer
   if (!AiMetaDataGetBool(AiNodeGetNodeEntry(arnoldNode), arnoldParamName, "linkable", &acceptLinks))
   {
      if (arnoldParamType == AI_TYPE_NODE)
         acceptLinks = true;
      else
         acceptLinks = (AiNodeEntryGetType(AiNodeGetNodeEntry(arnoldNode)) & AI_NODE_SHADER) != 0;
   }
   // ignoreWhenRendering flag
   if (acceptLinks && plug.isIgnoredWhenRendering()) return NULL;

   if (acceptLinks)
   {
      // Unlink first, since this may be called during an IPR update
      // FIXME: unlinking a component plug during an IPR session will not properly update the arnold scene.
      // we do not call AiNodeUnlink inside ProcessParameterInputs because for components it results in the warning:
      //   cannot unlink "ramp2.color.r" as the attribute was not separately linked
      // In order to avoid this warning we would need to do the unlinking inside an attributeChanged callback, where
      // we could know for certain that a plug had been disconnected, instead of calling it blindly as we do now.
      if (AiNodeIsLinked(arnoldNode, arnoldParamName))
         AiNodeUnlink(arnoldNode, arnoldParamName);
      AtNode *connected = ProcessParameterInputs(arnoldNode, plug, arnoldParamName, arnoldParamType);
      // if we're connected, we're done, otherwise call ProcessConstantParameter
      if (connected != NULL)
         return connected;
   }

   return ProcessConstantParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
}

/// Export value for a plug with no direct connections (may have child or element connections).
/// For arrays, calls ProcessArrayParameter.
/// For simple numeric types, calls AiNodeSet*.
/// For compound types, first calls ProcessParameterComponentInputs. If ProcessParameterComponentInputs returns
/// false (not all components linked) then AiNodeSet* is called for all components.
AtNode* CNodeTranslator::ProcessConstantParameter(AtNode* arnoldNode, const char* arnoldParamName,
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
      ProcessArrayParameter(arnoldNode, arnoldParamName, plug);
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
         if (RequiresMotionData() && strcmp(arnoldParamName, "placementMatrix") == 0)
         {
            // create an interpolation node for matrices
            AtNode* animNode = AddArnoldNode("anim_matrix", arnoldParamName);
            AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);

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
            ConvertMatrix(am, mm, m_session);
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
         ProcessArrayParameter(arnoldNode, arnoldParamName, plug);
      }
      break;
   case AI_TYPE_BYTE:
      AiNodeSetByte(arnoldNode, arnoldParamName, (unsigned char)plug.asChar());
      break;
   }
   return NULL;
}

AtArray* CNodeTranslator::InitArrayParameter(unsigned int arnoldParamType, unsigned int size)
{
   return AiArrayAllocate(size, 1, arnoldParamType);
}

void CNodeTranslator::SetArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, AtArray* array)
{
   // TODO: for IPR: call AiArrayDestroy on pre-existing arrays?
   AiNodeSetArray(arnoldNode, arnoldParamName, array);
}

void CNodeTranslator::ProcessArrayParameterElement(AtNode* arnoldNode, AtArray* array, const char* arnoldParamName, const MPlug& elemPlug, unsigned int arnoldParamType, unsigned int pos)
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

/// Allocate an AtArray, ProcessConstantArrayElement to fill it with values from the array plug, and call AiNodeSetArray.
/// Also calls ProcessParameterInputs.
void CNodeTranslator::ProcessArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, const MPlug& plug)
{
   const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(arnoldNode), arnoldParamName);
   const AtParamValue* defaultValue = AiParamGetDefault(paramEntry);
   unsigned int arnoldParamType = defaultValue->ARRAY->type;
   // index matters tells us whether to condense a sparse array or try to export everything
//         int indexMatters = MFnAttribute(plug.attribute()).indexMatters();
//         MIntArray indices;
//         if (indexMattrs)
//         {
//            // do a little prep work so that we can have a unified processing loop below
//            for (unsigned int i = 0; i < plug.numElements(); ++i)
//               indices.append(i);
//         }
//         else
//            plug.getExistingArrayAttributeIndices(indices);

   // for now do all elements
   AtArray* array = NULL;
   if (arnoldParamType == AI_TYPE_NODE)
   {
      MPlugArray inputs;
      MPlugArray conn;

      for (unsigned int i=0; i < plug.numElements(); ++i)
      {
         MPlug elemPlug = plug[i];
         elemPlug.connectedTo(conn, true, false);
         if (conn.length() == 1)
            inputs.append(conn[0]);
      }
      unsigned int size = inputs.length();
      if (size > 0)
      {
         array = InitArrayParameter(arnoldParamType, size);
         for (unsigned int i=0; i < size; ++i)
         {
            AtNode* linkedNode = ExportNode(inputs[i]);
            AiArraySetPtr(array, i, linkedNode);
         }
         SetArrayParameter(arnoldNode, arnoldParamName, array);
      }
      else
         // TODO: Change this to: AiNodeSetArray(arnoldNode, arnoldParamName, NULL);
         // when the arnold bug causing a crash (reported on 16-Jan-2011) is fixed.
         AiNodeSetArray(arnoldNode, arnoldParamName, AiArrayAllocate(0,0, AI_TYPE_NODE));
   }
   else
   {
      unsigned int size = plug.numElements();
      if (size > 0)
      {
         AtArray* array = InitArrayParameter(arnoldParamType, size);
         MPlug elemPlug;
         for (unsigned int i = 0; i < size; ++i)
         {
           // cout << plug.partialName(true, false, false, false, false, true) << " index " << i << endl;
           //plug.selectAncestorLogicalIndex(i, plug.attribute());
           elemPlug = plug[i];

           ProcessArrayParameterElement(arnoldNode, array, arnoldParamName, elemPlug, arnoldParamType, i);
         }
         SetArrayParameter(arnoldNode, arnoldParamName, array);
      }
   }
}

void CNodeTranslator::ProcessConstantArrayElement(int type, AtArray* array, unsigned int i, const MPlug& elem)
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
         ConvertMatrix(am, mm, m_session);
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
            linkedNode = ExportNode(connections[0]);
         }
         AiArraySetPtr(array, i, linkedNode);
      }
      break;
   } // switch
}

/// for automatically creating parameters
void CNodeTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, context.arnold);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeEntryLookUp(context.arnold.asChar()));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!helper.IsHidden(paramName))
         helper.MakeInput(paramName);
   }
   AiParamIteratorDestroy(nodeParam);
}
