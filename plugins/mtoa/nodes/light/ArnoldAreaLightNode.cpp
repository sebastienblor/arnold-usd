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
#include <maya/MGLFunctionTable.h>

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

CArnoldAreaLightNode::CArnoldAreaLightNode() {}
CArnoldAreaLightNode::~CArnoldAreaLightNode() {}

MStatus CArnoldAreaLightNode::compute( const MPlug&, MDataBlock& )
{
   return MS::kUnknownParameter;
}

void CArnoldAreaLightNode::draw( M3dView & view, const MDagPath & dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus )
{
   // Initialize GL function table first time through
   static MGLFunctionTable *gGLFT = NULL;
   if (gGLFT == NULL)
      gGLFT = MHardwareRenderer::theRenderer()->glFunctionTable();
      
   M3dView::ColorTable activeColorTable  = M3dView::kActiveColors;
   //M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
   //

   MFnDependencyNode myNode(thisMObject());
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
      gGLFT->glColor4f(0.75, 0, 0, 0.2f);
      break;
   }
   GLUquadricObj *qobj;
   qobj = gluNewQuadric();
   // Quad
   if (areaType == "quad")
   {
      gGLFT->glBegin(GL_QUADS);
      gGLFT->glVertex3f(-1.0f, 1.0f, 0.0f);
      gGLFT->glVertex3f( 1.0f, 1.0f, 0.0f);
      gGLFT->glVertex3f( 1.0f,-1.0f, 0.0f);
      gGLFT->glVertex3f(-1.0f,-1.0f, 0.0f);
      gGLFT->glEnd();
      gGLFT->glBegin(GL_LINES);
      gGLFT->glVertex3f(-1.0f, 1.0f, 0.0f);
      gGLFT->glVertex3f( 1.0f,-1.0f, 0.0f);
      gGLFT->glVertex3f(-1.0f,-1.0f, 0.0f);
      gGLFT->glVertex3f( 1.0f, 1.0f, 0.0f);
      // Done Drawing The direction
      gGLFT->glVertex3f( 0.0f, 0.0f, 0.0f);
      gGLFT->glVertex3f( 0.0f, 0.0f,-1.0f);
      gGLFT->glEnd();
   }
   // Cylinder
   else if (areaType == "cylinder")
   {
      gluQuadricDrawStyle(qobj, GLU_LINE);
      gluQuadricNormals(qobj, GLU_NONE);
      gGLFT->glPushMatrix();
      gGLFT->glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      gGLFT->glTranslatef(0.0f, 0.0f, -1.0f);
      gluCylinder(qobj, 1.0f, 1.0f, 2.0f, 20, 1);
      gGLFT->glPopMatrix();
   }
   // Disk
   else if (areaType == "disk")
   {
      gluQuadricDrawStyle(qobj, GLU_LINE);
      gluQuadricNormals(qobj, GLU_NONE);
      gGLFT->glPushMatrix();
      gluDisk(qobj, 0.0f, 1.0f, 20, 1);
      gGLFT->glPopMatrix();
      gGLFT->glBegin(GL_LINES);
      // Done Drawing The direction
      gGLFT->glVertex3f( 0.0f, 0.0f, 0.0f);
      gGLFT->glVertex3f( 0.0f, 0.0f,-1.0f);
      gGLFT->glEnd();
   }
   // Restore all GL bits
   gGLFT->glPopAttrib();
   view.endGL();
}

bool CArnoldAreaLightNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldAreaLightNode::boundingBox() const
{
   MPoint corner1( -1.0, -1.0, -1.0 );
   MPoint corner2( 1.0, 1.0, 1.0 );

   return MBoundingBox( corner1, corner2 );
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

   s_type = eAttr.create("aiTranslator", "type", 0);
   eAttr.addField("quad", 0);
   eAttr.addField("cylinder", 1);
   eAttr.addField("disk", 2);
   eAttr.setInternal(true);
   addAttribute( s_type );

   MAKE_COLOR(s_color, "color", "sc", 1, 1, 1);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setInternal(true);
   addAttribute(s_color);

   s_intensity = nAttr.create("intensity", "intensity", MFnNumericData::kFloat, 1.0f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   nAttr.setMin(0);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   addAttribute(s_intensity);

   s_affectDiffuse = nAttr.create("emitDiffuse", "emitDiffuse", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_affectDiffuse);

   s_affectSpecular = nAttr.create("emitSpecular", "emitSpecular", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_affectSpecular);

   // MAYA SPECIFIC INPUTS
   s_pointCamera = nAttr.createPoint("pointCamera", "p");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_pointCamera);

   s_normalCamera = nAttr.createPoint("normalCamera", "n");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_normalCamera);

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
