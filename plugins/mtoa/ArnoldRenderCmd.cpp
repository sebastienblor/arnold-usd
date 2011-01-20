
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
#include <maya/MRenderUtil.h>

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

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   const bool batch = args.isFlagSet("batch") ? true : false;
   renderSession->SetBatch(batch);

   // no camera set on interactive mode, abort
   if (!args.isFlagSet("camera") && !batch)
   {
      return MS::kFailure;
   }

   // Note: Maya seems to internally calls the preRender preLayerRender scripts
   //       as well as the postRender and postLayerRender ones
   
   // Check if in batch mode
   if (batch)
   {
      // TODO: This really needs to go. We're translating the whole scene for a couple of
      // render options.
      
      AtFloat startframe;
      AtFloat endframe;
      AtFloat byframestep;

      if (renderGlobals.isAnimated())
      {
         startframe = static_cast<float>(renderGlobals.frameStart.as(MTime::uiUnit()));
         endframe = static_cast<float>(renderGlobals.frameEnd.as(MTime::uiUnit()));
         byframestep = renderGlobals.frameBy;
      }
      else
      {
         startframe  = 0;
         endframe    = 0;
         byframestep = 1;
      }

      for (AtFloat framerender = startframe; framerender <= endframe; framerender += byframestep)
      {
         const CRenderOptions* renderOptions = renderSession->RenderOptions();
         if (renderOptions->isAnimated()) MGlobal::viewFrame((double)framerender);
         renderSession->ExecuteScript(renderGlobals.preRenderMel);

         renderSession->Translate();

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

         renderSession->Finish();
         renderSession->ExecuteScript(renderGlobals.postRenderMel);
      }
   }

   // or interactive mode
   else
   {
      int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
      int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
      MString camera = args.flagArgumentString("camera", 0);

      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      renderSession->Finish();                        // In case we're already rendering (e.g. IPR).
      renderSession->Translate();                     // Translate the scene from Maya.
      renderSession->SetCamera(camera);
      renderSession->SetResolution(width, height);
      renderSession->DoInteractiveRender();           // Start the render.
      renderSession->Finish();                        // Clean up.

      renderSession->ExecuteScript(renderGlobals.postRenderMel);
   }


   return status;
}
