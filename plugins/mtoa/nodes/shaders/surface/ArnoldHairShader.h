#ifndef ARNOLDHAIRSHADER_H
#define ARNOLDHAIRSHADER_H

#include <maya/MPxNode.h>

class CArnoldHairShaderNode 
   : public MPxNode
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

   // Input Attributes
   static MObject s_ambdiff;
   static MObject s_gloss;
   static MObject s_kd_ind;
   static MObject s_opacityR;
   static MObject s_opacityG;
   static MObject s_opacityB;
   static MObject s_opacity;
   static MObject s_rootcolorR;
   static MObject s_rootcolorG;
   static MObject s_rootcolorB;
   static MObject s_rootcolor;
   static MObject s_spec;
   static MObject s_spec_colorR;
   static MObject s_spec_colorG;
   static MObject s_spec_colorB;
   static MObject s_spec_color;
   static MObject s_tipcolorR;
   static MObject s_tipcolorG;
   static MObject s_tipcolorB;
   static MObject s_tipcolor;
   static MObject s_uparam;
   static MObject s_vparam;
   static MObject s_diffuse_cache;


   // Output Attributes

   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;

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


};  // class CArnoldHairShaderNode

#endif // ARNOLDHAIRSHADER_H
