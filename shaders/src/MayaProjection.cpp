#include <MayaUtils.h>

#include <ai.h>

#include <cstring>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

AI_SHADER_NODE_EXPORT_METHODS(MayaProjectionMtd);

namespace
{

enum MayaProjectionParams
{
   p_type,
   p_image,
   p_u_angle,
   p_v_angle,
   p_default_color,
   p_color_gain,
   p_color_offset,
   p_alpha_gain,
   p_alpha_offset,
   p_invert,
   p_wrap,
   p_local,
   p_placement_matrix,
   p_fit_type,
   p_fill_type,
   p_linked_camera,
   p_camera_near,
   p_camera_hfov,
   p_camera_aspect,
   p_use_reference_object
};

enum ProjectionType
{
   PT_NONE = 0,
   PT_PLANAR,
   PT_SPHERICAL,
   PT_CYLINDRICAL,
   PT_BALL,
   PT_CUBIC,
   PT_TRIPLANAR,
   PT_CONCENTRIC,
   PT_PERSPECTIVE
};

const char* gs_ProjectionTypeNames[] =
{
   "off",
   "planar",
   "spherical",
   "cylindrical",
   "ball",
   "cubic",
   "tri_planar",
   "concentric",
   "perspective",
   NULL
};

enum FitType
{
   FIT_NONE = 0,
   FIT_CAMERA_FILM_GATE, 
   FIT_CAMERA_RESOLUTION
};

const char *gs_FitTypeNames[] =
{
   "none",
   "camera_film_gate",
   "camera_resolution",
   NULL
};

enum FillType
{
   FILL_FILL = 0,
   FILL_HORIZONTAL,
   FILL_VERTICAL
};

const char *gs_FillTypeNames[] =
{
   "fill",
   "horizontal",
   "vertical",
   NULL
};

enum TargetPoint
{
   TP_SAMPLE = 0,
   TP_SAMPLE_DX,
   TP_SAMPLE_DY
};

inline float Adjust(float v)
{
  return 0.5f * (1.0f + v);
}

inline float FMod(float a, float b=1.0f)
{
   return (fabs(b) < 0.000001f ? a : (a - b * floor(a / b)));
}

inline float WrapDerivative(float d)
{
   if (fabs(d) > 0.5f)
   {
      return (d > 0.0f ? d - 1.0f : 1.0f + d);
   }
   else
   {
      return d;
   }
}

inline bool IsInsideBox(AtVector V)
{
   return (fabs(V.x) <= 1 && fabs(V.y) <= 1 && fabs(V.z) <= 1);
}

inline bool IsInsideSphere(AtVector V)
{
   return (V.x*V.x + V.y*V.y + V.z*V.z <= 1);
}

inline bool IsInsideCylinder(AtVector V)
{
   return ((V.x*V.x + V.z*V.z) <= 1 && fabs(V.y) <= 1);
}

AtPoint2 PlanarMapping(AtVector V)
{
   AtPoint2 st;

   st.x = Adjust(V.x);
   st.y = Adjust(V.y);

   return st;
}

AtPoint2 SphericalMapping(AtVector V, float uAngle, float vAngle)
{
   AtPoint2 st;

   AtVector Vn;
   AiV3Normalize(Vn, V);

   st.x = Adjust(atan2(Vn.x, Vn.z) / uAngle);
   st.y = Adjust(atan2(Vn.y, sqrt(Vn.x*Vn.x + Vn.z*Vn.z)) / vAngle);

   return st;
}

AtPoint2 CylindricalMapping(AtVector V, float uAngle)
{
   AtPoint2 st;

   AtVector Vn;
   AiV3Normalize(Vn, V);

   st.x = Adjust(atan2(Vn.x, Vn.z) * 2.0f / uAngle);
   st.y = Adjust(V.y);

   return st;
}

AtPoint2 CubicMapping(AtVector V)
{
   AtPoint2 st;

   float xAbs = fabs(V.x);
   float yAbs = fabs(V.y);
   float zAbs = fabs(V.z);

   float s = 0;
   float t = 0;
   float a = 0;

   if (xAbs >= yAbs && xAbs >= zAbs)
   {
      a = xAbs;
      s = V.z;
      t = V.y;
   }
   else if (yAbs >= xAbs && yAbs >= zAbs)
   {
      a = yAbs;
      s = V.x;
      t = V.z;
   }
   else if (zAbs >= xAbs && zAbs >= yAbs)
   {
      a = zAbs;
      s = V.x;
      t = V.y;
   }
   else
   {
      // Should never reach this block
   }

   // Avoid zero divide
   st.x = Adjust(s / (a + 0.000001f));
   st.y = Adjust(t / (a + 0.000001f));

   return st;
}

AtPoint2 BallMapping(AtVector V)
{
   AtPoint2 st;
   AtVector Vn;

   AiV3Normalize(Vn, V);

   Vn.z += 1.0f;

   AiV3Normalize(Vn, Vn);

   st.x = Adjust(Vn.x);
   st.y = Adjust(Vn.y);

   return st;
}

AtPoint2 TriPlanarMapping(AtVector V, AtVector N)
{
   AtPoint2 st;

   float ax = N.x > 0.0f ? N.x : -N.x;
   float ay = N.y > 0.0f ? N.y : -N.y;
   float az = N.z > 0.0f ? N.z : -N.z;

   if (ax >= ay && ax > az)
   {
      // yz plane
      st.x = V.z;
      st.y = V.y;
   }
   else if (ay > ax && ay > az)
   {
      // xz plane
      st.x = V.x;
      st.y = V.z;
   }
   else
   {
      // xy plane
      st.x = V.x;
      st.y = V.y;
   }

   st.x = Adjust(st.x);
   st.y = Adjust(st.y);

   return st;
}

AtVector ComputePoint(AtShaderGlobals *sg, TargetPoint which, bool local, AtMatrix *placement, AtMatrix *camera)
{
   AtPoint p;

   switch (which)
   {
   case TP_SAMPLE_DX:
      p = sg->P + (sg->dPdu * sg->dudx + sg->dPdv * sg->dvdx);
      if (local)
      {
         if (camera != 0)
         {
            AiM4PointByMatrixMult(&p, *camera, &p);
         }
         else
         {
            AiM4PointByMatrixMult(&p, sg->Minv, &p);
         }
      }
      break;
   case TP_SAMPLE_DY:
      p = sg->P + (sg->dPdu * sg->dudy + sg->dPdv * sg->dvdy);
      if (local)
      {
         if (camera != 0)
         {
            AiM4PointByMatrixMult(&p, *camera, &p);
         }
         else
         {
            AiM4PointByMatrixMult(&p, sg->Minv, &p);
         }
      }
      break;
   case TP_SAMPLE:
   default:
      if (local)
      {
         if (camera != 0)
         {
            AiM4PointByMatrixMult(&p, *camera, &(sg->P));
         }
         else
         {
            p = sg->Po;
         }
      }
      else
      {
         p = sg->P;
      }
   }

   if (placement)
   {
      AiM4PointByMatrixMult(&p, *placement, &p);
   }

   return AtVector(p);
}

};

node_parameters
{
   AiParameterENUM("projType", 0, gs_ProjectionTypeNames);
   AiParameterRGBA("image", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterFLT("uAngle", 180.0f);
   AiParameterFLT("vAngle", 90.0f);
   AiParameterRGB("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("alphaGain", 1.0f);
   AiParameterFLT("alphaOffset", 0.0f);
   AiParameterBOOL("invert", false);
   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   AiParameterMTX("placementMatrix", AI_M4_IDENTITY);
   AiParameterENUM("fitType", 1, gs_FitTypeNames);
   AiParameterENUM("fitFill", 0, gs_FillTypeNames);
   AiParameterNODE("linkedCamera", NULL);

   AiMetaDataSetBool(mds, "colorGain", "always_linear", true);
   AiMetaDataSetBool(mds, "colorOffset", "always_linear", true);

   // hide from auto-translation
   AiParameterFLT("cameraNearPlane", 1.0f);
   AiMetaDataSetBool(mds, "cameraNearPlane", "maya.hide", true);
   AiParameterFLT("cameraHorizontalFOV", 0.97738438111682457f); // 56 degrees
   AiMetaDataSetBool(mds, "cameraHorizontalFOV", "maya.hide", true);
   AiParameterFLT("cameraAspectRatio", 1.0f);
   AiMetaDataSetBool(mds, "cameraAspectRatio", "maya.hide", true);

   AiParameterBOOL("useReferenceObject", true);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

typedef struct 
{
   AtNode*  camera;
   AtArray* camera_fov;
   float    render_aspect;
   float    image_aspect;
   float    output_aspect;
} ShaderData;

node_initialize
{
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

   AiNodeSetLocalData(node, data);
}

node_update
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   
   AtNode *camera = (AtNode*)AiNodeGetPtr(node, "linkedCamera");
   if (camera != NULL) // Use a custom camera for the perspective projection
      data->camera = camera;
   else
      data->camera = AiUniverseGetCamera();
      
   if (data->camera != NULL)
   {
      AtArray* fov = AiNodeGetArray(data->camera, "fov");
      if(fov != NULL)
      {
         data->camera_fov = AiArrayCopy(fov);
         for (int i = 0; i < data->camera_fov->nkeys; ++i)
            AiArraySetFlt(data->camera_fov, i, ((float)AI_PI * AiArrayGetFlt(data->camera_fov, i)) / 180.f);
      }
   }

   data->image_aspect = 1.0f;
   AtNode *n = AiNodeGetLink(node, "image");
   if (n != NULL)
   {
      const AtParamEntry *pe = AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(n), "filename");
      if (pe != 0 && AiParamGetType(pe) == AI_TYPE_STRING)
      {
         unsigned int tw, th;
         if (AiTextureGetResolution(AiNodeGetStr(n, "filename"), &tw, &th))
         {
            data->image_aspect = float(tw) / float(th);
         }
      }
   }

   data->render_aspect = 1.0f;
   AtNode *univ = AiUniverseGetOptions();
   int xres = AiNodeGetInt(univ, "xres");
   int yres = AiNodeGetInt(univ, "yres");
   data->render_aspect = (float(xres) / float(yres));
   data->output_aspect = AiNodeGetFlt(univ, "aspect_ratio");
}

node_finish
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   AiFree(data);
}

shader_evaluate
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

   int pt = AiShaderEvalParamEnum(p_type);

   float uAngle = AiShaderEvalParamFlt(p_u_angle);
   float vAngle = AiShaderEvalParamFlt(p_v_angle);

   bool wrap   = (AiShaderEvalParamBool(p_wrap) == true);
   bool local  = (AiShaderEvalParamBool(p_local) == true);

   //AtMatrix* mappingCoordinate = AiShaderEvalParamMtx(p_placement_matrix);
   AtMatrix* space = AiShaderEvalParamMtx(p_placement_matrix);

   // AtMatrix ispace;
   // AiM4Invert(*space, ispace);
   // AtMatrix *mappingCoordinate = &ispace;

   AtMatrix *mappingCoordinate = space;

   AtRGBA outColor;

   AtPoint2 st(AI_P2_ZERO), stx(AI_P2_ZERO), sty(AI_P2_ZERO);
   bool mapped = false;
   AtVector P;

   const bool useReferenceObject = AiShaderEvalParamBool(p_use_reference_object);

   AtPoint tmpPts;
   bool usePref = useReferenceObject ? SetRefererencePoints(sg, tmpPts) : false;

   P = ComputePoint(sg, TP_SAMPLE, local, mappingCoordinate, 0);

   switch (pt)
   {
   case PT_PLANAR:
      if (wrap || IsInsideBox(P))
      {
         AtMatrix camm, *pcamm = 0;
         if (local)
         {
            // Maya is an awesome piece of crap!!
            // For planar mapping, local space means camera space
            AiWorldToCameraMatrix(data->camera, sg->time, camm);  
            pcamm = &camm;
            P = ComputePoint(sg, TP_SAMPLE, true, mappingCoordinate, pcamm);
         }
         st = PlanarMapping(P);
         stx = PlanarMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, pcamm));
         sty = PlanarMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, pcamm));
         mapped = true;
      }
      break;
   case PT_SPHERICAL:
      if (wrap || IsInsideSphere(P))
      {
         st = SphericalMapping(P, uAngle, vAngle);
         stx = SphericalMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, 0), uAngle, vAngle);
         sty = SphericalMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, 0), uAngle, vAngle);
         mapped = true;
      }
      break;
   case PT_CYLINDRICAL:
      if (wrap || IsInsideCylinder(P))
      {
         st = CylindricalMapping(P, uAngle);
         stx = CylindricalMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, 0), uAngle);
         sty = CylindricalMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, 0), uAngle);
         mapped = true;
      }
      break;
   case PT_CUBIC:
      if (wrap || IsInsideBox(P))
      {
         st = CubicMapping(P);
         stx = CubicMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, 0));
         sty = CubicMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, 0));
         mapped = true;
      }
      break;
   case PT_BALL:
      if (wrap || IsInsideSphere(P))
      {
         st = BallMapping(P);
         stx = BallMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, 0));
         sty = BallMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, 0));
         mapped = true;
      }
      break;
   case PT_TRIPLANAR:
      if (wrap || IsInsideBox(P))
      {
         AtVector tmpNrm;
         bool useNref = useReferenceObject ? SetRefererenceNormals(sg, tmpNrm) : false;
         // In local use camera space
         AtVector N = sg->N;
         AtMatrix camm, *pcamm = 0;
         if (local)
         {
            // See my comment on planar mapping
            AiWorldToCameraMatrix(data->camera, sg->time, camm);  
            pcamm = &camm;
            P = ComputePoint(sg, TP_SAMPLE, true, mappingCoordinate, pcamm);
            AiM4VectorByMatrixMult(&N, camm, &N);
         }
         AiM4VectorByMatrixMult(&N, *mappingCoordinate, &N);
         st = TriPlanarMapping(P, N);
         // Is there a way to get N for P+dPdx and P+dPdy?
         // Lets hope curvature is not too high
         stx = TriPlanarMapping(ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, pcamm), N);
         sty = TriPlanarMapping(ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, pcamm), N);
         mapped = true;
         if (useNref) RestoreNormals(sg, tmpNrm);
      }
      break;
   case PT_CONCENTRIC:
      // TODO
      break;
   case PT_PERSPECTIVE:
      {
         int fitType = AiShaderEvalParamEnum(p_fit_type);

         if (fitType != FIT_NONE)
         {
            if (wrap)
            {
               AtMatrix camm, *pcamm = 0;
               AiWorldToCameraMatrix(data->camera, sg->time, camm);
               pcamm = &camm;
               
               AtVector Pc = ComputePoint(sg, TP_SAMPLE, local, pcamm, 0);

               if (Pc.z < 0.0f)
               {
                  const float resAR = data->render_aspect;
                  const float imgAR = data->image_aspect;
                  const float outAR = data->output_aspect;
                  const float camAR = AiShaderEvalParamFlt(p_camera_aspect);

                  const float invOutAR = 1.0f / outAR;
                  const float invCamAR = 1.0f / camAR;
                  const float invImgAR = 1.0f / imgAR;

                  float hfov = AiArrayInterpolateFlt(data->camera_fov, sg->time, 0);
                  float nearp = AiShaderEvalParamFlt(p_camera_near);

                  float maxw = nearp * tan(0.5f * hfov);
                  float maxh = maxw / camAR;

                  // Compute u and v scale depending on fit and fill type
                  float uScale = 1.0f;
                  float vScale = 1.0f;

                  int fillType = AiShaderEvalParamEnum(p_fill_type);

                  if (fillType == FILL_FILL)
                  {
                     if (fitType == FIT_CAMERA_RESOLUTION)
                     {
                        vScale = resAR * invCamAR * invOutAR;
                     } 
                  }
                  else if (fillType == FILL_HORIZONTAL)
                  {
                     vScale = imgAR * invCamAR;
                  }
                  else // FILL_VERTICAL
                  {
                     if (fitType == FIT_CAMERA_RESOLUTION)
                     {
                        vScale = resAR * invCamAR * invOutAR;
                        uScale = resAR * invImgAR * invOutAR;
                     }
                     else
                     {
                        uScale = camAR * invImgAR;
                     }
                  }

                  st.x = Adjust(-nearp * uScale * Pc.x / (Pc.z * maxw));
                  st.y = Adjust(-nearp * vScale * Pc.y / (Pc.z * maxh));

                  AtVector Pxc = ComputePoint(sg, TP_SAMPLE_DX, local, pcamm, 0);
                  stx.x = Adjust(-nearp * uScale * Pxc.x / (Pxc.z * maxw));
                  stx.y = Adjust(-nearp * vScale * Pxc.y / (Pxc.z * maxh));

                  AtVector Pyc = ComputePoint(sg, TP_SAMPLE_DY, local, pcamm, 0);
                  sty.x = Adjust(-nearp * uScale * Pyc.x / (Pyc.z * maxw));
                  sty.y = Adjust(-nearp * vScale * Pyc.y / (Pyc.z * maxh));

                  mapped = true;
               }
            }
         }
         else
         {
            if (P.z < 0.0f && (wrap || (P.z >= -1.0f && fabs(P.x) <= -P.z && fabs(P.y) <= -P.z)))
            {
               st.x = Adjust(P.x / -P.z);
               st.y = Adjust(P.y / -P.z);

               AtVector Px = ComputePoint(sg, TP_SAMPLE_DX, local, mappingCoordinate, 0);
               stx.x = Adjust(Px.x / -Px.z);
               stx.y = Adjust(Px.y / -Px.z);

               AtVector Py = ComputePoint(sg, TP_SAMPLE_DY, local, mappingCoordinate, 0);
               sty.x = Adjust(Py.x / -Py.z);
               sty.y = Adjust(Py.y / -Py.z);

               mapped = true;
            }
         }
      }
      break;
   case PT_NONE:
   default:
      break;
   }

   if (mapped)
   {
      bool invert = (AiShaderEvalParamBool(p_invert) == true);

      AtRGB colorGain     = AiShaderEvalParamRGB(p_color_gain);
      AtRGB colorOffset   = AiShaderEvalParamRGB(p_color_offset);
      float alphaGain   = AiShaderEvalParamFlt(p_alpha_gain);
      float alphaOffset = AiShaderEvalParamFlt(p_alpha_offset);

      float u, v, dudx, dudy, dvdx, dvdy;

      u = sg->u;
      v = sg->v;
      dudx = sg->dudx;
      dudy = sg->dudy;
      dvdx = sg->dvdx;
      dvdy = sg->dvdy;

      sg->u = st.x;
      sg->v = st.y;
      sg->dudx = WrapDerivative(FMod(stx.x) - FMod(sg->u));
      sg->dudy = WrapDerivative(FMod(sty.x) - FMod(sg->u));
      sg->dvdx = WrapDerivative(FMod(stx.y) - FMod(sg->v));
      sg->dvdy = WrapDerivative(FMod(sty.y) - FMod(sg->v));

      outColor = AiShaderEvalParamRGBA(p_image);

      sg->u = u;
      sg->v = v;
      sg->dudx = dudx;
      sg->dudy = dudy;
      sg->dvdx = dvdx;
      sg->dvdy = dvdy;

      if (invert == 1)
      {
         outColor.r = 1 - outColor.r;
         outColor.g = 1 - outColor.g;
         outColor.b = 1 - outColor.b;
         outColor.a = 1 - outColor.a;
      }

      // Calculate color balance
      outColor.r = outColor.r * colorGain.r + colorOffset.r;
      outColor.g = outColor.g * colorGain.g + colorOffset.g;
      outColor.b = outColor.b * colorGain.b + colorOffset.b;
      outColor.a = outColor.a * alphaGain + alphaOffset;
   }
   else
   {
      AiRGBtoRGBA(AiShaderEvalParamRGB(p_default_color), outColor);
   }

   if (usePref) RestorePoints(sg, tmpPts);
   sg->out.RGBA = outColor;

}
