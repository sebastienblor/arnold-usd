#pragma once
#include "NodeTranslatorImpl.h"


class CDagTranslatorImpl : public CNodeTranslatorImpl
{
public :
   CDagTranslatorImpl(CNodeTranslator &translator) :
      CNodeTranslatorImpl(translator) {}
   ~CDagTranslatorImpl() {}

   virtual bool IsMayaTypeDag() const {return true;}
};
