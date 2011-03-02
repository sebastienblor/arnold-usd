#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

#include "platform/Platform.h"
#include "MayaScene.h"
#include "render/RenderSwatch.h"
#include "utils/AttrHelper.h"

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
typedef void     (*NodeInitFunction)(MObject&);
typedef void     (*NodeClassInitFunction)(MString);

class CMayaScene;

// Abstract base class for all Maya-to-Arnold node translators
//
class DLLEXPORT CNodeTranslator
{
   // protect this class from its subclasses: make methods that should not be
   // called by subclasses private
   friend class CMayaScene;
   friend class CRenderSwatchGenerator;

private:
   AtNode* DoExport(AtUInt step);
   AtNode* DoUpdate(AtUInt step);
   void DoCreateArnoldNode();

public:
   virtual ~CNodeTranslator()
   {}
   virtual void Init(const MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      m_object = object;
      m_fnNode.setObject(object);
      m_scene = scene;
      m_outputAttr = outputAttr;
      DoCreateArnoldNode();
   }
   virtual const char* GetArnoldNodeType() = 0;

protected:
   CNodeTranslator() {}
   virtual void Export(AtNode* atNode) = 0;
   virtual void ExportMotion(AtNode* atNode, AtUInt step){}
   // Update runs during IPR for step==0 (calls Export by default)
   virtual void Update(AtNode* atNode){Export(atNode);}
   // UpdateMotion runs during IPR for step>0 (calls ExportMotion by default)
   virtual void UpdateMotion(AtNode* atNode, AtUInt step){ExportMotion(atNode, step);}
   virtual bool RequiresMotionData()
   {
      return false;
   }
   virtual void Delete() {}
   void DoDelete();

   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix);

   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug& plug, const AtParamEntry* paramEntry, int elemen=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug &plug, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   void ExportUserAttribute(AtNode *anode);

   // get the arnold node that this translator is exporting (should only be used after all export steps are complete)
   AtNode* GetArnoldNode();
   virtual void SetArnoldNodeName(AtNode* arnoldNode);
   virtual AtNode* CreateArnoldNode();

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

private:
   AtNode* m_atNode;

protected:
   MObject m_object;
   CMayaScene* m_scene;
   MFnDependencyNode m_fnNode;
   MString m_outputAttr;

   // This stores callback IDs for the callbacks this
   // translator creates.
   MCallbackIdArray m_mayaCallbackIDs;

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
   virtual void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;
      // must call this after member initialization to ensure they are available to virtual functions like SetArnoldNodeName
      CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
   }
   virtual void Init(MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      MDagPath::getAPathTo(object, m_dagPath);
      m_fnNode.setObject(object);
      m_scene = scene;
      m_outputAttr = outputAttr;
      // must call this after member initialization to ensure they are available to virtual functions like SetArnoldNodeName
      CNodeTranslator::Init(object, scene, outputAttr);
   }
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
   void SetArnoldNodeName(AtNode* arnoldNode);

protected:
   MDagPath m_dagPath;
   MFnDagNode m_fnNode;
   static ObjectHandleToDagMap s_masterInstances;
};

class DLLEXPORT CShapeTranslator : public CDagTranslator
{
public:
   void ProcessRenderFlags(AtNode* node);
   // for initializer callbacks:
   static void MakeCommonAttributes(CBaseAttrHelper& helper);
};

#endif // NODETRANSLATOR_H
