/*
 * Windows window and event handling
 */


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


#include "renderview.h"


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


// If you add some slots, you'll have to run moc
#include "luts.moc"
