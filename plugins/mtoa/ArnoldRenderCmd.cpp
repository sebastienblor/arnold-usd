
#include "ArnoldRenderCmd.h"
#include "RenderInstance.h"

#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("cam", "camera", MSyntax::MArgType::kString);

   return syntax;
}

MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderInstance* renderInstance = CRenderInstance::GetInstance();
   MArgDatabase args(syntax(), argList);

   if (!args.isFlagSet("camera"))
   {
      return MS::kFailure;
   }

   renderInstance->Init();

   m_renderOptions.GetRenderOptions(&m_scene);

   renderInstance->SetGamma(m_renderOptions.outputGamma());

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
