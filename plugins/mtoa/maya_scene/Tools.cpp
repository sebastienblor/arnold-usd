
#include "MayaScene.h"

#include <maya/MPlug.h>
#include <maya/MDagPathArray.h>

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

bool CMayaScene::IsVisibleDag(MDagPath dagPath)
{
   MStatus stat = MStatus::kSuccess;
   while (stat == MStatus::kSuccess)
   {
      MFnDagNode node(dagPath.node());
      if (!IsVisible(node))
         return false;
      stat = dagPath.pop();
   }
   return true;
}

// Return whether the dag object in dagPath is the master instance. The master
// is the first instance that is completely visible (including parent transforms)
// for which full geometry should be exported
//
// always returns true if dagPath is not instanced.
// if dagPath is instanced, this searches the preceding instances
// for the first that is visible. if none are found, dagPath is considered the master.
//
// note: dagPath is assumed to be visible.
//
// @param[in]  dagPath      MDagPath object to check
// @param[out] masterDag    the master MDagPath result, only filled if result is false
// @return                  whether or not dagPath is a master
//
bool CMayaScene::IsMasterInstance(const MDagPath &dagPath, MDagPath &masterDag)
{
   if (dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(dagPath.node());
      AtUInt instNum = dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed dagPath is assumed to be visible)
         m_masterInstances[handle] = dagPath;
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = m_masterInstances[handle];
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(dagPath.node(), allInstances);
         AtUInt master_index = 0;
         for (; (master_index < dagPath.instanceNumber()); master_index++)
         {
            currDag = allInstances[master_index];
            if (IsVisibleDag(currDag))
            {
               // found it
               m_masterInstances[handle] = currDag;
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: dagPath is the master
         m_masterInstances[handle] = dagPath;
         return true;
      }
   }
   // not instanced: dagPath is the master
   return true;
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

