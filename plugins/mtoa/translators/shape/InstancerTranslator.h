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

protected:
   MFnParticleSystem m_fnParticleSystem;
   int m_particleCount;
   MString m_customAttrs;


   std::map<std::string, MVectorArray > m_out_customVectorAttrArrays;
   std::map<std::string, MDoubleArray > m_out_customDoubleAttrArrays;
   std::map<std::string, MIntArray > m_out_customIntAttrArrays;

   std::map<std::string, MVectorArray > m_instant_customVectorAttrArrays;
   std::map<std::string, MDoubleArray > m_instant_customDoubleAttrArrays;
   std::map<std::string, MIntArray > m_instant_customIntAttrArrays;



   MVectorArray m_instantVeloArray;
   std::vector< AtArray* > m_vec_matrixArrays;
   MStringArray m_objectNames;
   MDagPathArray m_objectDagPaths;
   std::map<int, int>  m_particleIDMap;
   std::map<int, MIntArray> m_particlePathsMap;
   //MStringArray m_instanceTags; // for debug purposes
   std::vector<bool> m_exportedSteps;

};
