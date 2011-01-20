#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

#include "platform/Platform.h"
#include "MayaScene.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCallbackIdArray.h>

class CMayaScene;

// Abstract base class for all Maya-to-Arnold node translators
//
class DLLEXPORT CNodeTranslator
{
public:
   virtual ~CNodeTranslator()
   {}
   static void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix);
   AtNode* DoExport(AtUInt step);
   AtNode* DoUpdate(AtUInt step);
   void DoDelete();
   
   // Overide this if you have some special callbacks to install.
   virtual void AddCallbacks();
   // Remove callbacks installed. This is virtual incase
   // a translator needs to do more than remove the managed
   // callbacks.
   virtual void RemoveCallbacks();
   virtual void Init(const MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      m_object = object;
      m_fnNode.setObject(object);
      m_scene = scene;
      m_outputAttr = outputAttr;
      m_atNode = NULL;
   }

protected:
   CNodeTranslator() {}
   virtual AtNode* Export() = 0;
   virtual void ExportMotion(AtNode* atNode, AtUInt step){};
   virtual void Update(AtNode* atNode) = 0;
   virtual void Delete() {}
   virtual void UpdateMotion(AtNode* atNode, AtUInt step){ExportMotion(atNode, step);};
   virtual bool RequiresMotionData()
   {
      return false;
   }
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const AtParamEntry* paramEntry, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* attrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, const char* mayaAttrib, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug& plug, const AtParamEntry* paramEntry, int elemen=-1);
   AtNode* ProcessParameter(AtNode* arnoldNode, MPlug &plug, const char* arnoldAttrib, int arnoldAttribType, int element=-1);
   void ExportDynamicFloatParameter(AtNode* arnoldNode, const char* paramName);
   void ExportDynamicBooleanParameter(AtNode* arnoldNode, const char* paramName);
   void ExportDynamicIntParameter(AtNode* arnoldNode, const char* paramName);
   void ExportUserAttribute(AtNode *anode);

   // Add a callback to the list to manage.
   void ManageCallback( const MCallbackId id );

   // Some simple callbacks used by many translators.
   static void NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData);
   static void NameChangedCallback(MObject &node, const MString &str, void *clientData);
   static void NodeDeletedCallback(MObject &node, MDGModifier &modifier, void *clientData);

protected:
   MObject m_object;
   CMayaScene* m_scene;
   MFnDependencyNode m_fnNode;
   MString m_outputAttr;
   AtNode* m_atNode;

   // This stores callback IDs for the callbacks this
   // translator creates.
   MCallbackIdArray m_mayaCallbackIDs;

   // This is a help that tells mtoa to re-export/update the node passed in.
   // Used by the IPR callbacks.
   static void UpdateIPR( void * clientData );
   
};

// Abstract base class for Dag node translators
//
typedef std::map<MObjectHandle, MDagPath, mobjcompare> ObjectHandleToDagMap;

class DLLEXPORT CDagTranslator : public CNodeTranslator
{
public:
   virtual void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;
   }
   virtual void Init(MObject& object, CMayaScene* scene, MString outputAttr="")
   {
      CNodeTranslator::Init(object, scene, outputAttr);
      MDagPath::getAPathTo(object, m_dagPath);
      m_fnNode.setObject(object);
      m_scene = scene;
      m_outputAttr = outputAttr;
   }
   static int GetMasterInstanceNumber(MObject node);
   virtual void AddCallbacks();

protected:
   CDagTranslator() : CNodeTranslator(){}
   bool IsMasterInstance(MDagPath &masterDag);
   void GetMatrix(AtMatrix& matrix);
   void ExportMatrix(AtNode* node, AtUInt step);
   AtInt ComputeVisibility(bool mayaStyleAttrs=false);
   static void AddVisibilityAttrs(MObject& node);
   virtual void Delete();

   void AddHierarchyCallbacks(const MDagPath & path);
protected:
   MDagPath m_dagPath;
   MFnDagNode m_fnNode;
   static ObjectHandleToDagMap s_masterInstances;
};

class DLLEXPORT CAutoTranslator : public CNodeTranslator
{
public:
   CAutoTranslator() :
      CNodeTranslator()
   {}
   static void* creator()
   {
      return new CAutoTranslator();
   }
   AtNode* Export();
   void Update(AtNode* atNode);
private:
   const AtNodeEntry* m_nodeEntry;
};

#endif // NODETRANSLATOR_H
