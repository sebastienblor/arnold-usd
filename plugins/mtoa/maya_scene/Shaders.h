#ifndef SHADERS_H
#define SHADERS_H

#include "NodeTranslator.h"

#define SHADER_TRANSLATOR(name)\
   class name : public CNodeTranslator\
   {\
   public:\
      static void* creator(){return new name();}\
      AtNode* Export();\
      void Update(AtNode* shader);\
   };

class CSkyShaderTranslator : public CDagTranslator
{
public:
   static void* creator(){return new CSkyShaderTranslator();}
   AtNode* Export();
   void Update(AtNode* shader);
};

SHADER_TRANSLATOR(CSurfaceShaderTranslator);
SHADER_TRANSLATOR(CLambertTranslator);
SHADER_TRANSLATOR(CFileTranslator);
SHADER_TRANSLATOR(CPlace2dTextureTranslator);
SHADER_TRANSLATOR(CBump2dTranslator);
SHADER_TRANSLATOR(CBump3dTranslator);
SHADER_TRANSLATOR(CSamplerInfoTranslator);
SHADER_TRANSLATOR(CPlusMinusAverageTranslator);
SHADER_TRANSLATOR(CRemapValueTranslator);
SHADER_TRANSLATOR(CRemapColorTranslator);
SHADER_TRANSLATOR(CProjectionTranslator);
SHADER_TRANSLATOR(CRampTranslator);

#endif
