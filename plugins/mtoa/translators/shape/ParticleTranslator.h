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
      m_inheritCacheTxfm(false)


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
   virtual void ExportParticleData(AtNode* particle, AtUInt step);

   AtNode* ExportInstance(AtNode* instance, const MDagPath& masterInstance);
   AtNode* ExportParticle(AtNode* particle, bool update);


protected:

   enum { m_renderTypeCloud,           // 0
          m_renderTypeTube,            // 1
          m_renderTypeBlobbySurface,   // 2
          m_renderTypeMultiPoint,      // 3
          m_renderTypeMultiStreak,     // 4
          m_renderTypeNumeric,         // 5
          m_renderTypePoint,           // 6
          m_renderTypeSphere,          // 7
          m_renderTypeSprite,          // 8
          m_renderTypeStreak           // 9
   };

   // these hold  each frame steps values
   std::vector< MVectorArray* >  out_positionArrays;
   std::vector< MVectorArray* >  out_colorArrays;
   std::vector< MDoubleArray* >  out_opacityArrays;
   std::vector< MDoubleArray* >  out_radiusArrays;
   std::vector< MDoubleArray* >  out_spriteScaleXArrays;
   std::vector< MDoubleArray* >  out_spriteScaleYArrays;

   // these hold each frame steps values  per map entry  for  custom attrs
   std::map<std::string,  std::vector< MVectorArray* > > out_customVectorAttrArrays;
   std::map<std::string,  std::vector< MDoubleArray* > > out_customDoubleAttrArrays;
   std::map<std::string,  std::vector< MIntArray*    > > out_customIntAttrArrays;


   // this is the main  ID->lookup map  we use to keep track of  the  particle id to  all the vectors of arrays
   std::map<int, int>  particleIDMap;

   MVectorArray instantVeloArray;
   MFnDagNode m_DagNode;
   MFnParticleSystem m_fnParticleSystem;
   AtInt m_particleCount;
   MString m_customAttrs;

   // FIXME: all these must start with "m_"
   // these are all checked  once per  export  on the 0-th step
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

};

#endif // PARTICLE_H
