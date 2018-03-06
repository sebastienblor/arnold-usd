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
#include "utils/MtoaLog.h"
#include "utils/time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// internal use only
AtNode* CNodeTranslatorImpl::DoExport()
{
   AtNode* node = m_atNode;
   MString outputAttr = m_tr.GetMayaOutputAttributeName();
   
   // FIXME : for now we're setting isExported to false when we ask for a full re-export
   // but as refactoring continues we'll stop doing it. 
   // And we'll restore it to false only when Delete() is called
   m_isExported = false; 
   if (node == NULL)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | Export requested but no Arnold node was created by this translator ("+ m_tr.GetTranslatorName() +")");
                    
      return NULL;
   }
   if (m_overrideSetsDirty) ExportOverrideSets();

   if (!m_session->IsExportingMotion())
   {
      if (MtoaTranslationInfo())
      {
         if (outputAttr != "")
            MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | Exporting on plug "+outputAttr+" ("+m_tr.GetTranslatorName()+")");
         else
            MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | Exporting ("+m_tr.GetTranslatorName()+")");
                       
      }

      m_tr.Export(node);
      ExportUserAttribute(node);
   }
   else if (m_tr.RequiresMotionData())
   {
      if (MtoaTranslationInfo())
      {
         if (outputAttr != "")
            MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | Exporting Motion on plug "+outputAttr+" ("+m_tr.GetTranslatorName()+")");
         else
            MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | Exporting Motion ("+m_tr.GetTranslatorName()+")");
      }
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
   
   if (node == NULL)
   {
      return NULL;
   }
   
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | "+
         m_tr.GetTranslatorName()+": Updating Arnold "+MString(AiNodeGetName(node))+
         "("+MString(AiNodeEntryGetName(AiNodeGetNodeEntry(node)))+")");
              

   if (m_overrideSetsDirty) ExportOverrideSets();

   if (!m_session->IsExportingMotion())
   {
#ifdef NODE_TRANSLATOR_REFERENCES 
      // before exporting, clear all the references
      // Note that this shouldn't happen here
      std::vector<CNodeTranslator *> previousRefs = m_references;
      m_references.clear();
#endif

      m_tr.Export(node);
      ExportUserAttribute(node);

#ifdef NODE_TRANSLATOR_REFERENCES 
      // now for all previous references, check if one of them has disappeared
      if (!previousRefs.empty())
      {
         // main goal is to make it fast on the most common cases,
         // which is when the list of references doesn't change during updates
         unsigned int minSize = AiMin(previousRefs.size(), m_references.size());
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
   // m_atRoot is what's at the root of this translator
   // It is not necessarily the main node (which happens with aov_write shaders)
   m_atRoot = m_tr.CreateArnoldNodes();

   // if no main arnold node has been created, set it here
   if (m_atNode == NULL)
      m_atNode = m_atRoot;

   // if CreateArnoldNodes forgot to return the root node, let's use 
   // the main arnold node
   if (m_atRoot == NULL)
      m_atRoot = m_atNode;

   if (MtoaTranslationInfo())
   {
      MString log = "[mtoa.translator]  "+m_tr.GetMayaNodeName()+" ("+GetMayaNodeTypeName()+"): ";
      log += "Translator "+m_tr.GetTranslatorName();
      if (m_atNode == NULL)
         log += " didn't create any node";
      else
      {
         log += " created arnold node ";
         log += AiNodeGetName(m_atNode);
         if (m_additionalAtNodes != NULL)
         {
            // note that the "root" m_atNode can be registered as a tagged node as well,
            // so this should be double-checked in the counting here
            int totalAdditionalNodes = 0;
            unordered_map<std::string, AtNode*>::const_iterator it = m_additionalAtNodes->begin();
            unordered_map<std::string, AtNode*>::const_iterator itEnd = m_additionalAtNodes->end();

            for ( ; it != itEnd; ++it)
            {
               AtNode *node = it->second;
               if (node != NULL && node != m_atNode)
                  totalAdditionalNodes++;
            }
            if (totalAdditionalNodes > 0)
            {
               log += ", plus ";
               log += totalAdditionalNodes;
               log += " additional nodes";
            }
         }
      }
      MtoaDebugLog(log);
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

   // closures will return 0 components
   if (numComponents == 0)
      return false;

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
   
   // first eventually initialize the translator
   m_tr.Init();

   ExportOverrideSets();

   // then create the arnoldNodes
   DoCreateArnoldNodes();

   // If the "arnold" name of this translator hasn't been set yet 
   // we set it with the type of the created arnold node
   if (m_abstract.arnold.length() == 0 && m_atNode)
      m_abstract.arnold = AiNodeEntryGetName(AiNodeGetNodeEntry(m_atNode));
   
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
         /*
         Commented out from ticket #2112, this seems to be useless now
            
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
         }*/
         if (set.hasFn(MFn::kSet))
            overrideSets.append(set);
      }
   }
   return status;
}



bool CNodeTranslatorImpl::ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug)
{
   resolvedOutputPlug=outputPlug;
   return true;
}

/// gather the active override sets containing this node
MStatus CNodeTranslatorImpl::ExportOverrideSets()
{
   MStatus status;

   MString nodeName = m_tr.GetMayaNodeName();
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
   if (MtoaTranslationInfo())
   {
      MString log = "[mtoa.translator]  "+m_tr.GetMayaNodeName()+" | "+m_tr.GetTranslatorName()+": Exported ";
      log += ns;
      log += " override sets.";
      MtoaDebugLog(log);
   }

   m_overrideSetsDirty = false;
   return status;

}
void CNodeTranslatorImpl::DirtyOverrideSets(CNodeTranslator *tr)
{
   if (tr == NULL)
      return;
   tr->m_impl->m_overrideSetsDirty = true;
   tr->RequestUpdate();
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
   {
      // FIXME Arnold5 what should we do with closures here ?
   case AI_TYPE_CLOSURE:
      // so I have a constant closure parameter. if in maya it is a color, I want to plug 
      // a closure node that returns a basic emission color
      unsigned int numChildren = plug.numChildren();
      if (numChildren== 3 || numChildren == 4)
      {
         std::string closureName = arnoldParamName;
         closureName += "_clos";
         AtNode *child = m_tr.GetArnoldNode(closureName.c_str());
         if (child == NULL)
            child = m_tr.AddArnoldNode("MayaFlatClosure", closureName.c_str());

         AiNodeSetRGB(child, "color", plug.child(0).asFloat(),
                         plug.child(1).asFloat(),
                         plug.child(2).asFloat());
         AiNodeLink(child, arnoldParamName, arnoldNode);

      }

      break;
   }
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
               AiMsgInfo("[mtoa] RGBA attribute %s has no alpha component: exporting as RGBA",
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
               if (mentry->value.INT() == 1) // scale distance
                  m_session->ScaleDistance(val);
            }
            continue;
         }
         AiMetaDataIteratorDestroy(miter);

         AiNodeSetFlt(arnoldNode, arnoldParamName, val);
      }
      break;
   case AI_TYPE_VECTOR2:
      {
         float x, y;
         MObject numObj = plug.asMObject();
         MFnNumericData numData(numObj);
         numData.getData2Float(x, y);
         AiNodeSetVec2(arnoldNode, arnoldParamName, x, y);
      }
      break;
   case AI_TYPE_MATRIX:
      {
         // special case for shaders with matrix values that represent transformations
         // FIXME: introduce "xform" metadata to explicitly mark a matrix parameter
         if (m_tr.RequiresMotionData() && strcmp(arnoldParamName, "placementMatrix") == 0)
         {
            // create an interpolation node for matrices
            AtNode* animNode = m_tr.GetArnoldNode(arnoldParamName);
            if (animNode == NULL)
               animNode = m_tr.AddArnoldNode("MtoaAnimMatrix", arnoldParamName);

            AtArray* matrices = AiArrayAllocate(1, m_tr.GetNumMotionSteps(), AI_TYPE_MATRIX);

            ProcessConstantArrayElement(AI_TYPE_MATRIX, matrices, m_tr.GetMotionStep(), plug);

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
   case AI_TYPE_NODE:
      // handled above by ProcessParameterInputs
      break;
   case AI_TYPE_ARRAY:
      {
         if (!plug.isArray())
         {
            MGlobal::displayError("[mtoa] Arnold parameter is of type array, but corresponding Maya attribute is not : " + plug.name());
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

      // Since Arnold 5, the arrays aren't initialized anymore, so even if it's connected 
      // we want to process it anyway
      switch(arnoldParamType)
      {
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
      case AI_TYPE_VECTOR2:
      case AI_TYPE_VECTOR:
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
      // now check if there is a connection to another node
      ProcessParameterInputs(arnoldNode, elemPlug, elemName.asChar(), arnoldParamType);
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
   // FIXME Arnold5 anything to be done with closures here ?
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
   case AI_TYPE_VECTOR2:
      {
         float x, y;
         MObject numObj = elem.asMObject();
         MFnNumericData numData(numObj);
         numData.getData2Float(x, y);
         AtVector2 vec2;
         vec2 = AtVector2(x, y);
         AiArraySetVec2(array, i, vec2);
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
         AtVector vec3 = AtVector(elem.child(0).asFloat(), elem.child(1).asFloat(), elem.child(2).asFloat());
         AiArraySetVec(array, i, vec3);
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
// FIXME note that "track" isn't needed anymore
AtNode* CNodeTranslatorImpl::ExportConnectedNode(const MPlug& outputPlug, bool track, CNodeTranslator** outTranslator)
{
   CNodeTranslator* translator = NULL;

   translator = m_session->ExportNode(outputPlug);

   if (translator != NULL)
   {
      if (outTranslator != NULL)
         *outTranslator = translator;

#ifdef NODE_TRANSLATOR_REFERENCES 
      AddReference(translator);
#endif

      if (translator->m_impl->m_atRoot == NULL && outputPlug.node().hasFn(MFn::kShadingEngine))
      {
         // I've exported a shading engine, but it returned a null result. 
         // This happens because there isn't any custom AOV.
         // In that case, to avoid breaking API compatibility for 3rd party extensions, 
         // I'm doing a passthrough and returning the connected shader here
         
         // FIXME when compatibility can be broken, we should refactor this,
         // I shouldn't have to do guesses based on the attribute name.
         MFnDependencyNode sgNode(outputPlug.node());
         
         AtNode *node = m_tr.GetArnoldNode();
         bool isVolume = false;
         static const AtString polymeshStr("polymesh");
         static const AtString pointsStr("points");
         static const AtString sphereStr("sphere");
         static const AtString boxStr("box");
         static const AtString proceduralStr("procedural");

         if (AiNodeIs(node, polymeshStr) || AiNodeIs(node, pointsStr) || AiNodeIs(node, boxStr) || AiNodeIs(node, sphereStr))
         {
            MFnDependencyNode fnDGNode(m_tr.GetMayaObject());
            MPlug stepSizePlug = fnDGNode.findPlug("stepSize");
            if (stepSizePlug.isNull())
               stepSizePlug = fnDGNode.findPlug("aiStepSize");

            if (!stepSizePlug.isNull())
               isVolume = (stepSizePlug.asFloat() > AI_EPSILON);

         }
         else if (AiNodeIs(node, proceduralStr)) // standins
            isVolume = false;
         else
         {
            // FIXME verify that native shapes like "volume" and "volume_implicit"
            // have derived types as well
            switch (AiNodeEntryGetDerivedType(AiNodeGetNodeEntry(node)))
            {
               case AI_NODE_SHAPE_VOLUME:
                  isVolume = true;
               break;
               case AI_NODE_SHAPE_IMPLICIT:
                  isVolume = false;
               break;
               case AI_NODE_SHAPE_PROCEDURAL:
                  isVolume = false;
               break;
               default:
               break;
            }
         } 
         
         MStringArray shaderAttrNames;

         // change the order of priority used to check the shader inputs
         // depending on whether we're supposed to render a volume or not
         if (isVolume)
         {
            shaderAttrNames.append("aiVolumeShader");
            shaderAttrNames.append("volumeShader");
            shaderAttrNames.append("aiSurfaceShader");
            shaderAttrNames.append("surfaceShader");
         } else
         {
            shaderAttrNames.append("aiSurfaceShader");
            shaderAttrNames.append("surfaceShader");
            shaderAttrNames.append("aiVolumeShader");
            shaderAttrNames.append("volumeShader");
         }
         MPlugArray connections;
         MPlug shaderPlug;
         
         for (unsigned int i = 0; i < 4; ++i)
         {
            MPlug plug = sgNode.findPlug(shaderAttrNames[i]);
            if (plug.isNull()) continue;
            
            connections.clear();
            plug.connectedTo(connections, true, false);
            if (connections.length() > 0)
            {
               shaderPlug = connections[0];
               break;
            }
         }

         if (!shaderPlug.node().hasFn(MFn::kShadingEngine)) // ensuring we're not entering a possible infinite loop
            return ExportConnectedNode(shaderPlug);
         
      }

      return translator->m_impl->m_atRoot; // return the node that is at the root of this translator
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
MString CNodeTranslatorImpl::MakeArnoldName(const char *nodeType, const char* tag)
{
   MString name = CNodeTranslator::GetArnoldNaming(m_tr.GetMayaObject());

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
      // FIXME this was copied from NodeUniqueName in time.h,
      // remember to change the original function in next ABI-breaking release
      char tmpName[MAX_NAME_SIZE];
      sprintf(tmpName, "%s_%08X%08llX",
         nodeType, MtoaTime(), MtoaTicks());
      
      name = tmpName;
   }
   return name;
}


// check if an AtArray is animated,  i.e. has different values on multiple keys
// it would be nice if this could be done in arnold core
static inline bool IsArrayAnimated(const AtArray* array)
{
   unsigned numElements = AiArrayGetNumElements(array);
   uint8_t numKeys = AiArrayGetNumKeys(array);

   switch (AiArrayGetType(array))
   {
      case AI_TYPE_BOOLEAN:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            bool valInit = AiArrayGetBool(array, i);

            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetBool(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_BYTE:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtByte valInit = AiArrayGetByte(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetByte(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_INT:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            int valInit = AiArrayGetInt(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetInt(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_UINT:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtUInt32 valInit = AiArrayGetUInt(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetUInt(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_FLOAT:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            float valInit = AiArrayGetFlt(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetFlt(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_RGB:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtRGB valInit = AiArrayGetRGB(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetRGB(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_RGBA:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtRGBA valInit = AiArrayGetRGBA(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetRGBA(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_VECTOR:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtVector valInit = AiArrayGetVec(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetVec(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_VECTOR2:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtVector2 valInit = AiArrayGetVec2(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if (valInit != AiArrayGetVec2(array, i + (k * numElements))) return true;
            }
         }
      return false;
      case AI_TYPE_MATRIX:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            AtMatrix mtxInit, mtx;
            mtxInit = AiArrayGetMtx(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               mtx = AiArrayGetMtx(array, i + (k * numElements));
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
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            const char *valInit = AiArrayGetStr(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if(strcmp(valInit, AiArrayGetStr(array, i + (k * numElements))) != 0) return true; 
            }
         }
      return false;
      case AI_TYPE_POINTER:
         for (AtUInt32 i = 0; i < numElements; ++i)
         {
            void *valInit = AiArrayGetPtr(array, i);
            for (AtByte k = 1; k < numKeys; ++k)
            {
               if(valInit != AiArrayGetPtr(array, i + (k * numElements))) return true; 
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

   // if motion length is null, return true otherwise next change won't be updated properly
   double motion_start, motion_end;
   m_session->GetMotionRange(motion_start, motion_end);
   if (std::abs(motion_end - motion_start) < AI_EPSILON)
      return true;

   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(m_atNode));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      if (AiParamGetType(paramEntry) != AI_TYPE_ARRAY)
      {
         continue;
      }
      AtArray *array = AiNodeGetArray(m_atNode, AiParamGetName(paramEntry));
      if (array != NULL && AiArrayGetNumKeys(array) > (AtByte)1)
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
      unordered_map<std::string, AtNode*>::const_iterator it = m_additionalAtNodes->begin();
      unordered_map<std::string, AtNode*>::const_iterator itEnd = m_additionalAtNodes->end();

      for ( ; it != itEnd; ++it)
      {
         AtNode *node = it->second;
         if (node == NULL || node == m_atNode) continue;

         nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
         while (!AiParamIteratorFinished(nodeParam))
         {
            const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
            if (AiParamGetType(paramEntry) != AI_TYPE_ARRAY)
            {
               continue;
            }
            AtArray *array = AiNodeGetArray(node, AiParamGetName(paramEntry));
            if (array != NULL && AiArrayGetNumKeys(array) > (AtByte)1)
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

const char* CNodeTranslatorImpl::GetArnoldNodeName()
{
   AtNode *node = m_tr.GetArnoldNode();
   if (node == NULL) return "";
   return AiNodeGetName(node);
}

const char* CNodeTranslatorImpl::GetArnoldTypeName()
{
   AtNode* node = m_tr.GetArnoldNode();
   if (NULL == node)
   {
      return NULL;
   }
   else
   {
      return AiNodeEntryGetName(AiNodeGetNodeEntry(node));
   }
}
MString CNodeTranslatorImpl::GetMayaNodeTypeName() const { return MFnDependencyNode(m_handle.object()).typeName(); }



