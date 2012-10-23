#include "CurveTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <vector>

void CCurveTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
   helper.MakeInput("min_pixel_width");
   helper.MakeInput("mode");

   CAttrData data;

   // FIXME: all attributes that are arnold-specific should have an "ai" prefix
   data.defaultValue.BOOL = false;
   data.name = "aiRenderCurve";
   data.shortName = "rcurve";
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT = 0.01f;
   data.name = "aiCurveWidth";
   data.shortName = "cwdth";
   helper.MakeInputFloat(data);

   data.defaultValue.INT = 5;
   data.name = "aiSampleRate";
   data.shortName = "srate";
   data.hasMin = true;
   data.min.INT = 1;
   helper.MakeInputInt(data);

   // FIXME: look into using CDagTranslator::MakeMayaVisibilityFlags
   data.defaultValue.BOOL = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);

   data.name = "aiCurveShader";
   data.shortName = "ai_curve_shader";
   helper.MakeInputNode(data);

   /*data.name = "widthProfile";
   data.shortName = "wdthP";
   helper.MakeInputCurveRamp(data);*/

}

AtNode* CCurveTranslator::CreateArnoldNodes()
{
   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());

   plug = fnNode.findPlug("aiRenderCurve");
   if (!plug.isNull() && plug.asBool() == false)
   {
      return NULL;
   }

   return AddArnoldNode("curves");
}

void CCurveTranslator::Export( AtNode *curve )
{
   Update(curve);
}

void CCurveTranslator::Update( AtNode *curve )
{
   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());

   plug = fnNode.findPlug("aiRenderCurve");
   if (!plug.isNull() && plug.asBool() == false)
   {
      return;
   }

   MObject objectCurveShape(m_dagPath.node());

   MFnDagNode fnDagNodeCurveShape(objectCurveShape);
   MFnDependencyNode fnDepNodeCurve(objectCurveShape);

   // Get curve lines
   MStatus stat;
   stat = GetCurveLines(objectCurveShape);
   // Bail if there isn't any curve data
   if (stat != MStatus::kSuccess) return;

   // Set curve matrix for step 0
   ExportMatrix(curve, 0);

   // The num points array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints             = AiArrayAllocate(1, 1, AI_TYPE_INT);


   // Check if we using a custom curve shader.
   AtNode* shader = NULL;
   MPlugArray curveShaderPlugs;
   plug = fnDepNodeCurve.findPlug("aiCurveShader");
   if (!plug.isNull())
   {
      plug.connectedTo(curveShaderPlugs, true, false);
      if (curveShaderPlugs.length() > 0)
      {
         shader = ExportRootShader(curveShaderPlugs[0]);
      }
   }

   if (shader == NULL)
   {
      shader = AiNode("hair");
      MString hairShaderName = fnDepNodeCurve.name();
      hairShaderName += "_hairShader";
      AiNodeSetStr(shader, "name", hairShaderName.asChar());

      // Add shader uparam and vparam names
      AiNodeSetStr(shader, "uparam", "uparamcoord");
      AiNodeSetStr(shader, "vparam", "vparamcoord");

      shader = ExportRootShader(shader);
   }

   // Iterate over all lines to get sizes for AiArrayAllocate
   int numPoints = 0;
   int numPointsInterpolation = 0;
   MStatus status;
   MVectorArray line;

   mayaCurve.GetCurvePoints(line);

   const int numRenderLineCVs = line.length();
   const int pointsInterpolationLine = numRenderLineCVs + 2;

   numPoints += numRenderLineCVs;
   numPointsInterpolation += pointsInterpolationLine;

   // Set num points
   AiArraySetInt(curveNumPoints, 0, pointsInterpolationLine);


   mayaCurve.curveNumPoints = numPoints;
   mayaCurve.curveNumPointsInterp = numPointsInterpolation;


   ProcessRenderFlags(curve);

   // Allocate memory for all curve points and widths
   AtArray* curvePoints = AiArrayAllocate(numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_POINT);
   AtArray* curveWidths = AiArrayAllocate(numPoints,              GetNumMotionSteps(), AI_TYPE_FLOAT);
   AtArray* curveColors = AiArrayAllocate(1,                      GetNumMotionSteps(), AI_TYPE_RGB);

   ProcessCurveLines(0,
                    curvePoints,
                    curveWidths,
                    curveColors);

   // Extra attributes
   AiNodeDeclare(curve, "colors",                  "uniform  ARRAY RGB");


   // Assign shader
   if (shader != NULL) AiNodeSetPtr(curve, "shader", shader);


   // curve specific Arnold render settings.
   plug = FindMayaPlug("aiMinPixelWidth");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = FindMayaPlug("aiMode");
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "radius",                    curveWidths);
   AiNodeSetArray(curve, "num_points",                curveNumPoints);
   AiNodeSetArray(curve, "points",                    curvePoints);
   AiNodeSetArray(curve, "colors",                    curveColors);

}

void CCurveTranslator::ExportMotion(AtNode *curve, unsigned int step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve, step);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;


   // Get curve lines
   MObject objectCurveShape(m_dagPath.node());
   MStatus stat = GetCurveLines(objectCurveShape);

   if (stat == MStatus::kSuccess)
   {
   ProcessCurveLines(step,
                    AiNodeGetArray(curve, "points"),
                    AiNodeGetArray(curve, "radius"),
                    AiNodeGetArray(curve, "colors"));
   }
}

void CCurveTranslator::ProcessCurveLines(unsigned int step,
                                       AtArray* curvePoints,
                                       AtArray* curveWidths,
                                       AtArray* curveColors)
{
   const int numPointsPerStep         = mayaCurve.curveNumPointsInterp;

   // Process all curve lines

   MVectorArray line;
   mayaCurve.GetCurvePoints(line);
   MDoubleArray widths;
   mayaCurve.GetCurveWidths(widths);
   MVectorArray colors;
   mayaCurve.GetCurveColors(colors);
   const int renderLineLength = line.length();

   // Ignore one or less cv curves
   if (renderLineLength > 1)
   {

      MVector* lineVertex = &(line[0]);

      // We need a couple extra points for interpolation
      // One at the beginning and one at the end (JUST POINTS , NO ATTRS)
      AtPoint curvePoint;
      AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
      AiArraySetPnt(curvePoints, (step * numPointsPerStep), curvePoint);

      AiArraySetRGB(curveColors, (step),
                    AiColorCreate(static_cast<float>(colors[0].x), static_cast<float>(colors[0].y), static_cast<float>(colors[0].z)));

      // Run down the strand adding the points and widths.
      for (int j = 0; j < renderLineLength; ++j, ++lineVertex)
      {
         AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
         AiArraySetPnt(curvePoints, j+1 + (step * numPointsPerStep), curvePoint);
         // Animated widths are not supported, so just export on step 0
         if (step == 0)
         {
            AiArraySetFlt(curveWidths, j, static_cast<float>(widths[j]/2.0));
            // AiArraySetRGB(curveColors, j, AiColorCreate(colors[j].x, colors[j].y, colors[j].z));
         }
      }

      // Last point duplicated.
      AiArraySetPnt(curvePoints, renderLineLength+1 +  (step * numPointsPerStep), curvePoint);
    }
 }

MStatus CCurveTranslator::GetCurveLines(MObject& curve)
{
   MPlug plug;
   float globalWidth = 1.0;
   MFnDependencyNode fnNode(GetMayaObject());
   plug = fnNode.findPlug("aiCurveWidth");
   if (!plug.isNull())
   {
     globalWidth =  plug.asFloat();
   }

   //plug = fnNode.findPlug("widthProfile");
   //MRampAttribute curveWidthTable(plug);


   MFnDependencyNode fnDepNodeCurve(curve);

   MVector fcolor;
   fcolor = MVector(1.0, 1.0, 1.0);

   MStatus stat;
   MPlug outputCurvePlug = fnDepNodeCurve.findPlug("editPoints", &stat);

   if (stat == MStatus::kSuccess)
   {
      MFnNurbsCurve nurbsCurve(curve);

      MPointArray cvs;
      double start, end;
      unsigned int numcvs;
      unsigned int sampleRate = 5;
      double incPerSample;

      plug = fnNode.findPlug("aiSampleRate");
      if (!plug.isNull())
      {
        sampleRate =  plug.asInt();
        if (sampleRate < 1)
         sampleRate = 1;
      }
      
      nurbsCurve.getKnotDomain(start, end);
      numcvs = (unsigned int)std::ceil((end - start) * sampleRate); 
      incPerSample = 1.0 / sampleRate;

      MPoint point;
      for(unsigned int i = 0; i < numcvs - 1; i++)
      {
         nurbsCurve.getPointAtParam(start + incPerSample * (double)i, point, MSpace::kWorld);
         cvs.append(point);
      }
      nurbsCurve.getPointAtParam(end, point, MSpace::kWorld);
      cvs.append(point);

      if (numcvs <=0)
      {
         return MStatus::kFailure;
      }

      MVectorArray line(numcvs);
      MDoubleArray width(numcvs);
      MVectorArray color(numcvs,fcolor);

      double minDomain, maxDomain = 0;
      nurbsCurve.getKnotDomain(minDomain,maxDomain);

      /// HACK, but ramp stuff is a pain to initialize
      //  this rule basically says, if the curve has no entries, then set it all to 1.0
      /*if (curveWidthTable.getNumEntries() == 0)
      {
        MFloatArray positions(1);
        MFloatArray values(1);
        MIntArray interps(1);
        positions[0] = 0.0;
        values[0] = 1.0;
        interps[0] = 1;
        curveWidthTable.addEntries(positions,values,interps);
      }*/


      // Transform from MPointArray to MVectorArray
      for (unsigned int j = 0; j < numcvs; j++)
      {
         MVector vector(cvs[j]);
         line[j] = vector;
         /*double rampParam;
         double closestPointTolerance = .1;

         nurbsCurve.getParamAtPoint(cvs[j], rampParam, closestPointTolerance);

         //cout << cvs[j] << " " << j << " -> " << rampParam << endl;
         // Transform the param value to 0 .. 1 range
         rampParam = (rampParam + minDomain) / (maxDomain + minDomain);
         float rampValue;
         curveWidthTable.getValueAtPosition(float(rampParam), rampValue);
         width[j] = globalWidth * rampValue;*/
         width[j] = globalWidth;

      }

      mayaCurve.SetCurvePoints(line);
      mayaCurve.SetCurveWidths(width);
      mayaCurve.SetCurveColors(color);

   }

   return MStatus::kSuccess;
}
