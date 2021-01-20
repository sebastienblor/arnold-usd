#pragma once

#include "ImagerTranslator.h"


class DLLEXPORT CImagerLightMixer
   :  public CImagerTranslator
{
public:
   static void* creator()
   {
      return new CImagerLightMixer();
   }

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* atNode);

};
