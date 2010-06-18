#ifndef ARNOLD_SKY_SHADER_H
#define ARNOLD_SKY_SHADER_H

#include <maya/MPxLocatorNode.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MBoundingBox.h>
#include <gl\gl.h>
#include <gl\glu.h>
   

// Environment Mapping needed files
#include <ai_types.h>
#include <ai_vector.h>
// Environment Mapping needed files

class CArnoldSkyShaderNode
   :  public MPxLocatorNode
{

public:

   virtual void postConstructor()
   {
      // Initialize colorData
      m_colorData   = NULL;
      m_UData       = NULL;
      m_VData       = NULL;
      m_goSample    = true;
      m_goUVSample  = true;

      setMPSafe(true);
   }

   virtual void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
   virtual void OnDraw(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status); 
   virtual bool isBounded() const;
   virtual bool drawLast() const;
   virtual bool isTransparent() const;
   virtual bool excludeAsLocator() const;
   virtual MBoundingBox boundingBox() const;
   virtual MStatus setDependentsDirty(const MPlug &plugBeingDirtied, MPlugArray &affectedPlugs);

   static void* creator();
   static MStatus initialize();
   void SampleSN(const MPlug &colorPlug);
   void DrawUVSphere(float radius, int divisionsX, int divisionsY, int format);
   int NumSampleBase();

   /******************************************************************************
      Environment mapping functions
   ******************************************************************************/
   // This is taken directly from arnold, and should be substituted as soon
   // as these functions become available through the API
   void MappingMirroredBall(const AtVector *dir, AtFloat *u, AtFloat *v);
   void MappingAngularMap(const AtVector *dir, AtFloat *u, AtFloat *v);
   void MappingLatLong(const AtVector *dir, AtFloat *u, AtFloat *v);
   float safe_acosf(float x);
   /******************************************************************************
      Environment mapping functions
   ******************************************************************************/


   static MTypeId id;

   // Input attributes
   static MObject s_XX;
   static MObject s_XY;
   static MObject s_XZ;
   static MObject s_X;
   static MObject s_X_angle;
   static MObject s_YX;
   static MObject s_YY;
   static MObject s_YZ;
   static MObject s_Y;
   static MObject s_Y_angle;
   static MObject s_ZX;
   static MObject s_ZY;
   static MObject s_ZZ;
   static MObject s_Z;
   static MObject s_Z_angle;
   static MObject s_colorR;
   static MObject s_colorG;
   static MObject s_colorB;
   static MObject s_color;
   static MObject s_intensity;
   static MObject s_format;
   static MObject s_casts_shadows;
   static MObject s_primary_visibility;
   static MObject s_visible_in_reflections;
   static MObject s_visible_in_refractions;
   static MObject s_diffuse_visibility;
   static MObject s_glossy_visibility;

   static MObject s_skyRadius;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

   static MObject s_sampling;
   static MObject s_hwtexalpha;

   byte *  m_colorData;
   int     m_colorDataSize;
   float * m_UData;
   float * m_VData;

   // Need to check if sampling again is needed
   bool   m_goSample;
   bool   m_goUVSample;

};  // class CArnoldSkyShaderNode

#endif // ARNOLD_SKY_SHADER_H

