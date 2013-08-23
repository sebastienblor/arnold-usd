#pragma once

#include "ShaderTranslator.h"

#define SHADER_TRANSLATOR(name)\
   class name : public CShaderTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      virtual void Export(AtNode* shader);\
      AtNode* CreateArnoldNodes();\
   };

#define SHADER_TRANSLATOR_MULTIOUT(name)\
   class name : public CShaderTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      virtual void Export(AtNode* shader);\
      AtNode* CreateArnoldNodes();\
      bool DependsOnOutputPlug() { return true; } \
   protected:\
      bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug){resolvedOutputPlug=outputPlug;return true;}\
   };

class CSkyShaderTranslator
   :  public CDagTranslator
{
public:
   static void* creator(){return new CSkyShaderTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

SHADER_TRANSLATOR(CLambertTranslator);
SHADER_TRANSLATOR(CPlace2DTextureTranslator);
SHADER_TRANSLATOR(CBump3DTranslator);
SHADER_TRANSLATOR_MULTIOUT(CSamplerInfoTranslator);
SHADER_TRANSLATOR(CPlusMinusAverageTranslator);
SHADER_TRANSLATOR_MULTIOUT(CRemapValueTranslator);
SHADER_TRANSLATOR_MULTIOUT(CParticleSamplerInfoTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CProjectionTranslator);
SHADER_TRANSLATOR(CRampTranslator);
SHADER_TRANSLATOR(CLayeredTextureTranslator);
SHADER_TRANSLATOR(CLayeredShaderTranslator);
SHADER_TRANSLATOR(CRemapHsvTranslator);
SHADER_TRANSLATOR_MULTIOUT(CDisplacementTranslator);
SHADER_TRANSLATOR(CMayaBlinnTranslator);
SHADER_TRANSLATOR(CPhysicalSkyTranslator);
SHADER_TRANSLATOR(CMayaTripleShadingSwitchTranslator);

void DisplacementTranslatorNodeInitializer(CAbTranslator context);

class CBump2DTranslator : public CShaderTranslator
{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator(){return new CBump2DTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   bool DisableCaching() {return true;};
};

class CAnimCurveTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CAnimCurveTranslator();}
   virtual void Export(AtNode* shader);
   virtual void ExportMotion(AtNode* shader, unsigned int step);
   AtNode* CreateArnoldNodes();
};

class CFileTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CFileTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);
};

class CAiHairTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiHairTranslator();}

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
};

class CAiImageTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiImageTranslator();}

   virtual void Export(AtNode* shader);

   AtNode* CreateArnoldNodes();
};
