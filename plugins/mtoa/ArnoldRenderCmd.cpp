
#include "ArnoldRenderCmd.h"
#include "render/RenderSession.h"

#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

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

   if (!args.isFlagSet("camera"))
   {
      return MS::kFailure;
   }

   renderSession->Reset();

   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;

   MString camera = args.flagArgumentString("camera", 0);

   renderSession->SetWidth(width);
   renderSession->SetHeight(height);
   renderSession->SetCamera(camera);

   const CRenderOptions* renderOptions = renderSession->RenderOptions();

   if (!renderOptions->BatchMode())
   {
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
   }
   else
   {
      renderSession->DoRender();
   }

   renderSession->End();

   return status;
}
