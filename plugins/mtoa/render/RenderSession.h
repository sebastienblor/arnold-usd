#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include "render/RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <maya/MGlobal.h>

#include <ai_nodes.h>
#include <ai_universe.h>


class CMayaScene;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();
   CMayaScene* GetMayaScene();

   void Init(ExportMode exportMode=MTOA_EXPORT_ALL, bool preMel=false, bool preLayerMel=false, bool preFrameMel=false);
   void End(bool postMel=false, bool postLayerMel=false, bool postFrameMel=false);

   void Reset(bool postMel=false, bool postLayerMel=false, bool postFrameMel=false, bool preMel=false, bool preLayerMel=false, bool preFrameMel=false);

   void SetBatch(bool batch);
   void SetWidth(int width);
   void SetHeight(int height);
   void SetCamera(MString cameraNode);
   void SetMultiCameraRender(bool multi);

   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   bool IsActive() const
   {
      return (AiUniverseIsActive() == TRUE);
   }

   void ExecuteScript(const MString &str, bool echo=false)
   {
      if (str.length() > 0)
      {
         MGlobal::executeCommand(str, echo);
      }
   }

   void DoRender();
   void DoBatchRender();
   void DoExport(MString customFileName = "", ExportMode exportMode=MTOA_EXPORT_ALL);

private:

   CRenderSession()
   {
   }

   void SetupRenderOutput();

private:

   CRenderOptions m_renderOptions;
   CMayaScene*    m_scene;

}; // class CRenderSession

#endif // RENDER_SESSION_H
