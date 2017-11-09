#ifndef TRANSLATORS_H
#define TRANSLATORS_H

#include "translators/shader/ShaderTranslator.h"

template<typename T>
class CApplyAbsOverrideTranslator : public CShaderTranslator
{
public:
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   static void* creator(){ return new CApplyAbsOverrideTranslator<T>(); }
};

template<typename T>
class CApplyRelOverrideTranslator : public CShaderTranslator
{
public:
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   static void* creator(){ return new CApplyRelOverrideTranslator<T>(); }
};

#endif // TRANSLATORS_H
