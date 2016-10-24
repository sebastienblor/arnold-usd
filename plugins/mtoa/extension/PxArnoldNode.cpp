#include "PxArnoldNode.h"
#include <maya/MStringArray.h>
#include <maya/MNodeClass.h>


// An Arnold node entry proxy
CPxArnoldNode::CPxArnoldNode(const MString &nodeName)
{
   name = nodeName;
   entry = NULL;
   if (name.numChars() > 0) entry = AiNodeEntryLookUp(name.asChar());
   if (NULL != entry)
   {
      const char* fileChar = AiNodeEntryGetFilename(entry);
      if (fileChar != 0)
         file = fileChar;
      else
         file = "";
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
      const char* fileChar = AiNodeEntryGetFilename(entry);
      if (fileChar != 0)
         file = fileChar;
      else
         file = "";
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
