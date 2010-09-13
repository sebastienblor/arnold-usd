
#include "ArnoldRenderCmd.h"
#include "render/RenderSession.h"

#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>
#include <maya/MStringArray.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);

   return syntax;
}

MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderSession* renderSession = CRenderSession::GetInstance();
   MArgDatabase args(syntax(), argList);
   MDagPath dagPath;

   bool batch = args.isFlagSet("batch") ? true : false;

   // no camera set on interactive mode, abort
   if (!args.isFlagSet("camera") && !batch)
   {
      return MS::kFailure;
   }


   renderSession->Reset();
   renderSession->SetBatch(batch);
   const CRenderOptions* renderOptions = renderSession->RenderOptions();

   // Check if in batch mode
   if (batch)
   {

      AtFloat startframe;
      AtFloat endframe;
      AtFloat byframestep;

      if (renderOptions->isAnimated())
      {
         startframe = renderOptions->startFrame();
         endframe = renderOptions->endFrame();
         byframestep = renderOptions->byFrameStep();
      }
      else
      {
         startframe = 0;
         endframe = 0;
         byframestep = 1;
      }

      for (AtFloat framerender = startframe; framerender <= endframe; framerender += byframestep)
      {

         if (renderOptions->isAnimated())
         {
            MGlobal::viewFrame((double)framerender);
         }

         renderSession->Reset();
         renderSession->SetBatch(batch);
         renderSession->RenderOptions();

         MStringArray cameras;
         MItDag  dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

         // get all renderable cameras
         for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
         {
            if (!dagIterCameras.getPath(dagPath))
            {
               AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
               return status;
            }

            MFnDependencyNode camDag(dagIterCameras.item());
            if(camDag.findPlug("renderable").asBool())
            {
               MFnDagNode cameraNode(dagPath);
               cameras.append(cameraNode.name().asChar());
            }
         }

         if (cameras.length()>1)
         {
            renderSession->SetMultiCameraRender(true);
         }

         for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
         {
            renderSession->SetCamera(cameras[arrayIter]);
            renderSession->DoBatchRender();
         }

         renderSession->End();
      
      }
   }

   // or interactive mode
   else
   {
      int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
      int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
      MString camera = args.flagArgumentString("camera", 0);
      renderSession->SetWidth(width);
      renderSession->SetHeight(height);
      renderSession->SetCamera(camera);
      
      // We need to set the current camera in renderView,
      // so the buttons render from the camera you want.
      MSelectionList list;
      MDagPath       cameraDagPath;
      list.add(camera);
      list.getDagPath(0, cameraDagPath);
      MRenderView::setCurrentCamera(cameraDagPath);

      if (renderOptions->useRenderRegion())
      {
         status = MRenderView::startRegionRender(renderOptions->width(),
                                                 renderOptions->height(),
                                                 renderOptions->minX(),
                                                 renderOptions->maxX(),
                                                 renderOptions->minY(),
                                                 renderOptions->maxY(),
                                                 !renderOptions->clearBeforeRender(),
                                                 true);
      }
      else
      {
         status = MRenderView::startRender(renderOptions->width(),
                                           renderOptions->height(),
                                           !renderOptions->clearBeforeRender(),
                                           true);
      }

      if (status == MS::kSuccess)
      {
         renderSession->DoRender();
         MRenderView::endRender();
      }

      renderSession->End();

   }


   return status;
}
