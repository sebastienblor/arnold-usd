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
   // shading engines are the root of tracking, so they ignore these calls and start their own
   virtual void TrackAOVs(AOVSet* aovs) {};
   virtual void TrackShaders(AtNodeSet* nodes) {};
   void ComputeAOVs();

protected:
   CShadingEngineTranslator() :
      CNodeTranslator()
   {
      m_shaders = new AtNodeSet;
   }
   virtual ~CShadingEngineTranslator()
   {
      delete m_shaders;
   }
   virtual void NodeChanged(MObject& node, MPlug& plug);
   
   MPlugArray m_customAOVPlugs;
};
