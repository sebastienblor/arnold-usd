#pragma once

#include "ParticleTranslator.h"

#include <maya/MFnParticleSystem.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimer.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>

#include <vector>
#include <map>



class CNParticleTranslator
   :   public CParticleTranslator
{
public:
   CNParticleTranslator() :
      CParticleTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "points";
   }

   static void* creator()
   {
      return new CNParticleTranslator();
   }

   static void NodeInitializer(CAbTranslator context);

   virtual bool IsCached();
};
