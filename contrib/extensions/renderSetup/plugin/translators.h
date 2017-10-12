#ifndef TRANSLATORS_H
#define TRANSLATORS_H

#include "translators/shader/ShaderTranslator.h"

template<typename T>
class CApplyAbsOverrideTranslator : public CShaderTranslator
{
public:
    AtNode* CreateArnoldNodes() override;
    void Export(AtNode* shape) override;
    static void* creator(){ return new CApplyAbsOverrideTranslator<T>(); }
};

template<typename T>
class CApplyRelOverrideTranslator : public CShaderTranslator
{
public:
    AtNode* CreateArnoldNodes() override;
    void Export(AtNode* shape) override;
    static void* creator(){ return new CApplyRelOverrideTranslator<T>(); }
};

#endif // TRANSLATORS_H
