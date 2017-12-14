#pragma once

#include "ShapeTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MFnInstancer.h>
#include <maya/MDagPathArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MTime.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnArrayAttrsData.h>
#include <vector>
#include <map>

class CInstancerTranslator
   :   public CShapeTranslator
{
protected:
   CInstancerTranslator() :
      CShapeTranslator()
   {}

public:
   static void* creator()
   {
      return new CInstancerTranslator();
   }
   virtual AtNode* CreateArnoldNodes();


   static void NodeInitializer(CAbTranslator context);
   
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode);
   
protected:
   AtByte ComputeMasterVisibility(const MDagPath& masterDagPath) const;
   void ExportInstances(AtNode* instancer);
   virtual void RequestUpdate();
   virtual void PostExport(AtNode *node);

protected:
   MFnParticleSystem m_fnParticleSystem;
   int m_particleCount;
   MString m_customAttrs;


   unordered_map<std::string, MVectorArray > m_out_customVectorAttrArrays;
   unordered_map<std::string, MDoubleArray > m_out_customDoubleAttrArrays;
   unordered_map<std::string, MIntArray > m_out_customIntAttrArrays;

   unordered_map<std::string, MVectorArray > m_instant_customVectorAttrArrays;
   unordered_map<std::string, MDoubleArray > m_instant_customDoubleAttrArrays;
   unordered_map<std::string, MIntArray > m_instant_customIntAttrArrays;



   MVectorArray m_instantVeloArray;
   std::vector< AtArray* > m_vec_matrixArrays;
   MStringArray m_objectNames;
   std::vector<bool> m_cloneInstances;
   MDagPathArray m_objectDagPaths;
   unordered_map<int, int>  m_particleIDMap;
   unordered_map<int, MIntArray> m_particlePathsMap;
   //MStringArray m_instanceTags; // for debug purposes
   std::vector<bool> m_exportedSteps;

};
