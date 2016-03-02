#include "SphereLocator.h"
#include "nodes/ShaderUtils.h"

#include <ai.h>

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

CSphereLocator::CSphereLocator()
{
}

CSphereLocator::~CSphereLocator()
{
}

bool CSphereLocator::isAbstractClass()
{
   return true;
}

AtVector SphereVertex(float phi, float theta)
{
   AtVector ret;
   ret.y = cosf(theta);
   float t = sinf(theta);
   ret.x = t * sinf(phi);
   ret.z = t * cosf(phi);
   return ret;
}

void SphereVertexGL(float radius, float phi, float theta)
{
   float y = cosf(theta) * radius;
   float t = sinf(theta) * radius;
   float x = t * sinf(phi);
   float z = t * cosf(phi);
   glVertex3f(x, y, z);
} 

void CSphereLocator::DrawUVSphere(float radius, int divisionsX, int divisionsY, int format, bool needsUV)
{
   const int numIndices = divisionsX * divisionsY * 6;
   const int divisionsX1 = divisionsX + 1;
   const int divisionsY1 = divisionsY + 1;
   const int numVertices = divisionsX1 * divisionsY1;
   bool rebuildCache = false;
   if (radius != m_cachedRadius)
      rebuildCache = true;
   else if (divisionsX != m_cachedDivisionX)
      rebuildCache = true;
   else if (divisionsY != m_cachedDivisionY)
      rebuildCache = true;
   else if (format != m_cachedFormat)
      rebuildCache = true;
   if (rebuildCache)
   {
      m_cachedRadius = radius;
      m_cachedDivisionX = divisionsX;
      m_cachedDivisionY = divisionsY;
      m_cachedFormat = format;
      
      m_UVData.resize(numVertices);
      m_positionData.resize(numVertices);
      m_indexData.resize(numIndices);

      AtVector dir;
      float u, v;

      for (int x = 0; x < divisionsX1; ++x)
      {
         const float phi = (float)AI_PITIMES2 * (float)x / (float)divisionsX;
         
         for (int y = 0; y < divisionsY1; ++y)
         {         
            const float theta = (float)AI_PI * (float)y / (float)divisionsY;
            dir = SphereVertex(phi, theta);            
            switch (format)
            {
               case 0: AiMappingMirroredBall(&dir, &u, &v); break;   // Mirrored Ball
               case 1: AiMappingAngularMap(&dir, &u, &v); break;     // Angular
               case 2: AiMappingLatLong(&dir, &u, &v); break;        // Latlong
               default: AiMappingLatLong(&dir, &u, &v);
            }
            const int id = x + y * divisionsX1;
            m_UVData[id].x = u;
            m_UVData[id].y = v;
            m_positionData[id] = dir * radius;
         }
      }

      int indexCounter = 0;
      for (unsigned int x = 0; x < (unsigned int)divisionsX; ++x)
      {
         const int x1 = x + 1;
         for (unsigned int y = 0; y < (unsigned int)divisionsY; ++y)
         {
            const int y1 = y + 1;
            m_indexData[indexCounter++] = x + y * divisionsX1;
            m_indexData[indexCounter++] = x1 + y * divisionsX1;
            m_indexData[indexCounter++] = x + y1 * divisionsX1;

            m_indexData[indexCounter++] = x1 + y * divisionsX1;
            m_indexData[indexCounter++] = x1 + y1 * divisionsX1;
            m_indexData[indexCounter++] = x + y1 * divisionsX1;

         }  
      }
   }

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &m_positionData[0]);
   if (needsUV)
   {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, &m_UVData[0]);
   }

   glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, &m_indexData[0]);

   if (needsUV)
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
}

void CSphereLocator::DrawSphereWireframe(float radius, int divisionsX, int divisionsY)
{
   glBegin(GL_LINES);
   
   for (int x = 0; x < divisionsX; ++x)
   {
      float phiB = (float)AI_PITIMES2 * (float)x / (float)divisionsX;
      float phiE = (float)AI_PITIMES2 * (float)(x + 1) / (float)divisionsX;
      for (int y = 0; y < divisionsY; ++y)
      {
         float thetaB = (float)AI_PI * (float)y / (float)divisionsY;
         float thetaE = (float)AI_PI * (float)(y + 1) / (float)divisionsY;

         SphereVertexGL(radius, phiB, thetaB);
         SphereVertexGL(radius, phiE, thetaB);
         
         SphereVertexGL(radius, phiB, thetaE);
         SphereVertexGL(radius, phiE, thetaE);
         
         SphereVertexGL(radius, phiB, thetaB);
         SphereVertexGL(radius, phiB, thetaE);
         
         SphereVertexGL(radius, phiE, thetaB);
         SphereVertexGL(radius, phiE, thetaE);
      }
   }
   
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
      unsigned int numSampleBase = NumSampleBase();
      unsigned int numSamples    = numSampleBase * numSampleBase;

      for (unsigned int i = 0; (i < numSampleBase); i++)
      {
         float valuei = static_cast<float>(i) / numSampleBase;
         for (unsigned int j = 0; (j < numSampleBase); j++)
         {
            float valuej = static_cast<float>(j) / numSampleBase;
            uCoords.append(valuej);
            vCoords.append(valuei);
            filterSizes.append(0.0f);
         }
      }

      MString depNodeSkyColorName(fileTexture.name() + ".outColor");      
      
      if (MS::kSuccess == MRenderUtil::sampleShadingNetwork(depNodeSkyColorName, 
              numSamples, false, false, cameraMat, 
              NULL, &uCoords, &vCoords, NULL, NULL, NULL, NULL, NULL, colors, transps))
      {
         m_colorData.resize(numSamples * 4);
         int alpha = 255;
         for(unsigned int i = 0; (i < colors.length()); i++)
         {
            MFloatVector fv = colors[i];
            fv *= 255;
            m_colorData[(i * 4) + 0] = static_cast<unsigned char>(CLAMP(static_cast<int>(fv.x), 0, 255));
            m_colorData[(i * 4) + 1] = static_cast<unsigned char>(CLAMP(static_cast<int>(fv.y), 0, 255));
            m_colorData[(i * 4) + 2] = static_cast<unsigned char>(CLAMP(static_cast<int>(fv.z), 0, 255));
            m_colorData[(i * 4) + 3] = static_cast<unsigned char>(alpha);
         }
      }
   }
   m_goSample = false;
}

bool CSphereLocator::setInternalValueInContext(const MPlug &plug, const MDataHandle &handle, MDGContext &context)
{     
   if (plug == s_color || plug == s_sampling)
   {
      m_goSample = true;
   }
   
   return MPxLocatorNode::setInternalValueInContext(plug, handle, context);
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

unsigned int CSphereLocator::NumSampleBase()
{
   MFnDependencyNode fnThisNode(thisMObject());
   int sampling = MPlug(thisMObject(), s_sampling).asShort();
   switch (sampling)
   {
      case 0:
         return 64;
      case 1:
         return 128;
      case 2:
         return 256;
      case 3:
         return 512;
      case 4:
         return 1024;
      default:
         return 256;
   }
}

void CSphereLocator::OnDraw(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   MStatus stat;
   const int divisions = 64;
   const int divisionsWireframe = 16;
   float radius;
   int facing;

   MFnDagNode fn(thisMObject());

   MPlug radiusPlug  = fn.findPlug("skyRadius");
   radiusPlug.getValue(radius);

   MPlug facingPlug  = fn.findPlug("skyFacing");
   facingPlug.getValue(facing);
   
   int renderMode;
   glGetIntegerv(GL_RENDER_MODE, &renderMode);
   // only render a wireframe representation, when in select mode
   // this gets rid most of the problems, and still lets the users select
   // the skydome light, the same way as mental ray ibl does
   
   if (renderMode == GL_SELECT)
   {
      DrawSphereWireframe(radius, divisionsWireframe, divisionsWireframe);
      return;
   }

   // do not write to the z buffer.
   glDepthMask(0);
   
   GLuint texture;

   int displayStyle     = view.displayStyle();
   int displayStatusInt = displayStatus;

   // 3 means wireframe
   if (displayStyle != 3)
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
         if (m_goSample == true)
         {
            MPlug plugColor = fn.findPlug(s_color);
            SampleSN(plugColor);
         }
         
         if (m_colorData.size())
         {
            // check the number of samples
            int numSampleBase = NumSampleBase();

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnable(GL_TEXTURE_2D);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, numSampleBase, numSampleBase , 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_colorData[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            float hwTexAlpha  = fn.findPlug("hwtexalpha").asFloat();
            glColor4f(0.0f, 0.0f, 0.0f, hwTexAlpha);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
            glEnable(GL_CULL_FACE);
            if (facing == 1)
               glCullFace(GL_FRONT);
            else
               glCullFace(GL_BACK);

            // Our Custom Sphere
            MPlug formatPlug  = fn.findPlug(s_format);
            int format;
            formatPlug.getValue(format);
            DrawUVSphere(radius, divisions * 4, divisions * 4, format);

            if (facing == 2)
            {
               // we want both face, we need to redraw a second inverted sphere :'(glCullFace(GL_BACK);
               glCullFace(GL_FRONT);
               DrawUVSphere(radius, divisions * 4, divisions * 4, format);               
            }
            glDisable(GL_TEXTURE_2D);
            glDeleteTextures(1, &texture);
         }
      }
      // else, there is a plain colour
      else
      {
         glColor4f(skyColorPlug.child(0).asFloat(), skyColorPlug.child(1).asFloat(), skyColorPlug.child(2).asFloat(), 0.6f);
         MPlug formatPlug  = fn.findPlug(s_format);
         int format;
         formatPlug.getValue(format);
         DrawUVSphere(radius, divisions * 4, divisions * 4, format, false);
      }

      if (facing != 2)
         glDisable(GL_CULL_FACE);

      // If it's selected, draw also wireframe
      if (displayStatusInt == 8)
      {
         glColor4f(1, 1, 0, 0.2f);
         DrawSphereWireframe(radius, divisionsWireframe, divisionsWireframe);
      }

   }
   else
   {
      if (displayStatusInt == 8)
         glColor4f(1, 1, 0, 0.2f);
      else
         glColor4f(0.75, 0, 0, 0.2f);
      DrawSphereWireframe(radius, divisionsWireframe, divisionsWireframe);
   }

   // re-enable depth writes
   glDepthMask(1);
   glDisable(GL_BLEND);
}

MBoundingBox CSphereLocator::boundingBox() const
{
   float scaleX, scaleY, scaleZ, radius;
   MFnDagNode fn(thisMObject());

   MPlug radiusPlug  = fn.findPlug("skyRadius");
   radiusPlug.getValue(radius);

   scaleX = radius;
   scaleY = radius;
   scaleZ = radius; 

   // expand the bounding box to fit all axes of the locator node
   MBoundingBox bbox;

   bbox.expand(MPoint(-1.0f*scaleX, 0.0f, 0.0f));
   bbox.expand(MPoint(scaleX, 0.0f, 0.0f));
   bbox.expand(MPoint(0.0f,-1.0f*scaleY, 0.0f));
   bbox.expand(MPoint(0.0f, 1.0f*scaleY, 0.0f));
   bbox.expand(MPoint(0.0f, 0.0f, -1.0f*scaleZ));
   bbox.expand(MPoint(0.0f, 0.0f, 1.0f*scaleZ));
   return bbox;
}

bool CSphereLocator::isBounded() const
{
   return true;
}

bool CSphereLocator::drawLast() const
{
   return false;
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
   eAttr.addField("Higher (512x512)", 3);
   eAttr.addField("Ultra (1024x1024)", 4);
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

