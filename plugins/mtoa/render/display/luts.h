/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


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

#include <QtCore/qbytearray.h>
#include <QtCore/qthread.h>
#include <iostream>


class CRenderView;

enum CRenderViewColorSpace
{
   RV_COLOR_SPACE_NONE,
   RV_COLOR_SPACE_SRGB,
   RV_COLOR_SPACE_REC709
};


struct CRenderViewCCSettings
{
   CRenderViewCCSettings() :  gamma(1.0),
                              exposure(0.f),
                              exposureFactor(1.f),
                              space(RV_COLOR_SPACE_SRGB),
                              dither(true)
                              {}
   float gamma;
   float exposure;
   float exposureFactor;
   CRenderViewColorSpace  space;
   bool  dither;
};

template <typename IN_T, typename OUT_T>
inline OUT_T reinterpret_type(const IN_T in)
{
   union { IN_T in; OUT_T out; } u;
   u.in = in;
   return u.out;
}

class CRenderViewCCWindow : public QMainWindow
{
Q_OBJECT

public:
   CRenderViewCCWindow(QWidget *parent, CRenderView &rv, CRenderViewCCSettings &ccSettings) : 
      QMainWindow(parent), 
      m_renderView(rv),
      m_colorCorrectSettings(ccSettings)
      {}

   virtual ~CRenderViewCCWindow() {}

   void init();

private:
   CRenderView &m_renderView;
   CRenderViewCCSettings &m_colorCorrectSettings;

   QLineEdit *m_gamma_edit;
   QSlider *m_gamma_slider;

   QLineEdit *m_exposure_edit;
   QSlider *m_exposure_slider;

   QCheckBox *m_dither_box;
   QComboBox *m_space_combo;



private slots:
   void gammaSliderChanged();
   void gammaTextChanged();
   void exposureSliderChanged();
   void exposureTextChanged();
   void ditherChanged();
   void colorSpaceChanged();
};





