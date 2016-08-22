#pragma once

#include "ShaderTranslator.h"

#include "maya/MPlugArray.h"

class DLLEXPORT CShadingEngineTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CShadingEngineTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);

   void ComputeAOVs();

protected:
   CShadingEngineTranslator();
   virtual ~CShadingEngineTranslator();
   virtual void NodeChanged(MObject& node, MPlug& plug);
   
   MPlugArray m_customAOVPlugs;
};
