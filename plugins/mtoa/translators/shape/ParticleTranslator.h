#ifndef PARTICLE_H
#define PARTICLE_H


#include "GeometryTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MNodeMessage.h>




class CParticleTranslator
   :   public CGeometryTranslator
{
public:
   static void* creator()
   {
      return new CParticleTranslator();
   }
   virtual AtNode* CreateArnoldNodes();


   static void NodeInitializer(CAbTranslator context);
   virtual void Update(AtNode* anode);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);
   virtual void UpdateMotion(AtNode* anode, AtUInt step);



protected:

   virtual void ExportParticleShaders(AtNode* particle);


   MObject GetNodeShadingGroup(MObject dagNode, int instanceNum);

   virtual void ExportCustomParticleData(AtNode* particle, AtUInt step);
   virtual void ExportParticleData(AtNode* particle, AtUInt step);

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportParticle(AtNode* particle, bool update);


protected:
   MFnDagNode m_DagNode;
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MString m_customAttrs;
};

#endif // PARTICLE_H
