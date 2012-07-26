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
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>

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
MObject CArnoldAreaLightNode::s_inputMesh;
MObject CArnoldAreaLightNode::s_lightVisible;
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
        m_boundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0)),
        m_displayList(-1)
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
   
   MStatus status;
   
   MFnMesh inputMesh(block.inputValue(s_inputMesh).asMesh(), &status);
   
   if (m_displayList != -1)
      glDeleteLists(m_displayList, 1);
   
   m_displayList = -1;   
   
   if (!status)
      return MS::kSuccess;
   
   const int numVertices = inputMesh.numVertices();
   
   if (numVertices == 0)
      return MS::kSuccess;
   
   m_boundingBox.clear();
   
   const AtVector* vertices = (const AtVector*)inputMesh.getRawPoints(&status);
   for (int i = 0; i < numVertices; ++i)
   {
      const AtVector& cv = vertices[i];
      m_boundingBox.expand(MPoint(cv.x, cv.y, cv.z));
   }
   
   m_displayList = glGenLists(1); // these are kinda old, but still one of the
   // fastest solution for simple display
   
   glNewList(m_displayList, GL_COMPILE);
   
   const int numPolygons = inputMesh.numPolygons();         

   for (int i = 0; i < numPolygons; ++i) 
   {
      glBegin(GL_LINE_STRIP);
      MIntArray vidx;
      inputMesh.getPolygonVertices(i, vidx);
      const unsigned int numVertices = vidx.length();
      for (unsigned int j = 0; j < numVertices; ++j)
         glVertex3fv(&vertices[vidx[j]].x);
      glVertex3fv(&vertices[vidx[0]].x);
      glEnd();
   }
   
   glEndList();
   
   return MS::kSuccess;
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
      gGLFT->glColor4f(0.75, 0, 0, 0.2f);
      break;
   }
   GLUquadricObj *qobj;
   qobj = gluNewQuadric();
   bool setBoundingBox = true;
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
   // Mesh
   else if (areaType == "mesh")
   {
      setBoundingBox = false;
      if (MPlug(tmo, s_update).asBool()) // forcing the compute to be called
         return;
      if (m_displayList != -1)
         glCallList(m_displayList);
   }
   // Cylinder
   else
   {
      gluQuadricDrawStyle(qobj, GLU_LINE);
      gluQuadricNormals(qobj, GLU_NONE);
      gGLFT->glPushMatrix();
      gGLFT->glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      gGLFT->glTranslatef(0.0f, 0.0f, -1.0f);
      gluCylinder(qobj, 1.0f, 1.0f, 2.0f, 20, 1);
      gGLFT->glPopMatrix();
   }
   
   // There is a reason for this
   // I can`t set attributeAffects with aiTranslator
   // because that parameter is not created in this node
   if (setBoundingBox)
      m_boundingBox = MBoundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0));
   
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
   
   s_inputMesh = tAttr.create("inputMesh", "input_mesh", MFnData::kMesh);
   tAttr.setStorable(true);
   addAttribute(s_inputMesh);
   
   s_lightVisible = nAttr.create("lightVisible", "light_visible", MFnNumericData::kBoolean);
   nAttr.setDefault(true);
   addAttribute(s_lightVisible);
   
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
   attributeAffects(s_inputMesh, aLightData);
   
   attributeAffects(s_inputMesh, s_update);

   return MS::kSuccess;
}

bool CArnoldAreaLightNode::excludeAsLocator() const
{
   return false;
}
