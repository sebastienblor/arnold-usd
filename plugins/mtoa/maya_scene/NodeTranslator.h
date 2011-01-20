#ifndef OBJECT_TRANSLATOR_H
#define OBJECT_TRANSLATOR_H

#include "platform/Platform.h"
#include "MayaScene.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>

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
   virtual void ExportMotion(AtNode* atNode, AtUInt step){ExportMotion(atNode, step);};
   virtual void Update(AtNode* atNode) = 0;
   virtual void UpdateMotion(AtNode* atNode, AtUInt step){};
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

protected:
   MObject m_object;
   CMayaScene* m_scene;
   MFnDependencyNode m_fnNode;
   MString m_outputAttr;
   AtNode* m_atNode;
};

// Abstract base class for Dag node translators
//

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

protected:
   CDagTranslator() : CNodeTranslator(){}
   bool IsMasterInstance(MDagPath &masterDag);
   void GetMatrix(AtMatrix& matrix);
   void ExportMatrix(AtNode* node, AtUInt step);
   AtInt ComputeVisibility(bool mayaStyleAttrs=false);

protected:
   MDagPath m_dagPath;
   MFnDagNode m_fnNode;
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

#endif
