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
   {}
   
   static void* creator()
   {
      return new CNParticleTranslator();
   }

   static void NodeInitializer(CAbTranslator context);

   virtual bool IsCached();
   virtual bool IsNParticle();
};
