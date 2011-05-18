#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

#include "platform/Platform.h"
#include "scene/MayaScene.h"
#include "render/RenderSwatch.h"
#include "utils/AttrHelper.h"
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
   virtual AtNode* Init(const MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      m_object = object;
      m_fnNode.setObject(object);
      m_scene = scene;
      m_outputAttr = outputAttr;
      return DoCreateArnoldNodes();
   }
   virtual MFnDependencyNode GetFnNode() const {return m_fnNode;}
   MString GetTranslatorName() {return m_abstract.name;}
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
   AtFloat GetCurrentFrame() {return m_scene->GetCurrentFrame();}
   bool IsMotionBlurEnabled() const {return m_scene->IsMotionBlurEnabled();}
   bool IsCameraMotionBlurEnabled() const {return m_scene->IsCameraMotionBlurEnabled();}
   bool IsObjectMotionBlurEnabled() const
   {
      bool local_motion_attr(true);
      MPlug plug = GetFnNode().findPlug("motionBlur");
      if (!plug.isNull())
         local_motion_attr = plug.asBool();
      return m_scene->IsObjectMotionBlurEnabled() && local_motion_attr;
   }
   bool IsDeformMotionBlurEnabled() const {return m_scene->IsDeformMotionBlurEnabled();}
   bool IsLightMotionBlurEnabled() const {return m_scene->IsLightMotionBlurEnabled();}
   AtUInt GetNumMotionSteps() const {return m_scene->GetNumMotionSteps();}
   AtFloat GetShutterSize() const {return m_scene->GetShutterSize();}
   AtUInt GetShutterType(){return m_scene->GetShutterType();}
   ExportMode GetExportMode() {return m_scene->GetExportMode();}

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

   AtNode* m_atNode;
   std::map<std::string, AtNode*> m_atNodes;
   MObject m_object;
   CMayaScene* m_scene;
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
typedef std::map<MObjectHandle, MDagPath, mobjcompare> ObjectHandleToDagMap;

class DLLEXPORT CDagTranslator : public CNodeTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_dagPath = dagPath;
      m_fnDagNode.setObject(dagPath);
      // must call this after member initialization to ensure they are available to virtual functions like SetArnoldNodeName
      return CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
   }

   virtual AtNode* Init(MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      MDagPath dagPath;
      MDagPath::getAPathTo(object, dagPath);
      return Init(dagPath, scene, outputAttr);
   }

   virtual MFnDagNode GetFnDagNode() const {return m_fnDagNode;}
   virtual bool IsDag() {return true;}

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
   void GetMatrix(AtMatrix& matrix);
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
