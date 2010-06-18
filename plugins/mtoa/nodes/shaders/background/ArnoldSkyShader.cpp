#include "ArnoldSkyShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrix.h>
#include <maya/MFloatMatrix.h>
#include <maya/MStringArray.h>

MTypeId CArnoldSkyShaderNode::id(ARNOLD_NODEID_SKY);

MObject CArnoldSkyShaderNode::s_XX;
MObject CArnoldSkyShaderNode::s_XY;
MObject CArnoldSkyShaderNode::s_XZ;
MObject CArnoldSkyShaderNode::s_X;
MObject CArnoldSkyShaderNode::s_X_angle;
MObject CArnoldSkyShaderNode::s_YX;
MObject CArnoldSkyShaderNode::s_YY;
MObject CArnoldSkyShaderNode::s_YZ;
MObject CArnoldSkyShaderNode::s_Y;
MObject CArnoldSkyShaderNode::s_Y_angle;
MObject CArnoldSkyShaderNode::s_ZX;
MObject CArnoldSkyShaderNode::s_ZY;
MObject CArnoldSkyShaderNode::s_ZZ;
MObject CArnoldSkyShaderNode::s_Z;
MObject CArnoldSkyShaderNode::s_Z_angle;
MObject CArnoldSkyShaderNode::s_colorR;
MObject CArnoldSkyShaderNode::s_colorG;
MObject CArnoldSkyShaderNode::s_colorB;
MObject CArnoldSkyShaderNode::s_color;
MObject CArnoldSkyShaderNode::s_intensity;
MObject CArnoldSkyShaderNode::s_format;
MObject CArnoldSkyShaderNode::s_casts_shadows;
MObject CArnoldSkyShaderNode::s_primary_visibility;
MObject CArnoldSkyShaderNode::s_visible_in_reflections;
MObject CArnoldSkyShaderNode::s_visible_in_refractions;
MObject CArnoldSkyShaderNode::s_diffuse_visibility;
MObject CArnoldSkyShaderNode::s_glossy_visibility;

MObject CArnoldSkyShaderNode::s_skyRadius;

MObject CArnoldSkyShaderNode::s_OUT_colorR;
MObject CArnoldSkyShaderNode::s_OUT_colorG;
MObject CArnoldSkyShaderNode::s_OUT_colorB;
MObject CArnoldSkyShaderNode::s_OUT_color;
MObject CArnoldSkyShaderNode::s_OUT_transparencyR;
MObject CArnoldSkyShaderNode::s_OUT_transparencyG;
MObject CArnoldSkyShaderNode::s_OUT_transparencyB;
MObject CArnoldSkyShaderNode::s_OUT_transparency;

MObject CArnoldSkyShaderNode::s_sampling;
MObject CArnoldSkyShaderNode::s_hwtexalpha;

void CArnoldSkyShaderNode::DrawUVSphere(float radius, int divisionsX, int divisionsY, int format)
{

   glBegin(GL_QUADS);

   int dtheta, dphi, theta, phi;
   dtheta = 360 / divisionsX;
   dphi    = 360 / divisionsY;
   double DTOR = 0.0174532925;

   int uv_counter = 0;
   if(m_goUVSample)
   {
      int numUVdata = divisionsX * divisionsY * 4;
      m_UData = new float[numUVdata];
      m_VData = new float[numUVdata];
   }

   for(theta = -90; theta <= 90-dtheta; theta+=dtheta) 
   {
      AtVector dir;
      AtFloat u, v;
      AtFloat x, y, z;

      // little fix to get UVS nicely
      //if (theta==90)
        // theta=89.99;

      for(phi = -270; phi <= 90-dphi; phi+=dphi) 
      {
         // little fix to get UVS nicely
         //if (phi==90)
           // phi=89.99;

         for(int numpoint=0; numpoint<4; numpoint++)
         {
            switch(numpoint)
            {
               case 3:
                  x = cos(theta*DTOR) * cos(phi*DTOR);
                  y = cos(theta*DTOR) * sin(phi*DTOR);
                  z = sin(theta*DTOR);
                  // 1st vertex of the quad
                  break;
               case 2:
                  x = cos((theta+dtheta)*DTOR) * cos(phi*DTOR);
                  y = cos((theta+dtheta)*DTOR) * sin(phi*DTOR);
                  z = sin((theta+dtheta)*DTOR);
                  // 2nd vertex of the quad
                  break;
               case 1:
                  x = cos((theta+dtheta)*DTOR) * cos((phi+dphi)*DTOR);
                  y = cos((theta+dtheta)*DTOR) * sin((phi+dphi)*DTOR);
                  z = sin((theta+dtheta)*DTOR);
                  // 3rd vertex of the quad
                  break;
               case 0:
                  x = cos(theta*DTOR) * cos((phi+dphi)*DTOR);
                  y = cos(theta*DTOR) * sin((phi+dphi)*DTOR);
                  z = sin(theta*DTOR);
                  // 4th and last vertex of the quad
                  break;
            }

            if(m_goUVSample)
            {
               AiV3Create(dir, x, -z, -y);
               AiV3Normalize(dir, dir);
               // Mirrored Ball
               if (format==0)
                  AiMappingMirroredBall(&dir, &u, &v);
               else
               {
                  // Angular
                  if (format==1)
                     AiMappingAngularMap(&dir, &u, &v);
                  // Latlong (and cubic since cubic is broken)
                  else
                  {
                     if (format==2)
                        AiMappingLatLong(&dir, &u, &v);
                     else
                        AiMappingCubicMap(&dir, &u, &v);
                  }
               }
               m_UData[uv_counter] = u;
               m_VData[uv_counter] = v;
            }

            glTexCoord2f(m_UData[uv_counter], m_VData[uv_counter]);
            glNormal3f(x, y, z);
            glVertex3f(x*radius, y*radius, z*radius);

            uv_counter++;
         }
      }
   }
   m_goUVSample = false;
   glEnd();
}

void CArnoldSkyShaderNode::SampleSN(const MPlug &colorPlug)
{
   MPlugArray conn;
   colorPlug.connectedTo(conn, true, false);
   if (conn.length()>0)
   {
      // Sample the shading network
      MMatrix mat;
      mat.setToIdentity();
      MFloatMatrix cameraMat( mat.matrix );

      MFloatArray uCoords, vCoords, filterSizes;
      MFloatVectorArray colors, transps;
      MPlugArray conn;
      colorPlug.connectedTo(conn, true, false);
      MFnDependencyNode depNodeSkyColor(conn[0].node());

      // Get all the data based on UVs
      MFnDependencyNode fnThisNode(thisMObject());
      int numSampleBase = NumSampleBase();
      int numSamples    = numSampleBase*numSampleBase;
 
      m_colorDataSize = numSamples*4;

      for (int i=0; i<numSampleBase; i++)
      {
         float valuei = (float)i / (float)numSampleBase;
         for (int j=0; j<numSampleBase; j++)
         {
            float valuej = (float)j / (float)numSampleBase;
            uCoords.append(valuej);
            vCoords.append(valuei);
            filterSizes.append(0.001);
         }
      }

      MString depNodeSkyColorName(depNodeSkyColor.name()+".outColor");
      MStatus status = MRenderUtil::sampleShadingNetwork(depNodeSkyColorName, numSamples, false, false, cameraMat, NULL, &uCoords, &vCoords, NULL, NULL, NULL, NULL, NULL, colors, transps);

      int numSamplesCol = numSamples*4;
      m_colorData = new byte[numSamplesCol];
 
      for(int i=0;i<colors.length();i++)
      {
         MFloatVector fv = colors[i];
         fv *= 255;
         m_colorData[i*4] = static_cast<byte>(static_cast<int>(fv.x));
         m_colorData[(i*4)+1] = static_cast<byte>(static_cast<int>(fv.y));
         m_colorData[(i*4)+2] = static_cast<byte>(static_cast<int>(fv.z));
         m_colorData[(i*4)+3] = static_cast<byte>(static_cast<int>(((transps[i].x)*255)));
      }
   }
   m_goSample = false;
}
MStatus CArnoldSkyShaderNode::setDependentsDirty(const MPlug &plugBeingDirtied, MPlugArray &affectedPlugs)
{
   MObject thisNode = thisMObject();
   MStatus status;
   MFnDependencyNode fnThisNode(thisNode);

   MStringArray attributesAffecting;

   attributesAffecting.append("color");
   attributesAffecting.append("sampling");

   for(int i=0; i<attributesAffecting.length(); i++)
   {
     MString attrName  = fnThisNode.name() + "." + attributesAffecting[i];
     if (plugBeingDirtied.name() == attrName)
     {
        m_goSample = true;
        break;
     }
   }

   MString formatName  = fnThisNode.name() + ".format"; 
   if (plugBeingDirtied.name() == formatName)
      m_goUVSample = true;


   return MS::kSuccess;
}
void CArnoldSkyShaderNode::draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   view.beginGL();
   // Get all GL bits
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   OnDraw(view, style, displayStatus);
   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

int CArnoldSkyShaderNode::NumSampleBase()
{
      MFnDependencyNode fnThisNode(thisMObject());
      int returnValue = pow(2.0,(fnThisNode.findPlug("sampling").asInt()+6));
      return returnValue;
}

void CArnoldSkyShaderNode::OnDraw(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   MStatus stat;
   const int divisions = 16;
   AtFloat radius;

   MFnDagNode fn(thisMObject());

   MPlug radiusPlug  = fn.findPlug("skyRadius");
   radiusPlug.getValue(radius);

   GLUquadricObj *quadratic;

   // do not write to the z buffer.
   glDepthMask(0);

   quadratic=gluNewQuadric();
   gluQuadricNormals(quadratic, GLU_FLAT);
   gluQuadricTexture(quadratic, GL_TRUE);
   glRotatef(90.0, 1.0, 0.0, 0.0);

   int displayStyle  = view.displayStyle();
   int displayStatusInt = displayStatus;

   // 3 means wireframe
   if(displayStyle != 3)
   {
      // Check if we have a texture file or a simple color
      // in our SkyShaderNode
      MPlug skyColorPlug  = fn.findPlug("color");
      MPlugArray conn;
      skyColorPlug.connectedTo(conn, true, false);

      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);

      // If there is a connection
      if (conn.length()>0)
      {
         // check if we need to resample
         if(m_goSample == true)
         {
            MPlug plugColor = fn.findPlug(s_color);
            SampleSN(plugColor);
         }

         // check the number of samples
         int numSampleBase = NumSampleBase();

         GLuint texture;
         glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture);
         glEnable(GL_TEXTURE_2D);
         glTexImage2D(GL_TEXTURE_2D, 0, 4, numSampleBase, numSampleBase , 0, GL_RGBA, GL_UNSIGNED_BYTE, m_colorData);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

         float hwTexAlpha  = fn.findPlug("hwtexalpha").asFloat();
         glColor4f(0.0f, 0.0f, 0.0f, 1 - hwTexAlpha);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
         glPolygonMode(GL_BACK, GL_FILL);

         // Our Custom Sphere
         MPlug formatPlug  = fn.findPlug(s_format);
         int format;
         formatPlug.getValue(format);
         DrawUVSphere(radius, divisions*4, divisions*4, format); 
         glDisable(GL_TEXTURE_2D);
      }
      // else, there is a plain colour
      else
      {
         glPolygonMode(GL_BACK, GL_FILL);
         glColor4f(skyColorPlug.child(0).asFloat(), skyColorPlug.child(1).asFloat(), skyColorPlug.child(2).asFloat(), 0.6f);
         gluSphere(quadratic, radius, divisions, divisions);
      }

      glDisable(GL_CULL_FACE);
      
      // If it's selected, draw also wireframe
      if (displayStatusInt == 6)
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glColor4f(1, 1, 0, 0.2f);
         gluSphere(quadratic, radius, divisions, divisions);
      }

   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glColor4f(1, 1, 0, 0.2f);
      gluSphere(quadratic, radius, divisions, divisions);
   }

   // re-enable depth writes
   glDepthMask(1);
   glDisable(GL_BLEND);
}

MBoundingBox CArnoldSkyShaderNode::boundingBox() const
{
   float scaleX, scaleY, scaleZ, radius;
   MFnDagNode fn(thisMObject());
   MDagPath dp;
   fn.getPath(dp);
   MFnDagNode fnt(dp.transform());

   MPlug scaleXPlug  = fnt.findPlug("scaleX");
   MPlug scaleYPlug  = fnt.findPlug("scaleY");
   MPlug scaleZPlug  = fnt.findPlug("scaleZ");
   MPlug radiusPlug  = fn.findPlug("skyRadius");
   scaleXPlug.getValue(scaleX);
   scaleYPlug.getValue(scaleY);
   scaleZPlug.getValue(scaleZ);
   radiusPlug.getValue(radius);

   scaleX = radius*scaleX/2;
   scaleY = radius*scaleY/2;
   scaleZ = radius*scaleZ/2;

   // expand the bounding box to fit all axes of the locator node
   MBoundingBox bbox;

   bbox.expand(MPoint(-0.5f*scaleX, 0.0f, 0.0f));
   bbox.expand(MPoint(0.5f*scaleX, 0.0f, 0.0f));
   bbox.expand(MPoint(0.0f,-0.5f*scaleY, 0.0f));
   bbox.expand(MPoint(0.0f, 0.5f*scaleY, 0.0f));
   bbox.expand(MPoint(0.0f, 0.0f, -0.5f*scaleZ));
   bbox.expand(MPoint(0.0f, 0.0f, 0.5f*scaleZ));
   return bbox;
}

bool CArnoldSkyShaderNode::isBounded() const 
{
   return true;
}

bool CArnoldSkyShaderNode::drawLast() const 
{
   return true;
}

bool CArnoldSkyShaderNode::isTransparent() const 
{
   return true;
}

bool CArnoldSkyShaderNode::excludeAsLocator() const 
{
   return false;
}

void* CArnoldSkyShaderNode::creator()
{
   return new CArnoldSkyShaderNode();
}

MStatus CArnoldSkyShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   MAKE_COLOR(s_color, "color", "sc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   s_intensity = nAttr.create("intensity", "i", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_intensity);

   MAKE_ENUM(s_format, "format", "for", 1, "sky", "format");

   s_casts_shadows = nAttr.create("casts_shadows", "shd", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_casts_shadows);
   
   s_primary_visibility = nAttr.create("primary_visibility", "pvis", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_primary_visibility);
   
   s_visible_in_reflections = nAttr.create("visible_in_reflections", "rfl", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_visible_in_reflections);
   
   s_visible_in_refractions = nAttr.create("visible_in_refractions", "rfr", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_visible_in_refractions);
   
   s_diffuse_visibility = nAttr.create("diffuse_visibility", "dvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_diffuse_visibility);
   
   s_glossy_visibility = nAttr.create("glossy_visibility", "gvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_glossy_visibility);

   s_skyRadius = nAttr.create("skyRadius", "gskrd", MFnNumericData::kFloat, 10000);
   MAKE_INPUT(nAttr, s_skyRadius);
   
   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   s_sampling = eAttr.create("sampling", "spl", 2);
   eAttr.addField("Low (64x64)", 0);
   eAttr.addField("Medium (128x128)", 1);
   eAttr.addField("High (256x256)", 2);
   eAttr.addField("Highest (512x512)", 3);
   addAttribute(s_sampling);

   s_hwtexalpha = nAttr.create("hwtexalpha", "hwta", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_hwtexalpha);

   return MS::kSuccess;
}

