
#include "ArnoldStandardShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

MTypeId CArnoldStandardShaderNode::id(ARNOLD_NODEID_STANDARD);

MObject CArnoldStandardShaderNode::s_Fresnel;
MObject CArnoldStandardShaderNode::s_Fresnel_affect_diff;
MObject CArnoldStandardShaderNode::s_IOR;
MObject CArnoldStandardShaderNode::s_Kb;
MObject CArnoldStandardShaderNode::s_Kd;
MObject CArnoldStandardShaderNode::s_diffuse_roughness;
MObject CArnoldStandardShaderNode::s_Kd_colorR;
MObject CArnoldStandardShaderNode::s_Kd_colorG;
MObject CArnoldStandardShaderNode::s_Kd_colorB;
MObject CArnoldStandardShaderNode::s_Kd_color;
MObject CArnoldStandardShaderNode::s_Kr;
MObject CArnoldStandardShaderNode::s_Kr_colorR;
MObject CArnoldStandardShaderNode::s_Kr_colorG;
MObject CArnoldStandardShaderNode::s_Kr_colorB;
MObject CArnoldStandardShaderNode::s_Kr_color;
MObject CArnoldStandardShaderNode::s_reflection_exit_use_environment;
MObject CArnoldStandardShaderNode::s_reflection_exit_colorR;
MObject CArnoldStandardShaderNode::s_reflection_exit_colorG;
MObject CArnoldStandardShaderNode::s_reflection_exit_colorB;
MObject CArnoldStandardShaderNode::s_reflection_exit_color;
MObject CArnoldStandardShaderNode::s_Krn;
MObject CArnoldStandardShaderNode::s_Ks;
MObject CArnoldStandardShaderNode::s_specular_brdf;
MObject CArnoldStandardShaderNode::s_specular_roughness;
MObject CArnoldStandardShaderNode::s_specular_anisotropy;
MObject CArnoldStandardShaderNode::s_specular_rotation;
MObject CArnoldStandardShaderNode::s_Ks_colorR;
MObject CArnoldStandardShaderNode::s_Ks_colorG;
MObject CArnoldStandardShaderNode::s_Ks_colorB;
MObject CArnoldStandardShaderNode::s_Ks_color;
MObject CArnoldStandardShaderNode::s_Ksn;
MObject CArnoldStandardShaderNode::s_Ksss;
MObject CArnoldStandardShaderNode::s_Ksss_colorR;
MObject CArnoldStandardShaderNode::s_Ksss_colorG;
MObject CArnoldStandardShaderNode::s_Ksss_colorB;
MObject CArnoldStandardShaderNode::s_Ksss_color;
MObject CArnoldStandardShaderNode::s_Kt;
MObject CArnoldStandardShaderNode::s_Kt_colorR;
MObject CArnoldStandardShaderNode::s_Kt_colorG;
MObject CArnoldStandardShaderNode::s_Kt_colorB;
MObject CArnoldStandardShaderNode::s_Kt_color;
MObject CArnoldStandardShaderNode::s_transmittanceR;
MObject CArnoldStandardShaderNode::s_transmittanceG;
MObject CArnoldStandardShaderNode::s_transmittanceB;
MObject CArnoldStandardShaderNode::s_transmittance;
MObject CArnoldStandardShaderNode::s_refraction_exit_use_environment;
MObject CArnoldStandardShaderNode::s_refraction_exit_colorR;
MObject CArnoldStandardShaderNode::s_refraction_exit_colorG;
MObject CArnoldStandardShaderNode::s_refraction_exit_colorB;
MObject CArnoldStandardShaderNode::s_refraction_exit_color;
MObject CArnoldStandardShaderNode::s_Phong_exponent;
MObject CArnoldStandardShaderNode::s_bounce_factor;
MObject CArnoldStandardShaderNode::s_enable_glossy_caustics;
MObject CArnoldStandardShaderNode::s_enable_reflective_caustics;
MObject CArnoldStandardShaderNode::s_enable_refractive_caustics;
MObject CArnoldStandardShaderNode::s_direct_diffuse;
MObject CArnoldStandardShaderNode::s_direct_specular;
MObject CArnoldStandardShaderNode::s_emission;
MObject CArnoldStandardShaderNode::s_emission_colorR;
MObject CArnoldStandardShaderNode::s_emission_colorG;
MObject CArnoldStandardShaderNode::s_emission_colorB;
MObject CArnoldStandardShaderNode::s_emission_color;
MObject CArnoldStandardShaderNode::s_indirect_diffuse;
MObject CArnoldStandardShaderNode::s_indirect_specular;
MObject CArnoldStandardShaderNode::s_opacityR;
MObject CArnoldStandardShaderNode::s_opacityG;
MObject CArnoldStandardShaderNode::s_opacityB;
MObject CArnoldStandardShaderNode::s_opacity;
MObject CArnoldStandardShaderNode::s_specular_Fresnel;
MObject CArnoldStandardShaderNode::s_sss_radiusR;
MObject CArnoldStandardShaderNode::s_sss_radiusG;
MObject CArnoldStandardShaderNode::s_sss_radiusB;
MObject CArnoldStandardShaderNode::s_sss_radius;
MObject CArnoldStandardShaderNode::s_OUT_colorR;
MObject CArnoldStandardShaderNode::s_OUT_colorG;
MObject CArnoldStandardShaderNode::s_OUT_colorB;
MObject CArnoldStandardShaderNode::s_OUT_color;
MObject CArnoldStandardShaderNode::s_OUT_transparencyR;
MObject CArnoldStandardShaderNode::s_OUT_transparencyG;
MObject CArnoldStandardShaderNode::s_OUT_transparencyB;
MObject CArnoldStandardShaderNode::s_OUT_transparency;
MObject CArnoldStandardShaderNode::s_OUT_glow_colorR;
MObject CArnoldStandardShaderNode::s_OUT_glow_colorG;
MObject CArnoldStandardShaderNode::s_OUT_glow_colorB;
MObject CArnoldStandardShaderNode::s_OUT_glow_color;
MObject CArnoldStandardShaderNode::s_OUT_matte_opacityR;
MObject CArnoldStandardShaderNode::s_OUT_matte_opacityG;
MObject CArnoldStandardShaderNode::s_OUT_matte_opacityB;
MObject CArnoldStandardShaderNode::s_OUT_matte_opacity;

MObject CArnoldStandardShaderNode::s_point_camera;
MObject CArnoldStandardShaderNode::s_normal_camera;
MObject CArnoldStandardShaderNode::s_light_data;
MObject CArnoldStandardShaderNode::s_light_direction;
MObject CArnoldStandardShaderNode::s_light_intensity;
MObject CArnoldStandardShaderNode::s_light_ambient;
MObject CArnoldStandardShaderNode::s_light_diffuse;
MObject CArnoldStandardShaderNode::s_light_specular;
MObject CArnoldStandardShaderNode::s_light_shadow_fraction;
MObject CArnoldStandardShaderNode::s_light_pre_shadow_intensity;
MObject CArnoldStandardShaderNode::s_light_blind_data;
MObject CArnoldStandardShaderNode::s_ray_origin;
MObject CArnoldStandardShaderNode::s_ray_direction;
MObject CArnoldStandardShaderNode::s_object_id;
MObject CArnoldStandardShaderNode::s_ray_sampler;
MObject CArnoldStandardShaderNode::s_ray_depth;
MObject CArnoldStandardShaderNode::s_triangle_normal_camera;

MObject CArnoldStandardShaderNode::aResolution;

MObjectArray CArnoldStandardShaderNode::s_PlugsAffecting;

void CArnoldStandardShaderNode::postConstructor()
{
   setMPSafe(true);
}

MStatus CArnoldStandardShaderNode::compute(const MPlug& plug, MDataBlock& data)
{

   if ((plug == s_OUT_color) || (plug.parent() == s_OUT_color))
   {
      MFloatVector resultColor(MFloatVector::zero);

      // get sample surface shading parameters
      MFloatVector normalCamera = data.inputValue(s_normal_camera).asFloatVector();
      MFloatVector pointCamera = data.inputValue(s_point_camera).asFloatVector();
      MFloatVector rayOrigin = data.inputValue(s_ray_origin).asFloatVector();
      MFloatVector rayDirection = data.inputValue(s_ray_direction).asFloatVector();

      // Diffuse shading attributes
      MFloatVector diffColor  = data.inputValue( s_Kd_color ).asFloatVector();
      float diffWeight = data.inputValue( s_Kd ).asFloat();
      float diffRoughness = data.inputValue( s_diffuse_roughness ).asFloat();

      // Specular shading attributes
      MFloatVector specColor  = data.inputValue( s_Ks_color ).asFloatVector();
      short specBrdf = data.inputValue( s_specular_brdf ).asShort();
      float specWeight = data.inputValue( s_Ks ).asFloat();
      float specRoughness = data.inputValue( s_specular_roughness ).asFloat();
      // Ignored, depreciated
      // float specExponent = data.inputValue( s_Phong_exponent ).asFloat();

      MFloatVector diffuseColor(MFloatVector::zero);
      MFloatVector specularColor(MFloatVector::zero);

      // get light list
      MArrayDataHandle lightData = data.inputArrayValue( s_light_data );
      int numLights = lightData.elementCount();
    
      // iterate through light list and get ambient/diffuse values
      for( int count=1; count <= numLights; count++ )
      {
         MDataHandle currentLight = lightData.inputValue();
         MFloatVector& lightColor = currentLight.child(s_light_intensity).asFloatVector();
        
         // Find the blind data
         void*& blindData = currentLight.child( s_light_blind_data ).asAddr();
     
         // find ambient component
         if( currentLight.child(s_light_ambient).asBool() )
         {
            diffuseColor += lightColor;
         }

         MFloatVector& lightDirection = currentLight.child(s_light_direction).asFloatVector();
        
         // diffuse component
         if( (0 != diffWeight) && (true == currentLight.child(s_light_diffuse).asBool()) )
         {
            float diffuseBrdf = 0;
            if ( blindData != NULL )
            {
               diffuseBrdf = MRenderUtil::diffuseReflectance( blindData, lightDirection, pointCamera, normalCamera, true );
            }
            else
            {
               diffuseBrdf = lightDirection * normalCamera;
            }
            if (diffRoughness != 0)
            {
               float sigma2 = diffRoughness * diffRoughness;
               float A = 1.0f - 0.5f * sigma2 / (sigma2 + 0.33f);
               float B = 0.45f * sigma2 / (sigma2 + 0.09f);
               float NdotV = normalCamera * rayDirection;
               float theta_r = acos(NdotV);
               MFloatVector V_perp_N = (rayDirection - normalCamera * NdotV).normal();
               float cos_theta_i = diffuseBrdf;
               float theta_i = acos(cos_theta_i);
               float cos_phi_diff = V_perp_N * (lightDirection - normalCamera * cos_theta_i);
               float alpha = MAX(theta_i, theta_r);
               float beta = MIN(theta_i, theta_r);

               // Simplified Oren-Nayar
               diffuseBrdf *= (A + B * MAX(0,cos_phi_diff) * sin(alpha) * tan(beta));
            }

            if (diffuseBrdf > 0.0f)
            {
               diffuseColor[0] = diffColor[0] * lightColor[0];
               diffuseColor[1] = diffColor[1] * lightColor[1];
               diffuseColor[2] = diffColor[2] * lightColor[2];
               diffuseColor *= diffWeight * diffuseBrdf;
            }
         }
         // specular component
         if( (0 != specWeight) && (true == currentLight.child(s_light_specular).asBool()) )
         {
            float specularBrdf = 0;
            MFloatVector specLightDirection = lightDirection;
            float lightAttenuation = 1.0;
            if ( blindData != NULL )
            {
               specLightDirection = MRenderUtil::maximumSpecularReflection( blindData, lightDirection, pointCamera, normalCamera, rayDirection );
               lightAttenuation = MRenderUtil::lightAttenuation( blindData, pointCamera, normalCamera, false );
            }
            else
            {
            }
            // Are we facing the light
            if ( specLightDirection * normalCamera > 0.0f )
            {
               switch (specBrdf)
               {
                  case AI_SPECULARTYPE_STRETCHEDPHONG :
                  case AI_SPECULARTYPE_WARDDUER :
                  case AI_SPECULARTYPE_COOKTORRANCE :
                  {
                     MFloatVector R = 2 * (specLightDirection * normalCamera) * normalCamera - specLightDirection;
                     float power = 0.5f / SQR(specRoughness) - 0.5f;
                     specularBrdf = powf( - R * rayDirection, power );
                  }
                  break;
                  default :
                  break;
               }
            }

            if (specularBrdf > 0.0f)
            {
               specularColor[0] = specColor[0] * lightColor[0];
               specularColor[1] = specColor[1] * lightColor[1];
               specularColor[2] = specColor[2] * lightColor[2];
               specularColor *= specWeight * specularBrdf;
            }
         }
         if( !lightData.next() ) break;
      }

      resultColor = diffuseColor + specularColor;
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
   {
      return MS::kUnknownParameter;
   }
}

void* CArnoldStandardShaderNode::creator()
{
   return new CArnoldStandardShaderNode();
}

MStatus CArnoldStandardShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute  eAttr;
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

   // s_Kd = nAttr.create("diffuse", "dc", MFnNumericData::kFloat, 0.7f);
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
   // Should use standard names to have viewport feedback
   // s_Kd_color =  nAttr.createColor( "color", "c");
   // nAttr.setDefault(1.0f, 1.0f, 1.0f);
   // MAKE_INPUT(nAttr, s_Kd_color);

   s_Ks = nAttr.create("Ks", "ks", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ks);

   MAKE_ENUM_INPUT(s_specular_brdf, "specular_brdf", "sbrdf", 0, "standard", "specular_brdf");

   s_specular_roughness = nAttr.create("specular_roughness", "srgh", MFnNumericData::kFloat, 0.467138f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_specular_roughness);

   s_specular_anisotropy = nAttr.create("specular_anisotropy", "sani", MFnNumericData::kFloat, 0.5f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_specular_anisotropy);

   s_specular_rotation = nAttr.create("specular_rotation", "srot", MFnNumericData::kFloat, 0.0f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_specular_rotation);

   MAKE_COLOR(s_Ks_color, "Ks_color", "ksc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Ks_color);

   s_Ksn = nAttr.create("Ksn", "ksn", MFnNumericData::kFloat, 0.05f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ksn);

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

   MAKE_COLOR(s_transmittance, "transmittance", "trs", 1, 1, 1);
   MAKE_INPUT(nAttr, s_transmittance);

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

   s_light_ambient = nAttr.create( "lightAmbient", "la", MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;
   nAttr.setHidden(true) ;

   s_light_diffuse = nAttr.create( "lightDiffuse", "ldf", MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_specular = nAttr.create( "lightSpecular", "ls", MFnNumericData::kBoolean);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_shadow_fraction = nAttr.create("lightShadowFraction", "lsf", MFnNumericData::kFloat);
   nAttr.setStorable(false) ;
   nAttr.setHidden(true) ;
   nAttr.setReadable(true) ;
   nAttr.setWritable(false) ;

   s_light_pre_shadow_intensity = nAttr.create("preShadowIntensity", "psi", MFnNumericData::kFloat);
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

   // resolution attribute for maya's hardware renderer
   aResolution = nAttr.create("resolution", "res",  MFnNumericData::kInt, 32);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(aResolution);
   // DEPENDENCIES

   // diffuse
   s_PlugsAffecting.append(s_Kd);
   s_PlugsAffecting.append(s_diffuse_roughness);
   s_PlugsAffecting.append(s_Kd_color);
   // specular
   s_PlugsAffecting.append(s_Ks);
   s_PlugsAffecting.append(s_specular_brdf);
   s_PlugsAffecting.append(s_specular_roughness);
   s_PlugsAffecting.append(s_specular_anisotropy);
   s_PlugsAffecting.append(s_specular_rotation);
   s_PlugsAffecting.append(s_Ks_color);
   s_PlugsAffecting.append(s_Phong_exponent);

   s_PlugsAffecting.append(s_Kt);
   s_PlugsAffecting.append(s_Kt_color);

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
