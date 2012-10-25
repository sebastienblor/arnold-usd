#ifndef SSS_STANDARD_H
#define SSS_STANDARD_H

#include <ai.h>
#include <stdio.h>
#include <math.h>
//#include <string>
//using namespace std;

#define MAX_NB_THREADS 256 // maybe wasting a little memory, better be sure

// OPTIONS 
class COptions
{
public:
   int GI_reflection_depth, GI_refraction_depth;
   // int threads;

   COptions() {}
   COptions(AtNode *in_node)
   {
      GI_reflection_depth = AiNodeGetInt(in_node, "GI_reflection_depth");
      GI_refraction_depth = AiNodeGetInt(in_node, "GI_refraction_depth");
      // threads = AiNodeGetInt(in_node, "threads");
   }
};


// the base node
class CNode
{
private:
public:
   // string name;
   virtual void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options) = 0;

   CNode() {}
   virtual ~CNode() {}
   // CNode(const char* in_name) : name(in_name) {}
};


// STANDARD 
class CStandardOutput
{
public:
   AtColor rgb;
   AtColor opacity;
   CStandardOutput() 
   {
      rgb = AI_RGB_BLACK;
      opacity = AI_RGB_WHITE;
   }
};


class CStandardParams
{
public:
   float     Kd;                               // texturable
   AtColor   Kd_color;                         // texturable
   float     roughness;
   float     Ks;                               // texturable
   AtColor   Ks_color;                         // texturable
   int       specular_brdf;
   float     specular_roughness;
   float     specular_anisotropy;
   float     specular_rotation;
   float     pexp;
   float     Kr;
   AtColor   Kr_color;
   AtColor   reflection_exit_color;
   bool      reflection_exit_use_environment;
   float     Kt;
   AtColor   Kt_color;
   AtColor   transmittance;
   float     refraction_roughness;
   AtColor   refraction_exit_color;
   bool      refraction_exit_use_environment;
   float     IOR;
   float     Kb;
   bool      Fresnel;
   float     Krn;
   bool      spec_Fresnel;                    // texturable
   float     Ksn;                             // texturable
   bool      Fresnel_on_diff;
   float     emission;
   AtColor   emission_color;
   float     direct_specular;                 // texturable
   float     indirect_specular;               // texturable
   float     direct_diffuse;
   float     indirect_diffuse;
   bool      enable_glossy_caustics;
   bool      enable_reflective_caustics;
   bool      enable_refractive_caustics;
   bool      enable_internal_reflections;
   float     Ksss;                            // texturable
   AtColor   Ksss_color;                      // texturable
   AtColor   sss_radius;                      // texturable
   float     bounce_factor;
   AtColor   opacity;

   bool      writeAOVs;
   char      directDiffuseAOV[512];
   char      inDirectDiffuseAOV[512];
   char      primarySpecularAOV[512];

   CStandardParams() // all zero-ed
   {
      Kd = 0.0f; // diffuse scale
      Kd_color = AI_RGB_WHITE;
      roughness = 0.0f;
      Ks = 0.0f; // specular scale
      Ks_color = AI_RGB_WHITE;
      specular_brdf = 2;
      specular_roughness = 0.467f; // as in the compound
      specular_anisotropy = 0.5;;
      specular_rotation = 0.0f;
      pexp = 0.0f;
      Kr = 0.0f; // reflection scale
      Kr_color = AI_RGB_WHITE;
      reflection_exit_color = AI_RGB_BLACK;
      reflection_exit_use_environment = false;
      Kt = 0.0f; // refraction scale
      Kt_color = AI_RGB_WHITE;
      transmittance = AI_RGB_WHITE;
      refraction_roughness = 0.0f;
      refraction_exit_color = AI_RGB_BLACK;
      refraction_exit_use_environment = false;
      IOR = 1.0f;
      Kb = 0.0f;
      Fresnel = false;
      Krn = 0.0f;
      spec_Fresnel = false;
      Ksn = 0.0f;
      Fresnel_on_diff = true;
      emission = 0.0f;
      emission_color = AI_RGB_WHITE;
      direct_specular = 1.0f;
      indirect_specular = 1.0f;
      direct_diffuse = 1.0f;
      indirect_diffuse = 1.0f;
      enable_glossy_caustics = false;
      enable_reflective_caustics = false;
      enable_refractive_caustics = false;
      enable_internal_reflections = true;
      Ksss = 0.0f; // OFF BY DEFAULT
      Ksss_color = AI_RGB_WHITE;
      sss_radius = 0.1f;
      bounce_factor = 1.0f;
      opacity = AI_RGB_WHITE;

      writeAOVs = false;
   }
};


// All the nodes classes derive from CNode and implement their own Evaluate
class CStandard : public CNode
{
public:
   CStandardParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode *p_Ksss, *p_Ksss_color, *p_sss_radius;

   CStandardOutput outputs[MAX_NB_THREADS];

   CStandard() 
   {
      // parameters that get a connection in the compound
      p_Ksss       = NULL;
      p_Ksss_color = NULL;
      p_sss_radius = NULL;
   }
   // CStandard(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// RAY SWITCH 
class CRaySwitchParams 
{
public:
   AtColor camera;
   AtColor shadow;
   AtColor reflection;
   AtColor refraction;
   AtColor diffuse;
   AtColor glossy;

   CRaySwitchParams()
   {
      camera     = AI_RGB_WHITE;
      shadow     = AI_RGB_WHITE;
      reflection = AI_RGB_WHITE;
      refraction = AI_RGB_WHITE;
      diffuse    = AI_RGB_WHITE;
      glossy     = AI_RGB_WHITE;
   }
};


class CRaySwitch : public CNode
{
public:
   CRaySwitchParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode *p_camera;
   CNode *p_shadow;
   CNode *p_reflection;
   CNode *p_refraction;
   CNode *p_diffuse;
   CNode *p_glossy;

   AtColor          outputs[MAX_NB_THREADS];

   CRaySwitch() 
   {
      p_camera = p_shadow = p_reflection = 
      p_refraction = p_diffuse = p_glossy = NULL;
   }
   // CRaySwitch(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// COLOR SWITCH 
class CColorSwitchParams 
{
public:
   AtColor   input1;
   AtColor   input2;
   bool      switcher;

   CColorSwitchParams()
   {
      input1   = AI_RGB_RED;
      input2   = AI_RGB_BLUE;
      switcher = true;
   }
};


class CColorSwitch : public CNode
{
public:
   CColorSwitchParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode              *p_input1, *p_input2;
   AtColor            outputs[MAX_NB_THREADS];

   CColorSwitch() 
   {
      p_input1 = p_input2 = NULL;
   }
   // CColorSwitch(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// SCALAR 2 COLOR
class CScalarToColorParams
{
public:
   float input, alpha;

   CScalarToColorParams()
   {
      input = 0.5f;
      alpha = 1.0f;
   }
};


class CScalarToColor : public CNode
{
public:
   CScalarToColorParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode                *p_input;
   AtColor              outputs[MAX_NB_THREADS];

   CScalarToColor() 
   {
      p_input = NULL;
   }
   // CScalarToColor(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// SCALAR MULTIPLY
class CScalarMultiplyParams 
{
public:
   float input1, input2;

   CScalarMultiplyParams()
   {
      input1 = 0.5f;
      input2 = 1.0f;
   }
};


class CScalarMultiply : public CNode
{
public:
   CScalarMultiplyParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode                 *p_input1, *p_input2;
   float                 outputs[MAX_NB_THREADS];

   CScalarMultiply() 
   {
      p_input1 = p_input2 = NULL;
   }
   // CScalarMultiply(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// SCALAR ADD
class CScalarAddParams 
{
public:
   float input1, input2;

   CScalarAddParams()
   {
      input1 = 0.0f;
      input2 = 0.0f;
   }
};


class CScalarAdd : public CNode
{
public:
   CScalarAddParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode          *p_input1, *p_input2;
   float          outputs[MAX_NB_THREADS];

   CScalarAdd() 
   {
      p_input1 = p_input2 = NULL;
   }
   // CScalarAdd(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// COLORS ADD
class CColorsAddParams 
{
public:
   AtColor baseColor, color1, color2, color3, color4;

   CColorsAddParams()
   {
      baseColor = AI_RGB_BLACK;
      color1 = AI_RGB_BLACK;
      color2 = AI_RGB_BLACK;
      color3 = AI_RGB_BLACK;
      color4 = AI_RGB_BLACK;
   }
};


class CColorsAdd : public CNode
{
public:
   CColorsAddParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode            *p_baseColor, *p_color1, *p_color2, *p_color3, *p_color4;
   AtColor          outputs[MAX_NB_THREADS];

   CColorsAdd() 
   {
      p_baseColor = p_color1 = p_color2 = p_color3 = p_color4 = NULL;
   }
   // CColorsAdd(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};



// COLORS ADD BOUND
class CColorClipParams 
{
public:
   AtColor color;

   CColorClipParams()
   {
      color = AI_RGB_BLACK;
   }
};


class CColorClip : public CNode
{
public:
   CColorClipParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode            *p_color;
   AtColor          outputs[MAX_NB_THREADS];

   CColorClip() 
   {
      p_color = NULL;
   }
   // CColorClip(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


// COLORS SCREEN
class CColorsScreenParams 
{
public:
   AtColor baseColor, color1, color2, color3, color4;

   CColorsScreenParams()
   {
      baseColor = AI_RGB_BLACK;
      color1 = AI_RGB_BLACK;
      color2 = AI_RGB_BLACK;
      color3 = AI_RGB_BLACK;
      color4 = AI_RGB_BLACK;
   }
};


class CColorsScreen : public CNode
{
public:
   CColorsScreenParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode               *p_baseColor, *p_color1, *p_color2, *p_color3, *p_color4;
   AtColor             outputs[MAX_NB_THREADS];

   CColorsScreen() 
   {
      p_baseColor = p_color1 = p_color2 = p_color3 = p_color4 = NULL;
   }
   // CColorsScreen(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};



// COLORS DIVIDE
class CColorsDivideParams 
{
public:
   AtColor color1, color2;

   CColorsDivideParams()
   {
      color1 = AI_RGB_BLACK;
      color2.r = color2.g = color2.b = 4.0f;
   }
};


class CColorsDivide : public CNode
{
public:
   CColorsDivideParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode               *p_color1, *p_color2;
   AtColor             outputs[MAX_NB_THREADS];

   CColorsDivide() 
   {
      p_color1 = p_color2 = NULL;
   }
   // CColorsDivide(const char* in_name) : CNode(in_name) {}
   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};



// COLOR STORE IN CHANNEL
class CColorStoreInChannelParams 
{
public:
   char    channel[512];
   AtRGBA  input;

   CColorStoreInChannelParams()
   {
      input = AI_RGBA_BLACK;
   }
};


class CColorStoreInChannel : public CNode
{
public:
   CColorStoreInChannelParams params[MAX_NB_THREADS];
   // parameters that get a connection in the compound
   CNode   *p_input;
   AtRGBA  outputs[MAX_NB_THREADS];

   CColorStoreInChannel() 
   {
      p_input = NULL;
   }

   void *Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options);
};


/////////////////////////////
// SSS PARAMETERS
/////////////////////////////

enum SSSParams {
   p_diffuse_color,
   p_diffuse_weight,
   p_diffuse_roughness,
   p_sss_weight,
   p_shallow_scatter_color,
   p_shallow_scatter_weight,
   p_shallow_scatter_radius,
   p_mid_scatter_color,
   p_mid_scatter_weight,
   p_mid_scatter_radius,
   p_deep_scatter_color,
   p_deep_scatter_weight,
   p_deep_scatter_radius,
   p_primary_reflection_color,
   p_primary_reflection_weight,
   p_primary_reflection_roughness,
   p_primary_reflection_specular_weight,
   p_primary_reflection_reflection_weight,
   p_primary_reflection_enable_fresnel_falloff,
   p_primary_reflection_fresnel_coefficient,
   p_secondary_reflection_color,
   p_secondary_reflection_weight,
   p_secondary_reflection_roughness,
   p_secondary_reflection_specular_weight,
   p_secondary_reflection_reflection_weight,
   p_secondary_reflection_enable_fresnel_falloff,
   p_secondary_reflection_fresnel_coefficient,
   p_global_sss_radius_multiplier,
   p_use_screen_compositing_for_colors,
   p_sample_sss_only_in_gi_rays,
   p_sample_sss_only_in_glossy_rays,
   p_aov_direct_diffuse,
   p_aov_indirect_diffuse,
   p_aov_primary_specular,
   p_aov_secondary_specular,
   p_aov_shallow_scatter,
   p_aov_mid_scatter,
   p_aov_deep_scatter
};

class CInstanceData;

class CSSSParams
{
public:
   AtColor diffuse_color;
   float diffuse_weight;
   float roughness;
   float sss_weight;
   AtColor shallow_scatter_color;
   float shallow_scatter_weight;
   float shallow_scatter_radius;
   AtColor mid_scatter_color;
   float mid_scatter_weight;
   float mid_scatter_radius;
   AtColor deep_scatter_color;
   float deep_scatter_weight;
   float deep_scatter_radius;
   AtColor primary_reflection_color;
   float primary_reflection_weight;
   float primary_reflection_roughness;
   float primary_reflection_specular_weight;
   float primary_reflection_reflection_weight;
   bool  primary_reflection_enable_fresnel_falloff;
   float primary_reflection_fresnel_coefficient;
   AtColor secondary_reflection_color;
   float secondary_reflection_weight;
   float secondary_reflection_roughness;
   float secondary_reflection_specular_weight;
   float secondary_reflection_reflection_weight;
   bool  secondary_reflection_enable_fresnel_falloff;
   float secondary_reflection_fresnel_coefficient;
   float global_sss_radius_multiplier;
   bool  use_screen_compositing_for_colors;
   bool  sample_sss_only_in_gi_rays;
   bool  sample_sss_only_in_glossy_rays;
   
   void Evaluate(AtNode *node, AtShaderGlobals *sg);
   void CopyTo(CInstanceData *iData, AtShaderGlobals *sg);
   void CopyFrom(CInstanceData *iData, AtShaderGlobals *sg);
};


// INSTANCE DATA 
class CInstanceData
{
public:
   COptions   options;

   CInstanceData() 
   {
      // SetNames(); // handy for debugging
   }
   CInstanceData(AtNode *in_optionsNode) : options(in_optionsNode) 
   {
      // SetNames(); // handy for debugging
   }
   /*
   void SetNames()
   {
      Standard_Diffuse_Primary_Specular.name = "Standard_Diffuse_Primary_Specular";
      Standard_Secondary_Specular.name = "Standard_Secondary_Specular";
      Standard.name = "Standard";
      Standard_SSS_Shallow.name = "Standard_SSS_Shallow";
      Standard_SSS_Mid.name = "Standard_SSS_Mid";
      Standard_SSS_Deep.name = "Standard_SSS_Deep";
      Ray_Switch.name = "Ray_Switch";
      Screen_Switch.name = "Screen_Switch";
      Diffuse_Switch.name = "Diffuse_Switch";
      Glossy_Switch.name = "Glossy_Switch";
      Shallow_Radius_Scalar_To_Color.name = "Shallow_Radius_Scalar_To_Color";
      Mid_Radius_Scalar_To_Color.name = "Mid_Radius_Scalar_To_Color";
      Deep_Radius_Scalar_To_Color.name = "Deep_Radius_Scalar_To_Color";
      Radius_To_Color.name = "Radius_To_Color";
      Multiply_ShallowSSS.name = "Multiply_ShallowSSS";
      Multiply_MidSSS.name = "Multiply_MidSSS";
      Multiply_DeepSSS.name = "Multiply_DeepSSS";
      Multiply_Shallow_Radius.name = "Multiply_Shallow_Radius";
      Multiply_Mid_Radius.name = "Multiply_Mid_Radius";
      Multiply_Deep_Radius.name = "Multiply_Deep_Radius";
      Multiply_By_Global_SSS_Weight.name = "Multiply_By_Global_SSS_Weight";
      Multiply_By_Global_SSS_Radius.name = "Multiply_By_Global_SSS_Radius";
      Divide_By_Three.name = "Divide_By_Three";
      Add_Diffuse_Shallow_Weights.name = "Add_Diffuse_Shallow_Weights";
      Add_Scatter_Weight.name = "Add_Scatter_Weight";
      Add_Deep_Weight.name = "Add_Deep_Weight";
      Add_Shallow_Mid_Radius.name = "Add_Shallow_Mid_Radius";
      Add_Deep_Radius.name = "Add_Deep_Radius";
      Screen.name = "Screen";
      Clip_Diffuse_Primary_Specular.name = "Clip_Diffuse_Primary_Specular";
      Clip_Secondary_Specular.name = "Clip_Secondary_Specular";
      Clip_Shallow.name = "Clip_Shallow";
      Clip_Mid.name = "Clip_Mid";
      Clip_Deep.name = "Clip_Deep";
      Divide_By_Four.name = "Divide_By_Four";
      Standards_Add.name = "Standards_Add";
      Colors_Add.name = "Colors_Add";
   }
   */
   CStandard       Standard_Diffuse_Primary_Specular,
                   Standard_Secondary_Specular, 
                   Standard, 
                   Standard_SSS_Shallow, 
                   Standard_SSS_Mid, 
                   Standard_SSS_Deep;
   CRaySwitch      Ray_Switch;
   CColorSwitch    Screen_Switch, 
                   Diffuse_Switch, 
                   Glossy_Switch;
   CScalarToColor  Shallow_Radius_Scalar_To_Color, 
                   Mid_Radius_Scalar_To_Color, 
                   Deep_Radius_Scalar_To_Color, 
                   Radius_To_Color;
   CScalarMultiply Multiply_ShallowSSS, 
                   Multiply_MidSSS, 
                   Multiply_DeepSSS,
                   Multiply_Shallow_Radius, 
                   Multiply_Mid_Radius, 
                   Multiply_Deep_Radius;
   CColorsScreen   Screen;
   CColorClip      Clip_Diffuse_Primary_Specular, 
                   Clip_Secondary_Specular, 
                   Clip_Shallow, Clip_Mid, Clip_Deep;
   CColorsAdd      Standards_Add, 
                   Colors_Add;
   // the four channel storers. The other aovs (primarySpecular, directDiffuse, 
   // inDirectDiffuse) are written directly by Standard_Diffuse_Primary_Specular
   CColorStoreInChannel Store_Secondary_Specular,
                        Store_Shallow_Scatter, 
                        Store_Mid_Scatter,
                        Store_Deep_Scatter;
};

#endif


