#pragma once

#include <extension/Extension.h>
#include <cstring>
#include <maya/MString.h>

#include <ai_nodes.h>

class CAbTranslator;



// A translator proxy
class CPxTranslator
{
   friend class CAbTranslator;
   friend class CExtension;
   friend class CExtensionImpl;
   friend class CExtensionsManager;

public:
   CPxTranslator(const MString &translatorName = "",
                 const MString &providerName = "",
                 const MString &providerFile = "",
                 TCreatorFunction creatorFunction = NULL,
                 TNodeInitFunction nodeInitFunction = NULL);
   ~CPxTranslator() {};

   inline bool operator==(const CPxTranslator& other) const { return name == other.name; }
   //inline bool operator!=(const CPxTranslator& other) const { return name != other.name; }
   //inline bool operator<(const CPxTranslator& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool IsNull() const {return (name == "");}
   MStatus ReadMetaData(const AtNodeEntry* arnoldNodeEntry, bool mappedMayaNode);

   std::string nameStr; // public stl version of the name for the hash
private:
   MString name;
   MString provider;
   MString file;
   MString arnold;
   TCreatorFunction creator;
   TNodeInitFunction initialize;
};


namespace std {

   template <>
   struct hash<CPxTranslator>
   {
      std::size_t operator()(const CPxTranslator& k) const
      {
         using std::size_t;
         using std::hash;
         using std::string;

         return (hash<string>()(k.nameStr));
      }
   };

}