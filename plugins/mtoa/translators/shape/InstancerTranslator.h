#ifndef INSTANCER_H
#define INSTANCER_H

#include "GeometryTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MFnInstancer.h>
#include <maya/MDagPathArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MIntArray.h>
#include <maya/MTime.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnArrayAttrsData.h>
#include <vector>
#include <map>

class CInstancerTranslator
   :   public CGeometryTranslator
{
protected:
   CInstancerTranslator() :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "ginstance";
   }
public:
   static void* creator()
   {
      return new CInstancerTranslator();
   }
   virtual AtNode* CreateArnoldNodes();


   static void NodeInitializer(CAbTranslator context);
   virtual void Update(AtNode* anode);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);
   virtual void UpdateMotion(AtNode* anode, AtUInt step);


protected:
   int ComputeMasterVisibility(const MDagPath& masterDagPath) const;
   virtual void ExportInstancer(AtNode* instancer, bool update);
   virtual void ExportInstances(AtNode* instancer, AtUInt step);


protected:
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MDagPath m_masterDag;
   MString m_customAttrs;


   std::map<std::string, MVectorArray > m_out_customVectorAttrArrays;
   std::map<std::string, MDoubleArray > m_out_customDoubleAttrArrays;
   std::map<std::string, MIntArray > m_out_customIntAttrArrays;

   std::map<std::string, MVectorArray > m_instant_customVectorAttrArrays;
   std::map<std::string, MDoubleArray > m_instant_customDoubleAttrArrays;
   std::map<std::string, MIntArray > m_instant_customIntAttrArrays;



   MVectorArray m_instantVeloArray;
   std::vector< AtArray* > m_vec_matrixArrays;
   MIntArray  m_startIndicesArray;
   MIntArray  m_pathIndicesArray;
   MStringArray m_objectNames;
   MDagPathArray m_objectDagPaths;
   std::map<int, int>  m_particleIDMap;
   MStringArray m_instanceTags; // for debug purposes

};

#endif // INSTANCER_H
