#include "NodeTranslator.h"
#include "render/RenderOptions.h"

#include <ai_ray.h>
#include <ai_metadata.h>

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

#define COMP_CONNECTIONS(plug, arnoldNode, arnoldAttrib, comp1, comp2, comp3) \
   int compConnected = 0;\
   MPlugArray conn;\
   for (unsigned int i=0; i < 3; i++){\
      plug.child(i).connectedTo(conn, true, false);\
      if (conn.length() > 0){\
         MString attrName = conn[0].partialName(false, false, false, false, false, true);\
         AtNode* node = ExportShader(conn[0].node(), attrName);\
         if (node != NULL){\
            ++compConnected;\
            MString compAttrName(arnoldAttrib);\
            switch(i)\
            {\
            case 0:\
               compAttrName += comp1;\
               break;\
            case 1:\
               compAttrName += comp2;\
               break;\
            case 2:\
               compAttrName += comp3;\
               break;\
            }\
            AiNodeLink(node, compAttrName.asChar(), arnoldNode);\
         }\
      }\
   }

#define COMP_CONNECTIONS_RGB(plug, arnoldNode, arnoldAttrib) \
      COMP_CONNECTIONS(plug, arnoldNode, arnoldAttrib, ".r", ".g", ".b")\
      if (compConnected != 3)\
         AiNodeSetRGB(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());

#define COMP_CONNECTIONS_RGBA(plug, arnoldNode, arnoldAttrib) \
      COMP_CONNECTIONS(plug, arnoldNode, arnoldAttrib, ".r", ".g", ".b")\
      if (compConnected != 3)\
         AiNodeSetRGBA(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), 1.0f);

#define COMP_CONNECTIONS_VEC(plug, arnoldNode, arnoldAttrib) \
      COMP_CONNECTIONS(plug, arnoldNode, arnoldAttrib, ".x", ".y", ".z")\
      if (compConnected != 3)\
         AiNodeSetVec(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());

#define COMP_CONNECTIONS_PNT(plug, arnoldNode, arnoldAttrib) \
      COMP_CONNECTIONS(plug, arnoldNode, arnoldAttrib, ".x", ".y", ".z")\
      if (compConnected != 3)\
         AiNodeSetPnt(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());

//------------ CNodeTranslator ------------//

// internal use only
AtNode* CNodeTranslator::DoExport(AtUInt step)
{
   if (m_atNode != NULL)
   {
      if (step == 0)
      {
         if (m_outputAttr != "")
            AiMsgDebug("[mtoa] [translator %s] Exporting on plug %s.%s.",
                  GetName().asChar(), GetFnNode().name().asChar(), m_outputAttr.asChar());
         else
            AiMsgDebug("[mtoa] [translator %s] Exporting on node %s.",
                  GetName().asChar(), GetFnNode().name().asChar());
         Export(m_atNode);
         ExportUserAttribute(m_atNode);
      }
      else if (RequiresMotionData())
      {
         if (m_outputAttr != "")
            AiMsgDebug("[mtoa] [translator %s] Exporting motion on plug %s.%s.",
                  GetName().asChar(), GetFnNode().name().asChar(), m_outputAttr.asChar());
         else
            AiMsgDebug("[mtoa] [translator %s] Exporting motion on node %s.",
                  GetName().asChar(), GetFnNode().name().asChar());

         ExportMotion(m_atNode, step);
      }

      // Add IPR callbacks on last step
      if (step == (GetNumMotionSteps()-1) &&
          GetExportMode() == MTOA_EXPORT_IPR)
      {
         AddIPRCallbacks();
      }
   }
   return m_atNode;
}

// internal use only
AtNode* CNodeTranslator::DoUpdate(AtUInt step)
{
   if (m_atNode != NULL)
   {
      if (step == 0)
      {
         Update(m_atNode);
         ExportUserAttribute(m_atNode);
      }
      else if (RequiresMotionData())
         UpdateMotion(m_atNode, step);

      // Add IPR callbacks on last step
      if (step == (GetNumMotionSteps()-1) &&
            GetExportMode() == MTOA_EXPORT_IPR)
      {
         AddIPRCallbacks();
      }
   }
   return m_atNode;
}

AtNode* CNodeTranslator::DoCreateArnoldNodes()
{
   m_atNode = CreateArnoldNodes();
   if (m_atNode == NULL)
      AiMsgWarning("[mtoa] [translator %s] Translation on %s returned an empty Arnold root node.",
            GetName().asChar(), GetFnNode().name().asChar());

   return m_atNode;
}

AtNode* CNodeTranslator::GetArnoldRootNode()
{
   return m_atNode;
}

AtNode* CNodeTranslator::GetArnoldNode(const char* tag)
{
   if (m_atNodes.count(tag))
      return m_atNodes[tag];
   AiMsgError("[mtoa] [translator %s] Translation has not created an Arnold node with tag \"%s\".", GetName().asChar(), tag);
   return NULL;
}

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
      AiMsgError("[mtoa] [translator %s] Arnold node type %s does not exist.", GetName().asChar(), type);
      return NULL;
   }
}

void CNodeTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = GetFnNode().name();
   if (m_outputAttr.numChars())
      name = name + "_" + m_outputAttr;
   if (strlen(tag))
      name = name +  "_" + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}

// Add callbacks to the node passed in. It's a few simple
// callbacks by default. Since this method is virtual - you can
// add whatever callbacks you need to trigger a fresh.
void CNodeTranslator::AddIPRCallbacks()
{
   MStatus status;
   MCallbackId id;
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(m_object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) ManageIPRCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(m_object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) ManageIPRCallback(id);

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(m_object,
                                             NameChangedCallback,
                                             this,
                                             &status);
   if (MS::kSuccess == status) ManageIPRCallback(id);
}

void CNodeTranslator::ManageIPRCallback(const MCallbackId id)
{
   m_mayaCallbackIDs.append(id);
}

void CNodeTranslator::RemoveIPRCallbacks()
{
   const MStatus status = MNodeMessage::removeCallbacks(m_mayaCallbackIDs);
   if (status == MS::kSuccess) m_mayaCallbackIDs.clear();
}


// This is a simple callback triggered when a node is marked as dirty.
void CNodeTranslator::NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData)
{
   AiMsgDebug("[mtoa] Translator callback for node dirty, plug that fired: %s, client data: %p.",
         plug.name().asChar(), clientData);
   UpdateIPR(clientData);
}

void CNodeTranslator::NameChangedCallback(MObject &node, const MString &str, void *clientData)
// This is a simple callback triggered when the name changes.
{
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] Node name changed, updating Arnold, client data: %p.",
            translator->GetName().asChar(), clientData);
      translator->SetArnoldNodeName(translator->GetArnoldRootNode());
   }
   else
   {
      AiMsgWarning("[mtoa] Translator callback for node name changed, no translator in client data: %p.", clientData);
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
            translator->GetName().asChar(), clientData);
      translator->RemoveIPRCallbacks();
      translator->Delete();
   }
   else
   {
      AiMsgWarning("[mtoa] Translator callback for node deleted, no translator in client data: %p.", clientData);
   }

   // Update Arnold without passing a translator, this just forces a redraw.
   CMayaScene::UpdateIPR();
}


void CNodeTranslator::UpdateIPR(void * clientData)
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa] [translator %s] Node dirty, updating Arnold, client data: %p.",
            translator->GetName().asChar(), clientData);
      translator->RemoveIPRCallbacks();
      CMayaScene::UpdateIPR(translator);
   }
   else
   {
      AiMsgDebug("[mtoa] UpdateIPR called, no translator in client data: %p.", clientData);
   }
}

void CNodeTranslator::ExportUserAttribute(AtNode *anode)
{
   MFnDependencyNode fnDepNode = GetFnNode();
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
                        AiArraySetBool(ary, i, pAttr[i].asChar());
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
                        rgba.r = static_cast<AtFloat>(r);
                        rgba.g = static_cast<AtFloat>(g);
                        rgba.b = static_cast<AtFloat>(b);
                        rgba.a = static_cast<AtFloat>(a);
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
                     AiNodeSetRGBA(anode, aname, static_cast<AtFloat>(r), static_cast<AtFloat>(g), static_cast<AtFloat>(b), static_cast<AtFloat>(a));
                  }
               }
               break;
            default:
               // not supported: k2Short, k2Long, k3Short, k3Long, kAddr
               AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
                     GetName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
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
                     AiArraySetFlt(ary, i, static_cast<AtFloat>(data[i]));
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
                     pnt.x = static_cast<AtFloat>(data[i].x);
                     pnt.y = static_cast<AtFloat>(data[i].y);
                     pnt.z = static_cast<AtFloat>(data[i].z);
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
                     vec.x = static_cast<AtFloat>(data[i].x);
                     vec.y = static_cast<AtFloat>(data[i].y);
                     vec.z = static_cast<AtFloat>(data[i].z);
                     AiArraySetVec(ary, i, vec);
                  }
                  AiNodeSetArray(anode, aname, ary);
               }
               break;
            default:
               // kMatrix, kNumeric (this one should have be caught be hasFn(MFn::kNumericAttribute))
               AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
                  GetName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
               break;
            }
         }
         else
            AiMsgError("[mtoa] [translator %s] Unsupported user attribute type for %s",
                  GetName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
      }
   }
}

// populate an arnold matrix with values from a maya matrix
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

MPlug CNodeTranslator::GetPlugElement(MFnDependencyNode& node, MPlug& plug, const std::string &attr)
{
   if (attr.length() == 0)
   {
      AiMsgError("[mtoa] Invalid plug name: \"%s\"", attr.c_str());
      return MPlug();
   }

   int idx = -1;
   std::string an = attr;
   size_t p1 = an.length() - 1;

   if (an[p1] == ']')
   {
      size_t p0 = an.rfind('[');

      if (p0 == std::string::npos)
      {
         AiMsgError("[mtoa] Invalid plug name: \"%s\"", attr.c_str());
         return MPlug();
      }

      std::string ai = an.substr(p0+1, p1-p0-1);

      if (sscanf(ai.c_str(), "%d", &idx) != 1)
      {
         AiMsgError("[mtoa] Invalid plug name: \"%s\"", attr.c_str());
         return MPlug();
      }

      an = an.substr(0, p0);
   }

   MPlug eplug;

   if (plug.isNull())
   {
      eplug = node.findPlug(an.c_str());
   }
   else
   {
      MObject childAttr = node.attribute(an.c_str());
      eplug = plug.child(childAttr);
   }

   if (idx >= 0)
   {
      eplug = eplug.elementByLogicalIndex(idx);
   }

   return eplug;
}

// This allows for: FindPlug(shader, "input3D[10].input3Dx")
//                  which MFnDependencyNode::findPlug() does not support
MPlug CNodeTranslator::FindPlug(MFnDependencyNode& node, const std::string& param)
{
   size_t p0, p1;
   MPlug cplug;

   p0 = 0;
   p1 = param.find('.', p0);

   while (p1 != std::string::npos)
   {
      cplug = GetPlugElement(node, cplug, param.substr(p0, p1-p0));

      if (cplug.isNull())
      {
         return cplug;
      }

      p0 = p1 + 1;
      p1 = param.find('.', p0);
   }
   
   return GetPlugElement(node, cplug, param.substr(p0));
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element)
{
   MStatus status;
   MPlug plug = FindPlug(m_fnNode, mayaAttrib);
   if (plug.isNull())
   {
      AiMsgWarning("[mtoa] [translator %s] Maya node %s does not have requested attribute %s.", GetName().asChar(),
            m_fnNode.name().asChar(), mayaAttrib);
      return NULL;
   }
   return ProcessParameter(arnoldNode, plug, AiParamGetName(paramEntry), AiParamGetType(paramEntry), element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element)
{
   MStatus status;
   MPlug plug = FindPlug(m_fnNode, attrib);
   if (plug.isNull())
   {
      AiMsgWarning("[mtoa] [translator %s] Maya node %s does not have requested attribute %s.",
            GetName().asChar(), m_fnNode.name().asChar(), attrib);
      return NULL;
   }
   return ProcessParameter(arnoldNode, plug, attrib, arnoldAttribType, element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   MStatus status;
   MPlug plug = FindPlug(m_fnNode, mayaAttrib);
   if (plug.isNull())
   {
      AiMsgWarning("[mtoa] [translator %s] Maya node %s does not have requested attribute %s.",
            GetName().asChar(), m_fnNode.name().asChar(), mayaAttrib);
      return NULL;
   }
   return ProcessParameter(arnoldNode, plug, arnoldAttrib, arnoldAttribType, element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, MPlug& plug, const AtParamEntry* paramEntry, int element)
{
   return ProcessParameter(arnoldNode, plug, AiParamGetName(paramEntry), AiParamGetType(paramEntry), element);
}

// export values from a maya plug to an arnold parameter, recursively following
// connections in the dependency graph.
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, MPlug& plug, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   if (arnoldNode == NULL)
   {
      AiMsgError("[mtoa] [translator %s] Cannot process %s parameter on null node.", GetName().asChar(), arnoldAttrib);
      return NULL;
   }

   if (element >= 0)
      plug = plug.elementByPhysicalIndex(element);

   MPlugArray connections;
   bool isShader = (AiNodeEntryGetType(arnoldNode->base_node) & (AI_NODE_OPTIONS)) ? false : true;
   // links only supported on shaders
   if (isShader)
      if (plug.isIgnoredWhenRendering())
      {
         return NULL;
      }
      plug.connectedTo(connections, true, false);

   if (connections.length() > 0)
   {
      // process connections
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* linkedNode = ExportShader(connections[0].node(), attrName);

      if (linkedNode != NULL)
      {
         AiNodeLink(linkedNode, arnoldAttrib, arnoldNode);
         return linkedNode;
      }
   }

   if (isShader)
      // Unlink first, since this may be called during an IPR update
      AiNodeUnlink(arnoldNode, arnoldAttrib);

   switch(arnoldAttribType)
   {
   case AI_TYPE_RGB:
      {
         COMP_CONNECTIONS_RGB(plug, arnoldNode, arnoldAttrib);
      }
      break;
   case AI_TYPE_RGBA:
      {
         // Is the source parameter RGB or RGBA?
         if (plug.numChildren() == 4)
         {
            AiNodeSetRGBA(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), plug.child(3).asFloat());
         }
         else
         {
            // FIXME: handle alphas!
            COMP_CONNECTIONS_RGBA(plug, arnoldNode, arnoldAttrib);
         }
      }
      break;
   case AI_TYPE_FLOAT:
      {
         AiNodeSetFlt(arnoldNode, arnoldAttrib, plug.asFloat());
      }
      break;
   case AI_TYPE_POINT2:
      {
         int compConnected = 0;
         MPlugArray conn;
         for (unsigned int i=0; i < 2; i++)
         {
            plug.child(i).connectedTo(conn, true, false);
            if (conn.length() > 0)
            {
               MString attrName = conn[0].partialName(false, false, false, false, false, true);
               AtNode* node = ExportShader(conn[0].node(), attrName);
               if (node != NULL)
               {
                  ++compConnected;
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".x";
                     break;
                  case 1:
                     compAttrName += ".y";
                     break;
                  }
                  AiNodeLink(node, compAttrName.asChar(), arnoldNode);
               }
            }
         }

         if (compConnected != 2)
         {
            float x, y;
            MObject numObj = plug.asMObject();
            MFnNumericData numData(numObj);
            numData.getData2Float(x, y);
            AiNodeSetPnt2(arnoldNode, arnoldAttrib, x, y);
         }
      }
      break;
   case AI_TYPE_MATRIX:
      {
         AtMatrix am;
         MObject matObj = plug.asMObject();
         MFnMatrixData matData(matObj);
         MMatrix mm = matData.matrix();
         ConvertMatrix(am, mm);
         AiNodeSetMatrix(arnoldNode, arnoldAttrib, am);
      }
      break;
   case AI_TYPE_BOOLEAN:
      {
         AiNodeSetBool(arnoldNode, arnoldAttrib, plug.asBool());
      }
      break;
   case AI_TYPE_ENUM:
      {
         AiNodeSetInt(arnoldNode, arnoldAttrib, plug.asInt());
      }
      break;
   case AI_TYPE_INT:
      {
         AiNodeSetInt(arnoldNode, arnoldAttrib, plug.asInt());
      }
      break;
   case AI_TYPE_STRING:
      {
         AiNodeSetStr(arnoldNode, arnoldAttrib, plug.asString().asChar());
      }
      break;
   case AI_TYPE_VECTOR:
      {
         COMP_CONNECTIONS_VEC(plug, arnoldNode, arnoldAttrib);
      }
      break;
   case AI_TYPE_POINT:
      {
         COMP_CONNECTIONS_PNT(plug, arnoldNode, arnoldAttrib);
      }
      break;
   }
   return NULL;
}

//------------ CDagTranslator ------------//

// populate an arnold AtMatrix with values from this Dag node's transformation.
// the dag node must have an inclusiveMatrix attribute.

ObjectHandleToDagMap CDagTranslator::s_masterInstances;

// Returns the instance number of the master instance (it's not always 0!)
// Returns -1 if no master instance has been encountered yet
int CDagTranslator::GetMasterInstanceNumber(MObject node)
{
   MObjectHandle handle = MObjectHandle(node);
   // if handle is not in the map, a new entry will be made with a default value
   MDagPath dagPath = s_masterInstances[handle];
   if (dagPath.isValid())
   {
      return dagPath.instanceNumber();
   }
   return -1;
}

void CDagTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name;
   // TODO: add a global option to control how names are exported
   if (true)
      name = m_dagPath.partialPathName();
   else
      name = m_dagPath.fullPathName();

   if (m_outputAttr.numChars())
      name = name + "_" + m_outputAttr;
   if (strlen(tag))
      name = name +  "_" + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}

void CDagTranslator::AddHierarchyCallbacks(const MDagPath & path)
{
   AiMsgDebug("[mtoa] [translator %s] Adding callbacks to parents of %s.", GetName().asChar(), path.partialPathName().asChar());

   // Loop through the whole dag path adding callbacks to them.
   MStatus status;
   MDagPath dag_path(path);
   dag_path.pop(); // Pop of the shape as that's handled by CNodeTranslator::AddIPRCallbacks.
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
         if (MS::kSuccess == status) ManageIPRCallback(id);
      }
   }
}


void CDagTranslator::AddIPRCallbacks()
{
   AddHierarchyCallbacks(m_dagPath);

   // Call the base class to get the others.
   CNodeTranslator::AddIPRCallbacks();
}

void CDagTranslator::Delete()
{
   // Arnold doesn't allow us to delete nodes
   // setting the visibility is as good as it gets
   // for now.
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
      AtUInt instNum = m_dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed m_dagPath is assumed to be visible)
         s_masterInstances[handle] = m_dagPath;
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = s_masterInstances[handle];
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(m_dagPath.node(), allInstances);
         AtUInt master_index = 0;
         for (; (master_index < m_dagPath.instanceNumber()); master_index++)
         {
            currDag = allInstances[master_index];
            if (CMayaScene::IsRenderablePath(currDag))
            {
               // found it
               s_masterInstances[handle] = currDag;
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: m_dagPath is the master
         s_masterInstances[handle] = m_dagPath;
         return true;
      }
   }
   // not instanced: m_dagPath is the master
   return true;
}

void CDagTranslator::GetRotationMatrix(AtMatrix& matrix)
{
   MObject transform = m_dagPath.transform();
   MFnTransform mTransform = MFnTransform(transform);
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
   MMatrix tm = m_dagPath.inclusiveMatrix();

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
void CDagTranslator::ExportMatrix(AtNode* node, AtUInt step)
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
AtInt CDagTranslator::ComputeVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == CMayaScene::IsRenderablePath(m_dagPath))
      return AI_RAY_UNDEFINED;

   AtInt visibility = AI_RAY_ALL;
   MPlug plug;

   plug = GetFnNode().findPlug("castsShadows");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = GetFnNode().findPlug("primaryVisibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }

   plug = GetFnNode().findPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFLECTED;
   }

   plug = GetFnNode().findPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFRACTED;
   }

   plug = GetFnNode().findPlug("aiVisibleInDiffuse");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = GetFnNode().findPlug("aiVisibleInGlossy");
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
