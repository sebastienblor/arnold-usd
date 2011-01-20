
#include "ArnoldIprCmd.h"
#include "render/RenderSession.h"

#include <ai_universe.h>

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderView.h>

#include <vector>


MSyntax CArnoldIprCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("n", "node", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("et", "elapsedTime", MSyntax::kString);

   return syntax;
}

MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{

   // IPR only works in interactive GUI mode
   if (!MRenderView::doesRenderEditorExist())
      return MS::kFailure;

   CRenderSession* renderSession = CRenderSession::GetInstance();
   //renderSession->SetBatch(false);


   MStatus status;
   MArgDatabase args(syntax(), argList);

   // "-mode" flag is not set, so we simply return a bool with the rendering status
   if (!args.isFlagSet("mode"))
   {
      setResult(renderSession->IsActive());
      return MS::kSuccess;
   }

   // Get argument to "-mode" flag
   MString mode = args.flagArgumentString("mode", 0);
   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
   MString camera = args.flagArgumentString("camera", 0);

   AiMsgDebug( "IPR Command mode: %s", mode.asChar() );

   // What mode are we in?
   if (mode == "start")
   {
      if (!renderSession->IsActive())
      {
         MStatus status = renderSession->PrepareIPR();
         if ( MS::kSuccess != status )
         {
            MGlobal::displayError( "Error starting Arnold IPR" );
            renderSession->StopIPR();
            return status;
         }
      }

      // Set stuff passed in:
      renderSession->SetWidth(width);
      renderSession->SetHeight(height);
      renderSession->SetCamera(camera);
      // No need to call render as Maya sends us "unpause" next.

   }

   else if (mode == "stop")
   {
      renderSession->StopIPR();
   }

   else if (mode == "refresh" )
   {
      renderSession->Reset();
      MStatus status = renderSession->PrepareIPR();
      if ( MS::kSuccess != status )
      {
         renderSession->StopIPR();
         MGlobal::displayError( "Error refeshing Arnold IPR" );
         return status;
      }

      // Start off the render.
      renderSession->DoIPRRender();
   }

   else if ( (mode == "region") || (mode == "render") )
   {
      renderSession->Interrupt();

      if ( !renderSession->IsActive() )
      {
         MGlobal::displayError( "Error rendering Arnold IPR, Arnold is not active." );
         return MS::kFailure;
      }

      if ( mode == "region" )
      {
         unsigned int left(-1);
         unsigned int right(-1);
         unsigned int bottom(-1);
         unsigned int top(-1);

         status = MRenderView::getRenderRegion( left, right, bottom, top );
         if ( status == MS::kSuccess)
         {
               renderSession->SetRegion( left, right, bottom, top );
         }
      }
      else
      {
            renderSession->SetWidth(width);
            renderSession->SetHeight(height);
      }

      // Set the render session camera.
      renderSession->SetCamera(camera);

      // Start off the render.
      renderSession->DoIPRRender();
   }

   else if (mode == "redo")
   {
      renderSession->Interrupt();
      // Set the render session camera.
      renderSession->SetCamera(camera);

      if ( !renderSession->IsActive() )
      {
         MGlobal::displayError( "Error starting Arnold IPR" );
         return MS::kFailure;
      }

      MStatus status;
      MString object = args.flagArgumentString("node", 0, &status );
      if ( MStatus::kSuccess == status )
      {
         MSelectionList list;
         MObject mobj;
         list.add(object);
         list.getDependNode(0, mobj);
         MFnDependencyNode dep_fn( mobj );
         renderSession->UpdateIPRScene( mobj );
      }

      // Start off the render.
      renderSession->DoIPRRender();
   }

   else if ( (mode == "pause") )
   {
      renderSession->PauseIPR();
   }

   else if (mode == "unpause")
   {
      if ( !renderSession->IsActive() )
      {
         MGlobal::displayError( "Error starting Arnold IPR" );
         return MS::kFailure;
      }

      // Start off the render.
      renderSession->UnPauseIPR();
   }

   else if ( (mode == "finishedIPR") )
   {
      renderSession->FinishedIPRTuning();

      // Format a bit of info for the renderview.
      const AtUInt64 mem_used( renderSession->GetUsedMemory() );
      MString rvInfo( "renderWindowEditor -edit -pcaption (\"    (Arnold IPR Renderer)\\n" );
      rvInfo += "Memory: ";
      rvInfo += (unsigned int)mem_used;
      rvInfo += "Mb";

      if ( args.isFlagSet( "elapsedTime" ) )
      {
         const MString elapsedTime(  args.flagArgumentString("elapsedTime", 0) );
         rvInfo += "    Render Time: ";
         rvInfo += elapsedTime;
      }

      rvInfo += "    Camera: ";
      rvInfo += renderSession->RenderOptions()->GetCameraName();
      rvInfo += "\") renderView;";

      MGlobal::executeCommandOnIdle( rvInfo, false );
   }

   return status;
}
