#pragma once

#include "ShapeTranslator.h"
#include "common/UnorderedContainer.h"
#include <maya/MFnParticleSystem.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimer.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>

#include <vector>
#include <map>



class CParticleTranslator
   :   public CShapeTranslator
{
public:
   CParticleTranslator() :
      CShapeTranslator(),
      m_hasRGB(false),
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
      m_exportId(false),
      m_minPixelWidth(0.0f)
   {}

   
   static void* creator()
   {
      return new CParticleTranslator();
   }
   virtual AtNode* CreateArnoldNodes();


   static void NodeInitializer(CAbTranslator context);
   virtual void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode);

   // We need to override getMatrix, 
   // so that instances return the expected result (#3281)
   virtual void GetMatrix(AtMatrix& matrix);

protected:

   virtual void ExportParticleShaders(AtNode* particle);
   virtual void ExportCustomParticleData(AtNode* particle);
   virtual void ExportPreambleData(AtNode* particle);
   virtual void GatherFirstStep(AtNode* particle);
   virtual void GatherBlurSteps(AtNode* particle, unsigned int step);
   virtual void InterpolateBlurSteps(AtNode* particle, unsigned int step);
   virtual void WriteOutParticle(AtNode* particle);
   virtual bool IsCached();
   virtual bool IsNParticle();

   virtual void GatherStandardPPData(MTime           curTime,
                                     MVectorArray*   positionArray ,
                                     MDoubleArray*   radiusArray ,
                                     MDoubleArray*   spriteScaleXPP ,
                                     MDoubleArray*   spriteScaleYPP ,
                                     MVectorArray*   rgbArray ,
                                     MDoubleArray*   opacityArray,
                                     MVectorArray    &velocityArray,
                                     MVectorArray    &accelerationArray,
                                     MIntArray        &particleId);

   AtNode* ExportInstance(AtNode* instance, const MDagPath& masterInstance);
   //AtNode* ExportParticle(AtNode* particle, bool update, unsigned int step);
   AtNode* ExportParticleNode(AtNode* particle, unsigned int step);


protected:

   virtual void RequestUpdate();

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
   std::vector< MDoubleArray* >  m_out_radiusArrays;
   std::vector< MDoubleArray* >  m_out_spriteScaleXArrays;
   std::vector< MDoubleArray* >  m_out_spriteScaleYArrays;
   MDoubleArray                  m_out_opacityArray;
   MVectorArray                  m_out_colorArray;


   // these hold each frame steps values  per map entry  for  custom attrs
   unordered_map<std::string,  MVectorArray* > m_out_customVectorAttrArrays;
   unordered_map<std::string,  MDoubleArray* > m_out_customDoubleAttrArrays;
   unordered_map<std::string,  MIntArray* > m_out_customIntAttrArrays;


   // this is the main  ID->lookup map  we use to keep track of  the  particle id to  all the vectors of arrays
   unordered_map<int, int>  m_particleIDMap;

   std::vector<bool> m_exportedSteps;
   
   MVectorArray m_instantVeloArray;
   MVectorArray m_instantAcceArray;
   MFnDagNode m_DagNode;
   MFnParticleSystem m_fnParticleSystem;
   int m_particleCount;
   MString m_customAttrs;


   bool		m_hasRGB;
   bool		m_hasOpacity;
   bool		m_hasRadiusPP;
   bool		m_isOpaque;
   bool		m_doMultiPoint;
   int		m_multiCount;
   double	m_multiRadius;
   bool		m_isSpritePP;
   bool		m_isSprite;
   double	m_particleSize;
   double	m_radius;
   double	m_pointSize;
   double	m_lineWidth;
   double	m_spriteScaleX;
   double	m_spriteScaleY;
   bool		m_doExtraAttributes;
   bool		m_deleteDeadParticles;
   bool		m_inheritCacheTxfm;
   bool		m_exportId;
   float    m_minPixelWidth;

};
