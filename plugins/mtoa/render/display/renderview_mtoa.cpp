
#include "renderview_mtoa.h"
#include "scene/MayaScene.h"


//#include <maya/MQtUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MGlobal.h>

#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnTransform.h>
#include <maya/MRenderUtil.h>
#include <maya/MQtUtil.h>
#include <maya/M3dView.h>
#include <maya/MDagPathArray.h>
#include <maya/MNodeMessage.h>

#include <maya/MSceneMessage.h>

CRenderViewMtoA::CRenderViewMtoA() : CRenderViewInterface(),
   m_rvSelectionCb(0),
   m_rvSceneSaveCb(0),
   m_rvSceneOpenCb(0),
   m_rvLayerManagerChangeCb(0),
   m_rvLayerChangeCb(0),
   m_rvColorMgtCb(0),
   m_convertOptionsParam(true)
{   
}

CRenderViewMtoA::~CRenderViewMtoA()
{
   if (m_rvSceneSaveCb)
   {
      MMessage::removeCallback(m_rvSceneSaveCb);
      m_rvSceneSaveCb = 0;
   }
   if (m_rvSceneOpenCb)
   {
      MMessage::removeCallback(m_rvSceneOpenCb);
      m_rvSceneOpenCb = 0;
   }
   if (m_rvSelectionCb)
   {
      MMessage::removeCallback(m_rvSelectionCb);
      m_rvSelectionCb = 0;
   }
   if (m_rvLayerManagerChangeCb)
   {
      MMessage::removeCallback(m_rvLayerManagerChangeCb);
      m_rvLayerManagerChangeCb = 0;
   }
   if (m_rvLayerChangeCb)
   {
      MMessage::removeCallback(m_rvLayerChangeCb);
      m_rvLayerChangeCb = 0;
   }
   if (m_rvColorMgtCb)
   {
      MMessage::removeCallback(m_rvColorMgtCb);
      m_rvColorMgtCb = 0;
   }
}
// Return all renderable cameras
static int GetRenderCamerasList(MDagPathArray &cameras)
{

   M3dView view;
   MDagPath activeCameraPath;
   MStatus viewStatus;
   view = M3dView::active3dView(&viewStatus);
   if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
   {
      cameras.append(activeCameraPath);
      return 1;
   }

   MItDag dagIter(MItDag::kDepthFirst, MFn::kCamera);
   MDagPath cameraPath;
   // MFnCamera cameraNode;
   MFnDagNode cameraNode;
   MPlug renderable;
   MStatus stat;
   while (!dagIter.isDone())
   {
      dagIter.getPath(cameraPath);
      cameraNode.setObject(cameraPath);
      renderable = cameraNode.findPlug("renderable", false, &stat);
      if (stat && renderable.asBool())
      {
         cameras.append(cameraPath);
      }
      dagIter.next();
   }
   int size = cameras.length();
   if (size > 1)
      MGlobal::displayWarning("More than one renderable camera. (use the -cam/-camera option to override)");
   else if (!size)
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");
   return size;
}

void CRenderViewMtoA::OpenMtoARenderView(int width, int height)
{
   // Check if attribute ARV_options exist
   // if it doesn't create it
   int exists = 0;
   MGlobal::executeCommand("attributeExists \"ARV_options\" \"defaultArnoldRenderOptions\" ", exists);

   if (!exists)
   {
      MGlobal::executeCommand("addAttr -ln \"ARV_options\"  -dt \"string\"  defaultArnoldRenderOptions");
   } 

   OpenRenderView(width, height, MQtUtil::mainWindow());

   if (exists && m_convertOptionsParam)
   {
      // assign the ARV_options parameter as it is the first time since I opened this scene
      MString optParam;
      MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.ARV_options\"", optParam);
      SetFromSerialized(optParam.asChar());
   }
   MStatus status;   
   if (m_rvSceneSaveCb == 0)
   {
      m_rvSceneSaveCb = MSceneMessage::addCallback(MSceneMessage::kBeforeSave, CRenderViewMtoA::SceneSaveCallback, (void*)this, &status);
   }
   if (m_rvSceneOpenCb == 0)
   {
      m_rvSceneOpenCb = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, CRenderViewMtoA::SceneOpenCallback, (void*)this, &status);
   }
   if (m_rvLayerManagerChangeCb == 0)
   {
      m_rvLayerManagerChangeCb = MEventMessage::addEventCallback("renderLayerManagerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }
   if (m_rvLayerChangeCb == 0)
   {
      m_rvLayerManagerChangeCb =  MEventMessage::addEventCallback("renderLayerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }

   MSelectionList activeList;
   activeList.add(MString(":defaultColorMgtGlobals"));
   
   // get the maya node contraining the color management options         
   if(activeList.length() > 0)
   {
      MObject colorMgtObject;
      activeList.getDependNode(0,colorMgtObject);

      if (m_rvColorMgtCb == 0)
      {
         m_rvColorMgtCb = MNodeMessage::addNodeDirtyCallback(colorMgtObject,
                                              ColorMgtCallback,
                                              this,
                                              &status);
      }

      if (m_convertOptionsParam) UpdateColorManagement(colorMgtObject);
   }
   m_convertOptionsParam = false;

   // Set image Dir
   MString workspace;
   status = MGlobal::executeCommand(MString("workspace -q -rd;"), workspace);
   if (status == MS::kSuccess)
   {
      workspace += "/images";
      SetDefaultImageDirectory(workspace.asChar());
   }
   MString tmpDir;
   status = MGlobal::executeCommand(MString("getenv \"TMPDIR\";"), tmpDir);
   if (status == MS::kSuccess)
   {
      SetTempDirectory(tmpDir.asChar());
   }

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {
      CRenderOptions *renderOptions = renderSession->RenderOptions();
      int consoleLogging = renderOptions->GetLogConsoleVerbosity();
      int fileLogging = renderOptions->GetLogFileVerbosity();
      SetLogging(consoleLogging, fileLogging);
   }
}
/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/

void CRenderViewMtoA::UpdateSceneChanges()
{
   if (AiUniverseIsActive())
   {
      CMayaScene::UpdateSceneChanges();
      SetFrame((float)CMayaScene::GetArnoldSession()->GetExportFrame());
      return;
   }

   // Universe isn't active, oh my....
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {   
      renderSession->SetRendering(false);
      CMayaScene::End();
   }
   // Re-export everything !
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);


   MDagPathArray cameras;
   GetRenderCamerasList(cameras);
   CMayaScene::ExecuteScript(renderGlobals.preMel);
   CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

   CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);

   if (!renderGlobals.renderAll)
   {
      MSelectionList selected;
      MGlobal::getActiveSelectionList(selected);
      CMayaScene::Export(&selected);
   }
   else
   {
      CMayaScene::Export();
   }

   // SetExportCamera mus be called AFTER CMayaScene::Export
   CMayaScene::GetArnoldSession()->SetExportCamera(cameras[0]);

   // Set resolution and camera as passed in.
   CMayaScene::GetRenderSession()->SetResolution(-1, -1);
   CMayaScene::GetRenderSession()->SetCamera(cameras[0]);


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

void CRenderViewMtoA::SceneSaveCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;

   const char *serialized = renderViewMtoA->Serialize();
   
   MString command = "setAttr -type \"string\" \"defaultArnoldRenderOptions.ARV_options\" \"";
   command += serialized;
   command +="\"";

   MGlobal::executeCommand(command);

}

void CRenderViewMtoA::SceneOpenCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   renderViewMtoA->m_convertOptionsParam = true;
   // next time I open the RenderView, convert the ARV_options param
}

void CRenderViewMtoA::RenderLayerChangedCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   renderViewMtoA->SetOption("Update Full Scene", "1");


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
   CArnoldSession *session = CMayaScene::GetArnoldSession();
   if (append)
   {
      if (selectionCount == 0) return;
      for (unsigned int i = 0; i < selectionCount; ++i)
      {
         MGlobal::selectByName(session->GetMayaObjectName(selectedNodes[i]), MGlobal::kAddToList);
      }

   } else 
   {
      if (selectionCount == 0) 
      {
         MGlobal::clearSelectionList();
         return;
      }
      
      MGlobal::selectByName(session->GetMayaObjectName(selectedNodes[0]), MGlobal::kReplaceList);
      for (unsigned int i = 1; i < selectionCount; ++i)
      {
         MGlobal::selectByName(session->GetMayaObjectName(selectedNodes[i]), MGlobal::kAddToList);
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
   if ((!receive) && m_rvSelectionCb)
   {
      MMessage::removeCallback(m_rvSelectionCb);
      m_rvSelectionCb = 0;
      return;
   }

   if (receive && (m_rvSelectionCb == 0))
   {

      m_rvSelectionCb = MEventMessage::addEventCallback("SelectionChanged",
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
   MMessage::removeCallback(m_rvSceneSaveCb);
   m_rvSceneSaveCb = 0;
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


void CRenderViewMtoA::UpdateColorManagement(MObject &node)
{
   MFnDependencyNode depNode(node);

// cfe -> ocio enabled
// cfp -> ocio path
// vtn  -> view transform name
// wsn  -> 
// otn
// potn 

   MStatus status;
   MPlug plug;
   plug = depNode.findPlug("cfe", &status);
   bool ocio = false;
   if (status == MS::kSuccess && plug.asBool())
   {
      ocio = true;
      SetOption("LUT.Tonemap", "OCIO");
   }
   
   plug = depNode.findPlug("cfp", &status);
   
   if (status == MS::kSuccess)
   {      
      std::string ocioFile = plug.asString().asChar();
      if (!ocioFile.empty())
      {
         SetOption("LUT.OCIO File", ocioFile.c_str());
      }

      if (ocio)
      {
         plug = depNode.findPlug("vtn", &status);
         if (status == MS::kSuccess)
         {
            const std::string viewTransform = plug.asString().asChar();
            size_t sep = viewTransform.find(" (");
            if (sep != std::string::npos)
            {
               std::string viewName = viewTransform.substr(0, sep);
               
               size_t lastPos = viewTransform.find(")", sep+2);
               if (lastPos != std::string::npos)
               {
                  std::string displayName = viewTransform.substr(sep + 2, lastPos - sep - 2);
                  SetOption("LUT.Display", displayName.c_str());
                  SetOption("LUT.View", viewName.c_str());

               }
            }
         }
      } else
      {
         plug = depNode.findPlug("vtn", &status);
         if (status == MS::kSuccess)
         {            
            const std::string viewTransform = plug.asString().asChar();
            if (viewTransform == "1.8 gamma")
            {
               SetOption("LUT.Tonemap", "Linear"); 
               SetOption("LUT.Gamma", "1.8"); 
               SetOption("LUT.Exposure", "0");
            } else if (viewTransform == "2.2 gamma")
            {
               SetOption("LUT.Tonemap", "Linear"); 
               SetOption("LUT.Gamma", "2.2"); 
               SetOption("LUT.Exposure", "0");
            } else if (viewTransform == "sRGB gamma")
            {
               SetOption("LUT.Tonemap", "sRGB");
               SetOption("LUT.Gamma", "1"); 
               SetOption("LUT.Exposure", "0");
            } else if (viewTransform == "Rec 709 gamma")
            {
               SetOption("LUT.Tonemap", "Rec709");
               SetOption("LUT.Gamma", "1"); 
               SetOption("LUT.Exposure", "0");
            } else if (viewTransform == "Raw")
            {
               SetOption("LUT.Tonemap", "Raw");
               SetOption("LUT.Gamma", "1"); 
               SetOption("LUT.Exposure", "0");
            } else if (viewTransform == "Log")
            {
               SetOption("LUT.Tonemap", "Log");
               SetOption("LUT.Gamma", "1");
               SetOption("LUT.Exposure", "0");
            }
         }
      }
   }
}
void CRenderViewMtoA::ColorMgtCallback(MObject& node, MPlug& plug, void* clientData)
{
   CRenderViewMtoA *rvMtoA = (CRenderViewMtoA *)clientData;
   rvMtoA->UpdateColorManagement(node);

}
