#pragma once

#include "StandinsTranslator.h"

class CMeshProceduralTranslator : public CArnoldStandInsTranslator
{
public:

   static void* creator()
   {
      return new CMeshProceduralTranslator();
   }
   static void NodeInitializer(CAbTranslator context);
};
