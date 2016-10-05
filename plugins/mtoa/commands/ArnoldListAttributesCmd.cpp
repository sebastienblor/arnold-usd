#include "ArnoldListAttributesCmd.h"
#include "common/AmMap.h"
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnAttribute.h>
#include <maya/MStringArray.h>

#include <map>
#include <string>

CArnoldListAttributesCmd::CArnoldListAttributesCmd()
{
   
}

CArnoldListAttributesCmd::~CArnoldListAttributesCmd()
{
   
}

void* CArnoldListAttributesCmd::creator()
{
   return new CArnoldListAttributesCmd();
}

static void GetAttributesFromNode(const MFnDependencyNode& node, AmMap<std::string, std::string>& attributeList, std::string* fullName = 0)
{
   const unsigned int attributeCount = node.attributeCount();
   for (unsigned int i = 0; i < attributeCount; ++i)
   {      
      MFnAttribute attr(node.attribute(i));
      if (attr.isHidden())
         continue;
      if (!attr.isWritable())
         continue;
      const std::string attrName = attr.name().asChar();
      AmMap<std::string, std::string>::iterator it = attributeList.find(attrName);
      if (it == attributeList.end())
      {
         if (fullName)
            attributeList[attrName] = *fullName + std::string(".") + attrName;
         else
            attributeList[attrName] = std::string(node.name().asChar()) + std::string(".") + attrName;
      }
   }
}

static void GetAttributesFromDgPath(const MDagPath& dg, AmMap<std::string, std::string>& attributeList)
{
   std::string fullName = dg.fullPathName().asChar();
   MFnDependencyNode node(dg.node());
   GetAttributesFromNode(node, attributeList, &fullName);
   unsigned int childCount = dg.childCount();
   for (unsigned int i = 0; i < childCount; ++i)
   {
      MDagPath dg2 = dg;
      dg2.push(dg.child(i));
      GetAttributesFromDgPath(dg2, attributeList);
   }
}

MStatus CArnoldListAttributesCmd::doIt(const MArgList& args)
{
   MStatus status;
   if (args.length() < 1)
   {
      status.perror("Not enough arguments!");
      return status;
   }  
   
   MSelectionList slist;
   status = slist.add(args.asString(0));
   
   if (!status)
   {
      status.perror("Cannot find set object!");
      return status;
   }
   
   MObject setObject;
   slist.getDependNode(0, setObject);
   MFnSet set(setObject, &status);
   
   if (!status)
   {
      status.perror("Cannot create MFnSet!");
      return status;
   }
   slist.clear();
   set.getMembers(slist, true);
   
   unsigned int numSetMembers = slist.length();
   
   AmMap<std::string, std::string> attributeMap;
   
   for (unsigned int i = 0; i < numSetMembers; ++i)
   {
      MDagPath dg;
      status = slist.getDagPath(i, dg);
      if (status)
         GetAttributesFromDgPath(dg, attributeMap);
      else
      {
         MObject nodeObject;
         status = slist.getDependNode(i, nodeObject);
         if (status)
            GetAttributesFromNode(nodeObject, attributeMap);
      }
   }
   
   MStringArray resultArray;
   
   resultArray.setLength((unsigned int)attributeMap.size());
   
   unsigned int id = 0;
   for (AmMap<std::string, std::string>::const_iterator it = attributeMap.begin(); it != attributeMap.end(); ++it)
      resultArray[id++] = it->second.c_str();
   
   setResult(resultArray);
   
   return MS::kSuccess;
}
