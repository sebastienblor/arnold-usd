#include "CurveTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVectorArray.h>

#include <vector>

typedef std::vector<float> CurveWidths;

AmMap<std::string, CurveWidths *> s_processedWidths;

static inline void ClearProcessedWidths()
{
   if (s_processedWidths.empty())
      return;


   AmMap<std::string, CurveWidths *>::iterator it = s_processedWidths.begin();
   AmMap<std::string, CurveWidths *>::iterator itEnd = s_processedWidths.end();

   for ( ; it != itEnd; ++it)
      delete it->second;
   
   s_processedWidths.clear();
}

CCurveTranslator::~CCurveTranslator()
{
   // just clear it if something was exported with this node
   // otherwise we'd clear too often during export 
   // (dummy translators are created/deleted all the time)
   if (!mayaCurve.points.empty())
      ClearProcessedWidths();

}
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
   data.hasMin = true;
   data.min.FLT = 0.0f;
   data.hasSoftMax = true;
   data.softMax.FLT = 1.0f;
   helper.MakeInputFloat(data);

   data.defaultValue.INT = 5;
   data.name = "aiSampleRate";
   data.shortName = "srate";
   data.hasMin = true;
   data.min.INT = 1;
   data.softMax.INT = 20;
   helper.MakeInputInt(data);

   data.hasSoftMax = false;

   data.name = "aiCurveShader";
   data.shortName = "ai_curve_shader";
   data.hasMin = false;
   data.defaultValue.RGB = AI_RGB_BLACK;
   helper.MakeInputRGB(data);

   data.defaultValue.BOOL = true;
   data.name = "aiExportRefPoints";
   data.shortName = "ai_exprpt";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   /*data.name = "widthProfile";
   data.shortName = "wdthP";
   helper.MakeInputCurveRamp(data);*/

}

AtNode* CCurveTranslator::CreateArnoldNodes()
{
   // first make sure the processed widths are cleared
   ClearProcessedWidths();

   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());

   plug = FindMayaPlug("aiRenderCurve");
   if (!plug.isNull() && plug.asBool() == false)
   {
      return NULL;
   }

   return AddArnoldNode("curves");
}

void CCurveTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   
   // we used to only set RECREATE_NODE for the .create attribute
   //MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   //if ((plugName == ".create")) SetUpdateMode(AI_RECREATE_NODE);

   // but ticket #2399 showed that curves aren't updated properly in arnold core,
   // for example when the curves width change.
   // So now we're always forcing to recreate the node
   if (!IsTransformPlug(plug))
      SetUpdateMode(AI_RECREATE_NODE);
   
   CShapeTranslator::NodeChanged(node, plug);
}

void CCurveTranslator::Export( AtNode *curve )
{
   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());

   plug = FindMayaPlug("aiRenderCurve");
   if (!plug.isNull() && plug.asBool() == false)
   {
      return;
   }

   plug = FindMayaPlug("aiExportRefPoints");

   if (!plug.isNull())
      exportReferenceObject = plug.asBool();
   else
      exportReferenceObject = false;

   MObject objectCurveShape(m_dagPath.node());

   MFnDagNode fnDagNodeCurveShape(objectCurveShape);
   MFnDependencyNode fnDepNodeCurve(objectCurveShape);

   // Get curve lines
   MStatus stat;
   stat = GetCurveLines(objectCurveShape, 0);
   // Bail if there isn't any curve data
   if (stat != MStatus::kSuccess) return;

   // Set curve matrix for step 0
   ExportMatrix(curve);

   // The num points array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints             = AiArrayAllocate(1, 1, AI_TYPE_INT);

   // Check if we using a custom curve shader.
   if (RequiresShaderExport())
   {
      AtNode* shader = NULL;
      MPlugArray curveShaderPlugs;
      plug = FindMayaPlug("aiCurveShader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlugs, true, false);
         if (curveShaderPlugs.length() > 0)
         {
            shader = ExportConnectedNode(curveShaderPlugs[0]);
         }
      }
      
      if (shader == NULL)
      {
         // check if the internal root shader was already created in a previous export
         shader = GetArnoldNode("rootShader");
         if (shader == NULL)
            shader = AddArnoldNode("hair", "rootShader");
            
         MString hairShaderName = fnDepNodeCurve.name();
         hairShaderName += "_hairShader";
         AiNodeSetStr(shader, "name", hairShaderName.asChar());

         // Add shader uparam and vparam names
         AiNodeSetStr(shader, "uparam", "uparamcoord");
         AiNodeSetStr(shader, "vparam", "vparamcoord");
      }
      SetRootShader(shader);      
   }   

   // Iterate over all lines to get sizes for AiArrayAllocate
   MStatus status;

   const int numPoints = (int)mayaCurve.points.size();
   const int numPointsInterpolation = numPoints + 2;

   // Set num points
   AiArraySetInt(curveNumPoints, 0, numPointsInterpolation);

   ProcessRenderFlags(curve);

   // Allocate memory for all curve points and widths
   AtArray* curvePoints = AiArrayAllocate(numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_POINT);
   AtArray* curveWidths = AiArrayAllocate(numPoints,              mayaCurve.widthConnected ? GetNumMotionSteps() : 1, AI_TYPE_FLOAT);
   AtArray* curveColors = AiArrayAllocate(1,                      1, AI_TYPE_RGB);
   AtArray* referenceCurvePoints = exportReferenceObject ? AiArrayAllocate(numPoints, 1, AI_TYPE_POINT) : 0;

   ProcessCurveLines(0,
                    curvePoints,
                    referenceCurvePoints,
                    curveWidths,
                    curveColors);

   // Extra attributes
   AiNodeDeclare(curve, "colors",                  "uniform  ARRAY RGB");

   if (exportReferenceObject)
   {
      AiNodeDeclare(curve, "Pref", "varying POINT");
      AiNodeSetArray(curve, "Pref", referenceCurvePoints);
   }

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

void CCurveTranslator::ExportMotion(AtNode *curve)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;


   // Get curve lines
   MObject objectCurveShape(m_dagPath.node());
   exportReferenceObject = false;
   int step = GetMotionStep();
   MStatus stat = GetCurveLines(objectCurveShape, step);

   if (stat == MStatus::kSuccess)
   {
   ProcessCurveLines(step,
                    AiNodeGetArray(curve, "points"),
                    0,
                    mayaCurve.widthConnected ? AiNodeGetArray(curve, "radius") : 0,
                    0);
   }
}

void CCurveTranslator::ProcessCurveLines(unsigned int step,
                                       AtArray* curvePoints,
                                       AtArray* referenceCurvePoints,
                                       AtArray* curveWidths,
                                       AtArray* curveColors)
{
   // Process all curve lines
   const int renderLineLength = mayaCurve.points.size();

   const int numPointsPerStep = renderLineLength + 2;

   exportReferenceObject = exportReferenceObject && (referenceCurvePoints != 0);

   // Ignore one or less cv curves
   if (renderLineLength > 1)
   {
      // We need a couple extra points for interpolation
      // One at the beginning and one at the end (JUST POINTS , NO ATTRS)
      AiArraySetPnt(curvePoints, (step * numPointsPerStep), mayaCurve.points[0]);
      if (curveColors)
         AiArraySetRGB(curveColors, step, mayaCurve.color);

      // Run down the strand adding the points and widths.
      for (int j = 0; j < renderLineLength; ++j)
      {
         AiArraySetPnt(curvePoints, j + 1 + (step * numPointsPerStep), mayaCurve.points[j]);
         // Animated widths are not supported, so just export on step 0
         if (step == 0)
         {
            if (exportReferenceObject)
               AiArraySetPnt(referenceCurvePoints, j, mayaCurve.referencePoints[j]);
            AiArraySetFlt(curveWidths, j, static_cast<float>(mayaCurve.widths[j] / 2.0));
         }
         else if (mayaCurve.widthConnected)
            AiArraySetFlt(curveWidths, j + (step * renderLineLength), static_cast<float>(mayaCurve.widths[j] / 2.0));
      }
      AiArraySetPnt(curvePoints, renderLineLength + 1 +  (step * numPointsPerStep), mayaCurve.points[renderLineLength - 1]);
    }
 }

MStatus CCurveTranslator::GetCurveLines(MObject& curve, unsigned int step)
{
   mayaCurve.clear(); // just to be sure
   MFnDependencyNode fnDepNodeCurve(curve);
   MStatus stat;
   MPlug outputCurvePlug = fnDepNodeCurve.findPlug("editPoints", &stat);
   if (stat == MStatus::kSuccess)
   {
      float globalWidth = 1.0;
      MPlug widthPlug = FindMayaPlug("aiCurveWidth");
      if (!widthPlug.isNull())
         globalWidth =  widthPlug.asFloat();

      MFnNurbsCurve nurbsCurve(curve);

      double start, end;
      unsigned int numcvs;
      unsigned int sampleRate = 5;
      double incPerSample;

      MPlug plug = FindMayaPlug("aiSampleRate");
      if (!plug.isNull())
      {
        sampleRate =  plug.asInt();
        if (sampleRate < 1)
         sampleRate = 1;
      }
      
      nurbsCurve.getKnotDomain(start, end);
      numcvs = (unsigned int)std::ceil((end - start) * sampleRate); 
      incPerSample = 1.0 / sampleRate;

      if (numcvs <=0)
         return MStatus::kFailure;

      mayaCurve.points.resize(numcvs);

      MPoint point;
      for(unsigned int i = 0; i < (numcvs - 1); i++)
      {
         nurbsCurve.getPointAtParam(MIN(start + incPerSample * (double)i, end), point, MSpace::kWorld);
         AiV3Create(mayaCurve.points[i], (float)point.x, (float)point.y, (float)point.z);
      }
      nurbsCurve.getPointAtParam(end, point, MSpace::kWorld);
      AiV3Create(mayaCurve.points[numcvs - 1], (float)point.x, (float)point.y, (float)point.z);

      mayaCurve.widths.resize(numcvs);
      mayaCurve.color = AI_RGB_WHITE;

      MPlugArray conns;
      widthPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
      {
         mayaCurve.widthConnected = true;

         // our static map contains the width arrays for a given target shader +  a given amount of CVs.
         // The key in the map is the name of the shader + '#' + the amount of CVs
         // This static map is cleared whenever a new node is created, an update is requested, or a node is deleted
         // since it's meant to remain local to each IPR update where curves are re-generated
         MString widthHash = MFnDependencyNode(conns[0].node()).name();
         widthHash += "#";
         widthHash += (int)numcvs;
         std::string widthHashStr = widthHash.asChar();

         CurveWidths *&processedCurveWidth = s_processedWidths[widthHashStr];
         if (processedCurveWidth == NULL)
         {
            // we haven't processed these widths yet, let's do it
            MFloatArray uCoords;
            MFloatArray vCoords;
            uCoords.setLength(numcvs);
            vCoords.setLength(numcvs);
            MFloatVectorArray resultColors;
            MFloatVectorArray resultTransparencies;
            MFloatArray filterSizes;
            filterSizes.setLength(numcvs);
            for (unsigned int i = 0; i < numcvs; ++i)
            {
               uCoords[i] = 0.0f;
               vCoords[i] = (float)i / (float)(numcvs + 1);
               filterSizes[i] = 0.001f;
            }
            MRenderUtil::sampleShadingNetwork(widthPlug.name(), numcvs, false, false, MFloatMatrix().setToIdentity(), 
                                             0, &uCoords, &vCoords, 0, 0, 0, 0, &filterSizes,
                                             resultColors, resultTransparencies);
            for (unsigned int i = 0; i < numcvs; ++i)
               mayaCurve.widths[i] = resultColors[i].x;

            processedCurveWidth = new std::vector<float>(mayaCurve.widths);
         } else
         {
            // we have already processed this width shader for the same amount of CVs, so it's useless to do it again.
            // we just copy the previous widths
            mayaCurve.widths = *processedCurveWidth;
         }
      }
      else 
      {
         mayaCurve.widthConnected = false;
         if (step == 0)
         {
            for (unsigned int i = 0; i < numcvs; ++i)
               mayaCurve.widths[i] = globalWidth;
         }
      }

      if (exportReferenceObject)
      {
         plug = FindMayaPlug("referenceObject");
         plug.connectedTo(conns, true, false);
         if (conns.length() > 0)
         {
            MObject referenceObject = conns[0].node();
            MFnNurbsCurve referenceCurve(referenceObject, &stat);
            if (stat)
            {
               referenceCurve.getKnotDomain(start, end);
               incPerSample = (end - start) / (double)numcvs;
               mayaCurve.referencePoints.resize(numcvs);
               for(unsigned int i = 0; i < numcvs - 1; i++)
               {
                  referenceCurve.getPointAtParam(MIN(start + incPerSample * (double)i, end), point, MSpace::kWorld);
                  AiV3Create(mayaCurve.referencePoints[i], (float)point.x, (float)point.y, (float)point.z);
               }
               referenceCurve.getPointAtParam(end, point, MSpace::kWorld);
               AiV3Create(mayaCurve.referencePoints[numcvs - 1], (float)point.x, (float)point.y, (float)point.z);
            }
            else exportReferenceObject = false;
         }
         else exportReferenceObject = false;
      }
   }
   return MStatus::kSuccess;
}
