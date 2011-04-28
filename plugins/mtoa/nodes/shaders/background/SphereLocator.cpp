#include "SphereLocator.h"
#include "nodes/ShaderUtils.h"

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
#include <maya/MItDependencyGraph.h>
#include <maya/MStringArray.h>

MTypeId CSphereLocator::id(ARNOLD_NODEID_SPHERE_LOCATOR);

MObject CSphereLocator::s_colorR;
MObject CSphereLocator::s_colorG;
MObject CSphereLocator::s_colorB;
MObject CSphereLocator::s_color;
MObject CSphereLocator::s_format;

MObject CSphereLocator::s_skyRadius;
MObject CSphereLocator::s_skyFacing;

MObject CSphereLocator::s_sampling;
MObject CSphereLocator::s_hwtexalpha;

void* CSphereLocator::creator()
{
   return new CSphereLocator();
}

bool CSphereLocator::isAbstractClass()
{
   return true;
}
void CSphereLocator::DrawUVSphere(float radius, int divisionsX, int divisionsY, int format)
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
      x = y = z = 0;

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
                  x = static_cast<float>(cos(theta * DTOR) * cos(phi * DTOR));
                  y = static_cast<float>(cos(theta * DTOR) * sin(phi * DTOR));
                  z = static_cast<float>(sin(theta * DTOR));
                  // 1st vertex of the quad
                  break;
               case 2:
                  x = static_cast<float>(cos((theta + dtheta) * DTOR) * cos(phi * DTOR));
                  y = static_cast<float>(cos((theta + dtheta) * DTOR) * sin(phi * DTOR));
                  z = static_cast<float>(sin((theta + dtheta) * DTOR));
                  // 2nd vertex of the quad
                  break;
               case 1:
                  x = static_cast<float>(cos((theta + dtheta) * DTOR) * cos((phi + dphi) * DTOR));
                  y = static_cast<float>(cos((theta + dtheta) * DTOR) * sin((phi + dphi) * DTOR));
                  z = static_cast<float>(sin((theta + dtheta) * DTOR));
                  // 3rd vertex of the quad
                  break;
               case 0:
                  x = static_cast<float>(cos(theta * DTOR) * cos((phi + dphi) * DTOR));
                  y = static_cast<float>(cos(theta * DTOR) * sin((phi + dphi) * DTOR));
                  z = static_cast<float>(sin(theta * DTOR));
                  // 4th and last vertex of the quad
                  break;
               default:
                  break;
            }

            if (m_goUVSample)
            {
               AiV3Create(dir, x, -z, -y);
               AiV3Normalize(dir, dir);
               switch (format)
               {
                  case 0: AiMappingMirroredBall(&dir, &u, &v); break;   // Mirrored Ball
                  case 1: AiMappingAngularMap(&dir, &u, &v); break;     // Angular
                  case 2: AiMappingLatLong(&dir, &u, &v); break;        // Latlong (and cubic since cubic is broken)
                  default: AiMappingCubicMap(&dir, &u, &v);
               }
               m_UData[uv_counter] = u;
               m_VData[uv_counter] = v;
            }

            glTexCoord2f(m_UData[uv_counter], m_VData[uv_counter]);
            glNormal3f(x, y, z);
            glVertex3f(x * radius, y * radius, z * radius);

            uv_counter++;
         }
      }
   }
   m_goUVSample = false;
   glEnd();
}

void CSphereLocator::SampleSN(MPlug &colorPlug)
{
   MStatus status;
   MPlugArray conn;
   colorPlug.connectedTo(conn, true, false);
   if (conn.length() > 0)
   {
      // Sample the shading network
      MMatrix mat;
      mat.setToIdentity();
      MFloatMatrix cameraMat(mat.matrix);

      MFloatArray uCoords, vCoords, filterSizes;
      MFloatVectorArray colors, transps;
      MPlugArray conn;
      //colorPlug.connectedTo(conn, true, false);
      //MFnDependencyNode depNodeSkyColor(conn[0].node());

      MItDependencyGraph itDG(colorPlug, MFn::kFileTexture,
                                                      MItDependencyGraph::kUpstream,
                                                      MItDependencyGraph::kBreadthFirst,
                                                      MItDependencyGraph::kNodeLevel,
                                                      &status);

      // Disable automatic pruning so that we can locate a specific plug
      itDG.disablePruningOnFilter();

      // If no texture file node was found, pass in an empty string as the texture filename
      // so that color information is outputted instead
      //
      MFnDependencyNode fileTexture;

      if (!itDG.isDone())
      {
         MObject textureNode = itDG.thisNode();
         fileTexture.setObject(textureNode);

      }


      // Get all the data based on UVs
      MFnDependencyNode fnThisNode(thisMObject());
      AtUInt numSampleBase = NumSampleBase();
      AtUInt numSamples    = numSampleBase * numSampleBase;

      for (AtUInt i = 0; (i < numSampleBase); i++)
      {
         float valuei = static_cast<float>(i) / numSampleBase;
         for (AtUInt j = 0; (j < numSampleBase); j++)
         {
            float valuej = static_cast<float>(j) / numSampleBase;
            uCoords.append(valuej);
            vCoords.append(valuei);
            filterSizes.append(0.001f);
         }
      }

      MString depNodeSkyColorName(fileTexture.name() + ".outColor");


      MStatus status = MRenderUtil::sampleShadingNetwork(depNodeSkyColorName, numSamples, false, false, cameraMat, NULL, &uCoords, &vCoords, NULL, NULL, NULL, NULL, NULL, colors, transps);

      m_colorData = new char[numSamples * 4];
      int alpha = 255;
      for(AtUInt i = 0; (i < colors.length()); i++)
      {
         MFloatVector fv = colors[i];
         fv *= 255;
         m_colorData[(i * 4) + 0] = static_cast<char>(static_cast<int>(fv.x));
         m_colorData[(i * 4) + 1] = static_cast<char>(static_cast<int>(fv.y));
         m_colorData[(i * 4) + 2] = static_cast<char>(static_cast<int>(fv.z));
         m_colorData[(i * 4) + 3] = static_cast<char>(static_cast<int>(alpha));
      }
   }
   m_goSample = false;
}

bool CSphereLocator::setInternalValueInContext(const MPlug &plug, const MDataHandle &handle, MDGContext &context)
{     
   if( plug == s_color || plug == s_sampling )
   {
      m_goSample = true;
   }

   if( plug == s_format )
      m_goUVSample = true;
   
   return MPxLocatorNode::setInternalValueInContext( plug, handle, context );
}


void CSphereLocator::draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   view.beginGL();
   // Get all GL bits
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   OnDraw(view, style, displayStatus);
   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

AtUInt CSphereLocator::NumSampleBase()
{
      MFnDependencyNode fnThisNode(thisMObject());
      return static_cast<AtUInt>(pow(2.0, (fnThisNode.findPlug("sampling").asInt() + 6)));
}

void CSphereLocator::OnDraw(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   MStatus stat;
   const int divisions = 16;
   AtFloat radius;
   int facing;

   MFnDagNode fn(thisMObject());

   MPlug radiusPlug  = fn.findPlug("skyRadius");
   radiusPlug.getValue(radius);

   MPlug facingPlug  = fn.findPlug("skyFacing");
   facingPlug.getValue(facing);

   GLUquadricObj *quadratic;
   GLuint texture;

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
      if (facing == 0 || facing == 2)
         glCullFace(GL_FRONT);
      else if (facing == 1)
         glCullFace(GL_BACK);



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

         glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture);
         glEnable(GL_TEXTURE_2D);
         glTexImage2D(GL_TEXTURE_2D, 0, 4, numSampleBase, numSampleBase , 0, GL_RGBA, GL_UNSIGNED_BYTE, m_colorData);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

         float hwTexAlpha  = fn.findPlug("hwtexalpha").asFloat();
         glColor4f(0.0f, 0.0f, 0.0f, hwTexAlpha);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
         glPolygonMode(GL_BACK, GL_FILL);

         // Our Custom Sphere
         MPlug formatPlug  = fn.findPlug(s_format);
         int format;
         formatPlug.getValue(format);
         DrawUVSphere(radius, divisions*4, divisions*4, format);

         if (facing == 2)
         {
            // we want both face, we need to redraw a second inverted sphere :'(
            glCullFace(GL_BACK);
            DrawUVSphere(radius, divisions*4, divisions*4, format);
            glCullFace(GL_FRONT);
         }
         glDisable(GL_TEXTURE_2D);
      }
      // else, there is a plain colour
      else
      {
         glPolygonMode(GL_BACK, GL_FILL);
         glColor4f(skyColorPlug.child(0).asFloat(), skyColorPlug.child(1).asFloat(), skyColorPlug.child(2).asFloat(), 0.6f);
         gluSphere(quadratic, radius, divisions, divisions);
      }

      if (facing != 2)
         glDisable(GL_CULL_FACE);

      // If it's selected, draw also wireframe
      if (displayStatusInt == 8)
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

   gluDeleteQuadric(quadratic);
   glDeleteTextures(1, &texture);
}

MBoundingBox CSphereLocator::boundingBox() const
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

bool CSphereLocator::isBounded() const
{
   return true;
}

bool CSphereLocator::drawLast() const
{
   return true;
}

bool CSphereLocator::isTransparent() const
{
   return true;
}

bool CSphereLocator::excludeAsLocator() const
{
   return false;
}


MStatus CSphereLocator::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   MAKE_COLOR(s_color, "color", "sc", 1, 1, 1);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setInternal(true);
   addAttribute(s_color);

   s_format = eAttr.create("format", "for", 2);
   eAttr.addField("mirrored_ball", 0);
   eAttr.addField("angular", 1);
   eAttr.addField("latlong", 2);
   eAttr.addField("cubic", 3);
   eAttr.setInternal(true);
   addAttribute(s_format);

   s_skyRadius = nAttr.create("skyRadius", "gskrd", MFnNumericData::kFloat, 1000);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_skyRadius);

   s_skyFacing = eAttr.create("skyFacing", "faci", 0);
   eAttr.addField("front", 0);
   eAttr.addField("back", 1);
   eAttr.addField("both", 2);
   eAttr.setInternal(true);
   addAttribute(s_skyFacing);

   s_sampling = eAttr.create("sampling", "spl", 2);
   eAttr.addField("Low (64x64)", 0);
   eAttr.addField("Medium (128x128)", 1);
   eAttr.addField("High (256x256)", 2);
   eAttr.addField("Highest (512x512)", 3);
   eAttr.setInternal(true);
   addAttribute(s_sampling);

   s_hwtexalpha = nAttr.create("hwtexalpha", "hwta", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_hwtexalpha);

   return MS::kSuccess;
}

