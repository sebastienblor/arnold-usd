#ifndef EXTENSION_H
#define EXTENSION_H

#include "scene/NodeTranslator.h"
#include "scene/TranslatorRegistry.h"

class DLLEXPORT CExtension
{
public:
   void RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   void RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   void RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                 NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   void RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                              NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
};

#endif /* EXTENSION_H */
