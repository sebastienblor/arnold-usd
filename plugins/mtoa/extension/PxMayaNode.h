#ifndef PXMAYANODE_H
#define PXMAYANODE_H

#include <maya/MPxNode.h>

// A Maya node class proxy
class CPxMayaNode
{
public:
   // in 2012 we can determine the node Id from the node name
   CPxMayaNode(const MString &typeName = "",
               const MTypeId &typeId = MTypeId(0),
               const MString &providerString = "",
               MCreatorFunction creatorFunction = NULL,
               MInitializeFunction initFunction = NULL,
               MPxNode::Type typeNode = MPxNode::kDependNode,
               const MString* classif = NULL);
   ~CPxMayaNode() {};

   inline bool operator==(const CPxMayaNode& other) const { return id == other.id; }
   inline bool operator!=(const CPxMayaNode& other) const { return id != other.id; }
   inline bool operator<(const CPxMayaNode& other) const { return id.id() < other.id.id(); }

   inline bool isNull() const {return (id.id() == 0);}

   MString provider;
   MString name;
   MTypeId id;
   // MString provider;
   MCreatorFunction creator;
   MInitializeFunction initialize;
   MPxNode::Type type;
   MString *classification;

private:
   MString m_classification;
};

#endif // PXMAYANODE_H
