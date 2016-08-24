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
   
   //------------------ Getters. Check if all of them are necessary

   // Get the MObject associated to this translator
   MObject GetMayaObject() const;

   // for multi-output translators, get the name of the output attribute in maya
   MString GetMayaOutputAttributeName() const;

   // Get the type of the Maya node. Mainly used for debug logs : do we want to keep it in the API?
   MString GetMayaNodeTypeName() const;

   // Get the main Arnold node for this translator
   AtNode* GetArnoldRootNode();

   // In case multiple Arnold nodes are registered with this translator, returns the one for a given tag
   AtNode* GetArnoldNode(const char* tag="");

   // Returns true if this translator has already been exported
   bool IsExported() const;

   // Get the the Maya plug for this given attribute name
   MPlug FindMayaPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;


   //---- The 4 functions below are mainly used for debug lobs. Do we want to keep them in the public API ?
   
   // Get the name of the Maya object. 
   MString GetMayaNodeName() const;
   // Get the name of the Arnold node
   const char* GetArnoldNodeName(const char* tag="");
   // Get the type of the Arnold node
   const char* GetArnoldTypeName(const char* tag="");
   // Get the name of this translator
   MString GetTranslatorName();

   // -------------------- Check if the functions below are really necessary for extensions
   // ---------------------If they are just used internally maybe we could use a different solution ?

   // overridable translator properties
   virtual bool IsMayaTypeDag() {return false;}

   /// Instead of caching translator exports, allow a Maya node to be exported multiple times, each time generating new arnold nodes
   virtual bool DisableCaching() {return false;}
   virtual bool DependsOnOutputPlug() {return false;} // translator performs different operations depending on the type of output plug

   
   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the Update callbacks.
   virtual void RequestUpdate();

   //------------------ Static functions, should we add more and more static functions 
   //------------------ or should they all stay in ArnoldSession
   static void NodeInitializer(CAbTranslator context);
   static void ExportUserAttributes(AtNode* anode, MObject object, CNodeTranslator* translator = 0);


   // --------------------SetUpdateMode : It would be better to make it protected, but for now it's not possible
   enum UpdateMode {
      AI_UPDATE_ONLY=0,
      AI_RECREATE_NODE = 1,
      AI_RECREATE_TRANSLATOR,
      AI_DELETE_NODE   
   };
   void SetUpdateMode(UpdateMode m);

protected:
   CNodeTranslator();

   // Function to be defined if variables need to be initialized based on maya node
   // (called just before CreateArnoldNodes)
   virtual void Init() {}

   // entry point to create the Arnold node(s)
   virtual AtNode* CreateArnoldNodes() = 0;

   // If multiple Arnold nodes are created by this translator, they should be registered with this function
   AtNode* AddArnoldNode(const char* type, const char* tag="");

   // entry point to convert the Maya node to Arnold
   virtual void Export(AtNode* atNode);

   // Determine if this node Requires Motion Blur export
   virtual bool RequiresMotionData() {return false;}

   // define what happens when exporting the motion steps (GetMotionStep() > 0)
   // will only be called if RequiresMotionData returns true
   virtual void ExportMotion(AtNode* atNode) {};

   // to be defined in order to control what happens when a given plug is modified
   virtual void NodeChanged(MObject& node, MPlug& plug); 

   // Overide this if you have some special callbacks to install.
   virtual void AddUpdateCallbacks();

   // Every callback used in AddUpdateCallbacks should be register using this function.
   // so that it's removed later
   void RegisterUpdateCallback(const MCallbackId id);

   // Convert the given Arnold attribute from the Maya object. By default, no maya attribute name is provided and it is searched by default
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, MString mayaAttrName  = "");
   // Convert the given Arnold attribute from the Maya Plug
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MPlug& plug);

   // Convert the given array attribute. By default, no param Type is provided so it's determined automatically. A child array MObject can be provided optionally
   void ProcessArrayParameter(AtNode* arnoldNode, const char* arnoldParamName, const MPlug& plug, unsigned int arnoldParamType = AI_TYPE_UNDEFINED, MObject *childArray = NULL);

   // Export (and eventually create) the arnold node connected to a given attribute.
   // Note that this is what causes the creation of a full shading tree, from the root shaders to the leafs
   AtNode* ExportConnectedNode(const MPlug& outputPlug);


   // -------------- What's below isn't done yet : Still to be checked which ones are needed in the public API   
  
   
   virtual void ComputeAOVs();
   
   /// Return false if the passed outputAttribute is invalid
   virtual bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug);

   // Delete the Arnold node(s) for this translator.
   virtual void Delete();

   
   
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
   
   virtual void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

   // Some simple callbacks used by many translators.
   static void NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData);
   static void NameChangedCallback(MObject& node, const MString& str, void* clientData);
   static void NodeAboutToBeDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData);
   //static void NodeDestroyedCallback(void* clientData);
   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix, const CArnoldSession* arnoldSession = 0);
   
protected:
   
   // would be better to make it private, but for now we're just trying to move there 
   // what is not necessary for the API
   CNodeTranslatorImpl * m_impl;

};
