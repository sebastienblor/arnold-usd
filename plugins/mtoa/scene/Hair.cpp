
#include "Hair.h"

#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MRenderLineArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MRenderLine.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MStatus.h>
#include <maya/MDagPathArray.h>
#include <maya/MHairSystem.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MRampAttribute.h>
#include <maya/MFn.h>

#include <maya/MFnPfxGeometry.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>

#include <ai.h>

#include <vector>

namespace
{
   AtVector2 GetHairRootUVs(const MVectorArray& line, const MDagPathArray& shapes)
   {
      AtVector2 rootUVs;
      
      // Find the closest point from the line[0] to the surface (shapes[index])
      // We should check for the closest point for all connected shapes
      // To support a hairsystem that was applied to more than one mesh

      MPoint closestPoint;
      double distance = 0;
      float uv[2] = {0.0f, 0.0f};
      MString currentUVSet;

      for (unsigned int i = 0; i < shapes.length(); i++)
      {
         MObject shapeNode = shapes[i].node();
         MFnMesh mesh(shapeNode);
         const MMatrix matrix = shapes[i].inclusiveMatrix();
         MMeshIntersector meshInt;
         meshInt.create(shapeNode, matrix);
         MPoint point(line[0].x, line[0].y, line[0].z);
         MPointOnMesh closest;

         currentUVSet = mesh.currentUVSetName();
         meshInt.getClosestPoint(point, closest);
         MPoint closePoint(closest.getPoint());
         
         if ((closePoint.distanceTo(point) < distance) || (i == 0))
         {
            distance = closePoint.distanceTo(point);
            closestPoint = closePoint;
            mesh.getUVAtPoint(closestPoint, uv, MSpace::kObject, &currentUVSet);
         }
      }
      
      rootUVs.x = uv[0];
      rootUVs.y = uv[1];

      return rootUVs;
   }

   void GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes)
   {
      MObjectArray follicles;
      MIntArray indices;

      // Loop through all follicles to find all connected shapes
      MHairSystem::getFollicle(hair, follicles, indices);
      for(AtUInt i = 0; i < follicles.length(); i++)
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
            for(unsigned int j = 0; j<shapes.length(); j++)
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
}

AtNode *CHairTranslator::Export()
{
   // Create the curve node
   //
   AtNode *curve = AiNode("curves");
   AiNodeSetStr(curve, "name", m_dagPath.fullPathName().asChar());
   Update(curve);
   return curve;
}

void CHairTranslator::Update(AtNode *curve)
{
   MObject child = m_dagPath.child(0);
   if (!child.hasFn(MFn::kHairSystem))
      return;

   MDagPath dagPath;
   MDagPath::getAPathTo(child, dagPath);
   MObject objectHairShape(dagPath.node());
   MFnDagNode fnDagNodeHairShape(objectHairShape);
   MFnDependencyNode fnDepNodeHair(objectHairShape);

   AtMatrix matrix;
   GetMatrix(matrix);

   AtUInt step = 0;

   bool motionBlur = m_scene->IsMotionBlurEnabled();
   bool motionBlurDeform = m_scene->IsObjectDeformMotionBlurEnabled();

   m_numMainLines = GetHairLines(objectHairShape, m_hairLines);

   if (!(m_numMainLines > 0))
      return;

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNodeHairShape.findPlug("override_hair", &status);
   bool customAttributes = (status == MS::kSuccess);
   bool transformHairDefinition = true;

   AtNode* shader = NULL;
   bool receiveShadows = true;
   bool selfShadows = true;
   bool sssUseGI = true;
   AtUInt sssMaxSamples = 100000;
   AtUInt mode = 0;
   AtInt visibility = AI_RAY_ALL;

   AtFloat sssSampleSpacing = 0.1f;
   AtFloat minPixelWidth = 0.0f;

   AtArray* curvePoints = NULL;     // The curve points array
   AtArray* curveWidths = NULL;     // The curve widths/radius array
   
   // The num points array (int array the size of numLines, no motionsteps)
   AtArray* curveNumPoints = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);

   // The uparamcoords array
   AtArray* curveUParamCoord = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_FLOAT);

   // The uparamcoords array
   AtArray* curveVParamCoord = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_FLOAT);

   // A couple of arrays to keep track of where each hairline begins in the points array (step 0)
   AtArray* curveNextLineStartsInterp = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);
   AtArray* curveNextLineStarts = AiArrayAllocate(m_numMainLines, 1, AI_TYPE_INT);

   if (customAttributes)
   {
      // Check if we override maya's hair attributes with an arnold hair shader
      if (fnDepNodeHair.findPlug("override_hair").asBool())
      {
         MPlugArray hairShaderPlug;
         fnDepNodeHair.findPlug("hair_shader").connectedTo(hairShaderPlug, true, false);
         if (hairShaderPlug.length()>0)
         {
            shader = m_scene->ExportShader(hairShaderPlug[0].node());
            // Avoid later processing since there is a shader connected
            transformHairDefinition = false;
         }
      }

      // Attributes
      receiveShadows   = fnDagNodeHairShape.findPlug("receive_shadows").asBool();
      selfShadows      = fnDagNodeHairShape.findPlug("self_shadows").asBool();
      sssUseGI         = fnDagNodeHairShape.findPlug("sss_use_gi").asBool();
      sssMaxSamples    = fnDagNodeHairShape.findPlug("sss_max_samples").asInt();
      sssSampleSpacing = fnDagNodeHairShape.findPlug("sss_sample_spacing").asFloat();
      minPixelWidth    = fnDagNodeHairShape.findPlug("min_pixel_width").asFloat();
      mode             = fnDagNodeHairShape.findPlug("mode").asInt();

      if (!fnDagNodeHairShape.findPlug("primary_visibility").asBool())
         visibility &= ~AI_RAY_CAMERA;

      if (!fnDagNodeHairShape.findPlug("castsShadows").asBool())
         visibility &= ~AI_RAY_SHADOW;

      if (!fnDagNodeHairShape.findPlug("visibleInReflections").asBool())
         visibility &= ~AI_RAY_REFLECTED;

      if (!fnDagNodeHairShape.findPlug("visibleInRefractions").asBool())
         visibility &= ~AI_RAY_REFRACTED;

      if (!fnDagNodeHairShape.findPlug("diffuse_visibility").asBool())
         visibility &= ~AI_RAY_DIFFUSE;

      if (!fnDagNodeHairShape.findPlug("glossy_visibility").asBool())
         visibility &= ~AI_RAY_GLOSSY;
   }

   // No custom shader assigned, try to transform maya hair's definition
   if (transformHairDefinition)
   {
      shader = AiNode("hair");
      MString hairShaderName = fnDepNodeHair.name();
      hairShaderName += "_hairShader";
      AiNodeSetStr(shader, "name", hairShaderName.asChar());

      // TODO PROCESS MAYA HAIR SHADING ATTRIBUTES
      // With mtoa Hair shader (also TODO) ticket 109 https://trac.solidangle.com/mtoa/ticket/109
   }

   // Iterate over all lines to get sizes for AiArrayAllocate
   int numPoints = 0;
   int numPointsInterpolation = 0;
   for (AtUInt i = 0; i < m_numMainLines; i++)
   {
      MStatus status;
      MVectorArray line = m_hairLines[i].GetCurvePoints();
      int renderLineLength = line.length();
      numPoints += renderLineLength;

      numPointsInterpolation += renderLineLength + 2;

      // Set num points
      AiArraySetInt(curveNumPoints, i, (renderLineLength+2));

      // Get the UV values of the surface point on which the hair is rooted
      if (customAttributes)
      {
         // Get connected shapes
         MDagPathArray connectedShapes;
         GetHairShapeMeshes(objectHairShape, connectedShapes);
         AtVector2 uvparam = GetHairRootUVs(line, connectedShapes);
         AiArraySetFlt(curveUParamCoord, i, uvparam.x);
         AiArraySetFlt(curveVParamCoord, i, uvparam.y);
      }

      // Store start point for the line on the array
      AiArraySetInt(curveNextLineStartsInterp, i, (numPointsInterpolation));
      AiArraySetInt(curveNextLineStarts, i, (numPoints));
   }

   // Allocate memory for remaining arrays
   if (motionBlurDeform || motionBlur)
   {
      curvePoints = AiArrayAllocate(numPointsInterpolation, m_scene->GetNumMotionSteps(), AI_TYPE_POINT);
   }
   else
   {
      curvePoints = AiArrayAllocate(numPointsInterpolation, 1, AI_TYPE_POINT);
   }
   curveWidths = AiArrayAllocate(numPoints, 1, AI_TYPE_FLOAT);

   // Set curve matrix
   AiNodeSetMatrix(curve, "matrix", matrix);

   // Assign shader
   AiNodeSetPtr(curve, "shader", shader);

   // Add shader uparam and vparam names
   AiNodeSetStr(shader, "uparam", "uparamcoord");
   AiNodeSetStr(shader, "vparam", "vparamcoord");

   // Visibility
   AiNodeSetInt(curve, "visibility", visibility);

   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Extra attributes
   if (customAttributes)
   {
      AiNodeSetBool(curve, "receive_shadows", receiveShadows);
      AiNodeSetBool(curve, "self_shadows", selfShadows);
      AiNodeSetBool(curve, "sss_use_gi", sssUseGI);
      AiNodeSetInt(curve, "sss_max_samples", sssMaxSamples);
      AiNodeSetFlt(curve, "sss_sample_spacing", sssSampleSpacing);
      AiNodeSetFlt(curve, "min_pixel_width", minPixelWidth);
      AiNodeSetInt(curve, "mode", mode);

      // User-Data attributes
      AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
      AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");
      AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
      AiNodeDeclare(curve, "next_line_starts", "constant ARRAY INT");
   }

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "num_points", curveNumPoints);
   AiNodeSetArray(curve, "points", curvePoints);
   AiNodeSetArray(curve, "radius", curveWidths);
   AiNodeSetArray(curve, "uparamcoord", curveUParamCoord);
   AiNodeSetArray(curve, "vparamcoord", curveVParamCoord);
   AiNodeSetArray(curve, "next_line_starts_interp", curveNextLineStartsInterp);
   AiNodeSetArray(curve, "next_line_starts", curveNextLineStarts);

   int curveNumPointsPerStep = AiArrayGetInt(curveNextLineStartsInterp, (m_numMainLines-1));
   // Process all hair lines
   for(AtUInt i = 0; i < m_numMainLines; i++)
   {
      MStatus status;
      MVectorArray line = m_hairLines[i].GetCurvePoints();
      int renderLineLength = line.length();

      // Ignore one or less cv curves
      if (renderLineLength > 1)
      {
         MDoubleArray widths = m_hairLines[i].GetCurveWidths();
         AtPoint curvePoint;

         int interpStartsIdx = i ? AiArrayGetInt(curveNextLineStartsInterp, i-1) : 0 ;
         int lineStartsIndex = i ? AiArrayGetInt(curveNextLineStarts, i-1) : 0 ;

         for (int j = 0; j < renderLineLength; j++)
         {
            // We need a couple extra points for interpolation
            // One at the beginning and one at the end (JUST POINTS, NO ATTRS)
            if (j == 0)
            {
               AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
               AiArraySetPnt(curvePoints, (j + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);
            }

            AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
            AiArraySetPnt(curvePoints, ((j+1) + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);

            // Animated widths are not supported, so just on step 0
            AiArraySetFlt(curveWidths, (j+lineStartsIndex), float(widths[j]));

            if (j == (renderLineLength-1))
            {
               AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
               AiArraySetPnt(curvePoints, ((j+2) + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);
            }
         }
      }
   }
}

void CHairTranslator::ExportMotion(AtNode *curve, AtUInt step)
{
   AtArray* curvePoints = AiNodeGetArray(curve, "points");
   AtArray* curveNextLineStartsInterp = AiNodeGetArray(curve, "next_line_starts_interp");

   int curveNumPointsPerStep = AiArrayGetInt(curveNextLineStartsInterp, (m_numMainLines-1));
   // Process all hair lines
   for(AtUInt i = 0; i < m_numMainLines; i++)
   {
      MStatus status;
      MVectorArray line = m_hairLines[i].GetCurvePoints();
      int renderLineLength = line.length();

      // Ignore one or less cv curves
      if (renderLineLength > 1)
      {
         MDoubleArray widths = m_hairLines[i].GetCurveWidths();
         AtPoint curvePoint;

         int interpStartsIdx = i ? AiArrayGetInt(curveNextLineStartsInterp, i-1) : 0 ;

         for (int j = 0; j < renderLineLength; j++)
         {
            // We need a couple extra points for interpolation
            // One at the beginning and one at the end (JUST POINTS , NO ATTRS)
            if (j == 0)
            {
               AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
               AiArraySetPnt(curvePoints, (j + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);
            }

            AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
            AiArraySetPnt(curvePoints, ((j+1) + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);

            if (j == (renderLineLength - 1))
            {
               AiV3Create(curvePoint, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
               AiArraySetPnt(curvePoints, ((j + 2) + interpStartsIdx + (step * curveNumPointsPerStep)), curvePoint);
            }
         }
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
         MVectorArray line = renderLine.getLine();
         MDoubleArray width = renderLine.getWidth();

         CHairLine hairLine;
         hairLine.SetCurvePoints(line);
         hairLine.SetCurveWidths(width);
         hairLines.push_back(hairLine);
      }

      // Check if motion blur is ON for hairs
      MFnDagNode fnDagNode(pfxHairPlug[0].node());

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

         MPlugArray nurbsHairPlug;
         MFnDependencyNode depNodeCurveHair(follicleHairPlug[0].node());
         depNodeCurveHair.findPlug("outCurve").connectedTo(nurbsHairPlug, false, true);

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

   return numMainLines;
}
