#pragma once

#include "NodeTranslator.h"

// Abstract base class for Dag node translators
//
typedef std::map<MObjectHandle, MDagPath, MObjectCompare> ObjectHandleToDagMap;

class DLLEXPORT CDagTranslator : public CNodeTranslator
{

public:
   virtual void Init();

   virtual MDagPath GetMayaDagPath() const { return m_dagPath; }
   virtual MString GetMayaPartialPathName() const { return m_dagPath.partialPathName(); }

   virtual void AddUpdateCallbacks();
   // for initializer callbacks:
   static void MakeMayaVisibilityFlags(CBaseAttrHelper& helper);
   // for initializer callbacks:
   static void MakeArnoldVisibilityFlags(CBaseAttrHelper& helper);

protected:
   CDagTranslator() : CNodeTranslator(){}
   virtual void Export(AtNode* atNode);
   virtual void ExportMotion(AtNode* atNode);
   
   virtual bool IsMasterInstance();
   virtual bool DoIsMasterInstance(const MDagPath& dagPath, MDagPath &masterDag);
   virtual MDagPath& GetMasterInstance();

   void GetRotationMatrix(AtMatrix& matrix);
   static void GetMatrix(AtMatrix& matrix, const MDagPath& path);
   virtual void GetMatrix(AtMatrix& matrix);
   void ExportMatrix(AtNode* node);
   // for computing a path different from m_dagPath
   AtByte ComputeVisibility(const MDagPath& path);
   AtByte ComputeVisibility();   
   
   void AddHierarchyCallbacks(const MDagPath & path);
   void SetArnoldNodeName(AtNode* arnoldNode, const char* tag="");

protected:
   MDagPath m_dagPath;
private:
   MDagPath m_masterDag;
   bool m_isMasterDag;

   // internal use only, don't override it
   virtual void CreateImplementation();
};
