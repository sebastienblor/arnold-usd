#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "attributes/AttrHelper.h"
#include "session/ArnoldSession.h"
#include "extension/AbTranslator.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCallbackIdArray.h>

#include <string>
#include <vector>
#include <map>

typedef void *   (*CreatorFunction)();
typedef void     (*NodeInitFunction)(MString);

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

private:
   AtNode* DoExport(AtUInt step);
   AtNode* DoUpdate(AtUInt step);
   AtNode* DoCreateArnoldNodes();
   void SetTranslatorName(MString name) {m_abstract.name = MString(name);}

public:
   MString GetTranslatorName() {return m_abstract.name;}

   virtual ~CNodeTranslator()
   {}
   virtual AtNode* Init(CArnoldSession* session, const MObject& object, MString outputAttr="")
   {
      m_session = session;
      m_object = object;
      m_fnNode.setObject(object);
      m_outputAttr = outputAttr;
      return DoCreateArnoldNodes();
   }
   virtual MObject GetMayaObject() const { return m_object; }
   virtual MString GetMayaNodeName() const { return m_fnNode.name(); }
   virtual MString GetMayaNodeTypeName() const { return m_fnNode.typeName(); }
   virtual MObject GetMayaObjectAttribute(MString attributeName) const { return m_fnNode.attribute(attributeName); }
   virtual MPlug FindMayaObjectPlug(MString attributeName) const { return m_fnNode.findPlug(attributeName); }

   virtual bool IsMayaTypeDag() {return false;}
   virtual bool IsMayaTypeRenderable() {return false;}

protected:
   CNodeTranslator()  :
      m_abstract(CAbTranslator())
   {}
   virtual void Export(AtNode* atNode) = 0;
   virtual void ExportMotion(AtNode* atNode, AtUInt step){}
   // Update runs during IPR for step==0 (calls Export by default)
   virtual void Update(AtNode* atNode){Export(atNode);}
   // UpdateMotion runs during IPR for step>0 (calls ExportMotion by default)
   virtual void UpdateMotion(AtNode* atNode, AtUInt step){ExportMotion(atNode, step);}
   virtual bool RequiresMotionData() {return false;}
   virtual AtNode* CreateArnoldNodes() = 0;
   virtual void Delete() {}
   void DoDelete();

   // Using the translator's MObject m_object and corresponding attrbuteName (default behavior)
   virtual AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, int element=-1);
   // For a specific Maya plug
   virtual AtNode* ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName, int arnoldParamType, const MPlug& plug);

   void ExportUserAttribute(AtNode *anode);

   // session info
   inline double GetExportFrame() const {return m_session->GetExportFrame();}
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_session->IsMotionBlurEnabled(type); }
   bool IsLocalMotionBlurEnabled() const
   {
      bool local_motion_attr(true);
      MPlug plug = FindMayaObjectPlug("motionBlur");
      if (!plug.isNull())
         local_motion_attr = plug.asBool();
      return local_motion_attr;
   }
   inline unsigned int GetNumMotionSteps() const {return m_session->GetNumMotionSteps();}
   inline float GetShutterSize() const {return m_session->GetShutterSize();}
   inline unsigned int GetShutterType() const {return m_session->GetShutterType();}
   inline ArnoldSessionMode GetSessionMode() const {return m_session->GetSessionMode();}

   // session action
   AtNode* ExportNode(MObject node, const MString &attrName="") { return m_session->ExportNode(node, attrName);}
   AtNode* ExportNode(MPlug& outputPlug) {return m_session->ExportNode(outputPlug);}
   AtNode* ExportDagPath(MDagPath &dagPath) {return m_session->ExportDagPath(dagPath);}

   // get the arnold node that this translator is exporting (should only be used after all export steps are complete)
   AtNode* GetArnoldRootNode();
   AtNode* GetArnoldNode(const char* tag);
   AtNode* AddArnoldNode(const char* type, const char* tag="");
   virtual void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

   // Add a callback to the list to manage.
   void ManageUpdateCallback(const MCallbackId id);

   // Overide this if you have some special callbacks to install.
   virtual void AddUpdateCallbacks();
   // Remove callbacks installed. This is virtual incase
   // a translator needs to do more than remove the managed
   // callbacks.
   virtual void RemoveUpdateCallbacks();
   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the Update callbacks.
   void RequestUpdate(void * clientData = NULL);

   // Some simple callbacks used by many translators.
   static void NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData);
   static void NameChangedCallback(MObject &node, const MString &str, void *clientData);
   static void NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData);

protected:
   CAbTranslator m_abstract;

   CArnoldSession* m_session;

   AtNode* m_atNode;
   std::map<std::string, AtNode*> m_atNodes;
   MObject m_object;
   MFnDependencyNode m_fnNode;
   MString m_outputAttr;

   // This stores callback IDs for the callbacks this
   // translator creates.
   MCallbackIdArray m_mayaCallbackIDs;

   // Manually defined translators can fill this information
   // to make debugging more explicit
   MString s_arnoldNodeName;
   
};

// Abstract base class for Dag node translators
//
typedef std::map<MObjectHandle, MDagPath, MObjectCompare> ObjectHandleToDagMap;

class DLLEXPORT CDagTranslator : public CNodeTranslator
{

public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_session = session;
      m_dagPath = dagPath;
      m_fnDagNode.setObject(dagPath);
      // must call this after member initialization to ensure they are available to virtual functions like SetArnoldNodeName
      AtNode * tmpRet = CNodeTranslator::Init(session, dagPath.node(), outputAttr);
      return tmpRet;
   }

   virtual AtNode* Init(CArnoldSession* session, MObject& object, MString outputAttr="")
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(object, dagPath);
      return Init(session, dagPath, outputAttr);
   }

   virtual MDagPath GetMayaDagPath() const { return m_dagPath; }
   virtual MString GetMayaPartialPathName() const { return m_fnDagNode.partialPathName(); }
   virtual bool IsMayaTypeDag() {return true;}
   virtual bool IsMayaTypeRenderable() {return true;}

   static int GetMasterInstanceNumber(MObject node);
   virtual void AddUpdateCallbacks();
   // for initializer callbacks:
   static void MakeMayaVisibilityFlags(CBaseAttrHelper& helper);
   // for initializer callbacks:
   static void MakeArnoldVisibilityFlags(CBaseAttrHelper& helper);

protected:
   CDagTranslator() : CNodeTranslator(){}
   bool IsMasterInstance(MDagPath &masterDag);
   void GetRotationMatrix(AtMatrix& matrix);
   virtual void GetMatrix(AtMatrix& matrix);
   void ExportMatrix(AtNode* node, AtUInt step);
   AtInt ComputeVisibility();
   virtual void Delete();
   void AddHierarchyCallbacks(const MDagPath & path);
   void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

protected:
   MDagPath m_dagPath;
   MFnDagNode m_fnDagNode;
   static ObjectHandleToDagMap s_masterInstances;
};


#endif // NODETRANSLATOR_H
