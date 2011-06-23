
#include "ArnoldStandardNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

MTypeId CArnoldStandardNode::id(ARNOLD_NODEID_STANDARD);

MObject CArnoldStandardNode::s_Fresnel;
MObject CArnoldStandardNode::s_Fresnel_affect_diff;
MObject CArnoldStandardNode::s_IOR;
MObject CArnoldStandardNode::s_Kb;
MObject CArnoldStandardNode::s_Kd;
MObject CArnoldStandardNode::s_diffuse_roughness;
MObject CArnoldStandardNode::s_Kd_colorR;
MObject CArnoldStandardNode::s_Kd_colorG;
MObject CArnoldStandardNode::s_Kd_colorB;
MObject CArnoldStandardNode::s_Kd_color;
MObject CArnoldStandardNode::s_Kr;
MObject CArnoldStandardNode::s_Kr_colorR;
MObject CArnoldStandardNode::s_Kr_colorG;
MObject CArnoldStandardNode::s_Kr_colorB;
MObject CArnoldStandardNode::s_Kr_color;
MObject CArnoldStandardNode::s_reflection_exit_use_environment;
MObject CArnoldStandardNode::s_reflection_exit_colorR;
MObject CArnoldStandardNode::s_reflection_exit_colorG;
MObject CArnoldStandardNode::s_reflection_exit_colorB;
MObject CArnoldStandardNode::s_reflection_exit_color;
MObject CArnoldStandardNode::s_Krn;
MObject CArnoldStandardNode::s_Ks;
MObject CArnoldStandardNode::s_Ks_colorR;
MObject CArnoldStandardNode::s_Ks_colorG;
MObject CArnoldStandardNode::s_Ks_colorB;
MObject CArnoldStandardNode::s_Ks_color;
MObject CArnoldStandardNode::s_Ksn;
MObject CArnoldStandardNode::s_Ksss;
MObject CArnoldStandardNode::s_Ksss_colorR;
MObject CArnoldStandardNode::s_Ksss_colorG;
MObject CArnoldStandardNode::s_Ksss_colorB;
MObject CArnoldStandardNode::s_Ksss_color;
MObject CArnoldStandardNode::s_Kt;
MObject CArnoldStandardNode::s_Kt_colorR;
MObject CArnoldStandardNode::s_Kt_colorG;
MObject CArnoldStandardNode::s_Kt_colorB;
MObject CArnoldStandardNode::s_refraction_exit_use_environment;
MObject CArnoldStandardNode::s_refraction_exit_colorR;
MObject CArnoldStandardNode::s_refraction_exit_colorG;
MObject CArnoldStandardNode::s_refraction_exit_colorB;
MObject CArnoldStandardNode::s_refraction_exit_color;
MObject CArnoldStandardNode::s_Kt_color;
MObject CArnoldStandardNode::s_Phong_exponent;
MObject CArnoldStandardNode::s_bounce_factor;
MObject CArnoldStandardNode::s_enable_glossy_caustics;
MObject CArnoldStandardNode::s_enable_reflective_caustics;
MObject CArnoldStandardNode::s_enable_refractive_caustics;
MObject CArnoldStandardNode::s_direct_diffuse;
MObject CArnoldStandardNode::s_direct_specular;
MObject CArnoldStandardNode::s_emission;
MObject CArnoldStandardNode::s_emission_colorR;
MObject CArnoldStandardNode::s_emission_colorG;
MObject CArnoldStandardNode::s_emission_colorB;
MObject CArnoldStandardNode::s_emission_color;
MObject CArnoldStandardNode::s_indirect_diffuse;
MObject CArnoldStandardNode::s_indirect_specular;
MObject CArnoldStandardNode::s_opacityR;
MObject CArnoldStandardNode::s_opacityG;
MObject CArnoldStandardNode::s_opacityB;
MObject CArnoldStandardNode::s_opacity;
MObject CArnoldStandardNode::s_specular_Fresnel;
MObject CArnoldStandardNode::s_sss_radiusR;
MObject CArnoldStandardNode::s_sss_radiusG;
MObject CArnoldStandardNode::s_sss_radiusB;
MObject CArnoldStandardNode::s_sss_radius;
MObject CArnoldStandardNode::s_OUT_colorR;
MObject CArnoldStandardNode::s_OUT_colorG;
MObject CArnoldStandardNode::s_OUT_colorB;
MObject CArnoldStandardNode::s_OUT_color;
MObject CArnoldStandardNode::s_OUT_transparencyR;
MObject CArnoldStandardNode::s_OUT_transparencyG;
MObject CArnoldStandardNode::s_OUT_transparencyB;
MObject CArnoldStandardNode::s_OUT_transparency;
MObject CArnoldStandardNode::s_OUT_glow_colorR;
MObject CArnoldStandardNode::s_OUT_glow_colorG;
MObject CArnoldStandardNode::s_OUT_glow_colorB;
MObject CArnoldStandardNode::s_OUT_glow_color;
MObject CArnoldStandardNode::s_OUT_matte_opacityR;
MObject CArnoldStandardNode::s_OUT_matte_opacityG;
MObject CArnoldStandardNode::s_OUT_matte_opacityB;
MObject CArnoldStandardNode::s_OUT_matte_opacity;

MObject CArnoldStandardNode::s_point_camera;
MObject CArnoldStandardNode::s_normal_camera;
MObject CArnoldStandardNode::s_light_data;
MObject CArnoldStandardNode::s_light_direction;
MObject CArnoldStandardNode::s_light_intensity;
MObject CArnoldStandardNode::s_light_ambient;
MObject CArnoldStandardNode::s_light_diffuse;
MObject CArnoldStandardNode::s_light_specular;
MObject CArnoldStandardNode::s_light_shadow_fraction;
MObject CArnoldStandardNode::s_light_pre_shadow_intensity;
MObject CArnoldStandardNode::s_light_blind_data;
MObject CArnoldStandardNode::s_ray_origin;
MObject CArnoldStandardNode::s_ray_direction;
MObject CArnoldStandardNode::s_object_id;
MObject CArnoldStandardNode::s_ray_sampler;
MObject CArnoldStandardNode::s_ray_depth;
MObject CArnoldStandardNode::s_triangle_normal_camera;

MObjectArray CArnoldStandardNode::s_PlugsAffecting;

void CArnoldStandardNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   setMPSafe(false);
}

MStatus CArnoldStandardNode::compute(const MPlug& plug, MDataBlock& data)
{

   if ((plug == s_OUT_color) || (plug.parent() == s_OUT_color))
   {
      MFloatVector resultColor(0.0,0.0,0.0);

      // get sample surface shading parameters
      MFloatVector& surfaceNormal = data.inputValue( s_normal_camera ).asFloatVector();
      MFloatVector& cameraPosition = data.inputValue( s_point_camera ).asFloatVector();

      // use for raytracing api enhancement below
      MFloatVector point = cameraPosition;
      MFloatVector normal = surfaceNormal;

      MFloatVector& surfaceColor  = data.inputValue( s_Kd_color ).asFloatVector();
      float diffuseReflectivity = data.inputValue( s_Kd ).asFloat();
      // float translucenceCoeff   = data.inputValue( aTranslucenceCoeff ).asFloat();
      // User-defined Reflection Color Gain
      float reflectGain = data.inputValue( s_Kr ).asFloat();
  
      // Phong shading attributes
      float power = data.inputValue( s_Phong_exponent ).asFloat();
      float spec = data.inputValue( s_Ks ).asFloat();

      float specularR, specularG, specularB;
      float diffuseR, diffuseG, diffuseB;
      diffuseR = diffuseG = diffuseB = specularR = specularG = specularB = 0.0;

      // get light list
      MArrayDataHandle lightData = data.inputArrayValue( s_light_data );
      int numLights = lightData.elementCount();
    
      // iterate through light list and get ambient/diffuse values
      for( int count=1; count <= numLights; count++ )
      {
         MDataHandle currentLight = lightData.inputValue();
         MFloatVector& lightIntensity = currentLight.child(s_light_intensity).asFloatVector();
        
         // Find the blind data
         void*& blindData = currentLight.child( s_light_blind_data ).asAddr();
     
         // find ambient component
         if( currentLight.child(s_light_ambient).asBool() )
         {
            diffuseR += lightIntensity[0];
            diffuseG += lightIntensity[1];
            diffuseB += lightIntensity[2];
         }

         MFloatVector& lightDirection = currentLight.child(s_light_direction).asFloatVector();
        
         if ( blindData == NULL )
         {
            // find diffuse and specular component
            if( currentLight.child(s_light_diffuse).asBool() )
            {                       
               float cosln = lightDirection * surfaceNormal;;                              
               if( cosln > 0.0f )  // calculate only if facing light
               {
                  diffuseR += lightIntensity[0] * cosln * diffuseReflectivity ;
                  diffuseG += lightIntensity[1] * cosln * diffuseReflectivity ;
                  diffuseB += lightIntensity[2] * cosln * diffuseReflectivity ;
               }

               cameraPosition.normalize();
                                                
               if( cosln > 0.0f ) // calculate only if facing light
               {                               
                  float RV = ( ( (2*surfaceNormal) * cosln ) - lightDirection ) * cameraPosition;
                  if( RV > 0.0 ) 
                  {
                     RV = 0.0;
                  }
                  if( RV < 0.0 ) 
                  {
                     RV = -RV;
                  }
   
                  if ( power < 0 ) 
                  {
                     power = -power;
                  }
  
                  float s = spec * powf( RV, power );

                  specularR += lightIntensity[0] * s; 
                  specularG += lightIntensity[1] * s; 
                  specularB += lightIntensity[2] * s; 
               }
            }    
         }
         else
         {
            float cosln = MRenderUtil::diffuseReflectance( blindData, lightDirection, point, surfaceNormal, true );
            if( cosln > 0.0f )  // calculate only if facing light
            {
               diffuseR += lightIntensity[0] * ( cosln * diffuseReflectivity );
               diffuseG += lightIntensity[1] * ( cosln * diffuseReflectivity );
               diffuseB += lightIntensity[2] * ( cosln * diffuseReflectivity );
            }

            cameraPosition.normalize();
                        
            if ( currentLight.child(s_light_specular).asBool() )
            {
               MFloatVector specLightDirection = lightDirection;
               MDataHandle directionH = data.inputValue( s_ray_direction );
               MFloatVector direction = directionH.asFloatVector();
               float lightAttenuation = 1.0;
                                                 
               specLightDirection = MRenderUtil::maximumSpecularReflection( blindData,lightDirection, point, surfaceNormal, direction );
               lightAttenuation = MRenderUtil::lightAttenuation( blindData, point, surfaceNormal, false );             

               // Are we facing the light
               if ( specLightDirection * surfaceNormal > 0.0f )
               {                       
                  MFloatVector rv = 2 * surfaceNormal * ( surfaceNormal * direction ) - direction;
                  float s = spec * powf( rv * specLightDirection, power );
                                                
                  specularR += lightIntensity[0] * s * lightAttenuation; 
                  specularG += lightIntensity[1] * s * lightAttenuation; 
                  specularB += lightIntensity[2] * s * lightAttenuation;
               }
            }
         }
         if( !lightData.next() ) break;
      }

      // factor incident light with surface color 
      resultColor[0] = ( diffuseR * surfaceColor[0] ) + specularR ;
      resultColor[1] = ( diffuseG * surfaceColor[1] ) + specularG ;
      resultColor[2] = ( diffuseB * surfaceColor[2] ) + specularB ;
  
      // add the reflection color
      if (reflectGain > 0.0) 
      {
         MStatus status;
         // required attributes for using raytracer
         // origin, direction, sampler, depth, and object id.
         //
         MDataHandle originH = data.inputValue( s_ray_origin, &status);
         MFloatVector origin = originH.asFloatVector();
 
         MDataHandle directionH = data.inputValue( s_ray_direction, &status);
         MFloatVector direction = directionH.asFloatVector();

         MDataHandle samplerH = data.inputValue( s_ray_sampler, &status);
         void*& samplerPtr = samplerH.asAddr();

         MDataHandle depthH = data.inputValue( s_ray_depth, &status);
         short depth = depthH.asShort();

         MDataHandle objH = data.inputValue( s_object_id, &status);
         void*& objId = objH.asAddr();

         MFloatVector reflectColor;
         MFloatVector reflectTransparency;

         MFloatVector& triangleNormal = data.inputValue( s_triangle_normal_camera ).asFloatVector();

         // compute reflected ray
         MFloatVector l = -direction;
         float dot = l * normal;
         if( dot < 0.0 ) dot = -dot;
         MFloatVector refVector = 2 * normal * dot - l;  // reflection ray
         float dotRef = refVector * triangleNormal;
         if( dotRef < 0.0 ) 
         {
            const float s = 0.01f;
            MFloatVector mVec = refVector - dotRef * triangleNormal;
            mVec.normalize();
            refVector = mVec + s * triangleNormal;
         }
         refVector.normalize();

         status = MRenderUtil::raytrace(
                                point,          //  origin
                                refVector,  //  direction
                                objId,          //  object id
                                samplerPtr, //  sampler info
                                depth,          //  ray depth
                                reflectColor,   // output color and transp
                                reflectTransparency);

         // add in the reflection color
         resultColor[0] += reflectGain * (reflectColor[0]);
         resultColor[1] += reflectGain * (reflectColor[1]);
         resultColor[2] += reflectGain * (reflectColor[2]);
          
      }
 
      // set ouput color attribute
      MDataHandle outColorHandle = data.outputValue( s_OUT_color );
      MFloatVector& outColor = outColorHandle.asFloatVector();
      outColor = resultColor;
      outColorHandle.setClean();
 
      return MS::kSuccess;
   }
   else if ((plug == s_OUT_transparency) || (plug.parent() == s_OUT_transparency))
   {
      float& trFloat ( data.inputValue( s_Kt ).asFloat());
      MFloatVector tr(trFloat, trFloat, trFloat);
      // set ouput color attribute
      MDataHandle outTransHandle = data.outputValue( s_OUT_transparency );
      MFloatVector& outTrans = outTransHandle.asFloatVector();
      outTrans = tr;
      data.setClean( plug );
      return MS::kSuccess;
   }
   else        
      return MS::kUnknownParameter;
}

void* CArnoldStandardNode::creator()
{
   return new CArnoldStandardNode();
}

MStatus CArnoldStandardNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnLightDataAttribute lAttr;

   s_Fresnel = nAttr.create("Fresnel", "frn", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_Fresnel);

   s_Fresnel_affect_diff = nAttr.create("Fresnel_affect_diff", "frndiff", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_Fresnel_affect_diff);

   s_IOR = nAttr.create("IOR", "ior", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_IOR);

   s_Kb = nAttr.create("Kb", "kb", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kb);

   s_Kd = nAttr.create("Kd", "kd", MFnNumericData::kFloat, 0.7f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kd);

   s_diffuse_roughness = nAttr.create("diffuse_roughness", "drgh", MFnNumericData::kFloat, 0.0f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_diffuse_roughness);

   MAKE_COLOR(s_Kd_color, "Kd_color", "kdc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Kd_color);

   s_Kr = nAttr.create("Kr", "kr", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kr);

   MAKE_COLOR(s_Kr_color, "Kr_color", "krc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Kr_color);

   s_Krn = nAttr.create("Krn", "krn", MFnNumericData::kFloat, 0.05f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Krn);

   s_reflection_exit_use_environment = nAttr.create("reflection_exit_use_environment", "rfxee", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_reflection_exit_use_environment);

   MAKE_COLOR(s_reflection_exit_color, "reflection_exit_color", "rfxec", 0.0f, 0.0f, 0.0f);
   MAKE_INPUT(nAttr, s_reflection_exit_color);

   s_Ks = nAttr.create("Ks", "ks", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ks);

   MAKE_COLOR(s_Ks_color, "Ks_color", "ksc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Ks_color);

   s_Ksn = nAttr.create("Ksn", "ksn", MFnNumericData::kFloat, 0.05f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ksn);

   s_Ksss = nAttr.create("Ksss", "ksss", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ksss);

   MAKE_COLOR(s_Ksss_color, "Ksss_color", "ksssc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Ksss_color);

   s_Kt = nAttr.create("Kt", "kt", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kt);

   MAKE_COLOR(s_Kt_color, "Kt_color", "ktc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Kt_color);

   s_refraction_exit_use_environment = nAttr.create("refraction_exit_use_environment", "rfcee", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_refraction_exit_use_environment);

   MAKE_COLOR(s_refraction_exit_color, "refraction_exit_color", "rfcec", 0.0f, 0.0f, 0.0f);
   MAKE_INPUT(nAttr, s_refraction_exit_color);

   s_Phong_exponent = nAttr.create("Phong_exponent", "phonge", MFnNumericData::kFloat, 10);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(2000);
   nAttr.setMin(0);
   nAttr.setMax(5000);
   MAKE_INPUT(nAttr, s_Phong_exponent);

   s_bounce_factor = nAttr.create("bounce_factor", "bouncef", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(4);
   MAKE_INPUT(nAttr, s_bounce_factor);

   s_enable_glossy_caustics = nAttr.create("enable_glossy_caustics", "gcau", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_enable_glossy_caustics);

   s_enable_reflective_caustics = nAttr.create("enable_reflective_caustics", "rcau", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_enable_reflective_caustics);

   s_enable_refractive_caustics = nAttr.create("enable_refractive_caustics", "fcau", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_enable_refractive_caustics);

   s_direct_diffuse = nAttr.create("direct_diffuse", "directd", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_direct_diffuse);

   s_direct_specular = nAttr.create("direct_specular", "directs", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_direct_specular);

   s_emission = nAttr.create("emission", "emiss", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_emission);

   MAKE_COLOR(s_emission_color, "emission_color", "emissc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_emission_color);

   s_indirect_diffuse = nAttr.create("indirect_diffuse", "indirectd", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_indirect_diffuse);

   s_indirect_specular = nAttr.create("indirect_specular", "indirects", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_indirect_specular);

   MAKE_COLOR(s_opacity, "opacity", "opac", 1, 1, 1);
   MAKE_INPUT(nAttr, s_opacity);

   s_specular_Fresnel = nAttr.create("specular_Fresnel", "specf", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_specular_Fresnel);

   MAKE_COLOR(s_sss_radius, "sss_radius", "sssr", 0.1f, 0.1f, 0.1f);
   MAKE_INPUT(nAttr, s_sss_radius);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   MAKE_COLOR(s_OUT_glow_color, "outGlowColor", "ogc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_glow_color);

   MAKE_COLOR(s_OUT_matte_opacity, "outMatteOpacity", "omo", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_matte_opacity);

   // OUTPUT ATTRIBUTES

   s_point_camera = nAttr.createPoint( "pointCamera", "pc" );
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setDefault(1.0f, 1.0f, 1.0f) ;
   nAttr.setHidden(true) ;

   s_normal_camera = nAttr.createPoint( "normalCamera", "n" );
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setDefault(1.0f, 1.0f, 1.0f) ;
   nAttr.setHidden(true) ;

   s_triangle_normal_camera = nAttr.createPoint( "triangleNormalCamera", "tn" );
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setDefault(1.0f, 1.0f, 1.0f);
   nAttr.setHidden(true);

   s_light_direction = nAttr.createPoint( "lightDirection", "ld" );
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;
   nAttr.setDefault(1.0f, 1.0f, 1.0f) ;

   s_light_intensity = nAttr.createColor( "lightIntensity", "li" );
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;
   nAttr.setDefault(1.0f, 1.0f, 1.0f) ;

   s_light_ambient = nAttr.create( "lightAmbient", "la",
                                                                  MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;
   nAttr.setHidden(true) ;

   s_light_diffuse = nAttr.create( "lightDiffuse", "ldf", 
                                                                  MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_specular = nAttr.create( "lightSpecular", "ls", 
                                                                  MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_shadow_fraction = nAttr.create("lightShadowFraction", "lsf",
                                                                                MFnNumericData::kFloat);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_pre_shadow_intensity = nAttr.create("preShadowIntensity", "psi",
                                                                           MFnNumericData::kFloat);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_blind_data = nAttr.createAddr("lightBlindData", "lbld");
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_data = lAttr.create( "lightDataArray", "ltd", 
                              s_light_direction, s_light_intensity, s_light_ambient,
                              s_light_diffuse, s_light_specular, 
                              s_light_shadow_fraction,
                              s_light_pre_shadow_intensity,
                              s_light_blind_data);
   lAttr.setArray(true) ;
   lAttr.setStorable(false) ;
   lAttr.setHidden(true) ;
   lAttr.setDefault(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, true, true,
                                         false, 0.0f, 1.0f, NULL) ;

   // rayOrigin
   MObject RayX = nAttr.create( "rayOx", "rxo", MFnNumericData::kFloat, 0.0 );
   MObject RayY = nAttr.create( "rayOy", "ryo", MFnNumericData::kFloat, 0.0 );
   MObject RayZ = nAttr.create( "rayOz", "rzo", MFnNumericData::kFloat, 0.0 );
   s_ray_origin = nAttr.create( "rayOrigin", "rog", RayX, RayY, RayZ );
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(false) ;

   // rayDirection 
   RayX = nAttr.create( "rayDirectionX", "rdx", MFnNumericData::kFloat, 1.0 );
   RayY = nAttr.create( "rayDirectionY", "rdy", MFnNumericData::kFloat, 0.0 );
   RayZ = nAttr.create( "rayDirectionZ", "rdz", MFnNumericData::kFloat, 0.0 );
   s_ray_direction = nAttr.create( "rayDirection", "rad", RayX, RayY, RayZ );
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(false) ;

   // objectId
   s_object_id = nAttr.createAddr( "objectId", "oi" );
   nAttr.setStorable(false) ; 
   nAttr.setHidden(true) ;
   nAttr.setReadable(false) ;

   // raySampler
   s_ray_sampler = nAttr.createAddr("raySampler", "rtr");
   nAttr.setStorable(false);
   nAttr.setHidden(true) ;
   nAttr.setReadable(false) ;

   // rayDepth
   s_ray_depth = nAttr.create( "rayDepth", "rd", MFnNumericData::kShort, 0.0 );
   nAttr.setStorable(false) ;
   nAttr.setHidden(true)  ;
   nAttr.setReadable(false) ;

   addAttribute(s_point_camera) ;
   addAttribute(s_normal_camera) ;
   addAttribute(s_triangle_normal_camera) ;
   addAttribute(s_light_data) ;
   addAttribute(s_ray_origin) ;
   addAttribute(s_ray_direction) ;
   addAttribute(s_object_id) ;
   addAttribute(s_ray_sampler) ;
   addAttribute(s_ray_depth) ;


   // DEPENDENCIES

   s_PlugsAffecting.append(s_Kd);
   s_PlugsAffecting.append(s_Kd_color);
   s_PlugsAffecting.append(s_Ks);
   s_PlugsAffecting.append(s_Ks_color);
   s_PlugsAffecting.append(s_Kt);
   s_PlugsAffecting.append(s_Kt_color);
   s_PlugsAffecting.append(s_Phong_exponent);
   s_PlugsAffecting.append(s_bounce_factor);
   s_PlugsAffecting.append(s_emission);

   for(AtUInt i = 0; (i < s_PlugsAffecting.length()); i++)
   {
      attributeAffects(s_PlugsAffecting[i], s_OUT_color);
   }

   attributeAffects(s_light_intensity, s_OUT_color);
   attributeAffects(s_point_camera, s_OUT_color);
   attributeAffects(s_normal_camera, s_OUT_color);
   attributeAffects(s_triangle_normal_camera, s_OUT_color);
   attributeAffects(s_light_data, s_OUT_color);
   attributeAffects(s_light_ambient, s_OUT_color);
   attributeAffects(s_light_specular, s_OUT_color);
   attributeAffects(s_light_diffuse, s_OUT_color);
   attributeAffects(s_light_direction, s_OUT_color);
   attributeAffects(s_light_shadow_fraction, s_OUT_color);
   attributeAffects(s_light_pre_shadow_intensity, s_OUT_color);
   attributeAffects(s_light_blind_data, s_OUT_color);
   attributeAffects(s_ray_origin,s_OUT_color);
   attributeAffects(s_ray_direction,s_OUT_color);
   attributeAffects(s_object_id,s_OUT_color);
   attributeAffects(s_ray_sampler,s_OUT_color);
   attributeAffects(s_ray_depth,s_OUT_color);

   attributeAffects(s_Kt, s_OUT_transparency);

   return MS::kSuccess;
}
