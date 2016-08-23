#include "platform/Platform.h"
#include "NodeTranslator.h"
#include "NodeTranslatorImpl.h"

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
CNodeTranslator::CNodeTranslator()
{
   m_impl = new CNodeTranslatorImpl(*this);
}
CNodeTranslator::~CNodeTranslator()
{
   delete m_impl;
}

AtNode* CNodeTranslator::ExportNode(const MPlug& outputPlug, bool track, CNodeTranslator** outTranslator)
{
   CNodeTranslator* translator = NULL;
   if (track)
      translator = GetSession()->ExportNode(outputPlug, m_impl->m_shaders, &m_impl->m_upstreamAOVs);
   else
      translator = GetSession()->ExportNode(outputPlug);
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
   translator = GetSession()->ExportDagPath(dagPath);
   if (translator != NULL)
      return translator->GetArnoldRootNode();
   return NULL;
}




/// Get actual plug to be used for that attribute name, either the one on the translated maya object,
/// or the one on the override set to be used, if any.
MPlug CNodeTranslator::FindMayaPlug(const MString &attrName, MStatus* ReturnStatus) const
{
   MStatus status(MStatus::kSuccess);

   MPlug plug = m_impl->FindMayaObjectPlug(attrName, &status);
   if ((MStatus::kSuccess == status) && !plug.isNull())
   {
      MStatus overstat;
      MString attrLongName = plug.partialName(false, true, true, false, true, true, &overstat);
      MPlug overridePlug = m_impl->FindMayaOverridePlug(attrLongName, &overstat);
      if ((MStatus::kSuccess == overstat) && !overridePlug.isNull())
      {
         plug = overridePlug;
         status = overstat;
      }
   }

   if (ReturnStatus != NULL) *ReturnStatus = status;
   return plug;
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
         if (GetSession()->IsActiveAOV(aov))
         {
            m_impl->m_localAOVs.insert(aov);
            AiMsgDebug("[mtoa.translator.aov] %-30s | \"%s\" is active on attr %s",
                       GetMayaNodeName().asChar(), value.asChar(), aovAttrs[i].asChar());
         }
      }
   }
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

// so by default we're not deleting the current node ?
bool CNodeTranslator::IsExported() const
{
   return m_impl->m_isExported;
}
// so by default we're not deleting the current node ?
void CNodeTranslator::Delete()
{
   m_impl->m_isExported = false;
}

AtNode* CNodeTranslator::GetArnoldRootNode()
{
   return m_impl->m_atNode;
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
   if (m_impl->m_atNodes.count(tag))
   {
      return m_impl->m_atNodes[tag];
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
      if (m_impl->m_atNodes.count(tag))
      {
         AiMsgWarning("[mtoa] Translator has already added Arnold node with tag \"%s\"", tag);
         return node;
      }
      else
         m_impl->m_atNodes[tag] = node;
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
      MString outputAttr = GetMayaOutputAttributeName();
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
   AtNode *node = GetArnoldNode(tag);
   if (node == NULL) return "";
   return AiNodeGetName(node);
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

void CNodeTranslator::NodeChanged(MObject& node, MPlug& plug)
{  
   // When the frame is changed for motion blur we can receive signals here,
   // but we want to ignore them
   // FIXME should we test this in RequestUpdate ?
   if (GetSession()->IsExportingMotion() && GetSession()->IsInteractiveRender()) return;

   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeChanged: translator %s, providing Arnold %s(%s): %p",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              GetArnoldNodeName(), GetArnoldTypeName(), GetArnoldNode());

   // name of the attribute that emitted a signal
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);

   // The Arnold translator has changed :
   // This means the current one won't be able to export as it should.
   // By setting its update mode to AI_RECREATE_TRANSLATOR this translator 
   // will be cleared and a new one will be generated
   if (plugName == ".aiTranslator") SetUpdateMode(AI_RECREATE_TRANSLATOR);

   RequestUpdate();
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
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(object,
                                             NameChangedCallback,
                                             this,
                                             &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   id = MNodeMessage::addNodeDestroyedCallback(object,
                                               NodeDestroyedCallback,
                                               this,
                                               &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);
}

void CNodeTranslator::RegisterUpdateCallback(const MCallbackId id)
{
   m_impl->m_mayaCallbackIDs.append(id);
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
      if (translator->GetSession()->IsExportingMotion() && translator->GetSession()->IsInteractiveRender()) return;
      translator->NodeChanged(node, plug);
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
         translator->SetUpdateMode(CNodeTranslator::AI_DELETE_NODE);

      // for nodes which don't have the updateMode set to AI_DELETE_NODE, what's the point of requesting an update ?
      // is this just going to re-export one last time them before they're deleted ?
      translator->RequestUpdate();  
   }
   else
   {
      MFnDependencyNode dnode(node);
      AiMsgWarning("[mtoa.translator.ipr] %-30s | Translator callback for node deleted, no translator in client data: %p.",
                   dnode.name().asChar(), clientData);
   }
}

void CNodeTranslator::NodeDestroyedCallback(void* clientData)
{
   CNodeTranslator* translator = static_cast<CNodeTranslator*>(clientData);
   if (translator != NULL)
   {
      //translator->RequestUpdate();
      // replacing the line above by the 2 following ones as result is the same
      CArnoldSession *session = CMayaScene::GetArnoldSession();
      session->RequestUpdate();      
      // note that LightLinker and ObjectSets are never called here
      // since they don't add this callback   
      
      translator->m_impl->RemoveUpdateCallbacks();
      translator->Delete();  
   }
}

/// add this node's AOVs into the passed AOVSet
void CNodeTranslator::RequestUpdate()
{

   // if hold updates is enabled (on RenderView only), don't ask for updates
   if (m_impl->m_holdUpdates) return;

   // we're changing the frame to evaluate motion blur, so we don't want more 
   // updates now
   if (GetSession()->IsInteractiveRender() && GetSession()->IsExportingMotion()) return;

   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate: Arnold node %s(%s): %p.",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              GetArnoldNodeName(), GetArnoldTypeName(), GetArnoldNode());

   
   if (GetSession()->GetSessionMode() == MTOA_SESSION_RENDERVIEW)
   {
      if (!m_impl->m_holdUpdates)
      {
         m_impl->m_holdUpdates = true;
         // Add translator to the list of translators to update
         GetSession()->QueueForUpdate(this);
      }
   } else
   {
      m_impl->RemoveUpdateCallbacks();
      // Add translator to the list of translators to update
      GetSession()->QueueForUpdate(this);
   }

   // Pass the update request to the export session
   GetSession()->RequestUpdate();
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
         pAttr = translator->m_impl->GetOverridePlug(pAttr);

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
  


/// Using the translator's m_handle Maya Object and specific attrName
AtNode* CNodeTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType, MString mayaAttrName)
{
   MStatus status = MStatus::kFailure;
   MPlug plug;

   if (mayaAttrName.length() > 0)
   {
      // specific maya attribute name

      // Get plug on the Maya object or override set if there is one
      plug = FindMayaPlug(mayaAttrName, &status);
   } else
   {
      // no maya attribute name, search it by default

      // attr name name remap
      const AtNodeEntry* arnoldNodeEntry = AiNodeGetNodeEntry(arnoldNode);
      CBaseAttrHelper helper(arnoldNodeEntry);
      MFnDependencyNode fnNode(GetMayaObject());
      if (helper.IsHidden(arnoldParamName))
      {
         AiMsgDebug("[mtoa.translator]  %s: Parameter %s is hidden on Arnold node %s(%s).",
            GetTranslatorName().asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(arnoldNodeEntry));
         return NULL;
      }

      // check paramName
      mayaAttrName = helper.GetMayaAttrName(arnoldParamName);
      MPlug plug = FindMayaPlug(mayaAttrName, &status);
      if ((MStatus::kSuccess != status) || plug.isNull())
      {
         // check aiParamName
         helper.SetPrefix("ai_");
         MString mayaAttrAiName = helper.GetMayaAttrName(arnoldParamName);
         plug = FindMayaPlug(mayaAttrAiName, &status);
      }
   }

   if ((MStatus::kSuccess != status) || plug.isNull())
   {
      AiMsgWarning("[mtoa.translator]  %s: Maya node %s(%s) does not have attribute %s to match parameter %s on Arnold node %s(%s).",
            GetTranslatorName().asChar(),
            GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar(),
            mayaAttrName.asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
      return NULL;
   }
   return ProcessParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
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
      AtNode *connected = m_impl->ProcessParameterInputs(arnoldNode, plug, arnoldParamName, arnoldParamType);
      // if we're connected, we're done, otherwise call ProcessConstantParameter
      if (connected != NULL)
         return connected;
   }

   return m_impl->ProcessConstantParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
}



/// Allocate an AtArray, ProcessConstantArrayElement to fill it with values from the array plug, and call AiNodeSetArray.
/// Also calls ProcessParameterInputs.
void CNodeTranslator::ProcessArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, const MPlug& plug, unsigned int arnoldParamType, MObject *childArray)
{
   if (arnoldParamType == AI_TYPE_UNDEFINED)
   {
      const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(arnoldNode), arnoldParamName);
      const AtParamValue* defaultValue = AiParamGetDefault(paramEntry);
      arnoldParamType = defaultValue->ARRAY->type;
   }
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
         array = AiArrayAllocate(size, 1, arnoldParamType);
         for (unsigned int i=0; i < size; ++i)
         {
            AtNode* linkedNode = ExportNode(inputs[i]);
            AiArraySetPtr(array, i, linkedNode);
         }
         AiNodeSetArray(arnoldNode, arnoldParamName, array);
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
         MPlug elemPlug;
         AtArray* array = AiArrayAllocate(size, 1, arnoldParamType);
         for (unsigned int i = 0; i < size; ++i)
         {
            elemPlug = plug[i];       
            if (childArray)
            {
               elemPlug = elemPlug.child(*childArray); 
            }
            m_impl->ProcessArrayParameterElement(arnoldNode, array, arnoldParamName, elemPlug, arnoldParamType, i);
         }
         AiNodeSetArray(arnoldNode, arnoldParamName, array);
      }
   }
}

void CNodeTranslator::SetUpdateMode(UpdateMode m) {m_impl->m_updateMode = MAX(m_impl->m_updateMode, m);}
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

MObject CNodeTranslator::GetMayaObject() const { return m_impl->m_handle.object(); }
MString CNodeTranslator::GetMayaNodeName() const { return MFnDependencyNode(m_impl->m_handle.object()).name(); }
MString CNodeTranslator::GetMayaOutputAttributeName() const { return m_impl->m_handle.attribute(); }

MString CNodeTranslator::GetMayaNodeTypeName() const { return MFnDependencyNode(m_impl->m_handle.object()).typeName(); }

double CNodeTranslator::GetExportFrame() const {return GetSession()->GetExportFrame();}
bool CNodeTranslator::IsMotionBlurEnabled(int type) const { return GetSession()->IsMotionBlurEnabled(type); }
bool CNodeTranslator::IsLocalMotionBlurEnabled() const
{
   bool local_motion_attr(true);
   MPlug plug = FindMayaPlug("motionBlur");
   if (!plug.isNull())
      local_motion_attr = plug.asBool();
   return local_motion_attr;
}
unsigned int CNodeTranslator::GetMotionStep() const {return GetSession()->GetMotionStep();}
unsigned int CNodeTranslator::GetNumMotionSteps() const {return GetSession()->GetNumMotionSteps();}
CArnoldSession* CNodeTranslator::GetSession() const {return m_impl->m_session;}
const CSessionOptions& CNodeTranslator::GetSessionOptions() const  { return m_impl->m_session->GetSessionOptions(); }
/*ArnoldSessionMode*/int CNodeTranslator::GetSessionMode() const {return m_impl->m_session->GetSessionMode();}
const MObject& CNodeTranslator::GetArnoldRenderOptions() const   { return m_impl->m_session->GetArnoldRenderOptions(); }
double CNodeTranslator::GetMotionByFrame() const {return m_impl->m_session->GetMotionByFrame(); }

MString CNodeTranslator::GetTranslatorName() {return m_impl->m_abstract.name;}



