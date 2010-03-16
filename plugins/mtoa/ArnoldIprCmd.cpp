
#include "ArnoldIprCmd.h"
#include "render/RenderSession.h"

#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>

MSyntax CArnoldIprCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);

   return syntax;
}

MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{
   // IPR only works in interactive GUI mode
   if (!MRenderView::doesRenderEditorExist())
      return MS::kFailure;

   CRenderSession* renderSession = CRenderSession::GetInstance();

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

   printf("MODE = %s\n", mode);

   if (mode == "start")
   {
      if (!renderSession->IsActive())
      {
         m_renderOptions.GetRenderOptions(&m_scene);

         renderSession->SetRenderOptions(&m_renderOptions);
         renderSession->Init();

         status = m_scene.ExportToArnold();

         MDagPath cameraPath;
         M3dView::active3dView().getCamera(cameraPath);
         MRenderView::setCurrentCamera(cameraPath);

         MFnDagNode cameraNode(cameraPath.node());
         AiNodeSetPtr(AiUniverseGetOptions(), "camera", AiNodeLookUpByName(cameraNode.name().asChar()));
      }
   }
   else if (mode == "stop")
   {
      renderSession->End();
   }
   else if (mode == "render")
   {
   }
   else if (mode == "refresh")
   {
      m_renderOptions.GetRenderOptions(&m_scene);

      //status = m_useRenderRegion ? MRenderView::startRegionRender(m_width, m_height, m_minx, m_maxx, m_miny, m_maxy, !m_clearBeforeRender, true)
      //                           : MRenderView::startRender(m_width, m_height, !m_clearBeforeRender, true);
      status = MRenderView::startRender(m_renderOptions.width(), m_renderOptions.height(), !m_renderOptions.clearBeforeRender(), true);

      if ( status == MS::kSuccess)
      {
         renderSession->DoRender();
         MRenderView::endRender();
      }
   }
   else if (mode == "pause")
   {
   }
   else if (mode == "unpause")
   {
   }
   else if (mode == "region")
   {
   }

   return status;
}
