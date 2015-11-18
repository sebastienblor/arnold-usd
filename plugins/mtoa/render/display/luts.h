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


// CubeLUT.h
// code obtainted from "Cube-Lut- Specifications"  http://wwwimages.adobe.com/content/dam/Adobe/en/products/speedgrade/cc/pdfs/cube-lut-specification-1.0.pdf

#include <string>
#include <vector>
#include <fstream>
using namespace std;
class CubeLUT 
{
public:
   typedef AtRGB tableRow;
   typedef vector <tableRow> table1D;
   typedef vector <table1D> table2D;
   typedef vector <table2D> table3D;
   enum LUTState { OK = 0, NotInitialized = 1, ReadError = 10, WriteError, PrematureEndOfFile, LineError,
                  UnknownOrRepeatedKeyword = 20, TitleMissingQuote, DomainBoundsReversed,
                  LUTSizeOutOfRange, CouldNotParseTableData };
   LUTState status;
   string title;
   tableRow domainMin;
   tableRow domainMax;
   table1D LUT1D;
   table3D LUT3D;
   float invRes;
   float res;
   bool is3d;

   CubeLUT ( void ) { status = NotInitialized; invRes = 1.f; is3d = false;res = 1.f;};
   LUTState LoadCubeFile ( ifstream & infile );
   LUTState SaveCubeFile ( ofstream & outfile );

   void applyLUT(AtRGB &rgb);

private:
   string ReadLine ( ifstream & infile, char lineSeparator);
   tableRow ParseTableRow ( const string & lineOfText );
};

// end CubeLUT.h


struct CRenderViewCCSettings
{
   CRenderViewCCSettings() :  gamma(1.0),
                              exposure(0.f),
                              exposureFactor(1.f),
                              space(RV_COLOR_SPACE_SRGB),
                              dither(true),
                              lut3d(NULL)
                              {}

   ~CRenderViewCCSettings() {
      if (lut3d) delete lut3d;


   }
   float gamma;
   float exposure;
   float exposureFactor;
   CRenderViewColorSpace  space;
   bool  dither;
   CubeLUT *lut3d;
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
      QMainWindow(parent, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint), 
      m_renderView(rv),
      m_colorCorrectSettings(ccSettings),
      m_moving(false)
      {}

   virtual ~CRenderViewCCWindow() {}

   void init();
   void adjustPosition();
protected:
   void moveEvent(QMoveEvent *event);
   

private:
   CRenderView &m_renderView;
   CRenderViewCCSettings &m_colorCorrectSettings;

   QLineEdit *m_gamma_edit;
   QSlider *m_gamma_slider;

   QLineEdit *m_exposure_edit;
   QSlider *m_exposure_slider;

   QCheckBox *m_dither_box;
   QComboBox *m_space_combo;

   QLineEdit *m_lut_file_edit;
   bool m_moving;


private slots:
   void gammaSliderChanged();
   void gammaTextChanged();
   void exposureSliderChanged();
   void exposureTextChanged();
   void ditherChanged();
   void colorSpaceChanged();
   void browseLutFile();
   void lutFileTextChanged();

};





