#ifndef ARNOLDVOLUMESCATTERINGSHADER_H
#define ARNOLDVOLUMESCATTERINGSHADER_H

#include <maya/MPxNode.h>

class CArnoldVolumeScatteringShaderNode
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
   static MObject s_affect_camera;
   static MObject s_affect_diffuse;
   static MObject s_affect_reflection;
   static MObject s_attenuation;
   static MObject s_density;
   static MObject s_eccentricity;
   static MObject s_importance_sampling;
   static MObject s_mscattering_depth;
   static MObject s_mscattering_samples;
   static MObject s_rgb_attenuationR;
   static MObject s_rgb_attenuationG;
   static MObject s_rgb_attenuationB;
   static MObject s_rgb_attenuation;
   static MObject s_rgb_densityR;
   static MObject s_rgb_densityG;
   static MObject s_rgb_densityB;
   static MObject s_rgb_density;
   static MObject s_samples;
   static MObject s_sampling_pattern;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldVolumeScatteringShaderNode

#endif // ARNOLDVOLUMESCATTERINGSHADER_H
