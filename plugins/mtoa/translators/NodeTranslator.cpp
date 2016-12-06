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
   case AI_TYPE_VECTOR2:
      nodeType = "aovWritePoint2"; // not renaming the shader for now...
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

CNodeTranslator::CNodeTranslator()
{
   // we can't call CreateImplementation here as it's a virtual function and we're in the constructor...
   m_impl = NULL;
}

CNodeTranslator::~CNodeTranslator()
{
   if (m_impl)
   {
      // don't forget to remove the callbacks
      m_impl->RemoveUpdateCallbacks();
      // so, we're not calling Delete() here. This is being done manually during updates;
      // This will prevent lots of calls to RemoveReference/RemoveBackReference when we destroy everything
      delete m_impl;
   }
}

void CNodeTranslator::CreateImplementation()
{
   m_impl = new CNodeTranslatorImpl(*this);
}

AtNode* CNodeTranslator::ExportConnectedNode(const MPlug& outputPlug)
{
   return m_impl->ExportConnectedNode(outputPlug);
}



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

void CNodeTranslator::Delete()
{
	
   // Destroy all Arnold nodes for this translator
#ifdef NODE_TRANSLATOR_REFERENCES
   // First get rid of all the connections to other translators
   for (std::vector<CNodeTranslator*>::iterator it = m_impl->m_references.begin(); it != m_impl->m_references.end(); ++it)
   {
      (*it)->m_impl->RemoveBackReference(this);
   }
   m_impl->m_references.clear();

   for (unordered_set<CNodeTranslator*>::iterator it = m_impl->m_backReferences.begin(); it != m_impl->m_backReferences.end(); ++it)
   {
      (*it)->m_impl->RemoveReference(this);
   }
   m_impl->m_backReferences.clear();
#endif


   if (m_impl->m_isProcedural)
   {
      // if this node is a procedural, we want to un-register it from the Arnold Session.
      // FIXME : make we get rid of this once dependency graph is properly implemented in arnold
      m_impl->m_session->UnRegisterProcedural(m_impl->m_atNode);
   }
   
   AiNodeDestroy(m_impl->m_atNode);
   m_impl->m_atNode = NULL;
   
   if (m_impl->m_additionalAtNodes)
   {
      for (unordered_map<std::string, AtNode*>::iterator it = m_impl->m_additionalAtNodes->begin(); it != m_impl->m_additionalAtNodes->end(); ++it)
         AiNodeDestroy(it->second);
      
      delete m_impl->m_additionalAtNodes;
      m_impl->m_additionalAtNodes = NULL;
   }
   

   m_impl->m_isExported = false;

   // is there anything else to be deleted ?
   // overrideSets are created at Init (not Export) so I guess we shouldn't be deleting them here

}

void CNodeTranslator::ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
{
   const CArnoldSession* session = CMayaScene::GetArnoldSession();

   if (session)
   {
      MTransformationMatrix trMat = mayaMatrix;
      trMat.addTranslation((-1.0) * session->GetOrigin(), MSpace::kWorld);
      MMatrix copyMayaMatrix = trMat.asMatrix();

      session->ScaleMatrix(copyMayaMatrix);
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

AtNode* CNodeTranslator::GetArnoldNode(const char* tag)
{
   if (tag == NULL || strlen(tag) == 0) return m_impl->m_atNode;

   if (m_impl->m_additionalAtNodes != NULL && m_impl->m_additionalAtNodes->count(tag))
   {
      return (*(m_impl->m_additionalAtNodes))[tag];
   }
   else
   {
      // Removing the debug log because we might call GetArnoldNode to be informed of whether an entry already exists
      //AiMsgError("[mtoa.translator]  %s: Translation has not created an Arnold node with tag \"%s\".", GetTranslatorName().asChar(), tag);
      return NULL;
   }
}

AtNode* CNodeTranslator::AddArnoldNode(const char* type, const char* tag)
{
   const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(type);
   // Make sure that the given type of node exists
   if (nodeEntry != NULL)
   {
      AtNode* node = AiNode(type);
      m_impl->SetArnoldNodeName(node, tag);

      if (tag != NULL && strlen(tag))
      {
         if (m_impl->m_additionalAtNodes == NULL) 
            m_impl->m_additionalAtNodes = new unordered_map<std::string, AtNode*>();
         if (m_impl->m_additionalAtNodes->count(tag))
         {
            AiMsgWarning("[mtoa] Translator has already added Arnold node with tag \"%s\"", tag);
            return node;
         }
         else
            (*(m_impl->m_additionalAtNodes))[tag] = node;
      } else
         m_impl->m_atNode = node;
      return node;
   }
   else
   {
      AiMsgError("[mtoa.translator]   %s: Arnold node type %s does not exist.", GetTranslatorName().asChar(), type);
      return NULL;
   }
}

void CNodeTranslator::NodeChanged(MObject& node, MPlug& plug)
{  
   // When the frame is changed for motion blur we can receive signals here,
   // but we want to ignore them
   // FIXME should we test this in RequestUpdate ?
   
   if (m_impl->m_session->IsExportingMotion() && m_impl->m_session->IsInteractiveRender()) return;

   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeChanged: translator %s, providing Arnold %s(%s): %p",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              m_impl->GetArnoldNodeName(), m_impl->GetArnoldTypeName(), GetArnoldNode());

   // name of the attribute that emitted a signal
   MString plugName = plug.partialName(false, false, false, false, false, true);

   // The Arnold translator has changed :
   // This means the current one won't be able to export as it should.
   // By setting its update mode to AI_RECREATE_TRANSLATOR this translator 
   // will be cleared and a new one will be generated
   if (plugName == "aiTranslator") SetUpdateMode(AI_RECREATE_TRANSLATOR);

   // ignore this signal emitted from "Render Setup" as it shouldn't trigger a refresh
   if (plugName.length() >= 15 && plugName.substringW(0, 14) == "renderLayerInfo") return;

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
                                                   NodeAboutToBeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // Just so people don't get confused with debug output.
   id = MNodeMessage::addNameChangedCallback(object,
                                             NameChangedCallback,
                                             this,
                                             &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   /* We shouldn't need this callback anymore as we already queue the node for deletion in NodeAboutToBeDeleted
   
   id = MNodeMessage::addNodeDestroyedCallback(object,
                                               NodeDestroyedCallback,
                                               this,
                                               &status);*/
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
      if (translator->m_impl->m_session->IsExportingMotion() && translator->m_impl->m_session->IsInteractiveRender()) return;
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
      translator->m_impl->SetArnoldNodeName(translator->GetArnoldNode());
      AiMsgDebug("[mtoa.translator.ipr]  %-30s | %s: NameChangedCallback: %p",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNode());
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NameChangedCallback: no translator in client data: %p.", translator->GetMayaNodeName().asChar(), clientData);
   }

   CMayaScene::GetRenderSession()->ObjectNameChanged(node, str);
}

// Arnold doesn't really support deleting nodes. But we can make things invisible,
// disconnect them, turn them off, etc.
void CNodeTranslator::NodeAboutToBeDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData)
{
   CNodeTranslator* translator = static_cast<CNodeTranslator*>(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Node deleted, deleting processed translator instance, client data: %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), clientData);

      // we're now always requesting an update when a node is deleted
//      if(node.apiType() == MFn::kMesh || node.apiType() == MFn::kLight)
      translator->SetUpdateMode(CNodeTranslator::AI_DELETE_NODE);

      translator->RequestUpdate();  
   }
   else
   {
      MFnDependencyNode dnode(node);
      AiMsgWarning("[mtoa.translator.ipr] %-30s | Translator callback for node deleted, no translator in client data: %p.",
                   dnode.name().asChar(), clientData);
   }
}

/* We shouldn't need this callback as we already receive NodeAboutToBeDeleted and queue the node for deletion

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
*/

void CNodeTranslator::RequestUpdate()
{

   // if hold updates is enabled, don't ask for updates
   if (m_impl->m_inUpdateQueue) return;

   // we're changing the frame to evaluate motion blur, so we don't want more 
   // updates now
   if (m_impl->m_session->IsInteractiveRender() && m_impl->m_session->IsExportingMotion()) return;

   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate: Arnold node %s(%s): %p.",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
              m_impl->GetArnoldNodeName(), m_impl->GetArnoldTypeName(), GetArnoldNode());


   m_impl->m_session->QueueForUpdate(this);   

   if (m_impl->m_isProcedural && m_impl->m_updateMode >= AI_RECREATE_NODE)
   {
      // If this is a procedural being re-generated, we must
      // advert the arnold session so that it checks for all 
      // lost connections
      m_impl->m_session->QueueProceduralUpdate(this);
   }
   
   // Pass the update request to the export session
   m_impl->m_session->RequestUpdate();
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
   AtRGB rgb = AtRGB(p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat());
   AiArraySetRGB(arr, element, rgb);
}

template <>
void TExportArrayAttribute<AI_TYPE_RGBA>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtRGBA rgba = AtRGBA (p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat(), p.child(3).asFloat());
   AiArraySetRGBA(arr, element, rgba);
}

template <>
void TExportArrayAttribute<AI_TYPE_VECTOR>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtVector vec = AtVector(p.child(0).asFloat(), p.child(1).asFloat(), p.child(2).asFloat());
   AiArraySetVec(arr, element, vec);
}


template <>
void TExportArrayAttribute<AI_TYPE_VECTOR2>(AtArray* arr, MPlug& plug, unsigned int element)
{
   MPlug p = plug[element];
   AtVector2 vec2 = AtVector2(p.child(0).asFloat(), p.child(1).asFloat());
   AiArraySetVec2(arr, element, vec2);
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
void TExportAttribute<AI_TYPE_VECTOR2>(AtNode* node, MPlug& plug, const char* attrName)
{
   AiNodeSetVec2(node, attrName, plug.child(0).asFloat(), plug.child(1).asFloat());
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
   if (AiNodeLookUpUserParameter(node, attrName) == NULL)
   {
      if (!declarationPointers[declType](node, attrName, ATTR)) return;
   }

   const unsigned int numElements = plug.numElements();
   AtArray* arr = AiArrayAllocate(numElements, 1, ATTR);
   for (unsigned int i = 0; i < numElements; ++i)
      TExportArrayAttribute<ATTR>(arr, plug, i);
   AiNodeSetArray(node, attrName, arr);

}

template <signed ATTR>
void TExportUserAttribute(AtNode* node, MPlug& plug, const char* attrName, EAttributeDeclarationType declType)
{
   if (plug.isArray())
      TExportUserAttributeArray<ATTR>(node, plug, attrName, declType);
   else
   {

      if (AiNodeLookUpUserParameter(node, attrName) == NULL)
      {
         if (!AiNodeDeclareConstant(node, attrName, ATTR)) return;
      }

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
   AtVector vec = AtVector((float)data[element].x, (float)data[element].y, (float)data[element].z);
   AiArraySetVec(array, element, vec);
}

template <>
void TExportUserAttributeData<AI_TYPE_RGB, MFnVectorArrayData>(AtArray* array, MFnVectorArrayData& data, unsigned int element)
{
   AtRGB rgb = AtRGB((float)data[element].x, (float)data[element].y, (float)data[element].z);
   AiArraySetRGB(array, element, rgb);
}

template <>
void TExportUserAttributeData<AI_TYPE_VECTOR, MFnPointArrayData>(AtArray* array, MFnPointArrayData& data, unsigned int element)
{
   AtVector vec = AtVector((float)data[element].x, (float)data[element].y, (float)data[element].z);
   AiArraySetVec(array, element, vec);
}

template <>
void TExportUserAttributeData<AI_TYPE_RGB, MFnPointArrayData>(AtArray* array, MFnPointArrayData& data, unsigned int element)
{
   AtRGB rgb = AtRGB((float)data[element].x, (float)data[element].y, (float)data[element].z);
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
      
      /*
      This line was preventing IPR updates of user attributes
      if (AiNodeLookUpUserParameter(anode, aname) != NULL)
         continue;
         */

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
            TExportUserAttribute<AI_TYPE_VECTOR2>(anode, pAttr, aname, attributeDeclaration);
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
            AtNode* connectedNode = translator->ExportConnectedNode(connectedPlug);
            if (connectedNode != 0)
            {
               if (AiNodeLookUpUserParameter(anode, aname) == NULL) 
               {
                  if (!AiNodeDeclareConstant(anode, aname, AI_TYPE_NODE)) continue;
               }
               AiNodeSetPtr(anode, aname, connectedNode);
            }
         }
      }      
      //else
      //   AiMsgError("[mtoa.translator]  %s: Unsupported user attribute type for %s",
      //         GetTranslatorName().asChar(), pAttr.partialName(true, false, false, false, false, true).asChar());
   }
}


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
      plug = FindMayaPlug(mayaAttrName, &status);
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
            GetMayaNodeName().asChar(), m_impl->GetMayaNodeTypeName().asChar(),
            mayaAttrName.asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
      return NULL;
   }
   return ProcessParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
}

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

   // It doesn't make sense to call this method during motion export
   if (IsExportingMotion())
   {
      AiMsgWarning("[mtoa] [translator %s] %s.%s: ProcessParameter should not be called during motion export",
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

void CNodeTranslator::ProcessArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, const MPlug& plug, unsigned int arnoldParamType, MObject *childArray)
{
   if (arnoldParamType == AI_TYPE_UNDEFINED)
   {
      const AtParamEntry* paramEntry = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(arnoldNode), arnoldParamName);
      const AtParamValue* defaultValue = AiParamGetDefault(paramEntry);
      arnoldParamType = AiArrayGetType(defaultValue->ARRAY());
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
            AtNode* linkedNode = ExportConnectedNode(inputs[i]);
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

void CNodeTranslator::SetUpdateMode(UpdateMode m) 
{
   if (m_impl->m_updateMode >= m) return; 

   m_impl->m_updateMode = m;

   if (m == AI_DELETE_NODE)
   {
      // needs to delete the sourceTranslator too
      if (m_impl->m_sourceTranslator) 
      {
         m_impl->m_sourceTranslator->SetUpdateMode(AI_DELETE_NODE);
         m_impl->m_sourceTranslator->RequestUpdate();
      }

      // We'll delete this node at next Render Update
      // We should advert our back references to re-export 
      for (unordered_set<CNodeTranslator*>::iterator it = m_impl->m_backReferences.begin(); it != m_impl->m_backReferences.end(); ++it)
      {
         (*it)->RequestUpdate();
      }
      
      // no longer receive any callbacks from this node, we're done
      m_impl->RemoveUpdateCallbacks();
      // removing translator from the list now while it is still active.
      // Note that the translator itself (and the Arnold nodes) will only 
      // be deleted in the next IPR update (CArnoldSession::DoUpdate()).
      // But for now we just want to remove the translator from the session list
      // since it's being accessed in the map based on the MObject/MPlug/MDagPath, etc...
      m_impl->m_session->EraseActiveTranslator(this);
   } else if (m >= AI_RECREATE_NODE)
   {
      // Since we'll recreate the arnold node, we must tell our back references to re-export
      for (unordered_set<CNodeTranslator*>::iterator it = m_impl->m_backReferences.begin(); it != m_impl->m_backReferences.end(); ++it)
      {
         (*it)->RequestUpdate();
      }
   }
}

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


double CNodeTranslator::GetExportFrame()
{
   return CMayaScene::GetArnoldSession()->GetExportFrame();
}
bool CNodeTranslator::IsMotionBlurEnabled(int type)
{
   return CMayaScene::GetArnoldSession()->IsMotionBlurEnabled(type); 
}

bool CNodeTranslator::IsLocalMotionBlurEnabled() const
{
   bool local_motion_attr(true);
   MPlug plug = FindMayaPlug("motionBlur");
   if (!plug.isNull())
      local_motion_attr = plug.asBool();
   return local_motion_attr;
}
unsigned int CNodeTranslator::GetMotionStep()
{   
   return (m_impl->m_session->IsExportingMotion() || RequiresMotionData()) ? CMayaScene::GetArnoldSession()->GetMotionStep() : 0;
}

unsigned int CNodeTranslator::GetNumMotionSteps()
{
   return (m_impl->m_session->IsExportingMotion() || RequiresMotionData()) ? CMayaScene::GetArnoldSession()->GetNumMotionSteps() : 1;
}
const CSessionOptions& CNodeTranslator::GetSessionOptions()
{
   return CMayaScene::GetArnoldSession()->GetSessionOptions(); 
}
ArnoldSessionMode CNodeTranslator::GetSessionMode()
{
   return CMayaScene::GetArnoldSession()->GetSessionMode();
}
const MObject& CNodeTranslator::GetArnoldRenderOptions()
{
   return CMayaScene::GetArnoldSession()->GetArnoldRenderOptions(); 
}

double CNodeTranslator::GetMotionByFrame()
{
   return CMayaScene::GetArnoldSession()->GetMotionByFrame(); 
}

MString CNodeTranslator::GetTranslatorName() {return m_impl->m_abstract.name;}

const double *CNodeTranslator::GetMotionFrames(unsigned int &count)
{
   const std::vector<double> &motionFrames = CMayaScene::GetArnoldSession()->GetMotionFrames();
   count = motionFrames.size();
   
   return (count == 0) ? NULL : &motionFrames[0];
}

CNodeTranslator *CNodeTranslator::GetTranslator(const MDagPath &dagPath)
{
   CArnoldSession *session = CMayaScene::GetArnoldSession();
   std::vector<CNodeTranslator*> translators;
   CNodeAttrHandle handle(dagPath);
   return session->GetActiveTranslator(handle);
}

CNodeTranslator *CNodeTranslator::GetTranslator(const MObject &object)
{
   CArnoldSession *session = CMayaScene::GetArnoldSession();
   std::vector<CNodeTranslator*> translators;
   CNodeAttrHandle handle(object);
   return session->GetActiveTranslator(handle);
}


void CNodeTranslator::RequestLightLinksUpdate()
{
   CMayaScene::GetArnoldSession()->FlagLightLinksDirty(true);

}
void CNodeTranslator::RequestTxUpdate()
{
   CMayaScene::GetArnoldSession()->RequestUpdateTx();
}
MString CNodeTranslator::GetArnoldNaming(const MObject &object)
{
   MString name = MFnDependencyNode(object).name();
   const MString &prefix = GetSessionOptions().GetExportPrefix();
   if (prefix.length() > 0)
      name = prefix + name;
   return name;
}


bool CNodeTranslator::IsExportingMotion() const
{
   return m_impl->m_session->IsExportingMotion();
}
