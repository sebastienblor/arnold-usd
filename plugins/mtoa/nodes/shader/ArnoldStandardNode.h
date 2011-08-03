#ifndef ARNOLDSTANDARDNODE_H
#define ARNOLDSTANDARDNODE_H

#include "extension/ExtensionsManager.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

class CArnoldStandardNode
   :  public MPxNode
{

private:

   static MObjectArray s_PlugsAffecting;

public:

   virtual void postConstructor();

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
   static MObject s_diffuse_roughness;
   static MObject s_Kd_colorR;
   static MObject s_Kd_colorG;
   static MObject s_Kd_colorB;
   static MObject s_Kd_color;
   static MObject s_Kr;
   static MObject s_Kr_colorR;
   static MObject s_Kr_colorG;
   static MObject s_Kr_colorB;
   static MObject s_Kr_color;
   static MObject s_reflection_exit_use_environment;
   static MObject s_reflection_exit_colorR;
   static MObject s_reflection_exit_colorG;
   static MObject s_reflection_exit_colorB;
   static MObject s_reflection_exit_color;
   static MObject s_enable_internal_reflections;
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
   static MObject s_refraction_exit_use_environment;
   static MObject s_refraction_exit_colorR;
   static MObject s_refraction_exit_colorG;
   static MObject s_refraction_exit_colorB;
   static MObject s_refraction_exit_color;
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
   static MObject s_OUT_glow_colorR;
   static MObject s_OUT_glow_colorG;
   static MObject s_OUT_glow_colorB;
   static MObject s_OUT_glow_color;
   static MObject s_OUT_matte_opacityR;
   static MObject s_OUT_matte_opacityG;
   static MObject s_OUT_matte_opacityB;
   static MObject s_OUT_matte_opacity;

   // swatch compute attributes
   static MObject s_point_camera;
   static MObject s_normal_camera;
   static MObject s_light_direction;
   static MObject s_light_intensity;
   static MObject s_light_ambient;
   static MObject s_light_diffuse;
   static MObject s_light_specular;
   static MObject s_light_shadow_fraction;
   static MObject s_light_pre_shadow_intensity;
   static MObject s_light_blind_data;
   static MObject s_light_data;
   static MObject s_ray_origin;
   static MObject s_ray_direction;
   static MObject s_object_id;
   static MObject s_ray_sampler;
   static MObject s_ray_depth;
   static MObject s_triangle_normal_camera;

};  // class CArnoldStandardNode

#endif // ARNOLDSTANDARDNODE
