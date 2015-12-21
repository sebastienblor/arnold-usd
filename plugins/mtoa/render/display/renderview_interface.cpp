
#include "renderview_interface.h"
#include "renderview.h"

 
void CRenderViewInterface::OpenRenderView(int width, int height, QWidget *parent)
{
   if (m_mainWindow)
   {
      // If the RenderView has already been created
      // let's just show it and re-render
      CRenderView &renderView = m_mainWindow->GetRenderView();
      if (AiRendering()) renderView.InterruptRender();

      renderView.InitRender(width, height);
      renderView.Show();
      renderView.UpdateRender();

   } else 
   {
      CRenderView *renderView = new CRenderView(width, height);
      m_mainWindow = renderView->GetMainWindow();
   }
   
}

void CRenderViewInterface::DestroyRenderView()
{
   if (m_mainWindow == NULL) return;

   CRenderView *renderView = &(m_mainWindow->GetRenderView());
   delete renderView;
   m_mainWindow = NULL;
}

void CRenderViewInterface::InterruptRender()
{
   if (m_mainWindow == NULL) return;
   m_mainWindow->GetRenderView().InterruptRender();
}

void CRenderViewInterface::ObjectNameChanged(const std::string &oldName, const std::string &newName)
{
   if (m_mainWindow == NULL) return;
   m_mainWindow->GetRenderView().ObjectNameChanged(oldName, newName);
}

void CRenderViewInterface::CloseRenderView()
{
   if (m_mainWindow == NULL) return;
   CRenderView &renderView = m_mainWindow->GetRenderView();

   renderView.FinishRender();
   renderView.Close();
}

void CRenderViewInterface::Render()
{
   if (m_mainWindow == NULL) return;
   m_mainWindow->GetRenderView().Render();
}

void CRenderViewInterface::SceneChanged()
{
   // to be implemented  

}

void CRenderViewInterface::SelectionChanged(const std::vector<AtNode *> &selection)
{
   // to be implemented

}
