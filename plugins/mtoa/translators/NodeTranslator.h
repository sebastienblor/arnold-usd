#pragma once


#include "platform/Platform.h"
#include "attributes/AttrHelper.h"
#include "session/SessionOptions.h"
#include "extension/AbTranslator.h"

#include <ai_nodes.h>
#include <ai_ray.h>
#include <ai_universe.h>

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

class CNodeTranslatorImpl;
class CArnoldSession;
struct CSessionOptions;

// Abstract base class for all Maya-to-Arnold node translators
//
class DLLEXPORT CNodeTranslator
{
   friend class CArnoldSession;
   friend class CExtensionsManager;
   friend class CExtension;
   friend class CExtensionImpl;
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

   // Return the corresponding Arnold Node.
   // In case multiple Arnold nodes are registered with this translator, returns the one for a given tag
   AtNode* GetArnoldNode(const char* tag=NULL);

   // Returns true if this translator has already been exported
   bool IsExported() const;

   // Get the the Maya plug for this given attribute name
   MPlug FindMayaPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;


   //---- The 4 functions below are mainly used for debug logs. Do we want to keep them in the public API ?
   
   // Get the name of the Maya object. 
   MString GetMayaNodeName() const;
   // Get the name of the Arnold node
   const char* GetArnoldNodeName(const char* tag="");
   // Get the type of the Arnold node
   const char* GetArnoldTypeName(const char* tag="");
   // Get the name of this translator
   MString GetTranslatorName();

   // If the translator needs to performs different operations depending on the type of output plug,
   // it will need to override this methid and return true
   virtual bool DependsOnOutputPlug() {return false;} 
   
   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the Update callbacks.
   virtual void RequestUpdate();

   enum UpdateMode {
      AI_UPDATE_ONLY=0,
      AI_RECREATE_NODE = 1,
      AI_RECREATE_TRANSLATOR,
      AI_DELETE_NODE   
   };
   void SetUpdateMode(UpdateMode m);

   // Export the MObject user attributes to the given Arnold node. This is useful when the target AtNode
   // is different than GetArnoldNode()
   static void ExportUserAttributes(AtNode* anode, MObject object, CNodeTranslator* translator = 0);

   // Static functions to get the MtoA Translator associated to a maya node
   static CNodeTranslator *GetTranslator(const MDagPath &dagPath);
   static CNodeTranslator *GetTranslator(const MObject &object);

   static void RequestLightLinksUpdate();
   static void RequestTxUpdate();

   static void NodeInitializer(CAbTranslator context);
   
protected:
   CNodeTranslator();

   // Function to be defined if variables need to be initialized based on maya node
   // (called just before CreateArnoldNodes)
   virtual void Init() {}

   // entry point to create the Arnold node(s)
   virtual AtNode* CreateArnoldNodes() = 0;

   // If multiple Arnold nodes are created by this translator, they should be registered with this function
   AtNode* AddArnoldNode(const char* type, const char* tag=NULL);

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

   // Returns the value of the "motionBlur" flag on this Maya object
   bool IsLocalMotionBlurEnabled() const;
   
   // Delete the Arnold node(s) for this translator.
   virtual void Delete();

// -------- Static functions

   // Some simple callbacks used by several translators.
   // They might be useful in case AddUpdateCallbacks is re-defined without calling the parent function
   static void NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData);
   static void NameChangedCallback(MObject& node, const MString& str, void* clientData);
   static void NodeAboutToBeDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData);

   // Convert a matrix from Maya to Arnold data
   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix);
   static double GetExportFrame();
   static unsigned int GetMotionStep();
   static unsigned int GetNumMotionSteps();
   static bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ANY);
   static const CSessionOptions& GetSessionOptions();
   static ArnoldSessionMode GetSessionMode();
   static const MObject& GetArnoldRenderOptions();
   static double GetMotionByFrame();
   static const double *GetMotionFrames(unsigned int &count);

protected:
   
   // internal use only
   CNodeTranslatorImpl * m_impl;
private:
   // internal use only, don't override it
   virtual void CreateImplementation();

};
