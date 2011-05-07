#include "PxTranslator.h"

// A Maya node proxy
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &providerString,
                             const MString &arnoldNodeName,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   provider = providerString;
   if (name.numChars() == 0) name = provider;
   arnold = arnoldNodeName;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &providerString,
                             const AtNodeEntry* arnoldNodeEntry,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   provider = providerString;
   if (name.numChars() == 0) name = provider;
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   creator = creatorFunction;
   initialize = nodeInitFunction;
}
