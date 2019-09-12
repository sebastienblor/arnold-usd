#include "ArnoldSkyNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>

MTypeId CArnoldSkyNode::id(ARNOLD_NODEID_SKY);

CStaticAttrHelper CArnoldSkyNode::s_attributes(CArnoldSkyNode::addAttribute);

MObject CArnoldSkyNode::s_XX;
MObject CArnoldSkyNode::s_XY;
MObject CArnoldSkyNode::s_XZ;
MObject CArnoldSkyNode::s_X;
MObject CArnoldSkyNode::s_X_angle;
MObject CArnoldSkyNode::s_YX;
MObject CArnoldSkyNode::s_YY;
MObject CArnoldSkyNode::s_YZ;
MObject CArnoldSkyNode::s_Y;
MObject CArnoldSkyNode::s_Y_angle;
MObject CArnoldSkyNode::s_ZX;
MObject CArnoldSkyNode::s_ZY;
MObject CArnoldSkyNode::s_ZZ;
MObject CArnoldSkyNode::s_Z;
MObject CArnoldSkyNode::s_Z_angle;

MObject CArnoldSkyNode::s_OUT_colorR;
MObject CArnoldSkyNode::s_OUT_colorG;
MObject CArnoldSkyNode::s_OUT_colorB;
MObject CArnoldSkyNode::s_OUT_color;
MObject CArnoldSkyNode::s_OUT_transparencyR;
MObject CArnoldSkyNode::s_OUT_transparencyG;
MObject CArnoldSkyNode::s_OUT_transparencyB;
MObject CArnoldSkyNode::s_OUT_transparency;

void* CArnoldSkyNode::creator()
{
   return new CArnoldSkyNode();
}

MStatus CArnoldSkyNode::initialize()
{
   MPxNode::inheritAttributesFrom("SphereLocator");

   MFnNumericAttribute nAttr;

   s_attributes.SetNode("sky");
   s_attributes.MakeInput("intensity");
   // FIXME: visibleInReflection, visibleInRefraction seem to be on the locator already but they are false by default
   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   return MS::kSuccess;
}

// Make sure this is marked to be excluded when
// the locator display filter is enabled
bool CArnoldSkyNode::excludeAsLocator() const
{
	return true;
}

void CArnoldSkyNode::draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
   if ((view.objectDisplay() & M3dView::kDisplayLocators) == 0) return;
   CSphereLocator::draw(view, DGpath, style, status);
}

void CArnoldSkyNode::postConstructor()
{

#if MAYA_API_VERSION >= 201700
   // Always make the node not receive or cast shadows
   //
   MFnDependencyNode node(thisMObject());
   MPlug plug = node.findPlug("receiveShadows", true);
   plug.setValue(false);
   plug = node.findPlug("castsShadows", true);
   plug.setValue(false);
   
#endif

   // Call parent postConstructor as it is not done automatically as the parent constructor
   CSphereLocator::postConstructor();

   setMPSafe(true);
}
