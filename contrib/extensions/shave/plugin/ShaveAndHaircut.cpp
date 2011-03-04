#include "ShaveAndHaircut.h"

#include "utils/AttrHelper.h"

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <ai_nodes.h>
#include <ai_vector.h>
#include <ai_ray.h>
#include <ai_msg.h>

MStatus CShaveTranslator::SetHairInfo()
{
   MStatus status;
   // shaveItHair init only reads MObjectArray, so we append our node into an array
   //
   MObjectArray hairNodes;
   hairNodes.append(m_dagPath.node());

   // export shaveAndHaircut info into a variable
   //
   status = shaveAPI::exportHair(hairNodes, &m_hairInfo);
   return status;
}

AtNode* CShaveTranslator::Export()
{
   MStatus status;
   // The curve node
   AtNode* curve = NULL;

   int debug = 0;
   AiMsgInfo("state: %d", debug++);
   // Only translate the shave node if its marked as a active
   //
   if (!m_fnNode.findPlug("active").asBool())
      return curve;

   AiMsgInfo("state: %d", debug++);

   // Create the curve node
   //
   curve = AiNode("curves");
   AiNodeSetStr(curve, "name", m_dagPath.fullPathName().asChar());
   Update(curve);
   return curve;
}

void CShaveTranslator::Update(AtNode* curve)
{
   // shaveItHair init only reads MObjectArray, so we append our node into an array
   //
   MObjectArray hairNodes;
   hairNodes.append(m_dagPath.node());

   // export shaveAndHaircut info into a variable
   //
   if (SetHairInfo() != MS::kSuccess)
      return;

   // check if motionblur is enabled
   //
   const bool mb = m_scene->IsMotionBlurEnabled() && m_scene->IsObjectMotionBlurEnabled();
   const bool mb_deform = mb && m_scene->IsObjectDeformMotionBlurEnabled();

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   m_fnNode.findPlug("override_shader", &status);
   bool customAttributes = (status == MS::kSuccess);

   // number of curves
   //
   int numMainLines = m_hairInfo.numHairs;
   // Get curves cv count
   int renderLineLength = (m_hairInfo.numVertices / numMainLines) -1;

   // The shader nodes
   AtNode* shader;
   AtNode* ramp;
   AtNode* placmentNode;
   // The curvePoints array
   AtArray* curvePoints;
   // The curveWidths array
   AtArray* curveWidths = NULL;

   // Get the transforms matrix
   AtMatrix matrix;
   GetMatrix(matrix);

   // The numPoints array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   // The root and tip color array
   AtArray* rootColor    = AiArrayAllocate(numMainLines, 1, AI_TYPE_RGB);
   AtArray* tipColor     = AiArrayAllocate(numMainLines, 1, AI_TYPE_RGB);
   
   // A couple of arrays to keep track of where each hairline begins in the
   // points array (step 0)
   AtArray* curveNextLineStartsInterp = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);
   AtArray* curveNextLineStarts = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   // The u and v paramcoords array
   AtArray* curve_uparamcoord = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);
   AtArray* curve_vparamcoord = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);

   // Curves shader,
   //
   if (m_fnNode.findPlug("override_shader").asBool())
   {
      MPlugArray curveShaderPlug;
      m_fnNode.findPlug("shave_shader").connectedTo(curveShaderPlug, true, false);

      if (curveShaderPlug.length())
         shader = m_scene->ExportShader(curveShaderPlug[0].node());
   }
   else
   {
      shader = AiNode("ShaveHair");

      // fade hairstrand tip
      if (m_fnNode.findPlug("tipFade").asBool())
      {
         ramp = AiNode("MayaRamp");
         placmentNode = AiNode("MayaPlace2DTexture");
         AiNodeSetStr(ramp, "type", "v");
         AiNodeSetArray(ramp, "positions", AiArray(2, 1, AI_TYPE_FLOAT, 0.55f, 1.0f));
         AiNodeSetArray(ramp, "colors", AiArray(2, 1, AI_TYPE_RGB, AI_RGB_WHITE, AI_RGB_BLACK));
         AiNodeSetBool(ramp, "overrideUV", TRUE);

         AiNodeLink (placmentNode, "uv", ramp);
         AiNodeLink (ramp, "strand_opacity", shader);
      }
   }

   // Add shader uparam and vparam names
   AiNodeSetStr(shader, "uparam", "uparamcoord");
   AiNodeSetStr(shader, "vparam", "vparamcoord");

   // Add root and tip color
   AiNodeSetStr(shader, "rootcolor", "rootcolorparam");
   AiNodeSetStr(shader, "tipcolor",  "tipcolorparam");

   // set specular
   AiNodeSetFlt(shader, "spec", m_fnNode.findPlug("specular").asFloat());
   AiNodeSetFlt(shader, "gloss", (m_fnNode.findPlug("gloss").asFloat() * 2000));

   int numPoints = 0;
   int numPointsInterpolation = 0;
   for(int i = 0; i < numMainLines; i++)
   {
      numPoints += renderLineLength;
      numPointsInterpolation += renderLineLength + 2;

      // Set numPoints
      AiArraySetInt(curveNumPoints, i, (renderLineLength+2));

      // set uv
      int hairRootIndex = m_hairInfo.hairStartIndices[i];

      AiArraySetFlt(curve_uparamcoord, i, m_hairInfo.uvws[hairRootIndex].x);
      AiArraySetFlt(curve_vparamcoord, i, m_hairInfo.uvws[hairRootIndex].y);

      AtColor shaveRootColors;
      shaveRootColors.r = m_hairInfo.rootColors[i].r;
      shaveRootColors.g = m_hairInfo.rootColors[i].g;
      shaveRootColors.b = m_hairInfo.rootColors[i].b;

      AtColor shaveTipColors;
      shaveTipColors.r = m_hairInfo.tipColors[i].r;
      shaveTipColors.g = m_hairInfo.tipColors[i].g;
      shaveTipColors.b = m_hairInfo.tipColors[i].b;

      AiArraySetRGB(rootColor, i, shaveRootColors);
      AiArraySetRGB(tipColor, i, shaveTipColors);

      // Store start point for the line on the array
      AiArraySetInt(curveNextLineStartsInterp, i, (numPointsInterpolation));
      AiArraySetInt(curveNextLineStarts, i, (numPoints));
   }

   // alocate memory for all curve points
   if(mb_deform || mb)
      curvePoints = AiArrayAllocate(numPointsInterpolation, m_scene->GetNumMotionSteps(), AI_TYPE_POINT);
   else
      curvePoints = AiArrayAllocate(numPointsInterpolation, 1, AI_TYPE_POINT);

   curveWidths = AiArrayAllocate(numPoints, 1, AI_TYPE_FLOAT);

   // Assign the shader to the curve
   //
   AiNodeSetPtr(curve, "shader", shader);

  // Extra attributes
   AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
   AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");

   AiNodeDeclare(curve, "rootcolorparam", "uniform RGB");
   AiNodeDeclare(curve, "tipcolorparam", "uniform RGB");

   AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
   AiNodeDeclare(curve, "next_line_starts", "constant ARRAY INT");

   if (customAttributes)
   {
      AiNodeSetBool(curve, "receive_shadows", m_fnNode.findPlug("receive_shadows").asBool());
      AiNodeSetBool(curve, "self_shadows", m_fnNode.findPlug("self_shadows").asBool());

      // set opaque
      if (m_fnNode.findPlug("tipFade").asBool())
         AiNodeSetBool(curve, "opaque", FALSE);
      else
         AiNodeSetBool(curve, "opaque", TRUE);

      // Subsurface Scattering
      //
      AiNodeSetBool(curve, "sss_use_gi", m_fnNode.findPlug("sss_use_gi").asBool());
      AiNodeSetInt(curve, "sss_max_samples", m_fnNode.findPlug("sss_max_samples").asInt());
      AiNodeSetFlt(curve, "sss_sample_spacing", m_fnNode.findPlug("sss_sample_spacing").asFloat());

      AiNodeSetFlt(curve, "min_pixel_width", m_fnNode.findPlug("min_pixel_width").asFloat());
      AiNodeSetInt(curve, "mode", m_fnNode.findPlug("mode").asInt());
  }
  else
  {
      AiNodeSetBool(curve, "opaque", false);
  }

   // custom arnold attribute
   AtInt visibility = ComputeVisibility();
   AiNodeSetInt(curve, "visibility", visibility);

   // set tesselation method
   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "radius", curveWidths);
   AiNodeSetArray(curve, "uparamcoord", curve_uparamcoord);
   AiNodeSetArray(curve, "vparamcoord", curve_vparamcoord);
   AiNodeSetArray(curve, "rootcolorparam", rootColor);
   AiNodeSetArray(curve, "tipcolorparam", tipColor);

   AiNodeSetArray(curve, "num_points", curveNumPoints);
   AiNodeSetArray(curve, "points", curvePoints);
   AiNodeSetArray(curve, "next_line_starts_interp", curveNextLineStartsInterp);
   AiNodeSetArray(curve, "next_line_starts", curveNextLineStarts);

   // set transform matrix
   if (mb)
   {
      AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(curve, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(curve, "matrix", matrix);
   }

   // Ignore one or less cv curves
   if ( renderLineLength > 1)
   {
      ProcessHairLines(0, curvePoints, curveNextLineStartsInterp, curveNextLineStarts, curveWidths);
   }
   m_hairInfo.clear();
}

void CShaveTranslator::UpdateMotion(AtNode* curve, AtUInt step)
{
   if (SetHairInfo() != MS::kSuccess)
      return;

   // check if motionblur is enabled
   //
   const bool mb = m_scene->IsMotionBlurEnabled() && m_scene->IsObjectMotionBlurEnabled();
   const bool mb_deform = mb && m_scene->IsObjectDeformMotionBlurEnabled();

   // Get curves cv count
   int renderLineLength = (m_hairInfo.numVertices /  m_hairInfo.numHairs) -1;

   AtArray* curveWidths = AiNodeGetArray(curve, "radius");
   AtArray* curvePoints = AiNodeGetArray(curve, "points");
   AtArray* curveNextLineStartsInterp = AiNodeGetArray(curve, "next_line_starts_interp");
   AtArray* curveNextLineStarts = AiNodeGetArray(curve, "next_line_starts");

   // set transform matrix
   if (mb)
   {
      // Get the transforms matrix
      AtMatrix matrix;
      GetMatrix(matrix);

      AtArray* matrices = AiNodeGetArray(curve, "matrix");
      AiArraySetMtx(matrices, step, matrix);
   }
   // Ignore one or less cv curves
   if ( (mb_deform || mb ) && renderLineLength > 1)
   {
      ProcessHairLines(step, curvePoints, curveNextLineStartsInterp, curveNextLineStarts, curveWidths);
   }
   m_hairInfo.clear();
}


void CShaveTranslator::ProcessHairLines(AtUInt step,
                                          AtArray* curvePoints,
                                          AtArray* curveNextLineStartsInterp,
                                          AtArray* curveNextLineStarts,
                                          AtArray* curveWidths)
{
   // Get curves cv count
   int renderLineLength = (m_hairInfo.numVertices / m_hairInfo.numHairs) -1;

   // Process all hair lines
   int strand;
   for(strand = 0; strand < m_hairInfo.numHairs; strand++)
   {
      int curve_num_points_per_step = AiArrayGetInt(curveNextLineStartsInterp, (m_hairInfo.numHairs-1));

      // tells us where the nextline starts,
      //
      int curve_line_interp_starts_i = strand ? AiArrayGetInt(curveNextLineStartsInterp, strand-1) : 0 ;
      int curve_line_starts_i = strand ? AiArrayGetInt(curveNextLineStarts, strand-1) : 0 ;

      AtPoint arnoldCurvePoint;

      // curve radius
      float rootRadius = m_hairInfo.rootRadii[strand];
      float tipRadius = m_hairInfo.tipRadii[strand];
      float radiusStepSize = (rootRadius - tipRadius) / (renderLineLength-1);
      float curveSize = rootRadius;

      int index = 0;
      for (int j = m_hairInfo.hairStartIndices[strand]; j < m_hairInfo.hairEndIndices[strand]-1; j++)
      {

         if (index == 0)
         {
            AiV3Create(arnoldCurvePoint, (AtFloat)  m_hairInfo.vertices[j].x, (AtFloat)  m_hairInfo.vertices[j].y, (AtFloat)  m_hairInfo.vertices[j].z);
            AiArraySetPnt(curvePoints, (index + curve_line_interp_starts_i + (step * curve_num_points_per_step)), arnoldCurvePoint);
         }

         AiV3Create(arnoldCurvePoint, (AtFloat)  m_hairInfo.vertices[j].x, (AtFloat)  m_hairInfo.vertices[j].y, (AtFloat)  m_hairInfo.vertices[j].z);
         AiArraySetPnt(curvePoints, ((index+1) + curve_line_interp_starts_i + (step * curve_num_points_per_step)), arnoldCurvePoint);

         // Animated widths are not supported, so just on STEP 0
         if (step == 0)
         {
            AiArraySetFlt(curveWidths, (index + curve_line_starts_i) , static_cast<float>(curveSize));
         }

         if (index == (renderLineLength-1))
         {
            AiV3Create(arnoldCurvePoint, (AtFloat)  m_hairInfo.vertices[j].x, (AtFloat)  m_hairInfo.vertices[j].y, (AtFloat)  m_hairInfo.vertices[j].z);
            AiArraySetPnt(curvePoints, ((index+2) + curve_line_interp_starts_i + (step * curve_num_points_per_step)), arnoldCurvePoint);
         }
         index++;

         // guard against minus values
         if (curveSize > 0)
            curveSize -= radiusStepSize;
      }
   }
}


void CShaveTranslator::NodeInitializer(MObject& node)
{
   CDynamicAttrHelper helper = CDynamicAttrHelper(node);
   CShapeTranslator::MakeMayaVisibilityFlags(helper);
   CShapeTranslator::MakeArnoldVisibilityFlags(helper);
}
