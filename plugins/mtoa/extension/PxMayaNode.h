#pragma once

#include "render/AOV.h"

#include <ai_node_entry.h>


#include <string>

#include <maya/MString.h>
#include <maya/MPxNode.h>
#include <maya/MPlug.h>

class CAbMayaNode;

#include <common/UnorderedContainer.h>
#include <vector>

// A Maya node class proxy
class CPxMayaNode
{
   friend class CExtension;
   friend class CExtensionImpl;   
   friend class CExtensionsManager;

public:
   // in 2012 we can determine the node Id from the node name
   CPxMayaNode(const MString &typeName = "",
               const MTypeId &typeId = MTypeId(0),
               const MString &providerName = "",
               const MString &providerFile = "",
               MCreatorFunction creatorFunction = NULL,
               MInitializeFunction initFunction = NULL,
               MPxNode::Type typeNode = MPxNode::kDependNode,
               const MString &classif = "");
   ~CPxMayaNode() {};

   inline bool operator==(const CPxMayaNode& other) const { return name == other.name; }
   inline bool operator!=(const CPxMayaNode& other) const { return name != other.name; }
   inline bool operator<(const CPxMayaNode& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool IsNull() const {return (name == "");}
   MStatus ReadMetaData(const AtNodeEntry* arnoldNodeEntry);

   void RegisterAOV(const MString &aovName,
                    int dataType,
                    const MString &aovAttr);

   // To be removed later. A single member for the name is surely enough
   // But I want to minimize risks for now
   std::string nameStr; // public stl string for the hash

private:
   void SetName(const MString &n)
   {
      name = n;
      nameStr = name.asChar(); // stl version for the unordered_map's key
   }

   MString name;
   MTypeId id;
   MString provider;
   MString file;
   // MString provider;
   MCreatorFunction creator;
   MInitializeFunction initialize;
   MPxNode::Type type;
   MString classification;
   MString arnold;
   CAbMayaNode *abstract;
   std::vector<CAOVData> m_aovs;
   bool m_aovShader;
};


namespace std {

#ifdef UNORDERED_NEEDS_TR1
   namespace tr1 {
#endif

      template <>
      struct hash<CPxMayaNode>
      {
         std::size_t operator()(const CPxMayaNode& k) const
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
