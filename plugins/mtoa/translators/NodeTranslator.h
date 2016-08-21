#pragma once

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "attributes/AttrHelper.h"

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

#define AI_ATT_SEP "."
#define AI_TAG_SEP "@"

class CNodeTranslatorImpl;
class CArnoldSession;

struct CSessionOptions;

MString GetAOVNodeType(int type);

// Abstract base class for all Maya-to-Arnold node translators
//
class DLLEXPORT CNodeTranslator
{
   // protect this class from its subclasses: make methods that should not be
   // called by subclasses private
   friend class CArnoldSession;
   friend class CExtensionsManager;
   friend class CExtension;
   friend class CRenderSwatchGenerator;
   friend class CMaterialView;
   friend class CNodeTranslatorImpl;

public:
   
   virtual ~CNodeTranslator();
   
   virtual AtNode* Init(CArnoldSession* session, const MObject& nodeObject, const MString& attrName="")
   {
      return Init(session, CNodeAttrHandle(nodeObject, attrName));
   }
   
   MObject GetMayaObject() const;
   MString GetMayaNodeName() const;
   MString GetMayaAttributeName() const;


   MString GetMayaNodeTypeName() const;
   MObject GetMayaObjectAttribute(MString attributeName) const;

   virtual AtNode* GetArnoldRootNode();
   virtual AtNode* GetArnoldNode(const char* tag="");
   virtual const char* GetArnoldNodeName(const char* tag="");
   virtual const char* GetArnoldTypeName(const char* tag="");
   MString GetTranslatorName();
   /// for translators that are associated with a specific arnold node
   MString GetArnoldNodeType();

   virtual MPlug FindMayaObjectPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;
   virtual MPlug FindMayaOverridePlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;
   virtual MPlug FindMayaPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;

   // overridable translator properties
   virtual bool IsMayaTypeDag() {return false;}

   /// Instead of caching translator exports, allow a Maya node to be exported multiple times, each time generating new arnold nodes
   virtual bool DisableCaching() {return false;}
   virtual bool DependsOnOutputPlug() {return false;} // translator performs different operations depending on the type of output plug

   virtual void TrackAOVs(AOVSet* aovs);
   virtual void TrackShaders(std::set<AtNode*> *nodes);

   // Overide this if you have some special callbacks to install.
   virtual void AddUpdateCallbacks();
   // Remove callbacks installed. This is virtual incase
   // a translator needs to do more than remove the managed
   // callbacks.
   virtual void RemoveUpdateCallbacks();
   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the Update callbacks.
   virtual void RequestUpdate();

   // this function was removed. The proper workflow now is to redefine
   // NodeChanged() and check the plug to eventually prevent the update (or set a different updateMode)
   //virtual bool RequireUpdate(const MPlug &param);

   static void NodeInitializer(CAbTranslator context);
   static void ExportUserAttributes(AtNode* anode, MObject object, CNodeTranslator* translator = 0);
      
enum UpdateMode {
   AI_UPDATE_ONLY=0,
   AI_RECREATE_NODE = 1,
   AI_RECREATE_TRANSLATOR,
   AI_DELETE_NODE   
};

   void SetUpdateMode(UpdateMode m);

protected:
   CNodeTranslator();

   // function to override to customize how a translator behaves during IPR updates
   virtual void NodeChanged(MObject& node, MPlug& plug); 

   virtual MStatus GetOverrideSets(MObject object, MObjectArray &overrideSets);
   virtual MStatus ExportOverrideSets();
   virtual MPlug GetOverridePlug(const MPlug &plug, MStatus* ReturnStatus=NULL) const;

   virtual void ComputeAOVs();
   void AddAOVDefaults(AtNode* shadingEngine, std::vector<AtNode*> &aovShaders);
   void WriteAOVUserAttributes(AtNode* atNode);
   
   AtNode* Init(CArnoldSession* session, const CNodeAttrHandle& object);

   virtual void Export(AtNode* atNode);
   virtual void ExportMotion(AtNode* atNode) {}; // by default don't do anything related to MotionBlur
   virtual void Update(AtNode* atNode){Export(atNode);}
   // UpdateMotion runs during IPR for step>0 (calls ExportMotion by default)
   virtual void UpdateMotion(AtNode* atNode){ExportMotion(atNode);}
   
   virtual bool RequiresMotionData() {return false;}
   /// Create nodes using AddArnoldNode(), and return the node which forms the root of the exported network
   virtual AtNode* CreateArnoldNodes() = 0;
   /// Return false if the passed outputAttribute is invalid
   virtual bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug);
   virtual void Delete() {}

   // Using the translator's MObject m_object and corresponding attrbuteName (default behavior)
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType);
   // For a specific Maya attribute on the translator Maya node
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MString& mayaAttrName);
   // For a specific Maya plug
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MPlug& plug);

   AtArray* InitArrayParameter(unsigned int arnoldParamType, unsigned int size);
   void SetArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, AtArray* array);
   virtual void ProcessArrayParameterElement(AtNode* arnoldNode, AtArray* array, const char* arnoldParamName, const MPlug& elemPlug, unsigned int arnoldParamType, unsigned int pos);
   virtual void ProcessArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, const MPlug& plug);
   void ProcessConstantArrayElement(int type, AtArray* array, unsigned int i, const MPlug& elem);
   AtNode* ProcessConstantParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MPlug& plug);

   void ExportUserAttribute(AtNode *anode);

   // session info
   double GetExportFrame() const;
   bool IsMotionBlurEnabled(int type = 0xFFFF/*MTOA_MBLUR_ANY*/) const; // FIXME find a way to restore MTOA_MBLUR_ANY
   bool IsLocalMotionBlurEnabled() const;
   unsigned int GetMotionStep() const;
   unsigned int GetNumMotionSteps() const;
   CArnoldSession* GetSession() const;
   const CSessionOptions& GetSessionOptions() const;
   int /*ArnoldSessionMode*/ GetSessionMode() const; // FIXME find a way to restore ArnoldSessionMode
   const MObject& GetArnoldRenderOptions() const;
   double GetMotionByFrame() const;

   // session action
   AtNode* ExportNode(const MPlug& outputPlug, bool track=true, CNodeTranslator** outTranslator = 0);
   AtNode* ExportDagPath(MDagPath &dagPath);

   virtual AtNode* AddArnoldNode(const char* type, const char* tag="");
   virtual void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

   // Add a callback to the list to manage.
   void ManageUpdateCallback(const MCallbackId id);

   // Some simple callbacks used by many translators.
   static void NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData);
   static void NameChangedCallback(MObject& node, const MString& str, void* clientData);
   static void NodeDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData);
   static void NodeDestroyedCallback(void* clientData);
   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix, const CArnoldSession* arnoldSession = 0);
   
protected:
   
   // would be better to make it private, but for now we're just trying to move there 
   // what is not necessary for the API
   CNodeTranslatorImpl * m_impl;

};
