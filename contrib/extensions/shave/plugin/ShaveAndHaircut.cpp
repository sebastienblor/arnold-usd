#include "ShaveAndHaircut.h"

#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <ai_nodes.h>

class CExtensionAttrHelper;

MStatus CShaveTranslator::UpdateHairInfo()
{
   MStatus status;
   // shaveItHair init only reads MObjectArray, so we append our node into an array
   //
   MObjectArray hairNodes;
   hairNodes.append(m_dagPath.node());

   // Export shaveAndHaircut info into a variable
   //
   status = shaveAPI::exportHair(hairNodes, &m_hairInfo);
   return status;
}

AtNode*  CShaveTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("curves");
}

void CShaveTranslator::Export(AtNode* curve)
{
   // Only translate the shave node if its marked as a active
   if (!FindMayaObjectPlug("active").asBool())
      return;

   Update(curve);
}

void CShaveTranslator::Update(AtNode* curve)
{
   // Export shaveAndHaircut info into a variable
   if (UpdateHairInfo() != MS::kSuccess)
      return;

   // Get number of curves
   const int numMainLines = m_hairInfo.numHairs;
   // Get number of line CV
   const int numRenderLineCVs = (m_hairInfo.numVertices / numMainLines) - 1;

   // The shader nodes
   // TODO: Kill these and export it properly.
   AtNode* shader       = NULL;
   AtNode* ramp         = NULL;
   AtNode* placementNode= NULL;

   // Export the transform matrix
   ExportMatrix(curve, 0);

   // Get the visibiliy and render flags set.
   ProcessRenderFlags(curve);

   // Curves shader
   MPlug plug;
   plug = m_fnNode.findPlug("aiOverrideHair");
   if (!plug.isNull() && plug.asBool())
   {
      MPlugArray curveShaderPlug;
      plug = m_fnNode.findPlug("aiHairShader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlug, true, false);
         if (curveShaderPlug.length() > 0)
         {
            shader = ExportNode(curveShaderPlug[0]);
         }
      }
   }

   // Default to the ShaveHair shader if nothing else has been set.
   if (shader == NULL)
   {
      shader = AiNode("ShaveHair");

      // Fade the hairstrand towards the tip.
      plug = m_fnNode.findPlug("tipFade");
      if (plug.asBool())
      {
         // If tip fade is on then the hairs are not opaque no
         // matter the attribute setting.
         AiNodeSetBool(curve, "opaque", false);
         
         ramp = AiNode("MayaRamp");
         placementNode = AiNode("MayaPlace2DTexture");
         AiNodeSetStr(ramp, "type", "v");

         AiNodeSetUInt(ramp, "numEntries", 2);
         AiNodeSetFlt(ramp, "position0", 0.55f);
         AiNodeSetRGB(ramp, "color0", 1.0f, 1.0f, 1.0f);
         AiNodeSetFlt(ramp, "position1", 1.0f);
         AiNodeSetRGB(ramp, "color1", 0.0f, 0.0f, 0.0f);

         AiNodeLink (placementNode, "uvCoord", ramp);
         AiNodeLink (ramp, "strand_opacity", shader);
      }

      // Add shader uparam and vparam names.
      AiNodeSetStr(shader, "uparam",   "uparamcoord");
      AiNodeSetStr(shader, "vparam",   "vparamcoord");
   
      // Add root and tip color.
      AiNodeSetStr(shader, "rootcolor","rootcolorparam");
      AiNodeSetStr(shader, "tipcolor", "tipcolorparam");
   
      // Set specular and gloss.
      plug = m_fnNode.findPlug("specular");
      AiNodeSetFlt(shader, "spec", plug.asFloat());

      plug = m_fnNode.findPlug("gloss");
      AiNodeSetFlt(shader, "gloss", plug.asFloat() * 2000.0f);
   }
   
   if (shader != NULL)
   {
      AiNodeSetPtr(curve, "shader", shader);
   }

   // Should we export the hair root and tip colour? Default to true.
   // Turning it off gives us a slimmer ass.
   plug = m_fnNode.findPlug("aiExportHairColors");
   bool export_curve_color = true;
   if (!plug.isNull())
   {
      export_curve_color = plug.asBool();
   }
   
   // The numPoints array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints             = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   // The root and tip color array
   AtArray* rootColor = NULL;
   AtArray* tipColor = NULL;
   if(export_curve_color)
   {
      rootColor = AiArrayAllocate(numMainLines, 1, AI_TYPE_RGB);
      tipColor  = AiArrayAllocate(numMainLines, 1, AI_TYPE_RGB);
   }

   plug = m_fnNode.findPlug("aiExportHairIDs");
   bool export_curve_id = true;
   if (!plug.isNull())
   {
      export_curve_id = plug.asBool();
   }

   AtArray * curveID = NULL;
   if (export_curve_id)
   {
      curveID = AiArrayAllocate(numMainLines, 1, AI_TYPE_UINT);
   }
   
   // A couple of arrays to keep track of where each hairline begins in the
   // points array (step 0)
   AtArray* curveNextLineStartsInterp  = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);
   AtArray* curveNextLineStarts        = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   // The U and V paramcoords array
   AtArray* curveUParamCoord          = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);
   AtArray* curveVParamCoord          = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);


   int numPoints = 0;
   int numPointsInterpolation = 0;
   for (int i = 0; i < numMainLines; ++i)
   {
      numPoints += numRenderLineCVs;
      numPointsInterpolation += numRenderLineCVs+2;

      // Set numPoints
      AiArraySetInt(curveNumPoints, i, (numRenderLineCVs+2));

      int hairRootIndex = m_hairInfo.hairStartIndices[i];

      // Set UV
      AiArraySetFlt(curveUParamCoord, i, m_hairInfo.uvws[hairRootIndex].x);
      AiArraySetFlt(curveVParamCoord, i, m_hairInfo.uvws[hairRootIndex].y);

      // Root and tip colours for the ShaveHair shader.
      // TODO: Make exporting all the info for the ShaveHair shader an option.
      if(export_curve_color)
      {
         AtColor shaveRootColors;
         shaveRootColors.r = m_hairInfo.rootColors[i].r;
         shaveRootColors.g = m_hairInfo.rootColors[i].g;
         shaveRootColors.b = m_hairInfo.rootColors[i].b;
   
         AtColor shaveTipColors;
         shaveTipColors.r = m_hairInfo.tipColors[i].r;
         shaveTipColors.g = m_hairInfo.tipColors[i].g;
         shaveTipColors.b = m_hairInfo.tipColors[i].b;

         AiArraySetRGB(rootColor, i, shaveRootColors);
         AiArraySetRGB(tipColor,  i, shaveTipColors);
      }

      if (export_curve_id)
      {
         AiArraySetUInt(curveID, i, (i));
      }

      // Store start point for the line on the array
      AiArraySetInt(curveNextLineStarts, i, (numPoints));
      AiArraySetInt(curveNextLineStartsInterp, i, (numPointsInterpolation));
   }

   // Allocate memory for all curve points and widths
   AtArray* curveWidths = AiArrayAllocate(numPoints, 1, AI_TYPE_FLOAT);

   // TODO: Change this to use RequiresMotionDeformData()
   AtArray* curvePoints = NULL;
   if (RequiresMotionData() || m_motionDeform)
      curvePoints = AiArrayAllocate(numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_POINT);
   else
      curvePoints = AiArrayAllocate(numPointsInterpolation, 1, AI_TYPE_POINT);

   // Set the required arrays
   AiNodeSetArray(curve, "num_points",             curveNumPoints);
   AiNodeSetArray(curve, "points",                 curvePoints);
   AiNodeSetArray(curve, "radius",                 curveWidths);

   // Add our extra attributes
   AiNodeDeclare(curve, "uparamcoord",             "uniform FLOAT");
   AiNodeDeclare(curve, "vparamcoord",             "uniform FLOAT");
   AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
   AiNodeDeclare(curve, "next_line_starts",        "constant ARRAY INT");

   // Set all extra data arrays
   AiNodeSetArray(curve, "uparamcoord",            curveUParamCoord);
   AiNodeSetArray(curve, "vparamcoord",            curveVParamCoord);
   AiNodeSetArray(curve, "next_line_starts_interp",curveNextLineStartsInterp);
   AiNodeSetArray(curve, "next_line_starts",       curveNextLineStarts);
   
   // Finally add and set the hair color arrays as needed.
   if(export_curve_color)
   {
      AiNodeDeclare(curve, "rootcolorparam",       "uniform RGB");
      AiNodeDeclare(curve, "tipcolorparam",        "uniform RGB");
      AiNodeSetArray(curve, "rootcolorparam",      rootColor);
      AiNodeSetArray(curve, "tipcolorparam",       tipColor);
   }

   if(export_curve_id)
   {
      AiNodeDeclare(curve, "curve_id",        "uniform UINT");
      AiNodeSetArray(curve, "curve_id",       curveID);
   }
   
   // Set tesselation method
   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Hair specific Arnold render settings.
   plug = m_fnNode.findPlug("aiMinPixelWidth");
   if (!plug.isNull())
   {
      AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());
   }

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = m_fnNode.findPlug("aiMode");
   if (!plug.isNull())
   {
      AiNodeSetInt(curve, "mode", plug.asInt());
   }

   // Ignore one or less cv curves.
   if (numRenderLineCVs > 1)
   {
      ProcessHairLines(0,
                       curvePoints,
                       curveNextLineStartsInterp,
                       curveNextLineStarts,
                       curveWidths);
   }

   
   // Clear/release the data from Shave.
    m_hairInfo.clear();
}

void CShaveTranslator::ExportMotion(AtNode* curve, AtUInt step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve, step);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   // Bail early if we've trouble getting data from Shave.
   if (UpdateHairInfo() != MS::kSuccess) return;
   
   // Get curves cv count
   const int numRenderLineCVs = (m_hairInfo.numVertices /  m_hairInfo.numHairs) -1;

   if (numRenderLineCVs > 1)
   {
      ProcessHairLines(step,
                       AiNodeGetArray(curve, "points"),
                       AiNodeGetArray(curve, "next_line_starts_interp"),
                       AiNodeGetArray(curve, "next_line_starts"),
                       AiNodeGetArray(curve, "radius"));
   }

   // Clear/release the data from Shave.
   m_hairInfo.clear();
}


void CShaveTranslator::ProcessHairLines(AtUInt step,
                                        AtArray* curvePoints,
                                        AtArray* curveNextLineStartsInterp,
                                        AtArray* curveNextLineStarts,
                                        AtArray* curveWidths)
{
   // Get number of CVs per hair/line/strand.
   const int numRenderLineCVs = (m_hairInfo.numVertices / m_hairInfo.numHairs)-1;
   const int numPointsPerStep = AiArrayGetInt(curveNextLineStartsInterp, (m_hairInfo.numHairs-1));


   // Process all hair lines
   for (int strand = 0; strand < m_hairInfo.numHairs; ++strand)
   {
      // Tells us where the nextline starts.
      const int curveLineInterpStartsIdx = strand ? AiArrayGetInt(curveNextLineStartsInterp, strand-1) : 0 ;
      const int curveLineStartsIdx       = strand ? AiArrayGetInt(curveNextLineStarts, strand-1) : 0 ;

      // Curve radius
      const float rootRadius = m_hairInfo.rootRadii[strand];
      const float tipRadius  = m_hairInfo.tipRadii[strand];
      const float radiusStepSize = (rootRadius - tipRadius) / (numRenderLineCVs-1);

      float curveSize = rootRadius;
      int index = 0;
      
      // This is a pointer to the first vertex in the hair strand.
      // It's incremement in the for loop below.
      shaveAPI::Vertex * vertex = &(m_hairInfo.vertices[m_hairInfo.hairStartIndices[strand]]);

      AtPoint arnoldCurvePoint;
      AiV3Create(arnoldCurvePoint, vertex->x, vertex->y, vertex->z);
      
      // Create a first point on the curve. Start and end are duplicated vertices.
      AiArraySetPnt(curvePoints,
                    (index + curveLineInterpStartsIdx + (step * numPointsPerStep)),
                    arnoldCurvePoint);
      
      // Loop through all the hair strands.
      for (int j = m_hairInfo.hairStartIndices[strand];
           j < m_hairInfo.hairEndIndices[strand]-1;
           ++j, ++index, ++vertex)
      {
         // Add the point.
         AiV3Create(arnoldCurvePoint, vertex->x, vertex->y, vertex->z);
         AiArraySetPnt(curvePoints,
                       ((index+1) + curveLineInterpStartsIdx + (step * numPointsPerStep)),
                       arnoldCurvePoint);

         // Animated widths are not supported, so just on STEP 0
         if (step == 0)
            AiArraySetFlt(curveWidths, (index + curveLineStartsIdx) , curveSize);

         // guard against minus values
         if (curveSize > 0) curveSize -= radiusStepSize;
      }

      // Last point (duplicate of the previous point).
      AiArraySetPnt(curvePoints,
                    ((index+1) + curveLineInterpStartsIdx + (step * numPointsPerStep)),
                    arnoldCurvePoint);
   }
}

void CShaveTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
   helper.MakeInput("min_pixel_width");
   helper.MakeInput("mode");

   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "aiExportHairColors";
   data.shortName = "ai_export_hair_colours";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "aiExportHairIDs";
   data.shortName = "ai_export_hair_ids";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiOverrideHair";
   data.shortName = "ai_override_hair";
   helper.MakeInputBoolean(data);

   data.name = "aiHairShader";
   data.shortName = "ai_hair_shader";
   helper.MakeInputNode(data);
}
