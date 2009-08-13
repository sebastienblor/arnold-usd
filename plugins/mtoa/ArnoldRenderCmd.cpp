
#include "ArnoldRenderCmd.h"
#include "RenderInstance.h"

#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MRenderView.h>

MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderInstance* renderInstance = CRenderInstance::GetInstance();

   renderInstance->Init();

   m_renderOptions.GetRenderOptions(&m_scene);

   renderInstance->SetGamma(m_renderOptions.outputGamma());

   status = m_scene.ExportToArnold();

   if (MRenderView::doesRenderEditorExist())
   {
      MDagPath cameraPath;
      M3dView::active3dView().getCamera(cameraPath);
      MRenderView::setCurrentCamera(cameraPath);

      MFnDagNode cameraNode(cameraPath.node());
      AiNodeSetPtr(AiUniverseGetOptions(), "camera", AiNodeLookUpByName(cameraNode.name().asChar()));
   }

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
         renderInstance->DoRender();

         MRenderView::endRender();
      }
   }
   else
   {
      renderInstance->DoRender();
   }

   renderInstance->End();

   return status;
}
