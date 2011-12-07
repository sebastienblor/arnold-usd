#ifndef PARTICLE_H
#define PARTICLE_H


#include "GeometryTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimer.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <vector>
#include <map>



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

   // these hold  each frame steps values
   std::vector< MVectorArray > out_positionArrays;
   std::vector< MVectorArray > out_colorArrays;
   std::vector< MDoubleArray >  out_opacityArrays;
   std::vector< MDoubleArray >  out_radiusArrays;
   std::vector< MDoubleArray >  out_spriteScaleXArrays;
   std::vector< MDoubleArray >  out_spriteScaleYArrays;

   // these hold  each frame steps values  per map entry  for  custom attrs
   std::map<std::string,  std::vector<MVectorArray > > out_customVectorAttrArrays;
   std::map<std::string,  std::vector<MDoubleArray > > out_customDoubleAttrArrays;
   std::map<std::string,  std::vector<MIntArray > > out_customIntAttrArrays;

   // these hold  the current frame steps values per map entry  for looping to update the  out_custom*  vectors
   std::map<std::string, MVectorArray > instant_customVectorAttrArrays;
   std::map<std::string, MDoubleArray > instant_customDoubleAttrArrays;
   std::map<std::string, MIntArray > instant_customIntAttrArrays;

   // this is the main  ID->lookup map  we use to keep track of  the  particle id to  all the vectors of arrays
   std::map<int, int>  particleIDMap;

   MVectorArray instantVeloArray;
   MIntArray  out_idArray;
   MFnDagNode m_DagNode;
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MString m_customAttrs;

   // FIXME: all these must start with "m_"
   // these are all checked  once per  export  on the 0-th step
   bool hasRGB;
   bool hasOpacity;
   bool hasRadiusPP;
   bool isOpaque;
   bool doMultiPoint;
   int multiCount;
   float multiRadius;
   bool isSpritePP;
   bool isSprite;
   float particleSize;
   float radius;
   float pointSize;
   float lineWidth;
   float spriteScaleX;
   float spriteScaleY;
   bool doExtraAttributes;
   bool deleteDeadParticles;

};

#endif // PARTICLE_H
