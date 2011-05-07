#include "PxArnoldNode.h"
#if MAYA_API_VERSION >= 201200
   #include <maya/MNodeClass.h>
#endif


// An Arnold node entry proxy
CPxArnoldNode::CPxArnoldNode(const MString &nodeName)
{
   name = nodeName;
   entry = NULL;
   if (name.numChars() > 0) entry = AiNodeEntryLookUp(name.asChar());
   if (NULL != entry) provider = AiNodeEntryGetFilename(entry);
}

CPxArnoldNode::CPxArnoldNode(const AtNodeEntry* nodeEntry)
{
   name = "";
   entry = nodeEntry;
   if (NULL != entry) name = AiNodeEntryGetName(entry);
   if (NULL != entry) provider = AiNodeEntryGetFilename(entry);
}
