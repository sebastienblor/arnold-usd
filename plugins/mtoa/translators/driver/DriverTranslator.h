#ifndef DRIVER_TRANSLATOR_H
#define DRIVER_TRANSLATOR_H

#include "translators/NodeTranslator.h"



//--------------- DriverTranslator ------------------------------------------


class DLLEXPORT CDriverTranslator
   :  public CNodeTranslator
{
public:
   static void* creator()
   {
      return new CDriverTranslator();
   }
   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);
};


#endif // DRIVER_TRANSLATOR_H
