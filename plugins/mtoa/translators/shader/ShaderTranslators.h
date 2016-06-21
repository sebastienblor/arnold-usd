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
SHADER_TRANSLATOR(CMayaPhongTranslator);
SHADER_TRANSLATOR(CMayaPhongETranslator);
SHADER_TRANSLATOR(CMayaAnisotropicTranslator);
SHADER_TRANSLATOR(CMayaRampShaderTranslator); 
SHADER_TRANSLATOR(CPhysicalSkyTranslator);

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
   
   // We disable cache to prevent reusing the same bump for
   //  different shaders:
   //
   // Maya:
   //  ----------       -----------
   //  | Bump2d | --->  | Shader1 |
   //  ---------- \     -----------
   //              \    -----------
   //               \-> | Shader2 |
   //                   -----------
   //             __
   //             ||
   //            _||_
   //            \  /
   //             \/
   //
   // Arnold:
   //  -----------     ---------
   //  | Shader1 | --> | Bump1 |
   //  -----------     ---------
   //  -----------     ---------
   //  | Shader2 | --> | Bump2 |
   //  -----------     ---------
   //
   bool DisableCaching() {return true;};
};

class CBump3DTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CBump3DTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   // We disable cache to prevent reusing the same bump for
   //  different shaders. As done in CBump2DTranslator
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
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
};
