
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

   m_renderOptions.GetRenderOptions(&m_scene);
   
   if (args.isFlagSet("width"))
   {
      m_renderOptions.SetWidth(args.flagArgumentInt("width", 0));
   }

   if (args.isFlagSet("height"))
   {
      m_renderOptions.SetHeight(args.flagArgumentInt("height", 0));
   }

   renderSession->SetRenderOptions(&m_renderOptions);
   renderSession->Init();

   status = m_scene.ExportToArnold();

   MString cameraName = args.flagArgumentString("camera", 0);
   
   AtNode* camera = AiNodeLookUpByName(cameraName.asChar());

   if (!camera)
   {
      cameraName += "Shape";
      camera = AiNodeLookUpByName(cameraName.asChar());
   }

   if (!camera)
      return MS::kFailure;

   AiNodeSetPtr(AiUniverseGetOptions(), "camera", camera);

   if (MRenderView::doesRenderEditorExist())
   {
      status = m_renderOptions.useRenderRegion() ? MRenderView::startRegionRender(m_renderOptions.width(),
                                                                                  m_renderOptions.height(),
                                                                                  m_renderOptions.minX(),
                                                                                  m_renderOptions.maxX(),
                                                                                  m_renderOptions.minY(),
                                                                                  m_renderOptions.maxY(),
                                                                                  !m_renderOptions.clearBeforeRender(),
                                                                                  true)
                                                 : MRenderView::startRender(m_renderOptions.width(),
                                                                            m_renderOptions.height(),
                                                                            !m_renderOptions.clearBeforeRender(),
                                                                            true);

      if ( status == MS::kSuccess)
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
