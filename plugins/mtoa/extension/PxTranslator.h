#ifndef PXTRANSLATOR_H
#define PXTRANSLATOR_H

#include <maya/MString.h>

#include <ai_nodes.h>
#include <ai.h>

typedef void *   (*TCreatorFunction)();
// typedef void     (*TNodeInitFunction)(MObject&);
typedef void     (*TNodeInitFunction)(MString);

// A translator proxy
class CPxTranslator
{
public:
   CPxTranslator(const MString &translatorName = "",
                 const MString &providerName = "",
                 const MString &providerFile = "",
                 const MString &arnoldNodeName = "",
                 TCreatorFunction creatorFunction = NULL,
                 TNodeInitFunction nodeInitFunction = NULL);
   CPxTranslator(const MString &translatorName,
                 const MString &providerName,
                 const MString &providerFile,
                 const AtNodeEntry* arnoldNodeEntry,
                 TCreatorFunction creatorFunction = NULL,
                 TNodeInitFunction nodeInitFunction = NULL);
   ~CPxTranslator() {};

   inline bool operator==(const CPxTranslator& other) const { return name == other.name; }
   inline bool operator!=(const CPxTranslator& other) const { return name != other.name; }
   inline bool operator<(const CPxTranslator& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool IsNull() const {return (name == "");}
   void Set(const CPxTranslator& other);
   void ReadMetaData(const AtNodeEntry* arnoldNodeEntry);

   MString name;
   MString provider;
   MString file;
   MString arnold;
   TCreatorFunction creator;
   TNodeInitFunction initialize;
};

#endif // PXTRANSLATOR_H
