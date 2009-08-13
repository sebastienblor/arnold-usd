
#include "ArnoldExportAssCmd.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MGlobal.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;

   return syntax;
}

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: Cannot export to .ass while rendering.");
      
      return MS::kFailure;
   }

   AiBegin();

   // TODO: For now, we will use stdout (in Maya, it will go to the output window)
   AiSetLogOptions(NULL, AI_LOG_ALL, 1000, 4);

   m_renderOptions.GetRenderOptions(&m_scene);

   status = m_scene.ExportToArnold();

   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      MDagPath cameraPath;
      M3dView::active3dView().getCamera(cameraPath);

      MFnDagNode cameraNode(cameraPath.node());
      AiNodeSetPtr(AiUniverseGetOptions(), "camera", AiNodeLookUpByName(cameraNode.name().asChar()));
   }

   MString fileName = VerifyFileName(m_renderOptions.outputAssFile(), m_renderOptions.outputAssCompressed());

   if (fileName == "")
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   else
   {
      AiMsgInfo("[mtoa] Exporting Maya scene to file '%s'", fileName.asChar());
      AiASSWrite(fileName.asChar(), AI_NODE_ALL, false);
   }

   AiEnd();

   return status;
}

MString CArnoldExportAssCmd::VerifyFileName(MString fileName, bool compressed)
{
   unsigned int len = fileName.length();

   if (!compressed)
   {
      if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() != ".ass"))
         fileName += ".ass";
   }
   else
   {
      if ((len < 7) || (fileName.substring(len - 7, len - 1).toLowerCase() != ".ass.gz"))
      {
         if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() == ".ass"))
            fileName += ".gz";
         else if ((len < 3) || (fileName.substring(len - 3, len - 1).toLowerCase() == ".gz"))
            fileName = fileName.substring(0, len - 4) + ".ass.gz";
         else
            fileName += ".ass.gz";
      }
   }

   return fileName;
}
