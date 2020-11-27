#pragma once

#include "translators/shape/ProceduralTranslator.h"

//#include "UsdProxyShapeListener.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CUsdProxyShapeTranslatorImpl;

class CUsdProxyShapeTranslator
   :   public CProceduralTranslator
{
public:
   CUsdProxyShapeTranslator();
   virtual ~CUsdProxyShapeTranslator();

   virtual void Export(AtNode* curve);
   virtual void ExportMotion(AtNode* curve);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   virtual void AddUpdateCallbacks();
   static void timeChangedCallback(void* clientData);
   static void* creator()
   {
      return new CUsdProxyShapeTranslator();
   }
   virtual void ExportShaders();
   virtual void NodeChanged(MObject& node, MPlug& plug);

   void StageChanged();

private:
   CUsdProxyShapeTranslatorImpl *m_impl;

};
