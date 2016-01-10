
#include "renderview_mtoa.h"
#include "scene/MayaScene.h"


//#include <maya/MQtUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MEventMessage.h>

#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnTransform.h>

static MCallbackId rvSelectionCb = 0;

/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/

void CRenderViewMtoA::UpdateSceneChanges()
{
   CMayaScene::UpdateSceneChanges();
   SetFrame((float)CMayaScene::GetArnoldSession()->GetExportFrame());
}

static void GetSelectionVector(std::vector<AtNode *> &selectedNodes)
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
unsigned int CRenderViewMtoA::GetSelectionCount()
{
   std::vector<AtNode *>selection;
   GetSelectionVector(selection);
   return (unsigned int)selection.size();
}
void CRenderViewMtoA::GetSelection(AtNode **selection)
{
   std::vector<AtNode *> selectionVec;
   GetSelectionVector(selectionVec);
   if (selectionVec.empty()) return;

   memcpy(selection, &selectionVec[0], selectionVec.size() * sizeof(AtNode*));
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
   
   renderViewMtoA->HostSelectionChanged((selection.empty()) ? NULL : (const AtNode **)&selection[0], selection.size());

}


void CRenderViewMtoA::SetSelection(const AtNode **selectedNodes, unsigned int selectionCount, bool append)
{   
   if (append)
   {
      if (selectionCount == 0) return;
      for (unsigned int i = 0; i < selectionCount; ++i)
      {
         MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[i])), MGlobal::kAddToList);
      }

   } else 
   {
      if (selectionCount == 0) 
      {
         MGlobal::clearSelectionList();
         return;
      }
      
      MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[0])), MGlobal::kReplaceList);
      for (unsigned int i = 1; i < selectionCount; ++i)
      {
         MGlobal::selectByName(MString(AiNodeGetName(selectedNodes[i])), MGlobal::kAddToList);
      }
   }
}

void CRenderViewMtoA::NodeParamChanged(AtNode *node, const char *paramNameChar)
{
   if (node != AiUniverseGetOptions()) return;
   std::string paramName = paramNameChar;

/*
   We used to advert the RenderSession that region had changed, but in fact this just causes more confusion
   since the renderSession will do other changes to "region_min*" "region_max*"
   So from now on, we're no longer telling the Render Session what is happening with the crop region
   and it's much simpler...

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

         renderSession->SetRegion(0, xres, 0, yres);
      } else
      {
         renderSession->SetRegion(minx, maxx , miny, maxy);
      }
      return;
   }

   */

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
   
CRenderViewMtoAPan::CRenderViewMtoAPan() : CRenderViewPanManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_rightDirection = m_camera.rightDirection(MSpace::kWorld);
   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);

   m_distFactor = 1.f;
   
   if (strcmp (AiNodeEntryGetName(AiNodeGetNodeEntry(arnold_camera)), "persp_camera") != 0) return;

   MPoint originalPosition = m_camera.eyePoint(MSpace::kWorld);
   MPoint center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = (float)center.distanceTo(originalPosition);

   m_distFactor = center_dist * tanf(AiNodeGetFlt(arnold_camera, "fov") * AI_DTOR);

   m_width = AiNodeGetInt(AiUniverseGetOptions(), "xres");
}

void CRenderViewMtoAPan::MouseDelta(int deltaX, int deltaY)
{
      // get the delta factor relative to the width 
   float delta[2];
   delta[0] = (-m_distFactor * ((deltaX)/((float)m_width)));
   delta[1] = (m_distFactor * ((deltaY)/((float)m_width)));

   MPoint newPosition = m_originalPosition + m_rightDirection * delta[0] + m_upDirection * delta[1];
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
}

CRenderViewMtoAZoom::CRenderViewMtoAZoom() : CRenderViewZoomManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_dist = (float)m_center.distanceTo(m_originalPosition);

   m_width = AiNodeGetInt(AiUniverseGetOptions(), "xres");
}

void CRenderViewMtoAZoom::MouseDelta(int deltaX, int deltaY)
{
   float delta = (float)deltaX / (float)m_width;

   if (delta > 0.9f)
   {
      float diff = delta - 0.9f;
      delta = 0.9f;
      while (diff > 0.f)
      {
         delta += MIN(diff, 1.f) * (1.f - delta) * 0.5f;
         diff -= 1.f;
      }
   }
   delta *= m_dist;   

   MPoint newPosition = m_originalPosition + m_viewDirection * delta;
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}

void CRenderViewMtoAZoom::WheelDelta(float delta)
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   MDagPath camDag;
   camList.getDagPath(0, camDag);

   if (!camDag.isValid()) return;
   MObject camNode = camDag.node();
   MFnCamera camera;
   camera.setObject(camDag);
      
   MVector view_direction = camera.viewDirection(MSpace::kWorld);
   MPoint originalPosition = camera.eyePoint(MSpace::kWorld);

   delta /= 120.f;
   MPoint center = camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = (float)center.distanceTo(originalPosition);
   delta *= center_dist / 10.f;
   MPoint newPosition = originalPosition;
   MVector zoom = view_direction;
   zoom *= delta;
   newPosition +=  zoom;
   camera.set(newPosition, view_direction, camera.upDirection(MSpace::kWorld), camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);
}

void CRenderViewMtoAZoom::FrameSelection()
{
   // FIXME to be extracted       
      // Frame the selected geometries bounding box
   
   MSelectionList selected; 
   MBoundingBox globalBox;

   MGlobal::getActiveSelectionList(selected);
   for (unsigned int i = 0; i < selected.length(); ++i)
   {
      MDagPath dagPath;
      if (selected.getDagPath(i, dagPath) != MS::kSuccess) continue;
      MStatus status;
      MFnDagNode dagShape(dagPath, &status);
      if (status != MS::kSuccess) continue;

      MBoundingBox boundingBox = dagShape.boundingBox(&status);
      if (status != MS::kSuccess) continue;

      MMatrix mtx = dagPath.inclusiveMatrix();

      boundingBox.transformUsing(mtx);
      globalBox.expand(boundingBox);
   }
   AtNode *arnoldCamera = AiUniverseGetCamera();
   if (arnoldCamera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnoldCamera, "name")));

   MDagPath camDag;
   camList.getDagPath(0, camDag);

   if (!camDag.isValid()) return;
   MObject camNode = camDag.node();
   MFnCamera camera;
   camera.setObject(camDag);
   MMatrix camToWorld = camDag.inclusiveMatrix();

   // don't want to change the viewDirection & upDirection
   MVector viewDirection = camera.viewDirection(MSpace::kWorld);
   MVector upDirection = camera.upDirection(MSpace::kWorld);
   MPoint eyePoint = camera.eyePoint(MSpace::kWorld);
   MPoint centerInterest = camera.centerOfInterestPoint(MSpace::kWorld);

   MPoint center = globalBox.center();
   MFloatMatrix projectionMatrix = camera.projectionMatrix();

   MPoint newPos = eyePoint + center - centerInterest;
   float centerDist = float(center.distanceTo(newPos));

   camToWorld[3][0] = newPos.x;
   camToWorld[3][1] = newPos.y;
   camToWorld[3][2] = newPos.z;

   MMatrix worldToCam = camToWorld.inverse();

   globalBox.transformUsing(worldToCam);

   MPoint minBox = globalBox.min(); // in camera space
   MPoint maxBox = globalBox.max(); // in camera space

   MMatrix proj;
   for (int i =0; i < 4; ++i)
   {
      for (int j = 0; j < 4; ++j)
      {
         proj[i][j] = projectionMatrix[i][j];
      }
   }
   minBox = minBox * proj;
   maxBox = maxBox * proj;
   minBox.x /= minBox.z;
   minBox.y /= minBox.z;
   maxBox.x /= maxBox.z;
   maxBox.y /= maxBox.z;
   
   float maxScreen = float(MAX(MAX(ABS(minBox.x), ABS(maxBox.x)), MAX(ABS(minBox.y), ABS(maxBox.y))));
   // if maxScreen == 1 -> don't zoom
   // > 1 need to zoom out
   // < 1 need to zoom in

   newPos = center;
   newPos -=  viewDirection * centerDist * maxScreen;
   camera.set(newPos, viewDirection, upDirection, camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);


}

CRenderViewMtoARotate::CRenderViewMtoARotate() : CRenderViewRotateManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_centerDist = float(m_center.distanceTo(m_originalPosition));
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   

}


void CRenderViewMtoARotate::MouseDelta(int deltaX, int deltaY)
{
   MStatus status;
   MDagPath camTransform = m_cameraPath;
   camTransform.pop();
   MFnTransform transformPath(camTransform, &status);
   if (status != MS::kSuccess) 
      return;
   

   MVector rightDirection = m_camera.rightDirection(MSpace::kWorld);
   MVector fElevationAxis = -rightDirection;
   fElevationAxis.normalize();

   MPoint previousRp = transformPath.rotatePivot(MSpace::kWorld);
   MVector previousRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   MPoint fPivotPoint = m_center;
   transformPath.setRotatePivot(fPivotPoint, MSpace::kWorld, true);
   MMatrix matrix = camTransform.inclusiveMatrix();
   
   MEulerRotation rot;

   transformPath.getRotation(rot);
   
   rot.incrementalRotateBy (fElevationAxis, .01f * (deltaY));
   
   rot.incrementalRotateBy (MGlobal::upAxis(), - .01f * (deltaX));

   
   transformPath.setRotation(rot);

   transformPath.setRotatePivot (previousRp, MSpace::kWorld, true);
   MVector nextRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   transformPath.translateBy(nextRt - previousRt, MSpace::kWorld);
   transformPath.setRotatePivotTranslation(previousRt, MSpace::kWorld);
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);


}
