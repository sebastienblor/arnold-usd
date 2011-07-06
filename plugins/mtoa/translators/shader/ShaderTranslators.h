#ifndef SHADER_TRANSLATORS_H
#define SHADER_TRANSLATORS_H

#include "ShaderTranslator.h"

#define SHADER_TRANSLATOR(name)\
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

#endif // SHADER_TRANSLATORS_H
