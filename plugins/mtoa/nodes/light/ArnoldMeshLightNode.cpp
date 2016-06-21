#include "ArnoldMeshLightNode.h"

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

MTypeId CArnoldMeshLightNode::id( ARNOLD_NODEID_MESH_LIGHT );
CStaticAttrHelper CArnoldMeshLightNode::s_attributes(CArnoldMeshLightNode::addAttribute);

// Inputs
MObject CArnoldMeshLightNode::s_inMesh;
MObject CArnoldMeshLightNode::s_showOriginalMesh;
MObject CArnoldMeshLightNode::s_colorR;
MObject CArnoldMeshLightNode::s_colorG;
MObject CArnoldMeshLightNode::s_colorB;
MObject CArnoldMeshLightNode::s_color;
MObject CArnoldMeshLightNode::s_intensity;
MObject CArnoldMeshLightNode::s_affectDiffuse;
MObject CArnoldMeshLightNode::s_affectSpecular;
MObject CArnoldMeshLightNode::s_lightVisible;
// Arnold outputs
MObject CArnoldMeshLightNode::s_OUT_colorR;
MObject CArnoldMeshLightNode::s_OUT_colorG;
MObject CArnoldMeshLightNode::s_OUT_colorB;
MObject CArnoldMeshLightNode::s_OUT_color;
MObject CArnoldMeshLightNode::s_OUT_transparencyR;
MObject CArnoldMeshLightNode::s_OUT_transparencyG;
MObject CArnoldMeshLightNode::s_OUT_transparencyB;
MObject CArnoldMeshLightNode::s_OUT_transparency;

// Maya specific inputs
MObject CArnoldMeshLightNode::s_pointCamera;
MObject CArnoldMeshLightNode::s_normalCamera;
// Maya specific Outputs
MObject CArnoldMeshLightNode::aLightDirection;
MObject CArnoldMeshLightNode::aLightIntensity;
MObject CArnoldMeshLightNode::aLightAmbient;
MObject CArnoldMeshLightNode::aLightDiffuse;
MObject CArnoldMeshLightNode::aLightSpecular;
MObject CArnoldMeshLightNode::aLightShadowFraction;
MObject CArnoldMeshLightNode::aPreShadowIntensity;
MObject CArnoldMeshLightNode::aLightBlindData;
MObject CArnoldMeshLightNode::aLightData;

#if MAYA_API_VERSION >= 201700
// Maya area light attributes
MObject CArnoldMeshLightNode::aDropOff;
MObject CArnoldMeshLightNode::aDecayRate;
MObject CArnoldMeshLightNode::aUseRayTraceShadows;
MObject CArnoldMeshLightNode::aDepthMapResolution;
MObject CArnoldMeshLightNode::aShadowColor;
#endif

CArnoldMeshLightNode::CArnoldMeshLightNode()
   : m_attrChangeId(0)
   , m_meshDirtyId(0)
   , m_vp1GeometryUpdate(true)
   , m_vp2GeometryUpdate(true)
#if MAYA_API_VERSION >= 201700
   , m_aiCastShadows(true)
   , m_aiCastVolumetricShadows(true)
#endif
{}

CArnoldMeshLightNode::~CArnoldMeshLightNode() 
{
   if (m_attrChangeId != 0)
      MMessage::removeCallback(m_attrChangeId);
   if (m_meshDirtyId != 0)
      MMessage::removeCallback(m_meshDirtyId);
}

void CArnoldMeshLightNode::postConstructor()
{
   // Make the node not cast nor receive shadows
   //
   MObject me = thisMObject();

#if MAYA_API_VERSION >= 201700
   MFnDependencyNode node(me);
   MPlug plug = node.findPlug("receiveShadows");
   plug.setValue(false);
   plug = node.findPlug("castsShadows");
   plug.setValue(false);
#endif

   m_attrChangeId = MNodeMessage::addAttributeChangedCallback(me, attrChangedCallback, this);
   scheduleGeometryUpdate();
}

// Map node's attribute value changes to ones understood by Maya
void CArnoldMeshLightNode::attrChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData)
{
   CArnoldMeshLightNode *node = static_cast<CArnoldMeshLightNode*>(clientData);
   if (!node)
      return; 

#if MAYA_API_VERSION >= 201700
   if (msg & MNodeMessage::kAttributeSet)
   {
      bool updateShadowAttr = false;
      MFnAttribute fnAttr(plug.attribute());
      if (fnAttr.name() == "aiCastShadows")
      {
         node->m_aiCastShadows = plug.asBool();
         updateShadowAttr = true;
      }
      else if (fnAttr.name() == "aiCastVolumetricShadows")
      {
         node->m_aiCastVolumetricShadows = plug.asBool();
         updateShadowAttr = true;
      }
      else if (fnAttr.name() == "aiDecayType")
      {
         int decayType = plug.asInt();
         int decayRate = 2;
         if (decayType == 0)
         {
            decayRate = 0;
         }
         else
         {
            decayRate = 2;
         }
         MPlug plug2(node->thisMObject(), aDecayRate);
         plug2.setValue( decayRate );
      }
      else if (fnAttr.name() == "aiShadowColor" ||
               fnAttr.name() == "aiShadowColorR" ||
               fnAttr.name() == "aiShadowColorG" ||
               fnAttr.name() == "aiShadowColorB")
      {
         float shadowColorR = 0.0f;
         float shadowColorG = 0.0f;
         float shadowColorB = 0.0f;

         MFnDependencyNode dependNode(plug.node());

         MPlug srcR = dependNode.findPlug("aiShadowColorR");
         srcR.getValue(shadowColorR);
         MPlug srcG = dependNode.findPlug("aiShadowColorG");
         srcG.getValue(shadowColorG);
         MPlug srcB = dependNode.findPlug("aiShadowColorB");
         srcB.getValue(shadowColorB);

         MPlug destR = dependNode.findPlug("shadowColorR");
         destR.setValue(shadowColorR);
         MPlug destG = dependNode.findPlug("shadowColorG");
         destG.setValue(shadowColorG);
         MPlug destB = dependNode.findPlug("shadowColorB");
         destB.setValue(shadowColorB);
      }

      if (updateShadowAttr)
      {
         MPlug plug(node->thisMObject(), aUseRayTraceShadows);
         plug.setValue(node->m_aiCastShadows || node->m_aiCastVolumetricShadows);
      }
   }
#endif

   if (plug == s_inMesh)
   {
      if (msg & MNodeMessage::kConnectionMade)
      {
         if (otherPlug.node().hasFn(MFn::kMesh))
         {
            if (node->m_meshDirtyId != 0)
            {
               MMessage::removeCallback(node->m_meshDirtyId);
            }
            node->m_meshDirtyId = MNodeMessage::addNodeDirtyCallback(otherPlug.node(), meshDirtyCallback, node);
            node->scheduleGeometryUpdate();
         }
      }
      else if (msg & MNodeMessage::kConnectionBroken)
      {
         if (node->m_meshDirtyId != 0)
         {
            MMessage::removeCallback(node->m_meshDirtyId);
         }
         node->scheduleGeometryUpdate();
      }
   }
}

void CArnoldMeshLightNode::meshDirtyCallback(MObject& node, MPlug& plug, void *clientData)
{
   CArnoldMeshLightNode *lightNode = static_cast<CArnoldMeshLightNode*>(clientData);
   if (!lightNode)
      return;

   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   if(plugName == ".pnts" || plugName == ".inMesh" || plugName == ".dispResolution")
   {
      lightNode->scheduleGeometryUpdate();
   }
}

MStatus CArnoldMeshLightNode::compute(const MPlug& plug, MDataBlock& block)
{
   if ((plug != aLightData) && (plug.parent() != aLightData))
      return MS::kUnknownParameter;

   return MS::kSuccess;
}

void CArnoldMeshLightNode::draw( M3dView & view, const MDagPath & dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus )
{
   if ((view.objectDisplay() & M3dView::kDisplayLights) == 0)
   {
      return;
   }

   M3dView::ColorTable activeColorTable = M3dView::kActiveColors;

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

   if (m_vp1GeometryUpdate)
   {
      m_vp1GeometryUpdate = false;
      MObject meshObj = GetMeshObject();
      m_drawPrimitive.update(meshObj);
   }

   m_drawPrimitive.draw();

   // Restore all GL bits
   glPopAttrib();
   view.endGL();
}

bool CArnoldMeshLightNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldMeshLightNode::boundingBox() const
{
   MObject meshObj = GetMeshObject();
   MFnMesh mesh(meshObj);
   return mesh.boundingBox();
}

void* CArnoldMeshLightNode::creator()
{
   return new CArnoldMeshLightNode();
}

MStatus CArnoldMeshLightNode::initialize()
{
   MFnLightDataAttribute lAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MFnMessageAttribute mAttr;
   MStatus         stat;

   s_inMesh = mAttr.create("inMesh", "inMesh");
   mAttr.setStorable(true);
   mAttr.setReadable(true);
   mAttr.setWritable(true);
   addAttribute(s_inMesh);

   s_showOriginalMesh = nAttr.create("showOriginalMesh", "showOriginalMesh", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_showOriginalMesh);

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

   s_lightVisible = nAttr.create("lightVisible", "lightVisible", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setChannelBox(true);
   addAttribute(s_lightVisible);

   // OUTPUT ATTRIBUTES
   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

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

   // MAYA SPECIFIC OUTPUTS
   aLightDirection = nAttr.createPoint("lightDirection", "ld");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(0.0f, 0.0f, -1.0f);

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
   lAttr.setReadable(true);
   lAttr.setWritable(false);
   lAttr.setStorable(false);
   lAttr.setHidden(true);
   lAttr.setDefault(0.0f, 0.0f, -1.0f, 1.0f, 0.5f, 0.2f,
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

#if MAYA_API_VERSION >= 201700
   // Area light attributes for display control
   aDropOff = nAttr.create("dropoff", "dro", MFnNumericData::kDouble);
   nAttr.setHidden(true);
   nAttr.setDefault(2.0);
   addAttribute(aDropOff);

   aDecayRate = nAttr.create( "decayRate", "de", MFnNumericData::kShort);
   nAttr.setHidden(true);
   nAttr.setDefault(2);
   addAttribute(aDecayRate);

   // Maya shadowing attributes
   aUseRayTraceShadows = nAttr.create( "useRayTraceShadows", "urs", MFnNumericData::kBoolean);
   nAttr.setDefault(true);
   nAttr.setHidden(true);
   addAttribute(aUseRayTraceShadows);

   aDepthMapResolution = nAttr.create( "dmapResolution", "dr", MFnNumericData::kShort);
   nAttr.setHidden(true);
   nAttr.setDefault(1024);
   addAttribute(aDepthMapResolution);

   // There is short-name clash so use shc versus sc
   aShadowColor = nAttr.createColor( "shadowColor", "shc" );
   nAttr.setHidden(true);
   nAttr.setDefault(0.0f, 0.0f, 0.0f);
   addAttribute(aShadowColor);
#endif

   return MS::kSuccess;
}

bool CArnoldMeshLightNode::excludeAsLocator() const
{
   return false;
}

MObject CArnoldMeshLightNode::GetMeshObject() const
{
   MPlug plug(thisMObject(), s_inMesh);
   if (plug.isDestination())
   {
      MObject sourceNode = plug.source().node();
      if (sourceNode.hasFn(MFn::kMesh))
      {
         return sourceNode;
      }
   }
   return MObject::kNullObj;
}

void CArnoldMeshLightNode::scheduleGeometryUpdate()
{
   m_vp1GeometryUpdate = true;
   m_vp2GeometryUpdate = true;
}
