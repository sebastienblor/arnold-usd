#ifndef SHADERS_H
#define SHADERS_H

#include "NodeTranslator.h"

#define SHADER_TRANSLATOR(name)\
   class DLLEXPORT name : public CNodeTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      virtual void Export(AtNode* shader);\
      const char* GetArnoldNodeType();\
   };

class DLLEXPORT CSkyShaderTranslator
   :  public CDagTranslator
{
public:
   static void* creator(){return new CSkyShaderTranslator();}
   virtual void Export(AtNode* shader);
   const char* GetArnoldNodeType();
};

SHADER_TRANSLATOR(CSurfaceShaderTranslator);
SHADER_TRANSLATOR(CLambertTranslator);
SHADER_TRANSLATOR(CFileTranslator);
SHADER_TRANSLATOR(CPlace2DTextureTranslator);
SHADER_TRANSLATOR(CBump2DTranslator);
SHADER_TRANSLATOR(CBump3DTranslator);
SHADER_TRANSLATOR(CSamplerInfoTranslator);
SHADER_TRANSLATOR(CPlusMinusAverageTranslator);
SHADER_TRANSLATOR(CRemapValueTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CProjectionTranslator);
SHADER_TRANSLATOR(CRampTranslator);
SHADER_TRANSLATOR(CLayeredTextureTranslator);
SHADER_TRANSLATOR(CLayeredShaderTranslator);

#endif // SHADERS_H
