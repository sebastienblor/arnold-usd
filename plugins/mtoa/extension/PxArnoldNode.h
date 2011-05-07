#ifndef PXARNOLDNODE_H
#define PXARNOLDNODE_H

#include <maya/MString.h>

#include <ai_nodes.h>
#include <ai.h>

// An Arnold node entry proxy
class CPxArnoldNode
{
public:
   // in 2012 we can determine the node Id from the node name
   CPxArnoldNode(const MString &nodeName);
   CPxArnoldNode(const AtNodeEntry* nodeEntry=NULL);
   ~CPxArnoldNode() {};

   inline bool operator==(const CPxArnoldNode& other) const { return name == other.name; }
   inline bool operator!=(const CPxArnoldNode& other) const { return name != other.name; }
   inline bool operator<(const CPxArnoldNode& other) const { return strcmp(name.asChar(), other.name.asChar()) < 0; }

   inline bool isNull() const {return (NULL == entry);}

   MString provider;
   MString name;
   const AtNodeEntry* entry;
};

#endif // PXARNOLDNODE_H
