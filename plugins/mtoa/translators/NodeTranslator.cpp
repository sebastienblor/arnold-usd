#include "platform/Platform.h"
#include "NodeTranslator.h"
#include "render/RenderOptions.h"
#include "extension/ExtensionsManager.h"
#include "attributes/Components.h"

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

#include <string>
#include <algorithm>
#include <iterator>

#include <ai_universe.h>
#include <assert.h>

#include "utils/time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace // <anonymous>
{
/*
   void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
   {
      for (int J = 0; (J < 4); ++J)
      {
         for (int I = 0; (I < 4); ++I)
         {
            matrix[I][J] = (float) mayaMatrix[I][J];
         }
      }
   }
*/

} // namespace

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

AtNode* CNodeTranslator::ExportNode(const MPlug& outputPlug, bool track)
{
   CNodeTranslator* translator = NULL;
   if (track)
      translator = m_session->ExportNode(outputPlug, m_shaders, &m_upstreamAOVs);
   else
      translator = m_session->ExportNode(outputPlug);
   if (translator != NULL)
      return translator->GetArnoldRootNode();
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
   MStatus status(MStatus::kSuccess);
   MPlug plug;
   // Check if a set override this plug's value
   std::vector<CNodeTranslator*>::iterator it;
   std::vector<CNodeTranslator*> translators;
   unsigned int novr = m_overrideSets.size();
   for (unsigned int i=0; i<novr; i++)
   {
      CNodeTranslator* translator = m_overrideSets[i];

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
         if (MStatus::kSuccess == fnSet.setObject(set))
         {
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

   if (node != NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | %s: Exporting Arnold %s(%s): %p",
                 GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
                 AiNodeGetName(node), AiNodeEntryGetName(AiNodeGetNodeEntry(node)),
                 node);
   }
   else
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Export requested but no Arnold node was created by this translator (%s)",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar());
      return NULL;
   }

   if (step == 0)
   {
      if (outputAttr != "")
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting on plug %s (%s)",
                    GetMayaNodeName().asChar(), outputAttr.asChar(), GetTranslatorName().asChar());
      else
         AiMsgDebug("[mtoa.translator]  %-30s | Exporting (%s)",
                    GetMayaNodeName().asChar(), GetTranslatorName().asChar());
      ExportOverrideSets();
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

   if (node != NULL)
   {
      AiMsgDebug("[mtoa.translator]  %-30s | %s: Updating Arnold %s(%s): %p",
                 GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
                 AiNodeGetName(node), AiNodeEntryGetName(AiNodeGetNodeEntry(node)),
                 node);
   }
   else
   {
      AiMsgDebug("[mtoa.translator]  %-30s | Update requested but no Arnold node was created by this translator (%s)",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar());
   }

   if (step == 0)
   {
      ExportOverrideSets();
      Update(node);
      ExportUserAttribute(node);
   }
   else if (RequiresMotionData())
   {
      UpdateMotion(node, step);
   }

   return GetArnoldRootNode();
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
void CNodeTranslator::ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
{
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) mayaMatrix[I][J];
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


// This is a simple callback triggered when a node is marked as dirty.
void CNodeTranslator::NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData)
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: plug that fired: %s, client data: %p.",
         MFnDependencyNode(node).name().asChar(), plug.name().asChar(), clientData);

   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: client data is translator %s, providing Arnold %s(%s): %p",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      translator->RequestUpdate(clientData);
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: no translator in client data: %p.", translator->GetMayaNodeName().asChar(), clientData);
   }
}

void CNodeTranslator::NameChangedCallback(MObject &node, const MString &str, void *clientData)
// This is a simple callback triggered when the name changes.
{
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
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
}

// Arnold doesn't really support deleting nodes. But we can make things invisible,
// disconnect them, turn them off, etc.
void CNodeTranslator::NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData)
{
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Node deleted, deleting processed translator instance, client data: %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), clientData);
      translator->RequestUpdate();
      translator->RemoveUpdateCallbacks();
      translator->Delete();
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | Translator callback for node deleted, no translator in client data: %p.",
                   translator->GetMayaNodeName().asChar(), clientData);
   }
}

/// add this node's AOVs into the passed AOVSet
void CNodeTranslator::RequestUpdate(void *clientData)
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate: Arnold node %s(%s): %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      translator->RemoveUpdateCallbacks();
      // Add translator to the list of translators to update
      m_session->QueueForUpdate(translator);
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
   DECLARATION_CONSTANT = 0,
   DECLARATION_UNIFORM = 1,
   DECLARATION_VARYING = 2
};

void CNodeTranslator::ExportUserAttribute(AtNode *anode)
{
   // TODO: allow overrides here too ?

   MObject object = GetMayaObject();
   MFnDependencyNode fnDepNode(GetMayaObject());


   for (unsigned int i=0; i<fnDepNode.attributeCount(); ++i)
   {
      MObject oAttr = fnDepNode.attribute(i);
      if (fnDepNode.attributeClass(oAttr) == MFnDependencyNode::kNormalAttr)
         continue; // we don`t need to check normal attributes,
      // they should be exported by the translator

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
      if (strncmp(aname, "uniform_", 8) == 0)
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
         AiMsgWarning("[mtoa] The mtoa_ prefix for constant attributes is deprecated, please use mtoa_constant_!");
      if (AiNodeLookUpUserParameter(anode, aname) != NULL)
         continue;
      MPlug pAttr(object, oAttr);
      if (oAttr.hasFn(MFn::kNumericAttribute))
      {
         MFnNumericAttribute nattr(oAttr);
         switch (nattr.unitType())
         {
         case MFnNumericData::kBoolean:
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY BOOL",
                                                  "uniform BOOL",
                                                  "varying BOOL"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_BOOLEAN);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     AiArraySetBool(ary, i, pAttr[i].asBool());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant BOOL"))
               {
                  AiNodeSetBool(anode, aname, pAttr.asBool());
               }
            }
            break;
         case MFnNumericData::kByte:
         case MFnNumericData::kChar:
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY BYTE",
                                                  "uniform BYTE",
                                                  "varying BYTE"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_BYTE);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     AiArraySetBool(ary, i, pAttr[i].asBool());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant BYTE"))
               {
                  AiNodeSetByte(anode, aname, pAttr.asChar());
               }
            }
            break;
         case MFnNumericData::kShort:
         case MFnNumericData::kLong:
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY INT",
                                                  "uniform INT",
                                                  "varying INT"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_INT);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     AiArraySetInt(ary, i, pAttr[i].asInt());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant INT"))
               {
                  AiNodeSetInt(anode, aname, pAttr.asInt());
               }
            }
            break;
         case MFnNumericData::kFloat:
         case MFnNumericData::kDouble:
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY FLOAT",
                                                  "uniform FLOAT",
                                                  "varying FLOAT"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_FLOAT);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     AiArraySetFlt(ary, i, pAttr[i].asFloat());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant FLOAT"))
               {
                  AiNodeSetFlt(anode, aname, pAttr.asFloat());
               }
            }
            break;
         case MFnNumericData::k2Float:
         case MFnNumericData::k2Double:
            if (pAttr.isArray())
            {
               AtPoint2 pnt2;
               static const char* declString[] = {"constant ARRAY POINT2",
                                                  "uniform POINT2",
                                                  "varying POINT2"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_POINT2);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     MFnNumericData data(pAttr[i].asMObject());
                     data.getData2Float(pnt2.x, pnt2.y);
                     AiArraySetPnt2(ary, i, pnt2);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant POINT2"))
               {
                  AtPoint2 pnt2;
                  MFnNumericData data(pAttr.asMObject());
                  data.getData2Float(pnt2.x, pnt2.y);
                  AiNodeSetPnt2(anode, aname, pnt2.x, pnt2.y);
               }
            }
            break;
         case MFnNumericData::k3Float:
         case MFnNumericData::k3Double:
            // point? vector? rgb?
            if (pAttr.isArray())
            {
               AtVector vec;
               static const char* declString[] = {"constant ARRAY VECTOR",
                                                  "uniform VECTOR",
                                                  "varying VECTOR"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_VECTOR);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     MFnNumericData data(pAttr[i].asMObject());
                     data.getData3Float(vec.x, vec.y, vec.z);
                     AiArraySetVec(ary, i, vec);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant VECTOR"))
               {
                  AtVector vec;
                  MFnNumericData data(pAttr.asMObject());
                  data.getData3Float(vec.x, vec.y, vec.z);
                  AiNodeSetVec(anode, aname, vec.x, vec.y, vec.z);
               }
            }
            break;
         case MFnNumericData::k4Double:
            // rgba? homogeneous point?
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY RGBA",
                                                  "uniform RGBA",
                                                  "varying RGBA"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtRGBA rgba;
                  double r, g, b, a;
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_RGBA);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     MFnNumericData data(pAttr[i].asMObject());
                     data.getData4Double(r, g, b, a);
                     rgba.r = static_cast<float>(r);
                     rgba.g = static_cast<float>(g);
                     rgba.b = static_cast<float>(b);
                     rgba.a = static_cast<float>(a);
                     AiArraySetRGBA(ary, i, rgba);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant RGBA"))
               {
                  double r, g, b, a;
                  MFnNumericData data(pAttr.asMObject());
                  data.getData4Double(r, g, b, a);
                  AiNodeSetRGBA(anode, aname, static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a));
               }
            }
            break;
         default:
            // not supported: k2Short, k2Long, k3Short, k3Long, kAddr
            AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
                  GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
            break;
         }
      }
      else if (oAttr.hasFn(MFn::kTypedAttribute))
      {
         MFnTypedAttribute tattr(oAttr);
         switch (tattr.attrType())
         {
         case MFnData::kString:
            if (pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY STRING",
                                                  "uniform STRING",
                                                  "varying STRING"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtArray *ary = AiArrayAllocate(pAttr.numElements(), 1, AI_TYPE_STRING);
                  for (unsigned int i=0; i<pAttr.numElements(); ++i)
                  {
                     AiArraySetStr(ary, i, pAttr[i].asString().asChar());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            else
            {
               if (AiNodeDeclare(anode, aname, "constant STRING"))
               {
                  AiNodeSetStr(anode, aname, pAttr.asString().asChar());
               }
            }
            break;
         case MFnData::kStringArray:
            if (!pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY STRING",
                                                  "uniform STRING",
                                                  "varying STRING"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  MFnStringArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_STRING);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetStr(ary, i, data[i].asChar());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            break;
         case MFnData::kDoubleArray:
            if (!pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY FLOAT",
                                                  "uniform FLOAT",
                                                  "varying FLOAT"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  MFnDoubleArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_FLOAT);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetFlt(ary, i, static_cast<float>(data[i]));
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            break;
         case MFnData::kIntArray:
            if (!pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY INT",
                                       "uniform INT",
                                       "varying INT"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  MFnIntArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_INT);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetInt(ary, i, data[i]);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            break;
         case MFnData::kPointArray:
            if (!pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY POINT",
                                       "uniform POINT",
                                       "varying POINT"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtPoint pnt;
                  MFnPointArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_POINT);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     pnt.x = static_cast<float>(data[i].x);
                     pnt.y = static_cast<float>(data[i].y);
                     pnt.z = static_cast<float>(data[i].z);
                     AiArraySetPnt(ary, i, pnt);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            break;
         case MFnData::kVectorArray:
            if (!pAttr.isArray())
            {
               static const char* declString[] = {"constant ARRAY VECTOR",
                                       "uniform VECTOR",
                                       "varying VECTOR"};
               if (AiNodeDeclare(anode, aname, declString[attributeDeclaration]))
               {
                  AtVector vec;
                  MFnVectorArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_VECTOR);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     vec.x = static_cast<float>(data[i].x);
                     vec.y = static_cast<float>(data[i].y);
                     vec.z = static_cast<float>(data[i].z);
                     AiArraySetVec(ary, i, vec);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
            }
            break;
         default:
            // kMatrix, kNumeric (this one should have be caught be hasFn(MFn::kNumericAttribute))
            AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
               GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
            break;
         }
      }
      else
         AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
               GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
   }
   
   // Exporting the UnexposedOptions parameter
   
   MPlug plug = fnDepNode.findPlug("aiUserOptions");
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
   // if the linkable metadata is not set, then only link if the node is a shader
   if (!AiMetaDataGetBool(AiNodeGetNodeEntry(arnoldNode), arnoldParamName, "linkable", &acceptLinks))
      acceptLinks = ((AiNodeEntryGetType(AiNodeGetNodeEntry(arnoldNode)) & AI_NODE_SHADER) != 0) ? true : false;

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
         AiNodeSetFlt(arnoldNode, arnoldParamName, plug.asFloat());
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
            ConvertMatrix(am, mm);
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
   }
   return NULL;
}

AtArray* CNodeTranslator::InitArrayParameter(unsigned int arnoldParamType, unsigned int size)
{
   return AiArrayAllocate(size, 1, arnoldParamType);
}

void CNodeTranslator::SetArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, AtArray* array)
{
   AiNodeSetArray(arnoldNode, arnoldParamName, array);
}

void CNodeTranslator::ProcessArrayParameterElement(AtNode* arnoldNode, AtArray* array, const char* arnoldParamName, const MPlug& elemPlug, unsigned int arnoldParamType, unsigned int pos)
{
   // connections:
   // An AI_TYPE_NODE param is controlled via a Maya message attribute. Unlike numeric attributes, in Maya
   // there is no way of assigning the value of a message attribute other than via a connection.
   // Therefore, we handle node/message connections in ProcessArrayElement
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
         ConvertMatrix(am, mm);
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

//------------ CDagTranslator ------------//
/// get override sets containing the passed Maya dag path
/// and add them to the passed MObjectArray
MStatus CDagTranslator::GetOverrideSets(MDagPath path, MObjectArray &overrideSets)
{
   MStatus status;

   MFnDagNode fnDag(path);
   unsigned int instNum = path.instanceNumber();
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
         if (MStatus::kSuccess == fnSet.setObject(set))
         {
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
/// and export them
MStatus CDagTranslator::ExportOverrideSets()
{
   MStatus status;

   m_overrideSets.clear();
   MDagPath path = m_dagPath;
   // Check for passed path
   MObjectArray overrideSetObjs;
   status = GetOverrideSets(path, overrideSetObjs);
   // If passed path is a shape, check for its transform as well
   // FIXME: do we want to consider full hierarchy ?
   // Also consider the sets the transform of that shape might be in
   const MObject transformObj = path.transform(&status);
   while ((MStatus::kSuccess == status) && (transformObj != path.node(&status)))
   {
      status = path.pop();
   }
   if (!(path == m_dagPath))
   {
      status = GetOverrideSets(path, overrideSetObjs);
   }
   // Exporting a set creates no Arnold object but allow IPR to track it
   MFnSet fnSet;
   unsigned int ns = overrideSetObjs.length();
   for (unsigned int i=0; i<ns; i++)
   {
      fnSet.setObject(overrideSetObjs[i]);
      m_overrideSets.push_back(m_session->ExportNode(fnSet.findPlug("message")));
   }

   return status;
}

/// set the name of the arnold node
void CDagTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = m_dagPath.partialPathName();
   // TODO: add a global option to control how names are exported
   // MString name = m_dagPath.fullPathName();
   if (DependsOnOutputPlug())
   {
      MString outputAttr = GetMayaAttributeName();

      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (strlen(tag))
      name = name + AI_TAG_SEP + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}

void CDagTranslator::AddHierarchyCallbacks(const MDagPath & path)
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Add DAG parents update callbacks for translator %p",
      path.partialPathName().asChar(), GetTranslatorName().asChar(), this);

   // Loop through the whole dag path adding callbacks to them.
   MStatus status;
   MDagPath dag_path(path);
   dag_path.pop(); // Pop of the shape as that's handled by CNodeTranslator::AddUpdateCallbacks.
   for(; dag_path.length() > 0; dag_path.pop())
   {
      MObject node = dag_path.node();
      if (node != MObject::kNullObj)
      {
         // We can use the normal NodeDirtyCallback here.
         MCallbackId id = MNodeMessage::addNodeDirtyCallback(node,
                                                             NodeDirtyCallback,
                                                             this,
                                                             &status);
         if (MS::kSuccess == status) ManageUpdateCallback(id);
      }
   }
}


void CDagTranslator::AddUpdateCallbacks()
{
   AddHierarchyCallbacks(m_dagPath);

   // Call the base class to get the others.
   CNodeTranslator::AddUpdateCallbacks();
}

void CDagTranslator::Delete()
{
   //AiNodeDestroy(GetArnoldRootNode());

   // Arnold doesn't allow us to create nodes in between to calls to AiRender
   // for the moment. For IPR we still need to rely on setting the visibility for now.
   AiNodeSetInt(GetArnoldRootNode(), "visibility",  AI_RAY_UNDEFINED);

}

// Return whether the dag object in dagPath is the master instance. The master
// is the first instance that is completely visible (including parent transforms)
// for which full geometry should be exported
//
// always returns true if dagPath is not instanced.
// if dagPath is instanced, this searches the preceding instances
// for the first that is visible. if none are found, dagPath is considered the master.
//
// note: dagPath is assumed to be visible.
//
// @param[out] masterDag    the master MDagPath result, only filled if result is false
// @return                  whether or not dagPath is a master
//
bool CDagTranslator::IsMasterInstance(MDagPath &masterDag)
{
   if (m_dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(m_dagPath.node());
      unsigned int instNum = m_dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed m_dagPath is assumed to be visible)
         m_session->AddMasterInstanceHandle(handle, m_dagPath);
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = m_session->GetMasterInstanceDagPath(handle);
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(m_dagPath.node(), allInstances);
         unsigned int master_index = 0;
         for (; (master_index < m_dagPath.instanceNumber()); master_index++)
         {
            currDag = allInstances[master_index];
            if (m_session->IsRenderablePath(currDag))
            {
               // found it
               m_session->AddMasterInstanceHandle(handle, currDag);
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: m_dagPath is the master
         m_session->AddMasterInstanceHandle(handle, m_dagPath);
         return true;
      }
   }
   // not instanced: m_dagPath is the master
   return true;
}

void CDagTranslator::GetRotationMatrix(AtMatrix& matrix)
{
   MObject transform = m_dagPath.transform();
   MFnTransform mTransform;
   mTransform.setObject(transform);
   MTransformationMatrix mTransformMatrix = mTransform.transformation();

   MMatrix tm = mTransformMatrix.asRotateMatrix();
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
}

void CDagTranslator::GetMatrix(AtMatrix& matrix, const MDagPath& path)
{
   MStatus stat;
   MMatrix tm = path.inclusiveMatrix(&stat);
   if (MStatus::kSuccess != stat)
   {
      AiMsgError("Failed to get transformation matrix for %s",  path.partialPathName().asChar());
   }
   ConvertMatrix(matrix, tm);
}

void CDagTranslator::GetMatrix(AtMatrix& matrix)
{
   GetMatrix(matrix, m_dagPath);
}

// this is a utility method which handles the common tasks associated with
// exporting matrix information. it properly handles exporting a matrix array
// if motion blur is enabled and required by the node. it should be called
// at each motion step
void CDagTranslator::ExportMatrix(AtNode* node, unsigned int step)
{
   AtMatrix matrix;
   GetMatrix(matrix);
   if (step == 0)
   {
      if (RequiresMotionData())
      {
         AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, 0, matrix);
         AiNodeSetArray(node, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(node, "matrix", matrix);
      }
   }
   else
   {
      AtArray* matrices = AiNodeGetArray(node, "matrix");
      AiArraySetMtx(matrices, step, matrix);
   }
}

int CDagTranslator::ComputeVisibility(const MDagPath& path)
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == m_session->IsRenderablePath(path))
      return AI_RAY_UNDEFINED;

   int visibility = AI_RAY_ALL;
   MPlug plug;

   plug = FindMayaPlug("castsShadows");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = FindMayaPlug("primaryVisibility");
   MString plugName = plug.name();
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }

   plug = FindMayaPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFLECTED;
   }

   plug = FindMayaPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFRACTED;
   }

   plug = FindMayaPlug("aiVisibleInDiffuse");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = FindMayaPlug("aiVisibleInGlossy");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }

   return visibility;
}

// use standardized render flag names to compute an arnold visibility mask
int CDagTranslator::ComputeVisibility()
{
   return ComputeVisibility(m_dagPath);
}

// Create Maya visibility attributes with standardized render flag names
//
// These are the attributes that compute the "visibility" parameter. there are other
// attributes like self_shadow and opaque that are computed separately
//
// This is for custom DAG nodes where none of the standard maya visibility attributes
// are available. typically CDagTranslator::AddArnoldVisibilityAttrs() is the appropriate function.
//
void CDagTranslator::MakeMayaVisibilityFlags(CBaseAttrHelper& helper)
{
   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "receiveShadows";
   data.shortName = "rsh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "visibleInReflections";
   data.shortName = "vir";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "visibleInRefractions";
   data.shortName = "vif";
   helper.MakeInputBoolean(data);
}

// create arnold visibility attributes with standardized render flag names
//
// These are the attributes that help compute the "visibility" parameter. there are other
// attributes like self_shadow and opaque that are computed separately
//
// arnold's visibiltity mask adds several relationships not available by default in Maya.
// use in conjunction with CDagTranslator::ComputeVisibility() or CShapeTranslator::ProcessRenderFlags().
//
void CDagTranslator::MakeArnoldVisibilityFlags(CBaseAttrHelper& helper)
{
   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "aiVisibleInDiffuse";
   data.shortName = "ai_vid";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "aiVisibleInGlossy";
   data.shortName = "ai_vig";
   helper.MakeInputBoolean(data);
}
