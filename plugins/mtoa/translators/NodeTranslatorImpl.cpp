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
      
      m_tr.ComputeAOVs();
      m_tr.Export(node);
      m_tr.ExportUserAttribute(node);
      m_tr.WriteAOVUserAttributes(node);
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

   m_isExported = true;
   return m_tr.GetArnoldRootNode();
}

// internal use only
AtNode* CNodeTranslatorImpl::DoUpdate()
{
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
      m_tr.Export(node);
      m_tr.ExportUserAttribute(node);
   }
   else if (m_tr.RequiresMotionData())
   {
      m_tr.ExportMotion(node);
   }
   m_isExported = true;

   return m_tr.GetArnoldRootNode();
}

void CNodeTranslatorImpl::DoCreateArnoldNodes()
{
   m_atNode = m_tr.CreateArnoldNodes();
   if (m_atNode == NULL)
      AiMsgDebug("[mtoa.translator]  %s (%s): Translator %s returned an empty Arnold root node.",
            m_tr.GetMayaNodeName().asChar(), m_tr.GetMayaNodeTypeName().asChar(), m_tr.GetTranslatorName().asChar());
   if (m_atNodes.count("") == 0)
      m_atNodes[""] = m_atNode;
}



/// Calls ExportNode and AiNodeLink if there are incoming connections to 'plug'
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
      CNodeTranslator* srcNodeTranslator = NULL;
      AtNode* srcArnoldNode = m_tr.ExportNode(srcMayaPlug, true, &srcNodeTranslator);

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




/// gather the active override sets containing this node
MStatus CNodeTranslatorImpl::ExportOverrideSets()
{
   MStatus status;
   m_overrideSets.clear();

   MObjectArray overrideSetObjs;

   if (m_tr.IsMayaTypeDag())
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


