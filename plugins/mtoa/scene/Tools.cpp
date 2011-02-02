
#include "MayaScene.h"

#include <maya/MFnRenderLayer.h>
#include <maya/MPlug.h>



// Check if a DagPath is in the current render layer.
//
// @param node   DagPath evaluated.
// @return       True if the object is in the layer.
//
bool CMayaScene::IsInRenderLayer(MDagPath dagPath)
{
   MObject renderLayerObj = MFnRenderLayer::currentLayer();

   MFnRenderLayer curLayer(renderLayerObj);

   bool isInRenderLayer = curLayer.inCurrentRenderLayer(dagPath);

   if (isInRenderLayer)
      return true;
   else
      return false;
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

// Check if a DagPath is visible and not templated, by attribute or layer.
//
// @param dagPath   DagPath evaluated.
// @return    		True if the object is not templated and visible.
//
bool CMayaScene::IsVisibleDag(MDagPath dagPath)
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

