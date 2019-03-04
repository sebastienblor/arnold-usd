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
SHADER_TRANSLATOR_MULTIOUT(CParticleSamplerInfoTranslator);
SHADER_TRANSLATOR(CLayeredShaderTranslator);
SHADER_TRANSLATOR(CRemapHsvTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CMayaBlinnTranslator);
SHADER_TRANSLATOR(CMayaPhongTranslator);
SHADER_TRANSLATOR(CMayaPhongETranslator);
SHADER_TRANSLATOR(CMayaAnisotropicTranslator);
SHADER_TRANSLATOR(CMayaRampShaderTranslator); 
SHADER_TRANSLATOR(CPhysicalSkyTranslator);
SHADER_TRANSLATOR(CMayaBlendColorsTranslator);
SHADER_TRANSLATOR(CMayaClampTranslator);
SHADER_TRANSLATOR(CMultiplyDivideTranslator);
SHADER_TRANSLATOR(CToonTranslator);
SHADER_TRANSLATOR(CConditionTranslator);
SHADER_TRANSLATOR(CLuminanceTranslator);
SHADER_TRANSLATOR(CReverseTranslator);
SHADER_TRANSLATOR(CSurfaceLuminanceTranslator);
SHADER_TRANSLATOR(CGammaCorrectTranslator);
SHADER_TRANSLATOR(CRgbToHsvTranslator);
SHADER_TRANSLATOR(CHsvToRgbTranslator);
SHADER_TRANSLATOR(CUserDataVec2Translator);
SHADER_TRANSLATOR(CUserDataBoolTranslator);
SHADER_TRANSLATOR(CUserDataVectorTranslator);
SHADER_TRANSLATOR(CSetRangeTranslator);

class CDisplacementTranslator : public CShaderTranslator
{
   CDisplacementTranslator() : CShaderTranslator(), m_isVectorDisp(false) {}
public:
   static void* creator(){return new CDisplacementTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   bool DependsOnOutputPlug() { return true; }

protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   bool m_isVectorDisp;
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


class CBump2DTranslator : public CShaderTranslator
{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator(){return new CBump2DTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
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
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   
private:
   void ReplaceFileToken(MString &filename, const MString &tokenIn, const MString &tokenOut);
   MString m_colorSpace;

   bool RequiresColorCorrect() const;
   bool RequiresUvTransform() const;
   
   bool m_hasColorCorrect;
   bool m_hasUvTransform;
};

class CCheckerTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CCheckerTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   
private:

   bool RequiresColorCorrect() const;
   bool RequiresUvTransform() const;
   
   bool m_hasColorCorrect;
   bool m_hasUvTransform;
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

class CAiRoundCornersTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiRoundCornersTranslator();}

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
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
class CAiPassthroughTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiPassthroughTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();

};

class CRampTranslator : public CShaderTranslator{
public:
   enum RampType
   {
      RT_V = 0,
      RT_U,
      RT_DIAGONAL,
      RT_RADIAL,
      RT_CIRCULAR,
      RT_BOX,
      RT_UV,
      RT_4CORNER,
      RT_TARTAN
   };
   enum RampInterpolationType
   {
      RIT_NONE = 0,
      RIT_LINEAR,
      RIT_EXP_UP,
      RIT_EXP_DOWN,
      RIT_SMOOTH,
      RIT_BUMP,
      RIT_SPIKE
   };

   static void* creator(){return new CRampTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
private:
   bool RequiresColorCorrect() const;
   bool RequiresUvTransform() const;
   AtNode *ExportUvTransform();
   AtNode *ExportColorCorrect(AtNode *target);
   void ExportRampKeys(AtNode *shader);
   void ExportRampType(AtNode *rampShader, RampType type);
   void GetUvSet();

   int m_type;
   AtNode *m_custom_uvs;
   bool m_hasColorCorrect;
   MString m_uvSet;
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

class CAiOslShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CAiOslShaderTranslator();}

   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);


};

class CSurfaceShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CSurfaceShaderTranslator();}

   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};

class CContrastTranslator : public CShaderTranslator
{   
public:
   CContrastTranslator() : CShaderTranslator(), m_isRgb(false) {}
   
   static void* creator(){return new CContrastTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   bool m_isRgb;
};   

class CRemapValueTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CRemapValueTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};

class CLayeredTextureTranslator : public CShaderTranslator
{
public:
   
enum LayeredTextureBlendMode
{
   BM_NONE = 0,
   BM_OVER,
   BM_IN,
   BM_OUT,
   BM_ADD,
   BM_SUBTRACT,
   BM_MULTIPLY,
   BM_DIFFERENCE,
   BM_LIGHTEN,
   BM_DARKEN,
   BM_SATURATE,
   BM_DESATURATE,
   BM_ILLUMINATE
};

   static void* creator(){return new CLayeredTextureTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};

class CProjectionTranslator : public CShaderTranslator
{
public:
enum ProjectionType
{
   PT_NONE = 0,
   PT_PLANAR,
   PT_SPHERICAL,
   PT_CYLINDRICAL,
   PT_BALL,
   PT_CUBIC,
   PT_TRIPLANAR,
   PT_CONCENTRIC,
   PT_PERSPECTIVE
};

enum FitType
{
   FIT_NONE = 0,
   FIT_CAMERA_FILM_GATE, 
   FIT_CAMERA_RESOLUTION
};
enum FillType
{
   FILL_FILL = 0,
   FILL_HORIZONTAL,
   FILL_VERTICAL
};


   static void* creator(){return new CProjectionTranslator();}
   virtual void Export(AtNode* shader);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   bool RequiresColorCorrect() const;
};