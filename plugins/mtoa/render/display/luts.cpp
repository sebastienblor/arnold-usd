/*
 * Windows window and event handling
 */


#include "renderview.h"


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

#include "display_gl.h"


#include "utility.h"
#include <emmintrin.h>


#include <iostream>
#include <sstream>
#include <fstream> 


void CRenderViewCCWindow::init()
{
   setWindowTitle("LUT / Color Correction");
   
   int line = 20;
   m_gamma_edit = new QLineEdit(this);
   m_gamma_edit->move(80, line);

   m_gamma_edit->resize(40, 20);
   m_gamma_edit->setValidator( new QDoubleValidator(0., 100., 2, this) );
   m_gamma_edit->setText(QString::number(m_colorCorrectSettings.gamma));
   

   m_gamma_slider = new QSlider(Qt::Horizontal, this);
   m_gamma_slider->move(130, line);

   
   m_gamma_slider->resize(150, 20);
   m_gamma_slider->setMinimum(0);
   m_gamma_slider->setMaximum(500);
   m_gamma_slider->setValue(int(m_colorCorrectSettings.gamma * 100));

   QLabel *label = new QLabel(QString("Gamma"), this);
   label->resize(40, 20);
   label->move(20, line );

   connect(m_gamma_slider, SIGNAL(valueChanged(int)), this, SLOT(gammaSliderChanged()));
   connect(m_gamma_edit, SIGNAL(returnPressed()), this, SLOT(gammaTextChanged()));

   line += 30;

   m_exposure_edit = new QLineEdit(this);
   m_exposure_edit->move(80, line);

   
   m_exposure_edit->resize(40, 20);
   m_exposure_edit->setValidator( new QDoubleValidator(-100., 100., 2, this) );
   m_exposure_edit->setText(QString::number(m_colorCorrectSettings.exposure));
   
   m_exposure_slider = new QSlider(Qt::Horizontal, this);
   m_exposure_slider->move(130, line);
   m_exposure_slider->resize(150, 20);
   m_exposure_slider->setMinimum(0);
   m_exposure_slider->setMaximum(1000);
   m_exposure_slider->setValue(int(m_colorCorrectSettings.exposure * 100) + 500);
   label = new QLabel(QString("Exposure"), this);
   label->move(20, line);
   label->resize(55, 20);

   connect(m_exposure_slider, SIGNAL(valueChanged(int)), this, SLOT(exposureSliderChanged()));
   connect(m_exposure_edit, SIGNAL(returnPressed()), this, SLOT(exposureTextChanged()));

   line += 30;

   m_dither_box = new QCheckBox( this);
   m_dither_box->move(80, line);
   m_dither_box->setChecked(m_colorCorrectSettings.dither);

   connect(m_dither_box, SIGNAL( stateChanged(int) ), this, SLOT(ditherChanged()));

   label = new QLabel(QString("Dither"), this);
   label->move(20, line);
   label->resize(40, 20);

   line += 30;

   m_space_combo = new QComboBox(this);
   m_space_combo->resize(90, 20);
   
   m_space_combo->move(80, line);

   m_space_combo->addItem("Linear");
   m_space_combo->addItem("sRGB");
   m_space_combo->addItem("Rec709");
   m_space_combo->setCurrentIndex(1);

   label = new QLabel(QString("Color Space"), this);
   label->move(20, line);
   label->resize(70, 20);

   connect(m_space_combo, SIGNAL( activated(int) ), this, SLOT(colorSpaceChanged()));
   line += 30;

   label = new QLabel(QString("LUT File"), this);
   label->move(20, line);
   label->resize(70, 20);

   m_lut_file_edit =  new QLineEdit(this);
   m_lut_file_edit->move(80, line);
   m_lut_file_edit->resize(180, 20);

   connect(m_lut_file_edit, SIGNAL(textChanged(const QString &)), this, SLOT(lutFileTextChanged()));

   QPushButton *button = new QPushButton("...", this);
   button->move(260, line);
   button->resize(30, 20);
   connect(button, SIGNAL( clicked() ), this, SLOT(browseLutFile()));

   line += 50;
   resize(300, line);
}

void CRenderViewCCWindow::gammaSliderChanged()
{
   m_colorCorrectSettings.gamma = ((float)m_gamma_slider->sliderPosition()) / 100.f;

   m_gamma_edit->blockSignals(true);
   m_gamma_edit->setText(QString::number(m_colorCorrectSettings.gamma));
   m_gamma_edit->blockSignals(false);
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::gammaTextChanged()
{

   QString gamma = m_gamma_edit->text();
   m_colorCorrectSettings.gamma = gamma.toFloat();
   m_gamma_slider->blockSignals(true);
   m_gamma_slider->setValue(int(m_colorCorrectSettings.gamma * 100));
   m_gamma_slider->blockSignals(false);
   m_renderView.refreshGLBuffer();
}


void CRenderViewCCWindow::exposureSliderChanged()
{
   m_colorCorrectSettings.exposure = ((float)m_exposure_slider->sliderPosition() - 500.f) / 100.f;
   m_colorCorrectSettings.exposureFactor = powf(2.f, m_colorCorrectSettings.exposure);

   m_exposure_edit->blockSignals(true);
   m_exposure_edit->setText(QString::number(m_colorCorrectSettings.exposure));
   m_exposure_edit->blockSignals(false);
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::exposureTextChanged()
{

   QString exposureStr = m_exposure_edit->text();
   m_colorCorrectSettings.exposure = exposureStr.toFloat();
   m_colorCorrectSettings.exposureFactor = powf(2.f, m_colorCorrectSettings.exposure);

   m_exposure_slider->blockSignals(true);
   m_exposure_slider->setValue(int(m_colorCorrectSettings.exposure * 100) + 500);
   m_exposure_slider->blockSignals(false);
   m_renderView.refreshGLBuffer();
}


void CRenderViewCCWindow::ditherChanged()
{
   m_colorCorrectSettings.dither = m_dither_box->isChecked();
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::colorSpaceChanged()
{
   m_colorCorrectSettings.space = (CRenderViewColorSpace)m_space_combo->currentIndex();
   m_renderView.refreshGLBuffer();  
}


void CRenderViewCCWindow::browseLutFile()
{
   QString lutFilename = QFileDialog::getOpenFileName(this, tr("Load LUT (.cube) file"), QString(), tr("CUBE Files (*.cube)"));
   if (lutFilename == "") return;

   std::string lutFilenameStr = lutFilename.toStdString();
   std::ifstream infile(lutFilenameStr.c_str());
   if(!infile.good())
   {
      std::cerr<<"Error : CUBE File not found"<<std::endl;
      return;
   }
   m_lut_file_edit->setText(lutFilename);

   if (m_colorCorrectSettings.lut3d) delete m_colorCorrectSettings.lut3d;

   m_colorCorrectSettings.lut3d = new CubeLUT;
   int ret = m_colorCorrectSettings.lut3d->LoadCubeFile ( infile );
   infile.close();
   if ( ret != CubeLUT::OK ) 
   {
      cout << "Could not parse the cube info in the input file. Return code = "<< ret << endl;
      return;
   }
   m_renderView.refreshGLBuffer();

}


void CRenderViewCCWindow::lutFileTextChanged()
{
   QString lutFilename(m_lut_file_edit->text());
   std::string lutFilenameStr = lutFilename.toStdString();
   std::ifstream infile(lutFilenameStr.c_str());
   if(!infile.good())
   {
      std::cerr<<"Error : CUBE File not found"<<std::endl;
      return;
   }

   if (m_colorCorrectSettings.lut3d) delete m_colorCorrectSettings.lut3d;

   m_colorCorrectSettings.lut3d = new CubeLUT;
   int ret = m_colorCorrectSettings.lut3d->LoadCubeFile ( infile );
   infile.close();
   if ( ret != CubeLUT::OK ) 
   {
      cout << "Could not parse the cube info in the input file. Return code = "<< ret << endl;
      return;
   }
   m_renderView.refreshGLBuffer();

}

using namespace std;

void CubeLUT::applyLUT(AtRGB &rgb)
{
   if (is3d)
   {
      if (rgb.r > 1.f || rgb.g > 1.f || rgb.b > 1.f ||
         rgb.r < 0.f || rgb.g < 0.f || rgb.b < 0.f) return;

      float tx, ty, tz;
      tx = (rgb.r * (res- 1.f));
      ty = (rgb.g * (res- 1.f));
      tz = (rgb.b * (res- 1.f));

      int coords[3];
      coords[0] = (int)tx;
      coords[1] = (int)ty;
      coords[2] = (int)tz;

      if (coords[0] == res -1) --coords[0];
      if (coords[1] == res -1) --coords[1];
      if (coords[2] == res -1) --coords[2];
      

      tx -= coords[0];
      ty -= coords[1];
      tz -= coords[2];

      const AtRGB &c000 = LUT3D[coords[0]][coords[1]][coords[2]];
      const AtRGB &c100 = LUT3D[coords[0] + 1][coords[1]][coords[2]];
      const AtRGB &c010 = LUT3D[coords[0]][coords[1] + 1][coords[2]];
      const AtRGB &c110 = LUT3D[coords[0] + 1][coords[1] + 1][coords[2]];

      const AtRGB &c001 = LUT3D[coords[0]][coords[1]][coords[2]+1];
      const AtRGB &c101 = LUT3D[coords[0]+1][coords[1]][coords[2]+1];
      const AtRGB &c011 = LUT3D[coords[0]][coords[1]+1][coords[2]+1];
      const AtRGB &c111 = LUT3D[coords[0]+1][coords[1]+1][coords[2]+1];

      rgb = (1.f-tx)*(1.f-ty)*(1.f-tz)*c000 +
               tx*(1.f-ty)*(1.f-tz)*c100 +
               (1.f-tx)*ty*(1.f-tz)*c010 +
               tx*ty*(1.f-tz)*c110 +
               (1.f-tx)*(1.f-ty)*tz*c001 +
               tx*(1.f-ty)*tz*c101 +
               (1.f-tx)*ty*tz*c011 +
               tx*ty*tz*c111;

   } else
   {

      // 1D cube file
      if (rgb.r > 1.f || rgb.g > 1.f || rgb.b > 1.f ||
         rgb.r < 0.f || rgb.g < 0.f || rgb.b < 0.f) return;

      float tx, ty, tz;
      tx = (rgb.r * (res- 1.f));
      ty = (rgb.g * (res- 1.f));
      tz = (rgb.b * (res- 1.f));

      int coords[3];
      coords[0] = (int)tx;
      coords[1] = (int)ty;
      coords[2] = (int)tz;

      if (coords[0] == res -1) --coords[0];
      if (coords[1] == res -1) --coords[1];
      if (coords[2] == res -1) --coords[2];
      

      tx -= coords[0];
      ty -= coords[1];
      tz -= coords[2];

      // independent linear interpolation for each of the channels
      rgb.r = tx * LUT1D[coords[0] + 1].r + (1.f - tx) * LUT1D[coords[0]].r;
      rgb.g = ty * LUT1D[coords[1] + 1].g + (1.f - ty) * LUT1D[coords[1]].g;
      rgb.b = tz * LUT1D[coords[2] + 1].b + (1.f - tz) * LUT1D[coords[2]].b;
   }
}
string CubeLUT::ReadLine ( ifstream & infile, char lineSeparator )
{
   // Skip empty lines and comments
   const char CommentMarker = '#';
   string textLine("");
   while ( textLine.size() == 0 || textLine[0] == CommentMarker )
   {
      if ( infile.eof() ) { status = PrematureEndOfFile; break; }
      getline ( infile, textLine, lineSeparator );
      if ( infile.fail() ) { status = ReadError; break; }
   }
   return textLine;
} // ReadLine

AtRGB CubeLUT::ParseTableRow ( const string & lineOfText )
{
   tableRow f;
   istringstream line ( lineOfText );
   
   line >> f.r;
   if ( line.fail() ) { status = CouldNotParseTableData; return f; };
   line >> f.g;
   if ( line.fail() ) { status = CouldNotParseTableData; return f; };
   line >> f.b;
   if ( line.fail() ) { status = CouldNotParseTableData; return f; };
   
   return f;
} // ParseTableRow


CubeLUT::LUTState CubeLUT::LoadCubeFile ( ifstream & infile )
{
   // Set defaults
   status = OK;
   title.clear();
   domainMin = AI_RGB_BLACK;
   domainMax = AI_RGB_WHITE;
   LUT1D.clear();
   LUT3D.clear();
   // Read file data line by line
   const char NewlineCharacter = '\n';
   char lineSeparator = NewlineCharacter;
   // sniff use of legacy lineSeparator
   const char CarriageReturnCharacter = '\r';
   for (int i = 0; i < 255; i++) 
   {
      char inc = infile.get();
      if ( inc == NewlineCharacter ) break;
      if ( inc == CarriageReturnCharacter )
      {
         if ( infile.get() == NewlineCharacter ) break;
         lineSeparator = CarriageReturnCharacter;
         clog << "INFO: This file uses non-compliant line separator \\r (0x0D)" << endl;
         break;
      }
      if ( i > 250 ) { status = LineError; break; }
   }
   infile.seekg ( 0 );
   infile.clear();
   // read keywords
   int N, CntTitle, CntSize, CntMin, CntMax;

   // each keyword to occur zero or one time
   N = CntTitle = CntSize = CntMin = CntMax = 0;
   while ( status == OK ) 
   {
      long linePos = infile.tellg();
      string lineOfText = ReadLine ( infile, lineSeparator );
      if ( ! status == OK ) break;
      // Parse keywords and parameters
      istringstream line ( lineOfText );
      string keyword;
      line >> keyword;
      if ( "+" < keyword && keyword < ":" ) 
      {
         // lines of table data come after keywords
         // restore stream pos to re-read line of data
         infile.seekg ( linePos );
         break;
      } else if ( keyword == "TITLE" && CntTitle++ == 0 ) 
      {
         const char QUOTE = '"';
         char startOfTitle;
         line >> startOfTitle;
         if ( startOfTitle != QUOTE ) { status = TitleMissingQuote; break; }
         getline ( line, title, QUOTE ); // read to "
      } else if ( keyword == "DOMAIN_MIN" && CntMin++ == 0 ) 
      {
         line >> domainMin[0] >> domainMin[1] >> domainMin[2];
      } else if ( keyword == "DOMAIN_MAX" && CntMax++ == 0 ) 
      {
         line >> domainMax[0] >> domainMax[1] >> domainMax[2];
      } else if ( keyword == "LUT_1D_SIZE" && CntSize++ == 0 ) 
      {
         line >> N;
         if ( N < 2 || N > 65536 ) { status = LUTSizeOutOfRange; break; }
         LUT1D = table1D ( N, tableRow() );
      } else if ( keyword == "LUT_3D_SIZE" && CntSize++ == 0 ) 
      {
         line >> N;
         if ( N < 2 || N > 256 ) { status = LUTSizeOutOfRange; break; }
         LUT3D = table3D ( N, table2D ( N, table1D ( N, tableRow () ) ) );
      } else { status = UnknownOrRepeatedKeyword; break; }

      if ( line.fail() ) { status = ReadError; break; }
   } // read keywords
   if ( status == OK && CntSize == 0 ) status = LUTSizeOutOfRange;
   if ( status == OK && ( domainMin[0] >= domainMax[0] || domainMin[1] >= domainMax[1]
         || domainMin[2] >= domainMax[2] ) ) status = DomainBoundsReversed;
   // read lines of table data
   if ( LUT1D.size() > 0 ) 
   {
      N = (int)LUT1D.size();
      for ( int i = 0; i < N && status == OK; i++ ) 
      {
         LUT1D [i] = ParseTableRow ( ReadLine ( infile, lineSeparator ) );
      }
      res = (float)LUT1D.size();
      invRes = 1.f / res;
      is3d = false;
   } else if (LUT3D.size() > 0)
   {
      N = (int)LUT3D.size();
      // NOTE that r loops fastest
      for ( int b = 0; b < N && status == OK; b++ ) 
      {
         for ( int g = 0; g < N && status == OK; g++ ) 
         {
            for ( int r = 0; r < N && status == OK; r++ ) 
            {
               LUT3D[r][g][b] = ParseTableRow ( ReadLine ( infile, lineSeparator ) );
            }
         }
         res = (float)LUT3D.size();
         invRes = 1.f / res;
         is3d = true;
      }
   } // read 3D LUT
   return status;
} // LoadCubeFile


CubeLUT:: LUTState CubeLUT:: SaveCubeFile ( ofstream & outfile )
{
   if ( ! status == OK ) return status; // Write only good Cubes
   // Write keywords
   const char SPACE = ' ';
   const char QUOTE = '"';
   if ( title.size() > 0 ) outfile << "TITLE" << SPACE << QUOTE << title << QUOTE << endl;
   outfile << "# Created by CubeLUT.cpp" << endl;
   outfile << "DOMAIN_MIN" << SPACE << domainMin[0] << SPACE << domainMin[1]<< SPACE << domainMin[2] << endl;
   outfile << "DOMAIN_MAX" << SPACE << domainMax[0] << SPACE << domainMax[1]<< SPACE << domainMax[2] << endl;
   // Write LUT data

   if ( LUT1D.size() > 0 ) 
   {
      int N = (int)LUT1D.size();
      outfile << "LUT_1D_SIZE" << SPACE << N << endl;
      for ( int i = 0; i < N && outfile.good(); i++ ) 
      {
         outfile << LUT1D[i] [0] << SPACE << LUT1D[i] [1] << SPACE << LUT1D[i] [2] << endl;
      }
   } else 
   {
      int N = (int)LUT3D.size();
      outfile << "LUT_3D_SIZE" << SPACE << N << endl;
      // NOTE that r loops fastest
      for ( int b = 0; b < N && outfile.good(); b++ ) 
      {
         for ( int g = 0; g < N && outfile.good(); g++ ) 
         {
            for ( int r = 0; r < N && outfile.good(); r++ ) 
            {
               outfile << LUT3D[r][g][b] [0] << SPACE << LUT3D[r][g][b] [1]
                             << SPACE << LUT3D[r][g][b] [2] << endl;
            }
         }
      }
   } // write 3D LUT
   outfile.flush();
   return ( outfile.good() ? OK : WriteError );
} // SaveCubeFile


// If you add some slots, you'll have to run moc
#include "luts.moc"
