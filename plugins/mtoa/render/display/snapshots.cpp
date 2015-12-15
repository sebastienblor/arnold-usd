
#include "renderview.h"
#include "snapshots.h"

#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <deque>

#ifdef _WIN64
#define _WIN32_WINNT    0x501
#define _WIN32_WINDOWS  0x501

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <intrin.h>
#else
#include <sys/time.h>
#endif

#include "utility.h"
#include <emmintrin.h>

#include <iostream>
#include <sstream>
#include <fstream> 
using namespace std;

CRenderViewSnapshots::~CRenderViewSnapshots()
{
   Clear();
}
void CRenderViewSnapshots::Clear()
{
   if (!m_storedSnapshots.empty())
   {
      for (size_t i = 0; i < m_storedSnapshots.size(); ++i)
      {
         delete m_storedSnapshots[i];
      }
      m_storedSnapshots.clear();
   }
}
void CRenderViewSnapshots::Init()
{
   m_scrollArea = new QScrollArea(this);
   m_scrollWidget = new QWidget(this);
   m_scrollArea->setWidget(m_scrollWidget);
   this->setCentralWidget(m_scrollArea);
   m_layout = new QHBoxLayout;
   QMargins margin(0, 0, 0, 0);
   m_layout->setContentsMargins(margin);
   m_scrollWidget->setLayout(m_layout);

   m_scrollWidget->installEventFilter(this);
   m_layout->installEventFilter(this);
   m_scrollArea->installEventFilter(this);

   setFixedSize(parentWidget()->width(), 106);

   m_scrollWidget->resize(0, 90);
   //m_scrollWidget->setFixedSize(parentWidget()->width(), 100);
   m_scrollArea->resize(parentWidget()->width() - 40, 100);

   QDockWidget *dock = new QDockWidget(tr("Comments"), this);
   dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
   dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
   m_commentsEdit = new QTextEdit(dock);
   m_commentsEdit->setText("");
   m_commentsEdit->resize(100, 100);
   dock->setWidget(m_commentsEdit);

   addDockWidget(Qt::RightDockWidgetArea, dock);

}

void CRenderViewSnapshots::AdjustPosition()
{
   QPoint position = parentWidget()->pos();
   m_moving = true;
   move(position.x(), position.y() + parentWidget()->height() + 30);
   setFixedSize( parentWidget()->width(), 106);  
   m_moving = false;
}

void CRenderViewSnapshots::StoreSnapshot(AtRGBA *buffer, int width, int height, const std::string &statusLog, CRenderViewCCSettings &ccSettings)
{
   AtRGBA *storedBuffer = (AtRGBA *)AiMalloc(width * height * sizeof(AtRGBA));
   memcpy(storedBuffer, buffer, width * height * sizeof(AtRGBA));

   StoredSnapshot *snapshot = new StoredSnapshot();
   snapshot->buffer = storedBuffer;
   snapshot->width = width;
   snapshot->height = height;
   snapshot->status = statusLog;

   std::ostringstream snapshotName; 
   snapshotName <<"Snapshot ";
   snapshotName<< int(m_storedSnapshots.size() + 1);
   snapshot->name = snapshotName.str();
   snapshot->button = new SnapshotThumbnail(m_scrollArea, *this, *snapshot);
   snapshot->button->setStyleSheet("QPushButton {  background-color : transparent; color : transparent; }");
   snapshot->button->setAutoFillBackground(true);
   snapshot->nameLabel = new QLabel(snapshot->button);

   snapshot->nameLabel->setText(QString(snapshot->name.c_str()));
   snapshot->nameLabel->move(5, 5);

   QImage imageCopy(width, height, QImage::Format_RGB16);

   AtRGB rgb;
   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++, storedBuffer++)
      {
         rgb = storedBuffer->rgb();

         if (ccSettings.space == RV_COLOR_SPACE_SRGB)
         {
            rgb.r = (rgb.r <= 0.0031308f) ? rgb.r * 12.92f : (1.055) * powf(rgb.r,1.f/2.4f) - 0.055f;
            rgb.g = (rgb.g <= 0.0031308f) ? rgb.g * 12.92f : (1.055) * powf(rgb.g,1.f/2.4f) - 0.055f;
            rgb.b = (rgb.b <= 0.0031308f) ? rgb.b * 12.92f : (1.055) * powf(rgb.b,1.f/2.4f) - 0.055f;

         } else if (ccSettings.space == RV_COLOR_SPACE_REC709)
         {
            rgb.r = (rgb.r >= 0.018f) ? (1.099 * (powf(rgb.r, 0.45f))) - 0.099 : (4.5f * rgb.r);
            rgb.g = (rgb.g >= 0.018f) ? (1.099 * (powf(rgb.g, 0.45f))) - 0.099 : (4.5f * rgb.g);
            rgb.b = (rgb.b >= 0.018f) ? (1.099 * (powf(rgb.b, 0.45f))) - 0.099 : (4.5f * rgb.b);

         } else if (ccSettings.space == RV_COLOR_SPACE_LUT_FILE && ccSettings.lut3d != NULL)
         {
            ccSettings.lut3d->ApplyLUT(rgb);
         }
         if (ccSettings.exposure != 0.f)
         {
            rgb *= ccSettings.exposureFactor;
         }
         if (ccSettings.gamma != 1.0f)
         {
            AiColorClamp(rgb, rgb, 0, 1);
            AiColorGamma(&rgb, ccSettings.gamma);
         }
         rgb.r = CLAMP(rgb.r, 0.f, 1.f);
         rgb.g = CLAMP(rgb.g, 0.f, 1.f);
         rgb.b = CLAMP(rgb.b, 0.f, 1.f);

         imageCopy.setPixel(x, y, qRgb(int(rgb.r * 255), int(rgb.g * 255), int(rgb.b * 255)));
      }
   }
   QImage imageThumbnail = imageCopy.scaledToHeight(72);
   QPixmap thumbnailPixmap = QPixmap::fromImage(imageThumbnail);
   QIcon buttonIcon(thumbnailPixmap);

   int widgetSize = m_scrollWidget->size().width();
   snapshot->button->setIcon(buttonIcon);
   QSize iconSize = thumbnailPixmap.size();
   iconSize.setWidth(iconSize.width());
   iconSize.setHeight(iconSize.height());

   snapshot->button->setIconSize(iconSize);
   widgetSize += snapshot->button->size().width();
   snapshot->button->move(0, 0);
   snapshot->button->resize(iconSize.width(), iconSize.height());

   snapshot->button->show();
   snapshot->comments = m_commentsEdit->toPlainText().toStdString();
   m_layout->addWidget(snapshot->button);

   m_scrollWidget->resize(widgetSize, m_scrollWidget->size().height());
   m_scrollWidget->setLayout(m_layout);

   m_storedSnapshots.push_back(snapshot);

   m_commentsEdit->setText(""); // clearing current comments ?
   m_renderComment = "";
}

// Stop displaying snapshots
void CRenderViewSnapshots::ClearSnapshotDisplay()
{
   // Already not showing something
   if (!m_displayingSnapshot) return;

   StoredSnapshot *snapshot = m_storedSnapshots[m_displayedSnapshotIndex];
   // can this happen ?
   if (snapshot == NULL) return;
   
   // storing the current comments into this snapshot
   snapshot->comments = m_commentsEdit->toPlainText().toStdString();

   // let's unselect this icon
   snapshot->nameLabel->setStyleSheet("QLabel { color : white; }");   
   snapshot->button->setStyleSheet("QPushButton {  background-color : transparent; color : transparent; }");
   //snapshot->button->setChecked(false);
   m_displayingSnapshot = false;
   //m_displayedSnapshotIndex = 0;
   UpdateComments();

   m_renderView.GetMainWindow()->SetDisplayingSnapshot(false);
   m_renderView.RefreshGLBuffer();

}

// Stop displaying snapshots
void CRenderViewSnapshots::DisplaySnapshot(int index)
{
   if (index < 0) 
   {
      ClearSnapshotDisplay();
      return;
   }

   // Avoid out-of-bounds
   index = MIN(index, (int)m_storedSnapshots.size() - 1);

   // First check if a previous snapshot is displayed
   if (m_displayingSnapshot)
   {
      // we were already selecting the same snapshot
      if (index == m_displayedSnapshotIndex) return;

      if (m_displayedSnapshotIndex >= 0)
      {
         // a previous, different snapshot was displayed
         m_storedSnapshots[m_displayedSnapshotIndex]->nameLabel->setStyleSheet("QLabel { color : white; }");   
         m_storedSnapshots[m_displayedSnapshotIndex]->button->setStyleSheet("QPushButton {  background-color : transparent; color : transparent; }");

         //storing the current comments into this snapshot
         m_storedSnapshots[m_displayedSnapshotIndex]->comments = m_commentsEdit->toPlainText().toStdString();
         //m_storedSnapshots[m_displayedSnapshotIndex]->button->setChecked(false);
      }
   } else
   {
      // we were previously showing the Render (not a snapshot), so we
      // store the previous comment in m_renderComment to restore it later
      m_renderComment = m_commentsEdit->toPlainText().toStdString();
   }

   StoredSnapshot *snapshot = m_storedSnapshots[index];
   // can this happen ?
   if (snapshot == NULL) return;

   //snapshot->button->setChecked(true);
   snapshot->button->setStyleSheet("QPushButton {  background-color : red; color : red; }");
   snapshot->nameLabel->setStyleSheet("QLabel { color : rgb(160, 0, 0); }");
   //snapshot->button->setFocus();

   m_scrollArea->ensureWidgetVisible(snapshot->button);

   m_displayingSnapshot = true;
   
   m_displayedSnapshotIndex = index; 
   UpdateComments(); 

   m_renderView.GetMainWindow()->SetDisplayingSnapshot(true);

   m_renderView.RefreshGLBuffer();

}

void CRenderViewSnapshots::DeleteStoredSnapshot(int index)
{
   // Should we pop-up a confirmation Dialog ??

   if (index < 0 || index >= (int)m_storedSnapshots.size()) return;

   m_layout->removeWidget(m_storedSnapshots[index]->button);
   int widgetWidth = m_scrollWidget->size().width() - m_storedSnapshots[index]->button->size().width();
   m_scrollWidget->resize(widgetWidth, m_scrollWidget->size().height());
   m_scrollWidget->setLayout(m_layout);

   delete m_storedSnapshots[index];
   m_storedSnapshots[index] = NULL;
   m_storedSnapshots.erase(m_storedSnapshots.begin() + index);

   if (m_displayingSnapshot)
   {
      if (m_storedSnapshots.empty())
      {
         m_displayingSnapshot = false;
         UpdateComments();
         m_renderView.GetMainWindow()->SetDisplayingSnapshot(false);
         m_renderView.RefreshGLBuffer();
      } else
      {
         index = MIN(m_displayedSnapshotIndex, (int)m_storedSnapshots.size() -1);
         m_displayedSnapshotIndex = -1;
         DisplaySnapshot(index);
      }
   } else
   {
      m_displayedSnapshotIndex = MIN(m_displayedSnapshotIndex, (int)m_storedSnapshots.size() -1); 
   }
}

void CRenderViewSnapshots::RenameSnapshot(int index)
{
   if (index < 0 || index >= (int) m_storedSnapshots.size()) return;
   StoredSnapshot *snapshot = m_storedSnapshots[index];

   snapshot->nameLabel->hide();
   if (snapshot->nameEdit == NULL)
   {
      snapshot->nameEdit = new SnapshotNameEdit(snapshot->button, *this);
   }
   snapshot->nameEdit->setText(QString(snapshot->name.c_str()));
   snapshot->nameEdit->setStyleSheet("QLineEdit {  background-color : white; color : blue; }");
   snapshot->nameEdit->show();
   snapshot->nameEdit->move(5, 5);
   snapshot->nameEdit->setFocus();

   m_renamingSnapshot = index;
   connect(snapshot->nameEdit, SIGNAL(editingFinished()), this, SLOT(SlotSnapshotRenamed()));
}

void CRenderViewSnapshots::UpdateComments()
{
   if(!m_displayingSnapshot)
   {
      m_commentsEdit->setText(QString(m_renderComment.c_str()));
      return;
   }
   if (m_displayedSnapshotIndex < 0 || m_displayedSnapshotIndex >= (int)m_storedSnapshots.size()) return;
   m_commentsEdit->setText(QString(m_storedSnapshots[m_displayedSnapshotIndex]->comments.c_str()));

}
void CRenderViewSnapshots::SlotSnapshotRenamed()
{
   if (m_renamingSnapshot < 0 || m_renamingSnapshot >= (int) m_storedSnapshots.size()) return;

   StoredSnapshot *snapshot = m_storedSnapshots[m_renamingSnapshot];
   if (snapshot->nameEdit) 
   {
      QString newName = snapshot->nameEdit->text();
      if (!newName.isEmpty())
      {
         snapshot->nameLabel->setText(newName);
         snapshot->name = newName.toStdString();
      }
      delete snapshot->nameEdit;
      snapshot->nameEdit = NULL;
      snapshot->nameLabel->show();
   }

   m_renamingSnapshot = -1;
}

bool CRenderViewSnapshots::eventFilter(QObject *obj, QEvent *event)
{
   if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      int key = keyEvent->key();
      if ( key == Qt::Key_Left || key == Qt::Key_Right 
         || key == Qt::Key_PageUp || key == Qt::Key_PageDown
         || key == Qt::Key_Backtab || key == Qt::Key_Backspace
         || key == Qt::Key_Home || key == Qt::Key_End 
         || key == Qt::Key_Up || key == Qt::Key_Down )
      {
         keyPressEvent(keyEvent);
         return true;
      }
   }
      
   // standard event processing
   return QObject::eventFilter(obj, event);
}

// Qt Virtual funcion redefined
void CRenderViewSnapshots::moveEvent(QMoveEvent *event)
{
   if (m_moving) return;

   m_moving = true;
   AdjustPosition();
   m_moving = false;
}

// Qt Virtual funcion redefined
void CRenderViewSnapshots::keyPressEvent(QKeyEvent *event)
{
   // if Enter -> select 1st displayed snapshot
   // if Right -> select next
   // if Left -> select previous
   int index = 0;
   switch (event->key())
   {
      case Qt::Key_Return:
      case Qt::Key_Enter:
         if (m_displayingSnapshot) ClearSnapshotDisplay();
         else DisplaySnapshot(m_displayedSnapshotIndex);
      return;

      case Qt::Key_Delete:
         if (m_displayingSnapshot) DeleteStoredSnapshot(m_displayedSnapshotIndex);
         return;

      case Qt::Key_Down:
         if (!m_displayingSnapshot) DisplaySnapshot(m_displayedSnapshotIndex);
         return;
      case Qt::Key_Up:
         if (m_displayingSnapshot) ClearSnapshotDisplay();
         return;

      case Qt::Key_Backtab:
      case Qt::Key_Right:

         if (m_displayingSnapshot)
         {
            index = MIN(m_displayedSnapshotIndex + 1, (int)m_storedSnapshots.size() - 1);
            DisplaySnapshot(index);
         }
         return;

      case Qt::Key_Left:
      case Qt::Key_Backspace:
         if (!m_displayingSnapshot) return;

         index = MAX(m_displayedSnapshotIndex - 1, 0);
         DisplaySnapshot(index);
         return;

      case Qt::Key_Home:
      case Qt::Key_PageUp:
         if (m_displayingSnapshot) DisplaySnapshot(0);
         return;


      case Qt::Key_End:
      case Qt::Key_PageDown:
         if (m_displayingSnapshot) DisplaySnapshot((int)m_storedSnapshots.size() - 1);
         return;

      case Qt::Key_F2:
         if (m_displayingSnapshot) RenameSnapshot(m_displayedSnapshotIndex);
         return;
   }
}


void CRenderViewSnapshots::LeftClickThumbnail(StoredSnapshot &snapshot)
{
   if(m_displayingSnapshot &&  GetDisplayedBuffer() == snapshot.buffer)
   {
      ClearSnapshotDisplay();
      return;
   }

   int index = snapshot.button->GetSnapshotIndex();
   if (index >= 0)
   {
      DisplaySnapshot(index);

   } else
   {
      ClearSnapshotDisplay();
   }

}


int SnapshotThumbnail::GetSnapshotIndex() const
{
   const std::vector<StoredSnapshot *> &storedSnapshots = m_window.GetStoredSnapshots();
   for (unsigned int i = 0; i < storedSnapshots.size(); ++i)
   {
      if (storedSnapshots[i]->button == this) return i;      
   }
   // shouldn't happen...
   return -1;
}

void SnapshotThumbnail::mousePressEvent(QMouseEvent * event)
{
   if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::MiddleButton)
   {
      m_window.LeftClickThumbnail(m_snapshot);
   }   
}


void SnapshotThumbnail::ShowContextMenu(const QPoint &pos)
{
   int index = GetSnapshotIndex();
   QMenu contextMenu(tr(m_snapshot.name.c_str()), this);
   QAction action1("Hide Snapshot  (Enter)", this);
   if (m_window.DisplayingSnapshot() && m_window.GetDisplayedBuffer() == m_snapshot.buffer)
   {      
      connect(&action1, SIGNAL(triggered()), this, SLOT(ClearSnapshotDisplay()));
      contextMenu.addAction(&action1);
   } else
   {
      action1.setText("Display Snapshot  (Enter)");
      connect(&action1, SIGNAL(triggered()), this, SLOT(DisplaySnapshot()));
      contextMenu.addAction(&action1);
   }
  
   
   QAction action2("Delete Snapshot   (Del)", this);
   connect(&action2, SIGNAL(triggered()), this, SLOT(DeleteSnapshot()));
   contextMenu.addAction(&action2);
   QAction action3("Rename Snapshot   (F2)", this);
   connect(&action3, SIGNAL(triggered()), this, SLOT(RenameSnapshot()));
   contextMenu.addAction(&action3);
   
   contextMenu.exec(mapToGlobal(pos));
}

void SnapshotThumbnail::DisplaySnapshot()
{
   int index = GetSnapshotIndex();

   if (index >= 0)
   {
      m_window.DisplaySnapshot(index);

   } else
   {
      m_window.ClearSnapshotDisplay();
   }
}

void SnapshotThumbnail::ClearSnapshotDisplay()
{
   m_window.ClearSnapshotDisplay();
}
void SnapshotThumbnail::RenameSnapshot()
{
   int index = GetSnapshotIndex();
   if (index >= 0) m_window.RenameSnapshot(index);
}
void SnapshotThumbnail::DeleteSnapshot()
{
   int index = GetSnapshotIndex();
   if (index >= 0) m_window.DeleteStoredSnapshot(index);
}

// Make sure to validate the lineEdit when we focus out !
void SnapshotNameEdit::focusOutEvent(QFocusEvent * event)
{
   m_window.SlotSnapshotRenamed();
}

#include "snapshots.moc"
