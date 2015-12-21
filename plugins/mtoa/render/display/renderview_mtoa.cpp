
#include "renderview_mtoa.h"

#include "scene/MayaScene.h"

#include <maya/MQtUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MEventMessage.h>

static MCallbackId rvSelectionCb = 0;

/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/

void CRenderViewMtoA::UpdateSceneChanges(const std::vector<AtNode*> *modifiedNodes, 
   const std::vector<AtNode *> *addedNodes,
   const std::vector<AtNode *> *deletedNodes)
{
   CMayaScene::UpdateSceneChanges();
}


void CRenderViewMtoA::GetSelection(std::vector<AtNode *> &selectedNodes)
{
   MSelectionList activeList;
   MGlobal::getActiveSelectionList(activeList);
   if( !activeList.isEmpty())
   {
      MObject depNode;
      activeList.getDependNode(0, depNode);
      if (depNode.hasFn(MFn::kTransform))
      {
         // from Transform to Shape
         MDagPath dagPath;
         activeList.getDagPath(0, dagPath);
         depNode = dagPath.child(0);
      }
      MFnDependencyNode nodeFn( depNode );

      AtNode *selected = AiNodeLookUpByName(nodeFn.name().asChar());
      if (selected) selectedNodes.push_back(selected);
   }
}

// For "Isolate Selected" debug shading mode,
// we need to receive the events that current
// Selection has changed
void CRenderViewMtoA::SelectionChangedCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   MSelectionList activeList;
   MGlobal::getActiveSelectionList(activeList);
   if( activeList.isEmpty()) return;

   MObject depNode;
   std::vector<AtNode *> selection;
   unsigned int count = activeList.length();

   for (unsigned int i = 0; i < count; ++i)
   {
      activeList.getDependNode(i, depNode);
      if (depNode.hasFn(MFn::kTransform))
      {
         // from Transform to Shape
         MDagPath dagPath;
         activeList.getDagPath(i, dagPath);
         depNode = dagPath.child(0);
      }

      MFnDependencyNode nodeFn( depNode );

      AtNode *selected_shader =  AiNodeLookUpByName (nodeFn.name().asChar());
      if(selected_shader) selection.push_back(selected_shader);
   }
   
   renderViewMtoA->HostSelectionChanged(selection);

}


void CRenderViewMtoA::SetSelection(const std::vector<AtNode *> &selectedNodes, bool append)
{   
   if (append)
   {
      if (selectedNodes.empty()) return;
      for (size_t i = 0; i < selectedNodes.size(); ++i)
      {
         MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[i])), MGlobal::kAddToList);
      }

   } else 
   {
      if (selectedNodes.empty()) 
      {
         MGlobal::clearSelectionList();
         return;
      }
      
      MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[0])), MGlobal::kReplaceList);
      for (size_t i = 1; i < selectedNodes.size(); ++i)
      {
         MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[i])), MGlobal::kAddToList);
      }
   }
}

void CRenderViewMtoA::NodeParamChanged(AtNode *node, const std::string &paramName)
{
   if (node != AiUniverseGetOptions()) return;

   if (paramName == "region_min_x" || paramName == "region_min_y" || paramName == "region_max_x" || paramName == "region_max_y")
   {
      CRenderSession* renderSession = CMayaScene::GetRenderSession();

      int minx = AiNodeGetInt(node, "region_min_x");
      int miny = AiNodeGetInt(node, "region_min_y");
      int maxx = AiNodeGetInt(node, "region_max_x");
      int maxy = AiNodeGetInt(node, "region_max_y");

      if (minx < 0 || miny < 0)
      {
         int xres = AiNodeGetInt(node, "xres");
         int yres = AiNodeGetInt(node, "yres");

         renderSession->SetRegion(0, 0 , xres, yres);
      } else
      {
         renderSession->SetRegion(minx, miny , maxx, maxy);
      }
      return;
   }

   if (paramName == "camera")
   {
      AtNode *cam = (AtNode*)AiNodeGetPtr(node, "camera");
      if (cam == NULL) return;

      std::string cameraName = AiNodeGetName(cam);
      // Search for the MDagPath for this camera   
      MItDag itDag(MItDag::kDepthFirst, MFn::kCamera);
      itDag.reset();

      while (!itDag.isDone())
      {
         MDagPath camPath;
         itDag.getPath(camPath);
         std::string camName = camPath.partialPathName().asChar();
         if (camName == cameraName)
         {
            CMayaScene::GetRenderSession()->SetCamera(camPath);
            break;
         }      
         itDag.next();
      }
   }
}

void CRenderViewMtoA::ReceiveSelectionChanges(bool receive)
{
   if (rvSelectionCb)
   {
      MMessage::removeCallback(rvSelectionCb);
      rvSelectionCb = 0;
   }

   if (receive)
   {

      rvSelectionCb = MEventMessage::addEventCallback("SelectionChanged",
                                      CRenderViewMtoA::SelectionChangedCallback,
                                      (void*)this);

   } 

}

void CRenderViewMtoA::RenderViewClosed()
{
   ReceiveSelectionChanges(false);
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {   
      renderSession->SetRendering(false);
      CMayaScene::End();
   }
}
CRenderViewPanManipulator *CRenderViewMtoA::GetPanManipulator()
{
   return new CRenderViewMtoAPan();
}

CRenderViewZoomManipulator *CRenderViewMtoA::GetZoomManipulator()
{
   return new CRenderViewMtoAZoom();
}

CRenderViewRotateManipulator *CRenderViewMtoA::GetRotateManipulator()
{
   return new CRenderViewMtoARotate();
}
   
