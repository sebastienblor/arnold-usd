
#include "ArnoldHairShader.h"

#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

MTypeId CArnoldHairShaderNode::id(ARNOLD_NODEID_HAIR);

MObject CArnoldHairShaderNode::s_ambdiff;
MObject CArnoldHairShaderNode::s_gloss;
MObject CArnoldHairShaderNode::s_kd_ind;
MObject CArnoldHairShaderNode::s_opacityR;
MObject CArnoldHairShaderNode::s_opacityG;
MObject CArnoldHairShaderNode::s_opacityB;
MObject CArnoldHairShaderNode::s_opacity;
MObject CArnoldHairShaderNode::s_rootcolorR;
MObject CArnoldHairShaderNode::s_rootcolorG;
MObject CArnoldHairShaderNode::s_rootcolorB;
MObject CArnoldHairShaderNode::s_rootcolor;
MObject CArnoldHairShaderNode::s_spec;
MObject CArnoldHairShaderNode::s_spec_colorR;
MObject CArnoldHairShaderNode::s_spec_colorG;
MObject CArnoldHairShaderNode::s_spec_colorB;
MObject CArnoldHairShaderNode::s_spec_color;
MObject CArnoldHairShaderNode::s_tipcolorR;
MObject CArnoldHairShaderNode::s_tipcolorG;
MObject CArnoldHairShaderNode::s_tipcolorB;
MObject CArnoldHairShaderNode::s_tipcolor;
MObject CArnoldHairShaderNode::s_uparam;
MObject CArnoldHairShaderNode::s_vparam;
MObject CArnoldHairShaderNode::s_diffuse_cache;

MObject CArnoldHairShaderNode::s_OUT_colorR;
MObject CArnoldHairShaderNode::s_OUT_colorG;
MObject CArnoldHairShaderNode::s_OUT_colorB;
MObject CArnoldHairShaderNode::s_OUT_color;

MObject CArnoldHairShaderNode::s_point_camera;
MObject CArnoldHairShaderNode::s_normal_camera;
MObject CArnoldHairShaderNode::s_light_data;
MObject CArnoldHairShaderNode::s_light_direction;
MObject CArnoldHairShaderNode::s_light_intensity;
MObject CArnoldHairShaderNode::s_light_ambient;
MObject CArnoldHairShaderNode::s_light_diffuse;
MObject CArnoldHairShaderNode::s_light_specular;
MObject CArnoldHairShaderNode::s_light_shadow_fraction;
MObject CArnoldHairShaderNode::s_light_pre_shadow_intensity;
MObject CArnoldHairShaderNode::s_light_blind_data;
MObject CArnoldHairShaderNode::s_ray_origin;
MObject CArnoldHairShaderNode::s_ray_direction;
MObject CArnoldHairShaderNode::s_object_id;
MObject CArnoldHairShaderNode::s_ray_sampler;
MObject CArnoldHairShaderNode::s_ray_depth;
MObject CArnoldHairShaderNode::s_triangle_normal_camera;

MStatus CArnoldHairShaderNode::compute(const MPlug& plug, MDataBlock& data)
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

      MFloatVector& surfaceColor  = data.inputValue( s_rootcolor ).asFloatVector();
      float diffuseReflectivity = data.inputValue( s_ambdiff ).asFloat();
      MFloatVector& specularColor = data.inputValue( s_spec_color ).asFloatVector();
  
      // Phong shading attributes
      float power = data.inputValue( s_gloss ).asFloat();
      float spec = data.inputValue( s_spec ).asFloat();

      float specularR, specularG, specularB;
      float diffuseR, diffuseG, diffuseB;
      diffuseR = diffuseG = diffuseB = specularR = specularG = specularB = 0;

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
      resultColor[0] = ( diffuseR * surfaceColor[0] ) + ( specularR * specularColor[0]);
      resultColor[1] = ( diffuseG * surfaceColor[1] ) + ( specularG * specularColor[1]);
      resultColor[2] = ( diffuseB * surfaceColor[2] ) + ( specularB * specularColor[2]);
  
      // set ouput color attribute
      MDataHandle outColorHandle = data.outputValue( s_OUT_color );
      MFloatVector& outColor = outColorHandle.asFloatVector();
      outColor = resultColor;
      outColorHandle.setClean();
 
      return MS::kSuccess;
   }
   else        
      return MS::kUnknownParameter;
}

void* CArnoldHairShaderNode::creator()
{
   return new CArnoldHairShaderNode();
}

MStatus CArnoldHairShaderNode::initialize()
{
   MFnNumericAttribute nAttr;
   MFnTypedAttribute   tAttr;
   MFnLightDataAttribute lAttr;
   MFnEnumAttribute eAttr;

   s_ambdiff = nAttr.create("ambdiff", "adf", MFnNumericData::kFloat, 0.6f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_ambdiff);

   s_gloss = nAttr.create("gloss", "gls", MFnNumericData::kFloat, 10.0f);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(2000);
   nAttr.setMin(0);
   nAttr.setMax(5000);
   MAKE_INPUT(nAttr, s_gloss);

   s_kd_ind = nAttr.create("kd_ind", "kdi", MFnNumericData::kFloat, 0.0f);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   MAKE_INPUT(nAttr, s_kd_ind);

   MAKE_COLOR(s_opacity, "opacity", "opa", 1, 1, 1);
   MAKE_INPUT(nAttr, s_opacity);

   MAKE_COLOR(s_rootcolor, "rootcolor", "rtc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_rootcolor);

   s_spec = nAttr.create("spec", "spc", MFnNumericData::kFloat, 1.0f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_spec);

   MAKE_COLOR(s_spec_color, "spec_color", "spcc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_spec_color);

   MAKE_COLOR(s_tipcolor, "tipcolor", "tpc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_tipcolor);

   s_uparam = tAttr.create("uparam", "uprm", MFnData::kString);
   MAKE_INPUT(tAttr, s_uparam); 

   s_vparam = tAttr.create("vparam", "vprm", MFnData::kString);
   MAKE_INPUT(tAttr, s_vparam); 

   s_diffuse_cache = nAttr.create("diffuse_cache", "dche", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_diffuse_cache); 

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 1, 1, 1);
   MAKE_OUTPUT(nAttr, s_OUT_color);

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

   attributeAffects(s_ambdiff, s_OUT_color);
   attributeAffects(s_gloss, s_OUT_color);
   attributeAffects(s_kd_ind, s_OUT_color);
   attributeAffects(s_rootcolor, s_OUT_color);
   attributeAffects(s_spec, s_OUT_color);
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

   return MS::kSuccess;
}
