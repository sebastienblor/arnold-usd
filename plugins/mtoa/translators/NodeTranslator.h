#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "attributes/AttrHelper.h"
#include "scene/MayaScene.h"
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
   friend class CRenderSession;
   friend class CMayaScene;
   friend class CExtensionsManager;
   friend class CExtension;
   friend class CRenderSwatchGenerator;

private:
   AtNode* DoExport(AtUInt step);
   AtNode* DoUpdate(AtUInt step);
   AtNode* DoCreateArnoldNodes();
   void SetTranslatorName(MString name) {m_abstract.name = MString(name);}

public:
   virtual ~CNodeTranslator()
   {}
   virtual AtNode* Init(CMayaScene* scene, const MObject& object, MString outputAttr="")
   {
      m_scene = scene;
      m_object = object;
      m_fnNode.setObject(object);
      m_outputAttr = outputAttr;
      return DoCreateArnoldNodes();
   }
   virtual MFnDependencyNode GetFnNode() const {return m_fnNode;}
   MString GetName() {return m_abstract.name;}
   virtual bool IsDag() {return false;}

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

   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix);

   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug& plug, const AtParamEntry* paramEntry, int elemen=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug &plug, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   void ExportUserAttribute(AtNode *anode);

   // scene info
   AtNode* ExportShader(MObject mayaShader, const MString &attrName="") { return m_scene->ExportShader(mayaShader, attrName);}
   AtNode* ExportShader(MPlug& shaderOutputPlug) {return m_scene->ExportShader(shaderOutputPlug);}
   AtNode* ExportDagPath(MDagPath &dagPath) {return m_scene->ExportDagPath(dagPath);}

   inline AtFloat GetCurrentFrame() const {return m_scene->GetCurrentFrame();}
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_scene->IsMotionBlurEnabled(type); }
   bool IsLocalMotionBlurEnabled() const
   {
      bool local_motion_attr(true);
      MPlug plug = GetFnNode().findPlug("motionBlur");
      if (!plug.isNull())
         local_motion_attr = plug.asBool();
      return local_motion_attr;
   }
   inline AtUInt GetNumMotionSteps() const {return m_scene->GetNumMotionSteps();}
   inline AtFloat GetShutterSize() const {return m_scene->GetShutterSize();}
   inline AtUInt GetShutterType() const {return m_scene->GetShutterType();}
   inline ExportMode GetExportMode()  const {return m_scene->GetExportMode();}

   // get the arnold node that this translator is exporting (should only be used after all export steps are complete)
   AtNode* GetArnoldRootNode();
   AtNode* GetArnoldNode(const char* tag);
   AtNode* AddArnoldNode(const char* type, const char* tag="");
   virtual void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

   // Add a callback to the list to manage.
   void ManageIPRCallback(const MCallbackId id);

   // Overide this if you have some special callbacks to install.
   virtual void AddIPRCallbacks();
   // Remove callbacks installed. This is virtual incase
   // a translator needs to do more than remove the managed
   // callbacks.
   virtual void RemoveIPRCallbacks();

   // Some simple callbacks used by many translators.
   static void NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData);
   static void NameChangedCallback(MObject &node, const MString &str, void *clientData);
   static void NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData);

protected:
   CAbTranslator m_abstract;

   CMayaScene* m_scene;

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

   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the IPR callbacks.
   static void UpdateIPR(void * clientData);

   static MPlug FindPlug(MFnDependencyNode& node, const std::string& param);
   static MPlug GetPlugElement(MFnDependencyNode& node, MPlug& plug, const std::string& attr);
   
};

// Abstract base class for Dag node translators
//
typedef std::map<MObjectHandle, MDagPath, MObjectCompare> ObjectHandleToDagMap;

class DLLEXPORT CDagTranslator : public CNodeTranslator
{

public:
   virtual AtNode* Init(CMayaScene* scene, MDagPath& dagPath, MString outputAttr="")
   {
      m_scene = scene;
      m_dagPath = dagPath;
      m_fnDagNode.setObject(dagPath);
      // must call this after member initialization to ensure they are available to virtual functions like SetArnoldNodeName
      AtNode * tmpRet = CNodeTranslator::Init(scene, dagPath.node(), outputAttr);
      return tmpRet;
   }

   virtual AtNode* Init(CMayaScene* scene, MObject& object, MString outputAttr="")
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(object, dagPath);
      return Init(scene, dagPath, outputAttr);
   }

   virtual MFnDagNode GetFnDagNode() const {return m_fnDagNode;}
   virtual bool IsDag() {return true;}
   virtual bool IsRenderable() {return true;}

   static int GetMasterInstanceNumber(MObject node);
   virtual void AddIPRCallbacks();
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
