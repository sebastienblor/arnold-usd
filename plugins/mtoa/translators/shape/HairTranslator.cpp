
#include "HairTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>

void CHairTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
   helper.MakeInput("min_pixel_width");
   helper.MakeInput("mode");

   CAttrData data;

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

AtNode* CHairTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("curves");
}

void CHairTranslator::Export( AtNode *curve )
{
   Update(curve);
}

void CHairTranslator::Update( AtNode *curve )
{
   MObject objectHairShape(m_dagPath.node());

   // Get connected shapes
   MDagPathArray connectedShapes;
   GetHairShapeMeshes(objectHairShape, connectedShapes);

   const bool hasConnectedShapes = connectedShapes.length() > 0;

   MFnDagNode fnDagNodeHairShape(objectHairShape);
   MFnDependencyNode fnDepNodeHair(objectHairShape);

   // Get hair lines
   m_numMainLines = GetHairLines(objectHairShape, m_hairLines);

   // Bail if there isn't any hair.
   if (!(m_numMainLines > 0)) return;

   // Set curve matrix for step 0
   ExportMatrix(curve, 0);

   // The num points array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints             = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);

   // The U and V param coords arrays
   AtArray* curveUParamCoord           = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_FLOAT);
   AtArray* curveVParamCoord           = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_FLOAT);

   // A couple of arrays to keep track of where each hairline begins in the points array (step 0)
   AtArray* curveNextLineStarts       = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);
   AtArray* curveNextLineStartsInterp = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);

   MPlug plug;
   // Check if we using a custom hair shader.
   AtNode* shader = NULL;
   plug = m_fnNode.findPlug("aiOverrideHair");
   if (!plug.isNull() && plug.asBool())
   {
      MPlugArray hairShaderPlugs;
      plug = fnDepNodeHair.findPlug("aiHairShader");
      if (!plug.isNull())
      {
         plug.connectedTo(hairShaderPlugs, true, false);
         if (hairShaderPlugs.length() > 0)
         {
            shader = ExportShader(hairShaderPlugs[0].node());
         }
      }
   }

   if (shader == NULL)
   {
      shader = AiNode("hair");
      MString hairShaderName = fnDepNodeHair.name();
      hairShaderName += "_hairShader";
      AiNodeSetStr(shader, "name", hairShaderName.asChar());

      // Add shader uparam and vparam names
      AiNodeSetStr(shader, "uparam", "uparamcoord");
      AiNodeSetStr(shader, "vparam", "vparamcoord");

      // TODO PROCESS MAYA HAIR SHADING ATTRIBUTES
      // With mtoa Hair shader (also TODO) ticket 109
      // https://trac.solidangle.com/mtoa/ticket/109
   }

   // Prepare connected shape info and intersection data
   // Support hairsystem that was applied to only one mesh
   MDagPath shapePath;
   MMeshIntersector meshInt;
   MFnMesh mesh;
   if (hasConnectedShapes)
   {
      shapePath = connectedShapes[0];
      MObject shapeNode = shapePath.node();
      const MMatrix matrix = shapePath.inclusiveMatrix();
      meshInt.create(shapeNode, matrix);

      mesh.setObject(shapeNode);
   }

   plug = m_fnNode.findPlug("aiExportHairIDs");
   bool export_curve_id = true;
   if (!plug.isNull())
   {
      export_curve_id = plug.asBool();
   }

   AtArray * curveID;
   if (export_curve_id)
   {
      curveID = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_UINT);
   }

   // Iterate over all lines to get sizes for AiArrayAllocate
   int numPoints = 0;
   int numPointsInterpolation = 0;
   MStatus status;
   MVectorArray line;
   for (AtUInt i = 0; i < m_numMainLines; ++i)
   {
      m_hairLines[i].GetCurvePoints(line);

      const int numRenderLineCVs = line.length();
      const int pointsInterpolationLine = numRenderLineCVs + 2;

      numPoints += numRenderLineCVs;
      numPointsInterpolation += pointsInterpolationLine;

      // Set num points
      AiArraySetInt(curveNumPoints, i, pointsInterpolationLine);

      // We should get the UV from the closest mesh for all connected shapes
      // To support a hairsystem that was applied to more than one mesh
      AtVector2 uvparam(AI_P2_ZERO);
      if (hasConnectedShapes) uvparam = GetHairRootUVs(line[0], meshInt, mesh);

      // Set UVs
      AiArraySetFlt(curveUParamCoord, i, uvparam.x);
      AiArraySetFlt(curveVParamCoord, i, uvparam.y);

      if (export_curve_id)
      {
         AiArraySetUInt(curveID, i, i);
      }

      // Store start point for the line on the array
      AiArraySetInt(curveNextLineStarts, i, numPoints);
      AiArraySetInt(curveNextLineStartsInterp, i, numPointsInterpolation);
   }

   ProcessRenderFlags(curve);

   // Allocate memory for all curve points and widths
   AtArray* curvePoints = AiArrayAllocate(numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_POINT);
   AtArray* curveWidths = AiArrayAllocate(numPoints,              GetNumMotionSteps(), AI_TYPE_FLOAT);

   ProcessHairLines(0,
                    curvePoints,
                    curveNextLineStartsInterp,
                    curveNextLineStarts,
                    curveWidths);
   // Clear out the temporary arrays.
   clear();

   // Extra attributes
   AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
   AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");
   AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
   AiNodeDeclare(curve, "next_line_starts",        "constant ARRAY INT");

   // Assign shader
   if (shader != NULL) AiNodeSetPtr(curve, "shader", shader);


   // Hair specific Arnold render settings.
   plug = GetFnNode().findPlug("aiMinPixelWidth");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = GetFnNode().findPlug("aiMode");
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "radius",                    curveWidths);
   AiNodeSetArray(curve, "num_points",                curveNumPoints);
   AiNodeSetArray(curve, "points",                    curvePoints);
   AiNodeSetArray(curve, "next_line_starts_interp",   curveNextLineStartsInterp);
   AiNodeSetArray(curve, "next_line_starts",          curveNextLineStarts);

   AiNodeSetArray(curve, "uparamcoord",               curveUParamCoord);
   AiNodeSetArray(curve, "vparamcoord",               curveVParamCoord);

   if(export_curve_id)
   {
      AiNodeDeclare(curve, "curve_id",        "uniform UINT");
      AiNodeSetArray(curve, "curve_id",       curveID);
   }
}

void CHairTranslator::ExportMotion(AtNode *curve, AtUInt step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve, step);

   // Same for object deformation, early out if it's not set.
   if (!IsDeformMotionBlurEnabled()) return;

   // Get hair lines
   MObject objectHairShape(m_dagPath.node());
   m_numMainLines = GetHairLines(objectHairShape, m_hairLines);
   
   ProcessHairLines(step,
                    AiNodeGetArray(curve, "points"),
                    AiNodeGetArray(curve, "next_line_starts_interp"),
                    AiNodeGetArray(curve, "next_line_starts"),
                    AiNodeGetArray(curve, "radius"));

   // Clear out the temporary arrays.
   clear();
}

void CHairTranslator::ProcessHairLines(AtUInt step,
                                       AtArray* curvePoints,
                                       AtArray* curveNextLineStartsInterp,
                                       AtArray* curveNextLineStarts,
                                       AtArray* curveWidths)
{
   const int numPointsPerStep         = AiArrayGetInt(curveNextLineStartsInterp,
                                                      curveNextLineStartsInterp->nelements-1);
   // Process all hair lines
   for (AtUInt strand = 0; strand < m_numMainLines; ++strand)
   {
      MVectorArray line;
      m_hairLines[strand].GetCurvePoints(line);
      MDoubleArray widths;
      m_hairLines[strand].GetCurveWidths(widths);
      const int renderLineLength = line.length();

      // Ignore one or less cv curves
      if (renderLineLength > 1)
      {
         const int curveLineStartsIdx       = strand ? AiArrayGetInt(curveNextLineStarts, strand-1)       : 0;
         const int curveLineInterpStartsIdx = strand ? AiArrayGetInt(curveNextLineStartsInterp, strand-1) : 0;

         MVector* lineVertex = &(line[0]);

         // We need a couple extra points for interpolation
         // One at the beginning and one at the end (JUST POINTS , NO ATTRS)
         AtPoint curvePoint;
         AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
         AiArraySetPnt(curvePoints,
                       curveLineInterpStartsIdx + (step * numPointsPerStep),
                       curvePoint);

         // Run down the strand adding the points and widths.
         for (int j = 0; j < renderLineLength; ++j, ++lineVertex)
         {
            AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
            AiArraySetPnt(curvePoints,
                          j+1 + curveLineInterpStartsIdx + (step * numPointsPerStep),
                          curvePoint);
            
            // Animated widths are not supported, so just export on step 0
            if (step == 0)
               AiArraySetFlt(curveWidths, j+curveLineStartsIdx, static_cast<float>(widths[j]/2.0));
         }

         // Last point duplicated.
         AiArraySetPnt(curvePoints,
                       renderLineLength+1 + curveLineInterpStartsIdx + (step * numPointsPerStep),
                       curvePoint);
       }
    }
 }

AtVector2 CHairTranslator::GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh)
{
   // Find the closest point on mesh from hair lineStart
   // And returns the UV at this point

   AtVector2 rootUVs;
   float uv[2];
 
   MPoint point(lineStart.x, lineStart.y, lineStart.z);
   MPointOnMesh closest;
   meshInt.getClosestPoint(point, closest);
   MPoint closestPoint(closest.getPoint());

   MString currentUVSet = mesh.currentUVSetName();
   mesh.getUVAtPoint(closestPoint, uv, MSpace::kObject, &currentUVSet);
   
   rootUVs.x = uv[0];
   rootUVs.y = uv[1];

   return rootUVs;
}

void CHairTranslator::GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes)
{
   MObjectArray follicles;
   MIntArray indices;

   // Loop through all follicles to find all connected shapes
   MHairSystem::getFollicle(hair, follicles, indices);
   for (AtUInt i = 0; i < follicles.length(); i++)
   {
      MFnDependencyNode depNodeFollicle(follicles[i]);
      MPlugArray meshes;
      depNodeFollicle.findPlug("inputMesh").connectedTo(meshes, true, false);
      if (meshes.length() > 0)
      {
         MFnDagNode meshDagNode(meshes[0].node());
         MDagPath dagPath;
         meshDagNode.getPath(dagPath);
         // Check if it is already there
         bool append = true;
         for (AtUInt j = 0; j < shapes.length(); j++)
         {
            if (shapes[j] == dagPath)
            {
               append = false;
               break;
            }
         }
         if (append)
            shapes.append(dagPath);
      }
   }
}

AtUInt CHairTranslator::GetHairLines(MObject& hair, std::vector<CHairLine>& hairLines)
{
   AtUInt numMainLines = 0;

   // Check out if there are pfx hairs
   MPlugArray pfxHairPlug;
   MFnDependencyNode fnDepNodeHair(hair);
   fnDepNodeHair.findPlug("outputRenderHairs").connectedTo(pfxHairPlug, false, true);
   if (pfxHairPlug.length() > 0)
   {
      MFnPfxGeometry pfxHair(pfxHairPlug[0].node());

      MRenderLineArray mainLines;
      MRenderLineArray leafLines;
      MRenderLineArray flowerLines;

      // And then all lines, we'll only need mainLines
      pfxHair.getLineData(mainLines, leafLines, flowerLines, true, true, true, true, true, true, true, false, true);
      numMainLines += mainLines.length();
      hairLines.reserve(numMainLines);

      // Move all renderlines to our CHairLine
      for (AtUInt i = 0; i < numMainLines; i++)
      {
         MStatus status;
         MRenderLine renderLine = mainLines.renderLine(i, &status);
         MVectorArray line  = renderLine.getLine();
         MDoubleArray width = renderLine.getWidth();

         CHairLine hairLine;
         hairLine.SetCurvePoints(line);
         hairLine.SetCurveWidths(width);
         hairLines.push_back(hairLine);
      }

      // As told in the docs, destructor does not free memory allocated by mainLines, leafLines etc
      mainLines.deleteArray();
      leafLines.deleteArray();
      flowerLines.deleteArray();
   }

   // Check for NURBS Curves
   MPlug outputHairPlug = fnDepNodeHair.findPlug("outputHair");
   if (outputHairPlug.numConnectedElements() > 0)
   {
      numMainLines += outputHairPlug.numConnectedElements();
      hairLines.reserve(numMainLines);

      float nurbsHairWidth = fnDepNodeHair.findPlug("hairWidth").asFloat();
      // NURBS hairs get width from the hairsystem multiplied by the rampattribute in that same node
      MRampAttribute hairWidthScale(fnDepNodeHair.findPlug("hairWidthScale"));

      // For all follicles, get the connected curve
      for (unsigned int i = 0; i < outputHairPlug.numConnectedElements(); i++)
      {
         MPlug connection = outputHairPlug.elementByPhysicalIndex(i);
         MPlugArray follicleHairPlug;
         connection.connectedTo(follicleHairPlug, false, true);

         if (follicleHairPlug.length() > 0)
         {
            MPlugArray nurbsHairPlug;
            MFnDependencyNode depNodeCurveHair(follicleHairPlug[0].node());
            depNodeCurveHair.findPlug("outCurve").connectedTo(nurbsHairPlug, false, true);

            if (nurbsHairPlug.length() > 0)
            {
               MFnNurbsCurve follicleCurve(nurbsHairPlug[0].node());

               MPointArray cvs;
               follicleCurve.getCVs(cvs, MSpace::kWorld);
               MVectorArray line;
               MDoubleArray width;

               // Transform from MPointArray to MVectorArray
               for (unsigned int j = 0; j < cvs.length(); j++)
               {
                  MVector vector(cvs[j]);
                  line.append(vector);
                  double rampParam;
                  follicleCurve.getParamAtPoint(cvs[j], rampParam);

                  // Transform the param value to 0 .. 1 range
                  rampParam = rampParam / float(follicleCurve.numSpans());
                  float rampValue;
                  hairWidthScale.getValueAtPosition(float(rampParam), rampValue);
                  width.append(nurbsHairWidth * rampValue);
               }

               CHairLine hairLine;
               hairLine.SetCurvePoints(line);
               hairLine.SetCurveWidths(width);
               hairLines.push_back(hairLine);
            }
         }
      }
   }

   return hairLines.size();
}
