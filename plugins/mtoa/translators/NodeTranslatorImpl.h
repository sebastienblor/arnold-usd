#pragma once
#include "NodeTranslator.h"
#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "attributes/AttrHelper.h"
#include "session/ArnoldSession.h"
#include "extension/AbTranslator.h"
#include "render/AOV.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCallbackIdArray.h>
#include <maya/MNodeMessage.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>


#define AI_ATT_SEP "."
#define AI_TAG_SEP "@"

#define NODE_TRANSLATOR_REFERENCES 1

class CNodeTranslator;


class CNodeTranslatorImpl
{
public :
   CNodeTranslatorImpl(CNodeTranslator &translator) : 
      m_handle(CNodeAttrHandle()),
      m_updateMode(CNodeTranslator::AI_UPDATE_ONLY),
      m_abstract(CAbTranslator()),
      m_session(NULL),
      m_atNode(NULL),
      m_additionalAtNodes(NULL),
      m_atRoot(NULL),
      m_overrideSets(),
      m_inUpdateQueue(false),
      m_animArrays(false),
      m_isExported(false),
      m_overrideSetsDirty(false),
      m_tr(translator){}
   virtual ~CNodeTranslatorImpl() {}

   virtual bool IsMayaTypeDag() const {return false;}
   
   /// Return false if the passed outputAttribute is invalid
   virtual bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug);
   
   virtual MString MakeArnoldName(const char *nodeType, const char* tag = NULL);
   virtual void DoCreateArnoldNodes();
   virtual void ExportDccName();

   AtNode* DoExport();
   AtNode* DoUpdate();
   
   AtNode* ProcessParameterInputs(AtNode* arnoldNode, const MPlug &plug,
                                                const char* arnoldParamName,
                                                int arnoldParamType);

   bool ProcessParameterComponentInputs(AtNode* arnoldNode, const MPlug &parentPlug,
                                                      const char* arnoldParamName,
                                                      int arnoldParamType);




   MPlug FindMayaOverridePlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;
   MPlug FindMayaObjectPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;

   // Remove callbacks installed. 
   void RemoveUpdateCallbacks();
   void Init(CArnoldSession* session, const MObject& nodeObject, const MString& attrName="", int instanceNumber = -1)
   {
      Init(session, CNodeAttrHandle(nodeObject, attrName, instanceNumber));
   }
   void Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      Init(session, CNodeAttrHandle(dagPath, outputAttr));
   }
   void Init(CArnoldSession* session, const CNodeAttrHandle& object);

   virtual MStatus ExportOverrideSets();
   static void DirtyOverrideSets(CNodeTranslator *tr);
   MPlug GetOverridePlug(const MPlug &plug, MStatus* ReturnStatus=NULL) const;

   AtNode* ProcessConstantParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MPlug& plug);
   void ProcessArrayParameterElement(AtNode* arnoldNode, AtArray* array, const char* arnoldParamName, const MPlug& elemPlug, unsigned int arnoldParamType, unsigned int pos);
   void ProcessConstantArrayElement(int type, AtArray* array, unsigned int i, const MPlug& elem);

   AtNode* ExportConnectedNode(const MPlug& outputPlug, bool track=true, CNodeTranslator** outTranslator = NULL);
   bool HasAnimatedArrays() const;

   inline bool DependsOnOutputPlug() {return m_tr.DependsOnOutputPlug();}

   /// Get the name of the Arnold node
   const char* GetArnoldNodeName();
   /// Get the type of the Arnold node
   const char* GetArnoldTypeName();
   /// Get the type of the Maya node. Mainly used for debug logs : do we want to keep it in the API?
   MString GetMayaNodeTypeName() const;

   static void AddNamingOptions(MString &name);

   CNodeAttrHandle m_handle;
   CNodeTranslator::UpdateMode m_updateMode;
   CAbTranslator m_abstract;

   CArnoldSession* m_session;

   AtNode* m_atNode;
   unordered_map<std::string, AtNode*> *m_additionalAtNodes;
   AtNode *m_atRoot; // shortcut to the node which is at the root of this translator
   // do not delete, it is supposed to be one of the translators nodes

   std::vector<CNodeTranslator*> m_overrideSets;

   // This stores callback IDs for the callbacks this
   // translator creates.
   MCallbackIdArray m_mayaCallbackIDs;
   bool m_inUpdateQueue; // for Arnold RenderView only
   bool m_animArrays;
   bool m_isExported;
   bool m_overrideSetsDirty;

   virtual void ExportUserAttribute(AtNode *anode);

   void AddReference(CNodeTranslator *tr, bool addReciprocal = true)
   {
      if(std::find(m_references.begin(), m_references.end(), tr) != m_references.end())
      {
         // translator already in our reference list
         // nothing to do
         return;
      }

      m_references.push_back(tr);

      // now add the back reference
      if (addReciprocal)
         tr->m_impl->AddBackReference(&m_tr, false);
   }
   void AddBackReference(CNodeTranslator *tr, bool addReciprocal = false)
   {
      // add this translator to our list of back references.
      // As this is a set, it might already be present
      m_backReferences.insert(tr);
      if (addReciprocal)
         tr->m_impl->AddReference(&m_tr, false);
   }
   void RemoveReference(CNodeTranslator *tr)
   {
      std::vector<CNodeTranslator*>::iterator it = std::find(m_references.begin(), m_references.end(), tr);
      if (it == m_references.end()) return; // how can this be called ?

      // fast erase, swap this vector element and the last one, then pop_back
      std::swap(*it, m_references.back());
      m_references.pop_back();

      // should we call tr->RemoveBackReference here ?
   }
   void RemoveBackReference(CNodeTranslator *tr)
   {
      m_backReferences.erase(tr);
      // for now we shouldn't need this test, as the reference is the other way around
   }
   void RemoveAllReferences()
   {
      if (m_references.empty()) return;
      std::vector<CNodeTranslator*>::iterator it = m_references.begin();
      std::vector<CNodeTranslator*>::iterator itEnd = m_references.end();
      for( ; it != itEnd; ++it)
      {
         (*it)->m_impl->RemoveBackReference(&m_tr);
      }
      m_references.clear();
   }
   void RemoveAllBackReferences()
   {
      if (m_backReferences.empty()) return;
      unordered_set<CNodeTranslator*>::iterator it = m_backReferences.begin();
      unordered_set<CNodeTranslator*>::iterator itEnd = m_backReferences.end();
      for( ; it != itEnd; ++it)
      {
         (*it)->m_impl->RemoveReference(&m_tr);
      }
      m_backReferences.clear();
   }

   // we could use AmSet for both, but in practice a node is usually connected to only a few other nodes.
   // On the other hand a single node could be referenced by thousands of other ones, 
   // for example a single shader assigned to the whole scene
   std::vector<CNodeTranslator *> m_references;
   unordered_set<CNodeTranslator *> m_backReferences;
protected:


protected:
   CNodeTranslator &m_tr;
};