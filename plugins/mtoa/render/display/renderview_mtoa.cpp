
#include "renderview_mtoa.h"
#include "renderview.h"

/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/

void CRenderViewMtoA::UpdateSceneChanges(const std::vector<AtNode*> &modifiedNodes, 
   const std::vector<AtNode *> &addedNodes,
   const std::vector<AtNode *> &deletedNodes)
{
   // To be implemented

}


void CRenderViewMtoA::GetSelection(const std::vector<AtNode *> &selectedNodes)
{
   // To be implemented
}

void CRenderViewMtoA::SetSelection(const std::vector<AtNode *> &selectedNode)
{
   // To be implemented
}

void CRenderViewMtoA::ReceiveSelectionChanges(bool receive)
{
   // To be implemented
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
   
