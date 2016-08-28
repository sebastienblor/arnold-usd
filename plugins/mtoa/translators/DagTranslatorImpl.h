#pragma once
#include "NodeTranslatorImpl.h"

typedef std::map<MObjectHandle, MDagPath, MObjectCompare> ObjectHandleToDagMap;


class CDagTranslatorImpl : public CNodeTranslatorImpl
{
public :
   CDagTranslatorImpl(CNodeTranslator &translator) :
      CNodeTranslatorImpl(translator),
      m_isMasterDag(true) {}
   ~CDagTranslatorImpl() {}

   virtual bool IsMayaTypeDag() const {return true;}
   virtual void SetArnoldNodeName(AtNode* arnoldNode, const char* tag=NULL);

   MDagPath m_masterDag;
   bool m_isMasterDag;

};
