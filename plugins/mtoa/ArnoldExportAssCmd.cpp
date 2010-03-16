
#include "ArnoldExportAssCmd.h"
#include "render/RenderSession.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;

   return syntax;
}

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderSession* renderSession = CRenderSession::GetInstance();

   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: Cannot export to .ass while rendering.");
      
      return MS::kFailure;
   }

   renderSession->Init();

   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      MDagPath cameraPath;
      M3dView::active3dView().getCamera(cameraPath);

      MFnDagNode cameraNode(cameraPath.node());
      renderSession->SetCamera(cameraNode.name());
   }

   renderSession->DoExport();

   renderSession->End();

   return status;
}
