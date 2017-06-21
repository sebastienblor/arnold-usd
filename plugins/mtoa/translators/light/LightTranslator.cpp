#include "LightTranslator.h"
#include "nodes/ShaderUtils.h"
#include "attributes/AttrHelper.h"

#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>
#include <maya/MPlugArray.h>
#include <maya/MFnNumericAttribute.h>
#include "translators/NodeTranslatorImpl.h"

#include <vector>
#include <string>

bool CLightTranslator::RequiresMotionData()
{
   return m_impl->m_session->IsMotionBlurEnabled(MTOA_MBLUR_LIGHT);
}

void CLightTranslator::Export(AtNode* light)
{
   MPlug plug;
   AtMatrix matrix;

   // Early out, light isn't visible so no point exporting anything else.
   if (!IsRenderable())
   {
      AiNodeSetDisabled(GetArnoldNode(), true);
      //AiNodeSetFlt(GetArnoldNode(), "intensity",  0.0f);
      return;
   }
   AiNodeSetDisabled(GetArnoldNode(), false);

   // FIXME: processing parameters means setting up links if the plug has an incoming connection
   // this doesn't always make sense in the context of a light.
   if (FindMayaPlug("aiUseColorTemperature").asBool())
   {
      AtRGB color = ConvertKelvinToRGB(FindMayaPlug("aiColorTemperature").asFloat());
      AiNodeSetRGB(light, "color", color.r, color.g, color.b);
   }
   else
      ProcessParameter(light, "color", AI_TYPE_RGB);
   ProcessParameter(light, "intensity", AI_TYPE_FLOAT);
   ProcessParameter(light, "shadow_color", AI_TYPE_RGB);
   AiNodeSetFlt(light,  "exposure",        FindMayaPlug("aiExposure").asFloat());

   const bool norm = FindMayaPlug("aiNormalize").asBool();

   if (norm && IsFinite())
   {
      /*float intensity = AiNodeGetFlt(light, "intensity");
      m_session->ScaleArea(intensity);
      AiNodeSetFlt(light, "intensity", intensity);*/
      float exposure = AiNodeGetFlt(light, "exposure");
      m_impl->m_session->ScaleLightExposure(exposure);
      AiNodeSetFlt(light, "exposure", exposure);
   }

   AiNodeSetBool(light, "cast_shadows",    FindMayaPlug("aiCastShadows").asBool());
   AiNodeSetFlt(light,  "shadow_density",  FindMayaPlug("aiShadowDensity").asFloat());
   
   AiNodeSetInt(light,  "samples",         FindMayaPlug("aiSamples").asInt());
   AiNodeSetBool(light, "normalize",       norm);
   AiNodeSetFlt(light,  "sss",             FindMayaPlug("aiSss").asFloat());
   AiNodeSetFlt(light,  "indirect",        FindMayaPlug("aiIndirect").asFloat());
   AiNodeSetFlt(light,  "volume",          FindMayaPlug("aiVolume").asFloat());
   AiNodeSetInt(light,  "max_bounces",     FindMayaPlug("aiMaxBounces").asInt());
   AiNodeSetInt(light,  "volume_samples",  FindMayaPlug("aiVolumeSamples").asInt());
   
   AiNodeSetStr(light,  "aov",  FindMayaPlug("aiAov").asString().asChar());
   
   AiNodeSetFlt(light,  "diffuse",         FindMayaPlug("aiDiffuse").asFloat());
   AiNodeSetFlt(light,  "specular",        FindMayaPlug("aiSpecular").asFloat());

   GetMatrix(matrix);

   ProcessParameter(light, "filters", AI_TYPE_ARRAY, FindMayaPlug("aiFilters"));
   if (RequiresMotionData())
   {
      AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, GetMotionStep(), matrix);
      AiNodeSetArray(light, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(light, "matrix", matrix);
   }

   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(light, "motion_start", (float)motionStart);
      AiNodeSetFlt(light, "motion_end", (float)motionEnd);
   }

}

void CLightTranslator::ExportMotion(AtNode* light)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, GetMotionStep(), matrix);
}

void CLightTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("cast_shadows");
   helper.MakeInput("shadow_density"); 
   helper.MakeInput("exposure");
   helper.MakeInput("samples");
   helper.MakeInput("normalize");
   helper.MakeInput("filters");
   helper.MakeInput("diffuse");
   helper.MakeInput("specular");
   helper.MakeInput("sss");
   helper.MakeInput("indirect");
   helper.MakeInput("volume");
   helper.MakeInput("max_bounces");
   helper.MakeInput("volume_samples");
   
   helper.MakeInput("aov");
   
   CAttrData data;
   data.defaultValue.BOOL() = false;
   data.name = "aiUseColorTemperature";
   data.shortName = "ai_use_color_temperature";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
   
   data.defaultValue.FLT() = 6500;
   data.name = "aiColorTemperature";
   data.shortName = "ai_color_temperature";
   data.hasSoftMin = true;
   data.hasSoftMax = true;
   data.softMin.FLT() = 1000.f;
   data.softMax.FLT() = 15000.f;
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.channelBox = false;
   data.keyable = false;

   helper.MakeInputFloat(data);
}

double BBSpectrum(double wavelength, double bbTemp)
{
    double wlm = wavelength * 1e-9;   /* Wavelength in meters */

    return (3.74183e-16 * pow(wlm, -5.0)) /
           (exp(1.4388e-2 / (wlm * bbTemp)) - 1.0);
}

AtRGB XYZtoRGB(float x, float y, float z)
{
   // using sRGB color space
   float m[3][3] =  { { 3.2404542f, -1.5371385f, -0.4985314f},
                      {-0.9692660f,  1.8760108f,  0.0415560f},
                      { 0.0556434f, -0.2040259f,  1.0572252f} };

   AtRGB rgb;
   for (int i = 0; i < 3; i++)
      rgb[i] = x * m[i][0] + y * m[i][1] + z * m[i][2];
   return rgb;

}

// reference, public domain code : http://www.fourmilab.ch/documents/specrend/specrend.c

AtRGB CLightTranslator::ConvertKelvinToRGB(float kelvin)
{
   double bbTemp = (double)kelvin;
   static double cie_colour_match[81][3] = {
      {0.0014,0.0000,0.0065}, {0.0022,0.0001,0.0105}, {0.0042,0.0001,0.0201},
      {0.0076,0.0002,0.0362}, {0.0143,0.0004,0.0679}, {0.0232,0.0006,0.1102},
      {0.0435,0.0012,0.2074}, {0.0776,0.0022,0.3713}, {0.1344,0.0040,0.6456},
      {0.2148,0.0073,1.0391}, {0.2839,0.0116,1.3856}, {0.3285,0.0168,1.6230},
      {0.3483,0.0230,1.7471}, {0.3481,0.0298,1.7826}, {0.3362,0.0380,1.7721},
      {0.3187,0.0480,1.7441}, {0.2908,0.0600,1.6692}, {0.2511,0.0739,1.5281},
      {0.1954,0.0910,1.2876}, {0.1421,0.1126,1.0419}, {0.0956,0.1390,0.8130},
      {0.0580,0.1693,0.6162}, {0.0320,0.2080,0.4652}, {0.0147,0.2586,0.3533},
      {0.0049,0.3230,0.2720}, {0.0024,0.4073,0.2123}, {0.0093,0.5030,0.1582},
      {0.0291,0.6082,0.1117}, {0.0633,0.7100,0.0782}, {0.1096,0.7932,0.0573},
      {0.1655,0.8620,0.0422}, {0.2257,0.9149,0.0298}, {0.2904,0.9540,0.0203},
      {0.3597,0.9803,0.0134}, {0.4334,0.9950,0.0087}, {0.5121,1.0000,0.0057},
      {0.5945,0.9950,0.0039}, {0.6784,0.9786,0.0027}, {0.7621,0.9520,0.0021},
      {0.8425,0.9154,0.0018}, {0.9163,0.8700,0.0017}, {0.9786,0.8163,0.0014},
      {1.0263,0.7570,0.0011}, {1.0567,0.6949,0.0010}, {1.0622,0.6310,0.0008},
      {1.0456,0.5668,0.0006}, {1.0026,0.5030,0.0003}, {0.9384,0.4412,0.0002},
      {0.8544,0.3810,0.0002}, {0.7514,0.3210,0.0001}, {0.6424,0.2650,0.0000},
      {0.5419,0.2170,0.0000}, {0.4479,0.1750,0.0000}, {0.3608,0.1382,0.0000},
      {0.2835,0.1070,0.0000}, {0.2187,0.0816,0.0000}, {0.1649,0.0610,0.0000},
      {0.1212,0.0446,0.0000}, {0.0874,0.0320,0.0000}, {0.0636,0.0232,0.0000},
      {0.0468,0.0170,0.0000}, {0.0329,0.0119,0.0000}, {0.0227,0.0082,0.0000},
      {0.0158,0.0057,0.0000}, {0.0114,0.0041,0.0000}, {0.0081,0.0029,0.0000},
      {0.0058,0.0021,0.0000}, {0.0041,0.0015,0.0000}, {0.0029,0.0010,0.0000},
      {0.0020,0.0007,0.0000}, {0.0014,0.0005,0.0000}, {0.0010,0.0004,0.0000},
      {0.0007,0.0002,0.0000}, {0.0005,0.0002,0.0000}, {0.0003,0.0001,0.0000},
      {0.0002,0.0001,0.0000}, {0.0002,0.0001,0.0000}, {0.0001,0.0000,0.0000},
      {0.0001,0.0000,0.0000}, {0.0001,0.0000,0.0000}, {0.0000,0.0000,0.0000}
   };
   
   double X = 0;
   double Y = 0;
   double Z = 0;
   double lambda = 380.0;
   for (int i = 0; lambda < 780.1; i++, lambda += 5.0)
   {
      double Me;
      Me = BBSpectrum(lambda, bbTemp);
      X += Me * cie_colour_match[i][0];
      Y += Me * cie_colour_match[i][1];
      Z += Me * cie_colour_match[i][2];
   }
   const double XYZ = (X + Y + Z);
   X /= XYZ;
   Y /= XYZ;
   Z /= XYZ;

   AtRGB rgb = XYZtoRGB((float)X, (float)Y, (float)Z);
   float w;
   w = (0.f < rgb.r) ? 0.f : rgb.r;
   w = (w < rgb.g) ? w : rgb.g;
   w = (w < rgb.b) ? w : rgb.b;
   
   if (w < 0.f)
   {
      rgb.r -= w;
      rgb.g -= w;
      rgb.b -= w;
   }
   
   float greatest = AiMax(rgb.r, AiMax(rgb.g, rgb.b));
    
   if (greatest > 0)
   {
      rgb.r /= greatest;
      rgb.g /= greatest;
      rgb.b /= greatest;
   }

   return rgb;
}


void CLightTranslator::NodeChanged(MObject& node, MPlug& plug)
{  
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);

   // this plug is dirtied when the light editor is opened
   // so we don't want to take it into account
   if (plugName == ".childIndex")
      return;

   CDagTranslator::NodeChanged(node, plug);
}
