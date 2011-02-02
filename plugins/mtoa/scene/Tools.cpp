
#include "MayaScene.h"

#include <maya/MFnRenderLayer.h>
#include <maya/MPlug.h>

// Test a dag path against the ExportFilter
// set for this CMayaScene
// Note that the ExportFilter filter.selected flag is treated
// separatly as it would not be efficient to iterate whole DAG
// and check a selected status for each node

DagFiltered CMayaScene::FilteredStatus(ExportFilter filter, MDagPath path)
{
   // Tests that cause the whole branch to be pruned
   if (filter.templated == true && IsTemplatedPath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   if (filter.hidden == true && !IsVisiblePath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;

   // Tests that cause the node to be ignored
   if (filter.notinlayer == true && !IsInRenderLayer(path))
      return MTOA_EXPORT_REJECTED_NODE;

   // Then test against all types passed in the MFN::Types array
   MObject obj = path.node();
   MFnDagNode node(obj);
   MString name = node.name();
   ExcludeSet::const_iterator sit(filter.excluded.begin()), send(filter.excluded.end());
   for(; sit!=send;++sit)
      if (obj.hasFn(*sit))
         return MTOA_EXPORT_REJECTED_NODE;

   return MTOA_EXPORT_ACCEPTED;
}

// Check if a DagPath is exported, given the current settings.
//
// @param dagPath   DagPath evaluated.
// @return        True if the object is not templated and visible.
bool CMayaScene::IsExportedPath(ExportFilter filter, MDagPath path)
{
   if (MTOA_EXPORT_ACCEPTED != FilteredStatus(filter, path))
      return false;

   while (MStatus::kSuccess == path.pop())
   {
      if (MTOA_EXPORT_REJECTED_BRANCH == FilteredStatus(filter, path))
         return false;
   }
   return true;
}

// Check if a DagPath is in the current render layer.
//
// @param node   DagPath evaluated.
// @return       True if the object is in the layer.
//
// TODO: use the renderLayer specified in the CMayaScene instead
bool CMayaScene::IsInRenderLayer(MDagPath dagPath)
{
   MObject renderLayerObj = MFnRenderLayer::currentLayer();

   MFnRenderLayer curLayer(renderLayerObj);

   return curLayer.inCurrentRenderLayer(dagPath);
}

// Check if a DagNode is templated, by attribute or layer.
//
// @param node	DagNode evaluated.
// @return    	True if the object is templated.
//
bool CMayaScene::IsTemplated(MFnDagNode node)
{
   MStatus status;

   if (node.isIntermediateObject())
     return false;

   MPlug templatePlug = node.findPlug("template", &status);
   MPlug overDispPlug = node.findPlug("overrideDisplayType", &status);

   if (status == MStatus::kFailure)
      return false;

   if (templatePlug.asBool())
     return true;
   else
      if (overDispPlug.asInt()==1)
         return true;
      else
         return false;
}

// Check if a DagNode is visible, by attribute or layer.
//
// @param node	DagNode evaluated.
// @return    	True if the object is visible.
//
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

// Check if a DagPath is visible.
//
// @param dagPath   DagPath evaluated.
// @return    		True if the object is not templated and visible.
//
bool CMayaScene::IsVisiblePath(MDagPath dagPath)
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

// Check if a DagPath is templated
// @param dagPath   DagPath evaluated.
// @return        True if the object is not templated and visible.
//
bool CMayaScene::IsTemplatedPath(MDagPath dagPath)
{

   MStatus stat = MStatus::kSuccess;
   while (stat == MStatus::kSuccess)
   {
      MFnDagNode node(dagPath.node());
      if (IsTemplated(node))
         return true;
      stat = dagPath.pop();
   }
   return false;
}

// Check if a DagPath is visible and not templated, by attribute or layer.
//
// @param dagPath   DagPath evaluated.
// @return        True if the object is not templated and visible.
bool CMayaScene::IsRenderablePath(MDagPath dagPath)
{

   MStatus stat = MStatus::kSuccess;
   while (stat == MStatus::kSuccess)
   {
      MFnDagNode node(dagPath.node());
      if (!IsVisible(node) || IsTemplated(node))
         return false;
      stat = dagPath.pop();
   }
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

