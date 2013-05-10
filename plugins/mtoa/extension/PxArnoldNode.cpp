#include "PxArnoldNode.h"
#include <maya/MStringArray.h>
#if MAYA_API_VERSION >= 201200
   #include <maya/MNodeClass.h>
#endif

#include "extension/PathUtils.h"

// An Arnold node entry proxy
CPxArnoldNode::CPxArnoldNode(const MString &nodeName)
{
   name = nodeName;
   entry = NULL;
   if (name.numChars() > 0) entry = AiNodeEntryLookUp(name.asChar());
   if (NULL != entry)
   {
      file = AiNodeEntryGetFilename(entry);
      MStringArray split;
      file.split('/', split);
      unsigned int l = split.length();
      provider = (l > 0) ? split[l-1] : file;
   }
}

CPxArnoldNode::CPxArnoldNode(const AtNodeEntry* nodeEntry)
{
   name = "";
   entry = nodeEntry;
   if (NULL != entry) name = AiNodeEntryGetName(entry);
   if (NULL != entry)
   {
      file = AiNodeEntryGetFilename(entry);
      MStringArray split;
      file.split('/', split);
      unsigned int l = split.length();
      provider = (l > 0) ? split[l-1] : file;
   }
}

void CPxArnoldNode::Set(const CPxArnoldNode& other)
{
   name = other.name;
   entry = other.entry;
   file = other.file;
   provider = other.provider;
}
