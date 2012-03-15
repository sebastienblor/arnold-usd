#ifndef OBJECT_SET_TRANSLATOR_H
#define OBJECT_SET_TRANSLATOR_H

#include "NodeTranslator.h"


class DLLEXPORT CObjectSetTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CObjectSetTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);

protected:
   CObjectSetTranslator() :
      CNodeTranslator()
   {}
   virtual ~CObjectSetTranslator()
   {}

};


#endif // OBJECT_SET_TRANSLATOR_H
