#ifndef EXTENSION_H
#define EXTENSION_H

#include "scene/NodeTranslator.h"

class DLLEXPORT CExtension
{
public:
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
};

#endif /* EXTENSION_H */
