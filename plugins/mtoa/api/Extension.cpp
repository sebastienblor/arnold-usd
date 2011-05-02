#include "Extension.h"
#include "scene/TranslatorRegistry.h"

// CExtension

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, translatorName, creator);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, translatorName, creator);
}

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                          NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, translatorName, creator, nodeClassInitializer, providedByPlugin);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                       NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, translatorName, creator, nodeClassInitializer, providedByPlugin);
}
