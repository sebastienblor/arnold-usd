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

#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>


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


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);



};


class CRenderView2DZoom : public CRenderView2DManipulator
{
public:
   CRenderView2DZoom(CRenderView &rv, int x, int y);
   virtual ~CRenderView2DZoom();


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

   static void wheel(CRenderView &renderView, float delta);


};



class CRenderView3DManipulator : public CRenderViewManipulator
{
public:
   CRenderView3DManipulator(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DManipulator();


   virtual void mouseMove(int x, int y);
   virtual void mouseRelease(int x, int y);

protected:

   MDagPath m_cameraPath;
   MFnCamera m_camera;
   
   MMatrix m_original_matrix;
   MPoint m_original_position;
};




class CRenderView3DPan : public CRenderView3DManipulator
{
public:
   CRenderView3DPan(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DPan();


   virtual void mouseMove(int x, int y);

protected : 
   MVector  m_up_direction;
   MVector  m_right_direction;
   MVector  m_view_direction;
   float    m_dist_factor;
};


class CRenderView3DZoom : public CRenderView3DManipulator
{
public:
   CRenderView3DZoom(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DZoom();


   virtual void mouseMove(int x, int y);

   static void wheel(CRenderView &renderView, float delta);

protected:
   MVector  m_view_direction;
   MVector  m_up_direction;
   MPoint   m_center;
};



class CRenderView3DRotate : public CRenderView3DManipulator
{
public:
   CRenderView3DRotate(CRenderView &rv, int x, int y);
   virtual ~CRenderView3DRotate();


   virtual void mouseMove(int x, int y);

protected:
   MPoint m_center;
   MVector m_up_direction;
   float m_center_dist;
   float m_orig_lon;
   float m_orig_lat;
   MVector m_view_direction;
   MVector m_orig_rotation;
   
};
