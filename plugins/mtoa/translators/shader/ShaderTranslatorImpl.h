#pragma once
#include "../NodeTranslatorImpl.h"


class CShaderTranslatorImpl : public CNodeTranslatorImpl
{
public :
   CShaderTranslatorImpl(CNodeTranslator &translator) :
      CNodeTranslatorImpl(translator) {}
   ~CShaderTranslatorImpl() {}


   virtual bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug);

   std::map<std::string, MPlugArray> m_aovShadingGroups;

};
