#pragma once

#include "ArnoldSession.h"
#include "../render/display/renderview_mtoa.h"

class DLLEXPORT CArnoldRenderViewSession : public CArnoldSession
{

public:

   CArnoldRenderViewSession(bool viewport = false);
   virtual ~CArnoldRenderViewSession();
   
   //void Export(MSelectionList* selected = NULL) override 

   void CloseRenderView();
   void SetRenderViewOption(const MString &option, const MString &value);
   MString GetRenderViewOption(const MString &option);
   
   bool IsActive() const {return m_active;}
   void OpenRenderView();
   void SetCamerasList();
   void SetStatus(MString status) override;
   AtNode* ExportOptions() override;
   void UpdateSessionOptions() override;
   void ChangeCurrentFrame(MTime time, bool forceViewport) override;
   MStatus ExportCameras(MSelectionList* selected = NULL) override;
   void Render(bool ipr = true);
   void RequestUpdate() override;
   void ObjectNameChanged(MObject &node, const MString &str) override;
   void InterruptRender();
   void RequestUpdateImagers(bool listChanged) override;
   static void CloseOtherViews(const MString& destination);
   //static void RenderViewPlayblast(bool state, void * data);
   void SetPlayblasting(bool b) {m_isPlayblasting = b;}

   void Clear() override;
   AtRenderSession *GetRenderSession() override;
   
   CRenderViewMtoA &GetRenderView();

   static const std::string &GetRenderViewSessionId();
   static const std::string &GetViewportSessionId();
   
protected:
   void NewNode(MObject &node) override;
   bool m_viewport;


   bool m_active;
   bool m_isPlayblasting;
   bool m_optionsExported;
}; 
