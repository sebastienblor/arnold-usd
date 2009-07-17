
#include "Utils.h"

#include <maya/MPlug.h>

bool IsVisible(MFnDagNode node)
{
   MStatus status;

   if (node.isIntermediateObject())
      return false;

   MPlug visPlug = node.findPlug("visibility", &status);

   if (status == MStatus::kFailure)
      return false;

   return visPlug.asBool();
}  // IsVisible()

