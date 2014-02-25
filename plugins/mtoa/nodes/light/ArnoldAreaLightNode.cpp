#include "ArnoldAreaLightNode.h"

#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatVector.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR          15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR          17 // pale blue

MTypeId CArnoldAreaLightNode::id( ARNOLD_NODEID_AREA_LIGHT );
CStaticAttrHelper CArnoldAreaLightNode::s_attributes(CArnoldAreaLightNode::addAttribute);
MObject CArnoldAreaLightNode::s_type;

// Inputs
MObject CArnoldAreaLightNode::s_colorR;
MObject CArnoldAreaLightNode::s_colorG;
MObject CArnoldAreaLightNode::s_colorB;
MObject CArnoldAreaLightNode::s_color;
MObject CArnoldAreaLightNode::s_intensity;
MObject CArnoldAreaLightNode::s_affectDiffuse;
MObject CArnoldAreaLightNode::s_affectSpecular;
MObject CArnoldAreaLightNode::s_update;
// Arnold outputs
MObject CArnoldAreaLightNode::s_OUT_colorR;
MObject CArnoldAreaLightNode::s_OUT_colorG;
MObject CArnoldAreaLightNode::s_OUT_colorB;
MObject CArnoldAreaLightNode::s_OUT_color;
MObject CArnoldAreaLightNode::s_OUT_transparencyR;
MObject CArnoldAreaLightNode::s_OUT_transparencyG;
MObject CArnoldAreaLightNode::s_OUT_transparencyB;
MObject CArnoldAreaLightNode::s_OUT_transparency;
// Maya specific intputs
MObject CArnoldAreaLightNode::s_pointCamera;
MObject CArnoldAreaLightNode::s_normalCamera;
// Maya specific Outputs
MObject CArnoldAreaLightNode::aLightDirection;
MObject CArnoldAreaLightNode::aLightIntensity;
MObject CArnoldAreaLightNode::aLightAmbient;
MObject CArnoldAreaLightNode::aLightDiffuse;
MObject CArnoldAreaLightNode::aLightSpecular;
MObject CArnoldAreaLightNode::aLightShadowFraction;
MObject CArnoldAreaLightNode::aPreShadowIntensity;
MObject CArnoldAreaLightNode::aLightBlindData;
MObject CArnoldAreaLightNode::aLightData;

CArnoldAreaLightNode::CArnoldAreaLightNode() :
        m_boundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0))
{ }

CArnoldAreaLightNode::~CArnoldAreaLightNode() {}

MStatus CArnoldAreaLightNode::compute(const MPlug& plug, MDataBlock& block)
{
   // no need for GL stuff in the batch mode
   if (plug != s_update || MGlobal::mayaState() == MGlobal::kBatch)
      return MS::kUnknownParameter;
   
   // do this calculation every time if
   // the mesh is changed, because aiTranslator cannot affect update
   block.setClean(s_update);
      
   return MS::kSuccess;
}

class CLinePrimitiveData{
public:
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   GLenum elementType;

   void draw() // TODO : use VBOs
   {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertices.data());
      glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());
      glDisableClientState(GL_VERTEX_ARRAY);
   }
};

// use for static initialization
// These two classes could use fewer indices if we used
// glPrimitiveRestartIndex, but that's only available
// since OpenGL 3.1, and it doesn't worth the code duplication
class CDiskPrimitive : public CLinePrimitiveData{
public:
   CDiskPrimitive(GLsizei resolution = 20)
   {
      elementType = GL_LINE_STRIP;
      vertices.resize((resolution + 1) * 3);
      vertices[0] = 0.0f;
      vertices[1] = 0.0f;
      vertices[2] = 0.0f;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const float d = AI_PITIMES2 * (float(i) / float(resolution));
         float* v = &vertices[(i + 1) * 3];
         v[0] = cosf(d);
         v[1] = sinf(d);
         v[2] = 0.0f;
      }
      indices.resize(resolution * 4);
      const GLsizei res2 = resolution * 2;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const GLsizei i2 = i * 2;
         indices[i2] = 0;
         indices[i2 + 1] = i + 1;
         indices[i2 + res2] = i + 1;
         indices[i2 + res2 + 1] = (i + 1) % resolution + 1;
      }
   }
};

class CCylinderPrimitive : public CLinePrimitiveData{
public:
   CCylinderPrimitive(GLsizei resolution = 20)
   {
      vertices.resize(resolution * 6);
      const GLsizei indexDiff = resolution * 3;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const float d = AI_PITIMES2 * (float(i) / float(resolution));
         float* v = &vertices[i * 3];
         v[0] = cosf(d);
         v[1] = 1.0f;
         v[2] =  sinf(d);
         v = &vertices[i * 3 + indexDiff];
         v[0] = cosf(d);
         v[1] = -1.0f;
         v[2] = sinf(d);
      }
      indices.resize(resolution * 6);
      const GLsizei res2 = resolution * 2;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const GLsizei i2 = i * 2;
         const GLsizei i1 = (i + 1) % resolution;
         indices[i2] = i;
         indices[i2 + 1] = i1;
         indices[i2 + res2] = i + resolution;
         indices[i2 + res2 + 1] = i1 + resolution;
         const GLsizei i2o = i2 + resolution * 4;
         indices[i2o] = i;
         indices[i2o + 1] = i + resolution;
      }
   }
};

void CArnoldAreaLightNode::draw( M3dView & view, const MDagPath & dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus )
{
   if ((view.objectDisplay() & M3dView::kDisplayLights) == 0) return;
   M3dView::ColorTable activeColorTable  = M3dView::kActiveColors;
   //M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
   //

   MStatus status;
   MObject tmo = thisMObject();
   MFnDependencyNode myNode(tmo);
   MPlug translatorPlug = myNode.findPlug("aiTranslator");
   MString areaType = translatorPlug.asString();   
   
   view.beginGL();
   // Get all GL bits
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   // Display color
   switch (displayStatus)
   {
   case M3dView::kLead:
      view.setDrawColor(LEAD_COLOR, activeColorTable);
      break;
   case M3dView::kActive:
      view.setDrawColor(ACTIVE_COLOR, activeColorTable);
      break;
   case M3dView::kActiveAffected:
      view.setDrawColor(ACTIVE_AFFECTED_COLOR, activeColorTable);
      break;
   /*
   case M3dView::kDormant:
      view.setDrawColor(DORMANT_COLOR, dormantColorTable);
      break;
   case M3dView::kHilite:
      view.setDrawColor(HILITE_COLOR, activeColorTable);
      break;
   */
   default:
      glColor4f(0.75, 0, 0, 0.2f);
      break;
   }
   bool setBoundingBox = true;
   // Quad
   if (areaType == "quad")
   {
      glBegin(GL_LINES);
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glVertex3f( 1.0f, 1.0f, 0.0f);      
      glVertex3f( 1.0f, 1.0f, 0.0f);
      glVertex3f( 1.0f,-1.0f, 0.0f);      
      glVertex3f( 1.0f,-1.0f, 0.0f);
      glVertex3f(-1.0f,-1.0f, 0.0f);      
      glVertex3f(-1.0f,-1.0f, 0.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);      
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glVertex3f( 1.0f,-1.0f, 0.0f);
      glVertex3f(-1.0f,-1.0f, 0.0f);
      glVertex3f( 1.0f, 1.0f, 0.0f);
      // Done Drawing The direction
      glVertex3f( 0.0f, 0.0f, 0.0f);
      glVertex3f( 0.0f, 0.0f,-1.0f);
      glEnd();
   }
   // Disk
   else if (areaType == "disk")
   {      
      glPushMatrix();
      MTransformationMatrix transformMatrix(dagPath.inclusiveMatrix());
      double scale[3];
      transformMatrix.getScale(scale, MSpace::kWorld);
      if (scale[0] != scale[1]) // non uniform scaling across x and y
      {     
         if (scale[0] != 0.0)
            glScaled(1.0 / scale[0], 1.0, 1.0);
         if (scale[1] != 0)
            glScaled(1.0, 1.0 / scale[1], 0.0);
         const double avs = (scale[0] + scale[1]) * 0.5;
         glScaled(avs, avs, 1.0);
      }
      static CDiskPrimitive primitive;
      primitive.draw();

      glPopMatrix();
      glBegin(GL_LINES);
      // Done Drawing The direction
      glVertex3f( 0.0f, 0.0f, 0.0f);
      glVertex3f( 0.0f, 0.0f,-1.0f);
      glEnd();
   }
   // Cylinder
   else if (areaType == "cylinder")
   {
      glPushMatrix();      
      MTransformationMatrix transformMatrix(dagPath.inclusiveMatrix());
      double scale[3];
      transformMatrix.getScale(scale, MSpace::kWorld);
      if (scale[0] != scale[2]) // non uniform scaling across x and y
      {     
         if (scale[0] != 0.0)
            glScaled(1.0 / scale[0], 1.0, 1.0);
         if (scale[2] != 0)
            glScaled(1.0, 1.0, 1.0 / scale[2]);
         const double avs = (scale[0] + scale[2]) * 0.5;
         glScaled(avs, 1.0, avs);
      }
      CCylinderPrimitive primitive;
      primitive.draw();
      glPopMatrix();
   }
   
   // There is a reason for this
   // I can`t set attributeAffects with aiTranslator
   // because that parameter is not created in this node
   if (setBoundingBox)
      m_boundingBox = MBoundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0));
   
   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

bool CArnoldAreaLightNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldAreaLightNode::boundingBox() const
{
   return m_boundingBox;;
}

void* CArnoldAreaLightNode::creator()
{
   return new CArnoldAreaLightNode();
}

MStatus CArnoldAreaLightNode::initialize()
{
   MFnLightDataAttribute lAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MStatus         stat;

   MAKE_COLOR(s_color, "color", "sc", 1, 1, 1);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setInternal(true);
   nAttr.setChannelBox(true);
   addAttribute(s_color);

   s_intensity = nAttr.create("intensity", "intensity", MFnNumericData::kFloat, 1.0f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   nAttr.setMin(0);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setChannelBox(true);
   addAttribute(s_intensity);

   s_affectDiffuse = nAttr.create("emitDiffuse", "emitDiffuse", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_affectDiffuse);

   s_affectSpecular = nAttr.create("emitSpecular", "emitSpecular", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_affectSpecular);

   // MAYA SPECIFIC INPUTS
   s_pointCamera = nAttr.createPoint("pointCamera", "p");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_pointCamera);

   s_normalCamera = nAttr.createPoint("normalCamera", "n");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_normalCamera);
   
   s_update = nAttr.create("update", "upt", MFnNumericData::kBoolean);
   nAttr.setDefault(false);
   addAttribute(s_update);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // MAYA SPECIFIC OUTPUTS

   aLightDirection = nAttr.createPoint("lightDirection", "ld");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(-1.0f, 0.0f, 0.0f);

   aLightIntensity = nAttr.createColor("lightIntensity", "li");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(1.0f, 0.5f, 0.2f);

   aLightAmbient = nAttr.create("lightAmbient", "la", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightDiffuse = nAttr.create("lightDiffuse", "ldf", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightSpecular = nAttr.create("lightSpecular", "ls", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf", MFnNumericData::kFloat);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(0.0f);

   aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi", MFnNumericData::kFloat);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(0.0f);

   aLightBlindData = nAttr.createAddr("lightBlindData", "lbld");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);

   aLightData = lAttr.create("lightData", "ltd",
                              aLightDirection, aLightIntensity,
                              aLightAmbient, aLightDiffuse, aLightSpecular,
                              aLightShadowFraction, aPreShadowIntensity,
                              aLightBlindData);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   lAttr.setStorable(false);
   lAttr.setHidden(true);
   lAttr.setDefault(-1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
                     true, true, true, 0.0f, 1.0f, NULL);

   addAttribute(aLightData);

   attributeAffects(s_pointCamera, aLightData);
   attributeAffects(s_normalCamera, aLightData);

   attributeAffects (aLightIntensity, aLightData);
   attributeAffects (aLightDirection, aLightData);
   attributeAffects (aLightAmbient, aLightData);
   attributeAffects (aLightDiffuse, aLightData);
   attributeAffects (aLightSpecular, aLightData);
   attributeAffects (aLightShadowFraction, aLightData);
   attributeAffects (aPreShadowIntensity, aLightData);
   attributeAffects (aLightBlindData, aLightData);
   attributeAffects (aLightData, aLightData);

   attributeAffects(s_color, aLightData);
   attributeAffects(s_intensity, aLightData);
   attributeAffects(s_affectDiffuse, aLightData);
   attributeAffects(s_affectSpecular, aLightData);

   return MS::kSuccess;
}

bool CArnoldAreaLightNode::excludeAsLocator() const
{
   return false;
}
