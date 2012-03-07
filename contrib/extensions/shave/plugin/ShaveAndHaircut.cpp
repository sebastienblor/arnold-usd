#include "ShaveAndHaircut.h"

#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <ai_nodes.h>

#include "utils/time.h"

class CExtensionAttrHelper;

MStatus CShaveTranslator::UpdateHairInfo()
{
   MStatus status;
   // shaveItHair init only reads MObjectArray, so we append our node into an array
   //
   MObjectArray hairNodes(1, m_dagPath.node());

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

AtNode* CShaveTranslator::CreateShaveShader(AtNode* curve)
{
   AtNode* shader = AiNode("ShaveHair");
   char nodeName[MAX_NAME_SIZE];
   AiNodeSetStr(shader, "name", NodeUniqueName(shader, nodeName));
   MFnDependencyNode fnNode(GetMayaObject());
   // Fade the hairstrand towards the tip.
   MPlug plug = fnNode.findPlug("tipFade");
   if (plug.asBool())
   {
      // If tip fade is on then the hairs are not opaque no
      // matter the attribute setting.
      AiNodeSetBool(curve, "opaque", false);

      AtNode* ramp = AiNode("MayaRamp");
      AiNodeSetStr(ramp, "name", NodeUniqueName(ramp, nodeName));
      AtNode* placementNode = AiNode("MayaPlace2DTexture");
      AiNodeSetStr(placementNode, "name", NodeUniqueName(placementNode, nodeName));
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
   plug = fnNode.findPlug("specular");
   ProcessParameter(shader, "spec", AI_TYPE_FLOAT, plug);

   plug = fnNode.findPlug("gloss");
   ProcessParameter(shader, "gloss", AI_TYPE_FLOAT, plug);

   plug = fnNode.findPlug("specularTint");
   ProcessParameter(shader, "spec_color", AI_TYPE_RGB, plug);

   plug = fnNode.findPlug("amb/diff");
   ProcessParameter(shader, "ambdiff", AI_TYPE_FLOAT, plug);

   plug = fnNode.findPlug("aiDiffuseCache");
   ProcessParameter(shader, "diffuse_cache", AI_TYPE_BOOLEAN, plug);

   plug = fnNode.findPlug("aiIndirect");
   ProcessParameter(shader, "kd_ind", AI_TYPE_FLOAT, plug);

   plug = fnNode.findPlug("aiDirectDiffuse");
   ProcessParameter(shader, "direct_diffuse", AI_TYPE_FLOAT, plug);

   plug = fnNode.findPlug("aiAovDirectDiffuse");
   ProcessParameter(shader, "aov_direct_diffuse", AI_TYPE_STRING, plug);

   plug = fnNode.findPlug("aiAovIndirectDiffuse");
   ProcessParameter(shader, "aov_indirect_diffuse", AI_TYPE_STRING, plug);

   plug = fnNode.findPlug("aiAovDirectSpecular");
   ProcessParameter(shader, "aov_direct_specular", AI_TYPE_STRING, plug);

   return shader;
}

void CShaveTranslator::Update(AtNode* curve)
{
   // Export shaveAndHaircut info into a variable
   if (UpdateHairInfo() != MS::kSuccess)
      return;

   // The shader nodes
   // TODO: Kill these and export it properly.
   AtNode* shader       = NULL;

   // Export the transform matrix
   ExportMatrix(curve, 0);

   // Get the visibiliy and render flags set.
   ProcessRenderFlags(curve);

   // Curves shader
   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());
   plug = fnNode.findPlug("aiOverrideHair");
   if (!plug.isNull() && plug.asBool())
   {
      MPlugArray curveShaderPlug;
      plug = fnNode.findPlug("aiHairShader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlug, true, false);
         if (curveShaderPlug.length() > 0)
         {
            shader = ExportRootShader(curveShaderPlug[0]);
         }
      }
   }

   // Default to the ShaveHair shader if nothing else has been set.
   if (shader == NULL)
   {
      shader = ExportRootShader(CreateShaveShader(curve));
   }
   
   if (shader != NULL)
   {
      AiNodeSetPtr(curve, "shader", shader);
   }

   // Should we export the hair root and tip colour? Default to true.
   // Turning it off gives us a slimmer ass.
   plug = fnNode.findPlug("aiExportHairColors");
   bool export_curve_color = true;
   if (!plug.isNull())
   {
      export_curve_color = plug.asBool();
   }
   
   // The numPoints array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints             = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_INT);

   // The root and tip color array
   AtArray* rootColor = NULL;
   AtArray* tipColor = NULL;
   if(export_curve_color)
   {
      rootColor = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_RGB);
      tipColor  = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_RGB);
   }

   plug = fnNode.findPlug("aiExportHairIDs");
   bool export_curve_id = true;
   if (!plug.isNull())
   {
      export_curve_id = plug.asBool();
   }

   AtArray * curveID = NULL;
   if (export_curve_id)
   {
      curveID = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_UINT);
   }
   

   // The U and V paramcoords array
   AtArray* curveUParamCoord          = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_FLOAT);
   AtArray* curveVParamCoord          = AiArrayAllocate(m_hairInfo.numHairs, 1, AI_TYPE_FLOAT);

   for (int i = 0; i < m_hairInfo.numHairs; ++i)
   {
      int hairRootIndex = m_hairInfo.hairStartIndices[i];

      // Set numPoints
      int numRenderLineCVs = m_hairInfo.hairEndIndices[i] - hairRootIndex;
      AiArraySetInt(curveNumPoints, i, (numRenderLineCVs+2));

      // Set UV
      AiArraySetFlt(curveUParamCoord, i, m_hairInfo.uvws[hairRootIndex].x);
      AiArraySetFlt(curveVParamCoord, i, m_hairInfo.uvws[hairRootIndex].y);

      // Root and tip colours for the ShaveHair shader.
      // TODO: Make exporting all the info for the ShaveHair shader an option.
      if (export_curve_color)
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
   }

   // Allocate memory for all curve points and widths (aka radii)

   // The required size of the radius array varies by curve basis.  The equation is:
   //
   //  (np - 4) / vstep + 2
   //
   // where np is the number of points in the strand, and vstep is the number of points per segment
   // (which varies depending on which curve basis you choose).
   //
   // table of vstep values:
   //   3 - bezier
   //   1 - b-spline
   //   1 - catmull-rom
   //   2 - hermite
   //   4 - power
   // for shave we use catmull-rom, which means np -2.
   // It seems (though I have yet to confirm with SA) that the discrepancy is due to duplicate knots at the
   // beginning and end of each strand in arnold curves, which are not considered in the "radius" array.
   // Shave does not include these extra knots in its HairInfo.vertices array.
   AtArray* curveWidths = AiArrayAllocate(m_hairInfo.numHairVertices, 1, AI_TYPE_FLOAT);

   int numPointsInterpolation = m_hairInfo.numHairVertices + (m_hairInfo.numHairs * 2);

   AtArray* curvePoints = NULL;
   // TODO: Change this to use RequiresMotionDeformData()
   if (RequiresMotionData() && IsMotionBlurEnabled(MTOA_MBLUR_DEFORM))
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
   AiNodeSetArray(curve, "uparamcoord",            curveUParamCoord);
   AiNodeSetArray(curve, "vparamcoord",            curveVParamCoord);

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
   plug = fnNode.findPlug("aiMinPixelWidth");
   if (!plug.isNull())
   {
      AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());
   }

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = fnNode.findPlug("aiMode");
   if (!plug.isNull())
   {
      AiNodeSetInt(curve, "mode", plug.asInt());
   }

   ProcessHairLines(0,
                    curvePoints,
                    curveNumPoints,
                    curveWidths);

   m_hairInfo.clear();
}

void CShaveTranslator::ExportMotion(AtNode* curve, unsigned int step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve, step);

   if (IsMotionBlurEnabled(MTOA_MBLUR_DEFORM))
   {
      // Bail early if we've trouble getting data from Shave.
      //
      if (UpdateHairInfo() != MS::kSuccess) return;

      ProcessHairLines(step,
                       AiNodeGetArray(curve, "points"),
                       AiNodeGetArray(curve, "num_points"),
                       AiNodeGetArray(curve, "radius"));

      m_hairInfo.clear();
   }
}

void CShaveTranslator::ProcessHairLines(const unsigned int step,
                                        AtArray* curvePoints,
                                        AtArray* curveNumPoints,
                                        AtArray* curveWidths)
{
   // Get number of CVs per hair/line/strand.
   const int numPointsPerStep = m_hairInfo.numHairVertices + (m_hairInfo.numHairs * 2);

   // Tells us where the nextline starts.
   int curveLineInterpStartsIdx = 0;
   int curveLineStartsIdx       = 0;

   // Process all hair lines
   for (int strand = 0; strand < m_hairInfo.numHairs; ++strand)
   {
      int numHairPoints = m_hairInfo.hairEndIndices[strand] - m_hairInfo.hairStartIndices[strand];

      // Curve radius
      const float rootRadius = m_hairInfo.rootRadii[strand];
      const float tipRadius  = m_hairInfo.tipRadii[strand];
      const float radiusStepSize = (rootRadius - tipRadius) / (numHairPoints-2);

      float curveSize = rootRadius;
      int index = 0;

      // This is a pointer to the first vertex in the hair strand.
      // It's incremement in the for loop below.
      shaveAPI::Vertex * vertex = &(m_hairInfo.vertices[m_hairInfo.hairStartIndices[strand]]);

      AtPoint arnoldCurvePoint;
      AiV3Create(arnoldCurvePoint, vertex->x, vertex->y, vertex->z);
      
      // Create a first point on the curve. Start and end are duplicated vertices.
      AiArraySetPnt(curvePoints,
                    (curveLineInterpStartsIdx + (step * numPointsPerStep)),
                    arnoldCurvePoint);
      
      // Loop through all the hair strand indices.
      for (int j = m_hairInfo.hairStartIndices[strand];
           j < m_hairInfo.hairEndIndices[strand];
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
         if (curveSize > 0.0f) curveSize -= radiusStepSize;
         if (curveSize < 0.0f) curveSize = 0.0f;
      }

      // Last point (duplicate of the previous point).
      AiArraySetPnt(curvePoints,
                    ((index+1) + curveLineInterpStartsIdx + (step * numPointsPerStep)),
                    arnoldCurvePoint);

      curveLineInterpStartsIdx += numHairPoints +2;
      curveLineStartsIdx += numHairPoints;
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

   CExtensionAttrHelper helper2(context.maya, "ShaveHair");
   helper2.MakeInput("diffuse_cache");

   helper2.MakeInput("direct_diffuse");

   helper2.GetAttrData("kd_ind", data);
   data.name = "aiIndirect";
   helper2.MakeInputFloat(data);

   helper2.MakeInput("aov_direct_diffuse");
   helper2.MakeInput("aov_direct_specular");
   helper2.MakeInput("aov_indirect_diffuse");
}
