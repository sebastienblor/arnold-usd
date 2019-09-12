#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ArnoldLightBlockerNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif   


const MTypeId CArnoldLightBlockerNode::id(ARNOLD_NODEID_LIGHT_BLOCKER);

MObject CArnoldLightBlockerNode::s_geometryType;
MObject CArnoldLightBlockerNode::s_density;
MObject CArnoldLightBlockerNode::s_shader;
MObject CArnoldLightBlockerNode::s_roundness;
MObject CArnoldLightBlockerNode::s_widthEdge;
MObject CArnoldLightBlockerNode::s_heightEdge;
MObject CArnoldLightBlockerNode::s_ramp;
MObject CArnoldLightBlockerNode::s_rampAxis;

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR          15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR          17 // pale blue

CArnoldLightBlockerNode::CArnoldLightBlockerNode()
{
   
}

CArnoldLightBlockerNode::~CArnoldLightBlockerNode()
{
   
}

#if MAYA_API_VERSION >= 201700
void CArnoldLightBlockerNode::postConstructor()
{
   // Always make the node not receive or cast shadows
   //
   MFnDependencyNode node(thisMObject());
   MPlug plug = node.findPlug("receiveShadows", true);
   plug.setValue(false);
   plug = node.findPlug("castsShadows", true);
   plug.setValue(false);
}
#endif

void* CArnoldLightBlockerNode::creator()
{
   return new CArnoldLightBlockerNode();
}

MStatus CArnoldLightBlockerNode::initialize()
{
   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;
   
   s_geometryType = eAttr.create("geometryType", "geometry_type");
   eAttr.addField("box", 0);
   eAttr.addField("sphere", 1);
   eAttr.addField("plane", 2);
   eAttr.addField("cylinder", 3);
   eAttr.setDefault(0);
   eAttr.setChannelBox(true);
   addAttribute(s_geometryType);
   
   s_density = nAttr.create("density", "dens", MFnNumericData::kFloat);
   nAttr.setDefault(0.f);
   nAttr.setSoftMin(0.f);
   nAttr.setSoftMax(1.f);
   nAttr.setChannelBox(true);
   addAttribute(s_density);
   
   s_shader = nAttr.createColor("shader", "shad");
   nAttr.setDefault(0.0f, 0.0f, 0.0f);
   nAttr.setKeyable(true);
   nAttr.setConnectable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setChannelBox(false);
   addAttribute(s_shader);
   
   s_roundness = nAttr.create("roundness", "rnds", MFnNumericData::kFloat);
   nAttr.setDefault(0.f);
   nAttr.setSoftMin(0.f);
   nAttr.setSoftMax(1.f);
   nAttr.setChannelBox(true);
   addAttribute(s_roundness);
   
   s_widthEdge = nAttr.create("widthEdge", "width_edge", MFnNumericData::kFloat);
   nAttr.setDefault(0.f);
   nAttr.setSoftMin(0.f);
   nAttr.setSoftMax(1.f);
   nAttr.setChannelBox(true);
   addAttribute(s_widthEdge);
   
   s_heightEdge = nAttr.create("heightEdge", "height_edge", MFnNumericData::kFloat);
   nAttr.setDefault(0.f);
   nAttr.setSoftMin(0.f);
   nAttr.setSoftMax(1.f);
   nAttr.setChannelBox(true);
   addAttribute(s_heightEdge);
   
   s_ramp = nAttr.create("ramp", "rmp", MFnNumericData::kFloat);
   nAttr.setDefault(0.f);
   nAttr.setSoftMin(0.f);
   nAttr.setSoftMax(1.0f);
   nAttr.setChannelBox(false);
   addAttribute(s_ramp);
   
   s_rampAxis = eAttr.create("rampAxis", "ramp_axis");
   eAttr.addField("x", 0);
   eAttr.addField("y", 1);
   eAttr.addField("z", 2);
   eAttr.setDefault(0);
   eAttr.setChannelBox(true);
   addAttribute(s_rampAxis);
   
   return MS::kSuccess;
}

bool CArnoldLightBlockerNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldLightBlockerNode::boundingBox() const
{
   return MBoundingBox(MPoint(0.5, 0.5, 0.5), MPoint(-0.5, -0.5, -0.5));
}

void CArnoldLightBlockerNode::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   view.beginGL();
   
   glPushAttrib(GL_POLYGON_BIT | GL_CURRENT_BIT);
   
   MObject tmo = thisMObject();
   MStatus status;
   
   const short geometryType = MPlug(tmo, s_geometryType).asShort();
   
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   M3dView::ColorTable activeColorTable  = M3dView::kActiveColors;
   
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
   
   switch (geometryType)
   {
   case 0: // box
      {
         static CBoxPrimitive primitive(0.5f);
         primitive.draw();
      }
      break;
   case 1: // sphere
      {
         static CSpherePrimitive primitive(0.5f);
         primitive.draw();
      }
      break;
   case 2: // plane
      glBegin(GL_QUADS);
      
      glVertex3f(-0.5f, -0.5f, 0.f);
      glVertex3f(0.5f, -0.5f, 0.f);
      glVertex3f(0.5f, 0.5f, 0.f);
      glVertex3f(-0.5f, 0.5f, 0.f);
      
      glEnd();
      break;
   case 3: // cylinder
      {
         static CCylinderPrimitive primitive(0.5f, 0.5f);
         primitive.draw();
      }
      break;
   }
   
   glPopAttrib();
   
   view.endGL();
}
