#pragma once
#include "NodeTranslatorImpl.h"


class CDagTranslatorImpl : public CNodeTranslatorImpl
{
public :
   CDagTranslatorImpl(CNodeTranslator &translator) :
      CNodeTranslatorImpl(translator),
      m_isMasterDag(true) {}
   ~CDagTranslatorImpl() {}

   virtual bool IsMayaTypeDag() const {return true;}
   virtual MString MakeArnoldName(const char *nodeType, const char* tag = NULL);
   virtual void ExportUserAttribute(AtNode *anode);
   virtual MStatus ExportOverrideSets();
   
   MDagPath m_masterDag;
   bool m_isMasterDag;

};
