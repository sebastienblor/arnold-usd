#include "Extension.h"
#include "scene/TranslatorRegistry.h"

// CExtension

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, creator, nodeClassInitializer, providedByPlugin);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, creator, nodeClassInitializer, providedByPlugin);
}
