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

const char* CShaveTranslator::GetArnoldNodeType()
{
   return "curves";
}

void CShaveTranslator::Export( AtNode* curve)
{
   // Only translate the shave node if its marked as a active
   if (!m_fnNode.findPlug("active").asBool())
      return;

   Update(curve);
}

void CShaveTranslator::Update(AtNode* curve)
{
   // shaveItHair init only reads MObjectArray, so we append our node into an array
   //
   MObjectArray hairNodes;
   hairNodes.append(m_dagPath.node());

   // export shaveAndHaircut info into a variable
   if (SetHairInfo() != MS::kSuccess) return;

   // number of curves
   const int numMainLines = m_hairInfo.numHairs;
   // Get curves cv count
   const int renderLineLength = (m_hairInfo.numVertices / numMainLines) -1;

   // The shader nodes
   // TODO: Kill these and export it properly.
   AtNode* shader       = NULL;
   AtNode* ramp         = NULL;
   AtNode* placmentNode = NULL;
   // The curvePoints array
   AtArray* curvePoints;
   // The curveWidths array
   AtArray* curveWidths = NULL;

   // Export the transform matrix
   ExportMatrix(curve, 0);

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

   // Get the visibiliy and render flags set.
   ProcessRenderFlags(curve);
   
   // Curves shader
   //
   MPlug plug;
   plug = m_fnNode.findPlug("override_shader");
   if (!plug.isNull() && plug.asBool())
   {
      MPlugArray curveShaderPlug;
      plug = m_fnNode.findPlug("hair_shader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlug, true, false);
         if (curveShaderPlug.length())
         {
            shader = m_scene->ExportShader(curveShaderPlug[0].node());
         }
      }
   }

   // Default to the ShaveHair shader if nothing else has been set.
   if ( shader == NULL)
   {
      shader = AiNode("ShaveHair");

      // Fade hairstrand tip
      plug = m_fnNode.findPlug("tipFade");
      if (plug.asBool())
      {
         // If tip fade is on then the hairs are not opaque no
         // matter the attribute setting.
         AiNodeSetBool(curve, "opaque", false);
         
         ramp = AiNode("MayaRamp");
         placmentNode = AiNode("MayaPlace2DTexture");
         AiNodeSetStr(ramp, "type", "v");
         AiNodeSetArray(ramp, "positions", AiArray(2, 1, AI_TYPE_FLOAT, 0.55f, 1.0f));
         AiNodeSetArray(ramp, "colors", AiArray(2, 1, AI_TYPE_RGB, AI_RGB_WHITE, AI_RGB_BLACK));

         AiNodeLink (placmentNode, "uvCoord", ramp);
         AiNodeLink (ramp, "strand_opacity", shader);
      }

      // Add shader uparam and vparam names
      AiNodeSetStr(shader, "uparam", "uparamcoord");
      AiNodeSetStr(shader, "vparam", "vparamcoord");
   
      // Add root and tip color
      AiNodeSetStr(shader, "rootcolor", "rootcolorparam");
      AiNodeSetStr(shader, "tipcolor",  "tipcolorparam");
   
      // set specular
      plug = m_fnNode.findPlug("specular");
      AiNodeSetFlt(shader, "spec", plug.asFloat());
      plug = m_fnNode.findPlug("gloss");
      AiNodeSetFlt(shader, "gloss", plug.asFloat() * 2000);
   }

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
   if(m_motionDeform || m_motion)
      curvePoints = AiArrayAllocate(numPointsInterpolation, m_scene->GetNumMotionSteps(), AI_TYPE_POINT);
   else
      curvePoints = AiArrayAllocate(numPointsInterpolation, 1, AI_TYPE_POINT);

   curveWidths = AiArrayAllocate(numPoints, 1, AI_TYPE_FLOAT);

   // Assign the shader to the curve
   //
   if ( shader != NULL ) AiNodeSetPtr(curve, "shader", shader);

  // Extra attributes
   AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
   AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");

   AiNodeDeclare(curve, "rootcolorparam", "uniform RGB");
   AiNodeDeclare(curve, "tipcolorparam", "uniform RGB");

   AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
   AiNodeDeclare(curve, "next_line_starts", "constant ARRAY INT");

   // Hair specific Arnold render settings.
   plug = m_fnNode.findPlug("min_pixel_width");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = m_fnNode.findPlug("mode");
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   // set tesselation method
   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "radius",                    curveWidths);
   AiNodeSetArray(curve, "uparamcoord",               curve_uparamcoord);
   AiNodeSetArray(curve, "vparamcoord",               curve_vparamcoord);
   AiNodeSetArray(curve, "rootcolorparam",            rootColor);
   AiNodeSetArray(curve, "tipcolorparam",             tipColor);

   AiNodeSetArray(curve, "num_points",                curveNumPoints);
   AiNodeSetArray(curve, "points",                    curvePoints);
   AiNodeSetArray(curve, "next_line_starts_interp",   curveNextLineStartsInterp);
   AiNodeSetArray(curve, "next_line_starts",          curveNextLineStarts);

   // Ignore one or less cv curves
   if ( renderLineLength > 1)
   {
      ProcessHairLines(0, curvePoints, curveNextLineStartsInterp, curveNextLineStarts, curveWidths);
   }
   m_hairInfo.clear();
}

void CShaveTranslator::ExportMotion(AtNode* curve, AtUInt step)
{
   // Bail early if we've trouble getting data from Shave.
   if (SetHairInfo() != MS::kSuccess) return;
   // Set transform matrix
   ExportMatrix(curve, step);

   // Check if motionblur is enabled
   const bool mb        = m_scene->IsMotionBlurEnabled() && m_scene->IsObjectMotionBlurEnabled();
   const bool mb_deform = mb && m_scene->IsObjectDeformMotionBlurEnabled();

   // Get curves cv count
   int renderLineLength = (m_hairInfo.numVertices /  m_hairInfo.numHairs) -1;

   AtArray* curveWidths                = AiNodeGetArray(curve, "radius");
   AtArray* curvePoints                = AiNodeGetArray(curve, "points");
   AtArray* curveNextLineStartsInterp  = AiNodeGetArray(curve, "next_line_starts_interp");
   AtArray* curveNextLineStarts        = AiNodeGetArray(curve, "next_line_starts");

   // Ignore one or less cv curves
   if ( (mb_deform || mb ) && renderLineLength > 1)
   {
      ProcessHairLines(step,
                       curvePoints,
                       curveNextLineStartsInterp,
                       curveNextLineStarts,
                       curveWidths);
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
   const int renderLineLength = (m_hairInfo.numVertices / m_hairInfo.numHairs) -1;

   // Process all hair lines
   for( int strand = 0; strand < m_hairInfo.numHairs; ++strand)
   {
      const int curve_num_points_per_step = AiArrayGetInt(curveNextLineStartsInterp, (m_hairInfo.numHairs-1));

      // tells us where the nextline starts,
      //
      const int curve_line_interp_starts_i = strand ? AiArrayGetInt(curveNextLineStartsInterp, strand-1) : 0 ;
      const int curve_line_starts_i = strand ? AiArrayGetInt(curveNextLineStarts, strand-1) : 0 ;

      AtPoint arnoldCurvePoint;

      // curve radius
      const float rootRadius = m_hairInfo.rootRadii[strand];
      const float tipRadius = m_hairInfo.tipRadii[strand];
      const float radiusStepSize = (rootRadius - tipRadius) / (renderLineLength-1);
      float curveSize = rootRadius;

      int index = 0;
      for (int j = m_hairInfo.hairStartIndices[strand]; j < m_hairInfo.hairEndIndices[strand]-1; ++j, ++index)
      {

         if (index == 0)
         {
            AiV3Create(arnoldCurvePoint,
                       (AtFloat)  m_hairInfo.vertices[j].x,
                       (AtFloat)  m_hairInfo.vertices[j].y,
                       (AtFloat)  m_hairInfo.vertices[j].z);
            AiArraySetPnt(curvePoints,
                          (index + curve_line_interp_starts_i + (step * curve_num_points_per_step)),
                          arnoldCurvePoint);
         }

         AiV3Create(arnoldCurvePoint,
                    (AtFloat)  m_hairInfo.vertices[j].x,
                    (AtFloat)  m_hairInfo.vertices[j].y,
                    (AtFloat)  m_hairInfo.vertices[j].z);
         AiArraySetPnt(curvePoints,
                       ((index+1) + curve_line_interp_starts_i + (step * curve_num_points_per_step)),
                       arnoldCurvePoint);

         // Animated widths are not supported, so just on STEP 0
         if (step == 0)
         {
            AiArraySetFlt(curveWidths, (index + curve_line_starts_i) , static_cast<float>(curveSize));
         }

         if (index == (renderLineLength-1))
         {
            AiV3Create(arnoldCurvePoint,
                       (AtFloat)  m_hairInfo.vertices[j].x,
                       (AtFloat)  m_hairInfo.vertices[j].y,
                       (AtFloat)  m_hairInfo.vertices[j].z);
            AiArraySetPnt(curvePoints,
                          ((index+2) + curve_line_interp_starts_i + (step * curve_num_points_per_step)),
                          arnoldCurvePoint);
         }

         // guard against minus values
         if (curveSize > 0) curveSize -= radiusStepSize;
      }
   }
}


void CShaveTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
//   helper.MakeInput("min_pixel_width");
//   helper.MakeInput("mode");

}
