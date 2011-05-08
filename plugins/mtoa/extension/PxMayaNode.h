#ifndef PXMAYANODE_H
#define PXMAYANODE_H

#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>

// A Maya node class proxy
class CPxMayaNode
{
public:
   // in 2012 we can determine the node Id from the node name
   CPxMayaNode(const MString &typeName = "",
               const MTypeId &typeId = MTypeId(0),
               const MString &providerName = "",
               const MString &providerFile = "",
               MCreatorFunction creatorFunction = NULL,
               MInitializeFunction initFunction = NULL,
               MPxNode::Type typeNode = MPxNode::kDependNode,
               const MString* classif = NULL);
   ~CPxMayaNode() {};

   inline bool operator==(const CPxMayaNode& other) const { return name == other.name; }
   inline bool operator!=(const CPxMayaNode& other) const { return name != other.name; }
   inline bool operator<(const CPxMayaNode& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool IsNull() const {return (name == "");}
   void Set(const CPxMayaNode& other);
   void ReadMetaData(const AtNodeEntry* arnoldNodeEntry);

   MString name;
   MTypeId id;
   MString provider;
   MString file;
   // MString provider;
   MCreatorFunction creator;
   MInitializeFunction initialize;
   MPxNode::Type type;
   MString *classification;

private:
   MString m_classification;
};

#endif // PXMAYANODE_H
