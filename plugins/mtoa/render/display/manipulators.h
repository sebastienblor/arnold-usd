/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <QtGui/qevent.h>
#include <QtGui/qmainwindow.h>

#include <iostream>


#include "renderview_interface.h"


class QMenu;
class CRenderView;

class CRenderViewManipulator
{
public:
   CRenderViewManipulator(CRenderView &rv, int x, int y) :
         m_renderView(rv),
         m_startX(x),
         m_startY(y) {}


   virtual ~CRenderViewManipulator() {}

   virtual void MouseMove(int x, int y);
   virtual void MouseRelease(int x, int y);

protected:
   CRenderView &m_renderView;
   int m_startX;
   int m_startY;
};



class CRenderViewCropRegion : public CRenderViewManipulator
{
public:
   CRenderViewCropRegion(CRenderView &rv, int x, int y);
   virtual ~CRenderViewCropRegion();


   virtual void MouseMove(int x, int y);
   virtual void MouseRelease(int x, int y);

};


class CRenderView2DManipulator : public CRenderViewManipulator
{
public:
   CRenderView2DManipulator(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DManipulator();


protected:
   int  m_previousPan[2];
   float m_previousZoom;

};

class CRenderView2DPan : public CRenderView2DManipulator
{
public:
   CRenderView2DPan(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DPan();


   virtual void MouseMove(int x, int y);
   virtual void MouseRelease(int x, int y);



};


class CRenderView2DZoom : public CRenderView2DManipulator
{
public:
   CRenderView2DZoom(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DZoom();


   virtual void MouseMove(int x, int y);
   virtual void MouseRelease(int x, int y);

   static void Wheel(CRenderView &renderView, float delta);


};



class CRenderView3DManipulator : public CRenderViewManipulator
{
public:
   CRenderView3DManipulator(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DManipulator();


   virtual void MouseMove(int x, int y);
   virtual void MouseRelease(int x, int y);

protected:
/*
   MDagPath m_cameraPath;
   MFnCamera m_camera;
   
   MMatrix m_originalMatrix;
   MPoint m_originalPosition;
*/
   CRenderViewInterface *m_interface;      
};


class CRenderView3DPan : public CRenderView3DManipulator
{
public:
   CRenderView3DPan(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DPan();


   virtual void MouseMove(int x, int y);

protected : 
   CRenderViewPanManipulator *m_panManipulator;
};


class CRenderView3DZoom : public CRenderView3DManipulator
{
public:
   CRenderView3DZoom(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DZoom();


   virtual void MouseMove(int x, int y);

   static void Wheel(CRenderView &renderView, float delta);
   static void FrameSelection(CRenderView &renderView);

protected:
   CRenderViewZoomManipulator *m_zoomManipulator;
};



class CRenderView3DRotate : public CRenderView3DManipulator
{
public:
   CRenderView3DRotate(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DRotate();


   virtual void MouseMove(int x, int y);

protected:
   CRenderViewRotateManipulator *m_rotateManipulator;
};
