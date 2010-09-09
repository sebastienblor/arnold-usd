
#include "MayaScene.h"

#include <maya/MPlug.h>

bool CMayaScene::IsVisible(MFnDagNode node)
{
   MStatus status;

   if (node.isIntermediateObject())
      return false;

   MPlug visPlug = node.findPlug("visibility", &status);
   MPlug overVisPlug = node.findPlug("overrideVisibility", &status);

   if (status == MStatus::kFailure)
      return false;

   if (visPlug.asBool() && overVisPlug.asBool())
      return true;
   else
      return false;
}

void CMayaScene::GetMatrix(AtMatrix& matrix, const MDagPath& dagPath)
{
   MMatrix tm = dagPath.inclusiveMatrix();

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
}

void CMayaScene::ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
{
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) mayaMatrix[I][J];
      }
   }
}

