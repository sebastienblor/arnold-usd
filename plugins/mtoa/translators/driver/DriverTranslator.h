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
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);

protected:
   // FIXME: not implemented until these translators are tracked
   virtual void AddUpdateCallbacks();
   virtual void RemoveUpdateCallbacks();
};


#endif // DRIVER_TRANSLATOR_H
