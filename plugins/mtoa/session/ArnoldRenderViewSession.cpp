#include "ArnoldRenderViewSession.h"
#include <maya/MGlobal.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"

static CRenderViewMtoA *s_renderView = nullptr;
static CRenderViewMtoA *s_optionsView = nullptr;

CArnoldRenderViewSession::CArnoldRenderViewSession(bool viewport) : 
                           CArnoldSession(), 
                           m_viewport(viewport),
                           m_active(false),
                           m_isPlayblasting(false)
{
   AddUpdateCallbacks();
   if (viewport)
      m_sessionOptions.SetExportOverscan(true);
}
CArnoldRenderViewSession::~CArnoldRenderViewSession()
{
   // We're about to delete the current universe, before that we
   // want the renderview to clear its render session
   GetRenderView().FinishRender();
   GetRenderView().SetUniverse(nullptr);
}
void CArnoldRenderViewSession::SetStatus(MString status)
{
   GetRenderView().SetStatusInfo(status.asChar());
}

void CArnoldRenderViewSession::CloseRenderView()
{
   GetRenderView().CloseRenderView();
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
   std::string res(GetRenderView().GetOption(option.asChar()));
   return MString(res.c_str());
}


void CArnoldRenderViewSession::OpenRenderView()
{
   int width = 1024;
   int height = 1024;
   // FIXME !!! at this stage the options were not translated, so we might not know yet what the resolution is.
   // Should we check the render globals ?
   if (!m_sessionOptions.GetResolution(width, height))
   {
      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);
      width = renderGlobals.width;
      height = renderGlobals.height;
   }
   
   GetRenderView().SetUniverse(m_universe);
   GetRenderView().OpenMtoARenderView(width, height);
   GetRenderView().SetFrame((float)m_sessionOptions.GetExportFrame());
   GetRenderView().SetStatusInfo("");
   GetRenderView().SetLogging(m_sessionOptions.GetLogConsoleVerbosity(), m_sessionOptions.GetLogFileVerbosity());
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
   GetRenderView().SetFrame((float)m_sessionOptions.GetExportFrame());
   GetRenderView().SetStatusInfo("");
   GetRenderView().SetLogging(m_sessionOptions.GetLogConsoleVerbosity(), m_sessionOptions.GetLogFileVerbosity());
   return options;
}
void CArnoldRenderViewSession::InterruptRender()
{
   GetRenderView().InterruptRender();
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
   AiRenderSetHintBool(GetRenderView().GetRenderSession(), request_imager_update, true);
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

void CArnoldRenderViewSession::Clear()
{
   // First ensure the renderview cleared its own render session
   GetRenderView().FinishRender();
   GetRenderView().SetUniverse(nullptr);
   CArnoldSession::Clear();
   GetRenderView().SetUniverse(m_universe);
   GetRenderView().RequestFullSceneUpdate();
}
void CArnoldRenderViewSession::CloseOtherViews(const MString& destination)
{

   bool viewFound = false;
   M3dView thisView;

   if (destination.length() > 0)
      viewFound = (M3dView::getM3dViewFromModelPanel(destination, thisView) == MStatus::kSuccess);

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