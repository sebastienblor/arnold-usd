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


class QMenu;
class CRenderView;

class CRenderViewManipulator
{
public:
   CRenderViewManipulator(CRenderView &rv, int x, int y) :
         m_renderView(rv),
         m_start_x(x),
         m_start_y(y) {}


   virtual ~CRenderViewManipulator() {}

   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

protected:
   CRenderView &m_renderView;
   int m_start_x;
   int m_start_y;
};



class CRenderViewCropRegion : public CRenderViewManipulator
{
public:
   CRenderViewCropRegion(CRenderView &rv, int x, int y);
   virtual ~CRenderViewCropRegion();


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

};

class CRenderView2DPan : public CRenderViewManipulator
{
public:
   CRenderView2DPan(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DPan();


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

protected:
   int  m_previousPan[2];
   float m_previousZoom;

};


class CRenderView2DZoom : public CRenderViewManipulator
{
public:
   CRenderView2DZoom(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DZoom();


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

   static void wheel(CRenderView &renderView, float delta);

protected:
   int  m_previousPan[2];
   float m_previousZoom;

};

