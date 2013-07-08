#include "ArnoldPhotometricLightNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <ai_metadata.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatVector.h>
#include <maya/MFnStringData.h>

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR          15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR          17 // pale blue

MTypeId CArnoldPhotometricLightNode::id(ARNOLD_NODEID_PHOTOMETRIC_LIGHT);

CStaticAttrHelper CArnoldPhotometricLightNode::s_attributes(CArnoldPhotometricLightNode::addAttribute);

// Inputs
MObject CArnoldPhotometricLightNode::s_colorR;
MObject CArnoldPhotometricLightNode::s_colorG;
MObject CArnoldPhotometricLightNode::s_colorB;
MObject CArnoldPhotometricLightNode::s_color;
MObject CArnoldPhotometricLightNode::s_intensity;
MObject CArnoldPhotometricLightNode::s_affectDiffuse;
MObject CArnoldPhotometricLightNode::s_affectSpecular;
MObject CArnoldPhotometricLightNode::s_filename;
// Arnold outputs
MObject CArnoldPhotometricLightNode::s_OUT_colorR;
MObject CArnoldPhotometricLightNode::s_OUT_colorG;
MObject CArnoldPhotometricLightNode::s_OUT_colorB;
MObject CArnoldPhotometricLightNode::s_OUT_color;
MObject CArnoldPhotometricLightNode::s_OUT_transparencyR;
MObject CArnoldPhotometricLightNode::s_OUT_transparencyG;
MObject CArnoldPhotometricLightNode::s_OUT_transparencyB;
MObject CArnoldPhotometricLightNode::s_OUT_transparency;
// Maya specific intputs
MObject CArnoldPhotometricLightNode::s_pointCamera;
MObject CArnoldPhotometricLightNode::s_normalCamera;
// Maya specific Outputs
MObject CArnoldPhotometricLightNode::aLightDirection;
MObject CArnoldPhotometricLightNode::aLightIntensity;
MObject CArnoldPhotometricLightNode::aLightAmbient;
MObject CArnoldPhotometricLightNode::aLightDiffuse;
MObject CArnoldPhotometricLightNode::aLightSpecular;
MObject CArnoldPhotometricLightNode::aLightShadowFraction;
MObject CArnoldPhotometricLightNode::aPreShadowIntensity;
MObject CArnoldPhotometricLightNode::aLightBlindData;
MObject CArnoldPhotometricLightNode::aLightData;


CArnoldPhotometricLightNode::CArnoldPhotometricLightNode() :
        m_boundingBox(MPoint(1.0, 0.7, 0.7), MPoint(-0.7, -2.0, -0.7))
{ }

CArnoldPhotometricLightNode::~CArnoldPhotometricLightNode() {}

void* CArnoldPhotometricLightNode::creator()
{
   return new CArnoldPhotometricLightNode();
}

MStatus CArnoldPhotometricLightNode::initialize()
{
   MFnLightDataAttribute lAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MStatus         stat;
   MFnStringData sData;

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
   
   /*s_filename = tAttr.create("aiFilename", "ai_filename", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_filename);*/
   
   

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

MStatus CArnoldPhotometricLightNode::compute(const MPlug& plug, MDataBlock& block)
{
   if ((plug != aLightData) && (plug.parent() != aLightData))
      return MS::kUnknownParameter;


    return MS::kSuccess;
}

void CArnoldPhotometricLightNode::draw( M3dView & view, const MDagPath & dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus )
{    
   M3dView::ColorTable activeColorTable  = M3dView::kActiveColors;

   MStatus status;
   
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
   default:
      glColor4f(0.75, 0, 0, 0.2f);
      break;
   }

   
   {
      
      
      glBegin(GL_LINE_LOOP);
      for (int i=0; i < 360; i+=10)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(cos(degInRad)*0.5f,sin(degInRad)*0.5f,0.0f);
      }
      glEnd();
      glBegin(GL_LINES);
      for (int i=0; i < 360; i+=45)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(cos(degInRad)*0.5f,sin(degInRad)*0.5f,0.0f);
         glVertex3f(cos(degInRad)*0.7f,sin(degInRad)*0.7f,0.0f);
      }
      glEnd();
      
      glBegin(GL_LINE_LOOP);
      for (int i=0; i < 360; i++)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(0.0f,cos(degInRad)*0.5f,sin(degInRad)*0.5f);
      }
      glEnd();
      glBegin(GL_LINES);
      for (int i=0; i < 360; i+=45)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(0.0f,cos(degInRad)*0.5f,sin(degInRad)*0.5f);
         glVertex3f(0.0f,cos(degInRad)*0.7f,sin(degInRad)*0.7f);
      }
      glEnd();
      
      glBegin(GL_LINE_LOOP);
      for (int i=0; i < 360; i++)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(cos(degInRad)*0.5f,0.0f,sin(degInRad)*0.5f);
      }
      glEnd();
      glBegin(GL_LINES);
      for (int i=0; i < 360; i+=45)
      {
         float degInRad = float(i*2*M_PI/360);
         glVertex3f(cos(degInRad)*0.5f,0.0f,sin(degInRad)*0.5f);
         glVertex3f(cos(degInRad)*0.7f,0.0f,sin(degInRad)*0.7f);
      }
      glEnd();
      
      
      glBegin(GL_LINES);
      // Done Drawing The direction
      glVertex3f( 0.0f,-0.5f, 0.0f);
      glVertex3f( 0.0f,-2.0f, 0.0f);
      glVertex3f( 0.3f,-1.5f, 0.0f);
      glVertex3f( 0.0f,-2.0f, 0.0f);
      glVertex3f(-0.3f,-1.5f, 0.0f);
      glVertex3f( 0.0f,-2.0f, 0.0f);
      
      glVertex3f( 0.5f, 0.0f, 0.0f);
      glVertex3f( 1.0f, 0.0f, 0.0f);
      glVertex3f( 1.0f,-0.3f, 0.0f);
      glVertex3f( 1.0f, 0.3f, 0.0f);
      
      glEnd();
   }
   
   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

bool CArnoldPhotometricLightNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldPhotometricLightNode::boundingBox() const
{
   return m_boundingBox;;
}

bool CArnoldPhotometricLightNode::excludeAsLocator() const
{
   return false;
}

