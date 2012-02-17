#ifndef SHADING_ENGINE_TRANSLATOR_H
#define SHADING_ENGINE_TRANSLATOR_H

#include "ShaderTranslator.h"

#include "maya/MPlugArray.h"

class DLLEXPORT CShadingEngineTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CShadingEngineTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);
};


#endif
