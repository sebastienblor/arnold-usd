#include "ArnoldLightPortal.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"
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

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR          15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR          17 // pale blue


MTypeId CArnoldLightPortalNode::id(ARNOLD_NODEID_LIGHT_PORTAL);

//MObject CArnoldLightPortalNode::s_update;

CStaticAttrHelper CArnoldLightPortalNode::s_attributes(CArnoldLightPortalNode::addAttribute);

void* CArnoldLightPortalNode::creator()
{
   return new CArnoldLightPortalNode();
}
CArnoldLightPortalNode::CArnoldLightPortalNode() :
   m_boundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0))
{}

MStatus CArnoldLightPortalNode::initialize()
{
//   s_attributes.SetNode("quad_light");

   //CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);
   return MS::kSuccess;
}

void CArnoldLightPortalNode::postConstructor()
{
   // Call parent postConstructor as it is not done automatically as the parent constructor
   MPxLocatorNode::postConstructor();
   setMPSafe(true);
}

MStatus CArnoldLightPortalNode::compute(const MPlug& plug, MDataBlock& block)
{
   return MS::kUnknownParameter;
}

void CArnoldLightPortalNode::draw( M3dView & view, const MDagPath & dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus )
{
   if ((view.objectDisplay() & M3dView::kDisplayLights) == 0) return;
   M3dView::ColorTable activeColorTable  = M3dView::kActiveColors;
   //M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
   //

   MStatus status;
   MObject tmo = thisMObject();
   MFnDependencyNode myNode(tmo);
   
   view.beginGL();
   // Get all GL bits
   glPushAttrib(GL_POLYGON_BIT | GL_CURRENT_BIT);
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
   

   m_boundingBox = MBoundingBox(MPoint(1.0, 1.0, 1.0), MPoint(-1.0, -1.0, -1.0));

   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

bool CArnoldLightPortalNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldLightPortalNode::boundingBox() const
{
   return m_boundingBox;
}

bool CArnoldLightPortalNode::excludeAsLocator() const
{
   return false;
}
