
#include "ArnoldExportAssCmd.h"
#include "render/RenderSession.h"
#include "maya_scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("s", "selected");
   syntax.addFlag("f", "filename", MSyntax::kString);
   return syntax;
}

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderSession* renderSession = CRenderSession::GetInstance();

   MArgDatabase argDB(newSyntax(), argList, &status);

   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: Cannot export to .ass while rendering.");
      return MS::kFailure;
   }


   // We don't need renderview_display so we need to set Batch mode on.
   renderSession->SetBatch(true);
   MString customFileName = "";

   // custom filename
   if(argDB.isFlagSet("filename"))
      argDB.getFlagArgument("filename", 0, customFileName);


   // exporet only selected objects
   if(argDB.isFlagSet("selected"))
   {
      renderSession->Init(MTOA_EXPORT_SELECTED, true, true, true);
      if (MGlobal::mayaState() == MGlobal::kInteractive)
         renderSession->DoExport(customFileName, MTOA_EXPORT_SELECTED);
   }

   // export the entire maya scene
   else
   {
      renderSession->Init(MTOA_EXPORT_ALL, true, true, true);

      if (MGlobal::mayaState() == MGlobal::kInteractive)
      {
         MDagPath cameraPath;
         M3dView::active3dView().getCamera(cameraPath);

         MFnDagNode cameraNode(cameraPath.node());
         renderSession->SetCamera(cameraNode.name());
      }
      renderSession->DoExport(customFileName);
   }

   renderSession->End(true, true, true);

   return status;
}
