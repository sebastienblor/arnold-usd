/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once

#include "luts.h"

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <QtGui/qevent.h>
#include <QtGui/qmainwindow.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qthread.h>
#include <iostream>


class CRenderView;
class CRenderViewSnapshots;
class SnapshotThumbnail;
class SnapshotNameEdit;
struct StoredSnapshot;

class SnapshotThumbnail : public QPushButton {
Q_OBJECT

public:
   SnapshotThumbnail(QWidget *parent, CRenderViewSnapshots &win, StoredSnapshot &snapshot ) : 
                     QPushButton(parent), m_window(win), m_snapshot(snapshot) 
   {/*setCheckable(true);*/
      this->setContextMenuPolicy(Qt::CustomContextMenu);

      connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(ShowContextMenu(const QPoint &)));
   }

   virtual ~SnapshotThumbnail() {}

   int GetSnapshotIndex() const;

protected:
   virtual void mousePressEvent(QMouseEvent * e);

   CRenderViewSnapshots &m_window;
   StoredSnapshot &m_snapshot;
private slots:
   void ShowContextMenu(const QPoint &pos); 
   void DisplaySnapshot();
   void DeleteSnapshot();
   void RenameSnapshot();
   void ClearSnapshotDisplay();

};

class SnapshotNameEdit : public QLineEdit {
public:
   SnapshotNameEdit(QWidget *parent, CRenderViewSnapshots &win) : QLineEdit(parent), m_window(win){}
   virtual ~SnapshotNameEdit() {}

protected:
   virtual void focusOutEvent(QFocusEvent * event);

   CRenderViewSnapshots &m_window;

};

struct StoredSnapshot {
   StoredSnapshot() : buffer(NULL), width(0), height(0), button(NULL), nameLabel(NULL), nameEdit(NULL) {}
   ~StoredSnapshot() {
      if (buffer) AiFree(buffer); 
      // until we figure out what's going on, let's just hide the button
      // it will be deleted when the renderview is destroyed anyway
      if(button) button->hide();//delete button;
   }


   std::string name;
   AtRGBA* buffer;
   int width;
   int height;
   std::string status;
   SnapshotThumbnail *button;
   QLabel *nameLabel;
   SnapshotNameEdit *nameEdit;
   std::string comments;
};


class CRenderViewSnapshots : public QMainWindow
{
Q_OBJECT

public:

 
   CRenderViewSnapshots(QWidget *parent, CRenderView &rv) : 
      QMainWindow(parent, Qt::Window | /*Qt::FramelessWindowHint |*/ Qt::CustomizeWindowHint), 
      m_renderView(rv),
      m_moving(false),
      m_displayingSnapshot(false),
      m_displayedSnapshotIndex(0),
      m_renamingSnapshot(-1)
      {}

   virtual ~CRenderViewSnapshots();

   void Clear();
   void Init();
   void AdjustPosition();
   bool DisplayingSnapshot() const {return m_displayingSnapshot;}
   AtRGBA *GetDisplayedBuffer() {return m_storedSnapshots[m_displayedSnapshotIndex]->buffer;}
   int GetDisplayedBufferWidth() {return m_storedSnapshots[m_displayedSnapshotIndex]->width;}
   int GetDisplayedBufferHeight() {return m_storedSnapshots[m_displayedSnapshotIndex]->height;}
   int GetDisplayedBufferIndex() {return m_displayedSnapshotIndex;}
   const std::string &GetDisplayedStatus() const {return  m_storedSnapshots[m_displayedSnapshotIndex]->status;}

   void StoreSnapshot(AtRGBA *buffer, int width, int height, const std::string &statusLog, CRenderViewCCSettings& ccSettings);

   void ClearSnapshotDisplay();
   void DisplaySnapshot(int index);
   void DeleteStoredSnapshot(int index);
   void RenameSnapshot(int index);
   
   void LeftClickThumbnail(StoredSnapshot &snapshot);
   const std::vector<StoredSnapshot*> &GetStoredSnapshots() const {return m_storedSnapshots;}
   
protected:
   void moveEvent(QMoveEvent *event);
   void keyPressEvent ( QKeyEvent * event );
   
   bool eventFilter(QObject *obj, QEvent *event);

   void UpdateComments();

private:
   CRenderView &m_renderView;
   QScrollArea *m_scrollArea;
   QWidget *m_scrollWidget;
   QTextEdit *m_commentsEdit;
   QHBoxLayout *m_layout;

   std::vector<StoredSnapshot*> m_storedSnapshots;   
   bool m_moving;
   bool m_displayingSnapshot;
   int m_displayedSnapshotIndex;

   int m_renamingSnapshot;
   std::string m_renderComment;

public slots:
   void SlotSnapshotRenamed();
};





