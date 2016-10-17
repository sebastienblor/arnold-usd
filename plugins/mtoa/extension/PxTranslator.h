#pragma once

#include <extension/Extension.h>
#include <cstring>
#include <maya/MString.h>

#include <ai_nodes.h>
#include <common/UnorderedContainer.h>

class CAbTranslator;



// A translator proxy
class CPxTranslator
{
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
   inline bool operator!=(const CPxTranslator& other) const { return name != other.name; }
   inline bool operator<(const CPxTranslator& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool IsNull() const {return (name == "");}
   MStatus ReadMetaData(const AtNodeEntry* arnoldNodeEntry, bool mappedMayaNode);

   // To be removed later. A single member for the name is surely enough
   // But I want to minimize risks for now
   std::string nameStr; // public stl version of the name for the hash
private:
   void SetName(const MString &n)
   {
      name = n;
      nameStr = name.asChar(); // stl version for the unordered_map's key
   }

   MString name;
   MString provider;
   MString file;
   MString arnold;
   TCreatorFunction creator;
   TNodeInitFunction initialize;
};

namespace std {

#ifdef UNORDERED_NEEDS_TR1
   namespace tr1 {
#endif

      template <>
      struct hash<CPxTranslator>
      {
         std::size_t operator()(const CPxTranslator& k) const
         {
            using std::size_t;
            using std::string;

            return (hash<string>()(k.nameStr));
         }
      };
   
#ifdef UNORDERED_NEEDS_TR1
   }
#endif
   
}
