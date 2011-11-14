#ifndef SHADER_TRANSLATORS_H
#define SHADER_TRANSLATORS_H

#include "ShaderTranslator.h"

// we use ShaderTranslator here for its specialized ResolveOutputPlug method
#define SHADER_TRANSLATOR(name)\
   class name : public CShaderTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      virtual void Export(AtNode* shader);\
      AtNode* CreateArnoldNodes();\
   };

// we don't use ShaderTranslator here to get a simpler ResolveOutputPlug method
#define SHADER_TRANSLATOR_MULTIOUT(name)\
   class name : public CNodeTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      virtual void Export(AtNode* shader);\
      AtNode* CreateArnoldNodes();\
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
SHADER_TRANSLATOR(CFileTranslator);
SHADER_TRANSLATOR(CPlace2DTextureTranslator);
SHADER_TRANSLATOR(CBump3DTranslator);
SHADER_TRANSLATOR_MULTIOUT(CSamplerInfoTranslator);
SHADER_TRANSLATOR_MULTIOUT(CPlusMinusAverageTranslator);
SHADER_TRANSLATOR_MULTIOUT(CRemapValueTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CProjectionTranslator);
SHADER_TRANSLATOR(CRampTranslator);
SHADER_TRANSLATOR(CLayeredTextureTranslator);
SHADER_TRANSLATOR(CLayeredShaderTranslator);

class CBump2DTranslator : public CNodeTranslator
{
public:
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
   virtual void ExportMotion(AtNode* shader, AtUInt step);
   AtNode* CreateArnoldNodes();
};
#endif // SHADER_TRANSLATORS_H
