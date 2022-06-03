#include "ArnoldRenderViewSession.h"
#include "SessionManager.h"
#include <maya/MGlobal.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"
#include "utils/ConstantStrings.h"

#include <string>

static CRenderViewMtoA *s_renderView = nullptr;
static CRenderViewMtoA *s_optionsView = nullptr;

static const std::string s_renderViewSessionId = "arnoldRenderView";
static const std::string s_arnoldViewportSessionId = "arnoldViewport";

CArnoldRenderViewSession::CArnoldRenderViewSession(bool viewport) : 
                           CArnoldSession(), 
                           m_viewport(viewport),
                           m_active(false),
                           m_isPlayblasting(false),
                           m_optionsExported(false)
{
   AddUpdateCallbacks();
   m_sessionOptions.SetExportResolutionOverscan(false); // MTOA-942 disable overscan for ARV otherwise this causes a crash (ARNOLD-11990)
   if (viewport)
   {
      m_sessionOptions.SetExportResolutionOverscan(true); 
      m_sessionOptions.SetExportCameraOverscan(true);
      
      // We need to ensure motion blur is disabled for AVP, otherwise
      // changing the current frame could force a new render   
      m_sessionOptions.DisableMotionBlur();
   }
}
CArnoldRenderViewSession::~CArnoldRenderViewSession()
{
   CRenderViewMtoA &renderview = GetRenderView();
   // We're about to delete the current universe, before that we
   // want the renderview to clear its render session
   renderview.FinishRender(true);
   renderview.SetUniverse(nullptr);
   renderview.SetSession(nullptr);
   renderview.RequestFullSceneUpdate();
}
void CArnoldRenderViewSession::SetStatus(MString status)
{
   GetRenderView().SetStatusInfo(status.asChar());
}

void CArnoldRenderViewSession::CloseRenderView()
{
   GetRenderView().CloseRenderView();
   if (!m_viewport)
      CSessionManager::DeleteActiveSession(s_renderViewSessionId);

}

void CArnoldRenderViewSession::SetRenderViewOption(const MString &option, const MString &value)
{
   GetRenderView().SetOption(option.asChar(), value.asChar());
}

void CArnoldRenderViewSession::RequestUpdate()
{
   CArnoldSession::RequestUpdate();
   if (m_isPlayblasting)
   {
      GetRenderView().SetOption("Wait Render", "1");
      // why this ? SetProgressive(false);
   }
   GetRenderView().SceneChanged();


   /* Code from RenderSession::UpdateRenderView
         if (is_playblasting)
      {
         s_renderView->SetOption("Wait Render", "1");
         SetProgressive(false);
      }
      else
      {
         SetProgressive(true);
      }
      // This will tell the render View that the scene has changed
      // it will decide whether to re-render or not
      s_renderView->SceneChanged();

      */


}

void CArnoldRenderViewSession::ObjectNameChanged(MObject &node, const MString &str)
{
   // Need to tell ARV that an object name has changed
   MFnDependencyNode fnNode(node);
   const char *newName = fnNode.name().asChar();
   const char *oldName = str.asChar();
   GetRenderView().ObjectNameChanged(newName, oldName);
}

MString CArnoldRenderViewSession::GetRenderViewOption(const MString &option)
{
   const char *resChar = GetRenderView().GetOption(option.asChar());
   if (resChar == nullptr)
      return MString();

   return MString(resChar);
}

// Some additional options data need to be setup for ARV
static void FillRenderViewOptions(CRenderViewMtoA &renderview, CSessionOptions &sessionOptions)
{
   renderview.SetFrame((float)sessionOptions.GetExportFrame());
   renderview.SetStatusInfo("");
   AtUniverse *universe = renderview.GetUniverse();
   if (universe)
      sessionOptions.SetupLog(universe);
   
   renderview.SetLogging(sessionOptions.GetLogConsoleVerbosity(), 
                         sessionOptions.GetLogFileVerbosity());
   
}

void CArnoldRenderViewSession::OpenRenderView()
{
   int width = 1024;
   int height = 1024;
   if (!m_sessionOptions.GetResolution(width, height))
   {
      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);
      width = renderGlobals.width;
      height = renderGlobals.height;
   }

   if (!m_optionsExported)
   {
      // If the options have not been exported yet, xres and yres will be left to their default value.
      // We want at least to set the proper width/height resolution, so that it shows properly in 
      // the renderview buffer MTOA-744
      AtNode *options= AiUniverseGetOptions(m_universe);
      AiNodeSetInt(options, str::xres, width);
      AiNodeSetInt(options, str::yres, height);
   }
   
   GetRenderView().SetUniverse(m_universe);
   GetRenderView().OpenMtoARenderView(width, height);

   FillRenderViewOptions(GetRenderView(), m_sessionOptions);
}

void CArnoldRenderViewSession::SetCamerasList()
{
   MString camerasList;

   MDagPathArray cameras = GetRenderCameras(true); // true means include the viewport cam
   
   bool foundPersp = false;
   // In the renderview menu, we want the partial names, not the final arnold name
   for (unsigned int i = 0; i < cameras.length(); ++i)
   {
      MString camName = cameras[i].partialPathName();
      if (camerasList.length() > 0)
         camerasList += ";";
      camerasList += camName;

      if (camName == "perspShape")
         foundPersp = true;
   }
   // always add persp camera
   if (!foundPersp)
   {
      if (camerasList.length() > 0)
         camerasList += ";";
      camerasList += "perspShape";
   }
   GetRenderView().SetOption("Cameras", camerasList.asChar());
}

MStatus CArnoldRenderViewSession::ExportCameras(MSelectionList* selected)
{
   // Force the export of default persp camera for ARV (#3655)
   if (selected == nullptr)
   {
      MSelectionList list;
      list.add("perspShape");
      MDagPath perspPath;

      if (list.length() > 0 && list.getDagPath(0, perspPath) == MStatus::kSuccess)
         ExportDagPath(perspPath);

   }
   return CArnoldSession::ExportCameras(selected);
}

AtNode* CArnoldRenderViewSession::ExportOptions()
{
   AtNode *options = CArnoldSession::ExportOptions();
   m_optionsExported = true;

   FillRenderViewOptions(GetRenderView(), m_sessionOptions);
   return options;
}


// This function will be called every time the options node was modified during IPR
void CArnoldRenderViewSession::UpdateSessionOptions()
{
   // First call the parent class that will update the sessionOptions
   CArnoldSession::UpdateSessionOptions();
   // Then, ensure we provide the correct data to ARV
   FillRenderViewOptions(GetRenderView(), m_sessionOptions);

   // We need to ensure motion blur is disabled for AVP, otherwise
   // changing the current frame could force a new render
   if (m_viewport)
      m_sessionOptions.DisableMotionBlur();
   
}
void CArnoldRenderViewSession::ChangeCurrentFrame(double time, bool forceViewport)
{
   // Don't do anything if we're in an AVP session, we don't want to 
   // change the current frame as it could recursively update a viewport render
   if (!m_viewport)
      CArnoldSession::ChangeCurrentFrame(time, forceViewport);
}

void CArnoldRenderViewSession::InterruptRender()
{
   GetRenderView().InterruptRender();
}
void CArnoldRenderViewSession::RenderSequence(float start, float end, float step)
{
   m_active = true;
   GetRenderView().InterruptRender();
   GetRenderView().RenderSequence(start, end, step);
}
void CArnoldRenderViewSession::Render(bool ipr)
{
   m_active = true;
   if (ipr)
      GetRenderView().SetOption("Run IPR", "1");
   else
   {
      GetRenderView().InterruptRender();
      GetRenderView().Render();
   }
}
void CArnoldRenderViewSession::RequestUpdateImagers(bool listChanged)
{
   CArnoldSession::RequestUpdateImagers(listChanged);
   // Tell ARV to update the list of imagers
   GetRenderView().SetOption("Update Imagers", listChanged ? "Rewire Imagers" : "1");
   static AtString request_imager_update("request_imager_update");
   AiRenderSetHintBool(GetRenderSession(), request_imager_update, true);
}
void CArnoldRenderViewSession::NewNode(MObject &node)
{
   CArnoldSession::NewNode(node);
   // new cameras shouldn't restart IPR
   if (node.hasFn(MFn::kCamera)) 
   {
      // Add this camera to the renderview list.
      // Note that we want to provide the partial name
      MFnDagNode dagNodeFn(node);
      MDagPath path;
      const MStatus status = dagNodeFn.getPath(path);
      if (status == MS::kSuccess)
         GetRenderView().SetOption("Add Camera", path.partialPathName().asChar());
      return;
   }   
}

// Here we return the render session owned by ARV
AtRenderSession *CArnoldRenderViewSession::GetRenderSession()
{
   return GetRenderView().GetRenderSession();
}
void CArnoldRenderViewSession::Clear()
{
   // First ensure the renderview cleared its own render session
   GetRenderView().FinishRender();
   GetRenderView().SetUniverse(nullptr);
   CArnoldSession::Clear();
   GetRenderView().SetUniverse(m_universe);
   m_optionsExported = false;
   GetRenderView().RequestFullSceneUpdate();
}
void CArnoldRenderViewSession::CloseOtherViews(const MString& destination)
{

   bool viewFound = false;
   M3dView thisView;

   // If we start rendering with ARV, we will call this function with the renderview session name
   // as the destination (as if it was a viewport panel name) . This allows us to distinguish 
   // this use case and know when we need to kill the renderview session or not
   if (destination != MString(s_renderViewSessionId.c_str()))
   {
      CSessionManager::DeleteActiveSession(s_renderViewSessionId);

      if (destination.length() > 0)
         viewFound = (M3dView::getM3dViewFromModelPanel(destination, thisView) == MStatus::kSuccess);
   }
   
   // Close all but the destination render override if there is one
   // If the destination is an empty string it is the ARV.
   for (unsigned int i = 0, viewCount = M3dView::numberOf3dViews(); i < viewCount; ++i)
   {
      M3dView view;
      M3dView::get3dView(i, view);
      if (view.renderOverrideName() == "arnoldViewOverride" &&
               (!viewFound || view.widget() != thisView.widget()))
         view.setRenderOverrideName("");
   }
}
CRenderViewMtoA &CArnoldRenderViewSession::GetRenderView()
{
   if (m_viewport)
   {
      if (s_optionsView == nullptr)
      {
         s_optionsView = new CRenderViewMtoA(this);
         s_optionsView->SetViewportRendering(true);
      } else
         s_optionsView->SetSession(this);
      return *s_optionsView;
   }
   if (s_renderView == nullptr)
   {
      s_renderView = new CRenderViewMtoA(this);
   } else
      s_renderView->SetSession(this);
   return *s_renderView;
}

const std::string &CArnoldRenderViewSession::GetRenderViewSessionId()
{
   return s_renderViewSessionId;
}

const std::string &CArnoldRenderViewSession::GetViewportSessionId()
{
   return s_arnoldViewportSessionId;

}
