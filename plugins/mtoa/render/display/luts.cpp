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
using namespace std;

void CRenderViewCCWindow::Init()
{
   QScrollArea *scrollArea = new QScrollArea(this);
   QWidget *scrollWidget = new QWidget(this);
   scrollArea->setWidget(scrollWidget);
   this->setCentralWidget(scrollArea);

   setWindowTitle("LUT / Color Correction");
   

   int line = 20;
   m_gammaEdit = new QLineEdit(scrollWidget);
   m_gammaEdit->move(80, line);

   m_gammaEdit->resize(40, 20);
   m_gammaEdit->setValidator( new QDoubleValidator(0., 100., 2, this) );
   m_gammaEdit->setText(QString::number(m_colorCorrectSettings.gamma));
   

   m_gammaSlider = new QSlider(Qt::Horizontal, scrollWidget);
   m_gammaSlider->move(130, line);

   
   m_gammaSlider->resize(150, 20);
   m_gammaSlider->setMinimum(0);
   m_gammaSlider->setMaximum(500);
   m_gammaSlider->setValue(int(m_colorCorrectSettings.gamma * 100));

   QLabel *label = new QLabel(QString("Gamma"), scrollWidget);
   label->resize(40, 20);
   label->move(20, line );

   connect(m_gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(GammaSliderChanged()));
   connect(m_gammaEdit, SIGNAL(editingFinished()), this, SLOT(GammaTextChanged()));

   line += 30;

   m_exposureEdit = new QLineEdit(scrollWidget);
   m_exposureEdit->move(80, line);

   
   m_exposureEdit->resize(40, 20);
   m_exposureEdit->setValidator( new QDoubleValidator(-100., 100., 2, this) );
   m_exposureEdit->setText(QString::number(m_colorCorrectSettings.exposure));
   
   m_exposureSlider = new QSlider(Qt::Horizontal, scrollWidget);
   m_exposureSlider->move(130, line);
   m_exposureSlider->resize(150, 20);
   m_exposureSlider->setMinimum(0);
   m_exposureSlider->setMaximum(1000);
   m_exposureSlider->setValue(int(m_colorCorrectSettings.exposure * 100) + 500);
   label = new QLabel(QString("Exposure"), scrollWidget);
   label->move(20, line);
   label->resize(55, 20);

   connect(m_exposureSlider, SIGNAL(valueChanged(int)), this, SLOT(ExposureSliderChanged()));
   connect(m_exposureEdit, SIGNAL(editingFinished()), this, SLOT(ExposureTextChanged()));

   line += 30;
/*
   m_dither_box = new QCheckBox( this);
   m_dither_box->move(80, line);
   m_dither_box->setChecked(m_colorCorrectSettings.dither);

   connect(m_dither_box, SIGNAL( stateChanged(int) ), this, SLOT(ditherChanged()));

   label = new QLabel(QString("Dither"), this);
   label->move(20, line);
   label->resize(40, 20);

   line += 30;
*/
   m_spaceCombo = new QComboBox(scrollWidget);
   m_spaceCombo->resize(90, 20);
   
   m_spaceCombo->move(80, line);

   m_spaceCombo->addItem("Linear");
   m_spaceCombo->addItem("sRGB");
   m_spaceCombo->addItem("Rec709");
   m_spaceCombo->addItem("LUT File");

   m_spaceCombo->setCurrentIndex(1);

   label = new QLabel(QString("Color Space"), scrollWidget);
   label->move(20, line);
   label->resize(70, 20);

   connect(m_spaceCombo, SIGNAL( activated(int) ), this, SLOT(ColorSpaceChanged()));
   line += 30;

   m_lutFileLabel = new QLabel(QString("LUT File"), scrollWidget);
   m_lutFileLabel->move(20, line);
   m_lutFileLabel->resize(70, 20);

   m_lutFileEdit =  new QLineEdit(scrollWidget);
   m_lutFileEdit->move(80, line);
   m_lutFileEdit->resize(180, 20);

   connect(m_lutFileEdit, SIGNAL(editingFinished()), this, SLOT(LutFileTextChanged()));

   m_lutFileButton = new QPushButton("...", scrollWidget);
   m_lutFileButton->move(260, line);
   m_lutFileButton->resize(30, 20);
   connect(m_lutFileButton, SIGNAL( clicked() ), this, SLOT(BrowseLutFile()));
   line += 30;

   m_lutFileButton->setEnabled(false);
   m_lutFileEdit->setEnabled(false);
   m_lutFileLabel->setEnabled(false);


//==================================

   QGroupBox *bgBox = new QGroupBox(scrollWidget);
   bgBox->setTitle("Background");
   bgBox->move(5, line);

   int bgLine = line;

   line += 20;

   label = new QLabel(QString("BG"), bgBox);
   label->move(20 - 5, line - bgLine);
   label->resize(40, 20);

   m_bgCombo = new QComboBox(bgBox);
   m_bgCombo->resize(90, 20);
   
   m_bgCombo->move(80 - 5, line- bgLine);

   m_bgCombo->addItem("BG Color");
   m_bgCombo->addItem("BG Image");
   m_bgCombo->addItem("Checker");
   m_bgCombo->setCurrentIndex(0);

   connect(m_bgCombo, SIGNAL( activated(int) ), this, SLOT(BgChanged()));

   line += 30;

   m_bgColorLabel = new QLabel(QString("Color"), bgBox);
   m_bgColorLabel->move(20 - 5, line- bgLine);
   m_bgColorLabel->resize(70, 20);

   m_bgColorButton = new QPushButton("", bgBox);
   m_bgColorButton->move(80 - 5, line- bgLine);
   m_bgColorButton->resize(20, 20);
   connect(m_bgColorButton, SIGNAL( clicked() ), this, SLOT(PickBgColor()));
   
   AtRGBA currentBgColor = m_renderView.GetGlWidget()->GetBackgroundColor();
   QColor col(int(currentBgColor.r * 255), int(currentBgColor.g * 255), int(currentBgColor.b * 255), 255); 
   QString qss = QString("background-color: %1").arg(col.name());
   m_bgColorButton->setStyleSheet(qss);

   line += 30;

   m_bgFileLabel = new QLabel(QString("Image"), bgBox);
   m_bgFileLabel->move(20 - 5, line- bgLine);
   m_bgFileLabel->resize(70, 20);

   m_bgFileEdit =  new QLineEdit(bgBox);
   m_bgFileEdit->move(80 - 5, line- bgLine);
   m_bgFileEdit->resize(170, 20);
   connect(m_bgFileEdit, SIGNAL(editingFinished()), this, SLOT(BgFileTextChanged()));

   m_bgFileButton = new QPushButton("...", bgBox);
   m_bgFileButton->move(250 - 5, line- bgLine);
   m_bgFileButton->resize(30, 20);
      
   connect(m_bgFileButton, SIGNAL( clicked() ), this, SLOT(BrowseBgFile()));
   
   line += 30;
   m_scaleLabel = new QLabel(QString("Scale"), bgBox);
   m_scaleLabel->move(20 - 5, line- bgLine);
   m_scaleLabel->resize(70, 20);

   m_scaleEditX = new QLineEdit(bgBox);
   m_scaleEditX->move(80 - 5, line- bgLine);
   m_scaleEditX->resize(40, 20);
   m_scaleEditX->setValidator( new QDoubleValidator(-1000., 1000., 2, this) );
   m_scaleEditX->setText("1");

   m_scaleEditY = new QLineEdit(bgBox);
   m_scaleEditY->move(140 - 5, line- bgLine);
   m_scaleEditY->resize(40, 20);
   m_scaleEditY->setValidator( new QDoubleValidator(-1000., 1000., 2, this) );
   m_scaleEditY->setText("1");

   connect(m_scaleEditX, SIGNAL(editingFinished()), this, SLOT(BgScaleChanged()));
   connect(m_scaleEditY, SIGNAL(editingFinished()), this, SLOT(BgScaleChanged()));


   line += 30;
   m_offsetLabel = new QLabel(QString("Offset"), bgBox);
   m_offsetLabel->move(20 - 5, line- bgLine);
   m_offsetLabel->resize(70, 20);

   m_offsetEditX = new QLineEdit(bgBox);
   m_offsetEditX->move(80 - 5, line- bgLine);
   m_offsetEditX->resize(40, 20);
   m_offsetEditX->setValidator( new QDoubleValidator(-1000., 1000., 2, this) );
   m_offsetEditX->setText("0");
   
   m_offsetEditY = new QLineEdit(bgBox);
   m_offsetEditY->move(140 - 5, line- bgLine);
   m_offsetEditY->resize(40, 20);
   m_offsetEditY->setValidator( new QDoubleValidator(-1000., 1000., 2, this) );
   m_offsetEditY->setText("0");
   

   connect(m_offsetEditX, SIGNAL(editingFinished()), this, SLOT(BgOffsetChanged()));
   connect(m_offsetEditY, SIGNAL(editingFinished()), this, SLOT(BgOffsetChanged()));
   line += 30;
   bgBox->resize(280, line - bgLine);

   line += 20;
   resize(300, line);

   scrollWidget->setFixedSize(290, line);
   scrollArea->resize(300, line);


   UpdateBgUI();
}

void CRenderViewCCWindow::UpdateBgUI()
{
   int bgCombo = m_bgCombo->currentIndex();

   m_bgColorLabel->setEnabled((bgCombo == 0));
   m_bgColorButton->setEnabled((bgCombo == 0));
   m_bgFileEdit->setEnabled((bgCombo == 1));
   m_bgFileLabel->setEnabled((bgCombo == 1));
   m_bgFileButton->setEnabled((bgCombo == 1));
   m_scaleLabel->setEnabled((bgCombo != 0));
   m_scaleEditX->setEnabled((bgCombo != 0));
   m_scaleEditY->setEnabled((bgCombo != 0));   
   m_offsetLabel->setEnabled((bgCombo != 0));
   m_offsetEditX->setEnabled((bgCombo != 0));
   m_offsetEditY->setEnabled((bgCombo != 0));
}
void CRenderViewCCWindow::UpdateBackground()
{
   int bgCombo = m_bgCombo->currentIndex();

   if (bgCombo == 0)
   {
      // bg color
      m_renderView.GetGlWidget()->SetBackgroundImage("");
      
   } else if (bgCombo == 1)
   {
      // bg texture
      QString filename = m_bgFileEdit->text();
      std::string filenameStr = (filename.isEmpty()) ? "" : filename.toStdString();
      m_renderView.GetGlWidget()->SetBackgroundImage(filenameStr);

   } else
   {
      // checker
      m_renderView.GetGlWidget()->SetBackgroundChecker();
   }

   m_renderView.RefreshGLBuffer();   
}

void CRenderViewCCWindow::BgChanged()
{
   UpdateBgUI();
   UpdateBackground();
}
void CRenderViewCCWindow::BgFileTextChanged()
{
   UpdateBackground();
}
void CRenderViewCCWindow::PickBgColor()
{
   AtRGBA currentBgColor = m_renderView.GetGlWidget()->GetBackgroundColor();
   QColor bg_color(int(currentBgColor.r * 255), int(currentBgColor.g * 255), 
      int(currentBgColor.b * 255), 255);

   QColor col = QColorDialog::getColor(bg_color, this);
   if(col.isValid())
   {
      QString qss = QString("background-color: %1").arg(col.name());
      m_bgColorButton->setStyleSheet(qss);
      currentBgColor.r = col.redF();
      currentBgColor.g = col.greenF();
      currentBgColor.b = col.blueF();
      currentBgColor.a = col.alphaF();

      m_renderView.GetGlWidget()->SetBackgroundColor(currentBgColor);
      m_renderView.Draw();
   }
}

void CRenderViewCCWindow::BrowseBgFile()
{  
   QString filename = QFileDialog::getOpenFileName(this, tr("Load Background file"), QString(), 
         tr("Image files (*.jpg *.cin *.png *.gif *.als *.rla *.sgi *.tga *.tif *.iff *.exr)"));

   if (filename.isEmpty()) return;
   
   m_bgFileEdit->blockSignals(true);
   m_bgFileEdit->setText(filename);
   m_bgFileEdit->blockSignals(false);

   UpdateBackground();
}

void CRenderViewCCWindow::BgScaleChanged()
{
   CRenderGLWidget::BackgroundData *bgData = m_renderView.GetGlWidget()->GetBackgroundData();
   if (bgData == NULL) return;

   bgData->scale.x = m_scaleEditX->text().toFloat();
   bgData->scale.y = m_scaleEditY->text().toFloat();
   m_renderView.Draw();
}

void CRenderViewCCWindow::BgOffsetChanged()
{
   CRenderGLWidget::BackgroundData *bgData = m_renderView.GetGlWidget()->GetBackgroundData();
   if (bgData == NULL) return;

   bgData->offset.x = m_offsetEditX->text().toFloat();
   bgData->offset.y = m_offsetEditY->text().toFloat();
   m_renderView.Draw();
}


void CRenderViewCCWindow::GammaSliderChanged()
{
   m_colorCorrectSettings.gamma = ((float)m_gammaSlider->sliderPosition()) / 100.f;

   m_gammaEdit->blockSignals(true);
   m_gammaEdit->setText(QString::number(m_colorCorrectSettings.gamma));
   m_gammaEdit->blockSignals(false);
   m_renderView.RefreshGLBuffer();
}

void CRenderViewCCWindow::GammaTextChanged()
{

   QString gamma = m_gammaEdit->text();
   m_colorCorrectSettings.gamma = gamma.toFloat();
   m_gammaSlider->blockSignals(true);
   m_gammaSlider->setValue(int(m_colorCorrectSettings.gamma * 100));
   m_gammaSlider->blockSignals(false);
   m_renderView.RefreshGLBuffer();
}


void CRenderViewCCWindow::ExposureSliderChanged()
{
   m_colorCorrectSettings.exposure = ((float)m_exposureSlider->sliderPosition() - 500.f) / 100.f;
   m_colorCorrectSettings.exposureFactor = powf(2.f, m_colorCorrectSettings.exposure);

   m_exposureEdit->blockSignals(true);
   m_exposureEdit->setText(QString::number(m_colorCorrectSettings.exposure));
   m_exposureEdit->blockSignals(false);
   m_renderView.RefreshGLBuffer();
}

void CRenderViewCCWindow::ExposureTextChanged()
{

   QString exposureStr = m_exposureEdit->text();
   m_colorCorrectSettings.exposure = exposureStr.toFloat();
   m_colorCorrectSettings.exposureFactor = powf(2.f, m_colorCorrectSettings.exposure);

   m_exposureSlider->blockSignals(true);
   m_exposureSlider->setValue(int(m_colorCorrectSettings.exposure * 100) + 500);
   m_exposureSlider->blockSignals(false);
   m_renderView.RefreshGLBuffer();
}


void CRenderViewCCWindow::DitherChanged()
{
   m_colorCorrectSettings.dither = m_ditherBox->isChecked();
   m_renderView.RefreshGLBuffer();
}

void CRenderViewCCWindow::ColorSpaceChanged()
{
   m_colorCorrectSettings.space = (CRenderViewColorSpace)m_spaceCombo->currentIndex();

   bool isLut = (m_colorCorrectSettings.space == RV_COLOR_SPACE_LUT_FILE);
   m_lutFileButton->setEnabled(isLut);
   m_lutFileEdit->setEnabled(isLut);
   m_lutFileLabel->setEnabled(isLut);
   m_renderView.RefreshGLBuffer();  
}


void CRenderViewCCWindow::BrowseLutFile()
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
   m_lutFileEdit->setText(lutFilename);

   if (m_colorCorrectSettings.lut3d) delete m_colorCorrectSettings.lut3d;

   m_colorCorrectSettings.lut3d = new CubeLUT;
   int ret = m_colorCorrectSettings.lut3d->LoadCubeFile ( infile );
   infile.close();
   if ( ret != CubeLUT::OK ) 
   {
      cout << "Could not parse the cube info in the input file. Return code = "<< ret << endl;
      return;
   }
   m_renderView.RefreshGLBuffer();

}


void CRenderViewCCWindow::LutFileTextChanged()
{
   QString lutFilename(m_lutFileEdit->text());
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
   m_renderView.RefreshGLBuffer();

}

void CRenderViewCCWindow::AdjustPosition()
{

   QPoint position = parentWidget()->pos();
   m_moving = true;
   move(position.x() + parentWidget()->width() + 15, position.y());
   setFixedSize(300, parentWidget()->height() -2);  
   m_moving = false;
}

// Qt Virtual funcion redefined
void CRenderViewCCWindow::moveEvent(QMoveEvent *event)
{
   if (m_moving) return;

   m_moving = true;
   AdjustPosition();
   m_moving = false;
}


void CubeLUT::ApplyLUT(AtRGB &rgb)
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
