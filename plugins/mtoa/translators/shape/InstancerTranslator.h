#ifndef INSTANCER_H
#define INSTANCER_H

#include "GeometryTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MFnInstancer.h>
#include <maya/MFnInstancer.h>
#include <maya/MDagPathArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MIntArray.h>
#include <maya/MTime.h>
#include <maya/MBoundingBox.h>


class CInstancerTranslator
   :   public CGeometryTranslator
{
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

   virtual void ExportInstancer(AtNode* instancer, bool update);
   virtual void ExportInstances(AtNode* instancer, AtUInt step);


protected:
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MDagPath m_masterDag;
   MString m_customAttrs;
};

#endif // INSTANCER_H
