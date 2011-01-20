#include "NodeTranslator.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"
#include "nodes/ArnoldNodeFactory.h"

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

#include <string>

//------------ CNodeTranslator ------------//

// internal use only
AtNode* CNodeTranslator::DoExport(AtUInt step)
{
   if (step == 0)
   {
      m_atNode = Export();
      if (m_atNode == NULL)
         AiMsgWarning("[mtoa] node %s did not return a valid arnold node. Motion blur and IPR will be disabled", m_fnNode.name().asChar());
      else
      {
         ExportUserAttribute(m_atNode);
         // If in IPR mode, install callbacks to detect changes.
         if ( m_scene->GetExportMode() == MTOA_EXPORT_IPR )
         {
            AddCallbacks();
         }
      }
   }
   else if (m_atNode != NULL)
   {
      if (RequiresMotionData())
      {
         ExportMotion(m_atNode, step);
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
         // If in IPR mode, install callbacks to detect changes.
         if ( m_scene->GetExportMode() == MTOA_EXPORT_IPR )
         {
            AddCallbacks();
         }
      }
      else if (RequiresMotionData())
         UpdateMotion(m_atNode, step);
   }
   return m_atNode;
}


// Add callbacks to the node passed in. It's a few simple
// callbacks by default. Since this method is virtual - you can
// add whatever callbacks you need to trigger a fresh.
void CNodeTranslator::AddCallbacks()
{
   MStatus status;
   MCallbackId id;
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(m_object,
                                           NodeDirtyCallback,
                                           this,
                                           &status );
   if ( MS::kSuccess == status ) ManageCallback( id );

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(m_object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status );
   if ( MS::kSuccess == status ) ManageCallback( id );

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(m_object,
                                             NameChangedCallback,
                                             this,
                                             &status );
   if ( MS::kSuccess == status ) ManageCallback( id );
}

void CNodeTranslator::ManageCallback( const MCallbackId id )
{
   m_mayaCallbackIDs.append( id );
}

void CNodeTranslator::RemoveCallbacks()
{
   const MStatus status = MNodeMessage::removeCallbacks( m_mayaCallbackIDs );
   if ( status == MS::kSuccess ) m_mayaCallbackIDs.clear();
}


// This is a simple callback triggered when a node is marked as dirty.
void CNodeTranslator::NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData)
{
   AiMsgDebug( "[mtoa] Node changed, updating Arnold. Plug that fired: %s %p", plug.name().asChar(), clientData );
   UpdateIPR( clientData );
}

// This is a simple callback triggered when a node is marked as dirty.
void CNodeTranslator::NameChangedCallback(MObject &node, const MString &str, void *clientData)
{
   AiMsgDebug( "[mtoa] Node name changed, updating Arnold" );
   UpdateIPR( clientData );
}

// Arnold doesn't really support deleting nodes. But we can make things invisible,
// disconnect them, turn them off, etc.
void CNodeTranslator::NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData)
{
   AiMsgDebug( "[mtoa] Node deleted, updating Arnold %p", clientData );
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if ( translator != 0x0 )
   {
      translator->RemoveCallbacks();
      translator->Delete();
   }

   // Update Arnold without passing a translator, this just forces a redraw.
   CMayaScene::UpdateIPR();
}


void CNodeTranslator::UpdateIPR( void * clientData )
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if ( translator != 0x0 )
   {
      translator->RemoveCallbacks();
      CMayaScene::UpdateIPR( translator );
   }
}


void CNodeTranslator::ExportUserAttribute(AtNode *anode)
{
   MFnDependencyNode fnDepNode(m_object);
   for (unsigned int i=0; i<fnDepNode.attributeCount(); ++i)
   {
      MObject oAttr = fnDepNode.attribute(i);

      MFnAttribute fnAttr(oAttr);
      MPlug pAttr(m_object, oAttr);

      MString name = fnAttr.name();
      if (name.indexW("mtoa_") == 0)
      {
         const char *aname = name.asChar() + 5;
         if(AiNodeLookUpUserParameter(anode, aname) != NULL)
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
               AiMsgError("[mtoa] unsupported user attribute type %s", pAttr.partialName(true, false, false, false, false, true).asChar() );
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
               AiMsgError("[mtoa] unsupported user attribute type %s", pAttr.partialName(true, false, false, false, false, true).asChar() );
               break;
            }
         }
         else
            AiMsgError("[mtoa] unsupported user attribute type %s", pAttr.partialName(true, false, false, false, false, true).asChar() );
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

void CNodeTranslator::ExportDynamicFloatParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetFlt(arnoldNode, paramName, plug.asFloat());
}

void CNodeTranslator::ExportDynamicBooleanParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetBool(arnoldNode, paramName, plug.asBool());
}

void CNodeTranslator::ExportDynamicIntParameter(AtNode* arnoldNode, const char* paramName)
{
   MPlug plug = m_fnNode.findPlug(paramName);
   if (!plug.isNull())
      AiNodeSetInt(arnoldNode, paramName, plug.asInt());
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element)
{

   MPlug plug = m_fnNode.findPlug(mayaAttrib);
   return ProcessParameter(arnoldNode, plug, AiParamGetName(paramEntry), AiParamGetType(paramEntry), element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element)
{
   MPlug plug = m_fnNode.findPlug(attrib);
   return ProcessParameter(arnoldNode, plug, attrib, arnoldAttribType, element);
}

AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   MPlug plug = m_fnNode.findPlug(mayaAttrib);
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
   AtNode* linkedNode = NULL;

   if (element >= 0)
      plug = plug.elementByPhysicalIndex(element);

   MPlugArray connections;
   bool isShader = (AiNodeEntryGetType(arnoldNode->base_node) & AI_NODE_SHADER) ? true : false;
   // links only supported on shaders
   if (isShader)
      plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
   {
      if (isShader)
         // Unlink first, since this may be called during an IPR update
         AiNodeUnlink(arnoldNode, arnoldAttrib);

      switch(arnoldAttribType)
      {
      case AI_TYPE_RGB:
         {
            bool compConnected = false;
            for (unsigned int i=0; i < 3; i++)
            {
               plug.child(i).connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  compConnected = true;
                  MString attrName = connections[0].partialName(false, false, false, false, false, true);
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".r";
                     break;
                  case 1:
                     compAttrName += ".g";
                     break;
                  case 2:
                     compAttrName += ".b";
                     break;
                  }
                  AtNode* node = m_scene->ExportShader(connections[0].node(), attrName);
                  if (node != NULL)
                     AiNodeLink(node, compAttrName.asChar(), arnoldNode);
               }
            }
            if (!compConnected)
               AiNodeSetRGB(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
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
               bool compConnected = false;
               for (unsigned int i=0; i < 3; i++)
               {
                  plug.child(i).connectedTo(connections, true, false);
                  if (connections.length() > 0)
                  {
                     compConnected = true;
                     MString attrName = connections[0].partialName(false, false, false, false, false, true);
                     MString compAttrName(arnoldAttrib);
                     switch(i)
                     {
                     case 0:
                        compAttrName += ".r";
                        break;
                     case 1:
                        compAttrName += ".g";
                        break;
                     case 2:
                        compAttrName += ".b";
                        break;
                     }
                     AtNode* node = m_scene->ExportShader(connections[0].node(), attrName);
                     if (node != NULL)
                        AiNodeLink(node, compAttrName.asChar(), arnoldNode);
                  }
               }
               if (!compConnected)
                  // For RGB source parameter, set alpha value to 1
                  AiNodeSetRGBA(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), 1);
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
            float x, y;
            MObject numObj = plug.asMObject();
            MFnNumericData numData(numObj);
            numData.getData2Float(x, y);
            AiNodeSetPnt2(arnoldNode, arnoldAttrib, x, y);
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
            bool compConnected = false;
            for (unsigned int i=0; i < 3; i++)
            {
               plug.child(i).connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  compConnected = true;
                  MString attrName = connections[0].partialName(false, false, false, false, false, true);
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".x";
                     break;
                  case 1:
                     compAttrName += ".y";
                     break;
                  case 2:
                     compAttrName += ".z";
                     break;
                  }
                  AtNode* node = m_scene->ExportShader(connections[0].node(), attrName);
                  if (node != NULL)
                     AiNodeLink(node, compAttrName.asChar(), arnoldNode);
               }
            }
            if (!compConnected)
               AiNodeSetVec(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_POINT:
         {
            AiNodeSetPnt(arnoldNode, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      }
   }
   else
   {
      // process connections
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      linkedNode = m_scene->ExportShader(connections[0].node(), attrName);

      if (linkedNode != NULL)
         AiNodeLink(linkedNode, arnoldAttrib, arnoldNode);
   }
   return linkedNode;
}

// CDagTranslator
//

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


//------------ CDagTranslator ------------//

void CDagTranslator::AddHierarchyCallbacks(const MDagPath & path)
{
   AiMsgDebug( "[mtoa] Adding callbacks to parents of: %s", path.partialPathName().asChar() );

   // Loop through the whole dag path adding callbacks to them.
   MStatus status;
   MDagPath dag_path( path );
   dag_path.pop(); // Pop of the shape as that's handled by CNodeTranslator::AddCallbacks.
   for( ; dag_path.length() > 0; dag_path.pop() )
   {
      MObject node = dag_path.node();
      if ( node != MObject::kNullObj )
      {
         // We can use the normal NodeDirtyCallback here.
         MCallbackId id = MNodeMessage::addNodeDirtyCallback(node,
                                                             NodeDirtyCallback,
                                                             this,
                                                             &status );
         if ( MS::kSuccess == status ) ManageCallback( id );
      }
   }
}


void CDagTranslator::AddCallbacks()
{
   AddHierarchyCallbacks( m_dagPath );

   // Call the base class to get the others.
   CNodeTranslator::AddCallbacks();
}

void CDagTranslator::Delete()
{
   // Arnold doesn't allow us to delete nodes
   // setting the visibility is as good as it gets
   // for now.
   AiNodeSetInt(m_atNode, "visibility",  AI_RAY_UNDEFINED);
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
            if (CMayaScene::IsVisibleDag(currDag))
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
         AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
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
AtInt CDagTranslator::ComputeVisibility(bool mayaStyleAttrs)
{
   AtInt visibility = AI_RAY_ALL;
   MPlug plug;
   if (mayaStyleAttrs)
   {
      plug = m_fnNode.findPlug("castsShadows");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_SHADOW;
      }

      plug = m_fnNode.findPlug("primaryVisibility");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_CAMERA;
      }

      plug = m_fnNode.findPlug("visibleInReflections");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFLECTED;
      }

      plug = m_fnNode.findPlug("visibleInRefractions");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFRACTED;
      }
   }
   else
   {
      plug = m_fnNode.findPlug("casts_shadows");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_SHADOW;
      }

      plug = m_fnNode.findPlug("primary_visibility");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_CAMERA;
      }

      plug = m_fnNode.findPlug("visible_in_reflections");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFLECTED;
      }

      plug = m_fnNode.findPlug("visible_in_refractions");
      if (!plug.isNull() && !plug.asBool())
      {
         visibility &= ~AI_RAY_REFRACTED;
      }
   }

   plug = m_fnNode.findPlug("diffuse_visibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = m_fnNode.findPlug("glossy_visibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }

   return visibility;
}

// create visibility attributes with standardized render flag names
void CDagTranslator::AddVisibilityAttrs(MObject& node)
{
   MFnNumericAttribute nAttr;
   MFnDependencyNode fnNode = MFnDependencyNode(node);
   MObject attr;

   attr = nAttr.create("primaryVisibility", "vis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("receiveShadows", "rsh", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("castsShadows", "csh", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("visibleInReflections", "vir", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("visibleInRefractions", "vif", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("selfShadows", "ssh", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("diffuseVisibility", "dvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);

   attr = nAttr.create("glossyVisibility", "gvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   fnNode.addAttribute(attr);
}

AtNode* CAutoTranslator::Export()
{
   MString mayaShader = m_fnNode.typeName();
   std::string arnoldNode = CArnoldNodeFactory::s_factoryNodes[mayaShader.asChar()].arnoldNodeName;
   AtNode* shader = NULL;
   m_nodeEntry = AiNodeEntryLookUp(arnoldNode.c_str());

   // Make sure that the given type of node exists
   if (m_nodeEntry != NULL)
   {
      shader = AiNode(arnoldNode.c_str());

      AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
      Update(shader);
   }
   return shader;
}

void CAutoTranslator::Update(AtNode *shader)
{
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(m_nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (!strncmp(paramName, "aov_", 4))
      {
         CRenderSession *renderSession = CRenderSession::GetInstance();
         const CRenderOptions *renderOptions = renderSession->RenderOptions();

         // do not check type for now
         std::string aovName(paramName);
         aovName = aovName.substr(4);
         if (renderOptions->FindAOV(aovName.c_str()) != size_t(-1))
         {
            AiNodeSetStr(shader, paramName, aovName.c_str());
         }
         else
         {
            AiNodeSetStr(shader, paramName, "");
         }
      }
      else if (strcmp(paramName, "name"))
      {
         AtInt paramType = AiParamGetType(paramEntry);

         // attr name name remap
         const char* attrName;
         if (!AiMetaDataGetStr(m_nodeEntry, paramName, "maya.name", &attrName))
            attrName = paramName;

         plug = m_fnNode.findPlug(attrName);
         ProcessParameter(shader, plug, paramName, paramType);
      }
   }

   MPlugArray connections;

   plug = m_fnNode.findPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* bump = m_scene->ExportShader(connections[0].node(), attrName);

      if (bump != NULL)
         AiNodeLink(bump, "@before", shader);
   }
}
