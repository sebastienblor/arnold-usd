#include "ArnoldListAttributesCmd.h"

#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnAttribute.h>

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

MStatus CArnoldListAttributesCmd::doIt(const MArgList& args)
{
   MStatus status;
   if (args.length() < 1)
   {
      status.perror("Not enough arguments!");
      return status;
   }  
   
   MSelectionList slist;
   slist.add(args.asString(0), &status);
   
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
   
   return MS::kSuccess;
}
