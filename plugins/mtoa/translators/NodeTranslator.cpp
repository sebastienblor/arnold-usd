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

#include <ai_universe.h>
#include <assert.h>


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


//------------ CNodeTranslator ------------//

// internal use only
AtNode* CNodeTranslator::DoExport(unsigned int step)
{
   AtNode* node = GetArnoldNode("");
   if (node != NULL)
   {
      m_step = step;
      if (step == 0)
      {
         if (m_outputAttr != "")
            AiMsgDebug("[mtoa] [translator %s] Exporting on plug %s.%s and upstream.",
                  GetTranslatorName().asChar(), GetMayaNodeName().asChar(), m_outputAttr.asChar());
         else
            AiMsgDebug("[mtoa] [translator %s] Exporting on node %s and upstream.",
                  GetTranslatorName().asChar(), GetMayaNodeName().asChar());
         Export(node);
         ExportUserAttribute(node);
      }
      else if (RequiresMotionData())
      {
         if (m_outputAttr != "")
            AiMsgDebug("[mtoa] [translator %s] Exporting motion on plug %s.%s and upstream..",
                  GetTranslatorName().asChar(), GetMayaNodeName().asChar(), m_outputAttr.asChar());
         else
            AiMsgDebug("[mtoa] [translator %s] Exporting motion on node %s and upstream.",
                  GetTranslatorName().asChar(), GetMayaNodeName().asChar());

         ExportMotion(node, step);
      }
   }
   return GetArnoldRootNode();
}

// internal use only
AtNode* CNodeTranslator::DoUpdate(unsigned int step)
{
   assert(AiUniverseIsActive());
   AtNode* node = GetArnoldNode("");

   AiMsgDebug("[mtoa] [translator %s] CNodeTranslator::DoUpdate step %i, node %p",
         GetTranslatorName().asChar(), step, node);

   if (node != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] Updating Arnold %s(%s) translated from Maya %s(%s): %p",
         GetTranslatorName().asChar(),
         AiNodeGetName(m_atNode), AiNodeEntryGetName(AiNodeGetNodeEntry(m_atNode)),
         GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(), m_atNode);
      if (step == 0)
      {
         Update(node);
         ExportUserAttribute(node);
      }
      else if (RequiresMotionData())
         UpdateMotion(node, step);
   }
   else
   {
      AiMsgWarning("[mtoa] [translator %s] Update requested but no Arnold node was created by this translator for Maya %s(%s)",
         GetTranslatorName().asChar(), GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar());
   }

   return GetArnoldRootNode();
}

AtNode* CNodeTranslator::DoCreateArnoldNodes()
{
   m_atNode = CreateArnoldNodes();
   if (m_atNode == NULL)
      AiMsgWarning("[mtoa] [translator %s] Translation on %s returned an empty Arnold root node.",
            GetTranslatorName().asChar(), GetMayaNodeName().asChar());
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
      AiMsgError("[mtoa] [translator %s] Translation has not created an Arnold node with tag \"%s\".", GetTranslatorName().asChar(), tag);
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
      AiMsgError("[mtoa] [translator %s] Arnold node type %s does not exist.", GetTranslatorName().asChar(), type);
      return NULL;
   }
}

void CNodeTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = GetMayaNodeName();
   if (m_outputAttr.numChars())
      name = name + AI_ATT_SEP + m_outputAttr;
   if (strlen(tag))
      name = name + AI_TAG_SEP + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
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
   AiMsgDebug("[mtoa] [%s] Add Update callbacks", GetMayaNodeName().asChar());
   MStatus status;
   MCallbackId id;
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(m_object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(m_object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(m_object,
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
   AiMsgDebug("[mtoa] [translator] NodeDirtyCallback on node %s, plug that fired: %s, client data: %p.",
         MFnDependencyNode(node).name().asChar(), plug.name().asChar(), clientData);

   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator] NodeDirtyCallback: client data is translator %s on Maya node %s(%s), providing Arnold %s(%s): %p",
            translator->GetTranslatorName().asChar(),
            translator->GetMayaNodeName().asChar(), translator->GetMayaNodeTypeName().asChar(),
            translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      translator->RequestUpdate(clientData);
   }
   else
   {
      AiMsgWarning("[mtoa] [translator] NodeDirtyCallback: no translator in client data: %p.", clientData);
   }
}

void CNodeTranslator::NameChangedCallback(MObject &node, const MString &str, void *clientData)
// This is a simple callback triggered when the name changes.
{
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] Node name changed, updating Arnold, client data: %p.",
            translator->GetTranslatorName().asChar(), clientData);
      translator->SetArnoldNodeName(translator->GetArnoldRootNode());
      AiMsgDebug("[mtoa] [translator] NameChangedCallback: client data is translator %s on Maya node %s(%s), providing Arnold %s(%s): %p",
            translator->GetTranslatorName().asChar(),
            translator->GetMayaNodeName().asChar(), translator->GetMayaNodeTypeName().asChar(),
            translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
   }
   else
   {
      AiMsgWarning("[mtoa] [translator] NameChangedCallback: no translator in client data: %p.", clientData);
   }
}

// Arnold doesn't really support deleting nodes. But we can make things invisible,
// disconnect them, turn them off, etc.
void CNodeTranslator::NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData)
{
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] Node deleted, deleting processed translator instance, client data: %p.",
            translator->GetTranslatorName().asChar(), clientData);
      translator->RequestUpdate();
      translator->RemoveUpdateCallbacks();
      translator->Delete();
   }
   else
   {
      AiMsgWarning("[mtoa] Translator callback for node deleted, no translator in client data: %p.", clientData);
   }
}


void CNodeTranslator::RequestUpdate(void *clientData)
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] RequestUpdate on Maya node %s(%s) for Arnold node %s(%s): %p.",
            translator->GetTranslatorName().asChar(),
            translator->GetMayaNodeName().asChar(), translator->GetMayaNodeTypeName().asChar(),
            translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      translator->RemoveUpdateCallbacks();
      // Add translator to the list of translators to update
      m_session->QueueForUpdate(translator);
   }
   else
   {
      AiMsgDebug("[mtoa] [translator] RequestUpdate: no translator in client data: %p.", clientData);
   }

   // Pass the update request to the export session
   m_session->RequestUpdate();
}

void CNodeTranslator::ExportUserAttribute(AtNode *anode)
{
   MFnDependencyNode fnDepNode(GetMayaObject());

   for (unsigned int i=0; i<fnDepNode.attributeCount(); ++i)
   {
      MObject oAttr = fnDepNode.attribute(i);

      MFnAttribute fnAttr(oAttr);
      MPlug pAttr(m_object, oAttr);

      MString name = fnAttr.name();
      if (name.indexW("mtoa_") == 0)
      {
         const char *aname = name.asChar() + 5;
         if (AiNodeLookUpUserParameter(anode, aname) != NULL)
         {
            continue;
         }
         if (oAttr.hasFn(MFn::kNumericAttribute))
         {
            MFnNumericAttribute nattr(oAttr);
            switch (nattr.unitType())
            {
            case MFnNumericData::kBoolean:
               if (pAttr.isArray())
               {
                  if (AiNodeDeclare(anode, aname, "constant ARRAY BOOL"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY BYTE"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY INT"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY FLOAT"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY POINT2"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY VECTOR"))
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY RGBA"))
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
               AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
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
                  if (AiNodeDeclare(anode, aname, "constant ARRAY STRING"))
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
               if (!pAttr.isArray() && AiNodeDeclare(anode, aname, "constant ARRAY STRING"))
               {
                  MFnStringArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_STRING);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetStr(ary, i, data[i].asChar());
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
               break;
            case MFnData::kDoubleArray:
               if (!pAttr.isArray() && AiNodeDeclare(anode, aname, "constant ARRAY FLOAT"))
               {
                  MFnDoubleArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_FLOAT);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetFlt(ary, i, static_cast<float>(data[i]));
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
               break;
            case MFnData::kIntArray:
               if (!pAttr.isArray() && AiNodeDeclare(anode, aname, "constant ARRAY INT"))
               {
                  MFnIntArrayData data(pAttr.asMObject());
                  AtArray *ary = AiArrayAllocate(data.length(), 1, AI_TYPE_INT);
                  for (unsigned int i=0; i<data.length(); ++i)
                  {
                     AiArraySetInt(ary, i, data[i]);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
               break;
            case MFnData::kPointArray:
               if (!pAttr.isArray() && AiNodeDeclare(anode, aname, "constant ARRAY POINT"))
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
               break;
            case MFnData::kVectorArray:
               if (!pAttr.isArray() && AiNodeDeclare(anode, aname, "constant ARRAY VECTOR"))
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
               break;
            default:
               // kMatrix, kNumeric (this one should have be caught be hasFn(MFn::kNumericAttribute))
               AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
                  GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
               break;
            }
         }
         else
            AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
                  GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
      }
   }
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


// Using the translator's MObject m_object and corresponding attrbuteName (default behavior)
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType, int element)
{
   MStatus status;

   // attr name name remap
   const AtNodeEntry* arnoldNodeEntry = AiNodeGetNodeEntry(arnoldNode);
   CBaseAttrHelper helper(arnoldNodeEntry);
   if (!helper.IsHidden(arnoldParamName))
   {
      MString mayaAttribName = helper.GetMayaAttrName(arnoldParamName);
      MPlug plug = m_fnNode.findPlug(mayaAttribName, true, &status);

      if (MStatus::kSuccess == status && !plug.isNull())
      {
         if (element >= 0)
         {
            // FIXME: I'd expect elementByLogicalIndex for a user passed parameter
            // but I've never seen element used in the code anyway
            if (plug.isArray())
            {
               plug = plug.elementByPhysicalIndex(element);
            }
            else
            {
               AiMsgWarning("[mtoa] [translator %s] Plug %s does not represent a multi attribute, can't get element %i.",
                     GetTranslatorName().asChar(), m_fnNode.name().asChar(), element);
            }
         }
         return ProcessParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] Maya node %s(%s) does not have attribute %s to match parameter %s on Arnold node %s(%s).",
               GetTranslatorName().asChar(),
               GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
               mayaAttribName.asChar(), arnoldParamName,
               AiNodeGetName(arnoldNode), AiNodeEntryGetName(arnoldNodeEntry));
      }
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
      AiMsgError("[mtoa] [translator %s] Cannot process parameter %s on null node.",
            GetTranslatorName().asChar(), arnoldParamName);
      return NULL;
   }
   if (plug.isNull())
   {
      AiMsgError("[mtoa] [translator %s] Null Maya plug was passed as source for parameter %s on Arnold node %s(%s)",
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
   // AiMsgDebug("[mtoa] [translator %s] Processing Maya %s(%s) for Arnold %s.%s(%s).",
   //      GetTranslatorName().asChar(),
   //      plug.name().asChar(), MFnDependencyNode(plug.node()).typeName().asChar(),
   //      AiNodeGetName(arnoldNode), arnoldParamName, AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));

   AtBoolean acceptLinks = FALSE;
   // if the linkable metadata is not set, then only link if the node is a shader
   if (!AiMetaDataGetBool(AiNodeGetNodeEntry(arnoldNode), arnoldParamName, "linkable", &acceptLinks))
      acceptLinks = ((AiNodeEntryGetType(AiNodeGetNodeEntry(arnoldNode)) & AI_NODE_SHADER) != 0) ? TRUE : FALSE;

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
   switch(arnoldParamType)
   {
   case AI_TYPE_RGB:
      {
         if (!ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
         {
            AiNodeSetRGB(arnoldNode, arnoldParamName,
                         plug.child(0).asFloat(),
                         plug.child(1).asFloat(),
                         plug.child(2).asFloat());
         }
      }
      break;
   case AI_TYPE_RGBA:
      {
         // Is the source parameter RGB or RGBA?
         if (!ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
         {
            AiNodeSetRGBA(arnoldNode, arnoldParamName,
                          plug.child(0).asFloat(),
                          plug.child(1).asFloat(),
                          plug.child(2).asFloat(),
                          plug.child(3).asFloat());
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
         if (!ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
         {
            float x, y;
            MObject numObj = plug.asMObject();
            MFnNumericData numData(numObj);
            numData.getData2Float(x, y);
            AiNodeSetPnt2(arnoldNode, arnoldParamName, x, y);
         }
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
         if (!ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
         {
            AiNodeSetVec(arnoldNode, arnoldParamName,
                         plug.child(0).asFloat(),
                         plug.child(1).asFloat(),
                         plug.child(2).asFloat());
         }
      }
      break;
   case AI_TYPE_POINT:
      {
         if (!ProcessParameterComponentInputs(arnoldNode, plug, arnoldParamName, arnoldParamType))
         {
            AiNodeSetPnt(arnoldNode, arnoldParamName,
                         plug.child(0).asFloat(),
                         plug.child(1).asFloat(),
                         plug.child(2).asFloat());
         }
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
   AtArray* array = AiArrayAllocate(size, 1, arnoldParamType);
   MPlug elemPlug;
   MPlugArray connections;
   for (unsigned int i = 0; i < size; ++i)
   {
      // cout << plug.partialName(true, false, false, false, false, true) << " index " << i << endl;
      //plug.selectAncestorLogicalIndex(i, plug.attribute());
      elemPlug = plug[i];

      // connections:
      // An AI_TYPE_NODE param is controlled via a Maya message attribute. Unlike numeric attributes, in Maya
      // there is no way of assigning the value of a message attribute other than via a connection.
      // Therefore, we handle node/message connections in ProcessArrayElement
      if (arnoldParamType != AI_TYPE_NODE)
      {
         MString elemName = MString(arnoldParamName) + "[" + i + "]";
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
                     ProcessConstantArrayElement(arnoldParamType, array, i, elemPlug);
                  }
               }
               break;
            default:
               // constant value
               ProcessConstantArrayElement(arnoldParamType, array, i, elemPlug);
            }
         }
      }
      else
      {
         // constant value
         ProcessConstantArrayElement(arnoldParamType, array, i, elemPlug);
      }
   } // for loop
   if (size) AiNodeSetArray(arnoldNode, arnoldParamName, array);
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
void CDagTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name;
   // TODO: add a global option to control how names are exported
   if (true)
      name = m_dagPath.partialPathName();
   else
      name = m_dagPath.fullPathName();

   if (m_outputAttr.numChars())
      name = name + AI_ATT_SEP + m_outputAttr;
   if (strlen(tag))
      name = name + AI_TAG_SEP + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}

void CDagTranslator::AddHierarchyCallbacks(const MDagPath & path)
{
   AiMsgDebug("[mtoa] [translator %s] Adding callbacks to parents of %s.", GetTranslatorName().asChar(), path.partialPathName().asChar());

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
            if (CArnoldSession::IsRenderablePath(currDag))
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

void CDagTranslator::GetMatrix(AtMatrix& matrix)
{
   MStatus stat;
   MMatrix tm = m_dagPath.inclusiveMatrix(&stat);
   if (MStatus::kSuccess != stat)
   {
      AiMsgError("Failed to get transformation matrix for %s",  m_dagPath.partialPathName().asChar());
   }
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
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

// use standardized render flag names to compute an arnold visibility mask
int CDagTranslator::ComputeVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == CArnoldSession::IsRenderablePath(m_dagPath))
      return AI_RAY_UNDEFINED;

   int visibility = AI_RAY_ALL;
   MPlug plug;

   plug = FindMayaObjectPlug("castsShadows");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = FindMayaObjectPlug("primaryVisibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }

   plug = FindMayaObjectPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFLECTED;
   }

   plug = FindMayaObjectPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFRACTED;
   }

   plug = FindMayaObjectPlug("aiVisibleInDiffuse");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = FindMayaObjectPlug("aiVisibleInGlossy");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }

   return visibility;
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
