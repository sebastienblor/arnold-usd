#ifndef ABMAYANODE_H
#define ABMAYANODE_H

#include "extension/PxTransator.h"

#include <maya/MString.h>

#include <ai_nodes.h>
#include <ai.h>

// A Maya node class abstract
// Hold the information necessary to customize base Maya node classes
// for different Arnold nodes

// Extend as it becomes necessary

class CAbTranslator
{
   friend class CPxTranslator;
public:
   CAbMayaNode(const MString &translatorName = "",
               const MString &arnoldClassName = "",
               const MString &providerName = "")
   : name(translatorName),
     arnold(arnoldClassName),
     provider(providerName)
   {}
   CAbMayaNode(const CPxTranslator &translator)
   : name(translator.name),
     arnold(translator.arnold),
     provider(translator.provider)
   {}

   MString name;
   MString arnold;
   MString provider;
};


#endif // ABMAYANODE_H
