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
   CParticleTranslator()
      :m_hasRGB(false),
      m_hasOpacity(false),
      m_hasRadiusPP(false),
      m_isOpaque(false),
      m_doMultiPoint(false),
      m_multiCount(1),
      m_multiRadius(0),
      m_isSpritePP(false),
      m_isSprite(false),
      m_particleSize(1),
      m_radius(.5),
      m_pointSize(1),
      m_lineWidth(1),
      m_spriteScaleX(1),
      m_spriteScaleY(1),
      m_doExtraAttributes(false),
      m_inheritCacheTxfm(false),
      m_exportId(false)


   {}


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
   virtual void ExportCustomParticleData(AtNode* particle, AtUInt step);
   virtual void ExportPreambleData(AtNode* particle);
   virtual void GatherFirstStep(AtNode* particle);
   virtual void GatherBlurSteps(AtNode* particle, AtUInt step);
   virtual void ComputeBlurSteps(AtNode* particle, AtUInt step);
   virtual void WriteOutParticle(AtNode* particle);

   virtual void GatherStandardPPData(MVectorArray*   positionArray ,
                                     MDoubleArray*   radiusArray ,
                                     MDoubleArray*   spriteScaleXPP ,
                                     MDoubleArray*   spriteScaleYPP ,
                                     MVectorArray*   rgbArray ,
                                     MDoubleArray*   opacityArray,
                                     MVectorArray    &velocityArray,
                                     MIntArray        &particleId);

   AtNode* ExportInstance(AtNode* instance, const MDagPath& masterInstance);
   //AtNode* ExportParticle(AtNode* particle, bool update, AtUInt step);
   AtNode* ExportParticleNode(AtNode* particle, AtUInt step);


protected:

   // by solid angle's conventions m_renderTypeSphere should be formatted like:  PARTICLE_TYPE_SPHERE
   enum ParticleRenderType
   {
          PARTICLE_TYPE_CLOUD,           // 0
          PARTICLE_TYPE_TUBE,            // 1
          PARTICLE_TYPE_BLOBBYSURFACE,   // 2
          PARTICLE_TYPE_MULTIPOINT,      // 3
          PARTICLE_TYPE_MULTISTREAK,     // 4
          PARTICLE_TYPE_NUMERIC,         // 5
          PARTICLE_TYPE_POINT,           // 6
          PARTICLE_TYPE_SPHERE,          // 7
          PARTICLE_TYPE_SPRITE,          // 8
          PARTICLE_TYPE_STREAK           // 9
   };

   // these hold  each frame steps values
   std::vector< MVectorArray* >  m_out_positionArrays;
   std::vector< MVectorArray* >  m_out_colorArrays;
   std::vector< MDoubleArray* >  m_out_opacityArrays;
   std::vector< MDoubleArray* >  m_out_radiusArrays;
   std::vector< MDoubleArray* >  m_out_spriteScaleXArrays;
   std::vector< MDoubleArray* >  m_out_spriteScaleYArrays;


   // these hold each frame steps values  per map entry  for  custom attrs
   std::map<std::string,  std::vector< MVectorArray* > > m_out_customVectorAttrArrays;
   std::map<std::string,  std::vector< MDoubleArray* > > m_out_customDoubleAttrArrays;
   std::map<std::string,  std::vector< MIntArray*    > > m_out_customIntAttrArrays;


   // this is the main  ID->lookup map  we use to keep track of  the  particle id to  all the vectors of arrays
   std::map<int, int>  m_particleIDMap;


   MVectorArray m_instantVeloArray;
   MFnDagNode m_DagNode;
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MString m_customAttrs;


   bool  m_hasRGB;
   bool  m_hasOpacity;
   bool  m_hasRadiusPP;
   bool  m_isOpaque;
   bool  m_doMultiPoint;
   int   m_multiCount;
   float m_multiRadius;
   bool  m_isSpritePP;
   bool  m_isSprite;
   float m_particleSize;
   float m_radius;
   float m_pointSize;
   float m_lineWidth;
   float m_spriteScaleX;
   float m_spriteScaleY;
   bool  m_doExtraAttributes;
   bool  m_deleteDeadParticles;
   bool  m_inheritCacheTxfm;
   bool  m_exportId;

};

#endif // PARTICLE_H
