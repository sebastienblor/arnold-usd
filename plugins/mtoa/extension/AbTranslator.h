#ifndef ABTRANSLATOR_H
#define ABTRANSLATOR_H

#include <maya/MString.h>

// A Maya node class abstract
// Hold the information necessary to customize base Maya node classes
// for different Arnold nodes

// Extend as it becomes necessary

class CAbTranslator
{
   friend class CPxTranslator;
public:
   CAbTranslator(const MString &translatorName = "",
               const MString &arnoldClassName = "",
               const MString &providerName = "")
   : name(translatorName),
     arnold(arnoldClassName),
     provider(providerName)
   {}

   MString name;
   MString arnold;
   MString provider;
};


#endif // ABTRANSLATOR_H
