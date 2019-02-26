#ifndef TRANSLATORS_H
#define TRANSLATORS_H

#include "translators/shader/ShaderTranslator.h"

// template<typename T>
// class CApplyAbsOverrideTranslator : public CShaderTranslator
// {
// public:
//    virtual AtNode* CreateArnoldNodes();
//    virtual void Export(AtNode* shape);
//    static void* creator(){ return new CApplyAbsOverrideTranslator<T>(); }
// };

// template<typename T>
// class CApplyRelOverrideTranslator : public CShaderTranslator
// {
// public:
// virtual AtNode* CreateArnoldNodes();
//    virtual void Export(AtNode* shape);
//    static void* creator(){ return new CApplyRelOverrideTranslator<T>(); }
// };

class CApplyRelIntOverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyRelIntOverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CApplyRelFloatOverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyRelFloatOverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CApplyRelFloat2OverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyRelFloat2OverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CApplyRelFloat3OverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyRelFloat3OverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};


class CApplyAbsIntOverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyAbsIntOverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CApplyAbsFloatOverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyAbsFloatOverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};

class CApplyAbsFloat2OverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyAbsFloat2OverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};


class CApplyAbsFloat3OverrideTranslator : public CShaderTranslator
{
public:
   static void* creator(){return new CApplyAbsFloat3OverrideTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
};


#endif // TRANSLATORS_H
