#pragma once

#include "ShaderTranslator.h"
#include "translators/DagTranslator.h"
#include <string>

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
SHADER_TRANSLATOR_MULTIOUT(CSamplerInfoTranslator);
SHADER_TRANSLATOR(CPlusMinusAverageTranslator);
SHADER_TRANSLATOR_MULTIOUT(CRemapValueTranslator);
SHADER_TRANSLATOR_MULTIOUT(CParticleSamplerInfoTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CProjectionTranslator);
SHADER_TRANSLATOR(CLayeredTextureTranslator);
SHADER_TRANSLATOR(CLayeredShaderTranslator);
SHADER_TRANSLATOR(CRemapHsvTranslator);
SHADER_TRANSLATOR(CMayaBlinnTranslator);
SHADER_TRANSLATOR(CMayaPhongTranslator);
SHADER_TRANSLATOR(CMayaPhongETranslator);
SHADER_TRANSLATOR(CMayaAnisotropicTranslator);
SHADER_TRANSLATOR(CMayaRampShaderTranslator); 
SHADER_TRANSLATOR(CPhysicalSkyTranslator);


class CDisplacementTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CDisplacementTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   bool DependsOnOutputPlug() { return true; }

protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};

class CMayaShadingSwitchTranslator : public CShaderTranslator{
private:
   std::string m_nodeType;
   int m_paramType;
public:
   CMayaShadingSwitchTranslator(const char* nodeType, int paramType);
   void Export(AtNode* shadingSwitch);
   AtNode* CreateArnoldNodes();
};

void* CreateSingleShadingSwitchTranslator();
void* CreateDoubleShadingSwitchTranslator();
void* CreateTripleShadingSwitchTranslator();
void* CreateQuadShadingSwitchTranslator();


void DisplacementTranslatorNodeInitializer(CAbTranslator context);
void ProjectionTranslatorNodeInitializer(CAbTranslator context);

class CBump2DTranslator : public CShaderTranslator
{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator(){return new CBump2DTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CBump3DTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CBump3DTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();

};

class CAnimCurveTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CAnimCurveTranslator();}
   virtual void Export(AtNode* shader);
   virtual void ExportMotion(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CFileTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CFileTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);
private:
   MString m_colorSpace;
};

class CAiHairTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiHairTranslator();}

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
};

class CAiStandardHairTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiStandardHairTranslator();}

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
};

class CAiImageTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiImageTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
private:
   MString m_colorSpace;
};

class CAiRaySwitchTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiRaySwitchTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};

class CAiSwitchShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiSwitchShaderTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};

class CRampTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CRampTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};


class CAiMixShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiMixShaderTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
};

class CAiAovWriteColorTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiAovWriteColorTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};

class CAiAovWriteFloatTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiAovWriteFloatTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};
