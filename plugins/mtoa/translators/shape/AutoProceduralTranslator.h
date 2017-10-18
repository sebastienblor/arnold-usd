#pragma once

#include "ProceduralTranslator.h"

/// A translator for auto-exporting custom Procedural Shapes.
class CAutoProceduralTranslator
   :  public CProceduralTranslator
{
public:
   static void* creator()
   {
      return new CAutoProceduralTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* node);
      
   static void NodeInitializer(CAbTranslator context)
         {CProceduralTranslator::NodeInitializer(context);}
   
};
