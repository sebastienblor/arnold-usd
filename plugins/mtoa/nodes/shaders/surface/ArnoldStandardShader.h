#ifndef ARNOLD_STANDARD_SHADER_H
#define ARNOLD_STANDARD_SHADER_H

#include <maya/MPxNode.h>

class CArnoldStandardShaderNode
   :  public MPxNode
{

public:

   virtual void postConstructor()
   {
      setMPSafe(true);
   }

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   static void* creator();
   static MStatus initialize();

	static MTypeId id;

   // Input attributes
   static MObject s_Fresnel;
   static MObject s_Fresnel_affect_diff;
   static MObject s_IOR;
   static MObject s_Kb;
   static MObject s_Kd;
   static MObject s_Kd_colorR;
   static MObject s_Kd_colorG;
   static MObject s_Kd_colorB;
   static MObject s_Kd_color;
   static MObject s_Kr;
   static MObject s_Kr_colorR;
   static MObject s_Kr_colorG;
   static MObject s_Kr_colorB;
   static MObject s_Kr_color;
   static MObject s_Krn;
   static MObject s_Ks;
   static MObject s_Ks_colorR;
   static MObject s_Ks_colorG;
   static MObject s_Ks_colorB;
   static MObject s_Ks_color;
   static MObject s_Ksn;
   static MObject s_Ksss;
   static MObject s_Ksss_colorR;
   static MObject s_Ksss_colorG;
   static MObject s_Ksss_colorB;
   static MObject s_Ksss_color;
   static MObject s_Kt;
   static MObject s_Kt_colorR;
   static MObject s_Kt_colorG;
   static MObject s_Kt_colorB;
   static MObject s_Kt_color;
   static MObject s_Phong_exponent;
   static MObject s_bounce_factor;
   static MObject s_enable_glossy_caustics;
   static MObject s_enable_reflective_caustics;
   static MObject s_enable_refractive_caustics;
   static MObject s_direct_diffuse;
   static MObject s_direct_specular;
   static MObject s_emission;
   static MObject s_emission_colorR;
   static MObject s_emission_colorG;
   static MObject s_emission_colorB;
   static MObject s_emission_color;
   static MObject s_indirect_diffuse;
   static MObject s_indirect_specular;
   static MObject s_opacityR;
   static MObject s_opacityG;
   static MObject s_opacityB;
   static MObject s_opacity;
   static MObject s_specular_Fresnel;
   static MObject s_sss_radiusR;
   static MObject s_sss_radiusG;
   static MObject s_sss_radiusB;
   static MObject s_sss_radius;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldStandardShaderNode

#endif // ARNOLD_STANDARD_SHADER_H
